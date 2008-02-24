##    Cakebox for NxOgre
##    Copyright (C) 2007  Robin "Betajaen" Southern, nxogre.org
##
##    This program is free software: you can redistribute it and/or modify
##    it under the terms of the GNU General Public License as published by
##    the Free Software Foundation, either version 3 of the License, or
##    (at your option) any later version.
##
##    This program is distributed in the hope that it will be useful,
##    but WITHOUT ANY WARRANTY  without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##    GNU General Public License for more details.
##
##    You should have received a copy of the GNU General Public License
##    along with self program.  If not, see <http://www.gnu.org/licenses/>.

        
        
import os        
import ogre.renderer.OGRE as ogre
import ogre.gui.betagui as BetaGUI
import ogre.io.OIS as OIS
import ogre.physics.PhysX as PhysX
import ogre.physics.NxOgre_09 as nxogre

from Enums import *

Action = Enum ('QUIT','SCREENSHOT','DEBUG_MODE','RESET_SCENE', 'PAUSE', 'SLOWER', 'FASTER',
         'OPTION_1','OPTION_2','OPTION_3','OPTION_4',
         'PAN_FORWARD','PAN_BACKWARD','PAN_LEFT','PAN_RIGHT','PAN_UP','PAN_DOWN',
         'X','NEG_X','Y','NEG_Y','Z','NEG_Z','MODIFIER',
         'MOUSE_PRIMARY','MOUSE_ALT','MOUSE_NONE','MOUSE_BOTH', 'MOUSE_LOOK' )
##/////////////////////////////////////////////////////////////////////////////

class Decoration ():
    def __init__(self, rend, inputh ,cake ):
            
        self.InputHandler = inputh
        self.Renderer = rend
        self.Cake = cake
                    
    def decorate(self):
        pass
    def cleanup(self):
        pass
        
    def onFrame(self, deltaTime):
        pass

    def  preStart(self, World):
        pass
        
    def getCamera(self ):
        return self.Camera 
    
    def getSceneManager(self):
        return self.SceneMgr

        
        
        
## From: http://www.ogre3d.org/phpBB2/viewtopic.php?p=183438#183438

class HighlightQueueListener ( ogre.RenderQueueListener ):
    def __init__ ( self ):
        ogre.RenderQueueListener.__init__(self)       

    def renderQueueStarted(self, queueGroupId, invocation, skipThisInvocation):
      ##RenderQueue containing the object to be highlighted
      if (queueGroupId == 90):

         rendersys = ogre.Root.getSingleton().getRenderSystem() 

         rendersys.clearFrameBuffer(ogre.FBT_STENCIL) 
         rendersys.setStencilCheckEnabled(True) 
         rendersys.setStencilBufferParams(ogre.CMPF_ALWAYS_PASS,1,0xFFFFFFFF,
                           ogre.SOP_KEEP,ogre.SOP_KEEP,ogre.SOP_REPLACE,False)       

      ##RenderQueue containing the outline
      if (queueGroupId == 91):

         rendersys = ogre.Root.getSingleton().getRenderSystem() 

         rendersys.setStencilCheckEnabled(True) 
         rendersys.setStencilBufferParams(ogre.CMPF_NOT_EQUAL,1,0xFFFFFFFF,
                  ogre.SOP_KEEP,ogre.SOP_KEEP,ogre.SOP_KEEP,False)       
      return skipThisInvocation


    def renderQueueEnded(self,queueGroupId,  invocation,repeatThisInvocation):
      if (( queueGroupId == 90 )  or  ( queueGroupId == 91 )):

         rendersys = ogre.Root.getSingleton().getRenderSystem() 
         rendersys.setStencilCheckEnabled(False) 
         rendersys.setStencilBufferParams()
      return repeatThisInvocation 

##/////////////////////////////////////////////////////////////////////////////

class Cake ():

    def __init__( self ):
        self.RenderConfiguration = {}
    
        self.RenderConfiguration.clear() 
    
        if os.name == "nt":
            self.RenderConfiguration["device"] = "Direct3D9" 
        else:
            self.RenderConfiguration["device"] = "GL" 
            
        self.RenderConfiguration["width"] = "600" 
        self.RenderConfiguration["height"] = "480" 
        self.RenderConfiguration["depth"] = "16" 
        self.RenderConfiguration["aa"] = "0" 
        self.RenderConfiguration["vsync"] = "Yes" 
        self.RenderConfiguration["fullscreen"] = "No" 
        self.mTargetActor = 0 
        
    def getWorld(self ) :
        return 0 

    def pre(self):
        self.getConfig() 
    
        print "$$$$$$$ PRE 1"   
        self.Renderer = Renderer(self.RenderConfiguration) 
        print "$$$$$$$ PRE 1"   
        self.InputHandler = InputHandler() 
        print "$$$$$$$ PRE 1"   
        window = self.Renderer.createWindow("NxOgre") 
        print "$$$$$$$ PRE 1"   
    
        self.Decoration = Frosting(self.Renderer, self.InputHandler, self) 
        print "$$$$$$$ PRE 1"   
    
        self.SceneMgr = self.Decoration.getSceneManager() 
        self.Camera = self.Decoration.getCamera() 
        print "$$$$$$$ PRE 7"   
        
        self.Renderer.createViewportAndBindToCamera(self.Camera) 
        self.InputHandler.start(window, self.Camera) 
        print "$$$$$$$ PRE 8"   
        
        ogre.TextureManager.getSingleton().setDefaultNumMipmaps(1) 
        ogre.MaterialManager.getSingleton().setDefaultTextureFiltering(ogre.TFO_ANISOTROPIC) 
        ogre.MaterialManager.getSingleton().setDefaultAnisotropy(8)  
        ogre.ResourceGroupManager.getSingleton().initialiseAllResourceGroups() 
        print "$$$$$$$ PRE 9"   
    
        self.Decoration.decorate() 
        print "$$$$$$$ PRE 10"   


    def post(self):
        self.Decoration.cleanup() 
        
        del self.Decoration 
    
        ## Delete scene
        self.Renderer.getWindow().removeAllViewports() 
        self.Renderer.getRoot().destroySceneManager(self.SceneMgr) 
        ## del window

    def __del__(self):
        ## del input handler
        del self.InputHandler 
        ## del renderer
        del self.Renderer 

    def getConfig(self):
    
         cf=ogre.ConfigFile() 
         cf.load("config.yaml",":",True) 
      
         seci = cf.getSectionIterator() 
         while (seci.hasMoreElements()):
            secName = seci.peekNextKey() 
            csettings = seci.getNext() 
            for i in csettings:
                if os.name =='nt':
                    k = i.key.lower()
                    v = i.value.upper() 
                else:
                    k = i.key
                    v = i.value 
                
                self.RenderConfiguration[k] = v 

    def onFramePre(self, deltaTime):
        if (self.InputHandler.isKeyDown(Action.QUIT)) :
            self.Renderer.stop() 
            return 
        
        self.Decoration.onFrame(deltaTime) 
        self.onFrame(deltaTime) 

    def startRendering(self,t):
        self.Decoration.preStart(self.WorldInstance) 
        self.Renderer.bind(t, self.InputHandler) 
        self.Renderer.start() 
    
    def start ( self ):
        pass
    def stop ( self ):
        pass
    def onFrame(self, deltaTime):
        pass
        
