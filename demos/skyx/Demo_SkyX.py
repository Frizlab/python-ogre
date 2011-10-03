# SkyX Demo 1: Basic clouds and terrain

# ---------------------------------------------------------------------------
# Include SkyX header files
# ---------------------------------------------------------------------------
#include <SkyX/SkyX.h>

import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ogre.addons.skyx as skyx
import ogre.io.OIS as OIS


# ----------------------------------------------------------------------------
# Misc
# ----------------------------------------------------------------------------
mShowInformation = False

def getConfigStringFromSkyXAtmosphereOptions( Options ) :
    hour = int(Options.Time.x)
    min  = int((Options.Time.x - hour)*60)

    timeStr = str(hour) + ":" + str(min)
    str_ = "SkyX Plugin demo (Press F1 to show/hide information)"
    if mShowInformation:
        str_ +=" - Simuation paused - \n"
    else:
        str_ + "\n-------------------------------------------------------------\nTime: " + timeStr + "\n"

    if (mShowInformation) :
        str_ += "-------------------------------------------------------------\n"
        str_ += "Time: " + timeStr + " [1, Shift+1] (+/-).\n"
        str_ += "Rayleigh multiplier: " + str(Options.RayleighMultiplier) + " [2, Shift+2] (+/-).\n"
        str_ += "Mie multiplier: " + str(Options.MieMultiplier) + " [3, Shift+3] (+/-).\n"
        str_ += "Exposure: " + str(Options.Exposure) + " [4, Shift+4] (+/-).\n"
        str_ += "Inner radius: " + str(Options.InnerRadius) + " [5, Shift+5] (+/-).\n"
        str_ += "Outer radius: " + str(Options.OuterRadius) + " [6, Shift+6] (+/-).\n"
        str_ += "Number of samples: " + str(Options.NumberOfSamples) + " [7, Shift+7] (+/-).\n"
        str_ += "Height position: " + str(Options.HeightPosition) + " [8, Shift+8] (+/-).\n"

    return str_

# ----------------------------------------------------------------------------
# Define the application object
# This is derived from ExampleApplication which is the class OGRE provides to
# make it easier to set up OGRE without rewriting the same code all the time.
# You can override extra methods of ExampleApplication if you want to further
# specialise the setup routine, otherwise the only mandatory override is the
# 'createScene' method which is where you set up your own personal scene.
# ----------------------------------------------------------------------------
class SkyXDemoListener(sf.FrameListener):
    def __init__ ( self, win, cam, sm, app):
        global mShowInformation
        sf.FrameListener.__init__(self, win, cam)
        self.sceneManager = sm
        self.keyBuffer = -1
        self.SkyX = app.SkyX
        self.textArea = app.textArea
        mShowInformation=False

    def frameStarted(self, frameEvent):
        global mShowInformation
        #self.keyboard.capture()

        # Check camera height
        raySceneQuery = self.sceneManager.\
                 createRayQuery(ogre.Ray(self.camera.getPosition() + ogre.Vector3(0,1000000,0),
                                ogre.Vector3().NEGATIVE_UNIT_Y))

        for queryResult in raySceneQuery.execute():
            if queryResult.worldFragment is not None:
                if (self.camera.getDerivedPosition().y < queryResult.worldFragment.singleIntersection.y + 30):
                    self.camera.setPosition(self.camera.getPosition().x,
                            queryResult.worldFragment.singleIntersection.y + 30,
                            self.camera.getPosition().z)
            
                
        SkyXOptions = self.SkyX.getAtmosphereManager().getOptions()

        # Time
        if (not mShowInformation):
            self.SkyX.setTimeMultiplier(.1) #0.1)
        else:
            self.SkyX.setTimeMultiplier(0.0)

        if (self.Keyboard.isKeyDown(OIS.KC_1) and not (self.Keyboard.isKeyDown(OIS.KC_LSHIFT) or self.Keyboard.isKeyDown(OIS.KC_RSHIFT))):
            self.SkyX.setTimeMultiplier(1.0)
        if (self.Keyboard.isKeyDown(OIS.KC_1) and  (self.Keyboard.isKeyDown(OIS.KC_LSHIFT) or self.Keyboard.isKeyDown(OIS.KC_RSHIFT))):
            self.SkyX.setTimeMultiplier(-1.0)

        # # Rayleigh multiplier
        # if (self.Keyboard.isKeyDown(OIS.KC_2) and !(self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
            # SkyXOptions.RayleighMultiplier += e.timeSinceLastFrame*0.025f
        # if (self.Keyboard.isKeyDown(OIS.KC_2) and  (self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
            # SkyXOptions.RayleighMultiplier -= e.timeSinceLastFrame*0.025f

        # # Mie multiplier
        # if (self.Keyboard.isKeyDown(OIS.KC_3) and !(self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
            # SkyXOptions.MieMultiplier += e.timeSinceLastFrame*0.025f
        # if (self.Keyboard.isKeyDown(OIS.KC_3) and  (self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
            # SkyXOptions.MieMultiplier -= e.timeSinceLastFrame*0.025f

        # # Exposure
        # if (self.Keyboard.isKeyDown(OIS.KC_4) and !(self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
            # SkyXOptions.Exposure += e.timeSinceLastFrame*0.5f
        # if (self.Keyboard.isKeyDown(OIS.KC_4) and  (self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
            # SkyXOptions.Exposure -= e.timeSinceLastFrame*0.5f

        # # Inner radius
        # if (self.Keyboard.isKeyDown(OIS.KC_5) and !(self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
            # SkyXOptions.InnerRadius += e.timeSinceLastFrame*0.25f
        # if (self.Keyboard.isKeyDown(OIS.KC_5) and  (self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
            # SkyXOptions.InnerRadius -= e.timeSinceLastFrame*0.25f

        # # Outer radius
        # if (self.Keyboard.isKeyDown(OIS.KC_6) and !(self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
            # SkyXOptions.OuterRadius += e.timeSinceLastFrame*0.25f
        # if (self.Keyboard.isKeyDown(OIS.KC_6) and  (self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
            # SkyXOptions.OuterRadius -= e.timeSinceLastFrame*0.25f

        # # Number of samples
        # if (self.Keyboard.isKeyDown(OIS.KC_7) and self.keyBuffer < 0 and!(self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
        # {
            # SkyXOptions.NumberOfSamples ++
            # self.keyBuffer = 0.25f
        # }
        # if (self.Keyboard.isKeyDown(OIS.KC_7) and self.keyBuffer < 0 and (self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
        # {
            # SkyXOptions.NumberOfSamples --
            # self.keyBuffer = 0.25f
        # }

        # # Height position
        # if (self.Keyboard.isKeyDown(OIS.KC_8) and !(self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
            # SkyXOptions.HeightPosition += e.timeSinceLastFrame*0.05f
        # if (self.Keyboard.isKeyDown(OIS.KC_8) and  (self.Keyboard.isKeyDown(OIS.KC_LSHIFT) || self.Keyboard.isKeyDown(OIS.KC_RSHIFT)))
            # SkyXOptions.HeightPosition -= e.timeSinceLastFrame*0.05f

        # self.SkyX.getAtmosphereManager().setOptions(SkyXOptions)

        # # Show/Hide information
        if (self.Keyboard.isKeyDown(OIS.KC_F1) and self.keyBuffer < 0):
            if not mShowInformation:
                mShowInformation=True
            else:
                mShowInformation=False

            self.keyBuffer = 0.25

        self.textArea.setCaption(getConfigStringFromSkyXAtmosphereOptions(SkyXOptions))

        self.SkyX.update(frameEvent.timeSinceLastFrame)

        self.keyBuffer -= frameEvent.timeSinceLastFrame

        # Update terrain material
        ogre.MaterialManager.getSingleton().getByName("Terrain").getTechnique(0).getPass(0)\
            .getFragmentProgramParameters().setNamedConstant("uLightY", -self.SkyX.getAtmosphereManager().\
            getSunDirection().y)

        return True


