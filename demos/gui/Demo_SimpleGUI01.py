# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http://python-ogre.org/
#
# It is likely based on original code from OGRE and/or PyOgre
# For the latest info, see http://www.ogre3d.org/
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------
import ogre.renderer.OGRE as ogre
import ogre.io.OIS as ois
import ogre.gui.SimpleGUI as gui
import SampleFramework as sf


class CallBack ( gui.MemberFunctionSlot ):
    """ Callback class for user events in SimpleGUI"""
    def __init__(self, function=""):
        gui.MemberFunctionSlot.__init__(self)
        self.function=function
    def execute(self, args):
        if self.function:
            self.function(args)
                
class CameraTrackApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        
        sceneManager.setAmbientLight ( (0.7, 0.7, 0.7) )
#         sceneManager.setSkyBox (True, "Examples/CloudySky") ## AJM THIS FAILS FOR SOME REASON?
        sceneManager.setSkyDome (True, "Examples/CloudySky")
        
        light = sceneManager.createLight('MainLight')
        light.setPosition (20, 80, 50)

        self.GUIManager = gui.GUIManager(self.renderWindow.getWidth(),self.renderWindow.getHeight())
        

        plane = ogre.Plane()
        plane.normal = ogre.Vector3.UNIT_Y
        plane.d = 200
        mm = ogre.MeshManager.getSingleton()
        mm.createPlane('FloorPlane', 'General', plane, 200000.0, 200000.0,
               20, 20, True, 1, 200.0, 200.0,  (0, 0, 1),
               ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, 
               True,True )


        entity = sceneManager.createEntity('floor', 'FloorPlane')
        entity.setMaterialName('Examples/RustySteel')
        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(entity)

        # create head entity
        headNode = sceneManager.getRootSceneNode().createChildSceneNode()
        entity = sceneManager.createEntity('head', 'ogrehead.mesh')
        headNode.attachObject(entity)

        #print type(entity[0])
        # make sure the camera track this node
        self.camera.setAutoTracking(True, headNode)

        # create the camera node & attach camera
        cameraNode = sceneManager.getRootSceneNode().createChildSceneNode()
        cameraNode.attachObject(self.camera)
        
        print "\n\n",self.camera,"\n"
#
        # set up spline animation of node
        animation = sceneManager.createAnimation('CameraTrack', 10)
        animation.interpolationMode = ogre.Animation.IM_SPLINE
        
        animationTrack = animation.createNodeTrack(0, cameraNode)
        
        key = animationTrack.createNodeKeyFrame(0)
        
        key = animationTrack.createNodeKeyFrame(2.5)
        key.setTranslate ( (500.0, 500.0, -1000.0))
        
        key = animationTrack.createNodeKeyFrame(5)
        key.setTranslate (  (-1500.0, 1000.0, -600.0))
        
        key = animationTrack.createNodeKeyFrame(7.5)
        key.setTranslate ( (0.0, -100.0, 0.0))
        
        key = animationTrack.createNodeKeyFrame(10.0)
        key.setTranslate (  (0.0, 0.0, 0.0))

        self.animationState = sceneManager.createAnimationState('CameraTrack')
        self.animationState.setEnabled (True)
    
        # add some fog
        sceneManager.setFog(ogre.FOG_EXP, (1, 1, 1), 0.0002)
        
        self.createGUI()

                
    def createGUI(self):
        self.GUIManager.createMouseCursor((0.05,0.05),"sgui.pointer")
        w = self.GUIManager.createWindow([0,0,0.3,0.2],"sgui.window")
        w.setText("Sample Text!")
        b = w.createButton((0.25,0.3,0.5,0.4),"sgui.button")
        b.setText("Exit!")
        
        self.cb1 = CallBack()
        self.cb1.function = self.my_eventHandler
        b.addEventHandler(gui.Widget.Event.SGUI_MOUSE_BUTTON_UP,self.cb1)

    def my_eventHandler(self, args):
        ## check if left mouse button is down
        if(args.button == gui.MouseButtonID.MB_Left):
            self.GuiFrameListener.requestShutdown()
        return True     
        
    def _createFrameListener(self):
#         self.frameListener = CameraTrackListener(self.renderWindow, self.camera, self.animationState)
#         self.root.addFrameListener(self.frameListener)
        self.GuiFrameListener= GuiFrameListener(self.renderWindow, self.camera)
        self.root.addFrameListener(self.GuiFrameListener)
        
    def handleQuit(self,  e):
        self.GuiFrameListener.requestShutdown()
        return True

class CameraTrackListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, animationState):
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.animationState = animationState

    def frameStarted(self, frameEvent):
        #MyTest #Crash    
        self.animationState.addTime(frameEvent.timeSinceLastFrame)
        return sf.FrameListener.frameStarted(self, frameEvent)

class GuiFrameListener ( sf.FrameListener, ois.MouseListener, ois.KeyListener ):
    ## NB using buffered input, this is the only change
    def __init__( self, win,  cam):
        sf.FrameListener.__init__(self,win, cam, True, True, True) 
        ois.KeyListener.__init__(self)
        ois.MouseListener.__init__(self)
        
        self.ShutdownRequested = False
        self.Mouse.setEventCallback(self)
        self.Keyboard.setEventCallback(self)

    ## Tell the frame listener to exit at the end of the next frame
    def requestShutdown(self):
        self.ShutdownRequested = True

    def frameEnded(self, evt):
        if (self.ShutdownRequested):
            return False
        else:
            return sf.FrameListener.frameEnded(self,evt)
    ##----------------------------------------------------------------##
    def mouseMoved( self, arg ):
        ms= arg.get_state()
        gui.GUIManager.getSingleton().injectMouseMove( ms.X.rel, ms.Y.rel )
        return True

    ##----------------------------------------------------------------##
    def mousePressed( self, arg,  _id ):
        b = gui.MouseButtonID.values[_id]
        gui.GUIManager.getSingleton().injectMouseButtonDown(b)
        return True

    ##----------------------------------------------------------------##
    def mouseReleased( self, arg, _id ):
        b = gui.MouseButtonID.values[_id]
        gui.GUIManager.getSingleton().injectMouseButtonUp(b)
        return True

    ##----------------------------------------------------------------##
    def keyPressed( self, arg ):
        if( arg.key == ois.KC_ESCAPE ):
            self.ShutdownRequested = True
        ## Now convert from OIS keycode to SimpleGUI one..            
        k = gui.KeyCode.values[arg.key]
        gui.GUIManager.getSingleton().injectKeyDown( k )
        gui.GUIManager.getSingleton().injectChar( chr(arg.text) )
        return True

    ##----------------------------------------------------------------##
    def keyReleased( self, arg ):
        k = gui.KeyCode.values[arg.key]
        gui.GUIManager.getSingleton().injectKeyDown( k )
        return True

if __name__ == '__main__':
    try:
        application = CameraTrackApplication()
        application.go()
    except ogre.OgreException, e:
        print e

    
