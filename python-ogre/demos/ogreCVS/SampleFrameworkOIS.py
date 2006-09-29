# Thanks to the PyOgre project for this
#
# This code is in the Public Domain
import ogre
import ois 


class Application(object):
    "This class is the base for an Ogre application."
    def __init__(self):
        self.frameListener = None
        self.root = None
        self.camera = None
        self.renderWindow = None
        self.sceneManager = None

    def __del__(self):
        "Clear variables, this should not actually be needed."
        del self.camera
        del self.sceneManager
        del self.frameListener
        del self.root
        del self.renderWindow

    def go(self):
        "Starts the rendering loop."
        if not self._setUp():
            return
        if self._isPsycoEnabled():
            self._activatePsyco()
        self.root.startRendering()


    def _setUp(self):
        """This sets up the ogre application, and returns false if the user
        hits "cancel" in the dialog box."""
        self.root = ogre.Root(ogre.getPluginPath())

        self._setUpResources()
        if not self._configure():
            return False

        self._chooseSceneManager()
        self._createCamera()
        self._createViewports()

        ogre.TextureManager.getSingleton().defaultNumMipmaps = 5

        self._createResourceListener()
        self._loadResources()

        self._createScene()
        self._createFrameListener()
        return True

    def _setUpResources(self):
        """This sets up Ogre's resources, which are required to be in
        resources.cfg."""
        config = ogre.ConfigFile()
        config.loadFromFile('resources.cfg' )
        for section, key, path in config.values:
            ogre.ResourceGroupManager.getSingleton().addResourceLocation(path, key, section)

    def _createResourceListener(self):
        """This method is here if you want to add a resource listener to check
        the status of resources loading."""
        pass

    def _loadResources(self):
        """This loads all initial resources.  Redefine this if you do not want
        to load all resources at startup."""
        ogre.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()

    def _configure(self):
        """This shows the config dialog and creates the renderWindow."""
        carryOn = self.root.showConfigDialog()
        if carryOn:
            self.renderWindow = self.root.initialise(True)
        return carryOn

    def _chooseSceneManager(self):
        """Chooses a default SceneManager."""
        self.sceneManager = self.root.createManager(ogre.ST_GENERIC,"Test")

    def _createCamera(self):
        """Creates the camera."""
        self.camera = self.sceneManager.createCamera('PlayerCam')
        self.camera.position = (0, 0, 500)
        self.camera.lookAt((0, 0, -300))
        self.camera.nearClipDistance = 5

    def _createViewports(self):
        """Creates the Viewport."""
        self.viewport = self.renderWindow.addViewport(self.camera)
        self.viewport.backgroundColour = (0,0,0)

    def _createScene(self):
        """Creates the scene.  Override this with initial scene contents."""
        pass

    def _createFrameListener(self):
        """Creates the FrameListener."""
        self.frameListener = FrameListener(self.renderWindow, self.camera)
        self.frameListener.showDebugOverlay(True)
        self.root.addFrameListener(self.frameListener)

    def _isPsycoEnabled(self):
        """Override this function and return True to turn on Psyco"""
        return False

    def _activatePsyco(self):
       """Import Psyco if available"""
       try:
           import psyco
           psyco.full()
       except ImportError:
           pass