##/////////////////////////////////////////////////////////////////////////////


class InputHandler ():
    

    def getMouseState(self):
        return self.Mouse.getMouseState()

    def isKeyDown(self, k):
        return self.Keyboard.isKeyDown(self.Keys[k]) 

    def __init__(self):
        self.Root = None
        self.Camera = None
        self.Keyboard = None
        self.Mouse = None
        self.Keys={}
        self.__resetKeys() 

    def __resetKeys(self):
    
        self.Keys={}
    
        ## #KEYCONFIG
        self.Keys[Action.QUIT] = OIS.KC_ESCAPE 
        self.Keys[Action.SCREENSHOT] = OIS.KC_F1 
        self.Keys[Action.DEBUG_MODE] = OIS.KC_F2 
        self.Keys[Action.PAUSE] = OIS.KC_F3 
        self.Keys[Action.SLOWER] = OIS.KC_F4 
        self.Keys[Action.FASTER] = OIS.KC_F5 
        self.Keys[Action.RESET_SCENE] = OIS.KC_F12 
        self.Keys[Action.OPTION_1] = OIS.KC_1 
        self.Keys[Action.OPTION_2] = OIS.KC_2 
        self.Keys[Action.OPTION_3] = OIS.KC_3 
        self.Keys[Action.OPTION_4] = OIS.KC_4 
        self.Keys[Action.PAN_FORWARD] = OIS.KC_W 
        self.Keys[Action.PAN_BACKWARD] = OIS.KC_S 
        self.Keys[Action.PAN_LEFT] = OIS.KC_A 
        self.Keys[Action.PAN_RIGHT] = OIS.KC_D 
        self.Keys[Action.PAN_UP] = OIS.KC_Q 
        self.Keys[Action.PAN_DOWN] = OIS.KC_Z 
        self.Keys[Action.MODIFIER] = OIS.KC_LSHIFT 
        self.Keys[Action.X] = OIS.KC_J 
        self.Keys[Action.NEG_X] = OIS.KC_L 
        self.Keys[Action.Y] = OIS.KC_U 
        self.Keys[Action.NEG_Y] = OIS.KC_M 
        self.Keys[Action.Z] = OIS.KC_I 
        self.Keys[Action.NEG_Z] = OIS.KC_K 
        self.Keys[Action.MOUSE_LOOK] = OIS.KC_LCONTROL 
    
    def start(self, window, camera ):
        ## create self.Mouse
        windowHnd = window.getCustomAttributeInt("WINDOW")
        self.InputManager = \
        OIS.createPythonInputSystem([("WINDOW",str(windowHnd))])
         
        ##Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
        self.Keyboard = self.InputManager.createInputObjectKeyboard( OIS.OISKeyboard, True )
        self.Mouse = self.InputManager.createInputObjectMouse( OIS.OISMouse, True ) 
    
    
    def stop(self):
        self.InputManager.destroyInputObject(self.Mouse) 
        self.InputManager.destroyInputObject(self.Keyboard) 
        self.InputManager.destroyInputSystem(self.InputManager) 
        
        ##  OIS.InputManager.destroyInputObject(self.Mouse) 
        ##  OIS.InputManager.destroyInputObject(self.Keyboard) 
    
    
    ##/////////////////////////////////////////////////////////////////////////////
    
    def onFrame(self, deltaTime):
        self.Keyboard.capture() 
        self.Mouse.capture() 


##/////////////////////////////////////////////////////////////////////////////
##/////////////////////////////////////////////////////////////////////////////

class Renderer ( ogre.FrameListener ):
    
    def getRoot(self):
        return self.Root
    def getWindow(self):
        return self.Window

    ## Scene manager handled in Cake, but self creates the viewport and window.

    def __init__(self, configuration):
        ogre.FrameListener.__init__ (self)    
        ## Start Renderer
        configuration["device"] = configuration["device"].lower()
    
        if (configuration["device"] == "direct3d9"):
            configuration["device"] = "Direct3D9 Rendering Subsystem" 
    
        else:
            configuration["device"] = "OpenGL Rendering Subsystem" 
    
    
        self.Root = ogre.Root("","","ogre.log") 
    
        cf = ogre.ConfigFile()
        cf.load("resources.cfg") 
    
        seci = cf.getSectionIterator() 
        
        while (seci.hasMoreElements()):
            secName = seci.peekNextKey() 
            settings = seci.getNext() 
            for i in settings:
                typeName = i.key 
                archName = i.value 
                ogre.ResourceGroupManager.getSingleton().addResourceLocation(archName, typeName, secName) 
        ppath=configuration['pluginspath']
        print "\n\nPATH: ", ppath,"\n"
        if os.name == "nt":
            self.Root.loadPlugin(os.path.join(ppath,"RenderSystem_Direct3D9"))
        self.Root.loadPlugin(os.path.join(ppath,"RenderSystem_GL") )
        self.Root.loadPlugin(os.path.join(ppath,"Plugin_ParticleFX")) 
        self.Root.loadPlugin(os.path.join(ppath,"Plugin_OctreeSceneManager") )
        self.Root.loadPlugin(os.path.join(ppath,"Plugin_CgProgramManager") )
        
        pRenderSystemList = self.Root.getAvailableRenderers()  
        for pRenderSystem in pRenderSystemList:
            if pRenderSystem.getName() == configuration["device"]:
                self.RenderSystem = pRenderSystem 
                break 
    
        self.Root.setRenderSystem(self.RenderSystem) 
        self.RenderSystem.setConfigOption("Full Screen", configuration["fullscreen"]) 
        self.RenderSystem.setConfigOption("VSync", configuration["vsync"]) 
        
        if (configuration["device"] == "Direct3D9 Rendering Subsystem"):
            self.RenderSystem.setConfigOption("Video Mode", 
                                   configuration["width"]
                                   + " x " +  configuration["height"]
                                   + " @ " +  configuration["depth"] + "-bit colour"  
                                ) 
    
            self.RenderSystem.setConfigOption("Anti aliasing","Level " + configuration["aa"]) 
    
    
        else :
            self.RenderSystem.setConfigOption("Video Mode", configuration["width"]   + " x " + configuration["height"]) 
            self.RenderSystem.setConfigOption("Colour Depth",configuration["depth"]) 
            self.RenderSystem.setConfigOption("FSAA", "0") 


##/////////////////////////////////////////////////////////////////////////////

    def __del__(self):
        if (self.Root):
            del self.Root 

