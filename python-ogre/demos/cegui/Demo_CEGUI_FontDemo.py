# -*- coding: utf-8 -*- 

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
#/////# -*- coding: latin-1 -*-
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
def cegui_reldim ( x ) :
    return CEGUI.UDim((x),0)

def getFontDict():
    SCHEME_FONT = {}
    SCHEME_FONT['Commonwealth-10'] = "Commonwealth-10.font"
    SCHEME_FONT['BlueHighway-12']  = "bluehighway-12.font"
    SCHEME_FONT['BlueHighway-10']  = "bluehighway-10.font"
    SCHEME_FONT['BlueHighway-8']   = "bluehighway-8.font"
    SCHEME_FONT['DejaVuSans-10']   = "DejaVuSans-10.font"
    SCHEME_FONT['Commonwealth-10'] = "Commonwealth-10.font"
    SCHEME_FONT['Iconified-12']    = "Iconified-12.font"
    SCHEME_FONT['fkp-16']          = "fkp-16.font"
    SCHEME_FONT['FairChar-30']     = "FairChar-30.font"
    return SCHEME_FONT


def getLanguageDict():
    LANGUAGE_DICT = {}
    LANGUAGE_DICT['English'] = "THIS IS SOME TEXT IN UPPERCASE\n and this is lowercase...\n Try Catching The Brown Fox While It's Jumping Over The Lazy Dog" 
    LANGUAGE_DICT['Russian'] = u"Всё ускоряющаяся эволюция компьютерных технологий предъявила жёсткие требования к производителям как собственно вычислительной техники, так и периферийных устройств.\n"
    LANGUAGE_DICT['Romanian'] = u"CEI PATRU APOSTOLI\n au fost trei:\n Luca şi Matfei\n"
    LANGUAGE_DICT['Danish'] = u"FARLIGE STORE BOGSTAVER\n og flere men små...\n Quizdeltagerne spiste jordbær med fløde, mens cirkusklovnen Walther spillede på xylofon\n"
    LANGUAGE_DICT['French'] = u'La Peña'
    return LANGUAGE_DICT

global MIN_POINT_SIZE
MIN_POINT_SIZE=6.0


