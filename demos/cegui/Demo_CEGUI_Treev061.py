###
###  This demo is a based on the cegui Tree example
###


##/*************************************************************************
##    Crazy Eddie's GUI System (http://www.cegui.org.uk)
##    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
##
##    This library is free software; you can redistribute it and/or
##    modify it under the terms of the GNU Lesser General Public
##    License as published by the Free Software Foundation; either
##    version 2.1 of the License, or (at your option) any later version.
##
##    This library is distributed in the hope that it will be useful,
##    but WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
##    Lesser General Public License for more details.
##
##    You should have received a copy of the GNU Lesser General Public
##    License along with this library; if not, write to the Free Software
##    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
##  *************************************************************************/


import sys
import os
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.io.OIS as OIS
import ogre.gui.CEGUI as CEGUI
import SampleFramework

import sys, exceptions, random

from CEGUI_framework import *   

# Note: only taharez scheme works, the cegui demo Vanilla TreeDemo.layout is
# missing
def getSchemeDict(scheme):
    SCHEME_DICT = {}

    if scheme=="Vanilla":
        SCHEME_DICT['SCHEME_FILE_NAME'] =  "VanillaSkin.scheme"
        SCHEME_DICT['IMAGES_FILE_NAME'] =  "Vanilla-Images"
        SCHEME_DICT['STATICIMAGE_NAME'] =  "Vanilla/StaticImage"
        SCHEME_DICT['TOOLTIP_NAME']     =  "Vanilla/Tooltip"
        SCHEME_DICT['LAYOUT_FILE_NAME'] =  "TreeDemo.layout"
        SCHEME_DICT['BRUSH_NAME'] =        "GenericBrush"
    else:
        SCHEME_DICT['SCHEME_FILE_NAME'] =  "TaharezLook.scheme"
        SCHEME_DICT['IMAGES_FILE_NAME'] =  "TaharezLook"
        SCHEME_DICT['STATICIMAGE_NAME'] =  "TaharezLook/StaticImage"
        SCHEME_DICT['TOOLTIP_NAME']     =  "TaharezLook/Tooltip"
        SCHEME_DICT['LAYOUT_FILE_NAME'] =  "TreeDemoTaharez.layout"
        SCHEME_DICT['BRUSH_NAME']       =  "TextSelectionBrush"
    return SCHEME_DICT


def getSchemeFonts():
    SCHEME_FONT = {}
    SCHEME_FONT['Commonwealth-10'] = "Commonwealth-10.font"
    SCHEME_FONT['BlueHighway-12']  = "bluehighway-12.font"
    SCHEME_FONT['BlueHighway-10']  = "bluehighway-10.font"
    SCHEME_FONT['BlueHighway-8']   = "bluehighway-8.font"
    return SCHEME_FONT


def cegui_reldim ( x ) :
    return CEGUI.UDim((x),0)

## ====================================================================
class MaterialEditor(object):

    def __init__(self, text):
       self.editBox = CEGUI.WindowManager.getSingleton().getWindow('MaterialEditor/MultiLineEditbox')
       self.editFrame = CEGUI.WindowManager.getSingleton().getWindow('MaterialEditor')
       self.enable(True)
       self.update(text)
       self.editFrame.setVisible(True)

    def __del__(self):
       self.enable(False)
       self.editFrame.setVisible(False)

    def enable(self, enableFlag):
        self.editFrame.setEnabled(enableFlag)

    def clear(self, enableFlag):
        self.editFrame.setText( '' )
        self.editBox.setText( '' )

    def update(self, text):
        self.editFrame.setText( text )
        mm = ogre.MaterialManager.getSingleton()
        rm = ogre.ResourceGroupManager.getSingleton()
        ds = rm.openResource(text, "General", True)
        self.editBox.setText(ds.getAsString())


## =========================================================

