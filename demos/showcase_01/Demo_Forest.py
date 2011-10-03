



import warnings 
warnings.simplefilter('ignore', RuntimeWarning) 
import ogre.renderer.OGRE as ogre
import ogre.io.OIS as ois
import ogre.addons.ogreforests as forests
import ogre.addons.plib as plib
#import ogre.sound.OgreAL as OgreAL
import ogre.addons.caelum as caelum
import SampleFramework as sf
import math, random, ctypes
warnings.simplefilter('default', RuntimeWarning)

try:
    import psyco
    psyco.full()
except:
    pass

WORLD_SIZE = 5000
WATER_LEVEL = 80

CONNECTED = False

TREE_FPS = 1.00 / 4.0

class ForestApplication(sf.Application, ogre.RenderTargetListener):
    def __init__(self):
        "Init Render Application"
        sf.Application.__init__(self)
        self.mPlane = None
        self.mPlaneEnt = None
        self.mReflectCam= None
        self.mPlaneNode= None
        self.connected = False
        ogre.RenderTargetListener.__init__(self)
    
    
    
    def _chooseSceneManager(self):
        # self.sceneManager = self.root.createSceneManager("TerrainSceneManager")
        self.sceneManager = self.root.createSceneManager("TerrainSceneManager")
        # self.sceneManager.setShadowTechnique(ogre.SHADOWTYPE_TEXTURE_ADDITIVE)

        
    def preRenderTargetUpdate(self,evt):
        self.mPlane.setVisible(False)
           
    def postRenderTargetUpdate(self,evt):
        self.mPlane.setVisible(True)
        
    def createReflectionSystem(self):
        "Override sf create scene"
        sceneManager = self.sceneManager
        camera = self.camera

        # Create a prefab plane
        self.mPlane = ogre.MovablePlane("ReflectPlane")
        self.mPlane.d = 0
        self.mPlane.normal =ogre.Vector3.UNIT_Y
        ogre.MeshManager.getSingleton().createPlane("ReflectionPlane", 
            ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, 
            self.mPlane, 10000.0, 10000.0, 
            1, 1, True, 1, 1.0, 1.0, ogre.Vector3.UNIT_Z,
            ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, 
            True,True
            )
        # Create plane entity
        self.mPlaneEnt = sceneManager.createEntity( "Plane", "ReflectionPlane" )
        # Attach the rtt entity to the root of the scene
        rootNode = sceneManager.getRootSceneNode()
        self.mPlaneNode = rootNode.createChildSceneNode()
        # Attach both the plane entity, and the plane definition
        self.mPlaneNode.attachObject(self.mPlaneEnt)
        self.mPlaneNode.attachObject(self.mPlane) 
        self.mPlaneNode.translate( ogre.Vector3(2500, WATER_LEVEL, 2500))

        # create RenderTexture
        rttTex = self.root.getRenderSystem().createRenderTexture( "RttTex", 512, 512, 
                                                                 ogre.TEX_TYPE_2D,ogre.PixelFormat.PF_R8G8B8 )

        
        self.mReflectCam = sceneManager.createCamera("ReflectCam")
        self.mReflectCam.setNearClipDistance(camera.getNearClipDistance())
        self.mReflectCam.setFarClipDistance(camera.getFarClipDistance())
        w = self.renderWindow.getViewport(0).getActualWidth() 
        h = self.renderWindow.getViewport(0).getActualHeight ()
        self.mReflectCam.setAspectRatio ( float(w)/float(h) )
        # create ViewPort        
        v = rttTex.addViewport( self.mReflectCam )
        v.setClearEveryFrame ( True  )
        v.setBackgroundColour (ogre.ColourValue.Black)

        oceanMat = ogre.MaterialManager.getSingleton().getByName("OceanCg_RTT")
        mPass = oceanMat.getTechnique(0).getPass(0)
        tState = mPass.getTextureUnitState(1)
        tState.setTextureName("RttTex")
        tState.setColourOperationEx( ogre.LBX_BLEND_MANUAL, ogre.LBS_TEXTURE, 
                               ogre.LBS_CURRENT, ogre.ColourValue.White,
                              ogre.ColourValue.White, 0.25)
                                
        tState.setTextureAddressingMode (ogre.TextureUnitState.TAM_CLAMP)
        #tState.setProjectiveTexturing(True, self.mReflectCam)
        
        
        rttTex.addListener(self)

        # set up linked reflection
        self.mReflectCam.enableReflection(self.mPlane)
        # Also clip
        
        # NOTE..  a difference in Python implementation
        ## this doesn't work as mPlane is treated as a Plane instead of a MoveablePlane 
        #self.mReflectCam.enableCustomNearClipPlane(self.mPlane) 
        ## So we have an override :)
        
        self.mReflectCam.enableCustomNearClipPlaneMP(self.mPlane) 

        #Give the plane a texture
        self.mPlaneEnt.setMaterialName("OceanCg_RTT")

        
    def getTerrainHeight( self, x, z ):
        #print "GetHeight"
        if ( x < 0 or z < 0 or x > WORLD_SIZE or z > WORLD_SIZE ):  return 0
        self.updateRay.setOrigin( ogre.Vector3(x, 0.0, z) )
        self.updateRay.setDirection( ogre.Vector3.UNIT_Y )
        self.raySceneQuery.setRay( self.updateRay )
        self.raySceneQuery.execute( self.raySceneQueryListener )
        return self.raySceneQueryListener.resultDistance
    
    
    def tryConnect(self):
        import socket as s
        # we need to send (and recieve) data on the local machine using port 4444
        (family, socktype, proto, canonname, sockaddr) = s.getaddrinfo ( "127.0.0.1",4444)[0] #s.gethostname(), 4444 )[0]
        self.socket = s.socket( s.AF_INET, s.SOCK_STREAM ) # create the client socket, note I'm testing with UDP traffic
        self.socket.connect ( sockaddr )
        CONNECTED = True
        self.connected = True
        print 'Client Connects'
        
        
    def __del__(self):
        self.socket.close()
        sf.Application.__del__(self)
        
    ##---- PagedGeometry test ----
    def createTrees(self):

        
        ## TREES
        ##Initialize the PagedGeometry engine
        self.trees = forests.PagedGeometry()
        self.trees.setCamera( self.camera )
        self.trees.setPageSize( 500 )
        self.trees.setBounds( ogre.FloatRect(0, 0, WORLD_SIZE, WORLD_SIZE) )
        
        self.mTex = ogre.TextureManager.getSingleton().createManual(
                        "DynamicTexture",
                        ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME,
                        ogre.TEX_TYPE_2D,
                        16, 16,
                        0,
                        ogre.PF_BYTE_BGRA,
                        ogre.TU_DYNAMIC)
    
        ##Set up LODs
        ##self.trees.addDetailLevelEntityPage(50, 50)
        self.trees.addDetailLevelBatchPage(100, 20)
        self.trees.addDetailLevelImpostorPage(2000, 100)
        ##Set up a TreeLoader for easy use
        self.treeLoader = forests.TreeLoader2D( self.trees, ogre.FloatRect(0, 0, WORLD_SIZE, WORLD_SIZE) )
        self.trees.setPageLoader(self.treeLoader)
        self.treeLoader.setHeightFunction(self, "getTerrainHeight")
        
    
        ##And add 20,000 trees to the scene through the "easy-to-use" TreeLoader class
        palm1 = self.sceneManager.createEntity("MyTree", "KinkyPalm.mesh")
        palm2 = self.sceneManager.createEntity("pineTree", "StraightPalm.mesh")
        palm3 = self.sceneManager.createEntity("palmBendy", "BendyPalm.mesh")
        bush1 = self.sceneManager.createEntity("bush1", "BigDarkBush.mesh")
        birch = self.sceneManager.createEntity("birch", "birch.mesh")
        benTree = self.sceneManager.createEntity("benTree", "benTree.mesh")
        spot_shroom = self.sceneManager.createEntity("redShroom", "spotty_shroom.mesh")
        blue_shroom = self.sceneManager.createEntity("blueShroom", "blue_shroom.mesh")
        beech = self.sceneManager.createEntity("beech", "beech_tree.mesh")
        newTree = self.sceneManager.createEntity("newTree", "Pine.mesh")
        self.treeModels = [ bush1, newTree ]#, palm2, spot_shroom ]
        # fix the damn rotations
        x=0
        z=0
        for i in range (30000):
            yaw = random.randrange(0, 360)
    
            if (random.randrange(0, 1000) <= 800):
                x += random.randrange(-10.0, 10.0)
                z += random.randrange(-10.0, 10.0)
                if (x < 0): x = 0 
                elif (x > WORLD_SIZE): x = WORLD_SIZE
                if (z < 0): z = 0 
                elif (z > WORLD_SIZE): z = WORLD_SIZE
            else:
                x = random.randrange(0, WORLD_SIZE)
                z = random.randrange(0, WORLD_SIZE)
            
            scale = random.randrange(9, 20) / 10
            
            if self.getTerrainHeight(x, z) > WATER_LEVEL:
                t_type = random.randrange(0, len(self.treeModels))
                self.treeLoader.addTree(self.treeModels[t_type], ogre.Vector2(x, z), ogre.Degree(yaw), scale)

        #self.treeLoader.setColorMap(self.mTex)
        ##Grass
        
        ##Initialize the PagedGeometry engine
        self.grass = forests.PagedGeometry(self.camera, 50)
        ##trees = new PagedGeometry(camera, 50, ogre.FloatRect(0, 0, WORLD_SIZE, WORLD_SIZE))
        
        ##Set up LODs
        self.grass.addDetailLevelGrassPage(100.0,0.0)
        
        ##Set up a TreeLoader for easy use
        self.grassLoader = forests.GrassLoader(self.grass)
        self.grass.setPageLoader(self.grassLoader)
        self.grassLoader.setHeightFunction(self, "getTerrainHeight")

        ## iterate grass materials
        for nam in ["grass"]:#, "dry_grass", "tall_grass"]:
            grassLayer = self.grassLoader.addLayer(nam)
            
            grassLayer.setAnimationEnabled(True)
            grassLayer.setSwaySpeed(1.5)
            grassLayer.setSwayLength(0.08)
            grassLayer.setSwayDistribution(10.0)
            
            grassLayer.setDensity(0.2)
            
            grassLayer.setMapBounds(ogre.FloatRect(0, 0, WORLD_SIZE, WORLD_SIZE))
            
            grassLayer.setColorMap("terrain_grass.jpg")
            #grassLayer.setColorMap(self.mTex)
            grassLayer.setColorMapFilter(forests.MAPFILTER_BILINEAR)
            
            grassLayer.setDensityMap("densitymap.png")
            grassLayer.setDensityMapFilter(forests.MAPFILTER_NONE)
        
            ##grassLayer.setMinimumSize(0.5,0.5)
            ##grassLayer.setMaximumSize(1.0, 1.0) 
            
            grassLayer.setMinimumSize(4.0,1.5)
            grassLayer.setMaximumSize(4.0,2.0)
            
            grassLayer.setFadeTechnique(forests.FADETECH_GROW)
        
        
    def createSkySystem(self):
        self.caelumSystem = caelum.CaelumSystem(self.root, self.sceneManager)
        
        ## Setup sun position model.
        self.spm = caelum.SimpleSunPositionModel (ogre.Degree (13))
        ## IMHO EarthSunPositionModel is broken, so it default to sspm.
        ##caelum.SunPositionModel *spm = new caelum.EarthSunPositionModel (ogre.Radian (ogre.Degree (37)), 0.5)
        
        ## Setup sun options
        self.caelumSystem.getSun ().setAmbientMultiplier (ogre.ColourValue(0.5, 0.5, 0.5, 100.0))
        self.caelumSystem.getSun ().setDiffuseMultiplier (ogre.ColourValue(3, 3, 2.7, 100.0))
        ## For green terrain:
        #self.caelumSystem.getSun ().setDiffuseMultiplier (ogre.ColourValue(0.5, 0.5, 0.5))
        self.caelumSystem.getSun ().setSpecularMultiplier (ogre.ColourValue(2.0, 2.0, 2.0, 100.0))
        self.caelumSystem.getSun ().setSunPositionModel (self.spm)
        self.caelumSystem.getSun ().setManageAmbientLight (True)

        sun = self.caelumSystem.getSun()
        
        ## Basic fogging setup.
        ## This is a hack until proper fogging is implemented.
        #self.caelumSystem.getSkyColourModel().setFogDensityMultiplier(0.0015)
        #self.caelumSystem.setManageFog(True)
        
        ## Setup cloud options

        self.caelumSystem.getClouds ().setCloudSpeed(ogre.Vector2(-0.00001, 0.00001))
        clouds = self.caelumSystem.getClouds()
        self.caelumSystem.getClouds().setCloudMassOffset(ogre.Vector2(0.5, 0.2))
        self.caelumSystem.getClouds().setCloudCover(0.5)
        self.caelumSystem.getClouds().setCloudBlendTime(7200 * 24)
        self.caelumSystem.getClouds().setCloudMassBlend(0.9)
        self.caelumSystem.getClouds().update(self.caelumSystem.getUniversalClock().getCurrentTime(), ogre.Vector3.UNIT_Y, ogre.ColourValue(0.5, 0.5, 0.5), ogre.ColourValue(3, 3, 2.7))
        #self.caelumSystem.getClouds().setAnimating(False)
        self.caelumSystem.getUniversalClock ().setTimeScale (256)
        ogre.ControllerManager.getSingleton().setTimeFactor (256)
        
        
        ## Setup starfield options
        self.caelumSystem.getStarfield ().setInclination (ogre.Degree (13))
        
        ## Set some time parameters
        self.caelumSystem.getUniversalClock ().setTimeScale (256)
        self.caelumSystem.getUniversalClock ().setCurrentTime (18000) ## Jan 1st, 5am


        ## Register all to the render window
        self.renderWindow.addListener(self.caelumSystem)
        
        
    def createEntities(self):
        self.robots = []

        for i in range(10):
            robo = Robot(self)
            robo.spawn(ogre.Vector3(2500 + (i * 10), 600, 2500), i)
            self.robots.append(robo)
            
        self.whales = []
        for i in range(3):
            
            whale = Whale(self)
            whale.spawn(ogre.Vector3(1800,300,2500), 10+i)
            self.whales.append(whale)

    
 
        
        
        tipi_ent = self.sceneManager.createEntity("tipi", "tipi.mesh")
        tipi_node = self.sceneManager.getRootSceneNode().createChildSceneNode()
        tipi_node.attachObject(tipi_ent)
        tipi_node.setPosition(ogre.Vector3(700, 100, 750))
        pos = tipi_node.getPosition()
        tipi_node.setPosition(pos.x, self.getTerrainHeight(700,750) + 0, pos.z)
        
        
        


    ##Loads world
    def _createScene(self):
        camera = self.camera
        camera.setNearClipDistance(4.00)

        self.updateRay = ogre.Ray()
        self.updateRay.setOrigin(camera.getPosition())
        self.updateRay.setDirection(ogre.Vector3.NEGATIVE_UNIT_Y)
        self.raySceneQuery = self.sceneManager.createRayQuery(self.updateRay)
        self.raySceneQuery.setQueryTypeMask(ogre.SceneManager.WORLD_GEOMETRY_TYPE_MASK )   
        self.raySceneQuery.setWorldFragmentType(ogre.SceneQuery.WFT_SINGLE_INTERSECTION)
        self.raySceneQueryListener = myRaySceneQueryListener()
        terrain_cfg = "./triplanar_terrain.cfg"
    
        self.sceneManager.setWorldGeometry( terrain_cfg )
        
        self.createReflectionSystem()
    
        ## Set a nice viewpoint
        camera.setPosition(2274,600,2586)
        camera.lookAt(2000, 500, 2600)
        ##camera.setPolygonMode(PM_WIREFRAME
        
        self.createSkySystem()
        
        self.createTrees()
        
        self.createEntities()


    def _createFrameListener(self):
        self.frameListener = GrassFrameListener(self)
        self.root.addFrameListener(self.frameListener)
