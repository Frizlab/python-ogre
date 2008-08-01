## /*
## -----------------------------------------------------------------------------
## This source file is based on Caelum's CaelumTest1
## -----------------------------------------------------------------------------
## */
import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.io.OIS as ois
import ogre.addons.caelum as caelum
import SampleFramework as sf
import ctypes

## ------------------------------------------------------------------ 
## our basic Python Ogre TerrainFrame Listener
## ------------------------------------------------------------------ 
class TerrainFrameListener(sf.FrameListener):

    def __init__(self, renderWindow, camera, sceneManager):
        sf.FrameListener.__init__(self, renderWindow, camera)

        self.sceneManager = sceneManager
        self.camera = camera
        self.moveSpeed = 50.0
        self.updateRay = ogre.Ray() 
        ## basic terrain ray query
        pos = camera.getPosition()
        d = ogre.Vector3.NEGATIVE_UNIT_Y
        self.raySceneQuery = sceneManager.createRayQuery(ogre.Ray( pos, d ))

    ## -------------------------------------------------------------- 
    def frameStarted(self, frameEvent):

        ## call parent framelistener
        ret =  sf.FrameListener.frameStarted(self, frameEvent)

        ## clamp camera to terrain
        self.updateRay.setOrigin (self.camera.getPosition())
        self.updateRay.setDirection (ogre.Vector3.NEGATIVE_UNIT_Y)
        self.raySceneQuery.Ray = self.updateRay
        for queryResult in self.raySceneQuery.execute():
            if queryResult.worldFragment is not None:
#                 pos = self.camera.getPosition()
#                 self.camera.setPosition (pos.x, pos.y - queryResult.distance + 10.0, pos.z)
#                 break
                wf = queryResult.worldFragment
                self.camera.setPosition(self.camera.getPosition().x,
                    wf.singleIntersection.y + 10,
                    self.camera.getPosition().z)
                break               
        return ret




class CaelumSpeedFrameListener(TerrainFrameListener):
    def __init__(self, renderWindow, camera, sceneManager, caelumSystem):
        TerrainFrameListener.__init__(self, renderWindow, camera, sceneManager)

        self.caelumSystem = caelumSystem
        self.speedFactor = 0
        self.paused = False
        self.timeTillNextUpdate = 0
        self.PostFiltering = False
        
        self.UpdateSpeedFactor(self.caelumSystem.getUniversalClock ().getTimeScale ())
    
    ## ------------------------------------------------------------------ 
    ## our own function to set time scale dependent on paused state
    ## saves a few if / else
    ## ------------------------------------------------------------------ 
#     def _setTimeScale(self, paused, t1, t2 ):
#         t = t1
#         if not paused: 
#             t = t2
#         self.caelumSystem.getUniversalClock().setTimeScale(t)
    
    ## Update speed factors.
    def UpdateSpeedFactor(self, factor):
        self.speedFactor = factor
        if self.paused:
            t = 0
        else:
            t= self.speedFactor
        self.caelumSystem.getUniversalClock ().setTimeScale (t)
        ogre.ControllerManager.getSingleton().setTimeFactor (t)

    ## ------------------------------------------------------------------ 
    def frameStarted(self, frameEvent):

        # clamp to terrain
        ret =  TerrainFrameListener.frameStarted(self, frameEvent)
        print "from framestarted", ret
        return ret
        ## keyboard input
        self.timeTillNextUpdate -= frameEvent.timeSinceLastFrame
        if (self.timeTillNextUpdate<= 0):
            if (self.Keyboard.isKeyDown (ois.KC_SPACE)):
                self.timeTillNextUpdate = 1
                self.paused = not self.paused
                self.UpdateSpeedFactor(self.speedFactor)

            if (self.Keyboard.isKeyDown(ois.KC_X)):
                self.timeTillNextUpdate = 0.25
                self.UpdateSpeedFactor(self.speedFactor/2)

            if (self.Keyboard.isKeyDown(ois.KC_C)):
                self.timeTillNextUpdate = 0.25
                self.UpdateSpeedFactor(self.speedFactor*2)

            if (self.Keyboard.isKeyDown(ois.KC_Z)):
                self.timeTillNextUpdate = 1
                self.UpdateSpeedFactor(self.speedFactor * -1)

            if (self.Keyboard.isKeyDown(ois.KC_F1)):
                self.timeTillNextUpdate = 1
                if self.PostFiltering:
                    self.PostFiltering = False
                else:
                    self.PostFiltering = True
                ogre.CompositorManager.getSingleton().setCompositorEnabled (self.window.getViewport (0), "Bloom", self.PostFiltering)

        ## Do some additional update. These should be removed from here;
        ## CaelumSystem should do all required updates.

        ## Update the haze sun light position
        if (self.caelumSystem.getSun ()):
            mat = ogre.MaterialManager.getSingleton ().getByName ("CaelumDemoTerrain")
            if mat.getTechnique (0).getPass ("CaelumHaze"):
                mat.getTechnique (0).getPass ("CaelumHaze").getVertexProgramParameters ().setNamedConstant ("sunDirection", 
                                                                                        self.caelumSystem.getSun ().getLightDirection ())
        return ret

