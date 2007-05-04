import ogre.renderer.OGRE as ogre
#from ogre.Math import *
import ogre.physics.OgreNewt as OgreNewt
Sin = ogre.Math.Sin
Cos = ogre.Math.Cos


# ------------------------ GAME ACTOR ----------------------------------------#
# - This class is the basis for all interactive or dynamic objects in the     #
# - game. To create actors you subclass this, and override functions and      #
# - properties.                                                               #
#                                                                             #
# - 
NUM_ACTORS = 0

X,Y,Z = 0,1,2

IDLE = 0

# class dealing with storage of nodes / entities / particles etc.
class mediaTree:
    def __init__(self, items, level='root'):
        self.item = self.node = self.liveResource = None
        self.tree = {}
        self.level = level
        self.isBuilt = False
        for item in items:
            if item['name'] == self.level:
                self.item = item
            if item['parent'] == self.level:
                self.tree[item['name']] = mediaTree(items, item['name'])
    
    def printTree(self, level='root', indent=1):
        if level == self.level:
            print '_' * indent, ':::', self.level
            print '_' * indent, self.item
            print '_' * indent, str(self.node), str(self.liveResource)
            print '_' * indent, 'Number of kids', str(len(self.tree))
            for b in self.tree:
                self.tree[b].printTree(b, indent+10)
        
    def find(self, name, item=None):
        if name == self.level:
            if not self.isBuilt:
                item = self.item
            else:
                item = self.node, self.liveResource
        else:
            for b in self.tree:
                item = self.tree[b].find( name, item )
        if item:
            return item
                
    def getKids(self, level, retList=[]):
        if level == self.level:
            retList += [self.tree[k].item for k in self.tree]
        else:
            [self.tree[k].getKids(level) for k in self.tree]
        return retList
                
    def build(self, sceneManager, actorName, parentNode):
        if not self.isBuilt:
            if self.item:
                print 'Building Item', self.item['name'], 'of type', self.item['rType'],
                print 'as child of', self.item['parent']
                name = actorName + self.item['name']
                self.prepResource(sceneManager, name)
                self.node = parentNode.createChildSceneNode()
                self.node.attachObject(self.liveResource)
                if 'offset' in self.item:
                    self.node.translate(ogre.Vector3(self.item['offset']))
            self.isBuilt = True
            for b in self.tree:
                self.tree[b].build( sceneManager, actorName, parentNode )

    def destroy(self, sceneManager):
        # destroy children first
        # recurse backwards up to the OgreNode
        for k in self.tree:
            self.tree[k].destroy( sceneManager )
        # Objects with no kids are deleted first
        if self.node and self.liveResource:
            print 'destroying sceneNode:', self.node.name, self.liveResource.name
            self.destroyResource( sceneManager )
            sceneManager.destroySceneNode(self.node.name)
            
    def hide(self, level='root'):
        if level == self.level:
            if self.node:
                self.node.setVisible(False)
                [ self.tree[k].hide( k ) for k in self.tree ]
        else: [ self.tree[k].hide(level) for k in self.tree ]
        
    def show(self, level='root'):
        if level == self.level:
            if self.node:
                self.node.setVisible(True)
                [ self.tree[k].hide( k ) for k in self.tree ]
        else: [ self.tree[k].hide(level) for k in self.tree ]
        


    def destroyResource(self, sceneManager):
        if self.liveResource:
            if self.item['rType'] == 'mesh':
                sceneManager.destroyEntity( self.liveResource.name )
            elif self.item['rType'] == 'particle':
                sceneManager.destroyParticleSystem( self.liveResource.name )
            elif self.item['rType'] == 'ribbon':
                sceneManager.destroyRibbonTrail( self.liveResource.name )

    def prepResource(self, sceneManager, name):
        if self.item['rType'] == 'mesh':
            self.liveResource = sceneManager.createEntity(name, self.item['rName'])
        elif self.item['rType'] == 'particle':
            self.liveResource = sceneManager.createParticleSystem(name, self.item['rName'])
        elif self.item['rType'] == 'billboard':
            self.liveResource = 'Billboard'
        elif self.item['rType'] == 'actor':
            self.liveResource = 'Actor'
        
        