class GuiApplication ( SampleFramework.Application ):
   
    def __init__(self):
        SampleFramework.Application.__init__(self)
        self.GUIRenderer=0
        self.GUIsystem =0
        self.EditorGuiSheet=0
        self.ListItems = []
        self.resourceEditor = None
        
    def __del__(self):
        del self.camera
        del self.sceneManager
        del self.frameListener
        if self.resourceEditor:
            self.resourceEditor = None
        try:
            if self.EditorGuiSheet:
                CEGUI.WindowManager.getSingleton().destroyWindow(self.EditorGuiSheet) 
        except:
            pass
        del self.GUIsystem
        del self.GUIRenderer
        del self.root
        del self.renderWindow        
   
    def _setUpResources(self):
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/gui", "FileSystem", "General", False)
        SampleFramework.Application._setUpResources(self)

    ## Just override the mandatory create scene method
    def _createScene(self):
        sceneManager = self.sceneManager
        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)

        ## Create a skydome
        self.sceneManager.setSkyDome(True, "Examples/CloudySky", 5, 8) 

        ## Create a light
        l = self.sceneManager.createLight("MainLight") 
        l.setPosition(20,80,50) 

        ## create Ogre CEGUI Renderer
        self.GUIRenderer = CEGUI.OgreCEGUIRenderer( self.renderWindow,\
                                                    ogre.RENDER_QUEUE_OVERLAY,\
                                                    False,\
                                                    3000,\
                                                    self.sceneManager) 
        ## setup GUI system
        self.GUIsystem = CEGUI.System(self.GUIRenderer) 
        logger = CEGUI.Logger.getSingleton()
        logger.setLoggingLevel( CEGUI.Informative ) 

        # we will make extensive use of the WindowManager.
        winMgr = CEGUI.WindowManager.getSingleton()

        ## get scheme characteristics
        SCHEME_DICT = getSchemeDict('TaharezLook')
        ##print SCHEME_DICT['SCHEME_FILE_NAME']
        ##print SCHEME_DICT['IMAGES_FILE_NAME']
        ##print SCHEME_DICT['STATICIMAGE_NAME']
        ##print SCHEME_DICT['TOOLTIP_NAME']
        ##print SCHEME_DICT['LAYOUT_FILE_NAME']
        ##print SCHEME_DICT['BRUSH_NAME']

        ## load scheme and set up defaults
        CEGUI.SchemeManager.getSingleton().loadScheme(SCHEME_DICT['SCHEME_FILE_NAME']) 
        self.GUIsystem.setDefaultMouseCursor(SCHEME_DICT['IMAGES_FILE_NAME'],  "MouseArrow") 
        ########

        ## Load Fonts
        SCHEME_FONTS = getSchemeFonts()
        for f in SCHEME_FONTS.keys():
            if not CEGUI.FontManager.getSingleton().isFontPresent(f):
                CEGUI.FontManager.getSingleton().createFont(SCHEME_FONTS[f])
        self.GUIsystem.setDefaultFont( "BlueHighway-12") 
        ########

        ## load image to use as a background
        CEGUI.ImagesetManager.getSingleton().createImagesetFromImageFile("BackgroundImage", "GPN-2000-001437.tga")
        
        ## here we will use a StaticImage as the root, then we can use it to place a background image
        background = winMgr.createWindow(SCHEME_DICT['STATICIMAGE_NAME'] , "background_wnd")
        ## set position and size
        background.setPosition(CEGUI.UVector2(cegui_reldim(0), cegui_reldim( 0)))
        background.setSize(CEGUI.UVector2(cegui_reldim(1), cegui_reldim( 1)))
        ## disable frame and standard background
        background.setProperty("FrameEnabled", "false")
        background.setProperty("BackgroundEnabled", "false")
        ## set the background image
        background.setProperty("Image", "set:BackgroundImage image:full_image")
        ## install this as the root GUI sheet
        CEGUI.System.getSingleton().setGUISheet(background)
        ########

            
        ## now we create a DefaultWindow which we will attach all the widgets to
        sheet = winMgr.createWindow("DefaultWindow", "root_wnd")
        ## attach this to the 'real' root
        background.addChildWindow(sheet)
        ########

        ## load tree layout
        TreeDemoWindow = winMgr.loadWindowLayout(SCHEME_DICT['LAYOUT_FILE_NAME'], False)
        ########
        
        background.addChildWindow(winMgr.getWindow('DemoWindow'))
        background.addChildWindow(winMgr.getWindow('MaterialEditor'))

        ## activate the background window
        background.activate()

        ### IMPORTANT 
        ###  - we have to create and hold a reference for our trees icon images
        drives = CEGUI.ImagesetManager.getSingleton().createImageset("DriveIcons.imageset")
        iconArray = []
        iconImages = [ "Artic",\
                       "Black",\
                       "Sunset",\
                       "DriveStack",\
                       "GlobalDrive",\
                       "Blue",\
                       "Lime",\
                       "Silver",\
                       "GreenCandy"]
        iconArray = [ drives.getImage(image) for image in iconImages ]
        self.iconArray = iconArray

        ## tree variables and events
        IMAGES_FILE_NAME = SCHEME_DICT['IMAGES_FILE_NAME']
        BRUSH_NAME = SCHEME_DICT['BRUSH_NAME']
        theTree = winMgr.getWindow('DemoWindow/Tree')
        theTree.initialise()

        theTree.subscribeEvent( CEGUI.Tree.EventSelectionChanged,\
                                self,\
                                "handleEventSelectionChanged")
        theTree.subscribeEvent( CEGUI.Tree.EventBranchOpened,\
                                self,\
                                "handleEventBranchOpened")
        theTree.subscribeEvent( CEGUI.Tree.EventBranchClosed,\
                                self,\
                                "handleEventBranchClosed")

        ########

        ## helper to add list to TreeCtrl
        def addListToTree(resourceList, parentItem, iconName):
            for r in resourceList:
                meshCtrlItem = CEGUI.TreeItem(r)