##/////////////////////////////////////////////////////////////////////////////

    def createWindow( self,name):
        self.Window = self.Root.initialise(True, "Powered by NxOgre and Python-Ogre") 
        return self.Window 


    def createSceneMgr( self, name):
        return self.Root.createSceneManager(ogre.ST_GENERIC, name) 


    def createViewportAndBindToCamera(self, c):
        self.Viewport = self.Window.addViewport(c) 
        c.setAspectRatio(self.Viewport.getActualWidth() / self.Viewport.getActualHeight()) 


    def start(self):
        self.Shutdown = False 
        self.Root.addFrameListener(self) 
        self.Root.startRendering() 


    def stop(self):
        self.Shutdown = True 


    def frameStarted(self, evt):
        self.InputHandler.onFrame(evt.timeSinceLastFrame) 
        self.Cake.onFramePre(evt.timeSinceLastFrame) 
    
        if (self.Shutdown):
            return False 
        else :
            return True 

    def frameEnded( self, evt):
        if (self.Shutdown):
            return False 
        if (self.Window.isClosed()) :
            self.Shutdown = True 
            return True 
        return True 

    def bind(self, t, h):
        self.Cake = t 
        self.InputHandler = h 


##///////////////////////////////////////////////////////////////////////////////////////////
##
##
##
##                          Terrain
##
##
##
##///////////////////////////////////////////////////////////////////////////////////////////

class terrain ( Decoration ):
    def __init__(self, r,  i, t):
        Decoration.__init__(self,r,i,t)
        self.SceneMgr = r.getRoot().createSceneManager("TerrainSceneManager") 
        self.Camera = self.SceneMgr.createCamera("Main") 
        self.Camera.setNearClipDistance(0.1) 
        
    def decorate():
        pass

    def cleanup(self):
        pass

    def onFrame(self, deltaTime):
        pass


##///////////////////////////////////////////////////////////////////////////////////////////
##
##
##
##                          Sandbox
##
##
##
##///////////////////////////////////////////////////////////////////////////////////////////

##/////////////////////////////////////////////////////////////////////////////

class Frosting ( Decoration, BetaGUI.BetaGUIListener ):


    def __init__(self, r, i, t) : 
        BetaGUI.BetaGUIListener.__init__(self)
        Decoration.__init__(self,r,i,t)
        self.SceneMgr = r.createSceneMgr("Main") 
        self.Camera = self.SceneMgr.createCamera("Main") 
        self.Camera.setNearClipDistance(0.1)
        self.RMB=False 
        self.Effects=[]  ## we need to keep effects around
        self.objects=[] ## hold newly created objects
            
        ## Building stuff
        self.BuildMode = Enum ('BM_NONE',
            'BM_MENU',
            'BM_GUI',
            'BM_ACTOR_MENU',
            'BM_ACTOR',
            'BM_JOINT_MENU',
            'BM_JOINT',
            'BM_FX_MENU',
            'BM_FX' )

    def decorate(self):
        self.SceneMgr.setAmbientLight(ogre.ColourValue(0.8,0.8,0.8)) 
        
        if (ogre.Root.getSingletonPtr().getRenderSystem().getCapabilities().hasCapability(ogre.RSC_HWRENDER_TO_TEXTURE)):
            self.SceneMgr.setShadowTextureSettings(1024, 2) 
        else :
            self.SceneMgr.setShadowTextureSettings(512, 2) 

        ##/ Grid
        ##self.SceneMgr.getRootSceneNode().attachObject(self.SceneMgr.createEntity("nx.floor","nx.floor.mesh")) 
    
        self.StaticGeom= self.SceneMgr.createStaticGeometry("Grid") 
        self.StaticGeom.addEntity(self.SceneMgr.createEntity("nx.floor", "nx.floor.mesh"), ogre.Vector3(0,-0.05,0)) 
        self.StaticGeom.addEntity(self.SceneMgr.createEntity("nx.axis", "nx.body.axis.mesh"), ogre.Vector3(0,0.01,0)) 
        self.StaticGeom.build() 
        self.StaticGeom.setCastShadows(False) 

        ## Light
        mSunLight = self.SceneMgr.createLight("SunLight") 
        mSunLight.setType(ogre.Light.LT_SPOTLIGHT) 
        mSunLight.setPosition(1500,1750,1300) 
        mSunLight.setSpotlightRange(ogre.Degree(30), ogre.Degree(50)) 
        dir_ = ogre.Vector3()
        dir_ = -mSunLight.getPosition() 
        dir_.normalise() 
        mSunLight.setDirection(dir_) 
        mSunLight.setDiffuseColour(0.35, 0.35, 0.38) 
        mSunLight.setSpecularColour(0.9, 0.9, 1) 

        ## Shadows

        ## Camera
        self.Camera.setPosition(10,10,10) 
        self.Camera.lookAt(0,0,0) 
    
        if 1:   # shadows...
            self.SceneMgr.setShadowTechnique(ogre.SHADOWTYPE_TEXTURE_ADDITIVE) 
            self.shadowCamera = ogre.LiSPSMShadowCameraSetup()
            self.SceneMgr.setShadowTexturePixelFormat(ogre.PixelFormat.PF_L8) 
            self.SceneMgr.setShadowTextureCasterMaterial(ogre.StringUtil.BLANK) 
            self.SceneMgr.setShadowTextureReceiverMaterial(ogre.StringUtil.BLANK) 
            self.SceneMgr.setShadowTextureSelfShadow(False) 
            self.SceneMgr.setShadowColour(ogre.ColourValue(0.6, 0.6, 0.6)) 
            self.SceneMgr.setShadowCameraSetup(self.shadowCamera) 

        self.Gui = BetaGUI.GUI("SandBoxGUI", "nxogrefont", 14, self.Renderer.getWindow()) 
#         self.Gui = BetaGUI.GUI( self.Renderer.getWindow()) 
        
        self.Cursor = self.Gui.createMousePointer(ogre.Vector2(24, 24), "bgui.pointer") 
#         self.Cursor = self.Gui.createMousePointer("bgui.pointer",24,24) 
        self.Gui.injectMouse(200,200, False) 
        self.Cursor.hide() 
        
        self.WidgetMenu = self.Gui.createWindow(ogre.Vector4(100,100,64,128), "widget.mm", BetaGUI.WindowFeatureType.WFT_NONE, "") 