##        self.roboListener = robotsListener(self)
##        self.root.addFrameListener(self.roboListener)
##        self.fL = RenderToTextureFrameListener(self.renderWindow, self.camera,
##                                                           self.mReflectCam, self.mPlaneNode)
##        self.root.addFrameListener(self.fL)
##        
##        self.visuals = VisualsListener(self)
##        self.root.addFrameListener(self.visuals)
##        
##        self.sounds = soundListener()
##        self.root.addFrameListener(self.sounds)

        
    def setTextureManual(self, texture, width, height, dataArray): 
        # Get the pixel buffer 
        pixelBuffer = texture.getBuffer() 
        # Lock the pixel buffer and get a pixel box 
        pointer = pixelBuffer.lock(0,width*height,ogre.HardwareBuffer.HBL_NORMAL) 
        #number 4 here is use to accommodate the bytes for r, g, b, a may vary with different pixel formats 
        storageclass = ctypes.c_uint8 * (width*height*4) 
        cbuffer=storageclass.from_address(ogre.CastInt(pointer))
        cbuffer[:] = dataArray[:]
        # Unlock the pixel buffer 
        pixelBuffer.unlock()
        
class Robot:
    def __init__(self, app):
        self.app = app
        self.speed = 5.0
        self.decisionTime = 5.0 + random.randint(2, 10)
        self.direction = ogre.Vector2(random.uniform(-1.0, 1.0), random.uniform(-1.0, 1.0))
        self.mustRotate = True
        
    def spawn(self, position, numActors):
        self.timer = 0.0
        self.ent = self.app.sceneManager.createEntity("robot_" + str(numActors), "robot.mesh")
        self.posNode = self.app.sceneManager.getRootSceneNode().createChildSceneNode()
        self.rotNode = self.posNode.createChildSceneNode()
        self.posNode.setScale(ogre.Vector3(0.1, 0.1, 0.1))
        self.rotNode.attachObject(self.ent)
        self.posNode.setPosition(position)
        self.rotNode.yaw(ogre.Degree(90))
        #print self.posNode.position
        self.animation = self.ent.getAnimationState('Walk')
        self.animation.Enabled = True

    def addTime(self, time):
        self.animation.addTime(time)
        #print self.Node.position
        self.timer += time
        if self.timer >= self.decisionTime:
            self.timer = 0
            self.direction = ogre.Vector2(random.uniform(-1.0, 1.0), random.uniform(-1.0, 1.0))
            self.direction.normalise()
            self.mustRotate = True
            self.posNode.setDirection(self.direction.x, 0, self.direction.y, self.posNode.TS_WORLD)
            print self.posNode.position
        if self.mustRotate:
            pass
                
        pos = self.posNode.position
        off = self.direction * (self.speed * time)
        self.posNode.setPosition(pos.x + off.x, pos.y, pos.z + off.y)
        
        
