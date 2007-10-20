# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http://python-ogre.org/
#
# It is likely based on original code from OGRE and/or PyOgre
# For the latest info, see http://www.ogre3d.org/
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------
import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as Ogre
import ogre.gui.CEGUI as CEGUI
from CompositorDemo_FrameListener import *
from ogre.renderer.OGRE import PixelFormat

# # /*************************************************************************
#                       CompositorDemo Methods
# # *************************************************************************/
class CompositorDemo():
    def __del__(self):
        if self.mGUISystem:
            del self.mGUISystem 
        del self.mGUIRenderer 
        del self.mFrameListener 
        del self.mRoot 

##--------------------------------------------------------------------------
    def go(self):
        if (not self.setup()):
            return False
        self.mRoot.startRendering() 

##--------------------------------------------------------------------------
    def setup(self):
        self.mRoot = Ogre.Root() 

        self.setupResources() 
        carryOn = self.configure() 
        if (not carryOn) : return False

        self.chooseSceneManager() 
        self.createCamera() 
        self.createViewports() 

        ## Set default mipmap level (NB some APIs ignore this)
        print "****3"
        Ogre.TextureManager.getSingleton().setDefaultNumMipmaps(5) 
        self.loadResources() 
        print "****4"
        ## Create the scene
        self.createScene() 
        
        print "***1"
        self.createFrameListener() 
        print "***2"
        ## load some GUI stuff for demo.
        ##loadAllMaterialControlFiles(mMaterialControlsContainer) 
        return True 


##--------------------------------------------------------------------------
    def configure(self):
        ## Show the configuration dialog and initialise the system
        ## You can skip this and use root.restoreConfig() to load configuration
        ## settings if you were sure there are valid ones saved in ogre.cfg
        if(self.mRoot.showConfigDialog()):
            ## If returned True, user clicked OK so initialise
            ## Here we choose to let the system create a default rendering window by pass_ing 'True'
            self.mWindow = self.mRoot.initialise(True) 
            return True 
        else:
            return False 

##--------------------------------------------------------------------------
    def chooseSceneManager(self):
        ## Get the SceneManager, in this case a generic one
        self.mSceneMgr = self.mRoot.createSceneManager(Ogre.ST_GENERIC, "ExampleSMInstance") 

##--------------------------------------------------------------------------
    def createCamera(self):
        ## Create the camera
        self.mCamera = self.mSceneMgr.createCamera("PlayerCam") 

        ## Position it at 500 in Z direction
        self.mCamera.setPosition(Ogre.Vector3(0,0,0)) 
        ## Look back along -Z
        self.mCamera.lookAt(Ogre.Vector3(0,0,-300)) 
        self.mCamera.setNearClipDistance(1) 


##--------------------------------------------------------------------------
    def createViewports(self):
        ## Create one viewport, entire window
        vp = self.mWindow.addViewport(self.mCamera) 
        vp.setBackgroundColour(Ogre.ColourValue(0,0,0)) 
    
        ## Alter the camera aspect ratio to match the viewport
        self.mCamera.setAspectRatio(vp.getActualWidth()/ vp.getActualHeight()) 

##--------------------------------------------------------------------------
    def setupResources(self):
        ## Load resource paths from config file
        
        config = Ogre.ConfigFile()
        config.load('resources.cfg' ) 
        seci = config.getSectionIterator()
        while seci.hasMoreElements():
            SectionName = seci.peekNextKey()
            Section = seci.getNext()
            for item in Section:
                Ogre.ResourceGroupManager.getSingleton().\
                    addResourceLocation(item.value, item.key, SectionName)
        Ogre.LogManager.getSingleton().logMessage( "Resource directories setup" ) 


##-----------------------------------------------------------------------------------
    def loadResources(self):
        ## Initialise, parse all scripts etc
        Ogre.ResourceGroupManager.getSingleton().initialiseAllResourceGroups() 


