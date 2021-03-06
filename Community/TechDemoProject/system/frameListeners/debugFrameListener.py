#-----------------------------------------------------------------------------#
#                                                                             #
#   This source code is part of the python-ogre techdemo project.             #
#                                                                             #
#   This program is released as public domain                                 #
#                                                                             #
#-----------------------------------------------------------------------------#
#   
#   TITLE: DebugFrameListener
#   DESCRIPTION: Input testing listener

# Set this through a level config file.
# eg: FRAME_LISTENER=debugFrameListener

import ogre.renderer.OGRE as ogre
import ogre.io.OIS as OIS
import logging

class Event:
    def __init__(self, time, name, function, *args):
        self.time = time
        self.name = name
        self.function = function
        self.args = args
    def __cmp__(self, other):
        return cmp(self.time, other.time)
    def __repr__(self):
        return ('Event Name = ' + self.name + ' - Event Time: ' + str(self.time))

class DebugFrameListener(ogre.FrameListener, OIS.KeyListener, OIS.MouseListener):
    # this framelistener does not deal with application level things
    # like shutdown etc. It is simply an input reader, in this case
    # it directly moves the camera, however, check the playerListener as
    # well, as this directs user input to the player actor, instead of 
    # directly affecting the camera

    def __init__(self, renderWindow, camera, sceneManager, player=None):

        ogre.FrameListener.__init__(self)
        OIS.KeyListener.__init__(self)
        OIS.MouseListener.__init__(self)
        self.renderWindow = renderWindow
        self.camera = camera
        self.moveScale = 1500.00
        self.fps = 60.0
        self.translateVector = ogre.Vector3(0.0, 0.0, 0.0)
        self.showframerate = True
        self.showDebugOverlay(True)
        self.steering = ogre.Degree(0.0)
        self.pitch = ogre.Degree(0.0)
        self.filtering = ogre.TFO_BILINEAR
        self.shadowType = ogre.SHADOWTYPE_NONE
        self.sceneManager = sceneManager
        self.setupInput()
        self.numScreenShots = 0
        self.Mouse.setEventCallback(self)
        self.Keyboard.setEventCallback(self)
        self.Joy = None
        
        self.events = [] # for the event Listener
        
        logging.debug('DebugFrameListener setup Complete::')
        
    def setupInput(self):
         windowHnd = self.renderWindow.getCustomAttributeInt("WINDOW")
         self.InputManager = \
             OIS.createPythonInputSystem([("WINDOW",str(windowHnd))])
            
         self.Keyboard = self.InputManager.createInputObjectKeyboard( OIS.OISKeyboard, True )
         self.Mouse = self.InputManager.createInputObjectMouse( OIS.OISMouse, True )
        
    def __del__(self):
##        ogre.WindowEventUtilities.removeWindowEventListener(self.renderWindow, self)
        self.windowClosed(self.renderWindow)
        print 'DebugFrameListener Deleted'
        
    def windowClosed(self, rw):
        #Only close for window that created OIS (mWindow)
        if rw is self.renderWindow:
            if self.InputManager :
                self.InputManager.destroyInputObjectMouse( self.Mouse )
                self.InputManager.destroyInputObjectKeyboard( self.Keyboard )
                if self.Joy:
                    self.InputManager.destroyInputObjectJoyStick( self.Joy )
                OIS.InputManager.destroyInputSystem(self.InputManager)
                self.InputManager=None
        print 'WindowClosedEvent'
        
        

    ## Tell the frame listener to exit at the end of the next frame
    def requestShutdown( self ):
        self.ShutdownRequested = True
        
        
    def frameStarted(self, evt):
        if(self.renderWindow.isClosed()):
            return False
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
    
    def updateStatistics(self):
        statistics = self.renderWindow
        self._setGuiCaption('Core/AverageFps', 'Average FPS: %f' % statistics.getAverageFPS())
        self._setGuiCaption('Core/CurrFps', 'Current FPS: %f' % statistics.getLastFPS())
        self._setGuiCaption('Core/BestFps',
                             'Best FPS: %f %d ms' % (statistics.getBestFPS(), statistics.getBestFrameTime()))