class Whale:
    def __init__(self, app):
        self.app = app
        self.speed = 8.0
        self.decisionTime = 10.0 + random.randint(2, 10)
        self.direction = ogre.Vector2(random.uniform(-1.0, 1.0), random.uniform(-1.0, 1.0))
        self.mustRotate = True
        
    def spawn(self, position, numActors):
        self.timer = 0.0
        self.ent = self.app.sceneManager.createEntity("whale_" + str(numActors), "whale.mesh")
        self.posNode = self.app.sceneManager.getRootSceneNode().createChildSceneNode()
        self.rotNode = self.posNode.createChildSceneNode()
        scal = random.uniform(2.0, 5.0)
        self.posNode.setScale(ogre.Vector3(scal, scal, scal))
        self.rotNode.attachObject(self.ent)
        self.posNode.setPosition(position)
        self.rotNode.yaw(ogre.Degree(90))
        #print self.posNode.position
        self.animation = self.ent.getAnimationState('Swim')
        self.animation.Enabled = True

    def addTime(self, time):
        self.animation.addTime(time)
        #print self.Node.position
        self.timer += time
        if self.timer >= self.decisionTime:
            self.timer = 0
            self.direction = ogre.Vector2(random.uniform(-1.0, 1.0), random.uniform(-1.0, 1.0))
            self.direction.normalise()
            self.mustRotate = True
            self.posNode.setDirection(self.direction.x, 0, self.direction.y, self.posNode.TS_WORLD)
            print self.posNode.position
        if self.mustRotate:
            pass
                
        pos = self.posNode.position
        off = self.direction * (self.speed * time)
        self.posNode.setPosition(pos.x + off.x, pos.y, pos.z + off.y)
        
        
        