class FrameListener(ogre.CombinedListener):
    """A default frame listener, which takes care of basic mouse and keyboard
    input."""
    def __init__(self, renderWindow, camera):
        ogre.CombinedListener.__init__(self)
        self.camera = camera
        self.renderWindow = renderWindow
        self.statisticsOn = True
        self.numScreenShots = 0
        self.timeUntilNextToggle = 0
        self.sceneDetailIndex = 0
        self.moveScale = 0.0
        self.rotationScale = 0.0
        self.translateVector = ogre.Vector3(0.0,0.0,0.0)
        self.filtering = ogre.TFO_BILINEAR
        self.showDebugOverlay(True)
        self.moveSpeed = 100.0
        self.rotationSpeed = 8.0

        self._setupInput()

    def _setupInput(self):
        import sys
        hwnd = ois.IntPtr()
        if sys.platform == 'win32':
            self.renderWindow.getCustomAttribute("HWND", hwnd)
        elif sys.platform.startswith('linux'):
            self.renderWindow.getCustomAttribute('GLXWINDOW', hwnd)
        else:
            raise Exception, 'Platform not supported by OIS'
        print "The Hwnd Value is ",	str(hwnd.value())
        print "My Hwnd Value is",self.renderWindow.getIntCustomAttribute("HWND")	
        print "The Hwnd Str Value is ",	str(hwnd.value())
        print "My Hwnd Value is",str(self.renderWindow.getIntCustomAttribute("HWND"))	
		
        #if sys.platform == 'win32':
        #    hwnd=self.renderWindow.getIntCustomAttribute("HWND")
        #elif sys.platform.startswith('linux'):
        #    hwnd=self.renderWindow.getIntCustomAttribute('GLXWINDOW')
        #else:
        #    raise Exception, 'Platform not supported by OIS'

        oisParams = {
            'WINDOW': str(self.renderWindow.getIntCustomAttribute("HWND")),
            'w32_mouse': ['DISCL_FOREGROUND', 'DISCL_EXCLUSIVE'],
        }
        print "created Window" 		
        self.inputManager = ois.InputManager.createInputSystem(oisParams)
        print "created input Manager" 		
        self.keyboard = self.inputManager.createInputObject(ois.OISKeyboard, False)
        print "created Keyboard" 		
        self.mouse = self.inputManager.createInputObject(ois.OISMouse, False)
        print "created mouse" 		

    def frameStarted(self, frameEvent):
        #print "trying to capture mouse" 		
        self.keyboard.capture()
        #print "captured keyboard" 		
        self.mouse.capture()
        #print "captured mouse" 		
        if self.timeUntilNextToggle >= 0:
            self.timeUntilNextToggle -= frameEvent.timeSinceLastFrame

        if frameEvent.timeSinceLastFrame == 0:
            self.moveScale = 1
            self.rotationScale = 0.1
        else:
            self.moveScale = self.moveSpeed * frameEvent.timeSinceLastFrame
            self.rotationScale = self.rotationSpeed * frameEvent.timeSinceLastFrame

        self.rotationX = ogre.Degree(0.0)
        self.rotationY = ogre.Degree(0.0)
        self.translateVector = ogre.Vector3(0.0, 0.0, 0.0)

        if not self._processUnbufferedKeyInput(frameEvent):
            return False

        self._processUnbufferedMouseInput(frameEvent)

        self._moveCamera()
        return True

    def frameEnded(self, frameEvent):
        self._updateStatistics()
        return True

    def showDebugOverlay(self, show):
        """Turns the debug overlay (frame statistics) on or off."""
        overlay = ogre.OverlayManager.getSingleton().getByName('Core/DebugOverlay')
        if overlay is None:
            raise ogre.Exception(111, "Could not find overlay Core/DebugOverlay", "SampleFramework.py")
        if show:
            overlay.show()
        else:
            overlay.hide()

    def _processUnbufferedKeyInput(self, frameEvent):
        if self.keyboard.isKeyDown(ois.KC_A):
            self.translateVector.x = -self.moveScale

        if self.keyboard.isKeyDown(ois.KC_D):
            self.translateVector.x = self.moveScale

        if self.keyboard.isKeyDown(ois.KC_UP) or self.keyboard.isKeyDown(ois.KC_W):
            self.translateVector.z = -self.moveScale

        if self.keyboard.isKeyDown(ois.KC_DOWN) or self.keyboard.isKeyDown(ois.KC_S):
            self.translateVector.z = self.moveScale

        if self.keyboard.isKeyDown(ois.KC_PGUP):
            self.translateVector.y = self.moveScale

        if self.keyboard.isKeyDown(ois.KC_PGDOWN):
            self.translateVector.y = - self.moveScale

        if self.keyboard.isKeyDown(ois.KC_RIGHT):
            self.rotationX = - self.rotationScale

        if self.keyboard.isKeyDown(ois.KC_LEFT):
            self.rotationX = self.rotationScale

        if self._isToggleKeyDown(ois.KC_F):
            self.statisticsOn = not self.statisticsOn
            self.showDebugOverlay(self.statisticsOn)

        if self._isToggleKeyDown(ois.KC_T):
            if self.filtering == ogre.TFO_BILINEAR:
                self.filtering = ogre.TFO_TRILINEAR
                ogre.MaterialManager.getSingleton().defaultAnisotropy = 1
            elif self.filtering == ogre.TFO_TRILINEAR:
                self.filtering = ogre.TFO_ANISOTROPIC
                ogre.MaterialManager.getSingleton().defaultAnisotropy = 8
            else:
                self.filtering = ogre.TFO_BILINEAR
                ogre.MaterialManager.getSingleton().defaultAnisotropy = 1

            ogre.MaterialManager.getSingleton().setDefaultTextureFiltering(self.filtering)
            self.showDebugOverlay(self.statisticsOn)

        if self._isToggleKeyDown(ois.KC_SYSRQ, 0.5):
            path = 'screenshot_%d.png' % self.numScreenShots
            self.numScreenShots += 1
            self.renderWindow.writeContentsToFile(path)
            self.renderWindow.debugText = 'screenshot taken: ' + path

        if self._isToggleKeyDown(ois.KC_R, 0.5):
            detailsLevel = [ ogre.SDL_SOLID,
                             ogre.SDL_WIREFRAME,
                             ogre.SDL_POINTS ]
            self.sceneDetailIndex = (self.sceneDetailIndex + 1) % len(detailsLevel)
            self.camera.detailLevel = detailsLevel[self.sceneDetailIndex]

        if self.keyboard.isKeyDown(ois.KC_ESCAPE):
            return False

        return True

    def _isToggleKeyDown(self, keyCode, toggleTime = 1.0):
        if self.keyboard.isKeyDown(keyCode)and self.timeUntilNextToggle <=0:
            self.timeUntilNextToggle = toggleTime
            return True
        return False

    def _processUnbufferedMouseInput(self, frameEvent):
        state = self.mouse.state
        if state.buttonDown(ois.MB_Right):
            self.translateVector.x += state.relX * 0.13
            self.translateVector.y -= state.relY * 0.13
        else:
            self.rotationX = ogre.Degree(- state.relX * 0.13)
            self.rotationY = ogre.Degree(- state.relY * 0.13)

        if state.relZ > 0:
            self.translateVector.z = - self.moveScale * 8.0
        if state.relZ < 0:
            self.translateVector.z = self.moveScale * 8.0

    def _moveCamera(self):
        self.camera.yaw(self.rotationX)
        self.camera.pitch(self.rotationY)
        self.camera.moveRelative(self.translateVector)

    def _updateStatistics(self):
        statistics = self.renderWindow
        self._setGuiCaption('Core/AverageFps', 'Average FPS: %f' % statistics.averageFPS)
        self._setGuiCaption('Core/CurrFps', 'Current FPS: %f' % statistics.lastFPS)
        self._setGuiCaption('Core/BestFps',
                             'Best FPS: %f %d ms' % (statistics.bestFPS, statistics.bestFrameTime))
        self._setGuiCaption('Core/WorstFps',
                             'Worst FPS: %f %d ms' % (statistics.worstFPS, statistics.worstFrameTime))
        self._setGuiCaption('Core/NumTris', 'Triangle Count: %d' % statistics.triangleCount)
        self._setGuiCaption('Core/DebugText', self.renderWindow.debugText)

    def _setGuiCaption(self, elementName, text):
        element = ogre.OverlayManager.getSingleton().getOverlayElement(elementName, False)
        element.caption = text