##-----------------------------------------------------------------------------------
    def createScene(self):
        print "***5"
        self.mSceneMgr.setShadowTechnique(Ogre.SHADOWTYPE_TEXTURE_MODULATIVE) 
        self.mSceneMgr.setShadowFarDistance(1000) 
        ## setup GUI system
        print "**6"
        self.mGUIRenderer = CEGUI.OgreCEGUIRenderer(self.mWindow, Ogre.RENDER_QUEUE_OVERLAY, False, 3000, self.mSceneMgr) 
        ## load scheme and set up defaults
        print "***7"
        self.mGUISystem = CEGUI.System(self.mGUIRenderer, configFile="CompositorDemoCegui.config") 
        CEGUI.System.getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow") 
        print "**8"
        Ogre.MovableObject.setDefaultVisibilityFlags(0x00000001) 

        ## Set ambient light
        self.mSceneMgr.setAmbientLight(Ogre.ColourValue(0.3, 0.3, 0.2)) 
        print "****a"
        l = self.mSceneMgr.createLight("Light2") 
        dir_ = Ogre.Vector3(-1,-1,0) 
        dir_.normalise() 
        l.setType(Ogre.Light.LT_DIRECTIONAL) 
        l.setDirection(dir_) 
        l.setDiffuseColour(1, 1, 0.8) 
        l.setSpecularColour(1, 1, 1) 

        print "****b"

        ## House
        pEnt = self.mSceneMgr.createEntity( "1", "tudorhouse.mesh" ) 
        n1 = self.mSceneMgr.getRootSceneNode().createChildSceneNode(Ogre.Vector3(350, 450, -200)) 
        n1.attachObject( pEnt ) 

        pEnt = self.mSceneMgr.createEntity( "2", "tudorhouse.mesh" ) 
        n2 = self.mSceneMgr.getRootSceneNode().createChildSceneNode(Ogre.Vector3(-350, 450, -200)) 
        n2.attachObject( pEnt ) 

        pEnt = self.mSceneMgr.createEntity( "3", "knot.mesh" ) 
        self.mSpinny = self.mSceneMgr.getRootSceneNode().createChildSceneNode(Ogre.Vector3(0, 0, 300)) 
        self.mSpinny.attachObject( pEnt ) 
        pEnt.setMaterialName("Examples/MorningCubeMap") 

        self.mSceneMgr.setSkyBox(True, "Examples/MorningSkyBox") 


        plane=Ogre.Plane()
        plane.normal = Ogre.Vector3.UNIT_Y 
        plane.d = 100 
        Ogre.MeshManager.getSingleton().createPlane("Myplane",
            Ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, plane,
            1500, 1500, 10, 10, True, 1, 5, 5, Ogre.Vector3.UNIT_Z) 
        pPlaneEnt = self.mSceneMgr.createEntity( "plane", "Myplane" ) 
        pPlaneEnt.setMaterialName("Examples/Rockwall") 
        pPlaneEnt.setCastShadows(False) 
        self.mSceneMgr.getRootSceneNode().createChildSceneNode().attachObject(pPlaneEnt) 

        self.mCamera.setPosition(-400, 50, 900) 
        self.mCamera.lookAt(0,80,0) 
        print "****b"

        self.connectEventHandlers() 
        ##/ Create a couple of hard coded postfilter effects as an example of how to do it
        ##/ but the preferred method is to use compositor scripts.
        print "****b"

        self.createEffects() 
        print "****b"

##-----------------------------------------------------------------------------------
    def createFrameListener(self):
        self.mFrameListener = CompositorDemo_FrameListener(self) 
        self.mRoot.addFrameListener(self.mFrameListener)

        self.mFrameListener.setSpinningNode(self.mSpinny) 

##--------------------------------------------------------------------------
    def connectEventHandlers(self):
        CEGUI.WindowManager.getSingleton().getWindow("ExitDemoBtn").\
            subscribeEvent(CEGUI.PushButton.EventClicked, self,"handleQuit") 

##-----------------------------------------------------------------------------------
    ##/ Create the hard coded postfilter effects
    def createEffects(self):
        ## Bloom compositor is loaded from script but here is the hard coded equivalent
