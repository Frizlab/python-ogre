#/***************************************************************************
# *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
# *
# *   Permission is hereby granted, free of charge, to any person obtaining
# *   a copy of this software and associated documentation files (the
# *   "Software"), to deal in the Software without restriction, including
# *   without limitation the rights to use, copy, modify, merge, publish,
# *   distribute, sublicense, and/or sell copies of the Software, and to
# *   permit persons to whom the Software is furnished to do so, subject to
# *   the following conditions:
# *
# *   The above copyright notice and this permission notice shall be
# *   included in all copies or substantial portions of the Software.
# *
# *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# *   OTHER DEALINGS IN THE SOFTWARE.
# ***************************************************************************/


import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.io.OIS as OIS
import ogre.gui.CEGUI as CEGUI
import SampleFramework

from CEGUI_framework import *


def createUVector2( x, y):
    return CEGUI.UVector2(CEGUI.UDim(x,0), CEGUI.UDim(y,0))

# Handler for the 'Demo -> New dialog' menu item
def demoNewDialog(args):
    # add a dialog to this pane so we have some more stuff to drag around :)
    dlg = CEGUI.WindowManager.getSingletonPtr().createWindow("WindowsLook/FrameWindow")
    dlg.setMinSize(CEGUI.UVector2(CEGUI.UDim(0,200),CEGUI.UDim(0,100)))
    dlg.setText("Drag me around too!")
    
    # we put this in the center of the viewport into the scrollable pane
    uni_center = CEGUI.UVector2( CEGUI.UDim(0.5,0), CEGUI.UDim(0.5,0) )
    d_root = CEGUI.WindowManager.getSingletonPtr().getWindow("RootWindow")
    d_pane = CEGUI.WindowManager.getSingletonPtr().getWindow("DialogPane")

    center = CEGUI.CoordConverter.windowToScreen(d_root, uni_center)
    target = CEGUI.CoordConverter.screenToWindow(d_pane.getContentPane(), center)
    dlg.setPosition(CEGUI.UVector2( CEGUI.UDim(0,target.d_x-100),
                                    CEGUI.UDim(0,target.d_y-50)))
    
    d_pane.addChildWindow(dlg)
    
    return True

#Handler for global hotkeys
def hotkeysHandler(e):
    k = e
    if k.scancode ==  CEGUI.Key.Space:
        # space is a hotkey for demo -> new dialog
        # this handler does not use the event args at all so this is fine :)
        # though maybe a bit hackish...
        demoNewDialog(e)
    else:
        return False
    return True
    
def sizedHandler(e):
    print "Sized"
    return True
def sizedHandler1(e):
    print "Sized1"
    return True
def sizedHandler2(e):
    print "Sized2"
    return True

class GEUIApplication(SampleFramework.Application):

    def __init__(self):
        SampleFramework.Application.__init__(self)
        self.CEGUIRenderer = 0
        self.CEGUISystem = 0
        self.menuItems = []

    def _createScene(self):

        sceneManager = self.sceneManager
        sceneManager.ambientLight = ogre.ColourValue(0.25, 0.25, 0.25)

        ## setup GUI system
        if CEGUI.Version__.startswith ("0.6"):
            self.CEGUIRenderer = CEGUI.OgreRenderer(self.renderWindow,
                ogre.RENDER_QUEUE_OVERLAY, False, 3000, self.sceneManager)
            self.CEGUIsystem = CEGUI.System(self.GUIRenderer)
            # load TaharezLook scheme
            CEGUI.SchemeManager.getSingleton().loadScheme("WindowsLook.scheme")
            ## load the default font
            if not CEGUI.FontManager.getSingleton().isFontPresent("DejaVuSans-10.font"):
                d_font = CEGUI.FontManager.getSingleton().createFont("DejaVuSans-10.font")
            else:
                d_font = CEGUI.FontManager.getSingleton().getFont("DejaVuSans-10")
            wndlook = CEGUI.ImagesetManager.getSingleton().getImageset("WindowsLook")
        else:
            self.CEGUIRenderer = CEGUI.OgreRenderer.bootstrapSystem()
            self.CEGUIsystem = CEGUI.System.getSingleton()
            # load TaharezLook scheme
            CEGUI.SchemeManager.getSingleton().create("WindowsLook.scheme")
            ## load the default font
            d_font = CEGUI.FontManager.getSingleton().create("DejaVuSans-10.font")
            wndlook = CEGUI.ImagesetManager.getSingleton().get("WindowsLook")

        CEGUI.Logger.getSingleton().loggingLevel = CEGUI.Insane

        # to look more like a real application, we override the autoscale setting
        # for both skin and font
        wndlook.setAutoScalingEnabled(False)
        d_font.setProperty("AutoScaled", "false")

        # set the mouse cursor
        d_system = CEGUI.System.getSingletonPtr()
        self.mousearrow = wndlook.getImage("MouseArrow") # we need to ensure the mousetexture remains around
        d_system.setDefaultMouseCursor(self.mousearrow)

        # set the default tooltip type
        d_system.setDefaultTooltip("WindowsLook/Tooltip")

        # We need the window manager to set up the test interface :)
        d_wm = CEGUI.WindowManager.getSingletonPtr()

        # create a root window
        # this will be a static, to give a nice app'ish background
        d_root = d_wm.createWindow("WindowsLook/Static","RootWindow")
        d_root.setProperty("FrameEnabled", "false")
        # root window will take care of hotkeys
        d_root.subscribeEvent(
                    CEGUI.Window.EventKeyDown, hotkeysHandler,"")
        d_root.subscribeEvent(
                    CEGUI.Window.EventSized, sizedHandler,"")


        d_system.setGUISheet(d_root)

        # create a menubar.
        # this will fit in the top of the screen and have options for the demo
        bar_bottom = CEGUI.UDim(0,d_font.getLineSpacing(2))

        bar = d_wm.createWindow("WindowsLook/Menubar")
        bar.setArea(CEGUI.UDim(0,0),CEGUI.UDim(0,0),CEGUI.UDim(1,0),bar_bottom)
        bar.setAlwaysOnTop(True) # we want the menu on top
        d_root.addChildWindow(bar)

        # fill out the menubar
        self.createMenu(bar)

        # create a scrollable pane for our demo content
        d_pane = d_wm.createWindow("WindowsLook/ScrollablePane","DialogPane")
        print "\n\n",d_pane
        print type ( d_pane )
        d_pane.setArea(CEGUI.URect( CEGUI.UDim(0,0),
                                    bar_bottom,
                                    CEGUI.UDim(1,0),
                                    CEGUI.UDim(1,0)))
        # this scrollable pane will be a kind of virtual desktop in the sense
        # that it's bigger than the screen 3000 x 3000 pixels
        d_pane.setContentPaneAutoSized(False)
        d_pane.setContentPaneArea(CEGUI.Rect(0,0,3000,3000))
        d_root.addChildWindow(d_pane)

        # add a dialog to this pane so we have something to drag around :)
        dlg = d_wm.createWindow("WindowsLook/FrameWindow")
        dlg.setMinSize(CEGUI.UVector2(CEGUI.UDim(0,250),CEGUI.UDim(0,100)))
        dlg.setSize(CEGUI.UVector2(CEGUI.UDim(0,250),CEGUI.UDim(0,100)))

        dlg.setText("Drag me around")
        d_pane.addChildWindow(dlg)
        d_pane.subscribeEvent(
                    CEGUI.Window.EventSized, sizedHandler1,"")
        dlg.subscribeEvent(
                    CEGUI.Window.EventSized, sizedHandler2,"")

