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

def getPageTextList(): 
    PageText= [
        "This is page three",\
        "And this is page four, it's not too different from page three, isn't it?",\
        "Yep, you guessed, this is page five",\
        "And this is page six",\
        "Seven",\
        "Eight",\
        "Nine. Quite boring, isn't it?",\
        "Ten",\
        "Eleven",\
        "Twelve",\
        "Thirteen",\
        "Fourteen",\
        "Fifteen",\
        "And, finally, sixteen. Congrats, you found the last page!" ]
    return PageText

def cegui_reldim ( x ) :
    return CEGUI.UDim((x),0)

class GEUIApplication(SampleFramework.Application):

    def __init__(self):
        SampleFramework.Application.__init__(self)
        self.CEGUIRenderer = 0
        self.CEGUISystem = 0
        self.ListItems = []

    def _createScene(self):

        sceneManager = self.sceneManager
        sceneManager.ambientLight = ogre.ColourValue(0.25, 0.25, 0.25)

        ## setup GUI system
        if CEGUI.Version__.startswith ("0.6"):
            self.CEGUIRenderer = CEGUI.OgreRenderer(self.renderWindow,
                ogre.RENDER_QUEUE_OVERLAY, False, 3000, self.sceneManager)
            self.CEGUIsystem = CEGUI.System(self.GUIRenderer)
            # load TaharezLook scheme
            CEGUI.SchemeManager.getSingleton().loadScheme("TaharezLook.scheme")
            ## load the default font
            if not CEGUI.FontManager.getSingleton().isFontPresent("DejaVuSans-10.font"):
                d_font = CEGUI.FontManager.getSingleton().createFont("DejaVuSans-10.font")
            else:
                d_font = CEGUI.FontManager.getSingleton().getFont("DejaVuSans-10")
            # load an image to use as a background
            CEGUI.ImagesetManager.getSingleton().createImagesetFromImageFile("BackgroundImage", "GPN-2000-001437.tga")
        else:
            self.CEGUIRenderer = CEGUI.OgreRenderer.bootstrapSystem()
            self.CEGUIsystem = CEGUI.System.getSingleton()
            # load TaharezLook scheme
            CEGUI.SchemeManager.getSingleton().create("TaharezLook.scheme")
            ## load the default font
            d_font = CEGUI.FontManager.getSingleton().create("DejaVuSans-10.font")
            # load an image to use as a background
            CEGUI.ImagesetManager.getSingleton().createFromImageFile("BackgroundImage", "GPN-2000-001437.tga")

        ## set the logging level
        CEGUI.Logger.getSingleton().loggingLevel = CEGUI.Insane

        ## we will make extensive use of the WindowManager.
        winMgr = CEGUI.WindowManager.getSingleton()

        CEGUI.System.getSingleton().setDefaultMouseCursor ("TaharezLook", "MouseArrow")


        # here we will use a StaticImage as the root, then we can use 
        # it to place a background image
        background = winMgr.createWindow ("TaharezLook/StaticImage")
        ## set area rectangle
        background.setArea(CEGUI.URect (  cegui_reldim (0), cegui_reldim (0),
                                          cegui_reldim (1), cegui_reldim (1)))
        # disable frame and standard background
        background.setProperty ("FrameEnabled", "false")
        background.setProperty ("BackgroundEnabled", "false")
        # set the background image
        background.setProperty ("Image", "set:BackgroundImage image:full_image")
        # install this as the root GUI sheet
        CEGUI.System.getSingleton ().setGUISheet(background)

        # set tooltip styles (by default there is none)
        CEGUI.System.getSingleton().setDefaultTooltip ("TaharezLook/Tooltip")

        # load some demo windows and attach to the background 'root'
        # because of the 2nd parameter, all windows get prefixed with
        # "TabControlDemo/"
        ## workaround for python ogre, we don't have loadWindowLayout with prefix
        ## so we assing random prefix to ensure unique window loaded and then set
        ## prefix

        prefix = "TabControlDemo/"
        w = winMgr.loadWindowLayout("TabControlDemo.layout", prefix,"General")
        background.addChildWindow(w)

        tc = winMgr.getWindow ("TabControlDemo/Frame/TabControl")

        # Add some pages to tab control
        tc.addTab (winMgr.loadWindowLayout("TabPage1.layout", prefix, "General"))
        tc.addTab (winMgr.loadWindowLayout("TabPage2.layout", prefix, "General"))

        # What did it load?
        ## WindowManager::WindowIterator 
        #it =  CEGUI.WindowManager::getSingleton().getIterator()
        #for(; !it.isAtEnd() ; ++it) {
        #    const char* windowName = it.getCurrentValue()->getName().c_str();
        #    printf("Name: %s\n", windowName);
        #}

        winMgr.getWindow("TabControlDemo/Page1/AddTab").subscribeEvent (
            CEGUI.PushButton.EventClicked,self, "handleAddTab")

        # Click to visit this tab
        winMgr.getWindow("TabControlDemo/Page1/Go").subscribeEvent(
            CEGUI.PushButton.EventClicked,self,
            "handleGoto")

        # Click to make this tab button visible (when scrolling is required)
        winMgr.getWindow("TabControlDemo/Page1/Show").subscribeEvent (
            CEGUI.PushButton.EventClicked, self,
            "handleShow")

        winMgr.getWindow("TabControlDemo/Page1/Del").subscribeEvent (
            CEGUI.PushButton.EventClicked, self,
            "handleDel")

        rb = winMgr.getWindow("TabControlDemo/Page1/TabPaneTop")
        rb.setSelected (tc.getTabPanePosition() == CEGUI.TabControl.Top)
        rb.subscribeEvent (
            CEGUI.RadioButton.EventSelectStateChanged, self,
            "handleTabPanePos")

        rb = winMgr.getWindow("TabControlDemo/Page1/TabPaneBottom")
        rb.setSelected (tc.getTabPanePosition() == CEGUI.TabControl.Bottom)
        rb.subscribeEvent (
            CEGUI.RadioButton.EventSelectStateChanged, self, 
            "handleTabPanePos")

        sb = winMgr.getWindow("TabControlDemo/Page1/TabHeight")
        sb.setScrollPosition (tc.getTabHeight().d_offset)
        sb.subscribeEvent (
            CEGUI.Scrollbar.EventScrollPositionChanged, self, 
            "handleTabHeight")

        sb = winMgr.getWindow("TabControlDemo/Page1/TabPadding")
        sb.setScrollPosition (tc.getTabTextPadding().d_offset)
        sb.subscribeEvent (
            CEGUI.Scrollbar.EventScrollPositionChanged, self, 
            "handleTabPadding")

        self.refreshPageList()


    def refreshPageList(self):
        winMgr = CEGUI.WindowManager.getSingleton()
        # Check if the windows exists
        lbox = None
        tc = None
        if winMgr.isWindowPresent("TabControlDemo/Page1/PageList"):
            lbox = winMgr.getWindow("TabControlDemo/Page1/PageList")

        if winMgr.isWindowPresent("TabControlDemo/Frame/TabControl"):
            tc = winMgr.getWindow("TabControlDemo/Frame/TabControl")

        if lbox and tc:
            lbox.resetList()
            for i in range( tc.getTabCount() ):
                item = CEGUI.ListboxTextItem(tc.getTabContentsAtIndex(i).getName())
                item.setSelectionBrushImage("TaharezLook",
                     "MultiListSelectionBrush")
                # ensure Python is in control of it's "deletion"
                item.AutoDeleted = False
                lbox.addItem( item)
                self.ListItems.append(item)


    def handleTabPanePos(self, e):

        if e.window.getID()==0:
           tpp = CEGUI.TabControl.Top
        elif e.window.getID()==1:
           tpp = CEGUI.TabControl.Bottom
        else:
            return False

        # Check if the window exists
        winMgr = CEGUI.WindowManager.getSingleton()
        if winMgr.isWindowPresent("TabControlDemo/Frame/TabControl"):
            winMgr.getWindow (
                "TabControlDemo/Frame/TabControl").setTabPanePosition(tpp)
        return True

    def handleTabHeight(self, e):
        sb = e.window

        # Check if the window exists
        winMgr = CEGUI.WindowManager.getSingleton()
        if winMgr.isWindowPresent("TabControlDemo/Frame/TabControl"):
            CEGUI.WindowManager.getSingleton().getWindow(
                "TabControlDemo/Frame/TabControl").setTabHeight (
                    CEGUI.UDim (0, sb.getScrollPosition()))

        # The return value mainly sais that we handled it, 
        # not if something failed.
        return True

    def handleTabPadding(self, e):
        sb = e.window

        # Check if the window exists
        winMgr = CEGUI.WindowManager.getSingleton()
        if winMgr.isWindowPresent("TabControlDemo/Frame/TabControl"):
            winMgr.getWindow (
                "TabControlDemo/Frame/TabControl").setTabTextPadding (
                    CEGUI.UDim (0, sb.getScrollPosition ()))
        return True

    def handleAddTab(self, e):
        sb = e.window
        winMgr = CEGUI.WindowManager.getSingleton()
        # Check if the window exists
        if winMgr.isWindowPresent("TabControlDemo/Frame/TabControl"):
            tc = winMgr.getWindow (
                "TabControlDemo/Frame/TabControl")

            # Add some tab buttons once
            for num in range(3,17):
                prefix = "TabControlDemo/Page%d" %(num)
                if not winMgr.isWindowPresent(prefix):
                    pg = None
                    #try:
                    pg = winMgr.loadWindowLayout("TabPage.layout", prefix,"General")

                    #except:
                    #    print "Some error occured while adding a tabpage. Please see the logfile." 
                    prefix = prefix + "Text"
                    # This window has just been created while loading the layout
                    if winMgr.isWindowPresent(prefix):
                        txt = winMgr.getWindow(prefix)
                        txt.setText( getPageTextList()[num - 3])
                        pgname = "Page %d" %(num)
                        pg.setText(pgname)
                        tc.addTab(pg)
                        self.refreshPageList()
        return True

    def handleGoto(self, e):
        winMgr = CEGUI.WindowManager.getSingleton()
        # Check if the windows exists
        lbox = None
        tc = None
        if winMgr.isWindowPresent("TabControlDemo/Page1/PageList"):
            lbox = winMgr.getWindow(
                "TabControlDemo/Page1/PageList")
        if winMgr.isWindowPresent("TabControlDemo/Frame/TabControl"):
            tc = winMgr.getWindow (
                "TabControlDemo/Frame/TabControl")
        if lbox and tc:
            lbi = lbox.getFirstSelectedItem()
            if lbi:
                tc.setSelectedTab(lbi.getText())
        return True

    def handleShow(self, e):
        winMgr = CEGUI.WindowManager.getSingleton()
        # Check if the windows exists
        lbox = None
        tc = None
        if winMgr.isWindowPresent("TabControlDemo/Page1/PageList"):
            lbox = winMgr.getWindow(
                "TabControlDemo/Page1/PageList")
        if winMgr.isWindowPresent("TabControlDemo/Frame/TabControl"):
            tc = winMgr.getWindow (
                "TabControlDemo/Frame/TabControl")

        if lbox and tc:
            lbi = lbox.getFirstSelectedItem()
            if lbi:
                tc.makeTabVisible(lbi.getText())

        return True

    def handleDel(self, e):
        winMgr = CEGUI.WindowManager.getSingleton()
        # Check if the windows exists
        lbox = None
        tc = None
        if winMgr.isWindowPresent("TabControlDemo/Page1/PageList"):
            lbox = winMgr.getWindow(
                "TabControlDemo/Page1/PageList")
        if winMgr.isWindowPresent("TabControlDemo/Frame/TabControl"):
            tc = winMgr.getWindow (
                "TabControlDemo/Frame/TabControl")

        if lbox and tc:
            lbi = lbox.getFirstSelectedItem()
            if lbi:
                tc.removeTab(lbi.getText())
                # Remove the actual window from Cegui
                winMgr.destroyWindow(lbi.getText())
                self.refreshPageList()
        return True


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
        self.MenuMode=True
        self.keepRendering = True   # whether to continue rendering or not


    def frameStarted(self, evt):
        return GuiFrameListener.frameStarted(self,evt)


if __name__ == '__main__':
    try:
        ta = GEUIApplication()
        ta.go()
    except ogre.OgreException, e:
        print e