#         self.WidgetMenu = self.Gui.createWindow(100,100,64,128, "widget.mm", BetaGUI.WindowFeatureType.WFT_NONE, "", None) 
        
        self.callback = BetaGUI.Callback(self)
        self.WidgetMenuButton=[]
        
        
        button = self.WidgetMenu.createButton(ogre.Vector4(7,21,51,19), "widget.mm.bodies","", self.callback ) 
        button.setText("bodies")
        self.WidgetMenuButton.append( button )
        
        button = self.WidgetMenu.createButton(ogre.Vector4(7,51,51,20), "widget.mm.joints","", self.callback ) 
        button.setText("joints")
        self.WidgetMenuButton.append( button )
        
        button = self.WidgetMenu.createButton(ogre.Vector4(7,81,51,19), "widget.mm.fx","", self.callback ) 
        button.setText("fx")
        self.WidgetMenuButton.append( button )
        
        self.WidgetMenu.setFadeOut(True) 
        self.WidgetMenu.hide() 
    
        self.WidgetCreate = self.Gui.createWindow(ogre.Vector4(100,100,128,256), "widget.create", BetaGUI.WindowFeatureType.WFT_NONE, "") 
        self.WidgetCreate.createButton(ogre.Vector4(6,6,56,56), "icon.cube", "cube.1m", self.callback ) 
        self.WidgetCreate.createButton(ogre.Vector4(65,6,56,56), "icon.sphere", "sphere.50cm", self.callback ) 
        self.WidgetCreate.createButton(ogre.Vector4(6,65,56,56), "icon.sphere", "convex1", self.callback ) 
        
        self.WidgetCreate.hide() 

    
# # #   /*StencilOpQueueListener * self.StencilOpFrameListener 
# #         self.StencilOpFrameListener = ogre.StencilOpQueueListener() 
# #         self.SceneMgr.addRenderQueueListener(self.StencilOpFrameListener) */


        self.WidgetEdit = self.Gui.createWindow(ogre.Vector4(100,100,128,32), "widget.edit", BetaGUI.WindowFeatureType.WFT_NONE, "") 
        self.WidgetEdit.createButton(ogre.Vector4(0,0,32,32), "widget.angle","x", self.callback ) 
        self.WidgetEdit.createButton(ogre.Vector4(32,0,32,32), "widget.scale","a", self.callback ) 
        self.WidgetEdit.createButton(ogre.Vector4(64,0,32,32), "widget.material","", self.callback ) 
        self.WidgetEdit.createButton(ogre.Vector4(96,0,32,32), "widget.add", "", self.callback ) 
        self.WidgetEdit.hide() 
        
           
        ## Frame stuff
        self.timeSince = 0.0 
        self.mBuildMode = self.BuildMode.BM_NONE 
        self.SelectedWidgetMenuButton = 0 
        self.BuildRotMode = 0 
        self.BuildScaleMode = 0 
        self.BuildScaleModeAllOnly = False 

        self.Cursor.show() 
    
        self.BuildNode = self.SceneMgr.getRootSceneNode().createChildSceneNode("BuildNode") 
        self.BuildNode.setVisible(False) 
        self.BuildMesh = "" 
        self.RMB = False 
    
        self.GlowNode = self.SceneMgr.getRootSceneNode().createChildSceneNode("GlowNode") 
        self.GlowNode.setVisible(False) 
    
        self.HighLightListener = HighlightQueueListener() 
        self.SceneMgr.addRenderQueueListener(self.HighLightListener) 
        self.BuildMaterials= []
        for x in range (11): self.BuildMaterials.append(0)
        self.CurrentBuildMaterial = 0 
        self.BuildMaterials[0] = "nx.cube" 
        self.BuildMaterials[1] = "nx.sphere" 
        self.BuildMaterials[2] = "nx.flag" 
        self.BuildMaterials[3] = "nx.rope" 
        self.BuildMaterials[4] = "nx.floor" 
        self.BuildMaterials[5] = "nx.70s1" 
        self.BuildMaterials[6] = "nx.lava" 
        self.BuildMaterials[7] = "nx.planet1" 
        self.BuildMaterials[8] = "nx.planet2" 
        self.BuildMaterials[9] = "nx.stone1" 
        self.BuildMaterials[10] = "nx.tartan" 
        if 0:
            self.ReflectCam =  self.SceneMgr.createCamera("ReflectCam") 
            
            self.RTTTex = self.Renderer.getRoot().getRenderSystem().createRenderTexture("RttTex", 
                                                            512,384, ogre.TEX_TYPE_2D, ogre.PixelFormat.PF_R8G8B8 )  
            
            self.ReflectCam =  self.SceneMgr.createCamera("ReflectCam") 
            self.ReflectCam.setPosition(self.Camera.getPosition()) 
            self.ReflectCam.setOrientation(self.Camera.getOrientation()) 
            self.ReflectCam.setNearClipDistance(self.Camera.getNearClipDistance()) 
            self.ReflectCam.setFarClipDistance(self.Camera.getFarClipDistance()) 
            
            v = self.RTTTex.addViewport(self.ReflectCam) 
            self.ReflectCam.setAspectRatio(v.getWidth() / v.getHeight()) 
            v.setOverlaysEnabled(False) 
            v.setClearEveryFrame( True ) 
            v.setBackgroundColour(ogre.ColourValue.Black) 
            
            
            mat = ogre.MaterialManager.getSingleton().create("RttMat", ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME) 
            t = mat.getTechnique(0).getPass(0).createTextureUnitState("RttTex") 
            t.setTextureAddressingMode(ogre.TextureUnitState.TAM_CLAMP) 
            
            mat.clone("RttMat.over") 
            
            self.Gui.createWindow(ogre.Vector4(16,16,512,384), "RttMat", BetaGUI.WFT_NONE, "") 


    def preStart(self, w):
        self.World = w 
        self.Scene = self.World.getScenes()["Main"] 
        self.Ray = nxogre.RayCaster(self.Camera.getPosition(), ogre.Vector3(0,0,0), 100, nxogre.RayCaster.RCT_CLOSEST, self.Scene) 
        self.ClothRay = nxogre.ClothRayCaster(self.Scene) 
        self.HalfRenderWidth = self.Renderer.getWindow().getWidth() / 2 
        self.HalfRenderHeight = self.Renderer.getWindow().getHeight() / 2 
        self.Cake.mTargetActor = 0 
        self.Cake.TargetCloth = 0 


    def cleanup(self):
        ## Grid
        self.SceneMgr.removeRenderQueueListener(self.HighLightListener) 
        del self.HighLightListener 
        self.HighLightListener = 0 
    
        self.WidgetMenuButton=None
        
        ## GUI
        del self.Gui 
        del self.Ray 
        del self.ClothRay 


    def onFrame(self, deltaTime):

        ms = self.InputHandler.getMouseState() 
        
        self.timeSince += deltaTime 
        self.Gui.update(deltaTime) 