class GameActor:
    def __init__(self):
        
        # GENERAL
        self.life = 100 # how many hp?
        self.price = 0 # cost if bought/sold
        self.id = -1 # generic id						
        self.position = (100.0, 0.0, 0.0) # changed at spawn
        self.moving = False # object in motion?
        self.heightAdjust = 0.0 # how tall is it? (object height / 2)
        self.adjustHeight = True # do i need to stick to the ground?
        self.name = "actor" # my name
        self.media = {'name':'Sphere', 'parent':'root', 'rType':'mesh', 'rName':'sphere.mesh'}
        self.material = "Examples/RustySteel" # override again
        
        # MOVEMENT, HEIGHT ADJUST
        self.hAdjustTime = 5 # update this objects height every x frames
        self.hAdjustClock = 0 # counter for height updates
        self.direction = ogre.Vector3(0.0, 0.0, 1.0)		# X, Y, Z
        self.targetDirection = ogre.Vector3(1.0, 0.0, 0.0)		# X, Y, Z
        self.defaultDirection = ogre.Vector3(1.0, 0.0, 0.0)		# X, Y, Z
        self.rotationSpeed = 2.0 # speed of rotation (if not physics)
        self.nodeScale = ogre.Vector3(1.0,1.0,1.0) # scale of model
        self.mass = 100.00
        self.speed = 1.0 # speed of movement may need to be increased for physics
        self.maxSpeed = 0.0 # my max velocity (if not physics)
        
        # AI AND STATES
        self.calculateTargetEachFrame = False # am i thinking fast?
        self.target = None # actor i'm currently interested in
        self.raySceneQuery = None # passed at spawn
        self.ray = ogre.Ray() # my own ray to use in the query
        self.Alive = False # I have not been spawned yet
        self.isAnimated = False # override for animated meshes
        self.hasAI = False # can i think?
        self.updateAIFrequency = 1.0
        self.updateAITime = 1.0
        self.state = IDLE # generic mode flag
        self.toDelete = False # tell the datamanager when i need to die
        
        # ANIMATIONS
        self.animations = [] # my animations
        self.animPlaying = "" # name of the current animation
        self.animSpeed = 1.0
        self.nextAnim = "" # the next anim to switch to after previous has done
        self.animTime = 0 # counter for animation playback
        self.curAnimationStates = [] # animation states for my objects
        self.loopAnim = True # does the animation loop?
        
        # PHYSICS
        self.bodies = {} # hold a dict of all physics bodies attached to me
        self.hasPhysics = False # Enable physics?
        self.torque = 0.0 # forward motion
        self.steering = 0.0 # rotation direction
        
        # -- Old stuff from my previous collision code.
        #self.incontact = []
        #self.isColliding = False
        #self.collisionsEnabled = False 
        #self.radius = 50
    
    def SetTarget(self, gameActor):
        # pass another actor as a new target
        # and find out what direction it lies in
        self.target = gameActor
        self.calculateTargetDirection()
        
    def Spawn(self, position, SceneManager, rootNode, numActors, RaySceneQuery, updateResolution, world = None):
        # -- This function creates our object in the game
        # -- it's called after __init__ has finished, so you can
        # -- setup everything prior to the model appearing.
        # -- it also calls another post-setup routine after this one.
        # -- Again, use the post-setup to acess things like the node setup
        
        # try to keep our name unique by appending the number of actors
        self.name = str(self.name) + str(numActors)
        # get a reference to the root scenenode
        rootnode = rootNode ##SceneManager.getRootSceneNode()
        # get the engine's AI tick speed
        self.updateResolution = updateResolution
        # get a reference to the main sceneQuery object
        self.raySceneQuery = RaySceneQuery
        # get a reference to the physics world
##        self.world = None
##        if world:
##            self.world = world
##            print dir(self.world)
##        # create an ogre entity and load our mesh
##        self.Entity = SceneManager.createEntity(self.name, self.mesh)
        # create a parent or 'main' node
        self.OgreNode = rootnode.createChildSceneNode(self.name)
        # create and build the media tree
        self.MediaTree = mediaTree(self.media)
        self.MediaTree.build( SceneManager, self.name, self.OgreNode )
