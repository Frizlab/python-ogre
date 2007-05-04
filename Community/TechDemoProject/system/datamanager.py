import os, sys
import ogre.renderer.OGRE as ogre
import ogre.io.OIS as OIS
import ogre.physics.OgreNewt as OgreNewt
from dotscene import DotScene
from actors.characters import people
from frameListeners import debugFrameListener
import sys

frameListeners = {'debugFrameListener':debugFrameListener.DebugFrameListener}

actorDict = {}

sys.path.append('.\gameConfig')



class DataManager:
    def __init__(self, sceneManager, viewport, window, camera, framerate=60, physics=False):
        print 'DataManager Starting'
        self.sceneManager = sceneManager
        rt = self.sceneManager.getRootSceneNode()
        self.shadowType = ogre.SHADOWTYPE_STENCIL_ADDITIVE
        self.sceneManager.shadowTechnique = self.shadowType
        self.viewport = viewport
        self.renderWindow = window
        self.camera = camera
        self.numActors = 0000
        ray = ogre.Ray(ogre.Vector3(10.0,0.0,10.0), ogre.Vector3.NEGATIVE_UNIT_Y)
        self.raySceneQuery = self.sceneManager.createRayQuery(ray)
        self.cacheActors()
        self.actors = []
        self.objectsToDestroy = []
        self.animationStates = []
        self.animationSpeeds = []
        self.inputListener = None # The framelistener for receiving player input
        self.updateAITime = 0.0
        self.totalTime = 0.0
        self.updateResolution = 1 # Spread actor updates over frames by dividing by this number
        self.collisionList = []
        self.framerate = framerate # fps rate for physics updates
        self.staticRoot = rt.createChildSceneNode('Root_Static')
        self.actorRoot = rt.createChildSceneNode('Root_Actors')
        #self.StaticEntity = self.sceneManager.createEntity('Static', 'ellipsoid.mesh')
        #self.staticRoot.attachObject(self.StaticEntity)
        self.staticCollisionBody = None
        print 'Datamanager get world'
        if physics:
            self.world = OgreNewt.World()
            self.treeColParser = OgreNewt.TreeCollisionSceneParser( self.world ) # note - this IS a treecollision itself
            self.treeColLoader = OgreNewt.TreeCollisionSerializer()
            print 'world and loader / parser initialized'
        self.elapsed = 0.0 # used for physics update synching
        self.physTick = (1.0 / self.framerate)
        self.player = None
        self.defaultScene = 'testLevel.cfg'
        self.firstRun = True
        
        print 'DataManager Initialized ::'
        
    def __del__(self):
        # The Final delete action for dead objects    
        while self.objectsToDestroy:
            ob = self.objectsToDestroy.pop()
            print 'Destroying:', ob.name
