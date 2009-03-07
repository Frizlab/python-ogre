# -----------------------------------------------------------------------------
# This source file is part of OGRE
#     (Object-oriented Graphics Rself.endering Engine)
# For the latest info, see http:##www.ogre3d.org/

# Copyright (c) 2000-2006 Torus Knot Software Ltd
# Also see acknowledgements in Readme.html

# You may use this sample code for anything you like, it is not covered by the
# LGPL like the rest of the engine.
# -----------------------------------------------------------------------------

import ogre.renderer.OGRE as Ogre
import ogre.gui.CEGUI as CEGUI
import ogre.io.OIS as OIS
import ctypes
MOVESPEED = 30

def convertOISButtonToCegui( buttonID):
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

def cegui_reldim ( x ) :
    return CEGUI.UDim((x),0)
    
    
# /*************************************************************************
#                       HeatVisionListener Methods
# *************************************************************************/
class HeatVisionListener(Ogre.CompositorInstance.Listener):
##---------------------------------------------------------------------------
    def __init__(self):
        Ogre.CompositorInstance.Listener.__init__(self)
        self.timer = Ogre.Timer() 
        self.start =0.0
        self.end = 0.0
        self.curr = 0.0 
        self.notifyMaterialSetup (10,"hello")
        
##---------------------------------------------------------------------------
    def __del__(self):
       del self.timer 
##---------------------------------------------------------------------------
            
    def notifyMaterialSetup(self, pass_id, mat):
#         print "MATERIAL SETUP", self, pass_id, mat
        if(pass_id == 0xDEADBABE):
            self.timer.reset() 
            self.fpParams = mat.getTechnique(0).getPass(0).getFragmentProgramParameters() 
##---------------------------------------------------------------------------
    def notifyMaterialRender(self, pass_id, mat):
        if pass_id == 0xDEADBABE:
            ## "random_fractions" parameter
            self.fpParams.setNamedConstant("random_fractions", 
                            Ogre.Vector4(Ogre.Math.RangeRandom(0.0, 1.0), Ogre.Math.RangeRandom(0, 1.0), 0, 0)) 

            ## "depth_modulator" parameter
            inc = self.timer.getMilliseconds()/1000.0 
            if abs(self.curr-self.end) <= 0.001  :
                ## take a new value to reach
                self.end = Ogre.Math.RangeRandom(0.95, 1.0) 
                self.start = self.curr 
            else:
                if (self.curr > self.end): 
                    self.curr -= inc
                else : 
                    self.curr += inc 
            self.timer.reset() 

            self.fpParams.setNamedConstant("depth_modulator", Ogre.Vector4(self.curr, 0, 0, 0) )
##---------------------------------------------------------------------------

#   /*************************************************************************
#   hdrListener Methods
#   *************************************************************************/