##        # create a child-node
##        self.Node = self.OgreNode.createChildSceneNode("o_" + self.name)
##        # attach the entity to the sub-node
##        self.Node.attachObject(self.Entity)
        # set our position as a tuple, for easier use in python
        self.position = position
        # set the ogrenode position to match, this will move the child as well
        self.OgreNode.setPosition(ogre.Vector3(self.position))
        #scale
        self.OgreNode.setScale(self.nodeScale)
        # we are now alive ..
        self.Alive = True
##        # start animating
##        if self.isAnimated:
##            self.curAnimationState = self.Entity.getAnimationState(self.animations[0])
##            self.curAnimationState.enabled = True
        # adjust height to ground if needed
        if self.adjustHeight:
            self.updateHeight()
        #Setup any post-spawn options specific to a character
        self.setActorOptions(world)
        # setup the ogre node's rest state
##        self.Node.setInitialState()
##        self.Node.resetOrientation()

##    def __del__(self, sceneManager:
##        self.MediaTree.destroy()

    def setActorOptions(self, world):
        #Override this function in sub-classes
        # to setup any post-spawn options 
        # ie: options that need access to the entity / node
        # Note: Now, you setup animations here as well
        pass

        
    def connectPhysics(self, world):
        # -- This part is ripped wholesale from the python-ogre demos

        # create collision body
        col = OgreNewt.Cylinder(world, 2.5, 5)
        # now we make a new rigid body based on this collision shape.
        body = OgreNewt.Body( world, col )
        # we`re done with the collision shape, we can delete it now.
        del col
        # now we "attach" the rigid body to the scene node that holds the visual object, and set it's
        # original position and orientation.  all rigid bodies default to mass=0 (static, immobile), so
        # that's all we'll need to do for this object.  dynamic objects have a few more steps, so look
        # at the code in the FrameListener for more.
        body.attachToNode( self.OgreNode )
        body.setPositionOrientation( self.OgreNode.getPosition(), Ogre.Quaternion.IDENTITY )
        self.body = body
        # set a flag for the datamanager
        self.hasPhysics = True
        
    def getFloorDistance(self, direction=ogre.Vector3.NEGATIVE_UNIT_Y):
        # -- Function for non-physics objects over landscapes
        # -- Fires from *under* the floor upwards. therefore if the object is on the floor:
        # -- self.position[y] - floorDistance - self.heightAdjust = 0
        # -- otherwise, the offset can be calculated easily from the distance returned.
        dist = 0.0
        # setup our ray object
        self.ray.origin = ogre.Vector3(self.position[X], 0, self.position[Z])
        self.ray.direction = ogre.Vector3.UNIT_Y
        # attach our adjusted ray to the sceneQuery
        self.raySceneQuery.ray = self.ray
        # find the distance to the nearest world fragment.
        for queryResult in self.raySceneQuery.execute():
            if queryResult.worldFragment is not None or 'land' in queryResult.movable.name.lower():
                dist = queryResult.distance
        return dist
    
    def takeDamage(self, amount):
        # damage is dealt by the datamanager
        if self.Alive:
            self.life -= amount
            if self.life < 0.00:
                self.Die()
            
    def Die(self):
        # -- This function is where you can set any animations
        # -- or spawn other objects when this one dies
        # -- by default, it just causes the object to be
        # -- removed from the scene.
        self.Remove()

    
    def Update(self, actors, player, updateAITime, time):
        # -- Again, this is designed to be subclassed.
        # -- datamanager checks this objects think time
        # -- and calls this handler when its time to think.
        pass
        
    def animate(self, time):
        if self.isAnimated and len(self.curAnimationStates) > 0:
            for state in self.curAnimationStates:
                state.addTime(time * self.animSpeed)
            
    def move(self, time): # sublcass this
        pass
        

    def Remove(self):
        # stops animations, and sets flag for datamanager to delete me
        self.IsAnimated = False
        self.Alive = False
        self.moving = False
        self.toDelete = True
        
    def updateHeight(self):
        # -- This function makes objects stick to the floor as they move
        # -- This is throttled back by the hAdjustTime property, because
        # -- it uses a lot of cpu time. For slow moving objects the time
        # -- can be increased, resulting in slightly better performance
        if not self.hasPhysics:
            if not self.adjustHeight:
                if self.hAdjustClock == self.hAdjustTime:
                    self.hAdjustClock = 0
                    self.adjustHeight = True
                else:
                    self.hAdjustClock += 1
                return
            
            floorDist = self.getFloorDistance()
            
            
            # if we are already very close to the floor, we dont need to continue
            if (abs(self.position[Y] - floorDist - self.heightAdjust)) < 0.01:
                # this will cause a delay between the next height check
                self.adjustHeight = False
            else:
                # move the object so it sits on the floor
                self.position = (	self.position[X], 
                                    floorDist + self.heightAdjust, 
                                    self.position[Z])
                self.OgreNode.setPosition(ogre.Vector3(self.position))

    def rotate(self, fTime, targetDirection = None):
        if not self.hasPhysics:
            if self.calculateTargetEachFrame and targetDirection is None:
                self.calculateTargetDirection()
            if (self.targetDirection.x *-1 - self.direction.x,
                self.targetDirection.y *-1 - self.direction.y,
                self.targetDirection.z *-1 - self.direction.z) == (0.0, 0.0, 0.0):
                self.direction = self.targetDirection
                print "ON Target"
            else:
                dir = self.v3normalize(self.direction + (self.targetDirection - self.direction)*fTime*self.rotationSpeed)
                self.direction = dir
                self.OgreNode.setDirection(self.direction, self.OgreNode.TS_WORLD)
            #self.ogreNode.rotate(ogre.Vector3.UNIT_Y, self.offsetRotation)
        else:
            # need to find the direction to rotate in, in radians.
            # neg = turn left pos = turn right I think.
            # AngleBetweenVectors is the func we need
            # First, get the actual direction of our Newton Body as a vector
            p, q = self.torsoBody.getPositionOrientation()
            direction = q * ogre.Vector3(0.0, 0.0, 1.0)
            
            
    def calculateTargetDirection(self):
        if self.target is not None:
            newpos = self.target.OgreNode.position
            td = newpos - self.OgreNode.position
            td.y = 0.0
            self.targetDirection = td / td.length()
            return True
        else:
            self.targetDirection = self.defaultDirection
            return False
        
    def makeWalkingPhysics(self, world, entity):
        # make a walking character physics setup
        self.torque = 0.0 # forward motion
        self.steering = 0.0 # rotation direction on X,Z plane
        entity.setNormaliseNormals(True)
        aab = entity.boundingBox
        min = aab.getMinimum() * self.nodeScale
        max = aab.getMaximum() * self.nodeScale
        center = aab.getCenter() * self.nodeScale
        size = ogre.Vector3( abs(max.x-min.x), abs(max.y-min.y), abs(max.z-min.z) )
        if size.x < size.z:
            radius = size.z / 4.0
        else:
            radius = size.x / 4.0
        # feet
        feetSphere = OgreNewt.Ellipsoid(world, ogre.Vector3(radius, radius, radius))
        feetInertia = OgreNewt.CalcSphereSolid(self.mass, radius)
        feetBody = OgreNewt.Body(world, feetSphere)
        feetBody.setMassMatrix(10.0, feetInertia)
        
        feetLoc = self.OgreNode.position + ogre.Vector3(0, -size.y /2, 0)
        feetBody.setPositionOrientation( feetLoc, ogre.Quaternion.IDENTITY )
        feetBody.attachToNode(self.OgreNode)
        feetBody.setStandardForceCallback()
        self.bodies['feetBody'] = feetBody
        del feetSphere
        # torso
        torsoCapsule = OgreNewt.Capsule(world, radius, size.y)
        torsoInertia = OgreNewt.CalcCylinderSolid(self.mass, radius, size.y - radius)
        torsoBody = OgreNewt.Body(world, torsoCapsule)
        torsoBody.setMassMatrix(self.mass, torsoInertia)
        torsoBody.setPositionOrientation( self.OgreNode.position, ogre.Quaternion.IDENTITY )
        torsoBody.attachToNode(self.OgreNode)
        torsoBody.setStandardForceCallback()
        self.bodies['torsoBody'] = torsoBody
        # connect them, body is child of feet
        print world, torsoBody, feetBody
        joint = OgreNewt.Hinge(world, torsoBody, feetBody, center-ogre.Vector3(size.x/2,0,0), ogre.Vector3.UNIT_X)
        self.bodies['joint'] = joint
        feetBody.setJointRecursiveCollision(0)
        # -- Here, we set the userdata of the body to be the actor
        # -- Then we set a callback to the physics function at the
        # -- very top of this file.
        feetBody.setUserData( self )
        feetBody.physCallback = self.actorPhysCallback
        feetBody.setCustomForceAndTorqueCallback(feetBody, 'physCallback' )
        
        
    def physWalkForward(self):
        p, q = self.torsoBody.getPositionOrientation()
        self.torque = (q * ogre.Vector3( self.speed * ogre.Math.Cos(1), 0, self.speed * ogre.Math.Sin(1) ))
        
    def physWalkRotate(self, boolLR):
        if boolLR:
            self.steering -= self.rotationSpeed
        else:
            self.steering += self.rotationSpeed

        
    def physWalkStop(self):
        self.torque = ogre.Vector3(0,0,0)
        
    def actorPhysCallback( self, body ):
        actor = body.getUserData()
        torque = actor.torque
        steerDir = actor.steering
        if actor.bodies.has_key('torsoBody'):
            # -- this is a walking character
            # -- this callback is linked to the feet, so
            # -- body = character's feet. We roll the sphere
            # -- along the ground to move the character, by applying torque.
            # rotate to the right direction
            torso = actor.bodies['torsoBody']
            q1, p1 = torso.getPositionOrientation()
            q2 = ogre.Quaternion(ogre.Degree(self.steering),ogre.Vector3.UNIT_Y)
            #rotatedTorque = (q1*q2) * actor.torque
            #body.setTorque(rotatedTorque) 
            body.setTorque(q1 * ogre.Vector3( actor.speed * Cos(1), 0, actor.speed * Sin(1) ))
            
        else:
            # another type of physics object
            pass
        
    def makeSphereCollision(self, world):
        
        col = OgreNewt.Ellipsoid(world, ogre.Vector3(2,2,2))
        
        body = OgreNewt.Body( world, col)

        del col

        inertia = OgreNewt.CalcSphereSolid( 10.0, 1.0 )
        body.setMassMatrix( 10.0, inertia )

        ## attach to the scene node.
        body.attachToNode( self.OgreNode )
        body.setUserData( self )

        ## this is a standard callback that simply add a gravitational force (-9.8*mass) to the body.
        body.setStandardForceCallback()

        ## set the initial orientation and velocity!
        body.setPositionOrientation( self.OgreNode.position, self.OgreNode.orientation )
        body.setVelocity( ogre.Vector3(0, 4.00, 0) )
        self.bodies['ballbody'] = body
        
        ## note that we have to keep the bodies around :)
        
    def delBodies(self):
        del self.bodies
        
        
    
        
        
        
        
        
        
        
        
        

