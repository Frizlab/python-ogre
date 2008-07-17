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


def cegui_reldim ( x ) :
    return CEGUI.UDim((x),0)

def ceguimax ( x, y ) :
    if x>y: return x
    return y
def ceguimin ( x, y ) :
    if x<y: return x
    return y




class  DemoConsole():

    SubmitButtonID = 1
    EntryBoxID     = 2
    HistoryID      = 3

    def __init__(self, id_name, parent=None):

        d_root = CEGUI.WindowManager.getSingleton().loadWindowLayout("VanillaConsole.layout", id_name, 'General')
        self.d_historyPos = 0
        self.d_root = d_root
        self.d_history = []

        # we will destroy the console box windows ourselves
        d_root.setDestroyedByParent(False)



        # Do events wire-up
        d_root.subscribeEvent(
            CEGUI.Window.EventKeyDown, self, "handleKeyDown")

        d_root.getChild(self.SubmitButtonID).subscribeEvent(
                CEGUI.PushButton.EventClicked, self, "handleSubmit")
        d_root.getChild(self.EntryBoxID).subscribeEvent(
                CEGUI.Editbox.EventTextAccepted, self, "handleSubmit")

        # decide where to attach the console main window
        def attachTo(parent, sheet):
            if parent : return parent
            return sheet

        parent = attachTo(parent , CEGUI.System.getSingleton().getGUISheet() )

        # attach this window if parent is valid
        if parent:
            parent.addChildWindow(d_root)


    def __del__(self):
        # destroy the windows that we loaded earlier
        CEGUI.WindowManager.getSingleton().destroyWindow(self.d_root)
        self.d_root = 0


    def toggleVisibility(self):
        if self.d_root.isVisible(True):
            self.d_root.hide()
        else:
            self.d_root.show()


    def isVisible(self):
        return self.d_root.isVisible()

    def handleSubmit(self, args):

        # get the text entry editbox
        editbox = self.d_root.getChild(self.EntryBoxID)

        # get text out of the editbox
        edit_text = editbox.getText()

        # if the string is not empty
        if len(edit_text.c_str())>0:
            # add this entry to the command history buffer
            self.d_history.append(edit_text)
            # reset history position
            self.d_historyPos = len(self.d_history)
            # append newline to this entry
            edit_text += '\n'
            # get history window
            history = self.d_root.getChild(self.HistoryID)
            print dir(history)
            # append new text to history output
            history.setText(history.getText() + edit_text)
            # scroll to bottom of history output
            history.setCaratIndex(-1)
            # erase text in text entry box.
            editbox.setText("")

        # re-activate the text entry box
        editbox.activate()

        return True

    def handleKeyDown(self, args):

        # get the text entry editbox
        editbox = self.d_root.getChild(self.EntryBoxID)

        if args.scancode == CEGUI.Key.ArrowUp :
            self.d_historyPos = ceguimax(self.d_historyPos - 1, -1)
            if self.d_historyPos >= 0:
                editbox.setText(self.d_history[self.d_historyPos])
                editbox.setCaratIndex(-1)
            else:
                editbox.setText("")
            editbox.activate()

        elif args.scancode == CEGUI.Key.ArrowDown :
            self.d_historyPos = ceguimin(self.d_historyPos + 1, len(self.d_history) )
            if self.d_historyPos < len(self.d_history):
                editbox.setText(self.d_history[self.d_historyPos])
                editbox.setCaratIndex(-1)
            else:
                editbox.setText("")

            editbox.activate()

        else:
            return False

        return True


class GEUIApplication(SampleFramework.Application):

    def __init__(self):
        SampleFramework.Application.__init__(self)
        self.CEGUIRenderer = 0
        self.CEGUISystem = 0
        self.d_console = None

    def _createScene(self):

        sceneManager = self.sceneManager
        sceneManager.ambientLight = ogre.ColourValue(0.25, 0.25, 0.25)

        # initiaslise OgreCEGUI Renderer
 
        self.CEGUIRenderer = CEGUI.OgreCEGUIRenderer(self.renderWindow,
                ogre.RenderQueueGroupID.RENDER_QUEUE_OVERLAY,
                False, 3000,
                self.sceneManager)
        self.CEGUISystem = CEGUI.System(self.CEGUIRenderer)

        ## set the logging level
        CEGUI.Logger.getSingleton().loggingLevel = CEGUI.Insane

        ## we will make extensive use of the WindowManager.
        winMgr = CEGUI.WindowManager.getSingleton()

        # Load the scheme to initialse the VanillaSkin which we use in this sample
        CEGUI.SchemeManager.getSingleton().loadScheme("VanillaSkin.scheme")
        # set default mouse image
        CEGUI.System.getSingleton().setDefaultMouseCursor("Vanilla-Images", "MouseArrow")

        # load an image to use as a background
        CEGUI.ImagesetManager.getSingleton().createImagesetFromImageFile("BackgroundImage", "GPN-2000-001437.tga")

        # here we will use a StaticImage as the root, then we can use
        # it to place a background image
        background = winMgr.createWindow("Vanilla/StaticImage")
        # set area rectangle
        background.setArea(CEGUI.URect(cegui_reldim(0), cegui_reldim(0),
                cegui_reldim(1), cegui_reldim(1)))
        # disable frame and standard background
        background.setProperty("FrameEnabled", "false")
        background.setProperty("BackgroundEnabled", "false")
        # set the background image
        background.setProperty("Image", "set:BackgroundImage image:full_image")
        # install this as the root GUI sheet
        CEGUI.System.getSingleton().setGUISheet(background)

        if not CEGUI.FontManager.getSingleton().isFontPresent("Iconified-12"):
		    CEGUI.FontManager.getSingleton().createFont("Iconified-12.font")

        # load some demo windows and attach to the background 'root'
        background.addChildWindow(winMgr.loadWindowLayout("VanillaWindows.layout", False))

        # create an instance of the console class.
        self.d_console = DemoConsole("Demo")

        # listen for key presses on the root window.
        background.subscribeEvent(
            CEGUI.Window.EventKeyDown,
            self, 'handleRootKeyDown')

        # activate the background window
        background.activate()

    def handleRootKeyDown(self, args):
        keyArgs = args

        if keyArgs.scancode == CEGUI.Key.F12:
            self.d_console.toggleVisibility()

        else:
            return False
        return True



    def _createCamera(self):
        self.camera = self.sceneManager.createCamera("PlayerCam")
        self.camera.nearClipDistance = 5

    def _createFrameListener(self):
        self.frameListener = CEGUIFrameListener(self.renderWindow, self.camera, self.sceneManager)
        self.root.addFrameListener(self.frameListener)
        self.frameListener.showDebugOverlay(True)

    def cleanUp(self):
        print "Cleaning Up"
        del self.d_console

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
        self.MenuMode=True
        self.keepRendering = True   # whether to continue rendering or not



if __name__ == '__main__':
    try:
        ta = GEUIApplication()
        ta.go()
        ta.cleanUp()
    except ogre.OgreException, e:
        print e