class HDRListener(Ogre.CompositorInstance.Listener):
    ##---------------------------------------------------------------------------
    def __init__ (self):
        Ogre.CompositorInstance.Listener.__init__(self)
        self.x = 4
        self.y = 15
        ## Python vs C++ difference-- we use ctypes buffers
        storageclass = ctypes.c_float * (self.x*self.y)
        self.mBloomTexWeights=storageclass(1.1)
        ctypes.memset ( self.mBloomTexWeights, 0, self.x*self.y )
        self.mBloomTexOffsetsHorz=storageclass(1.1)
        ctypes.memset ( self.mBloomTexOffsetsHorz, 0, self.x*self.y )
        self.mBloomTexOffsetsVert=storageclass(1.1)
        ctypes.memset ( self.mBloomTexOffsetsVert, 0, self.x*self.y )
        
    ##---------------------------------------------------------------------------
    def notifyViewportSize(self, width, height):
        self.mVpWidth = width 
        self.mVpHeight = height 
    ##---------------------------------------------------------------------------
    def notifyCompositor(self, instance):
        ## Get some RTT dimensions for later calculations
        defIter = instance.getTechnique().getTextureDefinitionIterator() 
        while (defIter.hasMoreElements()) :
            _def = defIter.getNext() 
            if(_def.name == "rt_bloom0"):
                self.mBloomSize = _def.width  ## should be square
                ## Calculate gaussian texture offsets & weights
                deviation = 3.0 
                texelSize = 1.0 / self.mBloomSize 

                ## central sample, no offset
                self.mBloomTexOffsetsHorz[0] = 0.0 
                self.mBloomTexOffsetsHorz[1] = 0.0 
                self.mBloomTexOffsetsVert[0] = 0.0 
                self.mBloomTexOffsetsVert[1] = 0.0 
                self.mBloomTexWeights[0] = Ogre.Math.gaussianDistribution(0, 0, deviation) 
                self.mBloomTexWeights[1] = self.mBloomTexWeights[0]
                self.mBloomTexWeights[2] = self.mBloomTexWeights[0] 
                self.mBloomTexWeights[3] = 1.0 

                ## 'pre' samples
                for i in range (8):
                    self.mBloomTexWeights[i*self.x+0] = 1.25 * Ogre.Math.gaussianDistribution(i, 0, deviation)
                    self.mBloomTexWeights[i*self.x+1] = self.mBloomTexWeights[i*self.x]
                    self.mBloomTexWeights[i*self.x+2] = self.mBloomTexWeights[i*self.x] 
                    self.mBloomTexWeights[i*self.x+3] = 1.0 
                    self.mBloomTexOffsetsHorz[i*self.x+0] = i * texelSize 
                    self.mBloomTexOffsetsHorz[i*self.x+1] = 0.0 
                    self.mBloomTexOffsetsVert[i*self.x+0] = 0.0 
                    self.mBloomTexOffsetsVert[i*self.x+1] = i * texelSize 
                ## 'post' samples
                for i in range (8,15):
                    self.mBloomTexWeights[i*self.x+0] =  self.mBloomTexWeights[(i-7)*self.x+0] 
                    self.mBloomTexWeights[i*self.x+1] = self.mBloomTexWeights[i*self.x+0]
                    self.mBloomTexWeights[i*self.x+2] = self.mBloomTexWeights[i*self.x+0] 
                    self.mBloomTexWeights[i*self.x+3] = 1.0 

                    self.mBloomTexOffsetsHorz[i*self.x+0] = -self.mBloomTexOffsetsHorz[(i - 7)*self.x+0] 
                    self.mBloomTexOffsetsHorz[i*self.x+1] = 0.0 
                    self.mBloomTexOffsetsVert[i*self.x+0] = 0.0 
                    self.mBloomTexOffsetsVert[i*self.x+1] = -self.mBloomTexOffsetsVert[(i - 7)*self.x+1] 

    ##---------------------------------------------------------------------------
    def notifyMaterialSetup(self, pass_id, mat):
        ## Prepare the fragment params offsets
#       switch(pass_id)
#       ##case 994: ## rt_lum4
#       if pass_id ==  993: ## rt_lum3
#       case 992: ## rt_lum2
#       case 991: ## rt_lum1
#       case 990: ## rt_lum0
#           break 
#       case 800: ## rt_brightpass
#           break 
        
        if pass_id == 701: ## rt_bloom1
                ## horizontal bloom
                mat.load() 
                fparams = mat.getBestTechnique().getPass(0).getFragmentProgramParameters() 
                progName = mat.getBestTechnique().getPass(0).getFragmentProgramName() 
                fparams.setNamedConstantFloat("sampleOffsets",ctypes.addressof(self.mBloomTexOffsetsHorz), self.x) 
                fparams.setNamedConstantFloat("sampleWeights",ctypes.addressof(self.mBloomTexWeights), self.x) 
        elif pass_id == 700: ## rt_bloom0
                ## vertical bloom
                mat.load() 
                fparams = mat.getTechnique(0).getPass(0).getFragmentProgramParameters() 
                progName = mat.getBestTechnique().getPass(0).getFragmentProgramName() 
                fparams.setNamedConstantFloat("sampleOffsets",ctypes.addressof(self.mBloomTexOffsetsVert), self.x) 
                fparams.setNamedConstantFloat("sampleWeights",ctypes.addressof(self.mBloomTexWeights), self.x) 
    ##---------------------------------------------------------------------------
    def notifyMaterialRender(self, pass_id, mat):
        pass
    ##---------------------------------------------------------------------------


