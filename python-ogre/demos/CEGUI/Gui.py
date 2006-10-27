# /*
# -----------------------------------------------------------------------------
# This source file is part of OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http:##www.ogre3d.org/

# Copyright (c) 2000-2006 Torus Knot Software Ltd
# Also see acknowledgements in Readme.html

# You may use self sample code for anything you like, it is not covered by the
# LGPL like the rest of the engine.
# -----------------------------------------------------------------------------
# */

import Ogre as ogre
import CEGUI as CEGUI
import OIS as OIS
import SampleFramework

##----------------------------------------------------------------##
def convertOISMouseButtonToCegui( buttonID):
    if buttonID ==0:
        return CEGUI.LeftButton
    elif buttonID ==1:
        return CEGUI.RightButton
    elif buttonID ==2:
        return CEGUI.MiddleButton
    elif buttonID ==3:
        return CEGUI.X1Button
    else:
        return CEGUI.LeftButton

def _PointHack(x, y):
    return CEGUI.Vector2(x, y)
CEGUI.Point = _PointHack



class MouseListener ( OIS.MouseListener ):
    def __init__(self):
        OIS.MouseListener.__init__( self)

    ##----------------------------------------------------------------##
    def mouseMoved( self, arg ):
        CEGUI.System.getSingleton().injectMouseMove( arg.get_state().relX, arg.get_state().relY )
        return True

    ##----------------------------------------------------------------##
    def mousePressed(  self, arg, id ):
        CEGUI.System.getSingleton().injectMouseButtonDown(convertOISMouseButtonToCegui(id))
        return True

    ##----------------------------------------------------------------##
    def mouseReleased( self, arg, id ):
        CEGUI.System.getSingleton().injectMouseButtonUp(convertOISMouseButtonToCegui(id))
        return True

        
class GuiFrameListener(SampleFramework.FrameListener, OIS.KeyListener, OIS.MouseListener):

    def __init__(self, renderWindow, camera,  CEGUIRenderer):

        SampleFramework.FrameListener.__init__(self, renderWindow, camera, True, True, True)
        OIS.KeyListener.__init__(self)
        self.mouselistener = MouseListener ()
        #OIS.MouseListener.__init_(self)
        
        self.Mouse.setEventCallback(self.mouselistener)
        self.Keyboard.setEventCallback(self)
        
        self.ShutdownRequested = False
        self.GUIRenderer = CEGUIRenderer
        self.keepRendering = True   # whether to continue rendering or not
        self.numScreenShots = 0     # screen shot count

    ## Tell the frame listener to exit at the end of the next frame
    def requestShutdown( self ):
        self.ShutdownRequested = True

    def frameEnded(self, evt):
        if self.ShutdownRequested:
            return False
        else:
            return SampleFramework.FrameListener.frameEnded(self, evt)
            

    ##----------------------------------------------------------------##
    def keyPressed( self, arg ):
        if arg.key == OIS.KC_ESCAPE:
            self.ShutdownRequested = True
        CEGUI.System.getSingleton().injectKeyDown( arg.key )
        CEGUI.System.getSingleton().injectChar( arg.text )
        return True

    ##----------------------------------------------------------------##
    def keyReleased( self, arg ):
        CEGUI.System.getSingleton().injectKeyUp( arg.key )
        return True