#         self.keep = []
#         self.keep.append(d_pane)
#         self.keep.append(d_font)
#         self.keep.append(d_system)
#         self.keep.append(dlg)
#         self.keep.append(d_root)
#         self.keep.append(bar)
#         self.keep.append(wndlook)
#         self.keep.append(d_wm)
#         print dir(wndlook)
#         print wndlook.getTexture()

    # Creates the menu bar and fills it up :)
    def createMenu(self, bar):

        d_wm = CEGUI.WindowManager.getSingletonPtr()
        # file menu item
        file = d_wm.createWindow("WindowsLook/MenuItem")
        file.setText("File")
        bar.addChildWindow(file)
        
        # file popup
        popup = d_wm.createWindow("WindowsLook/PopupMenu")
        file.addChildWindow(popup)
        
        # quit item in file menu
        item = d_wm.createWindow("WindowsLook/MenuItem","QuitItem")
        item.setText("Quit")
        popup.addChildWindow(item)

        # demo menu item
        demo = d_wm.createWindow("WindowsLook/MenuItem")
        demo.setText("Demo")
        bar.addChildWindow(demo)

        # demo popup
        popup = d_wm.createWindow("WindowsLook/PopupMenu")
        demo.addChildWindow(popup)

        # demo -> new window
        item = d_wm.createWindow("WindowsLook/MenuItem","NewDialogWindow")
        item.setText("New dialog")
        item.setTooltipText("Hotkey: Space")

        item.subscribeEvent(
                    "Clicked", demoNewDialog,"")

        popup.addChildWindow(item)




    def _createCamera(self):
        self.camera = self.sceneManager.createCamera("PlayerCam")
        self.camera.nearClipDistance = 5

    def _createFrameListener(self):
        self.frameListener = CEGUIFrameListener(self.renderWindow, self.camera, self.sceneManager)
        self.root.addFrameListener(self.frameListener)
        self.frameListener.showDebugOverlay(True)

    def __del__(self):
        "Clear variables, this is needed to ensure the correct order of deletion"
        del self.camera
        del self.sceneManager
        del self.frameListener
        del self.CEGUISystem
        del self.CEGUIRenderer
        del self.root
        del self.renderWindow        
     
class CEGUIFrameListener(GuiFrameListener):

    def __init__(self, renderWindow, camera, sceneManager):
        GuiFrameListener.__init__(self, renderWindow, camera, sceneManager)

        self.keepRendering = True   # whether to continue rendering or not

        item = CEGUI.WindowManager.getSingleton().getWindow("QuitItem")
        item.subscribeEvent(
                "Clicked", self, "fileQuit")

        # if something goes wrong, log the issue but do not bomb!
        #except CEGUI.Exception, e:
        #    print e

    def fileQuit(self, args):
        self.keepRendering = False
        return True

    def frameStarted(self, evt):
        if not self.keepRendering:
            return False
        else:
            return GuiFrameListener.frameStarted(self,evt)


if __name__ == '__main__':
    try:
        ta = GEUIApplication()
        ta.go()
    except ogre.OgreException, e:
        print e