# #     /*************************************************************************
# #     self.gaussianListener Methods
# #     *************************************************************************/
class gaussianListener(Ogre.CompositorInstance.Listener):
    ##---------------------------------------------------------------------------
    def __init__ ( self ):
        Ogre.CompositorInstance.Listener.__init__(self)
        self.x = 4
        self.y = 15
        ## Python vs C++ difference-- we use ctypes buffers
        storageclass = ctypes.c_float * (self.x*self.y)
        self.mBloomTexWeights=storageclass(1.1)
        ctypes.memset ( self.mBloomTexWeights, 0, self.x*self.y )
        self.mBloomTexOffsetsHorz=storageclass(1.1)
        ctypes.memset ( self.mBloomTexOffsetsHorz, 0, self.x*self.y )
        self.mBloomTexOffsetsVert=storageclass(1.1)
        ctypes.memset ( self.mBloomTexOffsetsVert, 0, self.x*self.y )
        
    ##---------------------------------------------------------------------------
    def notifyViewportSize(self, width, height):
        self.mVpWidth = width 
        self.mVpHeight = height 
        ## Calculate gaussian texture offsets & weights
        deviation = 3.0 
        texelSize = 1.0 / min(self.mVpWidth,self.mVpHeight) 

        ## central sample, no offset
        self.mBloomTexOffsetsHorz[0] = 0.0 
        self.mBloomTexOffsetsHorz[1] = 0.0 
        self.mBloomTexOffsetsVert[0] = 0.0 
        self.mBloomTexOffsetsVert[1] = 0.0 
        self.mBloomTexWeights[0] = Ogre.Math.gaussianDistribution(0, 0, deviation)
        self.mBloomTexWeights[1] = self.mBloomTexWeights[0]
        self.mBloomTexWeights[2] = self.mBloomTexWeights[0] 
        self.mBloomTexWeights[3] = 1.0 

        ## 'pre' samples
        for i in range (1,8):
            self.mBloomTexWeights[i*self.x+0] = Ogre.Math.gaussianDistribution(i, 0, deviation)
            self.mBloomTexWeights[i*self.x+1] = self.mBloomTexWeights[i*self.x]
            self.mBloomTexWeights[i*self.x+2] =  self.mBloomTexWeights[i*self.x]
            self.mBloomTexWeights[i*self.x+3] = 1.0 
            self.mBloomTexOffsetsHorz[i*self.x+0] = i * texelSize 
            self.mBloomTexOffsetsHorz[i*self.x+1] = 0.0 
            self.mBloomTexOffsetsVert[i*self.x+0] = 0.0 
            self.mBloomTexOffsetsVert[i*self.x+1] = i * texelSize 
        ## 'post' samples
        for i in range (8,15):
            self.mBloomTexWeights[i*self.x+0] = self.mBloomTexWeights[(i - 7)*self.x+0]
            self.mBloomTexWeights[i*self.x+1] = self.mBloomTexWeights[i*self.x+0]
            self.mBloomTexWeights[i*self.x+2] = self.mBloomTexWeights[i*self.x+0] 
            self.mBloomTexWeights[i*self.x+3] = 1.0 

            self.mBloomTexOffsetsHorz[i*self.x+0] = -self.mBloomTexOffsetsHorz[(i - 7)*self.x+0] 
            self.mBloomTexOffsetsHorz[i*self.x+1] = 0.0 
            self.mBloomTexOffsetsVert[i*self.x+0] = 0.0 
            self.mBloomTexOffsetsVert[i*self.x+1] = -self.mBloomTexOffsetsVert[(i - 7)*self.x+1] 
    ##---------------------------------------------------------------------------
    def notifyMaterialSetup(self,  pass_id, mat):
        ## Prepare the fragment params offsets
        
        if pass_id == 701: ## blur horz
                ## horizontal bloom
                mat.load() 
                fparams = mat.getBestTechnique().getPass(0).getFragmentProgramParameters() 
                progName = mat.getBestTechnique().getPass(0).getFragmentProgramName() 
                fparams.setNamedConstantFloat("sampleOffsets",ctypes.addressof(self.mBloomTexOffsetsHorz), 15) 
                fparams.setNamedConstantFloat("sampleWeights",ctypes.addressof(self.mBloomTexWeights), 15) 

        elif pass_id == 700: ## blur vert
                ## vertical bloom
                mat.load() 
                fparams = mat.getTechnique(0).getPass(0).getFragmentProgramParameters() 
                progName = mat.getBestTechnique().getPass(0).getFragmentProgramName() 
                fparams.setNamedConstantFloat("sampleOffsets",ctypes.addressof(self.mBloomTexOffsetsVert), 15) 
                fparams.setNamedConstantFloat("sampleWeights",ctypes.addressof(self.mBloomTexWeights), 15) 
    ##---------------------------------------------------------------------------
# #     void self.gaussianListener.notifyMaterialRself.ender(Ogre.uint32 pass_id, Ogre.MaterialPtr &mat)
    ##---------------------------------------------------------------------------

    
    
WIDGET_XPOS= 0.0
WIDGET_YSTART= 0.1
WIDGET_YOFFSET =0.13
ITEM_YSIZE=15
ITEM_YSPACING =2

class ItemSelectorInterface():
    def __init__(self):
        pass

class ItemSelector():
    def __init__(self):
        pass
        