##class robotsListener(ogre.FrameListener):
##    def __init__(self, app):
##        ogre.FrameListener.__init__(self)
##        self.app = app
##        self.raySceneQuery = app.sceneManager.createRayQuery(ogre.Ray(app.camera.getPosition(),
##                                                            ogre.Vector3.NEGATIVE_UNIT_Y))
##        self.Ray = ogre.Ray()
##
##        
##    def frameStarted(self, frameEvent):
##        for r in self.app.robots:
##            r.addTime(frameEvent.timeSinceLastFrame)
##            self.Ray.setOrigin(r.posNode.position)
##            self.Ray.setDirection(ogre.Vector3.NEGATIVE_UNIT_Y)
##            self.raySceneQuery.Ray = self.Ray
##            for queryResult in self.raySceneQuery.execute():
##                if queryResult.worldFragment is not None:
##                    pos = r.posNode.position
##                    r.posNode.setPosition(pos.x, pos.y - queryResult.distance + 0.6, pos.z)
##                    break
##        for whale in self.app.whales:        
##            whale.addTime(frameEvent.timeSinceLastFrame)
##        return True
##        #return ogre.FrameListener.frameStarted(self, frameEvent)
    

    
class GrassFrameListener(sf.FrameListener):#, ogre.RaySceneQueryListener):
    def __init__(self, app):
        print "Making FrameListener"
        sf.FrameListener.__init__(self, app.renderWindow, app.camera)
        #ogre.RaySceneQueryListener.__init__(self)
        self.app = app