##      CompositorPtr comp = CompositorManager.getSingleton().create(
##              "Bloom", ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME
##          ) 
##          CompositionTechnique *t = comp.createTechnique() 
##              CompositionTechnique.TextureDefinition *def = t.createTextureDefinition("rt0") 
##              def.width = 128 
##              def.height = 128 
##              def.format = Ogre.PixelFormat.PF_A8R8G8B8 
##              CompositionTechnique.TextureDefinition *def = t.createTextureDefinition("rt1") 
##              def.width = 128 
##              def.height = 128 
##              def.format = Ogre.PixelFormat.PF_A8R8G8B8 
##              CompositionTargetPass_ *tp = t.createTargetPass_() 
##              tp.setInputMode(CompositionTargetPass_.IM_PREVIOUS) 
##              tp.setOutputName("rt1") 
##              CompositionTargetPass_ *tp = t.createTargetPass_() 
##              tp.setInputMode(CompositionTargetPass_.IM_NONE) 
##              tp.setOutputName("rt0") 
##              CompositionPass_ *pass_ = tp.createPass_() 
##              pass_.setType(CompositionPass_.PT_RENDERQUAD) 
##              pass_.setMaterialName("Ogre/Compositor/Blur0") 
##              pass_.setInput(0, "rt1") 
##              CompositionTargetPass_ *tp = t.createTargetPass_() 
##              tp.setInputMode(CompositionTargetPass_.IM_NONE) 
##              tp.setOutputName("rt1") 
##              CompositionPass_ *pass_ = tp.createPass_() 
##              pass_.setType(CompositionPass_.PT_RENDERQUAD) 
##              pass_.setMaterialName("Ogre/Compositor/Blur1") 
##              pass_.setInput(0, "rt0") 
##              CompositionTargetPass_ *tp = t.getOutputTargetPass_() 
##              tp.setInputMode(CompositionTargetPass_.IM_PREVIOUS) 
##              { CompositionPass_ *pass_ = tp.createPass_() 
##              pass_.setType(CompositionPass_.PT_RENDERQUAD) 
##              pass_.setMaterialName("Ogre/Compositor/BloomBlend") 
##              pass_.setInput(0, "rt1") 
        ## Glass compositor is loaded from script but here is the hard coded equivalent
        ##/ Glass effect