class ItemSelectorViewManager:
    def __init__(self,parentWindowName):
        self.mItemSelectorController =0
        self.mItemSelectorContainer=[]
        self.mParentWindow = CEGUI.WindowManager.getSingleton().getWindow(parentWindowName)
        ## add a scrollable pane as a child to the parent
        self.mScrollablePane = CEGUI.WindowManager.getSingleton().createWindow("TaharezLook/ScrollablePane",
                                    ("MainScrollPane"))
        self.mScrollablePane.setHorizontalAlignment(CEGUI.HA_CENTRE)
        self.mScrollablePane.setSize(CEGUI.UVector2(CEGUI.UDim(0.9, 0), CEGUI.UDim(0.75, 0)))
        self.mParentWindow.addChildWindow(self.mScrollablePane)
        self.mScrollablePane.setPosition(CEGUI.UVector2(CEGUI.UDim(WIDGET_XPOS, 0), CEGUI.UDim(WIDGET_YSTART, 0)))
        ## setup scrollable pane to resize to inside of parent window when parent resizes
        ## scrollbars should only become visible when required
        ## automatically handled by scrollable pane
##-----------------------------------------------------------------------------------
    def addItemSelector(self, displayText):
        ## add a new item selector
        ## determine new index for item
        idx = len(self.mItemSelectorContainer)
        item = ItemSelector()
        self.mItemSelectorContainer.append(item)
        
        ## create new checkbox
        
        item.CheckBoxWidget = CEGUI.WindowManager.getSingleton().createWindow("TaharezLook/Checkbox",
                                    "ItemCheckbox" + str(idx))
        checkbox=item.CheckBoxWidget
        ## set checkbox ID to selector ID
        checkbox.setID(idx)
        checkbox.setSize(CEGUI.UVector2(CEGUI.UDim(0, 140), CEGUI.UDim(0, ITEM_YSIZE)))
        checkbox.setText(displayText)
        checkbox.setProperty("HoverTextColour", CEGUI.PropertyHelper.colourToString(CEGUI.colour(1.0, 1.0, 0.0)))
        ## add event handler for when checkbox state changes
        checkbox.subscribeEvent(CEGUI.Checkbox.EventCheckStateChanged, self,"handleCheckStateChanged" )
        checkbox.setPosition(CEGUI.UVector2(CEGUI.UDim(0, 0), CEGUI.UDim(0, 12 + (ITEM_YSIZE + ITEM_YSPACING)* (idx))))
        ## add checkbox to the scroll pane
        self.mScrollablePane.addChildWindow(checkbox)

##-----------------------------------------------------------------------------------
    def setItemSelectorController(self, controller):
        self.mItemSelectorController = controller

##-----------------------------------------------------------------------------------
    def handleCheckStateChanged(self, e):
        ## activate controller if set
        if (self.mItemSelectorController):
            checkbox = e.window
            self.mItemSelectorController.itemStateChanged(checkbox.getID(), checkbox.isSelected())
            if checkbox.isSelected():
                selectColour =  0.0
            else: 
                selectColour = 1.0
            checkbox.setProperty("NormalTextColour",
                CEGUI.PropertyHelper.colourToString(CEGUI.colour(selectColour, 1.0, selectColour)))
        return True
    
    def getSelectorCount(self):
        return len(self.mItemSelectorContainer)
    def getItemSelectorText(self, index):
        return self.mItemSelectorContainer[index].CheckBoxWidget.getText()
        
# # /*************************************************************************
# #     CompositorDemo_FrameListener methods that handle all input for this Compositor demo.
# # *************************************************************************/
class CompositorDemo_FrameListener(Ogre.FrameListener, OIS.KeyListener,  OIS.MouseListener, ItemSelectorInterface):
    def __init__ (self, main):
        Ogre.FrameListener.__init__(self)
        OIS.KeyListener.__init__(self)
        OIS.MouseListener.__init__(self)
        ItemSelectorInterface.__init__(self)
        self.mMain = main
        self.mTranslateVector = Ogre.Vector3().ZERO
        self.mStatsOn = True
        self.mNumScreenShots = 0
        self.mWriteToFile = False
        self.mSkipCount = 0
        self.mUpdateFreq = 50
        mSceneDetailIndex = 0
        self.mQuit=False
        self.mAvgFrameTime=0.1
        self.mLastMousePositionSet=False
        self.mTimeUntilNextToggle=0
        self.mRotX=0
        self.mRotY=0
        self.mProcessMovement=False
        self.mUpdateMovement=False

        self.mLMBDown=False
        self.mRMBDown=False
        self.mMoveFwd=False
        self.mMoveBck=False
        self.mMoveLeft=False
        self.mMoveRight=False
        self.mSpinny=0
        self.mCompositorSelectorViewManager=0
        self.mMouse=0
        self.mKeyboard=0
        self.mDebugText=""


        Ogre.Root.getSingleton().addFrameListener(self) 

        ## using buffered input