##        self.sceneManager = app.sceneManager
        self.timeSince = 0.0
        self.timeTillNextUpdate = 0
        self.moveSpeed = 40.0
        self.updateToggle = True
        self.getLeafMaterialParams()
        self.camera = app.camera
        self.mReflectCam = app.mReflectCam       
        self.planeNode = app.mPlaneNode
        self.curFogSettings = [ app.sceneManager.getFogStart(),
                                app.sceneManager.getFogEnd(),
                                app.sceneManager.getFogDensity(),
                                app.sceneManager.getFogColour()
                                ]
        self.nextFogSettings = []
        self.transitionTime = 0.0
        self.timer = 0.0
        self.tree_timer = 0.0
        # use this to tweak the persons position
        self.raySceneQuery = app.sceneManager.createRayQuery(ogre.Ray(app.camera.getPosition(),
                                                                    ogre.Vector3.NEGATIVE_UNIT_Y))
        
        self.Ray = ogre.Ray()


    def getLeafMaterialParams(self):
        self.mActiveMaterial = ogre.MaterialManager.getSingleton().getByName("LOTTreeLeavesNoBones")
        self.mActivePass = self.mActiveMaterial.getTechnique(0).getPass(0)
        self.mActiveVertexProgram = self.mActivePass.getVertexProgram()
        self.mActiveVertexParameters = self.mActivePass.getVertexProgramParameters()
        
    def queryResult(self, fragment,  distance):
        self.resultDistance = distance
        return False

        
    def frameStarted(self, frameEvent):
        self.timeTillNextUpdate -= frameEvent.timeSinceLastFrame
        if (self.timeTillNextUpdate<= 0):
            if (self.Keyboard.isKeyDown (ois.KC_SPACE)):
                self.timeTillNextUpdate = 3
                self.doHarvesting()

        self.tree_timer += frameEvent.timeSinceLastFrame
        if self.tree_timer >= TREE_FPS:
            self.app.trees.update()
            self.app.grass.update()
            self.tree_timer = 0.0
            
            
        self.app.grassLoader.updateAnimation()
        self.timeSince = frameEvent.timeSinceLastFrame
        updateRay = ogre.Ray()
        updateRay.setOrigin (self.camera.getPosition() + ogre.Vector3(0.0, 10.0, 0.0))
        updateRay.setDirection (ogre.Vector3.NEGATIVE_UNIT_Y)
        self.raySceneQuery.Ray = updateRay
        pos = self.camera.getPosition()
        th = self.app.getTerrainHeight(pos.x, pos.z)
        self.camera.setPosition(pos.x, th+18.0, pos.z)