#       if (self.Cake.mTargetActor) :
#           self.GlowNode.setPosition(self.Cake.mTargetActor.getGlobalPosition()) 
#           self.GlowNode.setOrientation(self.Cake.mTargetActor.getGlobalOrientation()) 

        if (self.InputHandler.isKeyDown(Action.MODIFIER)):
            self.MoveScale = 10 * deltaTime 
        else:
            self.MoveScale = 30 * deltaTime 
    
        self.TranslateVector = ogre.Vector3.ZERO 
    
        if (self.InputHandler.isKeyDown(Action.PAN_LEFT)):           self.TranslateVector.x = -self.MoveScale
        if (self.InputHandler.isKeyDown(Action.PAN_RIGHT)):      self.TranslateVector.x = self.MoveScale
        if (self.InputHandler.isKeyDown(Action.PAN_FORWARD)):        self.TranslateVector.z = -self.MoveScale
        if (self.InputHandler.isKeyDown(Action.PAN_BACKWARD)):       self.TranslateVector.z = self.MoveScale
        if (self.InputHandler.isKeyDown(Action.PAN_UP)):         self.TranslateVector.y = self.MoveScale
        if (self.InputHandler.isKeyDown(Action.PAN_DOWN)):           self.TranslateVector.y = -self.MoveScale
    
        self.Camera.moveRelative(self.TranslateVector) 
##  self.ReflectCam.setPosition(self.Camera.getPosition()) 


        if (ms.buttonDown(OIS.MB_Right)  and  (ms.X.rel !=0  or  ms.Y.rel != 0)):
            self.Camera.yaw(ogre.Degree(-ms.X.rel * 0.13)) 
            self.Camera.pitch(ogre.Degree(-ms.Y.rel * 0.13)) 
    ##      self.ReflectCam.yaw(ogre.Degree(-ms.X.rel * 0.13)) 
    ##      self.ReflectCam.pitch(ogre.Degree(-ms.Y.rel * 0.13)) 
            self.RMB = True 
            self.Cursor.hide() 
            return 

        if (self.RMB == True  and  ms.buttonDown(OIS.MB_Right)):
            return 
    
        elif (self.RMB == True):
            self.Cursor.show() 
            self.RMB = False 

        if  not (self.InputHandler.isKeyDown(Action.MOUSE_LOOK)  or  ms.buttonDown(OIS.MB_Button3)) :
            self.Cursor.setPosition(self.Cursor.getLeft() + ms.X.rel, self.Cursor.getTop() + ms.Y.rel) 
            if (not self.Cursor.isVisible()):
                self.Cursor.show() 
        else:
            self.Cursor.hide()
             
        flag = False
        if ms.buttonDown(OIS.MB_Left)  and  self.timeSince >= 0.1:
            flag = True
        if  self.Gui.injectMouse(int(self.Cursor.getLeft()), int(self.Cursor.getTop()), flag ):
            if  ms.buttonDown(OIS.MB_Left):
                self.timeSince = 0 
                return 

        ## Mouse has moved or LMB or self.RMB are down
        ##  . Select Target Actor
        ##  . Move Target Actor
        ##  . Start the spawner system
        if ms.X.rel != 0  or  ms.Y.rel != 0  or  ms.buttonDown(OIS.MB_Left)  or  ms.buttonDown(OIS.MB_Right) :
            hp = ogre.Vector3() 
            a = self.cast(hp) 
            
            ## Actor placement (BM_ACTOR)
            ##  . Move it around.
            ##  . Update Widget edit.
            if (self.mBuildMode == self.BuildMode.BM_ACTOR) :
                if (ms.buttonDown(OIS.MB_Left)  and  self.Cake.mTargetActor == 0) :
                        p=ogre.Plane(self.Ray.getClosestRaycastHit().mWorldNormal, 
                                                    self.Ray.getClosestRaycastHit().mWorldImpact) 
                        pos = self.OgreRay.getPoint(self.OgreRay.intersects(p).second) 
                        pos.y += self.BuildNode.getScale().y * 0.5 
                        self.BuildNode.setPosition(pos) 
                        wep = self.worldToScreen(self.BuildNode.getPosition()) 
                        self.WidgetEdit.setPosition(int(wep.x + 48), int(wep.y + 48)) 
                return 



# #if 0:
#       ClothVertex cv = self.ClothRay.getClothVertex() 
#       self.Ray.castShape(RayCaster.AF_NONE) 
#       ogre.Plane p(self.Ray.getClosestRaycastHit().mWorldNormal, self.Ray.getClosestRaycastHit().mWorldImpact) 
#       cv.cloth.addForceAtVertex(self.OgreRay.getPoint(self.OgreRay.intersects(p).second) * 10, cv.vertex) 

#       if (self.Cake.TargetCloth  and  ms.buttonDown(OIS.MB_Left)) :
#           self.OgreRay = self.Camera.getCameraToViewportRay(float(self.Cursor.getLeft() / self.Renderer.getWindow().getWidth()),float(self.Cursor.getTop() / self.Renderer.getWindow().getHeight())) 
#           if (self.ClothRay) :
#               /*ClothVertex cv = self.ClothRay.getClothVertex() 
#               self.Ray.castShape(RayCaster.AF_NONE) 
#               ogre.Plane p(self.Ray.getClosestRaycastHit().mWorldNormal, self.Ray.getClosestRaycastHit().mWorldImpact) 
# */
#               std.cout << "Vertex Pos => " << self.Cake.TargetClothVertex.getGlobalPosition() << std.endl 
# /*
#               self.Cake.TargetCloth.addForceAtVertex(force, cv.vertex) 
#               NxVec3 fc = self.OgreRay.getPoint(self.OgreRay.intersects(p).second) - cv.getGlobalPosition() 
#               fc *= 10 
#               ##fc -= mActor.getPointVelocity(mActor.getGlobalPosition()) 
#               mActor.addForceAtPos(fc, mActor.getGlobalPosition()) */
#           

#           return 

# #endif

        ## There is an Actor and self.RMB is down
        ##  . Make the actor as the target Actor.


            if (a  and  ms.buttonDown(OIS.MB_Right)  and  self.timeSince >= 0.5) :
                self.timeSince = 0 
                if ( not a.isDynamic()) :
                    if (self.Cake.mTargetActor) :
                        if (self.Cake.mTargetActor.hasVisualisation()) :
# # # # #                           b = static_cast<Body*>(self.Cake.mTargetActor)  # AJM ??????????
                            b = self.Cake.mTargetActor
                            self.GlowNode.detachObject(b.getEntity().getName() + ".hilight") 
                            self.GlowNode.setVisible(False) 
                            self.GlowNode.getParentSceneNode().removeChild(self.GlowNode.getName()) 
                            self.SceneMgr.getRootSceneNode().addChild(self.GlowNode) 
                            self.GlowNode.resetToInitialState() 
                            b.getEntity().setRenderQueueGroup(2) 
                    self.Cake.mTargetActor = 0 
                    self.Cake.TargetCloth = 0 
                    return 
                else :
                    if (self.Cake.mTargetActor) :
                        if (self.Cake.mTargetActor.hasVisualisation()) :
