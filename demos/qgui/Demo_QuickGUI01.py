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
import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as Ogre
import ogre.io.OIS as ois
import ogre.gui.QuickGUI as gui
import SampleFramework as sf

class CallBack ( gui.MemberFunctionSlot ):
    """ Callback class for user events in QuickGUI"""
    def __init__(self, function=""):
        gui.MemberFunctionSlot.__init__(self)
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
    
    def setAnimationState(self, as):
        self.robotAnimationState = as

    def setVars (self, gm, tb ):
        self.mGUIManager = gm
        self.mouseOverTB = tb
        
    def frameStarted(self, evt):
        if (self.robotAnimationState != None): self.robotAnimationState.addTime(evt.timeSinceLastFrame)

        if( self.mGUIManager != None ) :
            self.mGUIManager .injectTime(evt.timeSinceLastFrame)

        if(self.mouseOverTB != None):
            self.mouseOverTB.setText(self.mGUIManager.getMouseOverWidget().getInstanceName())
        
        if(self.debugTB == None):
            self.debugTB = self.mGUIManager.getActiveSheet().getChildWidget("DebugTextBox") 
        else:
            self.debugTB.setText(self.mGUIManager.getMouseOverWidget().getOffset()) 
    
        return sf.FrameListener.frameStarted(self,evt)

    def frameEnded(self, evt):
        if (self.mShutdownRequested):
            return False
        else:
            return sf.FrameListener.frameEnded(self,evt)
    ##----------------------------------------------------------------##
    def mouseMoved( self, arg ):
        ms= arg.get_state()
        self.mGUIManager.injectMouseMove( ms.X.rel, ms.Y.rel )
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
            self.ShutdownRequested = True
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
        self.mGUIManager.injectKeyDown( k )
        return True



class QuickGUIDemoApp (sf.Application):

# #     def _setUpResources(self):
# #         """This sets up Ogre's resources, which are different for QuickGUI
# #         """
# #         config = Ogre.ConfigFile()
# #         config.load('resources.cfg.quickgui' ) 
# #         seci = config.getSectionIterator()
# #         while seci.hasMoreElements():
# #             SectionName = seci.peekNextKey()
# #             Section = seci.getNext()
# #             for item in Section:
# #                 Ogre.ResourceGroupManager.getSingleton().\
# #                     addResourceLocation(item.value, item.key, SectionName)
                    
    ## Just override the mandatory create scene method
    def _createScene(self):
        self.mGUIManager = gui.GUIManager()
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
# # #         self.camera.setAutoTracking(True, robotNode)

        plane = Ogre.Plane( Ogre.Vector3.UNIT_Y, 0 )
        Ogre.MeshManager.getSingleton().createPlane("ground",
           Ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, plane,
           1500,1500,20,20,True,1,5,5,Ogre.Vector3.UNIT_Z)
        ground = self.sceneManager.createEntity( "GroundEntity", "ground" )
        self.sceneManager.getRootSceneNode().createChildSceneNode().attachObject(ground)
        ground.setMaterialName("Examples/Rockwall")
        ground.setCastShadows(False)

        self.camera.setPosition(0,125,500)
        self.camera.pitch(Ogre.Radian(Ogre.Degree(-15)))
# 
        ## Setup Render To Texture for preview window
        self.rttTex = self.root.getRenderSystem().createRenderTexture( "self.rttTex",\
                        512, 512, Ogre.TextureType.TEX_TYPE_2D, Ogre.PixelFormat.PF_R8G8B8 )
        ## From CEGUI example.  The camera position doesn't fit the robot setup, so I changed it some.
        rttCam = self.sceneManager.createCamera("RttCam")
        camNode = self.sceneManager.getRootSceneNode().createChildSceneNode("rttCamNode")
        camNode.attachObject(rttCam)
        rttCam.setPosition(0,75,225)
        ##rttCam.setVisible(True)

        v = self.rttTex.addViewport( rttCam )
        ## Alternatively, use the main camera for the self.rttText, imitating the main screen
        ##Viewport *v = self.rttTex.addViewport( self.camera )
        v.setOverlaysEnabled( False )
        v.setClearEveryFrame( True )
        v.setBackgroundColour( Ogre.ColourValue.Black )

        self.mGUIManager.init(self.camera.getViewport(), "qgui" ) 
        self.mGUIManager.setSceneManager(self.sceneManager) 

        self.createGUI()
        
    def MakeCallback ( self, function ):
        cb = CallBack()
        cb.function = function
        self.callbacks.append( cb )
        return cb
        
    def createGUI(self):
        self.callbacks=[]
        Rect = gui.Rect
        Point = gui.Point
        Size = gui.Size
        self.mSheet = self.mGUIManager.getDefaultSheet()