class GuiApplication ( SampleFramework.Application ):
   
    def __init__(self):
        SampleFramework.Application.__init__(self)
        self.GUIRenderer=0
        self.GUIsystem =0
        self.EditorGuiSheet=0
        self.rttCounter=0
        self.DescriptionMap={}

        self.DescriptionMap[ "Demo8"] = "The main containing panel" 
        self.DescriptionMap[ "Demo8/Window1"] =  "A test window" 
        self.DescriptionMap[ "Demo8/Window1/Listbox"] = "A list box" 
        self.DescriptionMap[ "Demo8/Window1/Controls/Red"] =  "A colour slider" 
        self.DescriptionMap[ "Demo8/Window1/Controls/Green"] = "A colour slider" 
        self.DescriptionMap[ "Demo8/Window1/Controls/Blue"] =  "A colour slider" 
        self.DescriptionMap[ "Demo8/Window1/Controls/ColourSample"] =  "The colour that will be used for the selection when added to the list" 
        self.DescriptionMap[ "Demo8/Window1/Controls/Editbox"] =  "An edit box  self text will be added to the list" 
        self.DescriptionMap[ "Demo8/Window1/Controls/Add"] =  "Adds the text to the list" 
        self.DescriptionMap[ "Demo8/Window1/Controls/ins1"] =  "Some static text" 
    

    def __del__(self):
        ##
        ## important that things get deleted int he right order
        ##
        del self.camera
        del self.sceneManager
        del self.frameListener
        if self.EditorGuiSheet:
            CEGUI.WindowManager.getSingleton().destroyWindow(self.EditorGuiSheet) 
        del self.GUIsystem
        del self.GUIRenderer
        del self.root
        del self.renderWindow        
   
 
    ## Just override the mandatory create scene method
    def _createScene(self):
        sceneManager = self.sceneManager
        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)

        ## Create a skydome
        self.sceneManager.setSkyDome(True, "Examples/CloudySky", 5, 8) 

        ## Create a light
        l = self.sceneManager.createLight("MainLight") 
        ## Accept default settings: point light, white diffuse, just set position
        ## NB I could attach the light to a SceneNode if I wanted it to move automatically with
        ##  other objects, but I don't
        l.setPosition(20,80,50) 

        ## setup GUI system
        self.GUIRenderer = CEGUI.OgreCEGUIRenderer(self.renderWindow, 
            ogre.RENDER_QUEUE_OVERLAY, False, 3000, self.sceneManager) 

            
        self.GUIsystem = CEGUI.System(self.GUIRenderer) 

        logger = CEGUI.Logger.getSingleton()
        level = CEGUI.Informative
        logger.setLoggingLevel(level) 

        ogreHead = self.sceneManager.createEntity("Head", "ogrehead.mesh") 

        headNode = self.sceneManager.getRootSceneNode().createChildSceneNode() 
        headNode.attachObject(ogreHead) 


        ## Setup Render To Texture for preview window
        rttTex = self.root.getRenderSystem().createRenderTexture( "RttTex", 512, 512, ogre.TextureType.TEX_TYPE_2D, ogre.PixelFormat.PF_R8G8B8 ) 
        rttCam = self.sceneManager.createCamera("RttCam") 
        camNode = self.sceneManager.getRootSceneNode().createChildSceneNode("rttCamNode") 
        camNode.attachObject(rttCam) 
        rttCam.setPosition(0,0,200) 
        ##rttCam.setVisible(True) 
        v = rttTex.addViewport( rttCam ) 
        v.setOverlaysEnabled(False) 
        v.setClearEveryFrame( True ) 
        v.setBackgroundColour( ogre.ColourValue.Black ) 

            
        ## Retrieve CEGUI texture for the RTT
        rttTexture = self.GUIRenderer.createTexture( "RttTex") 

        rttImageSet = CEGUI.ImagesetManager.getSingleton().createImageset("RttImageset", rttTexture) 

        rttImageSet.defineImage( "RttImage", 
                CEGUI.Point(0.0, 0.0),
                CEGUI.Size(rttTexture.getWidth(), rttTexture.getHeight()),
                CEGUI.Point(0.0,0.0)) 

        ## load scheme and set up defaults
        CEGUI.SchemeManager.getSingleton().loadScheme("TaharezLookSkin.scheme") 
        self.GUIsystem.setDefaultMouseCursor("TaharezLook",  "MouseArrow") 
        self.GUIsystem.setDefaultFont( "BlueHighway-12") 
        print "Before Layout"
        sheet = CEGUI.WindowManager.getSingleton().loadWindowLayout("ogregui.layout")  
        print "After Layout"
        self.GUIsystem.setGUISheet(sheet) 

        objectComboBox = CEGUI.WindowManager.getSingleton().getWindow("OgreGuiDemo/TabCtrl/Page2/ObjectTypeList") 
        print "Done 1"    
        item = CEGUI.ListboxTextItem( "FrameWindow", 0) 
        print "Done 2"    
        objectComboBox.addItem(item) 