##        self._setGuiCaption('Core/WorstFps',
##                             'Worst FPS: %f %d ms' % (statistics.getWorstFPS(), statistics.getWorstFrameTime()))
##        self._setGuiCaption('Core/NumTris', 'Triangle Count: %d' % statistics.getTriangleCount())
##        self._setGuiCaption('Core/DebugText', Application.debugText)

    def _setGuiCaption(self, elementName, text):
        element = ogre.OverlayManager.getSingleton().getOverlayElement(elementName, False)
        element.setCaption(ogre.UTFString(text))
        

    def frameEnded(self, frameEvent):
        self.updateStatistics()
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

        if arg.key == OIS.KC_H:
            if self.shadowType == ogre.SHADOWTYPE_NONE:
                self.shadowType = ogre.SHADOWTYPE_STENCIL_MODULATIVE
                
            elif self.shadowType == ogre.SHADOWTYPE_STENCIL_MODULATIVE:
                self.shadowType = ogre.SHADOWTYPE_TEXTURE_MODULATIVE
                
            elif self.shadowType == ogre.SHADOWTYPE_TEXTURE_MODULATIVE:
                self.shadowType = ogre.SHADOWTYPE_TEXTURE_ADDITIVE
                
            else: 
                self.shadowType = ogre.SHADOWTYPE_NONE
            self.sceneManager.shadowTechnique = self.shadowType
                
            ogre.MaterialManager.getSingleton().setDefaultTextureFiltering(self.filtering)
            
        if arg.key == OIS.KC_P:
            path = 'screenshot_%d.png' % self.numScreenShots
            self.numScreenShots += 1
            self.renderWindow.writeContentsToFile(path)
            
        if arg.key == OIS.KC_F:
            self.showframerate = not self.showframerate
            self.showDebugOverlay(self.showframerate)
            
        if arg.key == OIS.KC_M:
            self.events.append(Event(1, 'DebugPrintEvent', 'event_LogEverything', None))
        return True

    ##----------------------------------------------------------------##
    def keyReleased( self, arg ):
        if arg.key == OIS.KC_W or arg.key == OIS.KC_S:
            self.translateVector.z = 0.0        
        elif arg.key == OIS.KC_A or arg.key == OIS.KC_D:
            self.translateVector.x = 0.0
        return True
    
class SkyFrameListener(DebugFrameListener):
    def __init__(self, renderWindow, camera, sceneManager, skyManager):
        DebugFrameListener.__init__(self, renderWindow, camera, sceneManager)
        self.skyManager = skyManager
        self.sky_yaw = ogre.Degree(0).valueRadians()
        self.sky_pitch = ogre.Degree(0).valueRadians()
        self.movingSky = False
        
    def requestShutdown( self ):
        self.ShutdownRequested = True
        
    def frameStarted(self, evt):
        if(self.renderWindow.isClosed()):
            return False
        self.time = evt.timeSinceLastFrame
        self.Keyboard.capture()    
        self.Mouse.capture()
        
        if not self.sky_pitch == 0.0:
            if not self.movingSky:
                if self.sky_pitch < 0.0:
                    self.sky_pitch= min(0.0, self.sky_pitch + 0.3)
                else:
                    self.sky_pitch= max(0.0, self.sky_pitch - 0.3)
        self.skyManager.mTime += self.sky_pitch
##        
##        for star in self.skyManager.mStarList:
##            q = ogre.Quaternion(self.sky_yaw, ogre.Vector3.UNIT_Z)
##            star.rotAxis = q * star.rotAxis
        return True

        
    def frameEnded(self, evt):
        return True
        
    def mouseMoved( self, arg ):
        return True
##        if self.movingSky:
##            self.sky_yaw = ogre.Degree(- arg.get_state().X.rel * 0.5).valueRadians()
##            self.sky_pitch = ogre.Degree(- arg.get_state().Y.rel * 0.5).valueRadians()
##        return True

    ##----------------------------------------------------------------##
    def mousePressed(  self, arg, id ):
        return True

    ##----------------------------------------------------------------##
    def mouseReleased( self, arg, id ):
        return True
        
    ##----------------------------------------------------------------##
    def keyPressed( self, arg ):
##        if arg.key == OIS.KC_LCONTROL:
##            self.movingSky = True
##        return True
        if arg.key == OIS.KC_I:
            self.sky_yaw -= ogre.Degree(5.0).valueRadians()
            self.movingSky = True
        elif arg.key == OIS.KC_K:
            self.sky_yaw += ogre.Degree(5.0).valueRadians()
            self.movingSky = True
        elif arg.key == OIS.KC_J:
            self.sky_pitch -= ogre.Degree(5.0).valueRadians()
            self.movingSky = True
        elif arg.key == OIS.KC_L:
            self.sky_pitch += ogre.Degree(5.0).valueRadians()
            self.movingSky = True
        
        # test world events:
        elif arg.key == OIS.KC_N:
            self.events.append( Event(4, 'SkyDebugEvent', 'testWorldEvent', 20, 'testValue') )
            
        return True

    ##----------------------------------------------------------------##
    def keyReleased( self, arg ):
##        print 'sky movement off', str(self.sky_pitch)
        self.movingSky = False
        return True
    
        
    
    