#         self.mSheet.setDefaultFont ("acmesa.12")
                    
        ## Main Menu and it's MenuLists
#         topMenu = self.mSheet.createMenu() 
#         topMenu.setDimensions(Rect(0,0,800,25))
#         
#         fileList = topMenu.addMenuList("File",0,60)
#         exitListItem = fileList.addListItem("Exit")
#         exitListItem.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback (self.evtHndlr_exitListItem ) )
#         
#         testList = topMenu.addMenuList("Tests",60,160) 
#         pointListItem = testList.addListItem("Main Page") 
#         pointListItem.addImage(Rect(0,0,12,25),"pointmode.png") 
#         pointListItem.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_CameraPoint) )
#         
#         wireframeListItem = testList.addListItem("Position/Size Test") 
#         wireframeListItem.addImage(Rect(0,0,12,25),"wireframemode.png") 
#         wireframeListItem.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_CameraWireFrame) )
#         
#         solidListItem = testList.addListItem("Solid") 
#         solidListItem.addImage(Rect(0,0,12,25),"solidmode.png") 
#         solidListItem.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_CameraSolid) )
# 
#         barColorList = topMenu.addMenuList("Progress Bar Color",220,200) 
#         redListItem = barColorList.addListItem("Red") 
#         redListItem.addImage(Rect(20,0,160,20),"listitem.red.png") 
#         redListItem.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_pbRed) )
#         
#         greenListItem = barColorList.addListItem("Green") 
#         greenListItem.addImage(Rect(20,0,160,20),"listitem.green.png") 
#         greenListItem.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_pbGreen) )
#          
#         blueListItem = barColorList.addListItem("Blue") 
#         blueListItem.addImage(Rect(20,0,160,20),"listitem.blue.png") 
#         blueListItem.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_pbBlue) )
# 
#         otherList = topMenu.addMenuList("Other",420,200) 
#         textColorListItem = otherList.addListItem("Text Color") 
#         textColorListItem.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_setTextWhite) )
#         
#         tcProperties = textColorListItem.addNStateButton(Rect(170,2.5,15,15)) 
#         tcProperties.addState("OpenProperties","properties.png") 
#         tcProperties.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_showSetTextDialog) )
#         
#         RenderStatsListItem = otherList.addListItem("Render Stats") 
#         toggleRenderStats = RenderStatsListItem.addNStateButton(Rect(170,2.5,15,15)) 
#         toggleRenderStats.addState("checked","qgui.unchecked.png") 
#         toggleRenderStats.addState("unchecked","qgui.checked.png") 
#         toggleRenderStats.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_toggleDebugDisplay))
# 
        ## Logos
        logoImage = self.mSheet.createImage()
#         logoImage.setDimensions(Rect(16,42,240,180))
        logoImage.setDimensions(Rect(-240,-180,1,1))

        screenLeftBottom =gui.Point (16, 42)
        screenLeftOffBottom = gui.Point (-240, -180)
        self.me1 = gui.MoveEffect(logoImage, 3, screenLeftOffBottom, screenLeftBottom, 1)
        self.mGUIManager.addEffect(self.me1)
        screenSize =gui.Size(240, 180)
        noSize =gui.Size(1, 1)
        self.se1 = gui.SizeEffect(logoImage, 3, noSize, screenSize, 1)
        self.mGUIManager.addEffect(self.se1)
        self.ae1 = gui.AlphaEffect(logoImage, 3, 0, 1, 3)
        self.mGUIManager.addEffect( self.ae1)
             
        
        logoLabel = self.mSheet.createLabel()