#         print "Done 3"    
#         del item
#         item = CEGUI.ListboxTextItem( "Horizontal Scrollbar", 1) 
#         print "Done 4"    
#         objectComboBox.addItem(item) 
#         print "Done 5"    
#         item2 = CEGUI.ListboxTextItem( "Vertical Scrollbar", 2) 
#         print "Done 6"    
#         objectComboBox.addItem(item2) 
#         print "Done 7"    
#         item3 = CEGUI.ListboxTextItem( "StaticText", 3) 
#         print "Done 8"    
#         objectComboBox.addItem(item3) 
#         print "Done 9"    
#         item4 = CEGUI.ListboxTextItem( "StaticImage", 4) 
#         objectComboBox.addItem(item4) 
#         item5 = CEGUI.ListboxTextItem( "Render to Texture", 5) 
#         objectComboBox.addItem(item5) 
#         print "Done 10"    
#         
        #self.setupEventHandlers() 
        print "Done 11"    
        
        
    ## Create new frame listener
    def _createFrameListener(self):
        self.frameListener = GuiFrameListener(self.renderWindow, self.camera, self.GUIRenderer) #self.sceneManager)
        self.root.addFrameListener(self.frameListener)
#        self.frameListener.showDebugOverlay(True)

    def setupEventHandlers(self):
        wmgr = CEGUI.WindowManager.getSingleton() 
        
         
        qb = wmgr.getWindow( "OgreGuiDemo/TabCtrl/Page1/QuitButton")
        qb.subscribeEvent(qb.EventClicked, self.handleQuit) 
        
        wmgr.getWindow( "OgreGuiDemo/TabCtrl/Page1/NewButton").subscribeEvent(
                CEGUI.PushButton.EventClicked, self.handleNew) 
        wmgr.getWindow( "OgreGuiDemo/TabCtrl/Page1/LoadButton").subscribeEvent(
                CEGUI.PushButton.EventClicked, self.handleLoad) 
        wmgr.getWindow( "OgreGuiDemo/TabCtrl/Page2/ObjectTypeList").subscribeEvent(
                CEGUI.Combobox.EventListSelectionAccepted,self.handleObjectSelection) 

#         wmgr.getWindow( "OgreGuiDemo/TabCtrl/Page1/QuitButton").subscribeEvent(
#                 CEGUI.PushButton.EventClicked, 
#                 CEGUI.Event.SubscriberSlot(GuiApplication.handleQuit, self)) 
#         wmgr.getWindow( "OgreGuiDemo/TabCtrl/Page1/NewButton").subscribeEvent(
#                 CEGUI.PushButton.EventClicked, 
#                 CEGUI.Event.SubscriberSlot(GuiApplication.handleNew, self)) 
#         wmgr.getWindow( "OgreGuiDemo/TabCtrl/Page1/LoadButton").subscribeEvent(
#                 CEGUI.PushButton.EventClicked, 
#                 CEGUI.Event.SubscriberSlot(GuiApplication.handleLoad, self)) 
#         wmgr.getWindow( "OgreGuiDemo/TabCtrl/Page2/ObjectTypeList").subscribeEvent(
#                 CEGUI.Combobox.EventListSelectionAccepted, 
#                 CEGUI.Event.SubscriberSlot(GuiApplication.handleObjectSelection, self)) 

    def setupEnterExitEvents(self, win):
        win.subscribeEvent(
            CEGUI.Window.EventMouseEnters, 
            CEGUI.Event.SubscriberSlot(GuiApplication.handleMouseEnters, self)) 
        win.subscribeEvent(
            CEGUI.Window.EventMouseLeaves, 
            CEGUI.Event.SubscriberSlot(GuiApplication.handleMouseLeaves, self)) 
        for i in range (win.getChildCount()):
            child = win.getChildAtIdx(i) 
            setupEnterExitEvents(child) 

    def setupLoadedLayoutHandlers(self):
        wmgr = CEGUI.WindowManager.getSingleton() 
        self.Red = wmgr.getWindow( "Demo8/Window1/Controls/Red")
        self.Green = wmgr.getWindow( "Demo8/Window1/Controls/Green") 
        self.Blue = wmgr.getWindow( "Demo8/Window1/Controls/Blue") 
        self.Preview = wmgr.getWindow( "Demo8/Window1/Controls/ColourSample") 
        self.List = wmgr.getWindow( "Demo8/Window1/Listbox") 
        self.EditBox = wmgr.getWindow( "Demo8/Window1/Controls/Editbox") 
        self.Tip = wmgr.getWindow( "Demo8/Window2/Tips") 
        
        self.Red.subscribeEvent(
              CEGUI.Scrollbar.EventScrollPositionChanged, 
              CEGUI.Event.Subscriber(GuiApplication.handleColourChanged, self)) 
        self.Green.subscribeEvent(
          CEGUI.Scrollbar.EventScrollPositionChanged, 
          CEGUI.Event.Subscriber(GuiApplication.handleColourChanged, self)) 
        self.Blue.subscribeEvent(
          CEGUI.Scrollbar.EventScrollPositionChanged, 
          CEGUI.Event.Subscriber(GuiApplication.handleColourChanged, self)) 
        
        wmgr.getWindow( "Demo8/Window1/Controls/Add").subscribeEvent(
          CEGUI.PushButton.EventClicked, 
          CEGUI.Event.Subscriber(GuiApplication.handleAdd, self)) 
        
        root = wmgr.getWindow("Demo8") 
        setupEnterExitEvents(root) 
      
    def createRttGuiObject(self):
        guiObjectName = "NewRttImage" + String(self.rttCounter) 
        
        rttImageSet = CEGUI.ImagesetManager.getSingleton().getImageset("RttImageset") 
        si = CEGUI.WindowManager.getSingleton().createWindow( "TaharezLook/StaticImage",  guiObjectName.c_str()) 
        si.setSize(CEGUI.UVector2( CEGUI.UDim(0.5, 0), CEGUI.UDim(0.4, 0))) 
        si.setProperty("Image", CEGUI.PropertyHelper.imageToString(rttImageSet.getImage( "RttImage"))) 
        self.rttCounter += 1 
        return si 