class TerrainApplication(sf.Application):

    def __init__(self):
        
        self.caelumSystem = None   ## Caelum system
        self.caelumModel = None    ## Caelum model
        self.spm = None
        self.raySceneQuery = None

        sf.Application.__init__(self)


    def __del__(self):

        print "Remove Window Listener" 
        self.renderWindow.removeListener(self.caelumSystem)
        self.root.removeFrameListener(self.frameListener)
        self.frameListener = None
        print "deleted Listeners" 
#         t = self.caelumSystem.getSun().setSunPositionModel(None)
#         print "Sun is None", t
        t = self.caelumSystem.getSun().getSunPositionModel()
        print "Sun is None", t


 
        ## ====================================================================
        ## This the one difference between the python and c++ demo. In c++ the
        ## Sun dtor deletes the SunPositionalModel. However in createScene we
        ## need to hold a reference to SunPositionModel (self.spm) other
        ## seg faults when it goes out of scope. Doing so means that a seg fault
        ## when C++ tries to delete the sun.EarthSunPositionModel. Temp workaround
        ## comment out the following in Sun.cpp and handle deletion ouselves.
        ## 
        ##    /*
        ##  if (mSunPositionModel)  {
        ##      delete mSunPositionModel
        ##      mSunPositionModel = 0
        ##  }
        ##    */
        ## 
        ## ====================================================================
        if self.spm:
            self.spm = 0
        if self.caelumSystem:
            self.caelumSystem.shutdown(False)
            self.caelumSystem = None

        del self.camera
        del self.sceneManager
        del self.frameListener
        del self.root
        del self.renderWindow


    def _chooseSceneManager(self):
        self.sceneManager = self.root.createSceneManager("TerrainSceneManager")

    def _createCamera(self):
        self.camera = self.sceneManager.createCamera('PlayerCam')
        
        ## Start the camera on a hill in the middle of the terrain
        ## looking towards Z+ (north).
        ## Sun should rise in the east(left) and set in the west(right).
        self.camera.setPosition (ogre.Vector3(775, 100, 997))
        self.camera.lookAt (ogre.Vector3(775, 100, 1000))
        
        ## Set camera clip distances. Its important to test with
        ## an infinite clip distance.
        self.camera.setFarClipDistance(0);
        self.camera.setNearClipDistance(5);
        
        self.raySceneQuery = self.sceneManager.createRayQuery(ogre.Ray(self.camera.getPosition(), ogre.Vector3.NEGATIVE_UNIT_Y))
        

    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera
        clouds = True
        skyDome = True
        
        ## a little hack to let me test this on a machine with a crap graphics card
        capabilities = ogre.Root.getSingleton().getRenderSystem().capabilities
        if not capabilities.hasCapability(ogre.RSC_VERTEX_PROGRAM) or\
                    not capabilities.hasCapability(ogre.RSC_FRAGMENT_PROGRAM):
            skyDome = False
            clouds = False
        else:
            ogre.CompositorManager.getSingleton().addCompositor(self.renderWindow.getViewport(0),"Bloom")
        camera.setNearClipDistance(0.01)
        componentMask = caelum.CaelumSystem.CaelumComponent(
                    caelum.CaelumSystem.CAELUM_COMPONENT_SKY_COLOUR_MODEL | 
#                     caelum.CaelumSystem.CAELUM_COMPONENT_SUN |
                    caelum.CaelumSystem.CAELUM_COMPONENT_SOLAR_SYSTEM_MODEL |
                    ## these cause run time errors on my crap laptop
#                     caelum.CaelumSystem.CAELUM_COMPONENT_SKY_DOME | 
#                     caelum.CaelumSystem.CAELUM_COMPONENT_STARFIELD |
#                     caelum.CaelumSystem.CAELUM_COMPONENT_CLOUDS |
#                     caelum.CaelumSystem.CAELUM_COMPONENT_GROUND_FOG |
                    0 )
        print componentMask
        ## Initialise Caelum
        self.caelumSystem = caelum.CaelumSystem(self.root.getSingletonPtr(), self.sceneManager ) ##,componentMask )
                
        self.caelumSystem.setManageSceneFog(True)
        self.caelumSystem.setSceneFogDensityMultiplier(0.0015)
        self.caelumSystem.setManageAmbientLight (True)
        self.caelumSystem.setMinimumAmbientLight (ogre.ColourValue (0, 0, 0.5))
         
        ## This is how you switch the sun implementation.
        ## This here is a no-op; but it's useful to test caelum doesn't crash when the second sun is created.
        self.spheresun = caelum.SphereSun(self.sceneManager, self.caelumSystem.getRootNode ())
        self.caelumSystem.setSun (self.spheresun)

        ## Setup sun options
        if self.caelumSystem.getSun ():
            self.caelumSystem.getSun ().setAmbientMultiplier (ogre.ColourValue(0.5, 0.5, 0.5))
            self.caelumSystem.getSun ().setDiffuseMultiplier (ogre.ColourValue(3, 3, 2.7))
            self.caelumSystem.getSun ().setSpecularMultiplier (ogre.ColourValue (5, 5, 5))
            self.caelumSystem.getSun ().setAutoDisable (True)
            self.caelumSystem.getSun ().setAutoDisableThreshold (0.1)

        if (self.caelumSystem.getMoon ()):
            self.caelumSystem.getMoon ().setAutoDisable (True)
            self.caelumSystem.getMoon ().setAutoDisableThreshold (0.1)

#         ## Setup cloud options.
#         ## Tweak these settings to make the demo look pretty.
        if self.caelumSystem.getClouds ():
           self.caelumSystem.getClouds ().setCloudSpeed(ogre.Vector2(0.000005, -0.000009))
           self.caelumSystem.getClouds ().setCloudBlendTime(3600 * 24)
           self.caelumSystem.getClouds ().setCloudCover(0.3)
           
  
        ## Register caelum to the render target
        self.renderWindow.addListener (self.caelumSystem)
        ogre.Root.getSingletonPtr().addFrameListener( self.caelumSystem )
        
        ## Put some terrain in the scene
        self.sceneManager.setWorldGeometry ("CaelumDemoTerrain.cfg")

    def _createFrameListener(self):
    #    pass
    #    return
        return
        self.frameListener = CaelumSpeedFrameListener(self.renderWindow, self.camera,self.sceneManager, self.caelumSystem)
#         self.root.addFrameListener(self.frameListener)

if __name__ == '__main__':
    try:
        application = TerrainApplication()
        application.go()

    except ogre.OgreException, e:
        print e