#         pl = OIS.ParamList
#         windowHnd = 0 

#         self.mMain.getRenderWindow().getCustomAttribute("WINDOW", windowHnd) 
#         windowHndStr  = str(windowHnd )
#         pl.insert(std.make_pair(std.string("WINDOW"), windowHndStr.str())) 

#         self.mInputManager = OIS.InputManager.createInputSystem( pl ) 

        windowHnd = self.mMain.getRenderWindow().getCustomAttributeInt("WINDOW")
        self.mInputManager = \
             OIS.createPythonInputSystem([("WINDOW",str(windowHnd))])
        
        ##Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
        self.mKeyboard = self.mInputManager.createInputObjectKeyboard( OIS.OISKeyboard, True )
        self.mMouse = self.mInputManager.createInputObjectMouse( OIS.OISMouse, True )

        width, height, depth, left, top = self.mMain.getRenderWindow().getMetrics(1,1,1,1,1)

        ##Set Mouse Region.. if window resizes, we should alter this to reflect as well
        ms = self.mMouse.getMouseState() 
        ms.width = width 
        ms.height = height 

        self.mMouse.setEventCallback(self) 
        self.mKeyboard.setEventCallback(self) 

        self.mGuiRenderer = CEGUI.System.getSingleton().getRenderer() 

        self.mGuiAvg   = CEGUI.WindowManager.getSingleton().getWindow("OPAverageFPS") 
        self.mGuiCurr  = CEGUI.WindowManager.getSingleton().getWindow("OPCurrentFPS") 
        self.mGuiBest  = CEGUI.WindowManager.getSingleton().getWindow("OPBestFPS") 
        self.mGuiWorst = CEGUI.WindowManager.getSingleton().getWindow("OPWorstFPS") 
        self.mGuiTris  = CEGUI.WindowManager.getSingleton().getWindow("OPTriCount") 
        self.mGuiDbg   = CEGUI.WindowManager.getSingleton().getWindow("OPDebugMsg") 
        self.mRoot    = CEGUI.WindowManager.getSingleton().getWindow("root") 

        self.registerCompositors() 
        self.initDebugRTTWindow() 
        self.connectEventHandlers() 
##--------------------------------------------------------------------------
    def __del__(self):
        if(self.mInputManager):
            self.mInputManager.destroyInputObject(self.mKeyboard) 
            self.mInputManager.destroyInputObject(self.mMouse) 
            OIS.InputManager.destroyInputSystem(self.mInputManager) 
            self.mInputManager = 0 

        del self.hvListener 
        del self.hdrListener 
        del self.gaussianListener 
        del self.mCompositorSelectorViewManager 
        
    def  setSpinningNode(self, node):
        self.mSpinny = node
##--------------------------------------------------------------------------
    def connectEventHandlers(self):
        wndw = CEGUI.WindowManager.getSingleton().getWindow("root") 

        wndw.subscribeEvent(CEGUI.Window.EventMouseMove, 
                    self, "handleMouseMove") 

        wndw.subscribeEvent(CEGUI.Window.EventMouseButtonUp, 
                    self,"handleMouseButtonUp") 

        wndw.subscribeEvent(CEGUI.Window.EventMouseButtonDown, 
                    self,"handleMouseButtonDown") 

        wndw.subscribeEvent(CEGUI.Window.EventMouseWheel, 
                    self,"handleMouseWheelEvent") 
        wndw.subscribeEvent(CEGUI.Window.EventKeyDown, 
                    self,"handleKeyDownEvent") 
        wndw.subscribeEvent(CEGUI.Window.EventKeyUp, 
                    self,"handleKeyUpEvent") 
