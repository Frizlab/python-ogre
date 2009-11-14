## /*
## -----------------------------------------------------------------------------
## This source file is part of OGRE
## (Object-oriented Graphics Rendering Engine)
## For the latest info, see http:##www.ogre3d.org/

## Copyright (c) 2000-2007 The OGRE Team
## Also see acknowledgements in Readme.html

## You may use this sample code for anything you like, it is not covered by the
## LGPL like the rest of the engine.
## -----------------------------------------------------------------------------
## */

## /*
## -----------------------------------------------------------------------------
## Filename:    QuickGUIDemo.h
## Description: A place for me to try out stuff with OGRE.
## -----------------------------------------------------------------------------
## */

## THIS NEEDS A CLEANUP TO MATCH THE CURRENT API's///

import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as Ogre
import ogre.io.OIS as ois
import ogre.gui.QuickGUI as gui
import SampleFramework as sf
import os

class CallBack ( gui.EventHandlerSlot ):
    """ Callback class for user events in QuickGUI"""
    def __init__(self, function=""):
        gui.EventHandlerSlot.__init__(self)
        self.function=function
    def execute(self, args):
        if self.function:
            self.function(args)
            
class GuiFrameListener ( sf.FrameListener, ois.MouseListener, ois.KeyListener ):
    ## NB using buffered input, this is the only change
    def __init__( self, win,  cam):
        sf.FrameListener.__init__(self, win, cam, True, True, True) 
        ois.KeyListener.__init__(self)
        ois.MouseListener.__init__(self)
        self.mShutdownRequested = False
        self.robotAnimationState=None
        self.Mouse.setEventCallback(self)
        self.Keyboard.setEventCallback(self)
        self.mGUIManager = None
        self.mouseOverTB = None
        self.debugTB = None
     

    ## Tell the frame listener to exit at the end of the next frame
    def requestShutdown(self):
        self.mShutdownRequested = True
    
    def setAnimationState(self, asIN):
        self.robotAnimationState = asIN
        
    def frameStarted(self, evt):
        if (self.robotAnimationState != None): self.robotAnimationState.addTime(evt.timeSinceLastFrame)
    
        return sf.FrameListener.frameStarted(self,evt)

    def frameEnded(self, evt):
        if (self.mShutdownRequested):
            return False
        else:
            return sf.FrameListener.frameEnded(self,evt)
    ##----------------------------------------------------------------##
    def mouseMoved( self, arg ):
        ms= arg.get_state()
        self.mGUIManager.injectMousePosition( ms.X.abs, ms.Y.abs )
        return True

    ##----------------------------------------------------------------##
    def mousePressed( self, arg,  _id ):
        b = gui.MouseButtonID.values[_id]
        self.mGUIManager.injectMouseButtonDown(b)
        return True

    ##----------------------------------------------------------------##
    def mouseReleased( self, arg, _id ):
        b = gui.MouseButtonID.values[_id]
        self.mGUIManager.injectMouseButtonUp(b)
        return True

    ##----------------------------------------------------------------##
    def  keyPressed( self, arg ):
        if( arg.key == ois.KC_ESCAPE ):
            self.mShutdownRequested = True
        elif ( arg.key == ois.KC_UP ):
            camPos = self.camera.getPostion()
            self.camera.setPosition(camPos.x, camPos.y, camPos.z-5)
        elif ( arg.key == ois.KC_DOWN ):
            camPos = self.camera.getPostion()
            self.camera.setPosition(camPos.x, camPos.y, camPos.z+5)
        
        ## Now convert from OIS keycode to QuickGUI one.            
        k = gui.KeyCode.values[arg.key]
        self.mGUIManager.injectKeyDown( k )
        self.mGUIManager.injectChar( int(arg.text) )
        return True

    ##----------------------------------------------------------------##
    def keyReleased( self, arg ):
        k = gui.KeyCode.values[arg.key]
        self.mGUIManager.injectKeyUp( k )
        return True

