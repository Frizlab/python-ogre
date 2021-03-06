#-----------------------------------------------------------------------------#
#                                                                             #
#   This source code is part of the python-ogre techdemo project.             #
#                                                                             #
#   This program is released as public domain                                 #
#                                                                             #
#-----------------------------------------------------------------------------#
#   
#   TITLE: base_actor
#   DESCRIPTION: Root Actor definition
#   AUTHOR: Ben Harling


import ogre.renderer.OGRE as ogre
import logging
#from ogre.Math import *
import ogre.physics.OgreNewt as OgreNewt
Sin = ogre.Math.Sin
Cos = ogre.Math.Cos

NUM_ACTORS = 0

X,Y,Z = 0,1,2

IDLE = 0

# ------------------------ EVENT CLASS ---------------------------------------#
# - A generic event, written and read by all entities in the game 
# - ie: actors, framelisteners, managers
class Event:
    def __init__(self, time, name, function, *args):
        self.time = time
        self.name = name
        self.function = function
        self.args = args
    def __cmp__(self, other):
        return cmp(self.time, other.time)
    def __repr__(self):
        return ('Event: name=' + self.name + ' time=' + str(self.time) + 'function=' +  str(self.function))
    

# ------------------------ MEDIATREE -----------------------------------------#
# - A helper class to deal with spawning / deleting of an actor's 
# - different media. It is a tree data structure, which mimics the
# - parent to child relationship of ogre nodes / entities
class mediaTree:
    def __init__(self, items, level='root'):
        '''Create a tree from a list of items'''
        self.item = self.node = self.liveResource = None
        self.tree = {}
        self.level = level
        self.isBuilt = False
        for item in items:
            #logging.debug(str(item))
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
        '''Find an item by name'''
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
        '''Get all children of a given level'''
        if level == self.level:
            retList += [self.tree[k].item for k in self.tree]
        else:
            [self.tree[k].getKids(level) for k in self.tree]
        return retList
                
    def build(self, sceneManager, actorName, parentNode):
        '''Create the media in the sceneManager, the starting point
        for attaching this actor is parentNode'''
        if not self.isBuilt:
            if self.item:
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
        '''Remove media from the sceneManager'''
        # destroy children first
        # recurse backwards up to the OgreNode
        for k in self.tree:
            self.tree[k].destroy( sceneManager )
        # Objects with no kids are deleted first
        if self.node and self.liveResource:
            logging.debug('destroying sceneNode:' + self.node.name + self.liveResource.name)
            self.destroyResource( sceneManager )
            sceneManager.destroySceneNode(self.node.name)
            
    def hide(self, level='root'):
        '''Hide all objects in-game that are children of <level>'''
        if level == self.level:
            if self.node:
                self.node.setVisible(False)
                [ self.tree[k].hide( k ) for k in self.tree ]
        else: [ self.tree[k].hide(level) for k in self.tree ]
        
    def show(self, level='root'):
        '''Show in-game objects that are children of <level>'''
        if level == self.level:
            if self.node:
                self.node.setVisible(True)
                [ self.tree[k].hide( k ) for k in self.tree ]
        else: [ self.tree[k].hide(level) for k in self.tree ]
        
        
    def destroyResource(self, sceneManager):
        '''Destroy the resource specifically at this level in the tree'''
        if self.liveResource:
            type = self.item['rType']
            if type == 'mesh':
                sceneManager.destroyEntity( self.liveResource.name )
            elif type == 'particle':
                sceneManager.destroyParticleSystem( self.liveResource.name )
            elif type == 'light':
                sceneManager.destroyLight( self.liveResource.name )
            elif type == 'ribbon':
                sceneManager.destroyRibbonTrail( self.liveResource.name )

    def prepResource(self, sceneManager, name):
        '''Prepare and create the resource at this specific level in the tree'''
        if self.item['rType'] == 'mesh':
            self.liveResource = sceneManager.createEntity(name + '_Entity', self.item['rName'])
        elif self.item['rType'] == 'particle':
            self.liveResource = sceneManager.createParticleSystem(name, self.item['rName'])
        elif self.item['rType'] == 'billboard':
            self.liveResource = sceneManager.createBillboardSet(name + '_Billboard', 1)
            self.liveResource.createBillboard(0,0,0, ogre.ColourValue(0.5,0.5,0.5))
            self.liveResource.MaterialName=self.item['rName']
            # do the rest of the billboard setup in setActorOptions()
        elif self.item['rType'] == 'light':
            self.liveResource = sceneManager.createLight(name)
            # Again, do the light setup in setActorOptions
        elif self.item['rType'] == 'actor':
            self.liveResource = 'Actor'
            