##--------------------------------------------------------------------------
    def frameStarted( self, evt):
        self.mMouse.capture() 
        self.mKeyboard.capture() 

        if( self.mMain.getRenderWindow().isActive() == False ):
            return False 
    
        if (self.mQuit):
            return False 
        else:
            self.mSkipCount+=1 
            if (self.mSkipCount >= self.mUpdateFreq):
                self.mSkipCount = 0 
                self.updateStats() 
            ## update movement process
            if(self.mProcessMovement or self.mUpdateMovement):
                if self.mMoveLeft:
                    self.mTranslateVector.x += self.mAvgFrameTime * -MOVESPEED
                if self.mMoveRight:
                    self.mTranslateVector.x += self.mAvgFrameTime * MOVESPEED
                if self.mMoveFwd:
                    self.mTranslateVector.z += self.mAvgFrameTime * -MOVESPEED
                if self.mMoveBck: 
                    self.mTranslateVector.z += self.mAvgFrameTime * MOVESPEED 

                self.mMain.getCamera().yaw(Ogre.Degree(self.mRotX)) 
                self.mMain.getCamera().pitch(Ogre.Degree(self.mRotY)) 
                self.mMain.getCamera().moveRelative(self.mTranslateVector) 

                self.mUpdateMovement = False 
                self.mRotX = 0 
                self.mRotY = 0 
                self.mTranslateVector = Ogre.Vector3().ZERO 

            if(self.mWriteToFile):
                self.mNumScreenShots +=1
                self.mMain.getRenderWindow().writeContentsToFile("frame_" +
                    str(self.mNumScreenShots) + ".png") 

            if (self.mSpinny):
                self.mSpinny.yaw(Ogre.Degree(10 * evt.timeSinceLastFrame)) 

            return True 

##--------------------------------------------------------------------------
    def mouseMoved (self, e):
        CEGUI.System.getSingleton().injectMouseMove( e.get_state().X.rel, e.get_state().Y.rel ) 
        CEGUI.System.getSingleton().injectMouseWheelChange(e.get_state().Z.rel) 
        return True 

##--------------------------------------------------------------------------
    def keyPressed ( self, e):
        ## give 'quitting' priority
        if (e.key == OIS.KC_ESCAPE):
            self.mQuit = True 
            return False 

        if (e.key == OIS.KC_SYSRQ ):
            ss = "screenshot_" + str(self.mNumScreenShots) + ".png" 
            self.mMain.getRenderWindow().writeContentsToFile(ss.str()) 
            self.mDebugText = "Saved: " + ss.str() 
            ##self.mTimeUntilNextToggle = 0.5 

        ## do event injection
        cegui = CEGUI.System.getSingleton() 
        cegui.injectKeyDown(e.key) 
        cegui.injectChar(e.text) 
        return True 

##--------------------------------------------------------------------------
    def keyReleased ( self, e):
        CEGUI.System.getSingleton().injectKeyUp(e.key) 
        return True 

##--------------------------------------------------------------------------
    def mousePressed ( self, e, _id):
        CEGUI.System.getSingleton().injectMouseButtonDown(convertOISButtonToCegui(_id)) 
        return True 

##--------------------------------------------------------------------------
    def mouseReleased ( self,e, _id):
        CEGUI.System.getSingleton().injectMouseButtonUp(convertOISButtonToCegui(_id)) 
        return True 

##--------------------------------------------------------------------------
    def convertOISButtonToCegui(self, ois_button_id):
        if ois_button_id == 0:
            return CEGUI.LeftButton
        if ois_button_id == 1:
            return CEGUI.RightButton 
        if ois_button_id == 2:
            return CEGUI.MiddleButton 
        if ois_button_id == 3:
            return CEGUI.X1Button 
        return CEGUI.LeftButton 

##--------------------------------------------------------------------------
    def updateStats(self):
        currFps = "Last FPS: " 
        avgFps = "Average FPS: " 
        bestFps = "Best FPS: " 
        worstFps = "Worst FPS: " 
        tris = "Triangle Count: " 

#         print dir( self.mMain.getRenderWindow() )
        stats = self.mMain.getRenderWindow().getStatistics() 
#         print stats
#         print dir(stats)
        self.mGuiAvg.setText(avgFps + str(stats.avgFPS)) 
        self.mGuiCurr.setText(currFps + str(stats.lastFPS)) 
#         self.mGuiBest.setText(bestFps + str(stats.bestFPS)
#             + " " + str(stats.bestFrameTime)+" ms") 
#         self.mGuiWorst.setText(worstFps + str(stats.worstFPS)
#             + " " + str(stats.worstFrameTime)+" ms") 

#         self.mGuiTris.setText(tris + str(stats.triangleCount)) 
        self.mGuiDbg.setText(self.mDebugText) 
        self.mAvgFrameTime = 1.0/(stats.avgFPS + 1.0) 
        if (self.mAvgFrameTime > 0.1): self.mAvgFrameTime = 0.1