class QuickGUIDemoApp (sf.Application):
    def __del__ (self ):
        self.guiroot.destroyGUIManager(self.mGUIManager)
        if self.guiroot: del self.guiroot
        if self.mGUIManager: del self.mGUIManager
        sf.Application.__del__(self)

    def _setUpResources(self):
        sf.setupLogging()
        sf.info ("Entering _setupresources")
        # register the quickgui script handler..
        gui.registerScriptReader()

        # load the default resources
        sf.Application._setUpResources ( self )
        
        # Now load any extra resource locations that we might need..  
        Ogre.ResourceGroupManager.getSingleton().addResourceLocation("media/textures","FileSystem", "General")
        Ogre.ResourceGroupManager.getSingleton().addResourceLocation("media/qgui.core.zip", "Zip")
        #Ogre.ResourceGroupManager.getSingleton().addResourceLocation("media","FileSystem", "General")
        sf.info ( "Done _setup resources")
                    
    ## Just override the mandatory create scene method
    def _createScene(self):
        sf.info ("Entering _createScene")
        self.mDebugDisplayShown=True
        ## Set ambient light
        self.sceneManager.setAmbientLight((0.5, 0.5, 0.5))

        ## Create a SkyBox
        self.sceneManager.setSkyBox(True,"Examples/EveningSkyBox")

        ## Create a light
        l = self.sceneManager.createLight("MainLight")
        ## Accept default settings: point light, white diffuse, just set position
        ## NB I could attach the light to a SceneNode if I wanted it to move automatically with
        ##  other objects, but I don't
        l.setPosition(0,175,550)
        
        self.robot = self.sceneManager.createEntity("ChuckNorris", "robot.mesh")

        robotNode = self.sceneManager.getRootSceneNode().createChildSceneNode()
        robotNode.attachObject(self.robot)
        robotNode.yaw(Ogre.Radian(Ogre.Degree(-90)))
        robotNode.scale = (1.25,1.25,1.25)
        
        ##  This ensures the camera doesn't move when we move the cursor.
        #3 However we do the same thing by setting MenuMode to True in the frameListerner further down
        self.camera.setAutoTracking(True, robotNode)

        plane = Ogre.Plane( Ogre.Vector3().UNIT_Y, 0 )
        Ogre.MeshManager.getSingleton().createPlane("ground",
           Ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, plane,
           1500,1500,20,20,True,1,5,5,Ogre.Vector3().UNIT_Z)
        ground = self.sceneManager.createEntity( "GroundEntity", "ground" )
        self.sceneManager.getRootSceneNode().createChildSceneNode().attachObject(ground)
        ground.setMaterialName("Examples/Rockwall")
        ground.setCastShadows(False)

        self.camera.setPosition(0,125,500)
        self.camera.pitch(Ogre.Radian(Ogre.Degree(-15)))

        ## Setup Render To Texture for preview window
        texture = Ogre.TextureManager.getSingleton().createManual( "RttTex", 
                        Ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, Ogre.TEX_TYPE_2D, 
                        512, 512, 0, Ogre.PixelFormat.PF_R8G8B8, Ogre.TU_RENDERTARGET )
        self.rttTex = texture.getBuffer().getRenderTarget()

        
        ## From CEGUI example.  The camera position doesn't fit the robot setup, so I changed it some.
        rttCam = self.sceneManager.createCamera("RttCam")
        camNode = self.sceneManager.getRootSceneNode().createChildSceneNode("rttCamNode")
        camNode.attachObject(rttCam)
        rttCam.setPosition(0,75,225)

        v = self.rttTex.addViewport( rttCam )
        ## Alternatively, use the main camera for the self.rttText, imitating the main screen
        ##Viewport *v = self.rttTex.addViewport( self.camera )
        v.setOverlaysEnabled( False )
        v.setClearEveryFrame( True )
        v.setBackgroundColour( Ogre.ColourValue().Black )
        sf.info ("Creating gui")
        self.guiroot = gui.Root()
        sf.info ("GUI OK, loading types")
        gui.SkinTypeManager.getSingleton().loadTypes()
        sf.info ("done load types")
        self.desc = gui.GUIManagerDesc()
        sf.info ("done self.desc")
#     
        self.mGUIManager = gui.Root.getSingleton().createGUIManager(self.desc)
        sf.info ("Created guimanager")
        self.mGUIManager.setSceneManager(self.sceneManager) 
        self.mGUIManager.viewport = self.camera.getViewport()