#     CEGUI.Window* createStaticImageObject(void)
#     {
#         static unsigned int siCounter = 0 
#         String guiObjectName = "NewStaticImage" + StringConverter.toString(siCounter) 

#         CEGUI.Imageset* imageSet = 
#             CEGUI.ImagesetManager.getSingleton().getImageset(
#                  "TaharezLook") 

#         CEGUI.Window* si = CEGUI.WindowManager.getSingleton().createWindow( "TaharezLook/StaticImage",  guiObjectName.c_str()) 
#         si.setSize(CEGUI.UVector2( CEGUI.UDim(0.2f, 0), CEGUI.UDim(0.2f, 0))) 
#         si.setProperty("Image", CEGUI.PropertyHelper.imageToString(
#             &imageSet.getImage( "ClientBrush"))) 

#         siCounter++ 

#         return si 
#     }

    def handleQuit(e):
        self.FrameListener.requestShutdown() 
        return True
    
    def handleNew( e):
        if self.EditorGuiSheet:
            CEGUI.WindowManager.getSingleton().destroyWindow(self.EditorGuiSheet) 
        
        self.EditorGuiSheet = CEGUI.WindowManager.getSingleton().createWindow("DefaultGUISheet", "NewLayout") 
        
        editorWindow = CEGUI.WindowManager.getSingleton().getWindow( "OgreGuiDemo2/MainWindow") 
        editorWindow.addChildWindow( self.EditorGuiSheet ) 
        return True

    def handleLoad(e):
        if(mEditorGuiSheet):
            CEGUI.WindowManager.getSingleton().destroyWindow(self.EditorGuiSheet) 
    
        selfEditorGuiSheet = CEGUI.WindowManager.getSingleton().loadWindowLayout("cegui8.layout")  
        self.setupLoadedLayoutHandlers() 
    
        editorWindow = CEGUI.WindowManager.getSingleton().getWindow( "OgreGuiDemo2/MainWindow") 
        editorWindow.addChildWindow(self.EditorGuiSheet) 
    
        return True


#     bool handleObjectSelection(const CEGUI.EventArgs& e)
#     {
#         static unsigned int windowNumber = 0 
#         static unsigned int vertScrollNumber = 0 
#         static unsigned int horizScrollNumber = 0 
#         static unsigned int textScrollNumber = 0 
#         String guiObjectName 
#         CEGUI.Window* window = 0 

