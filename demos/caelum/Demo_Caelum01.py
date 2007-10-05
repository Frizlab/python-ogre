## /*
## -----------------------------------------------------------------------------
## This source file is based on Caelum's CaelumTest1
## -----------------------------------------------------------------------------
## */

import ogre.renderer.OGRE as ogre
import ogre.io.OIS as ois
import ogre.addons.caelum as caelum
import SampleFramework as sf


## ------------------------------------------------------------------ 
## our basic Python Ogre TerrainFrame Listener
## ------------------------------------------------------------------ 
class TerrainFrameListener(sf.FrameListener):

    def __init__(self, renderWindow, camera, sceneManager):
        sf.FrameListener.__init__(self, renderWindow, camera)

        self.sceneManager = sceneManager
        self.camera = camera
        self.moveSpeed = 50.0

        ## basic terrain ray query
        pos = camera.getPosition()
        d = ogre.Vector3.NEGATIVE_UNIT_Y
        self.raySceneQuery = sceneManager.createRayQuery(ogre.Ray( pos, d ))

    ## -------------------------------------------------------------- 
    def frameStarted(self, frameEvent):

        ## call parent framelistener
        ret =  sf.FrameListener.frameStarted(self, frameEvent)

        ## clamp camera to terrain
        updateRay = ogre.Ray()
        updateRay.setOrigin (self.camera.getPosition())
        updateRay.setDirection (ogre.Vector3.NEGATIVE_UNIT_Y)
        self.raySceneQuery.Ray = updateRay
        for queryResult in self.raySceneQuery.execute():
            if queryResult.worldFragment is not None:
                pos = self.camera.getPosition()
                self.camera.setPosition (pos.x, pos.y - queryResult.distance + 10.0, pos.z)
                break
##            mCamera->setPosition(mCamera->getPosition().x,
##                i->worldFragment->singleIntersection.y + 10,
##                mCamera->getPosition().z);

        return ret