# 
        casi = self.robot.getAllAnimationStates().getAnimationStateIterator()
        state = 0
        while( casi.hasMoreElements() ):
            animName = casi.getNext().getAnimationName()
            if state == 0:
                self.robotAnimationState = self.robot.getAnimationState(animName)
                self.robotAnimationState.setEnabled(True)
                self.robotAnimationState.setTimePosition(0.0)
                self.robotAnimationState.setLoop(True)
            state+=1

        self.createGUI()
        
    def MakeCallback ( self, function ):
        cb = CallBack()
        cb.function = function
        self.callbacks.append( cb )
        return cb
        
    def makeWidget ( self, widgetType, dimensions, name=None, **attributes ):
        """ helper function for making widgets -- shows a cool feature of python in 
        that you can go looking for functions by name
        """
        dm = gui.DescManager.getSingletonPtr()
        func = getattr(dm, "getDefault"+widgetType+"Desc") 
        # create the widget description
        widget = func() 
        widget.resetToDefault()
        if name:
            widget.widget_name = name
        else:
            widget.widget_name = widgetType
        widget.widget_dimensions = dimensions
        # now handle any attributes
        for a in attributes.keys():
            try:
                setattr (widget, a, attributes[a] )
            except:
                print "Unable to set attribute ", a, "on widget", widget
        # find the function to create this widget
        func = getattr(self.mSheet, "create" + widgetType )
        return  func(widget)
    
    def addMenuItem ( self, menu, name ):
        dm = gui.DescManager.getSingletonPtr()
        md = dm.getDefaultMenuDesc()
        md.resetToDefault()
        md.widget_name = name
        md.textDesc.segments.append(gui.TextSegment("micross.12",gui.ColourValue().Black,name))
        ##menu.createMenu(md)

    def createGUI ( self ):  
        self.callbacks=[]
        sf.info ("Creating GUI")
        dm = gui.DescManager.getSingletonPtr()
        
        sd = dm.getDefaultSheetDesc()
        sd.resetToDefault()
        sd.widget_name = "QuickGui.MainSheet"
        sd.widget_dimensions.size = gui.Size(800,600)
        self.mSheet = gui.SheetManager.getSingleton().createSheet(sd)
        self.mSheet.addWidgetEventHandler(gui.WIDGET_EVENT_SIZE_CHANGED,self.MakeCallback( self.onSheetResized) )
        
		
        
        self.TreeView = self.makeWidget ( "TreeView", gui.Rect(0,75,200,300) )
        self.Panel = self.makeWidget ( "Panel", gui.Rect(225,75,150,217), name="MainPanel", 
                        widget_resizeFromBottom=True, widget_resizeFromRight = True )
        #self.bd = self.makeWidget( "ScrollButton", gui.Rect(400,250,75,23) )
        self.tb = self.makeWidget( "ToolBar", gui.Rect(0,0,800,20), name="TestToolBar")
        ##self.addMenuItem ( self.tb, "File")
        self.combo = self.makeWidget ( "ComboBox", gui.Rect(400,100,125,20), combobox_dropDownMaxHeight = 60 )
        
        if False:
          pd = descFactory.getInstance("DefaultPanelDesc")
          bd = descFactory.getInstance("DefaultButtonDesc")
          cbd = descFactory.getInstance("DefaultCheckBoxDesc")
          ltiDesc = descFactory.getInstance("DefaultListTextItemDesc")
          vsd = descFactory.getInstance("DefaultVScrollBarDesc")
          tad = descFactory.getInstance("DefaultTextAreaDesc")
          combod = descFactory.getInstance("DefaultComboBoxDesc")
          md = descFactory.getInstance("DefaultMenuDesc")
          textBoxD = descFactory.getInstance("DefaultTextBoxDesc")
          hsd = descFactory.getInstance("DefaultHScrollBarDesc")
          progDesc =descFactory.getInstance("DefaultProgressBarDesc")
          listDesc = descFactory.getInstance("DefaultListDesc")
          mld = descFactory.getInstance("DefaultMenuLabelDesc")
          tbd = descFactory.getInstance("DefaultToolBarDesc")
          cd = descFactory.getInstance("DefaultConsoleDesc")
          tvd = descFactory.getInstance("DefaultTreeViewDesc")
          sf.info ("CreateGUI part 1")

          md.resetToDefault()
          md.widget_name = "File"
          md.textDesc.segments.append(gui.TextSegment("micross.12",Ogre.ColourValue().Black,"File"))
          tb.createMenu(md)

          md.resetToDefault()
          md.widget_name = "Edit"
          md.textDesc.segments.append(gui.TextSegment("micross.12",Ogre.ColourValue().Black,"Edit"))
          tb.createMenu(md)

          md.resetToDefault()
          md.widget_name = "Format"
          md.textDesc.segments.append(gui.TextSegment("micross.12",Ogre.ColourValue().Black,"Format"))
          m1 = tb.createMenu(md)

          md.resetToDefault()
          md.widget_name = "WordWrap"
          md.textDesc.segments.append(gui.TextSegment("micross.12",Ogre.ColourValue().Black,"Word Wrap"))
          m2 = m1.createSubMenu(md)

          mld.resetToDefault()
          mld.widget_name = "Font"
          mld.textDesc.segments.append(gui.TextSegment("micross.12",Ogre.ColourValue().Black,"Font..."))
          m1.createMenuLabel(mld)

          mld.resetToDefault()
          mld.widget_name = "Item1"
          mld.textDesc.segments.append(gui.TextSegment("micross.12",Ogre.ColourValue().Black,"Item 1"))
          m2.createMenuLabel(mld)

          md.resetToDefault()
          md.widget_name = "Item2"
          md.textDesc.segments.append(gui.TextSegment("micross.12",Ogre.ColourValue().Black,"Item 2"))
          m3 = m2.createSubMenu(md)

          mld.resetToDefault()
          mld.widget_name = "Item2.1"
          mld.textDesc.segments.append(gui.TextSegment("micross.12",Ogre.ColourValue().Green,"Item 2.1"))
          m3.createMenuLabel(mld)


          ltiDesc.resetToDefault()
          ltiDesc.textDesc.horizontalTextAlignment = gui.TEXT_ALIGNMENT_HORIZONTAL_CENTER
          t =gui.VectorTextSegment()
          t.append(gui.TextSegment("micross.12",Ogre.ColourValue().White,"CB Item 1"))
          ltiDesc.textDesc.segments=t
          comboBox.createItem(ltiDesc)

          ltiDesc.resetToDefault()
          t =gui.VectorTextSegment()
          t.append(gui.TextSegment("micross.12",Ogre.ColourValue().White,"CB Item 2"))
          ltiDesc.textDesc.segments=t
          comboBox.createItem(ltiDesc)

          ltiDesc.resetToDefault()
          t =gui.VectorTextSegment()
          t.append(gui.TextSegment("micross.12",Ogre.ColourValue().White,"CB Item 3"))
          ltiDesc.textDesc.segments=t
          comboBox.createItem(ltiDesc)

          textBoxD.widget_name = "TestTextBox"
          textBoxD.textbox_defaultFontName = "micross.12"
          textBoxD.widget_dimensions = gui.Rect(350,350,100,20)
          textBoxD.textbox_maskText = True
          textBoxD.textbox_maskSymbol = 42
          self.mGUIManager.getActiveSheet().createTextBox(textBoxD)

          tad.widget_name = "TestArea1"
          tad.textarea_defaultFontName = "micross.16"
          tad.widget_dimensions = gui.Rect(575,300,200,200)
          self.mGUIManager.getActiveSheet().createTextArea(tad)

          cd.resetToDefault()
          cd.widget_dimensions = gui.Rect(300,300,200,200)

          vsd.resetToDefault() 
          vsd.widget_name = "TestVSD1" 
          vsd.widget_dimensions = gui.Rect(50,50,15,100) 
          win.createVScrollBar(vsd) 

          vsd.widget_name = "TestVSD2" 
          vsd.vscrollbar_scrollBarButtonLayout = gui.VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_UP 
          vsd.widget_dimensions = gui.Rect(70,50,15,100) 
          win.createVScrollBar(vsd) 

          vsd.widget_name = "TestVSD3" 
          vsd.vscrollbar_scrollBarButtonLayout = gui.VSCROLL_BAR_BUTTON_LAYOUT_ADJACENT_DOWN 
          vsd.widget_dimensions = gui.Rect(90,50,15,100) 
          win.createVScrollBar(vsd) 

          vsd.widget_name = "TestVSD4" 
          vsd.vscrollbar_scrollBarButtonLayout = gui.VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_BOTH 
          vsd.widget_dimensions = gui.Rect(110,50,15,100) 
          win.createVScrollBar(vsd) 

          vsd.widget_name = "TestVSD5" 
          vsd.vscrollbar_scrollBarButtonLayout = gui.VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_DOWN 
          vsd.widget_dimensions = gui.Rect(130,50,15,100) 
          win.createVScrollBar(vsd) 

          vsd.widget_name = "TestVSD6" 
          vsd.vscrollbar_scrollBarButtonLayout = gui.VSCROLL_BAR_BUTTON_LAYOUT_MULTIPLE_UP 
          vsd.widget_dimensions = gui.Rect(150,50,15,100) 
          win.createVScrollBar(vsd) 

          vsd.widget_name = "TestVSD7" 
          vsd.vscrollbar_scrollBarButtonLayout = gui.VSCROLL_BAR_BUTTON_LAYOUT_NONE 
          vsd.widget_dimensions = gui.Rect(170,50,15,100) 
          win.createVScrollBar(vsd) 

          hsd.resetToDefault() 
          hsd.widget_name = "TestHSD1" 
          hsd.widget_dimensions = gui.Rect(50,160,100,15) 
          win.createHScrollBar(hsd) 

          progDesc.widget_name = "Prog"
          progDesc.textDesc.segments.append(gui.TextSegment("micross.12",Ogre.ColourValue().White,"Progress Bar"))
          progDesc.textDesc.horizontalTextAlignment = gui.TEXT_ALIGNMENT_HORIZONTAL_CENTER
          progDesc.widget_dimensions = gui.Rect(100,400,200,35)
          progDesc.progressbar_fillDirection = gui.PROGRESSBAR_FILLS_POSITIVE_TO_NEGATIVE
          progDesc.progressbar_clippingEdge = gui.PROGRESSBAR_CLIP_RIGHT_TOP
          progDesc.progressbar_layout = gui.PROGRESSBAR_LAYOUT_VERTICAL
          self.mGUIManager.getActiveSheet().createProgressBar(progDesc)

          bd.resetToDefault()
          bd.widget_name = "Minus"
          bd.widget_dimensions = gui.Rect(100,440,100,35)
          bd.textDesc.segments.append(gui.TextSegment("micross.12",Ogre.ColourValue().Green,"Minus"))
          bd.textDesc.horizontalTextAlignment = gui.TEXT_ALIGNMENT_HORIZONTAL_CENTER
          minus = self.mGUIManager.getActiveSheet().createButton(bd)
          
          minus.addWidgetEventHandler(gui.WIDGET_EVENT_MOUSE_BUTTON_UP,self.MakeCallback (self.test1) )

          bd.widget_name = "Plus"
          bd.widget_dimensions = gui.Rect(200,440,100,35)
          bd.textDesc.segments.append(gui.TextSegment("micross.12",Ogre.ColourValue().Green,"Plus"))
          plus = self.mGUIManager.getActiveSheet().createButton(bd)
          
          plus.addWidgetEventHandler(gui.WIDGET_EVENT_MOUSE_BUTTON_UP,self.MakeCallback (self.test2) )

          listDesc.widget_name = "TestList"
          listDesc.widget_dimensions = gui.Rect(330,400,200,100)
          listDesc.list_listItemHeight = 20
          listDesc.list_supportMultiSelect = True
          list_ = self.mGUIManager.getActiveSheet().createList(listDesc)
          
          ## PythonOgre change - we don't have a clear() function on vectors so have
          ## to create a new one each time
          t =gui.VectorTextSegment()
          t.append(gui.TextSegment("micross.12",Ogre.ColourValue().White,"Item 1"))
          ltiDesc.textDesc.segments= t
          list_.createItem(ltiDesc)

          t =gui.VectorTextSegment()
          t.append(gui.TextSegment("micross.12",Ogre.ColourValue().Green,"Item 2"))
          ltiDesc.textDesc.segments=t
          list_.createItem(ltiDesc)

          t =gui.VectorTextSegment()
          t.append(gui.TextSegment("micross.12",Ogre.ColourValue().Red,"Item 3"))
          ltiDesc.textDesc.segments=t
          list_.createItem(ltiDesc)

          t =gui.VectorTextSegment()
          t.append(gui.TextSegment("micross.12",Ogre.ColourValue().Blue,"Item 4"))
          ltiDesc.textDesc.segments=t
          list_.createItem(ltiDesc)

          t =gui.VectorTextSegment()
          t.append(gui.TextSegment("micross.12",Ogre.ColourValue().Black,"Item 5"))
          ltiDesc.textDesc.segments=t
          list_.createItem(ltiDesc)

          cbd.widget_name = "TestCheckBox"
          cbd.widget_dimensions.size = gui.Size(25,25)
          cbd.widget_dimensions.position = gui.Point(250,350)
          self.mGUIManager.getActiveSheet().createCheckBox(cbd)
          
        self.mGUIManager.setActiveSheet(self.mSheet);
        
        if False:
            pd = dm.getDefaultPanelDesc()
            pd.resetToDefault()
            pd.widget_name = "AnimationViewerControlPanel"
            pd.widget_dimensions = gui.Rect(0,450,600,150)
            pd.containerwidget_supportScrollBars = False
            self.mControlPanel = self.mSheet.createPanel(pd)
            # CREATE IMAGES
            self.mGridMaterial = "AnimationViewer.Grid.Material"
            self.mDefaultCameraRTTName = "AnimationViewer.DefaultCam.RTT"
            self.mSideCameraRTTName = "AnimationViewer.SideCam.RTT"
            self.mTopCameraRTTName = "AnimationViewer.TopCam.RTT"
            self.mFrontCameraRTTName = "AnimationViewer.FrontCam.RTT"
            
            id = dm.getDefaultImageDesc()
            id.resetToDefault()
            id.image_updateEveryFrame = True
            id.widget_name = "TopView"
            id.image_imageName = self.mTopCameraRTTName
            id.widget_dimensions.size = gui.Size(300,225)
            self.mTopViewImage = self.mSheet.createImage(id)

            id.widget_name = "SideView"
            id.widget_dimensions.position = gui.Point(300,0)
            id.image_imageName = self.mSideCameraRTTName
            self.mSideViewImage = self.mSheet.createImage(id)

            id.widget_name = "FrontView"
            id.widget_dimensions.position = gui.Point(0,225)
            id.image_imageName = self.mFrontCameraRTTName
            self.mFrontViewImage = self.mSheet.createImage(id)

            id.widget_name = "PerspectiveView"
            id.widget_dimensions.position = gui.Point(300,225)
            id.image_imageName = self.mDefaultCameraRTTName
            self.mPerspectiveViewImage = self.mSheet.createImage(id)

    def test1(self, args):
      pb = self.mGUIManager.getActiveSheet().findWidget("Prog")
      pb.setProgress(pb.getProgress() - 10)

    def test2(self, args):
      pb = self.mGUIManager.getActiveSheet().findWidget("Prog")
      pb.setProgress(pb.getProgress() + 10)
      self.mGUIManager.getActiveSheet().getWindow("Window1").saveTextureToFile("test.png")
	
    def onSheetResized(self,  args):
        cpX = (self.mMeshList.getPosition().x / 2.0) - (self.mControlPanel.getWidth() / 2.0)
        cpY = self.mSheet.getHeight() - self.mControlPanel.getHeight()
        self.mControlPanel.setPosition(gui.Point(cpX,cpY))

        availableWidth = self.mMeshList.getPosition().x
        availableHeight = self.mControlPanel.getPosition().y
        newImageSize = gui.Size(availableWidth / 2.0,availableHeight / 2.0)

        self.mDefaultCamera.setAspectRatio(newImageSize.width / newImageSize.height)
        self.mSideCamera.setAspectRatio(newImageSize.width / newImageSize.height)
        self.mTopCamera.setAspectRatio(newImageSize.width / newImageSize.height)
        self.mFrontCamera.setAspectRatio(newImageSize.width / newImageSize.height)

        self.mTopViewImage.setSize(newImageSize)
        self.mSideViewImage.setSize(newImageSize)
        self.mFrontViewImage.setSize(newImageSize)
        self.mPerspectiveViewImage.setSize(newImageSize)

        self.mSideViewImage.setPosition(gui.Point(availableWidth / 2.0,0))
        self.mFrontViewImage.setPosition(gui.Point(0,availableHeight / 2.0))
        self.mPerspectiveViewImage.setPosition(gui.Point(availableWidth / 2.0,availableHeight / 2.0))

        offset = gui.Point(5,newImageSize.height - 25)
        self.mTopViewLabel.setPosition(self.mTopViewImage.getPosition() + offset)
        self.mSideViewLabel.setPosition(self.mSideViewImage.getPosition() + offset)
        self.mFrontViewLabel.setPosition(self.mFrontViewImage.getPosition() + offset)
        self.mPerspectiveViewLabel.setPosition(self.mPerspectiveViewImage.getPosition() + offset)
        
    def _createFrameListener(self):
        self.frameListener= GuiFrameListener(self.renderWindow, self.camera)
        ## This ensures the mouse doesn't move the camera.
        self.frameListener.setMenuMode ( True )
        self.root.addFrameListener(self.frameListener)
        self.frameListener.setAnimationState(self.robotAnimationState)
        self.frameListener.mGUIManager = self.mGUIManager

if __name__ == '__main__':
    try:
        application = QuickGUIDemoApp()
        application.go()
    except Ogre.OgreException, e:
        print e