#                 meshCtrlItem.setIcon(drives.getImage(iconName))
                image = iconArray[ iconImages.index(iconName) ]          
                meshCtrlItem.setIcon( image ) 
                meshCtrlItem.setSelectionBrushImage(IMAGES_FILE_NAME, BRUSH_NAME)
                meshCtrlItem.AutoDeleted = False
                meshCtrlItem.setFont("BlueHighway-12")
                parentItem.addItem(meshCtrlItem)
                self.ListItems.append(meshCtrlItem)

        ## helper to create TreeCtrl item
        def createListItem(text, parentItem, iconName):
            meshCtrlItem = CEGUI.TreeItem(text)
#             meshCtrlItem.setIcon(drives.getImage(iconName))
            image = iconArray[ iconImages.index(iconName) ]          
            meshCtrlItem.setIcon( image ) 
            meshCtrlItem.setSelectionBrushImage(IMAGES_FILE_NAME, BRUSH_NAME)
            meshCtrlItem.AutoDeleted = False
            meshCtrlItem.setFont("BlueHighway-12")
            parentItem.addItem(meshCtrlItem)
            self.ListItems.append(meshCtrlItem)
            return meshCtrlItem

        # create root Item
        newTreeCtrlEntryLvl1 = createListItem("Ogre Resources",
                                theTree,"Silver")
        # resource items
        overlayCtrlItem = createListItem("overlays", 
                                 newTreeCtrlEntryLvl1,"Lime")
        particleCtrlItem = createListItem("particles",
                                 newTreeCtrlEntryLvl1,"Lime")
        matCtrlItem =  createListItem("materials",
                                newTreeCtrlEntryLvl1,"Lime")
        matScriptCtrlItem =  createListItem("scripts",
                                matCtrlItem,"Lime")
        matProgramCtrlItem = createListItem("programs", 
                                matCtrlItem,"Lime")

        # add resource lists to tree
        resManager = ogre.ResourceGroupManager.getSingleton()

        ## overlays
        overlayList = resManager.findResourceNames("General", "*.overlay")
        addListToTree(overlayList,overlayCtrlItem,"Silver")

        ## particles
        particleList = resManager.findResourceNames("General", "*.particle")
        addListToTree(particleList,particleCtrlItem,"Silver")

        ## materials
        matList = resManager.findResourceNames("General", "*.material")
        addListToTree(matList,matScriptCtrlItem,"Silver")
        for p in ['cg', 'glsl','hlsl','frag']: 
            mProgramCtrlItem = createListItem(p, matProgramCtrlItem,"Lime")
            matList = resManager.findResourceNames("General", "*.%s" %(p))
            addListToTree(matList,mProgramCtrlItem,"Silver")
        
        return True
        
        
    ## Create new frame listener
    def _createFrameListener(self):
        self.frameListener = GuiFrameListener(self.renderWindow, self.camera, self.GUIRenderer) #self.sceneManager)
        self.root.addFrameListener(self.frameListener)
        self.frameListener.showDebugOverlay(False)

    def handleEventSelectionChanged(self, args):
        selectedItem = args.treeItem
        if selectedItem:
            text = selectedItem.getText().c_str()
            itemList = text.split('.')
            if len(itemList)>1:
                if self.resourceEditor:
                    self.resourceEditor = None
                ext = itemList[1]
                if ext == "material":
                    self.resourceEditor = MaterialEditor(text)
                elif ext == "cg":
                    self.resourceEditor = MaterialEditor(text)
                elif ext == "glsl":
                    self.resourceEditor = MaterialEditor(text)
                elif ext == "hlsl":
                    self.resourceEditor = MaterialEditor(text)
                elif ext == "frag":
                    self.resourceEditor = MaterialEditor(text)
                elif ext == "overlay":
                    self.resourceEditor = MaterialEditor(text)
                elif ext == "particle":
                    self.resourceEditor = MaterialEditor(text)

        return True

    def handleEventBranchOpened(self, args):
        print "Branch Open" 
        return True

    def handleEventBranchClosed(self, args):
        print "Branch Closed" 
        return True
                        
    def handleQuit(self, e):
        self.frameListener.requestShutdown() 
        return True

if __name__ == '__main__':
    try:
        ta = GuiApplication()
        ta.go()
    except ogre.OgreException, e:
        print e

    
        