# # # # #                           b = static_cast<Body*>(self.Cake.mTargetActor) 
                            b = self.Cake.mTargetActor
                            b.getEntity().setRenderQueueGroup(2) 
                            self.GlowNode.detachObject(b.getEntity().getName() + ".hilight") 
                            self.GlowNode.setVisible(False) 
                            self.GlowNode.getParentSceneNode().removeChild(self.GlowNode.getName()) 
                            self.SceneMgr.getRootSceneNode().addChild(self.GlowNode) 
                            self.GlowNode.resetToInitialState() 
                    self.Cake.mTargetActor = a 
                    self.Cake.TargetCloth = 0 
                    if (self.Cake.mTargetActor.hasVisualisation()) :
# # # # #                       Body* b = static_cast<Body*>(self.Cake.mTargetActor) 
                        b = self.Cake.mTargetActor 
                        if (self.SceneMgr.hasEntity(b.getEntity().getName() + ".hilight")) :
                            b.getEntity().setRenderQueueGroup(90) 
                            self.GlowNode.attachObject(self.SceneMgr.getEntity(b.getEntity().getName() + ".hilight")) 
                            self.GlowNode.resetToInitialState() 
                            self.GlowNode.getParentSceneNode().removeChild(self.GlowNode.getName()) 
    
                            b.getNode().addChild(self.GlowNode) 
                            self.GlowNode.setScale(ogre.Vector3(1.025,1.025,1.025)) 
    
                            self.GlowNode.setVisible(True) 
                        else :
                            e = b.getEntity().clone(b.getEntity().getName() + ".hilight") 
                            b.getEntity().setRenderQueueGroup(90) 
                            self.GlowNode.attachObject(e) 
                            self.GlowNode.resetToInitialState() 
                            self.GlowNode.setVisible(True) 
    
                            self.GlowNode.getParentSceneNode().removeChild(self.GlowNode.getName()) 
                            
                            b.getNode().addChild(self.GlowNode) 
                            self.GlowNode.setScale(ogre.Vector3(1.025,1.025,1.025)) 
                            e.setCastShadows(False) 
                            e.setMaterialName("black-outline")  
                            e.setRenderQueueGroup(91) 
                return 
    
            ## There is an actor, There is a TargetActor and LMB is down.
            ##  . Ignore the actor (it just means something is there). Move the TargetActor towards it.
            if (a  and  self.Cake.mTargetActor  and  ms.buttonDown(OIS.MB_Left)  and  self.Cake.TargetCloth == 0) :
                p = ogre.Plane (self.Ray.getClosestRaycastHit().mWorldNormal, 
                                            self.Ray.getClosestRaycastHit().mWorldImpact) 
                self.Cake.mTargetActor.moveTowards(self.OgreRay.getPoint(self.OgreRay.intersects(p).second), 
                                                self.Cake.mTargetActor.getMass() * 2.0) 
    
# #     #if 0:
# #             
# #     
# #             ## There isn't an Actor and self.RMB is down.
# #             ##  . Check if it's is a cloth, if it is. Move the vertex towards the mouse pointer and break
# #             if (ms.buttonDown(OIS.MB_Right)) :
# #                 if (a) :
# #                         self.timeSince = 0 
# #                         self.OgreRay = self.Camera.getCameraToViewportRay(float(self.Cursor.getLeft() / self.Renderer.getWindow().getWidth()),float(self.Cursor.getTop() / self.Renderer.getWindow().getHeight())) 
# #                         if (self.ClothRay.castAllCloths(self.OgreRay.getOrigin(), self.OgreRay.getDirection())) :
# #     
# #                             /*if (self.Cake.TargetCloth  and  self.Cake.TargetClothVertex.vertex > 1) :
# #                                 self.Cake.TargetCloth.freeVertex(self.Cake.TargetClothVertex.vertex) 
# #     
# #     
# #                             self.Cake.TargetCloth = self.ClothRay.getClothVertex().cloth 
# #                             self.Cake.TargetClothVertex = self.ClothRay.getClothVertex() 
# #                 
# #     
# #                         else :
# #                             self.Cake.TargetCloth = 0 
# #     
# #     
# #     
# #     
# #     
# #             if (ms.buttonDown(OIS.MB_Left)  and  self.Cake.TargetCloth) :
# #                 
# #                 self.Ray.castShape(RayCaster.AF_NONE) 
# #                 ogre.Plane p(self.Ray.getClosestRaycastHit().mWorldNormal, self.Ray.getClosestRaycastHit().mWorldImpact) 
# #                 
# #                 ##self.Cake.TargetCloth.attachVertexToGlobalPosition(self.Cake.TargetClothVertex.vertex, toNxVec3((self.OgreRay.getPoint(self.OgreRay.intersects(p).second)))) 
# #                 ##self.Cake.TargetClothVertex.cloth.addForceAtPos(-toNxVec3((self.OgreRay.getPoint(self.OgreRay.intersects(p).second))), 100, 3) 
# #                 self.Cake.TargetClothVertex.cloth.setExternalAcceleration(toNxVec3((self.OgreRay.getPoint(self.OgreRay.intersects(p).second)))) 
# #     
# #     
# #                 return 
# #     
# #     #endif
            ##elif (self.Cake.TargetCloth  and  self.Cake.TargetClothVertex.vertex>1) :
                ##self.Cake.TargetCloth.freeVertex(self.Cake.TargetClothVertex.vertex) 
    
        if (self.mBuildMode == self.BuildMode.BM_MENU  and  self.WidgetMenu.isVisible() == False) :
            self.mBuildMode = self.BuildMode.BM_NONE 
            self.Cursor.show() 
    
        elif (self.mBuildMode == self.BuildMode.BM_MENU  and  self.timeSince >=0.1) :
            if (ms.Z.rel < 0) :
                if (self.SelectedWidgetMenuButton == 2):
                    self.SelectedWidgetMenuButton = 0 
                else:
                    self.SelectedWidgetMenuButton += 1
    
                v = self.WidgetMenuButton[self.SelectedWidgetMenuButton].getPosition() 
                v.x += self.WidgetMenu.getPosition().x + 25 
                v.y += self.WidgetMenu.getPosition().y + 10 
                self.Cursor.setPosition(v.x,v.y) 
                self.Gui.injectMouse(int(self.Cursor.getLeft()), int(self.Cursor.getTop()),False) 
                self.timeSince = 0 
    
            elif (ms.Z.rel > 0) :
                ## Move up option
                if (self.SelectedWidgetMenuButton == 0):
                    self.SelectedWidgetMenuButton = 2 
                else:
                    self.SelectedWidgetMenuButton-=1
                
                v = self.WidgetMenuButton[self.SelectedWidgetMenuButton].getPosition() 
                v.x += self.WidgetMenu.getPosition().x + 25 
                v.y += self.WidgetMenu.getPosition().y + 10 
                self.Cursor.setPosition(v.x,v.y) 
                self.Gui.injectMouse(int(self.Cursor.getLeft()), int(self.Cursor.getTop()),False) 
                self.timeSince = 0 
    
            self.Cursor.hide() 
            return 
    
        if (ms.Z.rel != 0  and  self.timeSince >= 0.25  and  self.mBuildMode == self.BuildMode.BM_NONE) :
            self.timeSince = 0.0 
            self.WidgetMenu.setPosition(int(self.Cursor.getLeft() - (self.WidgetMenu.getSize().x * 0.5)),
                                            int(self.Cursor.getTop() - (self.WidgetMenu.getSize().y * 0.5))) 
            
            e1 = BetaGUI.AlphaEffect(self.WidgetMenu, 0.5, 0, 1,0)
            self.Gui.addEffect(e1) 
            self.Effects.append( e1 )                                           
            
            self.mBuildMode = self.BuildMode.BM_MENU 
            self.Cursor.hide() 
            v = self.WidgetMenuButton[0].getPosition() 
            v.x += self.WidgetMenu.getPosition().x + 25 
            v.y += self.WidgetMenu.getPosition().y + 10 
            self.Cursor.setPosition(v.x,v.y) 
            self.Gui.injectMouse(int(self.Cursor.getLeft()), int(self.Cursor.getTop()),False) 
    
        if (self.InputHandler.isKeyDown(Action.SCREENSHOT)  and  self.timeSince >=0.25) :
            self.timeSince = 0.0 
            self.RTTTex.writeContentsToTimestampedFile("Screenshot",".jpg") 
    
