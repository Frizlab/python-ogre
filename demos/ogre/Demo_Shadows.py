# -----------------------------------------------------------------------------
# This source file is part of OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http:##www.ogre3d.org/

# Copyright (c) 2000-2006 Torus Knot Software Ltd
# Also see acknowledgements in Readme.html

# You may use self sample code for anything you like, it is not covered by the
# LGPL like the rest of the engine.
# -----------------------------------------------------------------------------
import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as Ogre
import ogre.gui.CEGUI as CEGUI
import ogre.io.OIS as OIS
import ctypes
import SampleFramework

import exceptions, random

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
    
# # Entity* self.pPlaneEnt 
# # std.vector<Entity*> self.pColumns 
mLightAnimationState = 0 

## New depth shadowmapping
CUSTOM_ROCKWALL_MATERIAL="Ogre/DepthShadowmap/Receiver/RockWall" 
CUSTOM_CASTER_MATERIAL="Ogre/DepthShadowmap/Caster/Float"
CUSTOM_RECEIVER_MATERIAL="Ogre/DepthShadowmap/Receiver/Float" 
CUSTOM_ATHENE_MATERIAL="Ogre/DepthShadowmap/Receiver/Athene" 

BASIC_ROCKWALL_MATERIAL="Examples/Rockwall" 
BASIC_ATHENE_MATERIAL="Examples/Athene/NormalMapped" 

# This class 'wibbles' the light and billboard 
class LightWibbler (Ogre.ControllerValueFloat):

    def __init__( self, light,  billboard,   minColour,  maxColour,  minSize, maxSize):
        Ogre.ControllerValueFloat.__init__(self)
        self.mLight = light 
        self.mBillboard = billboard 
        self.mMinColour = minColour 
        self.mColourRange.r = maxColour.r - minColour.r 
        self.mColourRange.g = maxColour.g - minColour.g 
        self.mColourRange.b = maxColour.b - minColour.b 
        self.mMinSize = minSize 
        self.mSizeRange = maxSize - minSize 

    def getValue (self) :
        return self.intensity 

    def  setValue (self, value):
        self.intensity = value 

        newColour = Ogre.ColourValue()
        ## Attenuate the brightness of the light
        newColour.r = self.mMinColour.r + (self.mColourRange.r * self.intensity) 
        newColour.g = self.mMinColour.g + (self.mColourRange.g * self.intensity) 
        newColour.b = self.mMinColour.b + (self.mColourRange.b * self.intensity) 

        self.mLight.setDiffuseColour(newColour) 
        self.mBillboard.setColour(newColour) 
        ## set billboard size
        newSize = self.mMinSize + (self.intensity * self.mSizeRange) 
        self.mBillboard.setDimensions(newSize, newSize) 

##---------------------------------------------------------------------------
class GaussianListener( Ogre.CompositorInstance.Listener ):

    ## Array params - have to pack in groups of 4 since self is how Cg generates them
    ## also prevents dependent texture read problems if ops don't require swizzle
#   float self.mBloomTexWeights[15][4] 
#   float self.mBloomTexOffsetsHorz[15][4] 
#   float self.mBloomTexOffsetsVert[15][4] 

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

    def notifyMaterialRender(self, pass_id,mat):
        pass


class ShadowsListener ( SampleFramework.FrameListener, OIS.MouseListener, OIS.KeyListener ):
    def __init__ (self, win, cam, sm, app):
        SampleFramework.FrameListener.__init__(self, win, cam, True, True)
        OIS.KeyListener.__init__(self)
        OIS.MouseListener.__init__(self)
        self.app = app
        self.sceneManager = sm
        self.mWindow = win
        self.camera=cam
        self.mShutdownRequested = False
        self.mLMBDown=False
        self.mRMBDown=False
        self.mProcessMovement=False
        self.mUpdateMovement=False
        self.mMoveFwd=False
        self.mMoveBck=False
        self.mMoveLeft=False
        self.mMoveRight=False
        self.mLastMousePositionSet=False
        self.mAvgFrameTime =0.1
        self.mWriteToFile = False
        self.mTranslateVector = Ogre.Vector3.ZERO 
        self.mQuit = False
        self.mSkipCount = 0
        self.mUpdateFreq=10
        self.mRotX = 0 
        self.mRotY = 0 

# #         windowHnd = self.mWindow.getRenderWindow().getCustomAttributeInt("WINDOW")
# #         self.mInputManager = \
# #              OIS.createPythonInputSystem([("WINDOW",str(windowHnd))])
# #         
# #         ##Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
# #         self.mKeyboard = self.mInputManager.createInputObjectKeyboard( OIS.OISKeyboard, True )
# #         self.mMouse = self.mInputManager.createInputObjectMouse( OIS.OISMouse, True )

# #         width, height, depth, left, top = self.mWindow.getRenderWindow().getMetrics()

# #         ##Set Mouse Region.. if window resizes, we should alter this to reflect as well
# #         ms = self.mMouse.getMouseState() 
# #         ms.width = width 
# #         ms.height = height 

        self.Mouse.setEventCallback(self) 
        self.Keyboard.setEventCallback(self) 

        self.rootGuiPanel = CEGUI.WindowManager.getSingleton().getWindow("Shadows") 

        self.mMoveSpeed = 10.0 

        ## Set up a debug panel to display the shadow
        debugMat = Ogre.MaterialManager.getSingleton().create("Ogre/DebugShadowMap0", 
                Ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME) 
        debugMat.getTechnique(0).getPass(0).setLightingEnabled=False 
        shadowTex = self.sceneManager.getShadowTexture(0) 
        t = debugMat.getTechnique(0).getPass(0).createTextureUnitState(shadowTex.getName()) 
        t.setTextureAddressingMode(Ogre.TextureUnitState.TAM_CLAMP) 

        debugMat = Ogre.MaterialManager.getSingleton().create("Ogre/DebugShadowMap1", 
                        Ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME) 
        debugMat.getTechnique(0).getPass(0).setLightingEnabled=False 
        shadowTex = self.sceneManager.getShadowTexture(1) 
        t = debugMat.getTechnique(0).getPass(0).createTextureUnitState(shadowTex.getName()) 
        t.setTextureAddressingMode(Ogre.TextureUnitState.TAM_CLAMP) 

