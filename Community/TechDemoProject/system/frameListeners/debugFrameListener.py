# debugFrameListener

# Set this through a level config file.
# eg: FRAME_LISTENER=debugFrameListener

import ogre.renderer.OGRE as ogre
import ogre.io.OIS as OIS

class DebugFrameListener(ogre.FrameListener, OIS.KeyListener, OIS.MouseListener):
    # this framelistener does not deal with application level things
    # like shutdown etc. It is simply an input reader, in this case
    # it directly moves the camera, however, check the playerListener as
    # well, as this directs user input to the player actor, instead of 
    # directly affecting the camera

    def __init__(self, renderWindow, camera, player=None):

        ogre.FrameListener.__init__(self)
        OIS.KeyListener.__init__(self)
        OIS.MouseListener.__init__(self)
        self.renderWindow = renderWindow
        self.camera = camera
        self.moveScale = 1500.00
        self.fps = 60.0
        self.translateVector = ogre.Vector3(0.0, 0.0, 0.0)
        self.steering = ogre.Degree(0.0)
        self.pitch = ogre.Degree(0.0)
        self.setupInput()
        
        self.Mouse.setEventCallback(self)
        self.Keyboard.setEventCallback(self)
        print 'DebugFrameListener setup Complete::'
        
    def setupInput(self):
         windowHnd = self.renderWindow.getCustomAttributeInt("WINDOW")
         self.InputManager = \
             OIS.createPythonInputSystem([("WINDOW",str(windowHnd))])
            
         self.Keyboard = self.InputManager.createInputObjectKeyboard( OIS.OISKeyboard, True )
         self.Mouse = self.InputManager.createInputObjectMouse( OIS.OISMouse, True )

    ## Tell the frame listener to exit at the end of the next frame
    def requestShutdown( self ):
        self.ShutdownRequested = True
        
    def frameStarted(self, evt):
##        if(self.renderWindow.isClosed()):
##            return False
        self.steering = ogre.Degree(0.0)
        self.pitch = ogre.Degree(0.0)
        #self.translateVector = ogre.Vector3(0.0, 0.0, 0.0)
        
        self.time = evt.timeSinceLastFrame
        
        self.Keyboard.capture()    
        self.Mouse.capture()
        
        
        self.camera.yaw(self.steering)
        self.camera.pitch(self.pitch)
        self.camera.moveRelative(self.translateVector)
        return True
        

    def frameEnded(self, evt):
        return True
        
    def mouseMoved( self, arg ):
        self.steering = ogre.Degree(- arg.get_state().X.rel * 0.13)
        self.pitch = ogre.Degree(- arg.get_state().Y.rel * 0.13)
        return True

    ##----------------------------------------------------------------##
    def mousePressed(  self, arg, id ):
        return True

    ##----------------------------------------------------------------##
    def mouseReleased( self, arg, id ):
        return True
        
    ##----------------------------------------------------------------##
    def keyPressed( self, arg ):
        if arg.key == OIS.KC_W:
            self.translateVector.z = -self.moveScale * (self.time / self.fps)
        elif arg.key == OIS.KC_S:
            self.translateVector.z = self.moveScale * (self.time / self.fps)
        elif arg.key == OIS.KC_A:
            self.translateVector.x = -self.moveScale * (self.time / self.fps)
        elif arg.key == OIS.KC_D:
            self.translateVector.x = self.moveScale * (self.time / self.fps)
            
        return True

    ##----------------------------------------------------------------##
    def keyReleased( self, arg ):
        if arg.key == OIS.KC_W or arg.key == OIS.KC_S:
            self.translateVector.z = 0.0        
        elif arg.key == OIS.KC_A or arg.key == OIS.KC_D:
            self.translateVector.x = 0.0
        return True