# #         self.MoveScale = 40 * deltaTime 
# #         self.TranslateVector = ogre.Vector3.ZERO 
# #     
# #         if (self.InputHandler.isKeyDown(PAN_LEFT))          self.TranslateVector.x = -self.MoveScale :
# #         if (self.InputHandler.isKeyDown(PAN_RIGHT))     self.TranslateVector.x = self.MoveScale :
# #         if (self.InputHandler.isKeyDown(PAN_FORWARD))       self.TranslateVector.z = -self.MoveScale :
# #         if (self.InputHandler.isKeyDown(PAN_BACKWARD))      self.TranslateVector.z = self.MoveScale :
# #         if (self.InputHandler.isKeyDown(PAN_UP))            self.TranslateVector.y = self.MoveScale :
# #         if (self.InputHandler.isKeyDown(PAN_DOWN))          self.TranslateVector.y = -self.MoveScale :
# #         
# #         self.Camera.moveRelative(self.TranslateVector) 
# #         self.ReflectCam.setPosition(self.Camera.getPosition()) */

    def onButtonPress(self, b, c):
#         print b.getText()
#         for b in self.WidgetMenuButton:
#             print "   :", b
#         if c != 0:
#             print "Button Press"
#             print self.mBuildMode
#             print b.getPosition().x
#             print b
        ms = self.InputHandler.getMouseState() 
                
        if (b.getText() == self.WidgetMenuButton[0].getText()  and  c == 1) :
            self.Cursor.show() 
            self.mBuildMode = self.BuildMode.BM_ACTOR_MENU 
            e = BetaGUI.AlphaEffect(self.WidgetMenu, 0.5, 1, 0,0)
            self.Gui.addEffect(e) 
            self.Effects.append(e)
            e = BetaGUI.AlphaEffect(self.WidgetCreate, 0.5, 0, 1,0)
            self.Gui.addEffect(e)
            self.Effects.append(e) 
            return 
    
        if (self.mBuildMode == self.BuildMode.BM_ACTOR_MENU  and  c == 1) :
            print "ONE"
            e= BetaGUI.AlphaEffect(self.WidgetCreate, 0.5,1,0,0)
            self.Gui.addEffect(e) 
            self.Effects.append(e)
            
            self.BuildNode.resetToInitialState() 
            self.mBuildMode = self.BuildMode.BM_ACTOR 
            self.BuildMesh = b.getText() + ".mesh" 
    
            ## temp, will use button caption has meshname + ".mesh"
            if (self.SceneMgr.hasEntity(self.BuildMesh)) :
                self.BuildNode.attachObject(self.SceneMgr.getEntity(self.BuildMesh)) 
            else :
                e = self.SceneMgr.createEntity(self.BuildMesh, self.BuildMesh) 
                self.BuildNode.attachObject(e) 
            self.BuildNode.setVisible(True) 
            self.WidgetEdit.show() 
    
        if (b.getPosition().x == 96  and  self.mBuildMode == self.BuildMode.BM_ACTOR  and  c == 1) :
            sz = self.BuildNode.getScale() 
            if (self.BuildMesh == "cube.1m.mesh"):
                tempBody = self.Scene.createBody(self.BuildMesh, nxogre.CubeShape(sz.x,sz.y,sz.z), 
                            nxogre.Pose(self.BuildNode.getPosition(), self.BuildNode.getOrientation()), 
                                "Density: 100, node-scale:" + str(self.BuildNode.getScale())) 
            elif (self.BuildMesh == "sphere.50cm.mesh"):
                tempBody = self.Scene.createBody(self.BuildMesh, nxogre.SphereShape(sz.x * 0.5), 
                            nxogre.Pose(self.BuildNode.getPosition(), self.BuildNode.getOrientation()), 
                                "Density: 100, node-scale:" + str(ogre.Vector3(sz.x,sz.x,sz.x))) 
            elif (self.BuildMesh == "convex1.mesh"):
                tempBody = self.Scene.createBody(self.BuildMesh, 
                        nxogre.ConvexShape("convex1.mesh", "mesh-scale:" + str(sz)), 
                            nxogre.Pose(self.BuildNode.getPosition(), self.BuildNode.getOrientation()), 
                                "Density: 100, node-scale:" + str(ogre.Vector3(sz.x,sz.y,sz.z))) 
            self.BuildNode.setVisible(False) 
            self.BuildNode.detachAllObjects() 
    
            tempBody.getEntity().setMaterialName(self.BuildMaterials[self.CurrentBuildMaterial]) 
    
            e=BetaGUI.AlphaEffect(self.WidgetEdit, 0.25, 1,0,0)
            self.Gui.addEffect(e) 
            self.Effects.append(e)
            self.mBuildMode = self.BuildMode.BM_NONE 
            self.objects.append(tempBody)
            
        ##////////////////////////////////////////////////////////////////////////////////
        if (b.getPosition().x == 0) :
            if (c == 1) :
                self.BuildRotMode += 1 
                if (self.BuildRotMode == 3):
                    self.BuildRotMode = 0 
                if (self.BuildRotMode == 0) :
                    b.setText("x") 
                elif (self.BuildRotMode == 1) :
                    b.setText("y") 
                else :
                    b.setText("z") 
    
            if (self.BuildRotMode == 0) :
                self.BuildNode.roll(ogre.Degree(ms.Z.rel * 0.05), ogre.Node.TS_WORLD) 
            elif (self.BuildRotMode == 1) :
                self.BuildNode.yaw(ogre.Degree(ms.Z.rel * 0.05), ogre.Node.TS_WORLD) 
            else :
                self.BuildNode.pitch(ogre.Degree(ms.Z.rel * 0.05), ogre.Node.TS_WORLD) 
        ##////////////////////////////////////////////////////////////////////////////////
    
        if (b.getPosition().x == 32) :
            if (c == 1  and  self.BuildScaleModeAllOnly == False) :
                self.BuildScaleMode += 1 
                if (self.BuildScaleMode == 4):
                    self.BuildScaleMode = 0 
                if (self.BuildScaleMode == 0) :
                    b.setText("a") 
                elif (self.BuildScaleMode == 1) :
                    b.setText("x") 
                elif (self.BuildScaleMode == 2) :
                    b.setText("y") 
                else :
                    b.setText("z") 
    
            sz = self.BuildNode.getScale().y 
            pos = self.BuildNode.getPosition() 
            pos.y -= sz * 0.5 
            
            sz = ms.Z.rel * 0.001 
            newsz = self.BuildNode.getScale() 
    
            if (newsz.x <= 0  or  newsz.y <= 0  or  newsz.z <= 0):
                return 
            
            hc = False 
            if (self.BuildScaleMode == 0) :
                newsz += ogre.Vector3(sz,sz,sz) 
                hc = True 
            elif (self.BuildScaleMode == 1) :
                newsz += ogre.Vector3(sz,0,0) 
            elif (self.BuildScaleMode == 2) :
                newsz += ogre.Vector3(0,sz,0) 
                hc = True 
            elif (self.BuildScaleMode == 3) :
                newsz += ogre.Vector3(0,0,sz) 
    
            self.BuildNode.setScale(newsz) 
    
            newsz = self.BuildNode.getScale() 
            if (newsz.x < 0) :
                newsz.x = 0.01 
                self.BuildNode.setScale(newsz) 
    
            if (newsz.y < 0) :
                newsz.y = 0.01 
                self.BuildNode.setScale(newsz) 
    
            if (newsz.z < 0) :
                newsz.z = 0.01 
                self.BuildNode.setScale(newsz) 
    
            if (hc == True) :
                    pos = self.BuildNode.getPosition() 
                    pos.y += sz * 0.5 
                    self.BuildNode.setPosition(pos) 
    
        ##////////////////////////////////////////////////////////////////////////////////
        if (b.getPosition().x == 64  and  ms.Z.rel != 0) :
            
            if (ms.Z.rel > 0) :
                self.CurrentBuildMaterial += 1 
        
            if (ms.Z.rel < 0):
                self.CurrentBuildMaterial -= 1 
    
            if (self.CurrentBuildMaterial == -1):
                self.CurrentBuildMaterial = len(self.BuildMaterials) - 1 
    
            if self.CurrentBuildMaterial == len(self.BuildMaterials):
                self.CurrentBuildMaterial = 0 
    