#         ## Uncomment self to display the shadow textures
#         debugPanel = OverlayManager.getSingleton().createOverlayElement("Panel", "Ogre/DebugShadowPanel0")
#         debugPanel._setPosition(0.8, 0) 
#         debugPanel._setDimensions(0.2, 0.2) 
#         debugPanel.setMaterialName("Ogre/DebugShadowMap0") 
#         debugOverlay = OverlayManager.getSingleton().getByName("Core/DebugOverlay") 
#         debugOverlay.add2D(debugPanel) 
#         
#         debugPanel = OverlayManager.getSingleton().createOverlayElement("Panel", "Ogre/DebugShadowPanel1")
#         debugPanel._setPosition(0.8, 0.2) 
#         debugPanel._setDimensions(0.2, 0.2) 
#         debugPanel.setMaterialName("Ogre/DebugShadowMap1") 
#         debugOverlay.add2D(debugPanel) 
#         ##
        

##--------------------------------------------------------------------------
    def frameStarted( self, evt):
        self.Mouse.capture() 
        self.Keyboard.capture() 
        if( self.mWindow.isActive() == False ):
            return False 
    
        if (self.mQuit):
            return False 
        else:
            self.mSkipCount+=1 
            if (self.mSkipCount >= self.mUpdateFreq):
                self.mSkipCount = 0 
# # #                 self.updateStats() 
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

                self.camera.yaw(Ogre.Degree(self.mRotX)) 
                self.camera.pitch(Ogre.Degree(self.mRotY)) 
                self.camera.moveRelative(self.mTranslateVector) 

                self.mUpdateMovement = False 
                self.mRotX = 0 
                self.mRotY = 0 
                self.mTranslateVector = Ogre.Vector3.ZERO 

            if(self.mWriteToFile):
                self.mNumScreenShots +=1
                self.mWindow.writeContentsToFile("frame_" +
                    str(self.mNumScreenShots) + ".png") 
            return True 

    def frameEnded( self, evt):
        if (self.app.mAnimState):
            self.app.mAnimState.addTime(evt.timeSinceLastFrame) 
        if (self.mShutdownRequested):
            return False 
        else:
            return SampleFramework.FrameListener.frameEnded(self, evt) 


##--------------------------------------------------------------------------
    def mouseMoved (self, e):
        CEGUI.System.getSingleton().injectMouseMove( e.get_state().X.rel, e.get_state().Y.rel ) 
        CEGUI.System.getSingleton().injectMouseWheelChange(e.get_state().Z.rel) 
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
    def keyPressed ( self, e):
        ## give 'quitting' priority
        if (e.key == OIS.KC_ESCAPE):
            self.mQuit = True 
            return False 

        if (e.key == OIS.KC_SYSRQ ):
            ss = "screenshot_" + str(self.mNumScreenShots) + ".png" 
            self.mWindow.writeContentsToFile(ss.str()) 
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
            self.root.releaseInput() 

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
            self.root.captureInput() 

        return True 


##--------------------------------------------------------------------------
    def handleMouseWheelEvent( self, e):
        self.mTranslateVector.z += e.wheelChange * -5.0 
        self.mUpdateMovement = True 
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


##--------------------------------------------------------------------------
    def handleKeyDownEvent( self,  e):
        CheckMovementKeys( e.scancode , True) 

        return True 

##--------------------------------------------------------------------------
    def handleKeyUpEvent( self,  e):
        self.CheckMovementKeys( e.scancode, False ) 

        return True 


class ShadowsApplication ( SampleFramework.Application ):

    def __init__ ( self ):
        SampleFramework.Application.__init__(self)
        self.UNIFORM=0
        self.UNIFORM_FOCUSED=1
        self.LISPSM=2
        self.PLANE_OPTIMAL=3
        self.MAT_STANDARD=0
        self.MAT_DEPTH_FLOAT=1
        self.MAT_DEPTH_FLOAT_PCF=2
        
        self.ShadowProjection =['UNIFORM','UNIFORM_FOCUSED','LISPSM','PLANE_OPTIMAL']
        self.ShadowMaterial = ['MAT_STANDARD','MAT_DEPTH_FLOAT','MAT_DEPTH_FLOAT_PCF']
        self.pColumns = []
        self.ListItems=[]
        self.mMinLightColour=[0.2, 0.1, 0.0] 
        self.mMaxLightColour=[0.5, 0.3, 0.1]
        self.mMinFlareSize = 40 
        self.mMaxFlareSize = 80 

        
    def __del__ ( self ): 
        self.mDescWindow = 0 
        if(self.mGUISystem):
            del self.mGUISystem 
        if(self.mGUIRenderer):
            del self.mGUIRenderer 
        del self.mPlane 

    ## Override self to ensure FPU mode
    def _configure(self):
        ## Show the configuration dialog and initialise the system
        ## You can skip self and use root.restoreConfig() to load configuration
        ## settings if you were sure there are valid ones saved in ogre.cfg
        if(self.root.showConfigDialog()):
            ## Custom option - to use PlaneOptimalShadowCameraSetup we must have
            ## double-precision. Thus, set the D3D floating point mode if present, 
            ## no matter what was chosen
            optMap = self.root.getRenderSystem().getConfigOptions() 
            entry = optMap.has_key("Floating-point mode") 
            if entry:
                co= optMap["Floating-point mode"].currentValue
                if (co != "Consistent"):
                    self.root.getRenderSystem().setConfigOption ( "Floating-point mode","Consistent" )
                    Ogre.LogManager.getSingleton().logMessage("Demo_Shadows: overriding "
                        "D3D floating point mode to 'Consistent' to ensure precision "
                        "for plane-optimal camera setup option") 
            ## If returned True, user clicked OK so initialise
            ## Here we choose to let the system create a default rendering window by passing 'True'
            self.renderWindow = self.root.initialise(True, "Shadows Demo") 
            return True 
        else:
            return False 

    def generalSceneSetup(self):
        ## do self first so we generate edge lists
        self.sceneManager.setShadowTechnique(Ogre.SHADOWTYPE_STENCIL_ADDITIVE) 
        self.mCurrentShadowTechnique = Ogre.SHADOWTYPE_STENCIL_ADDITIVE 

        ## Set ambient light off
        self.sceneManager.setAmbientLight((0.0, 0.0, 0.0)) 

        ## Fixed light, dim
        self.mSunLight = self.sceneManager.createLight("SunLight") 
        self.mSunLight.setType(Ogre.Light.LT_SPOTLIGHT) 
        self.mSunLight.setPosition(1500,1750,1300) 
        self.mSunLight.setSpotlightRange(30, 50) 
        _dir = Ogre.Vector3()
        _dir = -self.mSunLight.getPosition() 
        _dir.normalise() 
        self.mSunLight.setDirection(_dir) 
        self.mSunLight.setDiffuseColour(0.35, 0.35, 0.38) 
        self.mSunLight.setSpecularColour(0.9, 0.9, 1) 

        ## Point light, movable, reddish
        self.mLight = self.sceneManager.createLight("Light2") 
        self.mLight.setDiffuseColour(self.mMinLightColour) 
        self.mLight.setSpecularColour(1, 1, 1) 
        self.mLight.setAttenuation(8000,1,0.0005,0) 

        ## Create light node
        mLightNode = self.sceneManager.getRootSceneNode().createChildSceneNode("MovingLightNode") 
        mLightNode.attachObject(self.mLight) 
        ## create billboard set
        bbs = self.sceneManager.createBillboardSet("lightbbs", 1) 
        bbs.setMaterialName("Examples/Flare") 
        bb = bbs.createBillboard(0,0,0,self.mMinLightColour) 
        ## attach
        mLightNode.attachObject(bbs) 