##        for queryResult in self.raySceneQuery.execute():
##            if queryResult.worldFragment is not None:
##                pos = self.camera.getPosition()
##                self.camera.setPosition (pos.x, pos.y - queryResult.distance + 18.0, pos.z)
##                break

        self.mReflectCam.setOrientation ( self.camera.getOrientation() )
        self.mReflectCam.setPosition (self.camera.getPosition())

        if self.camera.getPosition().y < WATER_LEVEL:
            self.app.sceneManager.setFog(ogre.FOG_LINEAR, ogre.ColourValue(0.0,0.0,0.1),
                                       0.001, 2, 50)
        else:
            self.app.sceneManager.setFog(ogre.FOG_LINEAR, self.app.caelumSystem.getSun().getSunLightColour(),
                                       0.001, 1000, 2500)
        
        ## Robotos
        
        for r in self.app.robots:
            r.addTime(frameEvent.timeSinceLastFrame)
            pos = r.posNode.getPosition()
            th = self.app.getTerrainHeight(pos.x, pos.z)
            r.posNode.setPosition(pos.x, th, pos.z)
            
        for whale in self.app.whales:        
            whale.addTime(frameEvent.timeSinceLastFrame)


        # Caelum Sunlight Ting
##        greyVal = int(255 * self.app.caelumSystem.getSun().getSunLightColour().r)
##        colArray = [greyVal, greyVal, greyVal, 255] * 16 * 16
##        self.app.setTextureManual(self.app.mTex, 16,16, colArray)
        # self.app.treeLoader.setColorMap( self.app.mTex )

        # ------------------------------------- SPECIAL LEAF MESH STUFF ------------------------------------ #
        
        # Get PreRotated Quad Thingy for leaf meshes..
