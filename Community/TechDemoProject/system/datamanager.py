import os, sys
import logging
import ogre.renderer.OGRE as ogre
import ogre.io.OIS as OIS
import ogre.physics.OgreNewt as OgreNewt
from dotscene import DotScene
from actors.characters import people
from actors.characters.boids import Bird, Dragon, FishBoid, FairyNest, SmallFairy

from frameListeners import debugFrameListener
from frameListeners import oceanListener
from frameListeners import skyManager
import sys
import eventManager

frameListeners = {'debugFrameListener':debugFrameListener.DebugFrameListener, 
                    'skyInteractFrameListener':debugFrameListener.SkyFrameListener}

actorDict = {}

sys.path.append('.\gameConfig')


class DataManager:
    def __init__(self, sceneManager, viewport, window, camera, framerate=60, physics=False):
        logging.debug('DataManager Starting')
        self.sceneManager = sceneManager
        self.sceneManager.setAmbientLight(ogre.ColourValue(0.1,0.05,0.05))
        rt = self.sceneManager.getRootSceneNode()
        self.shadowType = ogre.SHADOWTYPE_STENCIL_ADDITIVE
        self.sceneManager.shadowTechnique = self.shadowType
        self.viewport = viewport
        self.renderWindow = window
        self.eventManager = eventManager.EventManager()
        self.camera = camera
        self.numActors = 0000
        ray = ogre.Ray(ogre.Vector3(10.0,0.0,10.0), ogre.Vector3.NEGATIVE_UNIT_Y)
        self.raySceneQuery = self.sceneManager.createRayQuery(ray)
        self.raySceneQuery.setSortByDistance(True)
        self.cacheActors()
        self.activePlugins = [] # plugins that are currently loaded
        self.actors = []
        self.objectsToDestroy = []
        self.animationStates = []
        self.animationSpeeds = []
        self.inputListener = None # The framelistener for receiving player input
        self.updateAITime = 0.0
        self.totalTime = 0.0
        self.updateResolution = 1 # Spread actor updates over frames by dividing by this number
        self.hasSkyBox = False
        self.collisionList = []
        self.framerate = framerate # fps rate for physics updates
        self.staticRoot = rt.createChildSceneNode('Root_Static')
        self.foliageRoot = rt.createChildSceneNode('Root_Foliage')
        self.actorRoot = rt.createChildSceneNode('Root_Actors')
        #self.StaticEntity = self.sceneManager.createEntity('Static', 'ellipsoid.mesh')
        #self.staticRoot.attachObject(self.StaticEntity)
        self.staticCollisionBody = None
        logging.debug('Datamanager get world')
        if physics:
            self.world = OgreNewt.World()
            self.treeColParser = OgreNewt.TreeCollisionSceneParser( self.world ) # note - this IS a treecollision itself
            self.treeColLoader = OgreNewt.TreeCollisionSerializer()
            logging.debug('world and loader / parser initialized')
        self.elapsed = 0.0 # used for physics update synching
        self.physTick = (1.0 / self.framerate)
        self.player = None
        self.defaultScene = 'testLevel.cfg'
        self.firstRun = True
        
        logging.debug('DataManager Initialized ::')
        
    def __del__(self):
        self.clearScene()