# #         ## create controller, after self is will get updated on its own
# #         self.wfc=Ogre.WaveformControllerFunction(Ogre.WFT_SINE, 0.75, 0.5)
# #         func = Ogre.ControllerFunctionFloat(self.wfc) 
# #         contMgr = Ogre.ControllerManager.getSingleton() 
# #         self.lw=Ogre.LightWibbler(self.mLight, bb, self.mMinLightColour, self.mMaxLightColour, 
# #             self.mMinFlareSize, self.mMaxFlareSize)
# #         val = Ogre.ControllerValueFloat( self.lw ) 
# #         controller = contMgr.createController(contMgr.getFrameTimeSource(), val, func) 

        ##self.mLight.setPosition(Ogre.Vector3(300,250,-300)) 
        mLightNode.setPosition(Ogre.Vector3(300,1750,-700)) 


        ## Create a track for the light
        anim = self.sceneManager.createAnimation("LightTrack", 20) 
        ## Spline it for nice curves
        anim.setInterpolationMode(Ogre.Animation.IM_SPLINE) 
        ## Create a track to animate the camera's node
        track = anim.createNodeTrack(0, mLightNode) 
        ## Setup keyframes
        key = track.createNodeKeyFrame(0)  ## A startposition
        key.setTranslate(Ogre.Vector3(300,750,-700)) 
        key = track.createNodeKeyFrame(2) ##B
        key.setTranslate(Ogre.Vector3(150,800,-250)) 
        key = track.createNodeKeyFrame(4) ##C
        key.setTranslate(Ogre.Vector3(-150,850,-100)) 
        key = track.createNodeKeyFrame(6) ##D
        key.setTranslate(Ogre.Vector3(-400,700,-200)) 
        key = track.createNodeKeyFrame(8) ##E
        key.setTranslate(Ogre.Vector3(-200,700,-400)) 
        key = track.createNodeKeyFrame(10) ##F
        key.setTranslate(Ogre.Vector3(-100,850,-200)) 
        key = track.createNodeKeyFrame(12) ##G
        key.setTranslate(Ogre.Vector3(-100,575,180)) 
        key = track.createNodeKeyFrame(14) ##H
        key.setTranslate(Ogre.Vector3(0,750,300)) 
        key = track.createNodeKeyFrame(16) ##I
        key.setTranslate(Ogre.Vector3(100,850,100)) 
        key = track.createNodeKeyFrame(18) ##J
        key.setTranslate(Ogre.Vector3(250,800,0)) 
        key = track.createNodeKeyFrame(20) ##K == A
        key.setTranslate(Ogre.Vector3(300,750,-700)) 
        ## Create a new animation state to track self
        self.mAnimState = self.sceneManager.createAnimationState("LightTrack") 
        self.mAnimState.setEnabled(True) 
        ## Make light node look at origin, self is for when we
        ## change the moving light to a spotlight
        mLightNode.setAutoTracking(True, self.sceneManager.getRootSceneNode()) 

        ## Prepare athene mesh for normalmapping
        pAthene = Ogre.MeshManager.getSingleton().load("athene.mesh", 
            Ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME) 
        ret, src, dest =pAthene.suggestTangentVectorBuildParams( Ogre.VES_TANGENT , 1, 1)
        if ( not ret):
            pAthene.buildTangentVectors(VES_TANGENT, src, dest) 
        
        node = self.sceneManager.getRootSceneNode().createChildSceneNode() 
        self.mAthene = self.sceneManager.createEntity( "athene", "athene.mesh" ) 
        self.mAthene.setMaterialName(BASIC_ATHENE_MATERIAL) 
        node.attachObject( self.mAthene ) 
        node.translate(0,-27, 0) 
        node.yaw(Ogre.Degree(90)) 

        ## Columns
        for x in range (-2, 2 ):
            for z in range (-2, 2):
                if (x != 0 or z != 0):
                    str_ = "col" + str( x ) + "_" + str( z  )
                    node = self.sceneManager.getRootSceneNode().createChildSceneNode() 
                    pEnt = self.sceneManager.createEntity( str_, "column.mesh" ) 
                    pEnt.setMaterialName(BASIC_ROCKWALL_MATERIAL) 
                    self.pColumns.append(pEnt) 
                    node.attachObject( pEnt ) 
                    node.translate(x*300,0, z*300) 
        ## Skybox
        self.sceneManager.setSkyBox(True, "Examples/StormySkyBox") 

        ## Floor plane (use POSM plane def)
        self.mPlane = Ogre.MovablePlane("mPlane") 
        self.mPlane.normal = Ogre.Vector3.UNIT_Y 
        self.mPlane.d = 107 
        Ogre.MeshManager.getSingleton().createPlane("Myplane",
            Ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, self.mPlane,
            1500,1500,50,50,True,1,5,5,Ogre.Vector3.UNIT_Z) 
        self.pPlaneEnt = self.sceneManager.createEntity( "plane", "Myplane" ) 
        self.pPlaneEnt.setMaterialName(BASIC_ROCKWALL_MATERIAL) 
        self.pPlaneEnt.setCastShadows=False 
        self.sceneManager.getRootSceneNode().createChildSceneNode().attachObject(self.pPlaneEnt) 

        if (self.root.getRenderSystem().getCapabilities().hasCapability(Ogre.RSC_HWRENDER_TO_TEXTURE)):
            ## In D3D, use a 1024x1024 shadow texture
            self.sceneManager.setShadowTextureSettings(1024, 2) 
        else:
            ## Use 512x512 texture in GL since we can't go higher than the window res
            self.sceneManager.setShadowTextureSettings(512, 2) 

        self.sceneManager.setShadowColour(Ogre.ColourValue(0.5, 0.5, 0.5)) 

        ## incase infinite far distance is not supported
        self.camera.setFarClipDistance(100000) 

        ##self.sceneManager.setShowDebugShadows(True) 
        self.camera.setPosition(250, 20, 400) 
        self.camera.lookAt(0, 10, 0) 

    
    ## Just override the mandatory create scene method
    def _createScene(self):
        ## Need to detect D3D or GL for best depth shadowmapping
        if Ogre.Root.getSingleton().getRenderSystem().getName().find("GL") :
            self.mIsOpenGL = True 
        else:
            self.mIsOpenGL = False 
        ## set up general scene (self defaults to additive stencils)
        self.generalSceneSetup() 
        self.setupGUI() 

    ##/ Change basic shadow technique 
    def changeShadowTechnique(self, newTech):

        self.sceneManager.setShadowTechnique(newTech) 

        ## Below is for projection
        ##configureShadowCameras(self.mCurrentShadowTechnique, newTech) 

        self.configureLights(newTech) 

        ## Advanced modes - materials / compositors
        ##configureCompositors(self.mCurrentShadowTechnique, newTech) 
        ##configureTextures(self.mCurrentShadowTechnique, newTech) 
        ##configureShadowCasterReceiverMaterials(self.mCurrentShadowTechnique, newTech) 

        self.updateGUI(newTech) 
        self.mCurrentShadowTechnique = newTech 

    def configureLights(self, newTech):

        if newTech == Ogre.SHADOWTYPE_STENCIL_ADDITIVE:
            ## Fixed light, dim
            self.mSunLight.setCastShadows(True) 

            ## Point light, movable, reddish
            self.mLight.setType(Ogre.Light.LT_POINT) 
            self.mLight.setCastShadows(True) 
            self.mLight.setDiffuseColour(self.mMinLightColour) 
            self.mLight.setSpecularColour(1, 1, 1) 
            self.mLight.setAttenuation(8000,1,0.0005,0) 
        elif newTech == Ogre.SHADOWTYPE_STENCIL_MODULATIVE:
            ## Multiple lights cause obvious silhouette edges in modulative mode
            ## So turn off shadows on the direct light
            ## Fixed light, dim
            self.mSunLight.setCastShadows=False 

            ## Point light, movable, reddish
            self.mLight.setType(Light.LT_POINT) 
            self.mLight.setCastShadows(True) 
            self.mLight.setDiffuseColour(self.mMinLightColour) 
            self.mLight.setSpecularColour(1, 1, 1) 
            self.mLight.setAttenuation(8000,1,0.0005,0) 
        elif newTech == Ogre.SHADOWTYPE_TEXTURE_MODULATIVE or newTech == Ogre.SHADOWTYPE_TEXTURE_ADDITIVE:
            ## Fixed light, dim
            self.mSunLight.setCastShadows(True) 

            ## Change moving light to spotlight
            ## Point light, movable, reddish
            self.mLight.setType(Light.LT_SPOTLIGHT) 
            self.mLight.setDirection(Ogre.Vector3.NEGATIVE_UNIT_Z) 
            self.mLight.setCastShadows(True) 
            self.mLight.setDiffuseColour(self.mMinLightColour) 
            self.mLight.setSpecularColour(1, 1, 1) 
            self.mLight.setAttenuation(8000,1,0.0005,0) 
            self.mLight.setSpotlightRange(Degree(80),Degree(90)) 

    def setupGUI( self ):
        ## setup GUI system
        self.mGUIRenderer = CEGUI.OgreCEGUIRenderer(self.renderWindow, 
            Ogre.RENDER_QUEUE_OVERLAY, False, 3000, self.sceneManager) 

        self.mGUISystem = CEGUI.System(self.mGUIRenderer) 

        CEGUI.Logger.getSingleton().setLoggingLevel(CEGUI.Informative) 

        ## load scheme and set up defaults
        CEGUI.SchemeManager.getSingleton().loadScheme("TaharezLookSkin.scheme") 
        self.mGUISystem.setDefaultMouseCursor("TaharezLook", "MouseArrow") 
        self.mGUISystem.setDefaultFont("BlueHighway-12") 

        sheet = CEGUI.WindowManager.getSingleton().loadWindowLayout( "shadows.layout", False)  
        self.mGUISystem.setGUISheet(sheet) 

        ## Tooltips aren't big enough, do our own
        ##self.mGUISystem.setDefaultTooltip("TaharezLook/Tooltip") 

        wmgr = CEGUI.WindowManager.getSingleton() 
        ## Get description window
        self.mDescWindow = wmgr.getWindow("Shadows/Desc") 

        wnd = wmgr.getWindow("Shadows/Stencil") 
        
        wnd.subscribeEvent(CEGUI.Window.EventMouseEnters, 
            self,"handleMouseEnter") 
        wnd.subscribeEvent(CEGUI.Window.EventMouseLeaves, 
            self,"handleMouseLeave") 
        wnd = wmgr.getWindow("Shadows/Texture") 
        wnd.subscribeEvent(CEGUI.Window.EventMouseEnters, 
            self,"handleMouseEnter") 
        wnd.subscribeEvent(CEGUI.Window.EventMouseLeaves, 
            self,"handleMouseLeave") 
        wnd = wmgr.getWindow("Shadows/Additive") 
        wnd.subscribeEvent(CEGUI.Window.EventMouseEnters, 
            self,"handleMouseEnter") 
        wnd.subscribeEvent(CEGUI.Window.EventMouseLeaves, 
            self,"handleMouseLeave") 
        wnd = wmgr.getWindow("Shadows/Modulative") 
        wnd.subscribeEvent(CEGUI.Window.EventMouseEnters, 
            self,"handleMouseEnter") 
        wnd.subscribeEvent(CEGUI.Window.EventMouseLeaves, 
            self,"handleMouseLeave") 
        
        ## Combo doesn't raise enter / exit itself, have to grab subcomponents?
        cbo = wmgr.getWindow("Shadows/Projection") 
        cbo.subscribeEvent(CEGUI.Combobox.EventListSelectionAccepted,
            self,"handleProjectionChanged" ) 
        cbo.getEditbox().subscribeEvent(CEGUI.Window.EventMouseEnters, 
            self,"handleMouseEnterCombo" ) 
        cbo.getEditbox().subscribeEvent(CEGUI.Window.EventMouseLeaves, 
            self,"handleMouseLeave" ) 
        cbo.getDropList().subscribeEvent(CEGUI.Window.EventMouseEnters, 
            self,"handleMouseEnterCombo" ) 
        cbo.getDropList().subscribeEvent(CEGUI.Window.EventMouseLeaves, 
            self,"handleMouseLeave" ) 
        cbo.getPushButton().subscribeEvent(CEGUI.Window.EventMouseEnters, 
            self,"handleMouseEnterCombo" ) 
        cbo.getPushButton().subscribeEvent(CEGUI.Window.EventMouseLeaves, 
            self,"handleMouseLeave" ) 
        ## Populate projection
        ## Get a select image to be used for highlighting items in listbox when mouse moves over them
        selectImage = CEGUI.ImagesetManager.getSingleton().getImageset("TaharezLook").getImage("MultiListSelectionBrush") 
        li = CEGUI.ListboxTextItem("Uniform", self.UNIFORM) 
        li.setSelectionBrushImage(selectImage) 
        li.setTooltipText("Uniform: Shadows are rendered and projected using a uniform "
            "frustum for the whole light coverage. Simple and lowest quality.") 
        li.AutoDeleted=False
        self.ListItems.append(li)
        cbo.addItem(li) 
        cbo.setItemSelectState(li, True) 
        cbo.setText("Uniform") 
        self.mCurrentProjection = self.UNIFORM 
        li = CEGUI.ListboxTextItem("Uniform Focused", self.UNIFORM_FOCUSED) 
        li.setSelectionBrushImage(selectImage) 
        li.setTooltipText("Uniform Focused: As Uniform except that the frustum is "
            "focused on the visible area of the camera. Better quality than Uniform "
            "at the expense of some 'swimming'.") 
        li.AutoDeleted=False
        self.ListItems.append(li)
        cbo.addItem(li) 
        li = CEGUI.ListboxTextItem("LiSPSM", self.LISPSM) 
        li.setSelectionBrushImage(selectImage) 
        li.setTooltipText("LiSPSM: The frustum is distorted to take into account "
            "the perspective of the camera, and focused on the visible area. "
            "Good quality & flexibility.") 
        li.AutoDeleted=False
        self.ListItems.append(li)
        cbo.addItem(li) 
        li = CEGUI.ListboxTextItem("Plane Optimal", self.PLANE_OPTIMAL) 
        li.setSelectionBrushImage(selectImage) 
        li.setTooltipText("Plane Optimal: The frustum is optimised to project "
            "shadows onto a plane of interest. Best possible quality for the "
            "plane, less good for other receiver angles.")
        li.AutoDeleted=False
        self.ListItems.append(li) 
        cbo.addItem(li) 

        
        cbo = wmgr.getWindow("Shadows/Material")
        cbo.getEditbox().subscribeEvent(CEGUI.Window.EventMouseEnters, 
            self,"handleMouseEnterCombo" ) 
        cbo.getEditbox().subscribeEvent(CEGUI.Window.EventMouseLeaves, 
            self,"handleMouseLeave" ) 
        cbo.getDropList().subscribeEvent(CEGUI.Window.EventMouseEnters, 
            self,"handleMouseEnterCombo" ) 
        cbo.getDropList().subscribeEvent(CEGUI.Window.EventMouseLeaves, 
            self,"handleMouseLeave" ) 
        cbo.getPushButton().subscribeEvent(CEGUI.Window.EventMouseEnters, 
            self,"handleMouseEnterCombo" ) 
        cbo.getPushButton().subscribeEvent(CEGUI.Window.EventMouseLeaves, 
            self,"handleMouseLeave" ) 
        cbo.subscribeEvent(CEGUI.Combobox.EventListSelectionAccepted,
            self,"handleMaterialChanged" ) 

        li = CEGUI.ListboxTextItem("Standard", self.MAT_STANDARD) 
        li.setSelectionBrushImage(selectImage) 
        li.setTooltipText("Standard Material: Shadows are rendered into a simple RGB texture "
            " and are received only by objects that are not themselves shadow casters "
            " (no self-shadowing)") 
        li.AutoDeleted=False
        self.ListItems.append(li)
        cbo.addItem(li) 
        cbo.setItemSelectState(li, True) 
        cbo.setText("Standard") 
        self.mCurrentMaterial = self.MAT_STANDARD 

        ## Only add depth shadowmapping if supported
        if Ogre.GpuProgramManager.getSingleton().isSyntaxSupported("ps_2_0") or\
                    Ogre.GpuProgramManager.getSingleton().isSyntaxSupported("glsl"):

            li = CEGUI.ListboxTextItem("Depth Shadowmap", self.MAT_DEPTH_FLOAT) 
            li.setSelectionBrushImage(selectImage) 
            li.setTooltipText("Depth Shadowmap: Shadow caster depth is rendered into a "
                " floating point texture and a depth comparison is performed on receivers "
                " (self-shadowing allowed). Requires floating point textures and shader support.")
            li.AutoDeleted=False
            self.ListItems.append(li) 
            cbo.addItem(li) 

            li = CEGUI.ListboxTextItem("Depth Shadowmap (PCF)", self.MAT_DEPTH_FLOAT_PCF) 
            li.setSelectionBrushImage(selectImage) 
            li.setTooltipText("Depth Shadowmap (PCF): Shadow caster depth is rendered into a "
                " floating point texture and a depth comparison is performed on receivers "
                " (self-shadowing allowed), with a percentage closest filter. Requires "
                "floating point textures and shader support.") 
            li.AutoDeleted=False
            self.ListItems.append(li)
            cbo.addItem(li) 


        radio = wmgr.getWindow("Shadows/Stencil") 
        radio.setSelected(True) 
        radio.subscribeEvent(CEGUI.RadioButton.EventSelectStateChanged, 
            self,"handleShadowTypeChanged" ) 
        radio = wmgr.getWindow("Shadows/Texture")
        radio.subscribeEvent(CEGUI.RadioButton.EventSelectStateChanged, 
            self,"handleShadowTypeChanged" ) 
        radio = wmgr.getWindow("Shadows/Modulative")
        radio.subscribeEvent(CEGUI.RadioButton.EventSelectStateChanged, 
            self,"handleShadowTypeChanged" ) 
        radio = wmgr.getWindow("Shadows/Additive")
        radio.setSelected(True) 
        radio.subscribeEvent(CEGUI.RadioButton.EventSelectStateChanged, 
            self,"handleShadowTypeChanged" ) 

        self.mFixedBias = wmgr.getWindow("Shadows/DepthShadowTweakGroup/FixedBias") 
        self.mFixedBias.setScrollPosition(0.002) 
        self.mFixedBias.subscribeEvent(CEGUI.Scrollbar.EventScrollPositionChanged, 
            self,"handleParamsChanged" ) 
        self.mGradientBias = wmgr.getWindow("Shadows/DepthShadowTweakGroup/SlopeBias") 
        self.mGradientBias.setScrollPosition(0.0008) 
        self.mGradientBias.subscribeEvent(CEGUI.Scrollbar.EventScrollPositionChanged, 
            self,"handleParamsChanged" ) 
        self.mGradientClamp = wmgr.getWindow("Shadows/DepthShadowTweakGroup/SlopeClamp") 
        self.mGradientClamp.setScrollPosition(0.02) 
        self.mGradientClamp.subscribeEvent(CEGUI.Scrollbar.EventScrollPositionChanged, 
            self,"handleParamsChanged" ) 

        self.mDepthShadowTweak = wmgr.getWindow("Shadows/DepthShadowTweakGroup") 
        self.mDepthShadowTweak.setVisible=False 
        self.mFixedBiasText = wmgr.getWindow("Shadows/DepthShadowTweakGroup/FixedBiasText") 
        self.mGradientBiasText = wmgr.getWindow("Shadows/DepthShadowTweakGroup/SlopeBiasText") 
        self.mGradientClampText = wmgr.getWindow("Shadows/DepthShadowTweakGroup/SlopeClampText") 
 
        self.updateGUI(self.mCurrentShadowTechnique) 

    def updateGUI(self, newTech):
        isTextureBased=True
        if newTech & Ogre.SHADOWDETAILTYPE_TEXTURE:
            isTextureBased = False   
        
        ## Stencil based technique, turn off the texture-specific options
        wmgr = CEGUI.WindowManager.getSingleton() 
        win = wmgr.getWindow("Shadows/Projection") 
        win.setEnabled(isTextureBased) 
        win = wmgr.getWindow("Shadows/Material") 
        win.setEnabled(isTextureBased) 

    ##/ callback when mouse enters a described field (non-combo)
    def handleMouseEnter( self, e):
        winargs = e 
        self.mDescWindow.setText(winargs.window.getTooltipText()) 
        return True 

    ##/ callback when mouse leaves a described field
    def handleMouseLeave( self, e):
        ##if (self.mDescWindow):
        ##  self.mDescWindow.setText("") 
        return True 

    def updateTipForCombo(self, cbo):
        text = CEGUI.String( cbo.getTooltipText() )
        text.append(" ") 
        if (cbo.getSelectedItem()):
            text.append(cbo.getSelectedItem().getTooltipText()) 
        self.mDescWindow.setText(text) 


    ##/ callback when mouse enters a described field (combo)
    def handleMouseEnterCombo( self, e):
        winargs = e
        ## get tooltip from parent combo (events raised on contained components)
        cbo = winargs.window.getParent() 
        self.updateTipForCombo(cbo) 
        return True 

    ## Callback when a shadow type combo changed
    def handleShadowTypeChanged( self, e):

        ## Only trigger change on selected
        we = e 
        radio = we.window 
        if (radio.isSelected()):
            newTech = self.mCurrentShadowTechnique 
            if radio.getID == 1:
                ## stencil 
                newTech = ((newTech & ~Ogre.SHADOWDETAILTYPE_TEXTURE) | Ogre.SHADOWDETAILTYPE_STENCIL) 
                self.resetMaterials() 
            elif radio.getID == 2:
                ## texture
                newTech = ((newTech & ~Ogre.SHADOWDETAILTYPE_STENCIL) | Ogre.SHADOWDETAILTYPE_TEXTURE) 
            elif radio.getID == 3:
                ## additive
                newTech = ((newTech & ~Ogre.SHADOWDETAILTYPE_MODULATIVE) | Ogre.SHADOWDETAILTYPE_ADDITIVE) 
            elif radio.getID == 4:
                ## modulative
                newTech = ((newTech & ~Ogre.SHADOWDETAILTYPE_ADDITIVE) | Ogre.SHADOWDETAILTYPE_MODULATIVE) 

            self.changeShadowTechnique(newTech) 
        return True 

    def handleProjectionChanged( self, e):

        winargs = e
        cbo = winargs.window 

        if (cbo.getSelectedItem()):
            proj = cbo.getSelectedItem().getID() 
            if (proj != self.mCurrentProjection):
                ## AJM WARNING
                if proj == self.UNIFORM:
                    self.mCurrentShadowCameraSetup = Ogre.ShadowCameraSetupPtr(Ogre.DefaultShadowCameraSetup()) 
                elif proj == self.UNIFORM_FOCUSED:
                    self.focused = Ogre.FocusedShadowCameraSetup()
                    self.mCurrentShadowCameraSetup =\
                        Ogre.ShadowCameraSetup(self.focused) 
                elif proj == self.LISPSM:
                        self.mLiSPSMSetup = Ogre.LiSPSMShadowCameraSetup() 
                        ##self.mLiSPSMSetup.setUseAggressiveFocusRegion=False 
                        self.mCurrentShadowCameraSetup = Ogre.ShadowCameraSetupPtr(self.mLiSPSMSetup) 
                elif proj == self.PLANE_OPTIMAL:
                    self.mCurrentShadowCameraSetup =\
                        Ogre.ShadowCameraSetupPtr(Ogre.PlaneOptimalShadowCameraSetup(self.mPlane)) 

                self.mCurrentProjection = proj 
                self.sceneManager.setShadowCameraSetup(self.mCurrentShadowCameraSetup) 
                self.updateTipForCombo(cbo) 
                if (not self.mCustomRockwallVparams.isNull() and not self.mCustomRockwallFparams.isNull()):
                    ## set
                    self.setDefaultDepthShadowParams() 

        return True 


    def updateDepthShadowParams(self):

        self.mCustomRockwallFparams.setNamedConstant("fixedDepthBias", 
            self.mFixedBias.getScrollPosition()) 
        self.mCustomRockwallFparams.setNamedConstant("gradientScaleBias",
            self.mGradientBias.getScrollPosition()) 
        self.mCustomRockwallFparams.setNamedConstant("gradientClamp",
            self.mGradientClamp.getScrollPosition()) 

        self.mCustoself.mAtheneFparams.setNamedConstant("fixedDepthBias", 
            self.mFixedBias.getScrollPosition()) 
        self.mCustoself.mAtheneFparams.setNamedConstant("gradientScaleBias",
            self.mGradientBias.getScrollPosition()) 
        self.mCustoself.mAtheneFparams.setNamedConstant("gradientClamp",
            self.mGradientClamp.getScrollPosition()) 