#         logoLabel.setDimensions(Rect(60,240,120,30))
        logoLabel.setPosition( gui.Point(62,240))
        logoLabel.setText("Click Me >")
        self.imageToggleButton = self.mSheet.createNStateButton()
        self.imageToggleButton.setDimensions(Rect(180,240,40,30))
        self.imageToggleButton.addState("OgreLogo","qgui.checked.png")
        self.imageToggleButton.addState("SimpleGUILogo","qgui.unchecked.png")

        ## RTT Example Use
        rttImage = self.mSheet.createImage()
        rttImage.setDimensions(Rect(600,42,160,90))
        rttImage.setTexture("RttTex")
#         rttImage.setBorderWidth(10)

        ninjaWindow = self.mSheet.createWindow()
        ninjaWindow.hideTitlebar()
        ninjaWindow.setDimensions(Rect(600,150,160,120))

        animToggleButton = ninjaWindow.createNStateButton()
        animToggleButton.setDimensions(Rect(8,7.5,144,30))
        ## populate NStateButton with States - robot animations
        casi = self.robot.getAllAnimationStates().getAnimationStateIterator()
        state = 0
        while( casi.hasMoreElements() ):
            animName = casi.getNext().getAnimationName()
            animToggleButton.addState("State"+str(state),"qgui.button.png",animName)
            if state == 0:
                self.robotAnimationState = self.robot.getAnimationState(animName)
                self.robotAnimationState.setEnabled(True)
                self.robotAnimationState.setTimePosition(0.0)
                self.robotAnimationState.setLoop(True)
            state+=1
#         animToggleButton.addOnStateChangedEventHandler(self.evtHndlr_changeAnimations)
#         animToggleButton.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_changeAnimations) )
        animToggleButton.addOnStateChangedEventHandler(self.MakeCallback(self.evtHndlr_changeAnimations) )
        
        hurtButton = ninjaWindow.createButton()
        hurtButton.setDimensions(Rect(8,45,144,30))
        hurtButton.setText("Hurt")
        hurtButton.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_hurt) )
        
        healButton = ninjaWindow.createButton()
        healButton.setDimensions(Rect(8,82.5,144,30))
        healButton.setText("Heal")
        healButton.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_heal) )
        
        ## TrackBar Setup
        tb1 = self.mSheet.createHorizontalTrackBar() 
        tb1.setDimensions(Rect(320,60,160,24))
        tb1.setNumRegions(3)
        #tb1.setTickPosition(4) # Does not work! 3 regions . ticks: 0/1/2/3
        tb2 = self.mSheet.createVerticalTrackBar() 
        tb2.setDimensions(Rect(320,90,24,120))
        tb2.setNumTicks(10)
        #tb2.setTickPosition(9)
        
        ## Progress Bar Setup
        self.lifeBar = self.mSheet.createProgressBar()
        self.lifeBar.setDimensions(Rect(320,330,160,18))
        self.lifeBar.setInitialPixelOffset(0)
        HPLabel = self.mSheet.createLabel()
        HPLabel.setDimensions(Rect(320,315,56,42))
        HPLabel.setTexture("")
        HPLabel.setText("HP")
        HPLabel.appearOverWidget(self.lifeBar)
        
        self.lifeBarValueLabel = self.mSheet.createLabel()
        self.lifeBarValueLabel.setDimensions(Rect(440,315,56,42))
        self.lifeBarValueLabel.setTexture("")
        self.lifeBarValueLabel.setText("100")
        self.lifeBarValueLabel.appearOverWidget(self.lifeBar)

        ## Mouse Over window
        mouseOverWindow = self.mSheet.createWindow()
        mouseOverWindow.setDimensions(Rect(480,420,320,60))
        mouseOverWindow.hideTitlebar()
        
        mouseOverLabel = mouseOverWindow.createLabel()
        mouseOverLabel.setDimensions(Rect(0,0,320,30))
        mouseOverLabel.setText("Mouse Over Widget:")
        s = mouseOverLabel.getSize() 
        self.mouseOverTB = mouseOverWindow.createTextBox()
        self.mouseOverTB.setDimensions(Rect(0,s.height,320,30))
        self.mouseOverTB.setReadOnly(True)

        ## Login Portion
        l=self.mSheet.createLabel()
        l.setDimensions(Rect(16,360,160,30))
        l.setText("User Name:")
        
        l=self.mSheet.createLabel()
        l.setDimensions(Rect(16,390,160,30))
        l.setText("Password:")
        
        self.usernameTB = self.mSheet.createTextBox()
        self.usernameTB.setDimensions(Rect(180,360,200,30))
        self.usernameTB.setText("\tThe quick brown fox jumped over the red fence.") 
        
        self.passwordTB = self.mSheet.createTextBox()
        self.passwordTB.setDimensions(Rect(180,390,200,30))
        self.passwordTB.maskUserInput(ord('*'))
        
        loginButton = self.mSheet.createButton()
        loginButton.setDimensions(Rect(100,420,200,42))
        loginButton.setText("Login")
        loginButton.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_login) )
        self.loginResultLabel = self.mSheet.createLabel()
        self.loginResultLabel.setDimensions(Rect(0.0,462,480,30))
        self.loginResultLabel.setTexture("")
        
        ## Set Text Window
        self.stWindow = self.mSheet.createWindow()
        self.stWindow.setDimensions(Rect(560,270,240,120))
        self.stWindow.hide()
        self.stWindow.getTitleBar().setText("Set Text Color:")
        
        closeButton = self.stWindow.getTitleBar().getCloseButton() 

        l=self.stWindow.createLabel()
        l.setDimensions(Rect(40,30,60,30))
        l.setText("Color:")
        
        colorCB = self.stWindow.createComboBox()
        colorCB.setDimensions(Rect(125,30,100,30))
        
        colorCB.setDimensions(Rect(125,30,100,30))
        item = colorCB.addItem()
        item.setText("Red")
        item = colorCB.addItem()
        item.setText("Green")
        item = colorCB.addItem()
        item.setText("Blue")
        item = colorCB.addItem()
        item.setText("Black")
        item = colorCB.addItem()
        item.setText("White")
        item = colorCB.addItem()
        item.setText("Purple")
        item = colorCB.addItem()
        item.setText("Brown")
        