##        bbs = sceneManager.createBillboardSet("bb", 1)
##        bbs.createBillboard(ogre.Vector3.ZERO, trail.getInitialColour(0))
##        bbs.MaterialName="Examples/Flare"
##        animNode.attachObject(bbs)
        
# ------------------------ GAME ACTOR ----------------------------------------#
# - This class is the basis for all interactive or dynamic objects in the     #
# - game. To create actors you subclass this, and override functions and      #
# - properties.                                                               #
#                                                                             #
class GameActor:
    def __init__(self):
        '''Initialise a basic game actor with a selection of properties
        sub-class this to begin creating actors'''
        
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
        self.worldEventRegister = {'testWorldEvent':self.testWorldEvent} # A list of functions to expose to world events
        self.actorEventRegister = {} # A list of functions to expose to actor events
        self.events  = [] # events that I send out
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
        self.body = None
        self.bodies = {} # hold a dict of all physics bodies attached to me
        self.hasPhysics = False # Enable physics?
        self.torque = 0.0 # forward motion
        self.steering = 0.0 # rotation direction
        
    def __del__(self):
        '''Cleanup this actor'''
        del self.body
        while self.bodies:
            b= self.bodies.pop()
            del b
        
    def testWorldEvent(self, *args):
        '''DEBUG: Recieving world events'''
        retString = 'testWorldEvent Recieved' + str(args)
        logging.debug(retString)
        self.events.append( Event(0, 'actorRecievedEvent', 'flTestActorEvent', None) )
        
    def SetTarget(self, gameActor):
        '''Set the object of attention for this actor'''
        # pass another actor as a new target
        # and find out what direction it lies in
        self.target = gameActor
        self.calculateTargetDirection()
        
    def Spawn(self, position, SceneManager, rootnode, numActors, RaySceneQuery, updateResolution, world = None):
        '''This function creates our object in the game
        it's called after __init__ has finished, so you can
        setup everything prior to the model appearing.
        it also calls another post-setup routine after this one.
        use the setActorOptions to acess the mesh/nodes etc after
        spawning. Only on rare occasions should you need to actually
        redefine this method in a subclassed actor.'''
        
        # try to keep our name unique by appending the number of actors
        self.name = str(self.name) + str(numActors)
        # get the engine's AI tick speed
        self.updateResolution = updateResolution
        # get a reference to the main sceneQuery object
        self.raySceneQuery = RaySceneQuery
        # get a reference to the physics world
        # create a parent or 'main' node
        self.OgreNode = rootnode.createChildSceneNode(self.name)
        # create and build the media tree
        self.MediaTree = self.mediaTree = mediaTree(self.media)
        self.MediaTree.build( SceneManager, self.name, self.OgreNode )
        # set our position as a tuple, for easier use in python
        self.position = position
        # set the ogrenode position to match, this will move the child as well
        self.OgreNode.setPosition(ogre.Vector3(self.position))
        #scale
        self.OgreNode.setScale(self.nodeScale)
        # we are now alive ..
        self.Alive = True
        # adjust height to ground if needed
        if self.adjustHeight:
            self.updateHeight(world)
        #Setup any post-spawn options specific to a character
        self.setActorOptions(world)
        # setup the ogre node's rest state
        #self.Node.setInitialState()

    def setActorOptions(self, world):
        '''Override this function in sub-classes
        to setup any post-spawn options 
        ie: options that need access to the entity / node
        Note: Now, you setup animations here as well'''
        pass
        