##--------------------------------------------------------------------------
    def handleMouseMove( self,  e):
        if( self.mLMBDown and not self.mRMBDown):
            ## rotate camera
            self.mRotX += -e.moveDelta.d_x * self.mAvgFrameTime * 10.0 
            self.mRotY += -e.moveDelta.d_y * self.mAvgFrameTime * 10.0 
            CEGUI.MouseCursor.getSingleton().setPosition( self.mLastMousePosition ) 
            self.mUpdateMovement = True 
        else:
            if( self.mRMBDown and not self.mLMBDown):
                ## translate camera
                self.mTranslateVector.x += e.moveDelta.d_x * self.mAvgFrameTime * MOVESPEED 
                self.mTranslateVector.y += -e.moveDelta.d_y * self.mAvgFrameTime * MOVESPEED 
                ##self.mTranslateVector.z = 0 
                CEGUI.MouseCursor.getSingleton().setPosition( self.mLastMousePosition ) 
                self.mUpdateMovement = True 
            else:
                if( self.mRMBDown and self.mLMBDown):
                    self.mTranslateVector.z += (e.moveDelta.d_x + e.moveDelta.d_y) * self.mAvgFrameTime * MOVESPEED 
                    CEGUI.MouseCursor.getSingleton().setPosition( self.mLastMousePosition ) 
                    self.mUpdateMovement = True 

        return True 

##--------------------------------------------------------------------------
    def handleMouseButtonUp( self, e):

        ##Window* wndw = (( WindowEventArgs&)e).window 
        if e.button == CEGUI.LeftButton:
            self.mLMBDown = False 

        elif e.button == CEGUI.RightButton:
            self.mRMBDown = False 
        if( not self.mLMBDown and not self.mRMBDown ):
            CEGUI.MouseCursor.getSingleton().show() 
            if self.mLastMousePositionSet:
                CEGUI.MouseCursor.getSingleton().setPosition( self.mLastMousePosition ) 
                self.mLastMousePositionSet = False 
            self.mRoot.releaseInput() 

        return True 

##--------------------------------------------------------------------------
    def handleMouseButtonDown( self, e):

        ##Window* wndw = (( WindowEventArgs&)e).window 
        if( e.button == CEGUI.LeftButton ):
            self.mLMBDown = True 

        if( e.button == CEGUI.RightButton ):
            self.mRMBDown = True 

        if( self.mLMBDown or self.mRMBDown ):
            CEGUI.MouseCursor.getSingleton().hide() 
            if (not self.mLastMousePositionSet):
                self.mLastMousePosition = CEGUI.MouseCursor.getSingleton().getPosition() 
                self.mLastMousePositionSet = True 
            self.mRoot.captureInput() 

        return True 

##--------------------------------------------------------------------------
    def handleMouseWheelEvent( self, e):
        self.mTranslateVector.z += e.wheelChange * -5.0 
        self.mUpdateMovement = True 
        return True 

##--------------------------------------------------------------------------
    def handleKeyDownEvent( self,  e):
        CheckMovementKeys( e.scancode , True) 

        return True 

##--------------------------------------------------------------------------
    def handleKeyUpEvent( self,  e):
        self.CheckMovementKeys( e.scancode, False ) 

        return True 

##--------------------------------------------------------------------------
    def CheckMovementKeys( self, scancode, state ):

        if  scancode== Key.A:
                self.mMoveLeft = state 

        elif scancode == Key.D:
                self.mMoveRight = state 
 
        elif scancode == Key.S:
                self.mMoveBck = state 

        elif scancode == Key.W:
                self.mMoveFwd = state 

        self.mProcessMovement = self.mMoveLeft or self.mMoveRight or self.mMoveFwd or self.mMoveBck 


##-----------------------------------------------------------------------------------
    def itemStateChanged( self, index,  state):
        ## get the item text and tell compositor manager to set enable state
        compositor = str(self.mCompositorSelectorViewManager.getItemSelectorText(index))
        print type(compositor)
        print compositor 
        compositor = self.mCompositorSelectorViewManager.getItemSelectorText(index).c_str()
        print type(compositor)
        print compositor 
        Ogre.CompositorManager.getSingleton().\
            setCompositorEnabled(self.mMain.getRenderWindow().getViewport(0),compositor, state)
        self.updateDebugRTTWindow() 