##            self.sceneManager.destroySceneNode(ob.Node.name)
##            self.sceneManager.destroySceneNode(ob.OgreNode.name)
##            self.sceneManager.destroyEntity(ob.Entity)
            del ob
                # The Final delete action for dead objects    
        while self.actors:
            ob = self.actors.pop()
            print 'Destroying:', ob.name
            ob.MediaTree.destroy( self.sceneManager )
            self.sceneManager.destroySceneNode( ob.OgreNode.name )
            del ob
        print 'bodies'
        del self.sceneManager
        print 'sm'
        del self.viewport
        print 'vp'
        del self.renderWindow
        print 'rw'
        del self.camera
        print 'cam'
        del self.inputListener
        print 'fl'
        del self.staticCollisionBody
        print 'stat_col'
        #del self.treeColParser
        print 'parser'
        del self.world
        
    def cacheActors(self):
        pKeys = people.__dict__.keys()
        for k in pKeys:
            if '__' not in k:
                if not k in ['ogre', 'base_actor']:
                    actorDict[k] = people.__dict__[k]
            else:
                break
        
        # I couldn't get it to find my actor, what's up with that?    
        # For now, I'm manually adding it here.  
        actorDict["Bird"] = people.Bird
            
        print actorDict
                    
        
        
        
    
    def loadScene(self, scriptName):
        # load up a scene from a script. the script may load
        # an xml scene and moving models, or just one of those.
        # check out XXXXXXXXXX to see the script.
        # dirty script parsing begins here:
        print 'Loading Scene:', scriptName
        settings = []
        f = open(os.path.join(os.curdir ,'system', 'gameConfig', scriptName), 'r')
        script = f.readlines()
        for l in script:
            if '=' in l:
                key, value = l.split('=')
                settings.append([ key.strip(), value.strip() ])
        for ky in settings:
            k = ky[0]
            # parse the settings and load up stuff
            if k == 'RESOURCE_GROUP':
                ogre.ResourceGroupManager.getSingleton().initialiseResourceGroup(ky[1])
            if k == 'SKYBOX':
                self.sceneManager.setSkyBox(True, ky[1])
            if k == 'BASE_LAND_MESH':
                floor = self.sceneManager.createEntity("Landscape", ky[1] )
                floor.setNormaliseNormals(True)
                self.staticRoot.attachObject(floor)
                # DEBUG CODE HERE !!!!!
                self.staticRoot.pitch(ogre.Degree(-90))
                self.staticRoot.setScale(ogre.Vector3(2.5,2.5,2.5))
                #self.staticRoot.setPosition(ogre.Vector3(0,-10,0))
                # END DEBUG CODE
            if k == 'XML_SCENE':
                sceneFile = os.path.join(os.curdir, 'system', 'gameConfig', ky[1])
                self.scnLoader = DotScene( sceneFile, self.sceneManager, self.staticRoot)
            if k == 'XML_SCENE_PHYSICS':
                # We'll cache the collision trees,
                try:
                    # import a pre-cached collision scene
                    # TODO: MD5 checking to match collision to scene file
                    # incase one needs to be overwritten
                    f = open(ky[1], 'r')
                    tree = ogre.FileHandleDataStream(f)
                    self.treeColLoader.importTreeCollision( tree, self.treeColParser )
                    print 'Tree Collision imported........'
                    f.close()
                except:
                    # if we've already loaded a normal scene
                    # try generating the collision tree from it
                    # then save it if successful.
                    if hasattr(self, 'scnLoader'):
                        
                        stat_col = OgreNewt.TreeCollisionSceneParser( self.world )
                        stat_col.parseScene( self.staticRoot, True )
                        self.staticCollisionBody = OgreNewt.Body( self.world, stat_col )
                        self.staticCollisionBody.attachToNode( self.staticRoot )
                        #self.staticCollisionBody.setPositionOrientation( self.staticRoot.getPosition(), self.staticRoot.getOrientation() )
##                        stat_col = self.treeColParser.parseScene(self.staticRoot, False)
##                        stat_col.attachToNode(self.staticRoot)
                        self.treeColLoader.exportTreeCollision(stat_col, ky[1])
                        del stat_col
                        print 'Tree Collision Created and Exported'
                    else:
                        # nothing we can do if no scene has been loaded
                        # the order must be right in the config file
                        print 'No way to make Tree Collision'
                        pass
            if k == 'ACTOR': # 2 properties: ky[1]:actorName, ky[2]:position
                actor, pos = ky[1].split('@')
                x,y,z = pos.strip().split(',')
                pos = (float(x), float(y), float(z))
                self.spawnActor(actor.strip(), pos)
            if k == 'FRAME_LISTENER':
                fl = frameListeners[ky[1]]
                self.inputListener = fl(self.renderWindow, self.camera)
                ogre.Root.getSingleton().addFrameListener(self.inputListener)
            if k == 'COMPOSITOR':
                comp = ky[1]
                print 'ADDING COMPOSITOR'
                cms = ogre.CompositorManager.getSingleton()
                if not cms.hasCompositorChain(self.renderWindow.getViewport(0)):
                    newcomp = cms.addCompositor(self.renderWindow.getViewport(0), comp)
                    cms.setCompositorEnabled(self.renderWindow.getViewport(0), comp, True)
                else:
                    cms.setCompositorEnabled(self.renderWindow.getViewport(0),comp, False)
        OgreNewt.Debugger.getSingleton().init(self.sceneManager)
        OgreNewt.Debugger.getSingleton().showLines(self.world)
                        
                        
    def clearScene(self):
        # clear the current scene and models
        pass
    
    def getSceneLoaded(self):
        # return the name of the scene, or none.
        pass
        
    def getActiveObjects(self):
        # gets lists of all animating and/or moving objects
        animationStates = []
        animationSpeeds = []
        movingObjects = []
        for actor in self.actors:
            if actor.isAnimated:
                try:
                    x = actor.Entity.getAnimationState(actor.animPlaying)
                    actor.curAnimationState = x
                    animationStates.append(x)
                    self.animationSpeeds.append(actor.animationSpeed)
                    self.animationStates[-1].enabled = True
                except:
                    pass
            if actor.moving:
                movingObjects.append(actor)
        return animationStates, animationSpeeds, movingObjects
        
    def update(self, time):
        # --the main datamanager loop
        # AI is handled per object
        # TODO: implement a proper event manager
        #print 'DataManager update'
        if self.firstRun:
            self.loadScene(self.defaultScene)
            self.firstRun = False
        
        # objects to spawn this frame
        self.toSpawn = []
        
        # we may want to remove objects from the game
        self.objectsToDestroy = []
        
        # keep count of time
        anim_move = False
        self.updateAITime += time
        self.totalTime += time
        self.elapsed += time
        
        if self.elapsed > self.physTick:
            anim_move = True