# # # # #           e = static_cast<ogre.Entity*>(self.BuildNode.getAttachedObject(0)) 
            e = self.BuildNode.getAttachedObject(0)
            e.setMaterialName(self.BuildMaterials[self.CurrentBuildMaterial]) 
    
    ##////////////////////////////////////////////////////////////////////////////////



    def cast(self, hitpos):
        
        self.OgreRay = self.Camera.getCameraToViewportRay(self.Cursor.getLeft() / self.Renderer.getWindow().getWidth(),
                                    self.Cursor.getTop() / self.Renderer.getWindow().getHeight() ) 
        
        self.Ray.setOrigin(self.OgreRay.getOrigin()) 
        self.Ray.setDirection(self.OgreRay.getDirection()) 
        
        if (self.Ray.castShape(nxogre.RayCaster.AF_NONE)) :
            hitpos = self.Ray.getClosestRaycastHit().mWorldImpact 
            return self.Ray.getClosestActor() 
        else :
            return 0 


    def worldToScreen( self, worldPos):

        hcsPosition = self.Camera.getProjectionMatrix() * (self.Camera.getViewMatrix() * worldPos) 
        p=ogre.Vector2() 
      
        if (hcsPosition.x < -1.0):
            p.x = 0 
        elif (hcsPosition.x > 1.0):
            p.x = (self.HalfRenderWidth*2)+1 
        else:
            p.x = self.HalfRenderWidth + (self.HalfRenderWidth * hcsPosition.x) 
    
        if (hcsPosition.y < -1.0):
            p.y = 0 
        elif (hcsPosition.y > 1.0):
            p.y = (self.HalfRenderHeight*2)+1 
        else:
            p.y = self.HalfRenderHeight + (self.HalfRenderHeight * -hcsPosition.y) 
    
        return p 

##/////////////////////////////////////////////////////////////////////////////

class tester ( Decoration ):
    def __init__ (self, r,  i, t) : 
        Decoration.__init__(self,r,i,t)
        self.SceneMgr = r.createSceneMgr("Main") 
        self.Camera = self.SceneMgr.createCamera("Main") 
        self.Camera.setNearClipDistance(0.1) 


##/////////////////////////////////////////////////////////////////////////////

class fps ( Decoration ):
    def __init__ (self, r, i, t) : 
        Decoration.__init__(self,r,i,t)
        self.SceneMgr = r.createSceneMgr("Main") 
        self.Camera = self.SceneMgr.createCamera("Main") 
        self.Camera.setNearClipDistance(0.1) 

##/////////////////////////////////////////////////////////////////////////////

class Sponge_Cake ( Cake ):

    def __init__ ( self ):
        Cake.__init__(self)
    
    def start(self):

        self.World = nxogre.World(nxogre.PhysXDriverParams("log: html"))
        self.Scene = self.World.createScene("Main", self.SceneMgr, nxogre.SceneParams("gravity: yes, floor: yes, time-step-method: variable"))

        self.Scene.createBody("cube.1m.mesh", nxogre.CubeShape(1), ogre.Vector3(0,5,0), nxogre.ActorParams("mass: 10"))

    def stop(self):
        del self.World

    def onFrame(self, deltaTime):
        
#       // Want a key pressed?
#       //
#       //  if (mInputHandler->isKeyDown(Action::OPTION_1)) {
#       //      ...
#       //  }
#       //
#       // Search the solution for "#KEYCONFIG" to find the key configuration.

        pass
        
# #     //////////////////////////////////////////////////////////////////////////////////////////////////
    def getWorld(self):
        return self.World

if __name__ == '__main__':
    import exceptions,sys
    try:
        application = Sponge_Cake ()
        print "**** ", application, " CREATED OK"
        application.pre()
        print "**** ", application, " PRE OK"
        application.start()
        print "**** ", application, "STARTED"
        application.WorldInstance = application.getWorld()
        print "**** ", application
        application.startRendering(application)
        print "**** ", application
        application.stop()
        print "**** ", application
        application.post()

    except ogre.OgreException, e:
        print e