##-----------------------------------------------------------------------------------
    def registerCompositors(self):
        vp = self.mMain.getRenderWindow().getViewport(0) 
        self.hvListener = HeatVisionListener() 
        self.hdrListener = HDRListener() 
        self.gaussianListener = gaussianListener() 

        self.mCompositorSelectorViewManager = ItemSelectorViewManager("CompositorSelectorWin") 
        ## tell view manager to notify us when an item changes selection state
        self.mCompositorSelectorViewManager.setItemSelectorController(self) 
        ##iterate through Compositor Managers resources and add name keys ast Item selectors to Compositor selector view manager
        resourceIterator = Ogre.CompositorManager.getSingleton().getResourceIterator() 

        ## add all compositor resources to the view container
        while (resourceIterator.hasMoreElements()):
            resource = resourceIterator.getNext() 
            compositorName = resource.getName() 
            ## Don't add base Ogre/Scene compositor to view
            if (compositorName == "Ogre/Scene"):
                continue 

            self.mCompositorSelectorViewManager.addItemSelector(compositorName) 
            addPosition = -1 
            if (compositorName == "HDR"):
                ## HDR must be first in the chain
                addPosition = 0 
            instance = Ogre.CompositorManager.getSingleton().addCompositor(vp, compositorName, addPosition) 
            Ogre.CompositorManager.getSingleton().setCompositorEnabled(vp, compositorName, False) 
            ## special handling for Heat Vision which uses a listener
            if instance and (compositorName == "Heat Vision"):
                instance.addListener(self.hvListener) 
            elif instance and (compositorName == "HDR"):
                instance.addListener(self.hdrListener) 
                self.hdrListener.notifyViewportSize(vp.getActualWidth(), vp.getActualHeight()) 
                self.hdrListener.notifyCompositor(instance) 

            elif instance and (compositorName == "Gaussian Blur"):
                instance.addListener(self.gaussianListener) 
                self.gaussianListener.notifyViewportSize(vp.getActualWidth(), vp.getActualHeight()) 
    ##---------------------------------------------------------------------
    def initDebugRTTWindow(self):
        self.mDebugRTTStaticImage = CEGUI.WindowManager.getSingleton().getWindow("DebugRTTImage") 
        self.mDebugRTTListbox = CEGUI.WindowManager.getSingleton().getWindow("DebugRTTListbox")
        self.mDebugRTTListbox.subscribeEvent(CEGUI.Listbox.EventSelectionChanged,
            self,"handleRttSelection") 
    ##---------------------------------------------------------------------
    def handleRttSelection( self, e):
        if (self.mDebugRTTListbox.getSelectedCount() > 0):
            ## image set is in user data
            imgSet = self.mDebugRTTListbox.getFirstSelectedItem().getUserData() 

            self.mDebugRTTStaticImage.setProperty("Image",
                CEGUI.PropertyHelper.imageToString(imgSet.getImage("RttImage"))) 

        else:
            self.mDebugRTTStaticImage.setProperty("Image", "") 

        return True 
    ##---------------------------------------------------------------------
    def updateDebugRTTWindow(self):
        return
        ## Clear listbox
        self.mDebugRTTListbox.resetList() 
        ## Clear imagesets
        self.mDebugRTTStaticImage.setProperty("Image", "") 
        isIt = self.mDebugRTTImageSets.begin()
        while isIt != self.mDebugRTTImageSets.self.end():
            CEGUI.ImagesetManager.getSingleton().destroyImageset(isIt) 
            isIt+= 1
        self.mDebugRTTImageSets.clear() 
        ## Add an entry for each rself.ender texture for all active compositors
        vp = self.mMain.getRenderWindow().getViewport(0) 
        chain = Ogre.CompositorManager.getSingleton().getCompositorChain(vp) 
        it = chain.getCompositors() 
        while (it.hasMoreElements()):
            inst = it.getNext() 
            if (inst.getEnabled()):
                texIt = inst.getTechnique().getTextureDefinitionIterator() 
                while (texIt.hasMoreElements()):
                    texDef = texIt.getNext() 

                    ## Get instance name of texture
                    instName = inst.getTextureInstanceName(texDef.name) 
                    ## Create CEGUI texture from name of OGRE texture
                    tex = self.mMain.getGuiRself.enderer().createTexture(instName) 
                    ## Create imageset
                    imgSet = CEGUI.ImagesetManager.getSingleton().createImageset(
                            instName, tex) 
                    self.mDebugRTTImageSets.push_back(imgSet) 
                    imgSet.defineImage("RttImage",
                        CEGUI.Point(0.0, 0.0),
                        CEGUI.Size(tex.getWidth(), tex.getHeight()),
                        CEGUI.Point(0.0,0.0)) 


                    item = CEGUI.ListboxTextItem(texDef.name, 0, imgSet) ## new
                    item.setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush") 
                    item.setSelectionColours(CEGUI.colour(0,0,1)) 
                    self.mDebugRTTListbox.addItem(item) 