##    def connectPhysics(self, world):
##        # -- This part is ripped wholesale from the python-ogre demos
##
##        # create collision body
##        col = OgreNewt.Cylinder(world, 2.5, 5)
##        # now we make a new rigid body based on this collision shape.
##        body = OgreNewt.Body( world, col )
##        # we`re done with the collision shape, we can delete it now.
##        del col
##        # now we "attach" the rigid body to the scene node that holds the visual object, and set it's
##        # original position and orientation.  all rigid bodies default to mass=0 (static, immobile), so
##        # that's all we'll need to do for this object.  dynamic objects have a few more steps, so look
##        # at the code in the FrameListener for more.
##        body.attachToNode( self.OgreNode )
##        body.setPositionOrientation( self.OgreNode.getPosition(), Ogre.Quaternion.IDENTITY )
##        self.body = body
##        # set a flag for the datamanager
##        self.hasPhysics = True
        
    # Raycasts are queries into the physics world    
    # These handlers can be called from the actor's Update method
    # this is because they need a reference to the physics world
    def doDistanceRayCast(self, world, start, end):
        '''Do a rayCast to detect distance to the nearest physics body
        in the direction (start -> end)'''
        q = OgreNewt.BasicRaycast(world, start, end)
        info = q.getFirstHit()
        if info:
            if not info.mDistance == -1.0:
                off = end - start
                dist = off.length() * info.mDistance
                return dist
            else:
                return None
        
            
    def doDetailedRayCast(self, world, start, end, numResults):
        '''Do a detailed physics raycast, returning as much information as possible'''
        q = OgreNewt.BasicRaycast(world, start, end)
        info = q.getFirstHit()
        if info:
            if not info.mDistance == -1.0:
                off = end - start
                dist = off.length() * info.mDistance
                return dist
            
        
    def getFloorDistance(self, world, direction=ogre.Vector3.NEGATIVE_UNIT_Y):
        '''Get y distance between actor and nearest physics body, assumes the
        actor is above the floor'''
        start = self.OgreNode.position
        end = ogre.Vector3(self.OgreNode.position.x,
                            self.OgreNode.position.y -100,
                            self.OgreNode.position.z)
                            
        return self.doDistanceRayCast(world, start, end)
    
    def takeDamage(self, amount):
        '''damage is dealt by the datamanager, which calls this method'''
        if self.Alive:
            self.life -= amount
            if self.life < 0.00:
                self.Die()
            
    def Die(self):
        '''This function is where you can set any animations
        or spawn other objects when this one dies
        by default, it just causes the object to be
        removed from the scene.'''
        self.Remove()

    
    def Update(self, actors, player, updateAITime, world, time):
        '''Again, this is designed to be subclassed.
        datamanager checks this objects think time
        and calls this handler when its time to think.'''
        pass
        
    def animate(self, time):
        '''Animate all animating entities in the media tree'''
        if self.isAnimated and len(self.curAnimationStates) > 0:
            for state in self.curAnimationStates:
                state.addTime(time * self.animSpeed)
            
    def move(self, time):
        '''Move the actor, - subclass this to define actor movement'''
        pass
        

    def Remove(self):
        '''Stops animations, and sets flag for datamanager to delete me'''
        self.IsAnimated = False
        self.Alive = False
        self.moving = False
        self.toDelete = True
        
    def updateHeight(self, world):
        '''This function makes objects stick to the floor as they move
        This is throttled back by the hAdjustTime property, because
        it uses a lot of cpu time. For slow moving objects the time
        can be increased, resulting in slightly better performance'''
        if not self.hasPhysics:
            if not self.adjustHeight:
                if self.hAdjustClock == self.hAdjustTime:
                    self.hAdjustClock = 0
                    self.adjustHeight = True
                else:
                    self.hAdjustClock += 1
                return
            
            floorDist = self.getFloorDistance(world)
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
        '''Rotate this actor in the horizontal plane, towards its target direction'''
        if not self.hasPhysics:
            if self.calculateTargetEachFrame and targetDirection is None:
                self.calculateTargetDirection()
            if (self.targetDirection.x *-1 - self.direction.x,
                self.targetDirection.y *-1 - self.direction.y,
                self.targetDirection.z *-1 - self.direction.z) == (0.0, 0.0, 0.0):
                self.direction = self.targetDirection
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
        '''Calculate the direction to this actors target'''
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
        '''NOT QUITE WORKING YET:
        Will make a capsule-and-ball physics setup for emulating a walking
        humanoid character'''
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
        '''Callback for OgreNewt'''
        p, q = self.torsoBody.getPositionOrientation()
        self.torque = (q * ogre.Vector3( self.speed * ogre.Math.Cos(1), 0, self.speed * ogre.Math.Sin(1) ))
        
    def physWalkRotate(self, boolLR):
        '''Callback for OgreNewt'''
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
        '''Setup a simple rolling sphere for this actors collision system'''
        
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
        
        