##        vCenter = ogre.Vector3( 0.0, 0.0, 0.0 ) 
##        forward = self.app.camera.getDerivedDirection()
##        vRight = forward.crossProduct(ogre.Vector3.UNIT_Y) 
##        vUp = self.app.camera.getUp()
##        vRight.normalise() 
##        vUp.normalise()
##
##        vPoint0 = vCenter + (-vRight - vUp) 
##        vPoint1 = vCenter + ( vRight - vUp) 
##        vPoint2 = vCenter + ( vRight + vUp) 
##        vPoint3 = vCenter + (-vRight + vUp) 

        #single prerotated quad oriented towards the camera 
##        preRotatedQuad = ogre.Matrix4(
##                   vPoint0.x, vPoint0.y, vPoint0.z, 0.0,
##                   vPoint1.x, vPoint1.y, vPoint1.z, 0.0,
##                   vPoint2.x, vPoint2.y, vPoint2.z, 0.0,
##                   vPoint3.x, vPoint3.y, vPoint3.z, 0.0
##                   )
##
##        self.mActiveVertexParameters.setNamedConstant("preRotatedQuad[0]", preRotatedQuad, 16) 

##        FloatArray16 = ctypes.c_float * 16 
##        preRotatedQuad = FloatArray16( 
##                vPoint0.x, vPoint0.y, vPoint0.z, 0.0, 
##                vPoint1.x, vPoint1.y, vPoint1.z, 0.0, 
##                vPoint2.x, vPoint2.y, vPoint2.z, 0.0, 
##                vPoint3.x, vPoint3.y, vPoint3.z, 0.0 
##                ) 
##        #self.mActiveVertexParameters.setNamedConstant("preRotatedQuad[0]", ctypes.addressof(preRotatedQuad), 16)
##        self.mActiveVertexParameters.setNamedConstant("preRotatedQuad[0]", preRotatedQuad, 16)
        # ------------------------------------- SPECIAL LEAF MESH STUFF ------------------------------------ #
        
        return sf.FrameListener.frameStarted(self, frameEvent)
    
    def doHarvesting(self):
        print 'Harvesting;'
