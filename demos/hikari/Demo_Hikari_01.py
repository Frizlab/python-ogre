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
import sys
sys.path.insert(0,'..')
import PythonOgreConfig
import ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ogre.gui.hikari as hikari
import ogre.io.OIS as ois
import os

def TEST(caller, args):
    return True
    
class HikariApplication(sf.Application):
    def _createScene(self):
        
        sceneManager = self.sceneManager
        camera = self.camera
        
        sceneManager.ambientLight = ogre.ColourValue().White
        sceneManager.setShadowTechnique(ogre.SHADOWTYPE_TEXTURE_ADDITIVE)
        self.setupHikari()
        
    def setupHikari(self):
        self.hikariMgr = hikari.HikariManager(".\\media")
    
        self.fps = self.hikariMgr.createFlashOverlay("FPS", self.viewport, 130, 91, hikari.Position(hikari.TopLeft))
        self.fps.load("fps.swf")
        self.fps.setTransparent(True)
        self.fps.setDraggable(False)
        
        self.controls = self.hikariMgr.createFlashOverlay("Controls", self.viewport, 350, 400, hikari.Position(hikari.Center))
        self.controls.load("controls.swf")
        self.controls.setTransparent(True, True)

#         self.controls.bind("opacityChange", TEST,"" ) ## self, "onOpacityChange")
        self.controls.bind("opacityChange", self, "onOpacityChange")
        self.controls.bind("colorChange", self, "onColorChange")
        self.controls.bind("exitClick", self, "onExitClick")


    def onOpacityChange(self, caller, args):
        opacityPercent = args[0].getNumber()
        caller.setOpacity(opacityPercent / 100)
        return hikari.FlashValue()

    def onColorChange(self, caller, args):
        color = args[0].getNumberAsColor()
        self.viewport.setBackgroundColour(color)
        return hikari.FlashValue()
        
    def onExitClick(self, caller, args):
        self.frameListener.shouldQuit = True
        return hikari.FlashValue()
        
    def _createFrameListener(self):
        self.frameListener = HikariListener( self )
        self.root.addFrameListener(self.frameListener)

      
class HikariListener(sf.FrameListener, ois.MouseListener, ois.KeyListener):
    def __init__(self, app):
        print "INIT!!!"
        sf.FrameListener.__init__(self, app.renderWindow, app.camera, bufferedMouse=True, bufferedKeys = True)
        ois.KeyListener.__init__(self)
        ois.MouseListener.__init__(self)
        self.app = app
        self.Mouse.setEventCallback(self)
        self.MenuMode = True # stops the mouse moving the background
        self.Keyboard.setEventCallback(self)
        
    def mouseMoved(self,arg):
        state= arg.get_state()
        return self.app.hikariMgr.injectMouseMove(state.X.abs, state.Y.abs) or self.app.hikariMgr.injectMouseWheel(state.Z.rel)

    def mousePressed(self, arg, id):
        return self.app.hikariMgr.injectMouseDown(id)
    
    def mouseReleased(self, arg, id):
        return self.app.hikariMgr.injectMouseUp(id)
        
    def keyPressed( self,  arg):
        if self.app.hikariMgr.isAnyFocused():
            return True
        return False
        
    def keyReleased ( self, arg ):
        k = arg.key
        if k == ois.KC_ESCAPE:
            self.shouldQuit = True
        elif k == ois.KC_F2:
            self.app.sceneManager.getRootSceneNode().flipVisibility(True)
        return True
        
        
    def _inputSystemParameters (self ):
        """ the demo needs the system mouse
        """
        if os.name == 'nt':
            print "NT"
            return [("w32_mouse","DISCL_FOREGROUND"), ("w32_mouse", "DISCL_NONEXCLUSIVE")]
        else:
            return [("x11_mouse_grab","false"), ("x11_mouse_hide", "false")]  ## untested
        
        
    def frameRenderingQueued(self, frameEvent):              
        self.app.hikariMgr.update()
#         root.renderOneFrame()
#         Ogre::WindowEventUtilities::messagePump()
        
        stats = self.renderWindow.getStatistics()
        self.app.fps.callFunction("setFPS", hikari.Args(hikari.FlashValue(stats.lastFPS) ))
        return sf.FrameListener.frameRenderingQueued(self, frameEvent)

        
                              
if __name__ == '__main__':
    try:
        application = HikariApplication()
        application.go()
    except ogre.OgreException, e:
        print e