##class StaticObject(GameActor):
##    def __init__(self):
##        GameActor.__init__(self)
##        self.animationSpeed = 1.0
##        self.animationState = None
##    
##    
##class FlyingObject(GameActor):
##    def __init__(self):
##        GameActor.__init__(self)
##        self.animationSpeed = 1.0
##        self.animationState = None
##        self.state = None
##        
##    def move(self, time, colpairs):
##        delta = time * self.speed # * self.updateResolution
##        desired = self.direction * delta
##        self.OgreNode.setPosition(self.OgreNode.position + desired)
##        self.position = (self.OgreNode.position.x, self.OgreNode.position.y, self.OgreNode.position.z)
##    
##class RunningObject(GameActor):
##    def __init__(self):
##        GameActor.__init__(self)
##        self.animationSpeed = 1.0
##        self.animationState = None
##        self.hasAI = True
##        self.state = None
##        
##    def move(self, time, colpairs):
##        delta = time * self.speed # * self.updateResolution
##        if colpairs is not None:
##            desired = self.resolveCollisions(colpairs, delta) # returns a desired move cut off at target
##            # DEBUG ::::
##            if self.isColliding:
##                self.Node.showBoundingBox = True
##            else:
##                self.Node.showBoundingBox = False
##        else:
##            desired = self.direction * delta
##        self.OgreNode.setPosition(self.OgreNode.position + desired)
##        self.position = (self.OgreNode.position.x, self.OgreNode.position.y, self.OgreNode.position.z)
        
        