##        pos = self.app.camera.getPosition()
##        pos = str((pos.x, pos.y, pos.z)) 
##        self.app.socket.send ( pos )
##        incomming = self.app.socket.recv ( 256 )
##        print "Received:", incomming
##        self.app.debugText = 'You Found a ' + incomming
##        self.showDebugOverlay(True)

    def _moveCamera(self):
        self.camera.yaw(self.rotationX)
        self.camera.pitch(self.rotationY)
        try:
            self.camera.translate(self.translateVector) # for using OgreRefApp
        except AttributeError:
            self.camera.moveRelative(self.translateVector)


class myRaySceneQueryListener ( ogre.RaySceneQueryListener ):
    def queryResult(self, fragment,  distance):
        self.resultDistance = distance
        return False

##class RenderToTextureFrameListener(ogre.FrameListener):
##
##    def __init__(self, renderWindow, camera, mReflectCam , planeSceneNode):
##    
##        ogre.FrameListener.__init__(self)
##        self.camera = camera
##        self.mReflectCam = mReflectCam       
##        self.planeNode = planeSceneNode
##
##    def frameStarted(self, frameEvent):
##        #result = sf.FrameListener.frameStarted(self,frameEvent)
##        #if not result: return result
##        # Make sure reflection camera is updated too
##        self.mReflectCam.setOrientation ( self.camera.getOrientation() )
##        self.mReflectCam.setPosition (self.camera.getPosition())
##        # Rotate plane
####        dd = ogre.Degree(d=(30.0 * frameEvent.timeSinceLastFrame))
####        self.planeNode.yaw(dd, ogre.Node.TS_PARENT)
##        
##        return True
    
    
##class VisualsListener(ogre.FrameListener):
##    def __init__(self, app):
##        ogre.FrameListener.__init__(self)
##        self.app = app
##        self.camera = app.camera
##        self.curFogSettings = [ app.sceneManager.getFogStart(),
##                                app.sceneManager.getFogEnd(),
##                                app.sceneManager.getFogDensity(),
##                                app.sceneManager.getFogColour()
##                                ]
##        self.nextFogSettings = []
##        self.transitionTime = 0.0
##        self.timer = 0.0
##        
##    def setNextFog(self, time, fogStart, fogEnd, fogDens, FogColour):
##        self.transitionTime = time
##        self.nextFogSettings = [ fogStart, fogEnd, fogDens, FogColour ]
##        self.timer = 0.0
##        
##    def frameStarted(self, frameEvent):
##        if self.camera.getPosition().y < WATER_LEVEL:
##            self.app.sceneManager.setFog(ogre.FOG_LINEAR, ogre.ColourValue(0.0,0.0,0.1),
##                                       0.001, 2, 50)
##        else:
##            self.app.sceneManager.setFog(ogre.FOG_LINEAR, self.app.caelumSystem.getSun().getSunLightColour(),
##                                       0.001, 1000, 2500)
##                                    
##        #print self.app.camera.getPosition()
##        return True
                                        
##        if self.timer < self.transitionTime:
##            # if nothing is happening they both equal 0
##            self.timer += frameEvent.timeSinceLastFrame
##            
##            if self.timer >= self.transitionTime:
##                self.timer = 0.0
##                self.transitionTime = 0.0
##            else:
##                pc = float(self.timer / self.transitionTime)
##                print pc
##                self.app.sceneManager.setFog(ogre.FOG_LINEAR, self.nextFogSettings[3] * pc,
##                                                self.nextFogSettings[2] * pc,
##                                                self.nextFogSettings[0] * pc,
##                                                self.nextFogSettings[1] * pc)

                
class soundListener(ogre.FrameListener):
    def __init__(self):
        ogre.FrameListener.__init__(self)


        self.sched = plib.slScheduler ( 8000 ) 
        self.mixer = plib.smMixer ( "/dev/mixer" ) 

        self.mixer.setMasterVolume ( 20 )
        self.sched.setSafetyMargin ( 0.128 )

        self.s1 = plib.slSample ( 'forest_day.wav', self.sched )


        #   s1.adjustVolume ( 10.0 )
        #print dir(sched)
        self.sched.loopSample ( self.s1 )

    def frameStarted(self, event):
        self.sched.update ()
        return True
          

if __name__ == '__main__':
    try:
        application = ForestApplication()
        application.go()
    except ogre.OgreException, e:
        print e
