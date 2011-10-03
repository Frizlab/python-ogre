## /*
## -----------------------------------------------------------------------------
## This source file is based on Caelum's CaelumDemo
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

class CaelumSampleFrameListener(sf.FrameListener):

    def __init__(self, win, cam, app ):
        sf.FrameListener.__init__(self, win, cam)
        self.sceneManager = cam.getSceneManager()
        self.window = win
        self.camera = cam
        self.paused=False
        self.app = app
        
        # Pick components to create in the demo.
        # You can comment any of those and it should still work
        # Trying to disable one of these can be useful in finding problems.
        componentMask = caelum.CaelumSystem.CaelumComponent (
                caelum.CaelumSystem.CAELUM_COMPONENT_SUN |				
                caelum.CaelumSystem.CAELUM_COMPONENT_MOON |
                caelum.CaelumSystem.CAELUM_COMPONENT_SKY_DOME |
                ## caelum.CaelumSystem.CAELUM_COMPONENT_IMAGE_STARFIELD |
                caelum.CaelumSystem.CAELUM_COMPONENT_POINT_STARFIELD |
                caelum.CaelumSystem.CAELUM_COMPONENT_CLOUDS |
                0);
        componentMask = caelum.CaelumSystem.CAELUM_COMPONENTS_DEFAULT

        ## Initialise CaelumSystem.
        self.mCaelumSystem = caelum.CaelumSystem (ogre.Root.getSingletonPtr(), self.sceneManager, componentMask)

        ## Set time acceleration.
        self.mCaelumSystem.getUniversalClock ().setTimeScale (512)

        ## Register caelum as a listener.
        self.app.renderWindow.addListener (self.mCaelumSystem)
        ogre.Root.getSingletonPtr().addFrameListener (self.mCaelumSystem)

        self.UpdateSpeedFactor(self.mCaelumSystem.getUniversalClock ().getTimeScale ())
        self.timeTillNextUpdate = 0

    ## Update speed factors.
    def UpdateSpeedFactor(self, factor):
        self.speedFactor = factor;
        if self.paused:
            self.mCaelumSystem.getUniversalClock ().setTimeScale (0)
        else:            
            self.mCaelumSystem.getUniversalClock ().setTimeScale (self.speedFactor)
            
    
    def frameEnded(self, evt):
        if not sf.FrameListener.frameEnded(self, evt):
            return False

        ## Stop key repeat for these keys.
        ## keyboard input
        self.timeTillNextUpdate -= evt.timeSinceLastFrame
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
        return True

        
class CaelumSampleApplication(sf.Application):
    def _isPsycoEnabled(self):
        return True

    def _createFrameListener ( self ):
        self.FrameListener = CaelumSampleFrameListener (self.renderWindow, self.camera, self)
        self.root.addFrameListener (self.FrameListener)

    def _chooseSceneManager( self ):
        self.sceneManager = self.root.createSceneManager("TerrainSceneManager")

    def _createCamera( self ):
        ## Create the camera
        self.camera = self.sceneManager.createCamera("PlayerCam")

        ## Start the camera on a hill in the middle of the terrain
        ## looking towards Z+ (north).
        ## Sun should rise in the east(left) and set in the west(right).
        self.camera.setPosition (ogre.Vector3 (775, 100, 997))
        self.camera.lookAt (ogre.Vector3 (775, 100, 1000))

        self.camera.setNearClipDistance(5)

        ## Set camera clip distances. It's important to test with
        ## an infinite clip distance.
        self.camera.setFarClipDistance(0)
        ##mCamera.setFarClipDistance(10000);
   
        
    def _createScene ( self ):
        ## Put some terrain in the scene
        self.sceneManager.setWorldGeometry ("CaelumDemoTerrain.cfg")
        self.sceneManager.setAmbientLight ( ogre.ColourValue(0,0,0 ))
        
       
        
if __name__ == '__main__':
    try:
        application = CaelumSampleApplication()
        application.go()

    except ogre.OgreException, e:
        print e