#         colorCBdropList = colorCB.getDropDownList()
#         colorCBdropList.setNumberOfVisibleItems(6) 
#         
#         colorCBdropList.addListItem("Red")
#         colorCBdropList.addListItem("Green")
#         colorCBdropList.addListItem("Blue")
#         colorCBdropList.addListItem("Black")
#         colorCBdropList.addListItem("White") 
#         colorCBdropList.addListItem("Yellow") 
#         colorCBdropList.addListItem("Purple") 
#         colorCBdropList.addListItem("Brown") 
        
        setTextButton = self.stWindow.createButton()
        setTextButton.setDimensions(Rect(80,70,50,30))
        setTextButton.setText("Apply")
        setTextButton.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.evtHndlr_setTextColor) )
        # colorCB.clearList()
#         colorCBdropList.removeListItem(2)

        testButton = self.mSheet.createButton()
        testButton.setDimensions(Rect(1200,800,100,50))
        testButton.setText("TEST") 
        testButton.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.test)) 

        testButton2 = self.stWindow.createButton() 
        testButton2.setDimensions(Rect(500,190,100,50))
        testButton2.setText("TEST") 
        testButton2.addEventHandler(gui.Widget.EVENT_MOUSE_BUTTON_UP,self.MakeCallback(self.test)) 
 
        tb=self.mSheet.createTextBox() 
        tb.setDimensions(Rect(480,480,320,30))


    def test(self, args):
        self.mGUIManager.destroyWidget("TestButton1") 

    def evtHndlr_mainPage( self, args):
        self.mGUIManager.setActiveSheet(self.mSheet) 

    def evtHndlr_test1( self, args):
        self.mGUIManager.setActiveSheet(self.mSizePositionTest) 

    def evtHndlr_exitListItem(self, args):
        ## check if left mouse button is down
        if(args.button == gui.MB_Left):
            self.frameListener.requestShutdown()
        return True

    def evtHndlr_changeAnimations(self,args):
        ## disable previous animation
        self.robotAnimationState.setEnabled(False)
        self.robotAnimationState.setWeight(0.0)

        self.robotAnimationState = self.robot.getAnimationState(args.widget.getCurrentState().getText())
        self.robotAnimationState.setEnabled(True)
        self.robotAnimationState.setTimePosition(0.0)
        self.robotAnimationState.setWeight(1.0)
        self.robotAnimationState.setLoop(True)

        self.frameListener.setAnimationState(self.robotAnimationState)
        return True

    def evtHndlr_heal(self, args):
        currentProgress = self.lifeBar.getProgress()
        random = Ogre.Math.RangeRandom(currentProgress,1.0)
        self.lifeBar.setProgress(random)
        self.lifeBarValueLabel.setText(str(int(random * 100)))
        return True

    def evtHndlr_hurt(self, args):
        currentProgress = self.lifeBar.getProgress()
        random = Ogre.Math.RangeRandom(0.0,currentProgress)
        self.lifeBar.setProgress(random)
        self.lifeBarValueLabel.setText(str(int(random * 100)))
        return True

    def evtHndlr_pbRed(self,args):
        self.lifeBar.setTexture("progressbar.red.png")
        self.lifeBar.setProgress(1.0)
        self.lifeBarValueLabel.setText("100")

        return False

    def evtHndlr_pbGreen(self, args):
        self.lifeBar.setTexture("progressbar.green.png")
        self.lifeBar.setProgress(1.0)
        self.lifeBarValueLabel.setText("100")
        return True

    def evtHndlr_pbBlue(self, args):
        self.lifeBar.setTexture("progressbar.blue.png") 
        self.lifeBar.setProgress(1.0) 
        return True

    def evtHndlr_login(self, args):
        if( self.usernameTB.getCaption() == self.passwordTB.getCaption() ) : s = "Login Successful."
        else: s = "Username and/or Password do not match."
        self.loginResultLabel.setText(s)
        return True

    def evtHndlr_toggleDebugDisplay(self, args):
        self.mDebugDisplayShown = not self.mDebugDisplayShown
        self.frameListener.showDebugOverlay(self.mDebugDisplayShown)
        return True

    def evtHndlr_setTextWhite(self, args):
        self.stWindow.getTitleBar().getText().setColor(Ogre.ColourValue.White)
        return True

    def evtHndlr_showSetTextDialog(self, args):
        self.stWindow.setPosition(0.7,0.45)
        self.stWindow.show()
        return True
        
    def evtHndlr_CameraPoint(self, args):
        self.camera.polygonMode=Ogre.PM_POINTS
        return True
    
    def evtHndlr_CameraWireFrame(self, args):
        self.camera.polygonMode=Ogre.PM_WIREFRAME
        return True
        
    def evtHndlr_CameraSolid(self, args):
        self.camera.polygonMode=Ogre.PM_SOLID
        return True

    def evtHndlr_setTextColor(self, args):
        s = self.stWindow.getComboBox(0).getText().getCaption()
        
        if( s == "Red" ): c = Ogre.ColourValue.Red
        elif( s == "Green" ): c = Ogre.ColourValue.Green
        elif( s == "Blue" ): c = Ogre.ColourValue.Blue
        elif( s == "Black" ): c = Ogre.ColourValue.Black
        elif( s == "White" ): c = Ogre.ColourValue.White

        self.stWindow.getTitleBar().getText().setColor(c)
        return True
            
    def _createFrameListener(self):
        self.frameListener= GuiFrameListener(self.renderWindow, self.camera)
        ## This ensures the mouse doesn't move the camera.
        self.frameListener.setMenuMode ( True )
        self.root.addFrameListener(self.frameListener)
        self.frameListener.setAnimationState(self.robotAnimationState)
        self.frameListener.setVars(self.mGUIManager,self.mouseOverTB) 

    def handleQuit(self,e):
        self.frameListener.requestShutdown()
        return True

    def handleChange(self,e):
        self.mGUIManager.clearAll()
        return True

if __name__ == '__main__':
    try:
        application = QuickGUIDemoApp()
        application.go()
    except Ogre.OgreException, e:
        print e