##      CompositorPtr comp2 = CompositorManager.getSingleton().create(
##              "Glass", ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME
##          ) 
##          CompositionTechnique *t = comp2.createTechnique() 
##              CompositionTechnique.TextureDefinition *def = t.createTextureDefinition("rt0") 
##              def.width = 0 
##              def.height = 0 
##              def.format = Ogre.PixelFormat.PF_R8G8B8 
##              CompositionTargetPass_ *tp = t.createTargetPass_() 
##              tp.setInputMode(CompositionTargetPass_.IM_PREVIOUS) 
##              tp.setOutputName("rt0") 
##              CompositionTargetPass_ *tp = t.getOutputTargetPass_() 
##              tp.setInputMode(CompositionTargetPass_.IM_NONE) 
##              { CompositionPass_ *pass_ = tp.createPass_() 
##              pass_.setType(CompositionPass_.PT_RENDERQUAD) 
##              pass_.setMaterialName("Ogre/Compositor/Glasspass_") 
##              pass_.setInput(0, "rt0") 
        ##/ Motion blur effect
        comp3 = Ogre.CompositorManager.getSingleton().create(
                "Motion Blur", Ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME
            ) 
        t = comp3.createTechnique() 
        def_ = t.createTextureDefinition("scene") 
        def_.width = 0 
        def_.height = 0 
        def_.format = Ogre.PixelFormat.PF_R8G8B8 
        def_ = t.createTextureDefinition("sum") 
        def_.width = 0 
        def_.height = 0 
        def_.format = Ogre.PixelFormat.PF_R8G8B8 
        def_ = t.createTextureDefinition("temp") 
        def_.width = 0 
        def_.height = 0 
        def_.format = Ogre.PixelFormat.PF_R8G8B8 
        ##/ Render scene
        tp = t.createTargetPass() 
        tp.setInputMode(Ogre.CompositionTargetPass.IM_PREVIOUS) 
        tp.setOutputName("scene") 
        ##/ Initialisation pass_ for sum texture
        tp = t.createTargetPass() 
        tp.setInputMode(Ogre.CompositionTargetPass.IM_PREVIOUS) 
        tp.setOutputName("sum")
        tp.setOnlyInitial(True) 
        ##/ Do the motion blur
        tp = t.createTargetPass() 
        tp.setInputMode(Ogre.CompositionTargetPass.IM_NONE) 
        tp.setOutputName("temp") 
        pass_ = tp.createPass() 
        pass_.setType(Ogre.CompositionPass.PT_RENDERQUAD) 
        pass_.setMaterialName("Ogre/Compositor/Combine") 
        pass_.setInput(0, "scene") 
        pass_.setInput(1, "sum") 
        ##/ Copy back sum texture
        tp = t.createTargetPass() 
        tp.setInputMode(Ogre.CompositionTargetPass.IM_NONE) 
        tp.setOutputName("sum") 
        pass_ = tp.createPass() 
        pass_.setType(Ogre.CompositionPass.PT_RENDERQUAD) 
        pass_.setMaterialName("Ogre/Compositor/Copyback") 
        pass_.setInput(0, "temp") 
        ##/ Display result
        tp = t.getOutputTargetPass() 
        tp.setInputMode(Ogre.CompositionTargetPass.IM_NONE) 
        pass_ = tp.createPass() 
        pass_.setType(Ogre.CompositionPass.PT_RENDERQUAD) 
        pass_.setMaterialName("Ogre/Compositor/MotionBlur") 
        pass_.setInput(0, "sum") 
        ##/ Heat vision effect
        comp4 = Ogre.CompositorManager.getSingleton().create(
                "Heat Vision", Ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME
            ) 
        t = comp4.createTechnique() 
        def_ = t.createTextureDefinition("scene") 
        def_.width = 256 
        def_.height = 256 
        def_.format = Ogre.PixelFormat.PF_R8G8B8 
        def_ = t.createTextureDefinition("temp") 
        def_.width = 256 
        def_.height = 256 
        def_.format = Ogre.PixelFormat.PF_R8G8B8 
        ##/ Render scene
        tp = t.createTargetPass() 
        tp.setInputMode(Ogre.CompositionTargetPass.IM_PREVIOUS) 
        tp.setOutputName("scene") 
        ##/ Light to heat pass_
        tp = t.createTargetPass() 
        tp.setInputMode(Ogre.CompositionTargetPass.IM_NONE) 
        tp.setOutputName("temp") 
        pass_ = tp.createPass() 
        pass_.setType(Ogre.CompositionPass.PT_RENDERQUAD) 
        pass_.setIdentifier(0xDEADBABE)  ##/ Identify pass_ for use in listener
        pass_.setMaterialName("Fury/HeatVision/LightToHeat") 
        pass_.setInput(0, "scene") 
        ##/ Display result
        tp = t.getOutputTargetPass() 
        tp.setInputMode(Ogre.CompositionTargetPass.IM_NONE) 
        pass_ = tp.createPass() 
        pass_.setType(Ogre.CompositionPass.PT_RENDERQUAD) 
        pass_.setMaterialName("Fury/HeatVision/Blur") 
        pass_.setInput(0, "temp") 
##--------------------------------------------------------------------------
    def handleQuit( self, e):
        self.mRoot.queueEndRendering() 
        return True 

    def getCamera(self):
        return self.mCamera
        
    def getSceneManager(self):
        return self.mSceneMgr
        
    def getRenderWindow(self):
        return self.mWindow
        
    def  getMainNode(self) :
        return self.mMainNode
        
	def getGuiRenderer(self):
	    return self.mGUIRenderer        

if __name__ == '__main__':
    import exceptions,sys
    try:
        application = CompositorDemo()
        application.go()
    except Ogre.OgreException, e:
        print e