class CaelumSpeedFrameListener(TerrainFrameListener):
    def __init__(self, renderWindow, camera, sceneManager, caelumSystem):
        TerrainFrameListener.__init__(self, renderWindow, camera, sceneManager)

        self.caelumSystem = caelumSystem
        self.speedFactor = caelumSystem.getUniversalClock().getTimeScale()
        self.paused = False
        self.timeTillNextUpdate = 0.0
        self.PostFiltering = False

    ## ------------------------------------------------------------------ 
    ## our own function to set time scale dependent on paused state
    ## saves a few if / else
    ## ------------------------------------------------------------------ 
    def _setTimeScale(self, paused, t1, t2 ):
        t = t1
        if not paused: 
            t = t2
        self.caelumSystem.getUniversalClock().setTimeScale(t)


    ## ------------------------------------------------------------------ 
    def frameStarted(self, frameEvent):

        # clamp to terrain
        ret =  TerrainFrameListener.frameStarted(self, frameEvent)

        t = self.caelumSystem.getUniversalClock().getCurrentRelativeTime()
        self.caelumSystem.getSun().getSunPositionModel().setJulianDate(t)

        self.timeTillNextUpdate-= frameEvent.timeSinceLastFrame

        ## keyboard input
        if (self.timeTillNextUpdate<= 0):
            if (self.Keyboard.isKeyDown (ois.KC_SPACE)):

                self.timeTillNextUpdate = 1
                self._setTimeScale(self.paused, self.speedFactor, 0)
                self.paused = not self.paused

            if (self.Keyboard.isKeyDown(ois.KC_X)):

                self.timeTillNextUpdate = 0.25
                self.speedFactor *= 0.5
                self._setTimeScale(self.paused, 0, self.speedFactor)

            if (self.Keyboard.isKeyDown(ois.KC_C)):

                self.timeTillNextUpdate = 0.25
                self.speedFactor *= 2
                self._setTimeScale(self.paused, 0, self.speedFactor)

            if (self.Keyboard.isKeyDown(ois.KC_Z)):

                self.timeTillNextUpdate = 1
                self.speedFactor *= -1
                self._setTimeScale(self.paused, 0, self.speedFactor)

            if (self.Keyboard.isKeyDown(ois.KC_F1)):
                self.timeTillNextUpdate = 1
                if self.PostFiltering:
                    self.PostFiltering = False
                else:
                    self.PostFiltering = True

            ## EXTRA! Update the haze sun light position
            mat =  ogre.MaterialManager.getSingleton().getByName("CaelumDemoTerrain")
            mat.getTechnique(0).getPass("Haze").getVertexProgramParameters().setNamedConstant("sunDirection", self.caelumSystem.getSun().getSunDirection())

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

 
        ## ====================================================================
        ## This the one difference between the python and c++ demo. In c++ the
        ## Sun dtor deletes the SunPositionalModel. However in createScene we
        ## need to hold a reference to SunPositionModel (self.spm) other
        ## seg faults when it goes out of scope. Doing so means that a seg fault
        ## when C++ tries to delete the sun->EarthSunPositionModel. Temp workaround
        ## comment out the following in Sun.cpp and handle deletion ouselves.
        ## 
        ##    /*
        ##	if (mSunPositionModel)  {
        ##		delete mSunPositionModel;
        ##		mSunPositionModel = 0;
        ##	}
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
        self.camera.setPosition (ogre.Vector3(1100,1000,1000))
        self.raySceneQuery = self.sceneManager.createRayQuery(ogre.Ray(self.camera.getPosition(), ogre.Vector3.NEGATIVE_UNIT_Y))


    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        ogre.CompositorManager.getSingleton().addCompositor(self.renderWindow.getViewport(0),"Bloom")
        camera.setNearClipDistance(0.01)

        ## Initialise Caelum
        self.caelumSystem = caelum.CaelumSystem(self.root, self.sceneManager)
        self.spm = caelum.EarthSunPositionModel(ogre.Radian(ogre.Degree(37)), 0.5)

        ## Setup lighting parameters.
        self.caelumSystem.getSun().setAmbientMultiplier(ogre.ColourValue(0.5, 0.5, 0.5))
        self.caelumSystem.getSun().setDiffuseMultiplier(ogre.ColourValue(3, 3, 2.7))
        ## For green terrain:
        ## self.caelumSystem.getSun().setDiffuseMultiplier(ogre.ColourValue(0.1, 3, 0.1))

        self.caelumSystem.getSun().setSpecularMultiplier(ogre.ColourValue(5, 5, 5))
        self.caelumSystem.getSun().setSunPositionModel(self.spm)
        self.caelumSystem.getSun().setManageAmbientLight(True)


        ## Create and configure the sky colours model to use
        self.caelumModel = caelum.StoredImageElvBasedSkyColourModel()
        ## Call this before changing the gradients image!!
        self.caelumSystem.setSkyColourModel(self.caelumModel)	

        self.caelumModel.setSkyGradientsImage("EarthClearSky2.png")
        self.caelumSystem.setManageFog(True)
        self.caelumModel.setFogDensity(0.0015)

        ## Create a starfield
        self.caelumSystem.getStarfield().setInclination(ogre.Degree (13))
        self.caelumSystem.createStarfield("Starfield.jpg")

        ## Register all to the render window
        self.renderWindow.addListener(self.caelumSystem)

        ## Set some time parameters
        #t = time(&t);
        #struct tm *t2 = localtime (&t);

        self.caelumSystem.getUniversalClock().setTimeScale(512)
        ## Jan 1st, 5am
        self.caelumSystem.getUniversalClock().setCurrentTime(18000) 

        ## Put some terrain in the scene
        terrain_cfg = "terrain.cfg"
          
        #if ogre.OGRE_PLATFORM == ogre.OGRE_PLATFORM_APPLE:
        #    terrain_cfg = resourcePath + terrain_cfg
        self.sceneManager.setWorldGeometry( terrain_cfg )
        ##self.sceneManager.setAmbientLight(ogre.ColourValue.Black)


    def _createFrameListener(self):
    #    pass
    #    return
        self.frameListener = CaelumSpeedFrameListener(self.renderWindow, self.camera,self.sceneManager, self.caelumSystem)
        self.root.addFrameListener(self.frameListener)

if __name__ == '__main__':
    try:
        application = TerrainApplication()
        application.go()

    except ogre.OgreException, e:
        print e