class SkyxApplication(sf.Application):
    def __del__ ( self ):
        self.SkyX.remove()  # need to remove from the scene before deleting everything else
        sf.Application.__del__ ( self )

    # Just override the mandatory create scene method
    def _createScene(self):
        # Set some camera params
        self.camera.setFarClipDistance(30000)
        self.camera.setNearClipDistance(20)
        self.camera.setPosition(20000, 500, 20000)
        self.camera.setDirection(1, 0, 0)

        # Create our text area for display SkyX parameters
        self.createTextArea()

        print "****************"
        print self.sceneManager
        print self.camera
        print "****************"
        # Create SkyX
        self.SkyX = skyx.SkyX(self.sceneManager, self.camera)
        self.SkyX.create()

        # Add our ground atmospheric scattering pass to terrain material
        self.SkyX.getGPUManager().addGroundPass(
            ogre.MaterialManager.getSingleton().getByName("Terrain").\
                getTechnique(0).createPass(), 5000, ogre.SBT_TRANSPARENT_COLOUR)

        # Create our terrain
        self.sceneManager.setWorldGeometry("Terrain.cfg")

        # Add a basic cloud layer
        self.SkyX.getCloudsManager().add(skyx.CloudLayer.Options())

    def _createFrameListener( self ):
        self.frameListener = SkyXDemoListener(self.renderWindow, self.camera, self.sceneManager, self)
        self.root.addFrameListener(self.frameListener)

    def _chooseSceneManager( self ):
        # Create the SceneManager, in this case terrain scene manager
#        self.sceneManager = self.root.createSceneManager("TerrainSceneManager")
#        self.sceneManager = self.root.createSceneManager(ogre.ST_GENERIC)
        self.sceneManager = self.root.createSceneManager(ogre.ST_EXTERIOR_CLOSE)

    # Create text area for SkyX parameters
    def createTextArea( self ):
        # Create a panel
        panel = ogre.OverlayManager.getSingleton().createOverlayElement("Panel", "SkyXParametersPanel")
        panel.setMetricsMode(ogre.GMM_PIXELS)
        panel.setPosition(10, 10)
        panel.setDimensions(400, 400)

        # Create a text area
        self.textArea = ogre.OverlayManager.getSingleton().createOverlayElement("TextArea", "SkyXParametersTextArea")
        self.textArea.setMetricsMode(ogre.GMM_PIXELS)
        self.textArea.setPosition(0, 0)
        self.textArea.setDimensions(100, 100)
        self.textArea.setCaption("SkyX plugin demo")
        self.textArea.setCharHeight(16)
        self.textArea.setFontName("BlueHighway")
        self.textArea.setColourBottom(ogre.ColourValue(0.3, 0.5, 0.3))
        self.textArea.setColourTop(ogre.ColourValue(0.5, 0.7, 0.5))

        # Create an overlay, and add the panel
        overlay = ogre.OverlayManager.getSingleton().create("OverlayName")
        overlay.add2D(panel)

        # Add the text area to the panel
        panel.addChild(self.textArea)

        # Show the overlay
        overlay.show()

# ----------------------------------------------------------------------------
# Main function, just boots the application object
# ----------------------------------------------------------------------------
if __name__ == '__main__':
    import exceptions,sys
    try:
        application = SkyxApplication()
        application.go()
    except ogre.OgreException, e:
        print e

