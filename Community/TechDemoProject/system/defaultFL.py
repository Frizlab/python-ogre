#-----------------------------------------------------------------------------#
#                                                                             #
#   This source code is part of the python-ogre techdemo project.             #
#                                                                             #
#   This program is released as public domain                                 #
#                                                                             #
#-----------------------------------------------------------------------------#
#   
#   TITLE: Default Frame listener
#   DESCRIPTION: Application close event capture

import ogre.renderer.OGRE as ogre
import ogre.io.OIS as OIS

class DefaultFrameListener(ogre.FrameListener, ogre.WindowEventListener):
    """A default frame listener, which takes care of basic mouse and keyboard
    input."""
      
    def __init__(self, renderWindow, camera, datamanager):
        ogre.FrameListener.__init__(self)
        ogre.WindowEventListener.__init__(self)
        self.camera = camera
        self.renderWindow = renderWindow
        self.datamanager = datamanager
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
        self.displayCameraDetails = False
        self.MenuMode = False   # lets understand a simple menu function
        ## we can tell if we are using OgreRefapp based upon the camera class
        self.framerate = 1.0 / 30.0
        if self.camera.__class__ == ogre.Camera:
            self.RefAppEnable = False
        else:
            self.RefAppEnable = True
        self._setupInput()
        self.lastFrameTime = 0.0
        self.clock = 0.0
        self.events = []
        
    def __del__ (self ):
        print 'Deleting FrameListener'
        ogre.WindowEventUtilities.removeWindowEventListener(self.renderWindow, self)
        print 'Sending Window Closed'
        self.windowClosed(self.renderWindow)


        
        
    def windowClosed(self, rw):
        print 'Closing Window'
        del self.datamanager
        #Only close for window that created OIS (mWindow)
        if( rw == self.renderWindow ):
            if( self.InputManager ):
                self.InputManager.destroyInputObjectMouse( self.Mouse )
                self.InputManager.destroyInputObjectKeyboard( self.Keyboard )
                if self.Joy:
                    self.InputManager.destroyInputObjectJoyStick( self.Joy )
                OIS.InputManager.destroyInputSystem(self.InputManager)
                self.InputManager=None
      
    def _setupInput(self):
        # ignore buffered input
        
        windowHnd = self.renderWindow.getCustomAttributeInt("WINDOW")
        self.InputManager = \
            OIS.createPythonInputSystem([("WINDOW",str(windowHnd))])
         
        #pl = OIS.ParamList()
        #windowHndStr = str ( windowHnd)
        #pl.insert("WINDOW", windowHndStr)
        #im = OIS.InputManager.createInputSystem( pl )
         
        #Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
        self.Keyboard = self.InputManager.createInputObjectKeyboard( OIS.OISKeyboard, False )
        self.Mouse = self.InputManager.createInputObjectMouse( OIS.OISMouse, False )
        try :
            self.Joy = self.InputManager.createInputObjectJoyStick( OIS.OISJoyStick, False )
        except:
            self.Joy = False
         
        #Set initial mouse clipping size
        self.windowResized(self.renderWindow)
         
        self.showDebugOverlay(True)
         
        #Register as a Window listener
        ogre.WindowEventUtilities.addWindowEventListener(self.renderWindow, self);
         
    def setMenuMode(self, mode):
        self.MenuMode = mode
        
    def _UpdateSimulation( self, frameEvent ):
        # create a real version of this to update the simulation
        pass 
           
    def windowResized (self, rw):
        [width, height, depth, left, top] = rw.getMetrics()  # Note the wrapped function as default needs unsigned int's
        ms = self.Mouse.getMouseState()
        ms.width = width
        ms.height = height
         
    def windowClosed(self, rw):
        #Only close for window that created OIS (mWindow)
        if( rw == self.renderWindow ):
            if( self.InputManager ):
                self.InputManager.destroyInputObjectMouse( self.Mouse )
                self.InputManager.destroyInputObjectKeyboard( self.Keyboard )
            if self.Joy:
                self.InputManager.destroyInputObjectJoyStick( self.Joy )
            OIS.InputManager.destroyInputSystem(self.InputManager)
            self.InputManager=None
            
    def frameStarted(self, frameEvent):
        if(self.renderWindow.isClosed()):
            return False
        
        self.clock += frameEvent.timeSinceLastFrame
        
        ##Need to capture/update each device - this will also trigger any listeners
        self.Keyboard.capture()    
        self.Mouse.capture()
        if( self.Joy ):
            self.Joy.capture()
    
        ##bool buffJ = (mJoy) ? mJoy->buffered() : true;
    
        if self.timeUntilNextToggle >= 0:
            self.timeUntilNextToggle -= frameEvent.timeSinceLastFrame
            
        diff = self.clock - self.lastFrameTime
        if diff > self.framerate:
            self.datamanager.update(frameEvent.timeSinceLastFrame)
            self.lastFrameTime = self.clock
        
        if not self._processUnbufferedKeyInput(frameEvent):
            return False
        
        if not self.MenuMode:   # if we are in Menu mode we don't move the camera..
            self._processUnbufferedMouseInput(frameEvent)
        
        # Perform simulation step only if using OgreRefApp.  For simplicity create a function that simply does
        ###  "OgreRefApp.World.getSingleton().simulationStep(frameEvent.timeSinceLastFrame)"
        
        if  self.RefAppEnable:
            self._UpdateSimulation( frameEvent )
        return True

    def frameEnded(self, frameEvent):
        return True

    def showDebugOverlay(self, show):
        pass

    def _processUnbufferedKeyInput(self, frameEvent):
        if self.Keyboard.isKeyDown(OIS.KC_ESCAPE) or self.Keyboard.isKeyDown(OIS.KC_Q):
            del self.datamanager
            return False
        return True        
        
    def _isToggleKeyDown(self, keyCode, toggleTime = 1.0):
        if self.Keyboard.isKeyDown(keyCode)and self.timeUntilNextToggle <=0:
            self.timeUntilNextToggle = toggleTime
            return True
        return False
        
    def _isToggleMouseDown(self, Button, toggleTime = 1.0): 
        ms = self.Mouse.getMouseState() 
        if ms.buttonDown( Button ) and self.timeUntilNextToggle <=0: 
            self.timeUntilNextToggle = toggleTime 
            return True 
        return False 

    def _processUnbufferedMouseInput(self, frameEvent):
        ms = self.Mouse.getMouseState()
        if ms.buttonDown( OIS.MB_Right ):
            self.translateVector.x += ms.X.rel * 0.13
            self.translateVector.y -= ms.Y.rel * 0.13
        else:
            self.rotationX = ogre.Degree(- ms.X.rel * 0.13)
            self.rotationY = ogre.Degree(- ms.Y.rel * 0.13)

    def _moveCamera(self):
        self.camera.yaw(self.rotationX)
        self.camera.pitch(self.rotationY)
        try:
            self.camera.translate(self.translateVector) # for using OgreRefApp
        except AttributeError:
            self.camera.moveRelative(self.translateVector)

    def _updateStatistics(self):
        statistics = self.renderWindow
        self._setGuiCaption('Core/AverageFps', 'Average FPS: %f' % statistics.getAverageFPS())
        self._setGuiCaption('Core/CurrFps', 'Current FPS: %f' % statistics.getLastFPS())
        self._setGuiCaption('Core/BestFps',
                             'Best FPS: %f %d ms' % (statistics.getBestFPS(), statistics.getBestFrameTime()))
        self._setGuiCaption('Core/WorstFps',
                             'Worst FPS: %f %d ms' % (statistics.getWorstFPS(), statistics.getWorstFrameTime()))
        self._setGuiCaption('Core/NumTris', 'Triangle Count: %d' % statistics.getTriangleCount())
        self._setGuiCaption('Core/DebugText', Application.debugText)

    def _setGuiCaption(self, elementName, text):
        element = ogre.OverlayManager.getSingleton().getOverlayElement(elementName, False)
        ##d=ogre.UTFString("hell0")
        ##element.setCaption(d)
        
        #element.caption="hello"
        
        #element.setCaption("help")
        element.setCaption(ogre.UTFString(text))