#         self.mFixedBiasText.setText(StringConverter.toString(
#             self.mFixedBias.getScrollPosition(), 4, 5, '0', std.ios.fixed)) 
#         self.mGradientBiasText.setText(StringConverter.toString(
#             self.mGradientBias.getScrollPosition(), 4, 5, '0', std.ios.fixed)) 
#         self.mGradientClampText.setText(StringConverter.toString(
#             self.mGradientClamp.getScrollPosition(), 4, 5, '0', std.ios.fixed)) 


    def setDefaultDepthShadowParams(self):
        if self.mCurrentProjection == self.UNIFORM or self.mCurrentProjection==  self.UNIFORM_FOCUSED\
            or self.mCurrentProjection == self.PLANE_OPTIMAL:
            self.mFixedBias.setScrollPosition(0.0) 
            self.mGradientBias.setScrollPosition(0.0) 
        elif self.mCurrentProjection == self.LISPSM:
            self.mFixedBias.setScrollPosition(0.009) 
            self.mGradientBias.setScrollPosition(0.04) 
        self.updateDepthShadowParams() 

    def handleParamsChanged( self,e):
        if ( notself.mCustomRockwallVparams.isNull() and not self.mCustomRockwallFparams.isNull()):
            self.updateDepthShadowParams() 
        return True 

    def rebindDebugShadowOverlays(self):
        debugMat = Ogre.MaterialManager.getSingleton().getByName("Ogre/DebugShadowMap0") 
        shadowTex = self.sceneManager.getShadowTexture(0) 
        debugMat.getTechnique(0).getPass(0).getTextureUnitState(0).setTextureName(shadowTex.getName()) 

        debugMat = Ogre.MaterialManager.getSingleton().getByName("Ogre/DebugShadowMap1") 
        shadowTex = self.sceneManager.getShadowTexture(1) 
        debugMat.getTechnique(0).getPass(0).getTextureUnitState(0).setTextureName(shadowTex.getName()) 


    def resetMaterials(self):
        ## Sort out base materials
        self.pPlaneEnt.setMaterialName(BASIC_ROCKWALL_MATERIAL) 
        self.mAthene.setMaterialName(BASIC_ATHENE_MATERIAL) 
        for i in self.pColumns: 
            i.setMaterialName(BASIC_ROCKWALL_MATERIAL) 
        self.mCustomRockwallVparams.setNull() 
        self.mCustomRockwallFparams.setNull() 
        self.mCustoself.mAtheneVparams.setNull() 
        self.mCustoself.mAtheneFparams.setNull() 


    def handleMaterialChanged( self, e):
        winargs = e
        cbo = winargs.window

        if (cbo.getSelectedItem()):
            mat = cbo.getSelectedItem().getID() 
            if (mat != self.mCurrentMaterial):
                if mat ==  self.MAT_STANDARD:
                    self.sceneManager.setShadowTexturePixelFormat(Ogre.PF_L8) 
                    self.sceneManager.setShadowTextureCasterMaterial(Ogre.StringUtil.BLANK) 
                    self.sceneManager.setShadowTextureReceiverMaterial(Ogre.StringUtil.BLANK) 
                    self.sceneManager.setShadowTextureSelfShadow=False    
                    self.mDepthShadowTweak.setVisible=False 
                    self.resetMaterials() 
                elif mat ==  self.MAT_DEPTH_FLOAT:
                    if (self.mIsOpenGL):
                        ## GL performs much better if you pick half-float format
                        self.sceneManager.setShadowTexturePixelFormat(Ogre.PF_FLOAT16_R) 
                    else:
                        ## D3D is the opposite - if you ask for PF_FLOAT16_R you
                        ## get an integer format instead! You can ask for PF_FLOAT16_GR
                        ## but the precision doesn't work well
                        self.sceneManager.setShadowTexturePixelFormat(Ogre.PF_FLOAT32_R) 
                    self.sceneManager.setShadowTextureCasterMaterial(CUSTOM_CASTER_MATERIAL) 
                    self.sceneManager.setShadowTextureReceiverMaterial(CUSTOM_RECEIVER_MATERIAL) 
                    self.sceneManager.setShadowTextureSelfShadow(True)     
                    ## Sort out base materials
                    self.pPlaneEnt.setMaterialName(CUSTOM_ROCKWALL_MATERIAL) 
                    self.mAthene.setMaterialName(CUSTOM_ATHENE_MATERIAL) 
                    for i in self.pColumns:
                        i.setMaterialName(CUSTOM_ROCKWALL_MATERIAL) 

                    themat = Ogre.MaterialManager.getSingleton().getByName(CUSTOM_ROCKWALL_MATERIAL) 
                    self.mCustomRockwallVparams = themat.getTechnique(0).getPass(1).getShadowReceiverVertexProgramParameters() 
                    self.mCustomRockwallFparams = themat.getTechnique(0).getPass(1).getShadowReceiverFragmentProgramParameters() 
                    themat = Ogre.MaterialManager.getSingleton().getByName(CUSTOM_ATHENE_MATERIAL) 
                    self.mCustoself.mAtheneVparams = themat.getTechnique(0).getPass(1).getShadowReceiverVertexProgramParameters() 
                    self.mCustoself.mAtheneFparams = themat.getTechnique(0).getPass(1).getShadowReceiverFragmentProgramParameters() 
                    self.mDepthShadowTweak.setVisible(True) 
                    ## set the current params
                    self.setDefaultDepthShadowParams() 
                elif mat == self.MAT_DEPTH_FLOAT_PCF:
                    if (self.mIsOpenGL):
                        ## GL performs much better if you pick half-float format
                        self.sceneManager.setShadowTexturePixelFormat(Ogre.PF_FLOAT16_R) 
                    else:
                        ## D3D is the opposite - if you ask for PF_FLOAT16_R you
                        ## get an integer format instead! You can ask for PF_FLOAT16_GR
                        ## but the precision doesn't work well
                        self.sceneManager.setShadowTexturePixelFormat(Ogre.PF_FLOAT32_R) 

                    self.sceneManager.setShadowTextureCasterMaterial(CUSTOM_CASTER_MATERIAL) 
                    self.sceneManager.setShadowTextureReceiverMaterial(CUSTOM_RECEIVER_MATERIAL + "/PCF") 
                    self.sceneManager.setShadowTextureSelfShadow(True)     
                    ## Sort out base materials
                    self.pPlaneEnt.setMaterialName(CUSTOM_ROCKWALL_MATERIAL + "/PCF") 
                    self.mAthene.setMaterialName(CUSTOM_ATHENE_MATERIAL + "/PCF") 
                    for i in self.pColumns:
                        i.setMaterialName(CUSTOM_ROCKWALL_MATERIAL + "/PCF") 
                    themat = Ogre.MaterialManager.getSingleton().getByName(CUSTOM_ROCKWALL_MATERIAL + "/PCF") 
                    self.mCustomRockwallVparams = themat.getTechnique(0).getPass(1).getShadowReceiverVertexProgramParameters() 
                    self.mCustomRockwallFparams = themat.getTechnique(0).getPass(1).getShadowReceiverFragmentProgramParameters() 
                    themat = Ogre.MaterialManager.getSingleton().getByName(CUSTOM_ATHENE_MATERIAL + "/PCF") 
                    self.mCustoself.mAtheneVparams = themat.getTechnique(0).getPass(1).getShadowReceiverVertexProgramParameters() 
                    self.mCustoself.mAtheneFparams = themat.getTechnique(0).getPass(1).getShadowReceiverFragmentProgramParameters() 
                    self.mDepthShadowTweak.setVisible(True) 

                    ## set the current params
                    self.setDefaultDepthShadowParams() 

                self.mCurrentMaterial = mat 

                self.updateTipForCombo(cbo) 
                self.rebindDebugShadowOverlays() 
        return True 


    ## Create new frame listener
    def _createFrameListener(self):

        shadowListener = ShadowsListener(self.renderWindow, self.camera, self.sceneManager, self) 
        self.frameListener = shadowListener 
        self.frameListener.showDebugOverlay(True) 
        self.root.addFrameListener(self.frameListener) 

        ## Hook up Root sheet (handles main input)
        wnd = CEGUI.WindowManager.getSingleton().getWindow("Shadows") 
        wnd.subscribeEvent(CEGUI.Window.EventMouseMove, 
            self.frameListener,"handleMouseMove") 
        wnd.subscribeEvent(CEGUI.Window.EventMouseButtonUp, 
            self.frameListener,"handleMouseButtonUp") 
        wnd.subscribeEvent(CEGUI.Window.EventMouseButtonDown, 
            self.frameListener,"handleMouseButtonDown") 
        wnd.subscribeEvent(CEGUI.Window.EventMouseWheel, 
            self.frameListener,"handleMouseWheelEvent") 
        wnd.subscribeEvent(CEGUI.Window.EventKeyDown, 
            self.frameListener,"handleKeyDownEvent") 
        wnd.subscribeEvent(CEGUI.Window.EventKeyUp, 
            self.frameListener,"handleKeyUpEvent") 

#     def go(self):
#         if ( not self.setUp()):
#             return 
#         self.root.startRendering() 


if __name__ == '__main__':
    try:
        sa = ShadowsApplication()
        sa.go()
    except Ogre.OgreException, e:
        print e