##            while self.objectsToDestroy:
##            ob = self.objectsToDestroy.pop()
##            print 'Destroying:', ob.name
##            ob.MediaTree.destroy( self.sceneManager )
##            self.sceneManager.destroySceneNode( ob.OgreNode.name )
####            self.sceneManager.destroySceneNode(ob.Node.name)
####            self.sceneManager.destroySceneNode(ob.OgreNode.name)
####            self.sceneManager.destroyEntity(ob.Entity)
##            del ob
##        while self.actors:
##            ob = self.actors.pop()
##            print 'Destroying:', ob.name
##            ob.MediaTree.destroy( self.sceneManager )
##            self.sceneManager.destroySceneNode( ob.OgreNode.name )
##            del ob
##        print 'bodies'
##        #del self.sceneManager
##        print 'sm'
##        #del self.viewport
##        print 'vp'
##        #del self.renderWindow
##        print 'rw'
##        #del self.camera
##        print 'cam'
##        #del self.inputListener
##        print 'fl'
##        
##        print 'stat_col'
##        #del self.treeColParser
##        print 'parser'
##        #del self.world
        
        
    def cacheActors(self):
        pKeys = people.__dict__.keys()
        for k in pKeys:
            if '__' not in k:
                if not k in ['ogre', 'base_actor']:
                    actorDict[k] = people.__dict__[k]
        
        # I couldn't get it to find my actor, what's up with that?    
        # For now, I'm manually adding it here.  
        actorDict["Bird"] = Bird
        actorDict["Dragon"] = Dragon
        actorDict["FishBoid"] = FishBoid
        actorDict["FairyNest"] = FairyNest
        actorDict["SmallFairy"] = SmallFairy
            
        #print actorDict
                    
        
        
        
    
    def loadScene(self, scriptName):
        # load up a scene from a script. the script may load
        # an xml scene and moving models, or just one of those.
        # check out XXXXXXXXXX to see the script.
        # dirty script parsing begins here:
        logging.debug('Loading Scene:', scriptName)
        settings = []
        f = open(os.path.join(os.curdir ,'system', 'gameConfig', scriptName), 'r')
        script = f.readlines()
        for l in script:
            if '//' not in l:
                if '=' in l:
                    key, value = l.split('=')
                    settings.append([ key.strip(), value.strip() ])
        for ky in settings:
            k = ky[0]
            # parse the settings and load up stuff
            if k == 'RESOURCE_GROUP':
                ogre.ResourceGroupManager.getSingleton().initialiseResourceGroup(ky[1])
            if k == 'SKYBOX':
                self.hasSkyBox = True
                self.skyBoxMaterial = ky[1]
                self.sceneManager.setSkyBox(True, ky[1])
            if k == 'SKYMANAGER':
                skyMan = skyManager.SkyManager(True, True, self.sceneManager.getName())
                ogre.Root.getSingleton().addFrameListener(skyMan)
                self.activePlugins.append(skyMan)
                skyInteractListener = debugFrameListener.SkyFrameListener(self.renderWindow, self.camera, self.sceneManager, skyMan)
                ogre.Root.getSingleton().addFrameListener(skyInteractListener)
                self.activePlugins.append(skyInteractListener)
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
            if k == 'WATERLEVEL':
                waterManager = oceanListener.oceanRttManager(self.camera, self.renderWindow)
                waterManager._createScene(self.sceneManager, self.renderWindow)
                ogre.Root.getSingleton().addFrameListener(waterManager)
                self.activePlugins.append(waterManager)
            if k == 'FOLIAGE_SCENE':
                sceneFile = os.path.join(os.curdir, 'system', 'gameConfig', ky[1])
                self.foliageLoader = DotScene( sceneFile, self.sceneManager, self.foliageRoot)
            if k == 'XML_SCENE_PHYSICS':
                # We'll cache the collision trees,
                    # import a pre-cached collision scene
                    # TODO: MD5 checking to match collision to scene file
                    # incase one needs to be overwritten
##                    f = open(ky[1], 'r')
##                    tree = ogre.FileHandleDataStream(f)
                try:
                    f = open(ky[1], 'rb').read()
                    self.treeCollision = OgreNewt.TreeCollision(self.world)
                    dataS = ogre.DataStream(f)
                    self.treeColLoader.importTreeCollision( dataS, self.treeCollision)
                    logging.debug('Tree Collision imported........')
                    #f.close()
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
                        logging.debug('Tree Collision Created and Exported')
                    else:
                        # nothing we can do if no scene has been loaded
                        # the order must be right in the config file
                        logging.debug('No way to make Tree Collision')
                        pass
            if k == 'ACTOR': # 2 properties: ky[1]:actorName, ky[2]:position
                actor, pos = ky[1].split('@')
                x,y,z = pos.strip().split(',')
                pos = (float(x), float(y), float(z))
                self.spawnActor(actor.strip(), pos)
            if k == 'FRAME_LISTENER':
                fl = frameListeners[ky[1]]
                inputListener = fl(self.renderWindow, self.camera, self.sceneManager)
                ogre.Root.getSingleton().addFrameListener(inputListener)
                self.activePlugins.append(inputListener)
            if k == 'COMPOSITOR':
                comp = ky[1]
                logging.debug('ADDING COMPOSITOR')
                cms = ogre.CompositorManager.getSingleton()
                if not cms.hasCompositorChain(self.renderWindow.getViewport(0)):
                    newcomp = cms.addCompositor(self.renderWindow.getViewport(0), comp)
                    cms.setCompositorEnabled(self.renderWindow.getViewport(0), comp, True)
                else:
                    cms.setCompositorEnabled(self.renderWindow.getViewport(0),comp, False)
##        OgreNewt.Debugger.getSingleton().init(self.sceneManager)
##        OgreNewt.Debugger.getSingleton().showLines(self.world)
                        
                        
    def clearScene(self):
        # clear the current scene and models
        #del self.objectsToDestroy
        while self.objectsToDestroy:
            obNum = self.objectsToDestroy.pop()
            ob = self.actors.pop(obNum)
            ob.MediaTree.destroy( self.sceneManager )
            self.sceneManager.destroySceneNode( ob.OgreNode.name )
            del ob
        while self.actors:
            ob = self.actors.pop()
            ob.MediaTree.destroy( self.sceneManager )
            self.sceneManager.destroySceneNode( ob.OgreNode.name )
            del ob
        del self.staticCollisionBody
        del self.treeColLoader
        del self.activePlugins
        logging.debug('DataManager cleared scene')
        
        
        
    
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
        
