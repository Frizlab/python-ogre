# -----------------------------------------------------------------------------
# This source file is part of OGRE
#     (Object-oriented Graphics Rself.endering Engine)
# For the latest info, see http:##www.ogre3d.org/

# Copyright (c) 2000-2006 Torus Knot Software Ltd
# Also see acknowledgements in Readme.html

# You may use this sample code for anything you like, it is not covered by the
# LGPL like the rest of the engine.
# -----------------------------------------------------------------------------


# /*************************************************************************
# 	                    HeatVisionListener Methods
# *************************************************************************/
class HeatVisionListener():
##---------------------------------------------------------------------------
    def __init__(self):
		self.timer = Ogre.timer() 
        self.start = self.end = self.curr = 0.0 
##---------------------------------------------------------------------------
    def __del__(self):
       del self.timer 
##---------------------------------------------------------------------------
    def notifyMaterialSetup(self, pass_id, mat):
        if(pass_id == DEADBABE):
            self.timer.reset() 
            self.fpParams = mat.getTechnique(0).getPass(0).getFragmentProgramParameters() 
##---------------------------------------------------------------------------
    def notifyMaterialRself.ender(self, pass_id, mat):
        if pass_id == DEADBABE:
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
                if (self.curr > self.end): self.curr -= inc
                else : self.curr += inc 
            self.timer.reset() 

            self.fpParams.setNamedConstant("depth_modulator", Ogre.Vector4(self.curr, 0, 0, 0) 
##---------------------------------------------------------------------------

# 	/*************************************************************************
# 	self.hdrListener Methods
# 	*************************************************************************/
class self.hdrListener:
	##---------------------------------------------------------------------------
	def __init__ (self):
	    pass
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
				self.mBloomTexOffsetsHorz[0][0] = 0.0 
				self.mBloomTexOffsetsHorz[0][1] = 0.0 
				self.mBloomTexOffsetsVert[0][0] = 0.0 
				self.mBloomTexOffsetsVert[0][1] = 0.0 
				self.mBloomTexWeights[0][0] = self.mBloomTexWeights[0][1] =
					self.mBloomTexWeights[0][2] = Ogre.Math.gaussianDistribution(0, 0, deviation) 
				self.mBloomTexWeights[0][3] = 1.0 

				## 'pre' samples
				for i in range (8):
					self.mBloomTexWeights[i][0] = self.mBloomTexWeights[i][1] =
						self.mBloomTexWeights[i][2] = 1.25 * Ogre.Math.gaussianDistribution(i, 0, deviation) 
					self.mBloomTexWeights[i][3] = 1.0 
					self.mBloomTexOffsetsHorz[i][0] = i * texelSize 
					self.mBloomTexOffsetsHorz[i][1] = 0.0 
					self.mBloomTexOffsetsVert[i][0] = 0.0 
					self.mBloomTexOffsetsVert[i][1] = i * texelSize 
				## 'post' samples
				for i in range (8,15):
					self.mBloomTexWeights[i][0] = self.mBloomTexWeights[i][1] =
						self.mBloomTexWeights[i][2] = self.mBloomTexWeights[i - 7][0] 
					self.mBloomTexWeights[i][3] = 1.0 

					self.mBloomTexOffsetsHorz[i][0] = -self.mBloomTexOffsetsHorz[i - 7][0] 
					self.mBloomTexOffsetsHorz[i][1] = 0.0 
					self.mBloomTexOffsetsVert[i][0] = 0.0 
					self.mBloomTexOffsetsVert[i][1] = -self.mBloomTexOffsetsVert[i - 7][1] 

	##---------------------------------------------------------------------------
	def notifyMaterialSetup(self, pass_id, mat):
		## Prepare the fragment params offsets
# 		switch(pass_id)
# 		##case 994: ## rt_lum4
# 		if pass_id ==  993: ## rt_lum3
# 		case 992: ## rt_lum2
# 		case 991: ## rt_lum1
# 		case 990: ## rt_lum0
# 			break 
# 		case 800: ## rt_brightpass
# 			break 
		if pass_id == 701: ## rt_bloom1
				## horizontal bloom
				mat.load() 
				fparams = mat.getBestTechnique().getPass(0).getFragmentProgramParameters() 
				progName = mat.getBestTechnique().getPass(0).getFragmentProgramName() 
				fparams.setNamedConstant("sampleOffsets", self.mBloomTexOffsetsHorz[0], 15) 
				fparams.setNamedConstant("sampleWeights", self.mBloomTexWeights[0], 15) 

				break 
		elif pass_id == 700: ## rt_bloom0
				## vertical bloom
				mat.load() 
				fparams = mat.getTechnique(0).getPass(0).getFragmentProgramParameters() 
				progName = mat.getBestTechnique().getPass(0).getFragmentProgramName() 
				fparams.setNamedConstant("sampleOffsets", self.mBloomTexOffsetsVert[0], 15) 
				fparams.setNamedConstant("sampleWeights", self.mBloomTexWeights[0], 15) 

				break 
	##---------------------------------------------------------------------------
# # 	void self.hdrListener.notifyMaterialRself.ender(Ogre.uint32 pass_id, Ogre.MaterialPtr &mat)
	##---------------------------------------------------------------------------


# # 	/*************************************************************************
# # 	self.gaussianListener Methods
# # 	*************************************************************************/
class self.gaussianListener:
	##---------------------------------------------------------------------------
	def __init__ ( self ):
	    pass
	##---------------------------------------------------------------------------
	def notifyViewportSize(self, width, height):
		self.mVpWidth = width 
		self.mVpHeight = height 
		## Calculate gaussian texture offsets & weights
		deviation = 3.0 
		texelSize = 1.0 / min(self.mVpWidth, self.mVpHeight) 

		## central sample, no offset
		self.mBloomTexOffsetsHorz[0][0] = 0.0 
		self.mBloomTexOffsetsHorz[0][1] = 0.0 
		self.mBloomTexOffsetsVert[0][0] = 0.0 
		self.mBloomTexOffsetsVert[0][1] = 0.0 
		self.mBloomTexWeights[0][0] = self.mBloomTexWeights[0][1] =
			self.mBloomTexWeights[0][2] = Ogre.Math.gaussianDistribution(0, 0, deviation) 
		self.mBloomTexWeights[0][3] = 1.0 

		## 'pre' samples
		for i in range (1,8):
			self.mBloomTexWeights[i][0] = self.mBloomTexWeights[i][1] =
				self.mBloomTexWeights[i][2] = Ogre.Math.gaussianDistribution(i, 0, deviation) 
			self.mBloomTexWeights[i][3] = 1.0 
			self.mBloomTexOffsetsHorz[i][0] = i * texelSize 
			self.mBloomTexOffsetsHorz[i][1] = 0.0 
			self.mBloomTexOffsetsVert[i][0] = 0.0 
			self.mBloomTexOffsetsVert[i][1] = i * texelSize 
		## 'post' samples
		for i in range (8,15):
			self.mBloomTexWeights[i][0] = self.mBloomTexWeights[i][1] =
				self.mBloomTexWeights[i][2] = self.mBloomTexWeights[i - 7][0] 
			self.mBloomTexWeights[i][3] = 1.0 

			self.mBloomTexOffsetsHorz[i][0] = -self.mBloomTexOffsetsHorz[i - 7][0] 
			self.mBloomTexOffsetsHorz[i][1] = 0.0 
			self.mBloomTexOffsetsVert[i][0] = 0.0 
			self.mBloomTexOffsetsVert[i][1] = -self.mBloomTexOffsetsVert[i - 7][1] 
	##---------------------------------------------------------------------------
	def notifyMaterialSetup(self,  pass_id, mat):
		## Prepare the fragment params offsets
		if pass_id == 701: ## blur horz
				## horizontal bloom
				mat.load() 
				fparams = mat.getBestTechnique().getPass(0).getFragmentProgramParameters() 
				progName = mat.getBestTechnique().getPass(0).getFragmentProgramName() 
				fparams.setNamedConstant("sampleOffsets", self.mBloomTexOffsetsHorz[0], 15) 
				fparams.setNamedConstant("sampleWeights", self.mBloomTexWeights[0], 15) 

		elif pass_id == 700: ## blur vert
				## vertical bloom
				mat.load() 
				fparams = mat.getTechnique(0).getPass(0).getFragmentProgramParameters() 
				progName = mat.getBestTechnique().getPass(0).getFragmentProgramName() 
				fparams.setNamedConstant("sampleOffsets", self.mBloomTexOffsetsVert[0], 15) 
				fparams.setNamedConstant("sampleWeights", self.mBloomTexWeights[0], 15) 
	##---------------------------------------------------------------------------
# # 	void self.gaussianListener.notifyMaterialRself.ender(Ogre.uint32 pass_id, Ogre.MaterialPtr &mat)
	##---------------------------------------------------------------------------

# # /*************************************************************************
# # 	CompositorDemo_FrameListener methods that handle all input for this Compositor demo.
# # *************************************************************************/
class CompositorDemo_FrameListener:
    def __init__ (self, main):
        self.mMain = main
        self.mTranslateVector = Ogre.Vector3.ZERO
        mStatsOn = True
        self.mNumScreenShots = 0
        self.mWriteToFile = False
        self.mSkipCount = 0
        self.mUpdateFreq = 50
        mSceneDetailIndex = 0
        , self.mQuit(False)
        , self.mAvgFrameTime(0.1)
        , self.mLastMousePositionSet(False)
        , self.mTimeUntilNextToggle(0)
        , self.mRotX(0)
        , self.mRotY(0)
        , self.mProcessMovement(False)
        , self.mUpdateMovement(False)

        , self.mLMBDown(False)
        , self.mRMBDown(False)
        , self.mMoveFwd(False)
        , self.mMoveBck(False)
        , self.mMoveLeft(False)
        , self.mMoveRight(False)
		, self.mSpinny(0)
        , self.mCompositorSelectorViewManager(0)
		, self.mMouse(0)
		, self.mKeyboard(0)


        Ogre.Root.getSingleton().addFrameListener(self) 

        ## using buffered input
		pl = OIS.ParamList
		windowHnd = 0 

		self.mMain.getRenderWindow().getCustomAttribute("WINDOW", windowHnd) 
		windowHndStr  = str(windowHnd )
		pl.insert(std.make_pair(std.string("WINDOW"), windowHndStr.str())) 

		self.mInputManager = OIS.InputManager.createInputSystem( pl ) 

		##Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
		self.mKeyboard = static_cast<OIS.Keyboard*>(self.mInputManager.createInputObject( OIS.OISKeyboard, True )) 
		self.mMouse = static_cast<OIS.Mouse*>(self.mInputManager.createInputObject( OIS.OISMouse, True )) 

		width, height, depth, left, top = self.mMain.getRenderWindow().getMetrics()

		##Set Mouse Region.. if window resizes, we should alter this to reflect as well
		OIS.MouseState &ms = self.mMouse.getMouseState() 
		ms.width = width 
		ms.height = height 

		self.mMouse.setEventCallback(self) 
		self.mKeyboard.setEventCallback(self) 

        self.mGuiRenderer = CEGUI.System.getSingleton().getRself.enderer() 

        self.mGuiAvg   = CEGUI.WindowManager.getSingleton().getWindow("OPAverageFPS") 
        self.mGuiself.curr  = CEGUI.WindowManager.getSingleton().getWindow("OPself.currentFPS") 
        self.mGuiBest  = CEGUI.WindowManager.getSingleton().getWindow("OPBestFPS") 
        self.mGuiWorst = CEGUI.WindowManager.getSingleton().getWindow("OPWorstFPS") 
        self.mGuiTris  = CEGUI.WindowManager.getSingleton().getWindow("OPTriCount") 
        self.mGuiDbg   = CEGUI.WindowManager.getSingleton().getWindow("OPDebugMsg") 
        self.mRoot	  = CEGUI.WindowManager.getSingleton().getWindow("root") 

        registerCompositors() 
		initDebugRTTWindow() 
        connectEventHandlers() 
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
##--------------------------------------------------------------------------
    def connectEventHandlers(self):
        wndw = CEGUI.WindowManager.getSingleton().getWindow("root") 

        wndw.subscribeEvent(CEGUI.Window.EventMouseMove, 
                    CEGUI.Event.Subscriber(CompositorDemo_FrameListener.handleMouseMove, self)) 

        wndw.subscribeEvent(CEGUI.Window.EventMouseButtonUp, 
                    CEGUI.Event.Subscriber(CompositorDemo_FrameListener.handleMouseButtonUp, self)) 

        wndw.subscribeEvent(CEGUI.Window.EventMouseButtonDown, 
                    CEGUI.Event.Subscriber(CompositorDemo_FrameListener.handleMouseButtonDown, self)) 

        wndw.subscribeEvent(CEGUI.Window.EventMouseWheel, 
                    CEGUI.Event.Subscriber(CompositorDemo_FrameListener.handleMouseWheelEvent, self)) 
        wndw.subscribeEvent(CEGUI.Window.EventKeyDown, 
                    CEGUI.Event.Subscriber(CompositorDemo_FrameListener.handleKeyDownEvent, self )) 
        wndw.subscribeEvent(CEGUI.Window.EventKeyUp, 
                    CEGUI.Event.Subscriber(CompositorDemo_FrameListener.handleKeyUpEvent, self )) 
##--------------------------------------------------------------------------
    def started( self, evt):
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
                updateStats() 
            ## update movement process
            if(self.mProcessMovement or self.mUpdateMovement):
                self.mTranslateVector.x += self.mMoveLeft ? self.mAvgFrameTime * -MOVESPEED : 0 
                self.mTranslateVector.x += self.mMoveRight ? self.mAvgFrameTime * MOVESPEED : 0 
                self.mTranslateVector.z += self.mMoveFwd ? self.mAvgFrameTime * -MOVESPEED : 0 
                self.mTranslateVector.z += self.mMoveBck ? self.mAvgFrameTime * MOVESPEED : 0 

                self.mMain.getCamera().yaw(Ogre.Angle(self.mRotX)) 
                self.mMain.getCamera().pitch(Ogre.Angle(self.mRotY)) 
                self.mMain.getCamera().moveRelative(self.mTranslateVector) 

                self.mUpdateMovement = False 
                self.mRotX = 0 
                self.mRotY = 0 
                self.mTranslateVector = Ogre.Vector3.ZERO 

            if(self.mWriteToFile):
                self.mMain.getRenderWindow().writeContentsToFile("frame_" +
                    str(++self.mNumScreenShots) + ".png") 

			if (self.mSpinny):
				self.mSpinny.yaw(Ogre.Degree(10 * evt.timeSinceLastFrame)) 

            return True 

##--------------------------------------------------------------------------
    def mouseMoved (self, e):
        CEGUI.System.getSingleton().injectMouseMove( e.state.X.rel, e.state.Y.rel ) 
		CEGUI.System.getSingleton().injectMouseWheelChange(e.state.Z.rel) 
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
        self.currFps = "current FPS: " 
        avgFps = "Average FPS: " 
        bestFps = "Best FPS: " 
        worstFps = "Worst FPS: " 
        tris = "Triangle Count: " 


        stats = self.mMain.getRenderWindow().getStatistics() 

        self.mGuiAvg.setText(avgFps + str(stats.avgFPS)) 
        self.mGuiself.curr.setText(self.currFps + str(stats.lastFPS)) 
        self.mGuiBest.setText(bestFps + str(stats.bestFPS)
            + " " + str(stats.bestFrameTime)+" ms") 
        self.mGuiWorst.setText(worstFps + str(stats.worstFPS)
            + " " + str(stats.worstFrameTime)+" ms") 

        self.mGuiTris.setText(tris + str(stats.triangleCount)) 
        self.mGuiDbg.setText(self.mDebugText) 
        self.mAvgFrameTime = 1.0/(stats.avgFPS + 1.0) 
        if (self.mAvgFrameTime > 0.1) self.mAvgFrameTime = 0.1 :



##--------------------------------------------------------------------------
    def handleMouseMove( self,  e):
        if( self.mLMBDown and not self.mRMBDown):
            ## rotate camera
            self.mRotX += -e.moveDelta.d_x * self.mAvgFrameTime * 10.0 
            self.mRotY += -e.moveDelta.d_y * self.mAvgFrameTime * 10.0 
            MouseCursor.getSingleton().setPosition( self.mLastMousePosition ) 
            self.mUpdateMovement = True 
        else:
            if( self.mRMBDown and not self.mLMBDown):
                ## translate camera
                self.mTranslateVector.x += e.moveDelta.d_x * self.mAvgFrameTime * MOVESPEED 
                self.mTranslateVector.y += -e.moveDelta.d_y * self.mAvgFrameTime * MOVESPEED 
                ##self.mTranslateVector.z = 0 
                MouseCursor.getSingleton().setPosition( self.mLastMousePosition ) 
                self.mUpdateMovement = True 
            else:
                if( self.mRMBDown and self.mLMBDown):
                    self.mTranslateVector.z += (e.moveDelta.d_x + e.moveDelta.d_y) * self.mAvgFrameTime * MOVESPEED 
                    MouseCursor.getSingleton().setPosition( self.mLastMousePosition ) 
                    self.mUpdateMovement = True 

        return True 

##--------------------------------------------------------------------------
    def handleMouseButtonUp( self, e):
        using namespace CEGUI 

        ##Window* wndw = (( WindowEventArgs&)e).window 
        if e.button == LeftButton:
            self.mLMBDown = False 

        elif e.button == RightButton:
            self.mRMBDown = False 
        if( not self.mLMBDown and not self.mRMBDown ):
            MouseCursor.getSingleton().show() 
            if self.mLastMousePositionSet:
                MouseCursor.getSingleton().setPosition( self.mLastMousePosition ) 
                self.mLastMousePositionSet = False 
            self.mRoot.releaseInput() 

        return True 

##--------------------------------------------------------------------------
    def handleMouseButtonDown( self, e):

        ##Window* wndw = (( WindowEventArgs&)e).window 
        if( e.button == LeftButton ):
            self.mLMBDown = True 

        if( e.button == RightButton ):
            self.mRMBDown = True 

        if( self.mLMBDown or self.mRMBDown ):
            CEGUI.MouseCursor.getSingleton().hide() 
            if (not self.mLastMousePositionSet):
                self.mLastMousePosition = MouseCursor.getSingleton().getPosition() 
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
        Ogre.CompositorManager.getSingleton().setCompositorEnabled(self.mMain.getRenderWindow().getViewport(0),
            self.mCompositorSelectorViewManager.getItemSelectorText(index), state) 
		self.updateDebugRTTWindow() 
##-----------------------------------------------------------------------------------
    def registerCompositors(self):
        Ogre.Viewport *vp = self.mMain.getRenderWindow().getViewport(0) 
        self.hvListener = HeatVisionListener() 
		self.hdrListener = hdrListener() 
		self.gaussianListener = gaussianListener() 

        self.mCompositorSelectorViewManager = ItemSelectorViewManager("CompositorSelectorWin") 
        ## tell view manager to notify us when an item changes selection state
        self.mCompositorSelectorViewManager.setItemSelectorController(self) 
        ##iterate through Compositor Managers resources and add name keys ast Item selectors to Compositor selector view manager
        resourceIterator = Ogre.CompositorManager.getSingleton().getResourceIterator() 

        ## add all compositor resources to the view container
        while (resourceIterator.hasMoreElements())
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
	void CompositorDemo_FrameListener.initDebugRTTWindow(void)
		self.mDebugRTTStaticImage = CEGUI.WindowManager.getSingleton().getWindow((CEGUI.utf8*)"DebugRTTImage") 
		self.mDebugRTTListbox = static_cast<CEGUI.Listbox*>(
			CEGUI.WindowManager.getSingleton().getWindow((CEGUI.utf8*)"DebugRTTListbox")) 
		self.mDebugRTTListbox.subscribeEvent(CEGUI.Listbox.EventSelectionChanged,
			CEGUI.Event.Subscriber(&CompositorDemo_FrameListener.handleRttSelection, self)) 
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
		## Clear listbox
		self.mDebugRTTListbox.resetList() 
		## Clear imagesets
		self.mDebugRTTStaticImage.setProperty("Image", "") 
		for (ImageSetList.iterator isIt = mDebugRTTImageSets.begin() 
			isIt != mDebugRTTImageSets.self.end()  ++isIt)
			CEGUI.ImagesetManager.getSingleton().destroyImageset(*isIt) 
		mDebugRTTImageSets.clear() 
		## Add an entry for each rself.ender texture for all active compositors
		vp = self.mMain.getRenderWindow().getViewport(0) 
		chain = Ogre.CompositorManager.getSingleton().getCompositorChain(vp) 
		it = chain.getCompositors() 
		while (it.hasMoreElements())
			inst = it.getNext() 
			if (inst.getEnabled()):
				texIt = inst.getTechnique().getTextureDefinitionIterator() 
				while (texIt.hasMoreElements())
					texDef = texIt.getNext() 

					## Get instance name of texture
					instName = inst.getTextureInstanceName(texDef.name) 
					## Create CEGUI texture from name of OGRE texture
					tex = self.mMain.getGuiRself.enderer().createTexture(instName) 
					## Create imageset
					imgSet = CEGUI.ImagesetManager.getSingleton().createImageset(
							instName, tex) 
					mDebugRTTImageSets.push_back(imgSet) 
					imgSet.defineImage("RttImage",
						CEGUI.Point(0.0, 0.0),
						CEGUI.Size(tex.getWidth(), tex.getHeight()),
						CEGUI.Point(0.0,0.0)) 


					item = new CEGUI.ListboxTextItem(texDef.name, 0, imgSet) ## new
					item.setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush") 
					item.setSelectionColours(CEGUI.colour(0,0,1)) 
					self.mDebugRTTListbox.addItem(item) 