class GEUIApplication(SampleFramework.Application):

    def __init__(self):
        SampleFramework.Application.__init__(self)
        self.CEGUIRenderer = 0
        self.CEGUISystem = 0
        self.MenuMode = True
        self.ListItems = []

    def _createScene(self):

        sceneManager = self.sceneManager
        sceneManager.ambientLight = ogre.ColourValue(0.25, 0.25, 0.25)

        ## setup GUI system
        if CEGUI.Version__.startswith ("0.6"):
            self.CEGUIRenderer = CEGUI.OgreRenderer(self.renderWindow,
                ogre.RENDER_QUEUE_OVERLAY, False, 3000, self.sceneManager)
            self.CEGUIsystem = CEGUI.System(self.GUIRenderer)
        else:
            self.CEGUIRenderer = CEGUI.OgreRenderer.bootstrapSystem()
            self.CEGUIsystem = CEGUI.System.getSingleton()

        CEGUI.Logger.getSingleton().loggingLevel = CEGUI.Insane

        winMgr = CEGUI.WindowManager.getSingleton()

        ## load scheme and set up defaults
        if CEGUI.Version__.startswith ("0.6"):
            CEGUI.SchemeManager.getSingleton().loadScheme ("TaharezLook.scheme")
            CEGUI.System.getSingleton().setDefaultMouseCursor ("TaharezLook", "MouseArrow")
            # load an image to use as a background
            CEGUI.ImagesetManager.getSingleton().createImagesetFromImageFile("BackgroundImage", "GPN-2000-001437.tga")
            # load all the fonts except Commonwealth which has been already loaded
            fontDict = getFontDict()
            for f in fontDict.keys():
                if not CEGUI.FontManager.getSingleton().isFontPresent(f):
                    CEGUI.FontManager.getSingleton().createFont(fontDict[f])
        else:
            CEGUI.SchemeManager.getSingleton().create ("TaharezLook.scheme")
            CEGUI.System.getSingleton().setDefaultMouseCursor ("TaharezLook", "MouseArrow")
            # load an image to use as a background
            CEGUI.ImagesetManager.getSingleton().createFromImageFile("BackgroundImage", "GPN-2000-001437.tga")
            # load all the fonts except Commonwealth which has been already loaded
            fontDict = getFontDict()
            for f in fontDict.keys():
                if not CEGUI.FontManager.getSingleton().isDefined(f):
                    CEGUI.FontManager.getSingleton().create(fontDict[f])




        # here we will use a StaticImage as the root, then we 
        # can use it to place a background image
        background = winMgr.createWindow ("TaharezLook/StaticImage")
        # set area rectangle
        background.setArea (CEGUI.URect(cegui_reldim (0), cegui_reldim (0),
                                          cegui_reldim (1), cegui_reldim (1)))
        # disable frame and standard background
        background.setProperty ("FrameEnabled", "false")
        background.setProperty ("BackgroundEnabled", "false")
        # set the background image
        background.setProperty ("Image", "set:BackgroundImage image:full_image")
        # install this as the root GUI sheet
        CEGUI.System.getSingleton().setGUISheet (background)

        #/ set tooltip styles (by default there is none)
        CEGUI.System.getSingleton().setDefaultTooltip ("TaharezLook/Tooltip")

        # load some demo windows and attach to the background 'root'
        if CEGUI.Version__.startswith ("0.6"):
            background.addChildWindow (winMgr.loadWindowLayout ("FontDemo.layout", False))
        else:
            background.addChildWindow (winMgr.loadWindowLayout ("FontDemo.layout"))

        # Add the font names to the listbox
        fontDict = getFontDict()
        lbox = winMgr.getWindow ("FontDemo/FontList")
        for f in fontDict.keys():
            item = CEGUI.ListboxTextItem(f)
            item.setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush")
            # ensure Python is in control of it's "deletion"
            item.AutoDeleted = False
            lbox.addItem( item)
            self.ListItems.append(item)

        # set up the font listbox callback
        lbox.subscribeEvent(
                    CEGUI.Listbox.EventSelectionChanged, self, "handleFontSelection")
        # select the first font
        lbox.setItemSelectState(0, True)

        # Add language list to the listbox
        lbox = winMgr.getWindow ("FontDemo/LangList")
        languageDict = getLanguageDict()
        for l in languageDict.keys():
            item = CEGUI.ListboxTextItem(l)
            item.setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush")
            # ensure Python is in control of it's "deletion"
            item.AutoDeleted = False
            lbox.addItem( item)
            self.ListItems.append(item)

        # set up the language listbox callback
        lbox.subscribeEvent(
                    CEGUI.Listbox.EventSelectionChanged, self, "handleLangSelection")
        # select the first language
        lbox.setItemSelectState(0, True)

        winMgr.getWindow("FontDemo/AutoScaled").subscribeEvent(
            CEGUI.Checkbox.EventCheckStateChanged,
            self, 'handleAutoScaled')
        winMgr.getWindow("FontDemo/Antialiased").subscribeEvent (
            CEGUI.Checkbox.EventCheckStateChanged,
            self, 'handleAntialiased')
        winMgr.getWindow("FontDemo/PointSize").subscribeEvent (
            CEGUI.Scrollbar.EventScrollPositionChanged,
            self, 'handlePointSize')



    ## Event Handlers ##
    def setFontDesc(self):
        winMgr = CEGUI.WindowManager.getSingleton()
        mle = winMgr.getWindow("FontDemo/FontSample")
        f = mle.getFont()
        s = f.getProperty("Name")
        if f.isPropertyPresent("PointSize"):
            s += "." + f.getProperty("PointSize")
        winMgr.getWindow("FontDemo/FontDesc").setText(s)

    def handleFontSelection(self,e):
        lbox = e.window
        if lbox.getFirstSelectedItem():
            if CEGUI.Version__.startswith ("0.6"):
                f = CEGUI.FontManager.getSingleton().getFont(
                        lbox.getFirstSelectedItem().getText())
            else:
                f = CEGUI.FontManager.getSingleton().get(
                        lbox.getFirstSelectedItem().getText())

            winMgr = CEGUI.WindowManager.getSingleton()
            winMgr.getWindow("FontDemo/FontSample").setFont(f)
            b = f.isPropertyPresent("AutoScaled")
            cb = winMgr.getWindow("FontDemo/AutoScaled")
            cb.setEnabled(b)
            if b:
                cb.setSelected(CEGUI.PropertyHelper.stringToBool(f.getProperty ("AutoScaled")))

            b = f.isPropertyPresent("Antialiased")
            cb = winMgr.getWindow("FontDemo/Antialiased")
            cb.setEnabled (b)
            if b:
                cb.setSelected(CEGUI.PropertyHelper.stringToBool(f.getProperty ("Antialiased")))

            b = f.isPropertyPresent ("PointSize")
            sb = winMgr.getWindow("FontDemo/PointSize")
            sb.setEnabled(b)
            if b:
                sb.setScrollPosition ( CEGUI.PropertyHelper.stringToFloat(f.getProperty("PointSize")) - MIN_POINT_SIZE)

            self.setFontDesc()

        return True


    def handleAutoScaled (self, e):
        winMgr = CEGUI.WindowManager.getSingleton()
        cb = e.window
        mle = winMgr.getWindow("FontDemo/FontSample")
        f = mle.getFont()
        f.setProperty("AutoScaled",
                        CEGUI.PropertyHelper.boolToString(cb.isSelected()))

        return True


    def handleAntialiased(self, e):
        winMgr = CEGUI.WindowManager.getSingleton()
        cb = e.window
        mle = winMgr.getWindow("FontDemo/FontSample")
        f = mle.getFont()
        f.setProperty ("Antialiased",
                        CEGUI.PropertyHelper.boolToString(cb.isSelected()))

        return True

    def handlePointSize(self, e):
        winMgr = CEGUI.WindowManager.getSingleton()
        sb = e.window
        f = winMgr.getWindow("FontDemo/FontSample").getFont()
        f.setProperty ("PointSize",
                        CEGUI.PropertyHelper.intToString (
                            int(MIN_POINT_SIZE + sb.getScrollPosition ())))
        self.setFontDesc()

        return True

    def handleLangSelection(self, e):
        lbox = e.window
        if lbox.getFirstSelectedItem():
            idx = lbox.getItemIndex(lbox.getFirstSelectedItem())
            winMgr = CEGUI.WindowManager.getSingleton()

            cs = CEGUI.String()
            cs.assign( getLanguageDict().values()[idx].encode("utf-8") )
            winMgr.getWindow("FontDemo/FontSample").setText(cs)

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

        self.keepRendering = True   # whether to continue rendering or not


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