##        # update the sky
##        if self.skyManager:
##            self.skyManager.frameStarted(time)
##            
##        if self.skyManager.events:
##            logging.debug('SkyManager Event Found')
##            while self.skyManager.events:
##                    self.eventManager.addWorldEvent(self.skyManager.events.pop())

        # Loop thru all plugins and call the framestarted method.
        for plug in self.activePlugins:
            #plug.frameStarted(time)
            if plug.events:
                #logging.debug('FrameListener Event Found')
                while plug.events:
                    self.eventManager.addWorldEvent(plug.events.pop())
                    
        
        # Run the eventManager
        system_events = world_events = actor_events = []
        system_events, world_events, actor_events = self.eventManager.update(time)
        

        #logging.debug('worldevents %s ' % str(world_events))
            

        #logging.debug('actorEvents %s ' % str(actor_events))
            
        # execute any events on the datamanager
        if world_events:
            for evt in world_events:
                if hasattr(self, evt.function):
                    func = getattr(self, evt.function)
                    func(evt.args)
        
        if actor_events:
            #print 'Actor Events'
            for evt in actor_events:
                if hasattr(self, evt.function):
                    func = getattr(self, evt.function)
                    if evt.function == 'spawnActor':
                        self.spawnActor(evt.args[0], evt.args[1])
                    else:
                        func(evt.args)
                    
                    
        
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
                # apply World Events
                if world_events:
                    for evt in world_events:
                        if hasattr(actor, evt.function):
                            func = getattr(actor, evt.function)
                            func(evt.args)
                            
                if actor.events:
                    while actor.events:
                        self.eventManager.addActorEvent(actor.events.pop())
                    
                        
                if actor.hasAI:
                    if abs((self.updateAITime % actor.updateAIFrequency) - actor.updateAITime) <= time/2:
                        # this actor should think now
                        actor.Update(self.actors, self.player, self.updateAITime, self.world, time)
                        # Get actors requested events
##                        if actor.events:
##                            for evt in actor.events:
##                                self.eventManager.addActorEvent(evt)
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
                # the actor may have died before getting here
                if actor.Alive:
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
                
                self.objectsToDestroy.append(act)
            # -----------------------------------------------------------------
            
        
            
        # physics and visual update (stepped framerate)
        
##        self.world.update(self.framerate)

        
        # The Final delete action for dead objects    
        while self.objectsToDestroy:
            obNum = self.objectsToDestroy.pop()
            ob = self.actors.pop(obNum)
            ob.MediaTree.destroy( self.sceneManager )
            del ob.bodies
            del ob.body
            self.sceneManager.destroySceneNode( ob.OgreNode.name )
            logging.debug('DestroyingNode:' + ob.name)
            del ob
            
        
        # change camera view for underwater
        if hasattr(self, 'seaLevel'):
            if self.camera.position.y < 0.00:
                end = 15 + max(0, (150 + self.camera.position.y) )
                if self.camera.position.y < -200:
                    bFac = 0.04
                else:
                    ratio = ((200.00 - abs(self.camera.position.y)) / 200.00)
                    bFac = 0.3 * ratio
                    gFac = 0.15 * ratio
                # set the skybox seperately, so we can still see it underwater :)
                self.sceneManager.setFog(ogre.FogMode.FOG_LINEAR, ogre.ColourValue(0.04, gFac, bFac), 0.001, 50.0, end)
                if self.hasSkyBox:
                    mat = ogre.MaterialManager.getSingleton().getByName(self.skyBoxMaterial)
                    mat.setFog(False, ogre.FogMode.FOG_LINEAR, ogre.ColourValue(0.8, 0.9, 0.95), 0.001, 40000, 80000)
                    
                
            else:
                self.sceneManager.setFog(ogre.FogMode.FOG_LINEAR, ogre.ColourValue(0.8, 0.8, 0.9), 0.001, 40000.0, 80000.00)
        
        
        
    def spawnActor(self, actorName, position):
        # -- Spawns an actor into the game
        logging.debug('Spawning: ' + str(actorName) + str(position))
        # get a reference to the requested class
        actorFunc = actorDict[actorName]
        # instantiate it
        actor = actorFunc()
        # make certain of a unique name
        actor.name = actor.name + str(self.numActors)
        # call the spawn function to load the mesh etc.
        actor.Spawn(position, self.sceneManager, self.actorRoot, self.numActors, self.raySceneQuery, self.updateResolution, self.world)
        # increase the number of actors 
        # NOTE: This is for naming only!!, query len(self.actors) to find
        # the actual number of actors still alive in-game.
        self.numActors += 1
        # add to the actor list
        self.actors.append(actor)
        
        
    def getActorByName(self, name):
        # brute force find the actor by name
        for a in self.actors:
            if a.name == name:
                return a
    
    def showMainMenu(self):
        
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
