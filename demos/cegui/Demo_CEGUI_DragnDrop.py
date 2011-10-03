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

def handle_ItemDropped(args):
    if not args.window.getChildCount():
        # add dragdrop item as child of target if target has no item already
        args.window.addChildWindow(args.dragDropItem)
        # Now we must reset the item position from it's 'dropped' location,
        # since we're now a child of an entirely different window
        args.dragDropItem.setPosition(
            createUVector2(0.05, 0.05) )
    return True




class GEUIApplication(SampleFramework.Application):

    def __init__(self):
        SampleFramework.Application.__init__(self)
        self.CEGUIRenderer = 0
        self.CEGUISystem = 0

    def _createScene(self):

        sceneManager = self.sceneManager
        sceneManager.ambientLight = ogre.ColourValue(0.25, 0.25, 0.25)

        # initiaslise CEGUI Renderer
        if CEGUI.Version__.startswith ("0.6"):
            self.CEGUIRenderer = CEGUI.OgreRenderer(self.renderWindow,
                ogre.RENDER_QUEUE_OVERLAY, False, 3000, self.sceneManager)
            self.CEGUIRenderer = CEGUI.System(self.GUIRenderer)
        else:
            self.CEGUIRenderer = CEGUI.OgreRenderer.bootstrapSystem()
            self.CEGUIRenderer = CEGUI.System.getSingleton()

        CEGUI.Logger.getSingleton().loggingLevel = CEGUI.Insane

        # load TaharezLook scheme
        if CEGUI.Version__.startswith ("0.6"):
            CEGUI.SchemeManager.getSingleton().loadScheme("WindowsLook.scheme")
        else:
            CEGUI.SchemeManager.getSingleton().create("WindowsLook.scheme")

        # load font and setup default if not loaded via scheme
        if CEGUI.Version__.startswith ("0.6"):
            if not CEGUI.FontManager.getSingleton().isFontPresent("DejaVuSans-10.font"):
                try:
                    CEGUI.FontManager.getSingleton().createFont("DejaVuSans-10.font")
                except:
                    ## assume font is defined in loaded scheme
                    pass
            CEGUI.System.getSingleton().setDefaultMouseCursor("WindowsLook", "MouseArrow")
            CEGUI.System.getSingleton().setDefaultFont("DejaVuSans-10")

            # load the drive icons imageset
            CEGUI.ImagesetManager.getSingleton().createImageset("DriveIcons.imageset")
            ## load the initial layout
            CEGUI.System.getSingleton().setGUISheet(
                CEGUI.WindowManager.getSingleton().loadWindowLayout("DragDropDemo.layout", False))
        else:
            CEGUI.FontManager.getSingleton().create("DejaVuSans-10.font")
            CEGUI.System.getSingleton().setDefaultMouseCursor("WindowsLook", "MouseArrow")
            CEGUI.System.getSingleton().setDefaultFont("DejaVuSans-10")

            # load the drive icons imageset
            CEGUI.ImagesetManager.getSingleton().create("DriveIcons.imageset")


            ## load the initial layout
            CEGUI.System.getSingleton().setGUISheet(
                CEGUI.WindowManager.getSingleton().loadWindowLayout("DragDropDemo.layout"))

        ## setup events
        self._subscribeEvents()


    def _subscribeEvents(self):
        wmgr = CEGUI.WindowManager.getSingleton()


        # Subscribe the same handler to each of the twelve slots
        base_name = "Root/MainWindow/Slot"
        for i in range(1,13):
            try:
                # get the window pointer for this slot
                wnd =  wmgr.getWindow(base_name + CEGUI.PropertyHelper.intToString(i))
                wnd.subscribeEvent(
                    CEGUI.Window.EventDragDropItemDropped, handle_ItemDropped,"")
            except CEGUI.Exception, e:
                print e

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

        # Subscribe handler to deal with user closing the frame window
        try:
            main_wnd = CEGUI.WindowManager.getSingleton().getWindow("Root/MainWindow")
            main_wnd.subscribeEvent(
                CEGUI.FrameWindow.EventCloseClicked, self, "handle_CloseButton")

        # if something goes wrong, log the issue but do not bomb!
        except CEGUI.Exception, e:
            print e

    def handle_CloseButton(self, args):
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