##            while self.elapsed > self.physTick:
            self.world.update( self.framerate )
##                self.elapsed -= self.physTick
        else:
            if self.elapsed < self.physTick:
                ## not enough time has passed this loop, so ignore for now.
                anim_move = False
            else:
                self.world.update( self.framerate )
                self.elapsed = 0.0
        
##            if self.elapsed >= self.physTick:
##                anim_move = True
##                self.elapsed = 0.0
        
        # -- AI / GameLogic ---------------------------------------------------
        for act in xrange(len(self.actors)):
            
            # -----------------------------------------------------------------
            actor = self.actors[act]
            if actor.Alive:
                if actor.hasAI:
                    if abs((self.updateAITime % actor.updateAIFrequency) - actor.updateAITime) <= time/2:
                        # this actor should think now
                        actor.Update(self.actors, self.player, self.updateAITime, time)
            else:
                # this actor is dead or dying
                if actor.isAnimated:
                    if actor.curAnimationState.timePosition + (time * 2) > actor.curAnimationState.length:
                        actor.IsAnimated = False
                        actor.Remove()
                else:
                    # this actor is definitely dead, so flag it for deletion
                    actor.Remove()
                    
            if anim_move:
            # play the next animation frames            
                actor.animate(self.physTick)
                # move, if mobile and not physics controlled
                actor.move(self.physTick)
                        
            if actor.toDelete:
                # see if its a shot hitting something
                if hasattr(actor, "HitActor"):
                    x = self.getActorByName(actor.HitActor)
                    if x:
                        actortodamage = self.actorsList[actor.HitActor]
                        actortodamage.takeDamage(actor.damageDealt)
                self.objectsToDestroy.append(self.actors.pop(act))
            # -----------------------------------------------------------------
            
        # physics and visual update (stepped framerate)
        
##        self.world.update(self.framerate)

        
        # The Final delete action for dead objects    
        while self.objectsToDestroy:
            ob = self.objectsToDestroy.pop()
            print 'Destroying:', ob.name
            self.sceneManager.destroySceneNode(ob.Node.name)
            self.sceneManager.destroySceneNode(ob.OgreNode.name)
            self.sceneManager.destroyEntity(ob.Entity)
            del ob
        
        
    def spawnActor(self, actorName, position):
        # -- Spawns an actor into the game
        
        # get a reference to the requested class
        actorFunc = actorDict[actorName]
        # instantiate it
        actor = actorFunc()
        # call the spawn function to load the mesh etc.
        actor.Spawn(position, self.sceneManager, self.actorRoot, self.numActors, self.raySceneQuery, self.updateResolution, self.world)
        # increase the number of actors 
        # NOTE: This is for naming only!!, query len(self.actors) to find
        # the actual number of actors still alive in-game.
        self.numActors += 1
        # add the animations to our animation states list, and start the first animation
        self.actors.append(actor)
##        if (actor.IsAnimated):
##            for i in actor.animations:
##                addState = actor.Entity.getAnimationState(i)
##                self.animationStates.append(addState)
##                self.animationStates[-1].enabled = True
        
        
    def getActorByName(self, name):
        # brute force find the actor by name
        for a in self.actors:
            if a.name == name:
                return a
    
    def showMainMenu(self):
        # --We have a very simple main menu, so we're just going to
        # --do it with overlays, instead of cegui.
        # --Note: this is not the in-game menu, that is in the 
        # --DataManager, and uses inGameListener.py
        
        # First off, clear the scene
        if self.getSceneLoaded():
            self.clearScene()
            
        
        # Load resources for the menu, defined in resources.cfg
        ogre.ResourceGroupManager.getSingleton().initialiseResourceGroup('Core')
        ogre.ResourceGroupManager.getSingleton().initialiseResourceGroup('MainMenu')
        
        # get the datamanager to load the background scene
        self.loadScene('MainMenuBackdrop')
        
        # Load the main menu overlay script
        self.menuOverlay = ogre.OverlayManager.getSingleton().getByName('TekDemo/MainMenu')
        
        # load the frameListener for the menu
        self.menuFrameListener = menuFrameListener(self.renderWindow, self.camera, self.sceneManager)
        
        # setup the interaction between OIS and the overlay.
        self.setupMainMenu()
        
        # activate it to listen for users using the menu.
        self.root.addFrameListener(self.menuFrameListener)