#         ## Set a random position to place self object.
#         Real posX = Math.RangeRandom(0.0, 0.7)  
#         Real posY = Math.RangeRandom(0.1, 0.7)  

#         const CEGUI.WindowEventArgs& windowEventArgs = static_cast<const CEGUI.WindowEventArgs&>(e) 
#         CEGUI.ListboxItem* item = static_cast<CEGUI.Combobox*>(windowEventArgs.window).getSelectedItem() 

#         CEGUI.Window* editorWindow = CEGUI.WindowManager.getSingleton().getWindow( "OgreGuiDemo2/MainWindow") 

#         switch(item.getID())
#         {
#         case 0:
#             guiObjectName = "NewWindow" + StringConverter.toString(windowNumber) 
#             window = CEGUI.WindowManager.getSingleton().createWindow( "TaharezLook/FrameWindow",  guiObjectName.c_str()) 
#             window.setSize(CEGUI.UVector2(CEGUI.UDim(0.3f,0), CEGUI.UDim(0.3f,0))) 
#             window.setText( "New Window") 
#             windowNumber++ 
#             break 
#         case 1:
#             guiObjectName = "NewHorizScroll" + StringConverter.toString(horizScrollNumber) 
#             window = CEGUI.WindowManager.getSingleton().createWindow( "TaharezLook/HorizontalScrollbar",  guiObjectName.c_str()) 
#             window.setSize(CEGUI.UVector2(CEGUI.UDim(0.75f,0), CEGUI.UDim(0.03f,0))) 
#             horizScrollNumber++ 
#             break 
#         case 2:
#             guiObjectName = "NewVertScroll" + StringConverter.toString(vertScrollNumber) 
#             window = CEGUI.WindowManager.getSingleton().createWindow( "TaharezLook/VerticalScrollbar",  guiObjectName.c_str()) 
#             window.setSize(CEGUI.UVector2(CEGUI.UDim(0.03f,0), CEGUI.UDim(0.75f,0))) 
#             vertScrollNumber++ 
#             break 
#         case 3:
#             guiObjectName = "NewStaticText" + StringConverter.toString(textScrollNumber) 
#             window = CEGUI.WindowManager.getSingleton().createWindow( "TaharezLook/StaticText",  guiObjectName.c_str()) 
#             window.setSize(CEGUI.UVector2(CEGUI.UDim(0.25f,0), CEGUI.UDim(0.1f,0))) 
#             window.setText( "Example static text") 
#             textScrollNumber++ 
#             break 
#         case 4:
#             window = createStaticImageObject() 
#             break 
#         case 5:
#             window = createRttGuiObject() 
#             break 
#         } 

#         editorWindow.addChildWindow(window) 
#         window.setPosition(CEGUI.UVector2(CEGUI.UDim(posX, 0), CEGUI.UDim(posY, 0))) 

#         return True
#     }

    def handleColourChanged( e):
        self.Preview.setProperty("ImageColours",
            CEGUI.PropertyHelper.colourToString(CEGUI.colour(
                self.Red.getScrollPosition() / 255.0,
                self.Green.getScrollPosition() / 255.0,
                self.Blue.getScrollPosition() / 255.0))) 
        
        return True

    def handleAdd( e):
        listboxitem = CEGUI.ListboxTextItem (self.EditBox.getText()) 
        listboxitem.setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush") 
        listboxitem.setSelected(self.List.getItemCount() == 0) 
        listboxitem.setSelectionColours(
            CEGUI.PropertyHelper.stringToColourRect(self.Preview.getProperty("ImageColours"))) 
        self.List.addItem(listboxitem) 
        return True
 
    def handleMouseEnters( e):
        name = e.window.getName()
        try:
            self.Tip.setText( self.DescriptionMap[ name ])
        except:
            self.Tip.setText( "" )
        return True
        
    def handleMouseLeaves(e):
        self.Tip.setText( "") 
        return True
        
def FreehandleQuit(e):
    self.FrameListener.requestShutdown() 
    return True


if __name__ == '__main__':
    try:
        ta = GuiApplication()
        ta.go()
    except ogre.Exception, e:
        print e
        

