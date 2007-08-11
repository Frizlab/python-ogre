import ogre.renderer.OGRE as ogre
import ogre.io.OIS as ois
import ogre.physics.PhysX as physx
import ogre.physics.NxOgre as nxogre
import SampleFramework as sf
import BetaGui as BetaGUI
from Enums import *

Key = Enum ('QUIT','SCREENSHOT','DEBUG_MODE','RESET_SCENE', 'PAUSE', 'SLOWER', 'FASTER',
         'OPTION_1','OPTION_2','OPTION_3','OPTION_4',
         'PAN_FORWARD','PAN_BACKWARD','PAN_LEFT','PAN_RIGHT','PAN_UP','PAN_DOWN',
         'X','NEG_X','Y','NEG_Y','Z','NEG_Z',
         'MOUSE_PRIMARY','MOUSE_ALT','MOUSE_NONE','MOUSE_BOTH' )
         
MouseMode = Enum ('CAMERA_FOLLOW','CAMERA_FORCE','CAMERA_CONTROL','PICK')
         
class SimpleTutorial ( ogre.FrameListener, ois.KeyListener, BetaGUI.BetaGUIListener):


      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


      def __init__(self):
         ogre.FrameListener.__init__(self)
         ois.KeyListener.__init__(self)
         BetaGUI.BetaGUIListener.__init__(self)
         
         self.TutorialName = "???" 
         self.TutorialDescription = "???" 
         self.ShutdownNow = False 
         self.MouseMode = MouseMode.PICK 
         self.Construct = False 
         self.settings={}
         self.TranslateVector = ogre.Vector3.ZERO
      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def __del__(self):
         pass 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def Run(self):

            self.getSetup() 

            self.resetConfig() 
            self.getConfig() 

            self.setupRenderSystem() 
               
            cf =ogre.ConfigFile()
            cf.load("media/resources.cfg") 

            seci = cf.getSectionIterator() 
            while seci.hasMoreElements():
               SectionName = seci.peekNextKey()
               Section = seci.getNext()
               for item in Section:
                  ogre.ResourceGroupManager.getSingleton().\
                     addResourceLocation(item.value, item.key, SectionName)
                  
            self.Window = self.Root.initialise(True, "NxOgre: " + self.TutorialName + " - " + self.TutorialDescription) 

            self.SceneMgr = self.Root.createSceneManager(ogre.ST_GENERIC, "NxOgre")
            print "\nSM: ",self.SceneMgr ,"\n\n"
            self.Camera = self.SceneMgr.createCamera("MainCamera") 
            self.Camera.setPosition(ogre.Vector3(0,0,0)) 
            self.Camera.lookAt(ogre.Vector3(0,0,0)) 
            self.Camera.setNearClipDistance(0.1) 

            self.Viewport = self.Window.addViewport(self.Camera) 

            if not self.Construct:
               self.Viewport.setBackgroundColour(ogre.ColourValue.Black) 
            else:
               self.Viewport.setBackgroundColour(ogre.ColourValue.White) 
            self.Camera.setAspectRatio(self.Viewport.getActualWidth() / self.Viewport.getActualHeight()) 

            ogre.TextureManager.getSingleton().setDefaultNumMipmaps(1) 
            ogre.MaterialManager.getSingleton().setDefaultTextureFiltering(ogre.TFO_ANISOTROPIC) 
            ogre.MaterialManager.getSingleton().setDefaultAnisotropy(8)  
            ogre.ResourceGroupManager.getSingleton().initialiseAllResourceGroups() 

            self.Overlay = ogre.OverlayManager.getSingleton().create("GUI") 
            self.Overlay.setZOrder(549) 

            loadingBackMat = "boot.any" 

            if  not ogre.MaterialManager.getSingleton().getByName("boot." + self.TutorialName):
               loadingBackMat = "boot." + self.TutorialName 
            
            loadingBack = self.createOverlay("loadBack",ogre.Vector2(0,0),
                           ogre.Vector2(self.Viewport.getActualWidth(),
                           self.Viewport.getActualHeight()),loadingBackMat)      
            loading = self.createOverlay("load",
                  ogre.Vector2((self.Window.getWidth() / 2) - 64,(self.Window.getHeight() / 2) - 16),
                  ogre.Vector2(128,32),"gui_loading") 

            self.Overlay.show() 

            self.Root.renderOneFrame() 
            
            loading.hide() 
            loadingBack.hide() 
            
            self.createInputSys(self.Window) 
                        
            self.GUI = BetaGUI.GUI("nxogrefont",16) 
            
            self.GUI_ExitWindow = self.GUI.createWindow(ogre.Vector4(self.Window.getWidth() - 128- 4,4,128,22),\
                              "", BetaGUI.wt.NONE) 
            self.GUI_ExitButton =  self.GUI_ExitWindow.createButton(ogre.Vector4(128 -  22,0,22,22),
                               "bgui.exit", "", BetaGUI.Callback(self)) 
            self.GUI_PauseButton = self.GUI_ExitWindow.createButton(ogre.Vector4(128 - (22 * 2) - 4,0,22,22), 
                              "bgui.pause", "", BetaGUI.Callback(self)) 
            self.GUI_DebugButton = self.GUI_ExitWindow.createButton(ogre.Vector4(128 - (22 * 3) - 4,0,22,22), 
                              "bgui.debug", "", BetaGUI.Callback(self)) 

            ##self.Pointer = createOverlay("Pointer", ogre.Vector2((self.Window.getWidth() / 2) - 16, (self.Window.getHeight() / 2) - 16), ogre.Vector2(32,32), "nx.arrow") 
            ##self.Pointer.hide()      
            self.Pointer = self.GUI.createMousePointer(ogre.Vector2(24,24), "bgui.pointer") 
            self.GUI.injectMouse((self.Window.getWidth() / 2) - 16, (self.Window.getHeight() / 2) - 16, False) 
            self.Pointer.hide() 

            self.gui_pause = self.createOverlay("paused", ogre.Vector2(16, self.Window.getHeight() - 100 - 32), 
                        ogre.Vector2(128,128), "gui_pause") 
            self.gui_pause.hide() 

            self.gui_slowfast = self.createOverlay("slowfast", ogre.Vector2(16 + 100, self.Window.getHeight() - 100 - 32), 
                        ogre.Vector2(128,128), "", "0") 
            self.gui_slowfast.hide() 
            
            self.Camera.lookAt(0,0,0) 
            
            self.Caption1 = self.createOverlay("caption.1",ogre.Vector2(8,128),ogre.Vector2(16,256),""," ") 
            self.Caption2 = self.createOverlay("caption.2",ogre.Vector2(8,128 + 16),ogre.Vector2(16,256),""," ") 
            self.Caption3 = self.createOverlay("caption.3",ogre.Vector2(8,self.Window.getHeight() - 16 - 8),
                        ogre.Vector2(16,256),""," ") 
            
            
            self.ApplicationStart() 
            
            if not self.Construct :
               self.sg = self.SceneMgr.createStaticGeometry("grid") 
               self.CreateEntNode("nx.floor2", ogre.Vector3(0,-0.05,0)) 
               self.CreateEntNode("nx.body.axis", ogre.Vector3(0,0,0)) 
               self.sg.build() 


            if not self.Construct :
               self.SceneMgr.setFog(ogre.FOG_LINEAR, ogre.ColourValue.Black, 0.0005, 4000,4500) 
               self.SceneMgr.setAmbientLight(ogre.ColourValue(0.6,0.6,0.6)) 

            else :
               self.SceneMgr.setFog(ogre.FOG_LINEAR, ogre.ColourValue.White, 0.0005, 4000,4500) 
               self.SceneMgr.setAmbientLight(ogre.ColourValue(0.9,0.9,0.9)) 

            
            self.SceneMgr.setShadowTechnique( ogre.SHADOWTYPE_STENCIL_ADDITIVE ) 

            mLight = self.SceneMgr.createLight("SunLight") 
            mLight.setPosition( ogre.Vector3(150, 100, -150) ) 

            if (self.MouseMode == MouseMode.PICK):
               self.Pointer.show() 

            self.Root.addFrameListener(self) 
            
            self.Root.startRendering() 

            return True 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def setupRenderSystem(self):
         self.Root = ogre.Root("","","ogre.graphics.log") 
         self.Root.loadPlugin("RenderSystem_Direct3D9") 
         self.Root.loadPlugin("RenderSystem_GL") 
         self.Root.loadPlugin("Plugin_ParticleFX") 
         
         rs = "Direct3D9 Rendering Subsystem" 

         pRenderSystemList = self.Root.getAvailableRenderers()  
         
         for pRenderSystem in pRenderSystemList:
            if pRenderSystem.getName() == rs:
               self.RenderSystem = pRenderSystem 
               break 
            pRenderSystem+=1 

         self.Root.setRenderSystem(self.RenderSystem) 

         self.RenderSystem.setConfigOption("Full Screen", self.settings["fullscreen"]) 
         self.RenderSystem.setConfigOption("VSync", self.settings["vsync"]) 

         if rs == "Direct3D9 Rendering Subsystem" :
            self.RenderSystem.setConfigOption("Video Mode", 
                              self.settings["width"]
                              + " x " +  self.settings["height"]
                              + " @ " +  self.settings["depth"] + "-bit colour"  
            ) 
            
         else :
            self.RenderSystem.setConfigOption("Video Mode", self.settings["width"]   + " x " + self.settings["height"])

            self.RenderSystem.setConfigOption("Colour Depth",self.settings["depth"]) 
            ##self.RenderSystem.setConfigOption("FSAA", "2") 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def resetConfig(self):

         self.settings={}
         self.settings["device"] = "Direct3D9 Rendering Subsystem" 
         self.settings["width"] = "800" 
         self.settings["height"] = "600" 
         self.settings["depth"] = "32" 
         self.settings["aa"] = "0" 
         self.settings["vsync"] = "No" 
         self.settings["fullscreen"] = "No" 

         self.keys={} 
         self.keys[Key.QUIT] = ois.KC_ESCAPE 
         self.keys[Key.SCREENSHOT] = ois.KC_F1 
         self.keys[Key.DEBUG_MODE] = ois.KC_F2 
         self.keys[Key.PAUSE] = ois.KC_F3 
         self.keys[Key.SLOWER] = ois.KC_F4 
         self.keys[Key.FASTER] = ois.KC_F5 
         self.keys[Key.RESET_SCENE] = ois.KC_F12 
         self.keys[Key.OPTION_1] = ois.KC_1 
         self.keys[Key.OPTION_2] = ois.KC_2 
         self.keys[Key.OPTION_3] = ois.KC_3 
         self.keys[Key.OPTION_4] = ois.KC_4 
         self.keys[Key.PAN_FORWARD] = ois.KC_W 
         self.keys[Key.PAN_BACKWARD] = ois.KC_S 
         self.keys[Key.PAN_LEFT] = ois.KC_A 
         self.keys[Key.PAN_RIGHT] = ois.KC_D 
         self.keys[Key.PAN_UP] = ois.KC_Q 
         self.keys[Key.PAN_DOWN] = ois.KC_Z 
         self.keys[Key.X] = ois.KC_J 
         self.keys[Key.NEG_X] = ois.KC_L 
         self.keys[Key.Y] = ois.KC_U 
         self.keys[Key.NEG_Y] = ois.KC_M 
         self.keys[Key.Z] = ois.KC_I 
         self.keys[Key.NEG_Z] = ois.KC_K 


      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def getConfig(self):

         cf=ogre.ConfigFile() 
         cf.load("config.yaml",":",True) 
      
         seci = cf.getSectionIterator() 
         while (seci.hasMoreElements()):
            secName = seci.peekNextKey() 
            csettings = seci.getNext() 
            for i in csettings:
               if (i.key == "device") :
                  if (i.value == "dx9") :
                     self.settings["device"] = "Direct3D9 Rendering Subsystem" 
                  else :
                     self.settings["device"] = "OpenGL Rendering Subsystem"  ## Check..
               elif (i.key == "width") :
                  self.settings["width"] = i.value # str(i.value) ## AJM may have to parse int???

               elif (i.key == "height") :
                  self.settings["height"] = i.value #str(i.value) 

               elif (i.key == "depth") :
                  self.settings["depth"] = i.value #str(i.value) 

               elif (i.key == "aa") :
                  self.settings["aa"] = i.value #str(i.value) 

               elif (i.key == "fullscreen") :
                  self.settings["fullscreen"] = i.value 

               elif (i.key == "vsync") :
                  self.settings["aa"] = i.value 

      
      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def createInputSys(self, _window):
         ogre.LogManager.getSingletonPtr().logMessage("*** Initializing OIS ***") 
         
         windowHnd = self.Window.getCustomAttributeInt("WINDOW")
         self.InputManager = \
         ois.createPythonInputSystem([("WINDOW",str(windowHnd))])

         self.Keyboard = self.InputManager.createInputObjectKeyboard( ois.OISKeyboard, True )
         self.Mouse = self.InputManager.createInputObjectMouse( ois.OISMouse, True ) 
         self.Keyboard.setEventCallback(self) 
         self.Keyboard.setBuffered(True) 


      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def onButtonPress(self, ref):
         
         if ref == self.GUI_ExitButton:
            self.ShutdownNow = True 

         if ref == self.GUI_PauseButton  and  self.timeSince >= 0.5:
            if self.World.isPaused():
               nxogre.self.World.resume() 
               self.GUI_PauseButton.mmn = "bgui.pause" 
               self.GUI_PauseButton.mma = "bgui.pause.active" 

            else :
               self.World.pause()            
               self.GUI_PauseButton.mmn = "bgui.play" 
               self.GUI_PauseButton.mma = "bgui.play.active" 

            self.timeSince = 0.0 



         if ref == self.GUI_DebugButton  and  self.timeSince >= 0.25 :
            self.World.showDebugOverlay(self.Camera, self.Window) 
            self.World.debug( self.World.isDebugRendering() ^ 1) 

            if (self.World.isDebugRendering()) :
               self.GUI_DebugButton.mmn = "bgui.debugon" 
               self.GUI_DebugButton.mma = "bgui.debugon.active" 

            else :
               self.GUI_DebugButton.mmn = "bgui.debug" 
               self.GUI_DebugButton.mma = "bgui.debug.active" 

            self.timeSince = 0.0 

         self.GUIbuttonPressed(ref) 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def captureInput(self):
         self.Keyboard.capture() 
         self.Mouse.capture() 


      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def isKeyDown(self, k):
         return self.Keyboard.isKeyDown(self.keys[k]) 


      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def keyPressed( self, arg ):
         if (arg.text == 8) :
            self.GUI.injectBackspace(self.Pointer.getLeft(), self.Pointer.getTop()) 
            return True 

         if (arg.text < 32 or arg.text > 126):
            return True 

         k = chr(arg.text)
         self.GUI.injectKey(k, self.Pointer.getLeft(), self.Pointer.getTop()) 
         return True 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def keyReleased( self, arg ):
         ##self.GUI.injectKey(self.Keyboard.getAsString((OIS.KeyCode) arg.text), self.Pointer.getLeft(), self.Pointer.getTop()) 
         return True 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def getRMouseX(self):
         ms = self.Mouse.getMouseState() 
         return ms.X.rel 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def getRMouseY(self):
         ms = self.Mouse.getMouseState() 
         return ms.Y.rel 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def getRMouseZ(self):
         ms = self.Mouse.getMouseState() 
         return ms.Z.rel 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def getMouseButton(self):

         ms = self.Mouse.getMouseState() 
         
         if ms.buttons == 1:
            return Key.MOUSE_PRIMARY 
         elif ms.buttons == 2:
            return Key.MOUSE_ALT 
         elif ms.buttons == 3:
            return Key.MOUSE_BOTH 
         else:
            return Key.MOUSE_NONE 
   
      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def createOverlay(self, name, position, dimensions, material = "", caption = "", autoAdd = True) :
         type_ = "Panel" 
         if (caption != "") :
            type_ = "TextArea" 

         e = ogre.OverlayManager.getSingleton().createOverlayElement(type_, name, False)
         e.setMetricsMode(ogre.GMM_PIXELS) 
         e.setLeft(position.x) 
         e.setTop(position.y) 
         e.setWidth(dimensions.x) 
         e.setHeight(dimensions.y) 

         if material != "":
            e.setMaterialName(material) 
         
         if caption != "" :
            e.setCaption(caption) 
            e.setParameter("font_name", "nxogrefont") 
            e.setParameter("char_height", "16") 
            e.setParameter("horz_align", "left") 

         if autoAdd:
            self.Overlay.add2D(ogre.castAsOverlayContainer(e) ) 
            e.show() 
         return e 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def createPlane(self, _name,  _material,  _pos,  _size,  _subdivisions = ogre.Vector2(1,1)):
         _plane =ogre.Plane()
         _plane.normal = ogre.Vector3.UNIT_Y 
         _plane.d = 0 

         ogre.MeshManager.getSingleton().createPlane(_name + ".plane",
            ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, 
            _plane,_size.x,_size.y,1,1,True,1,_subdivisions.x,_subdivisions.y,ogre.Vector3.UNIT_Z) 
         
         _entity = self.SceneMgr.createEntity(_name + ".entity", _name + ".plane") 
         _entity.setMaterialName(_material) 
         _entity.setCastShadows(False) 
         
         self.sg.addEntity(_entity, _pos) 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def CreateEntNode(self, _name,  _pos) :
         _entity = self.SceneMgr.createEntity(_name + ".entity", _name + ".mesh") 
         _entity.setCastShadows(False) 
         self.sg.addEntity(_entity, _pos) 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


      def CreateNodeEntity(self, _name, ent,  _pos):
         n = self.SceneMgr.getRootSceneNode().createChildSceneNode(_name) 
         e = self.SceneMgr.createEntity(_name + ".ent", ent) 
         n.attachObject(e) 
         n.setPosition(_pos) 
         return n 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def Shutdown(self):
         del self.GUI 
         self.ApplicationStop() 
         self.Root.shutdown() 
         del self.Root 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def frameStarted( self, evt):
         
         if self.Window.isClosed():
            return False 

         self.captureInput() 

         if self.isKeyDown(Key.QUIT):
            self.ShutdownNow = True 
      
         if self.MouseMode == MouseMode.CAMERA_CONTROL:
            self.RotX = ogre.Degree(-getRMouseX() * 0.13) 
            self.RotY = ogre.Degree(-getRMouseY() * 0.13) 
            self.Camera.yaw(self.RotX) 
            self.Camera.pitch(self.RotY) 

         elif self.MouseMode == MouseMode.CAMERA_FORCE:
            self.RotX = ogre.Degree(-getRMouseX() * 0.13) 
            self.RotY = ogre.Degree(-getRMouseY() * 0.13) 
            self.Camera.yaw(self.RotX) 
            self.Camera.pitch(self.RotY) 

         elif self.MouseMode == MouseMode.PICK and self.getMouseButton() == Key.MOUSE_PRIMARY:
            self.Pointer.setPosition(
               self.Pointer.getLeft() + (getRMouseX()),
               self.Pointer.getTop() + (getRMouseY())
            ) 

         if not self.GUI.injectMouse(self.Pointer.getLeft(), self.Pointer.getTop(), True):
            self.mouseMoveOrGrab(self.Pointer.getLeft(), self.Pointer.getTop(), True) 

         elif self.MouseMode == MouseMode.PICK and self.getMouseButton() == Key.MOUSE_ALT:
            self.Pointer.setPosition(
               self.Pointer.getLeft() + (getRMouseX()),
               self.Pointer.getTop() + (getRMouseY())
            ) 

            mouseMoveOrGrab(self.Pointer.getLeft(), self.Pointer.getTop(), False) 


         elif self.MouseMode == MouseMode.PICK and self.getMouseButton() == Key.MOUSE_BOTH:

            self.RotX = ogre.Degree(-getRMouseX() * 0.13) 
            self.RotY = ogre.Degree(-getRMouseY() * 0.13) 
            self.Camera.yaw(self.RotX) 
            self.Camera.pitch(self.RotY) 

         elif self.MouseMode == MouseMode.PICK:
            self.Pointer.setPosition(
               self.Pointer.getLeft() + (getRMouseX()),
               self.Pointer.getTop() + (getRMouseY())
            ) 
            self.GUI.injectMouse(self.Pointer.getLeft(), self.Pointer.getTop(), False) 


         if self.MouseMode == MouseMode.PICK or self.MouseMode == MouseMode.CAMERA_CONTROL:

            if self.isKeyDown(Key.PAN_LEFT):
               self.TranslateVector.x = -self.MoveScale 

            if self.isKeyDown(Key.PAN_RIGHT):
               self.TranslateVector.x = self.MoveScale 

            if self.isKeyDown(Key.PAN_FORWARD):
               self.TranslateVector.z = -self.MoveScale 

            if self.isKeyDown(Key.PAN_BACKWARD):
               self.TranslateVector.z = self.MoveScale 

            if self.isKeyDown(Key.PAN_UP):
               self.TranslateVector.y = self.MoveScale 

            if self.isKeyDown(Key.PAN_DOWN):
               self.TranslateVector.y = -self.MoveScale 

            self.Camera.moveRelative(self.TranslateVector) 

      
         self.MoveScale = 16 * evt.timeSinceLastFrame 
         
         self.RotX = 0 
         self.RotY = 0 
         self.TranslateVector = ogre.Vector3.ZERO 

         ## Let the tutorial have some fun.
         self.timeSince += evt.timeSinceLastFrame 
         
         self.newFrame(evt.timeSinceLastFrame) 
         
         ##
         ##     Pausing NxOgre
         ##

         if self.isKeyDown(Key.DEBUG_MODE)  and  self.timeSince >= 0.25:

            ## Technically self only needs to be done once, and it should be in your setup code.
            print dir(self.World)
#             self.World.showDebugOverlay(self.Camera, self.Window) 
            
            ## Pause or unpause the DebugRenderer
            self.World.debug( self.World.isDebugRendering() ^ 1) 

            if (self.World.isDebugRendering()) :
               self.GUI_DebugButton.mmn = "bgui.debugon" 
               self.GUI_DebugButton.mma = "bgui.debugon.active" 

            else :
               self.GUI_DebugButton.mmn = "bgui.debug" 
               self.GUI_DebugButton.mma = "bgui.debug.active" 

            self.timeSince = 0.0 

         if self.isKeyDown(Key.SCREENSHOT)  and  self.timeSince >= 0.25:
            self.Window.writeContentsToFile("screenshot." + str(ogre.Math.Floor(self.Root.getTimer().getMilliseconds() / 1000)) + ".png") 
            self.timeSince = 0.0 

         if self.isKeyDown(Key.FASTER)  and  self.timeSince >= 0.25:
            self.speed *= 2 
            self.World.setTimeModifer(self.speed) 

            self.timeSince = 0.0 

            if (self.speed > 4):
               self.speed = 4 
            
            if (self.speed != 0) :
               self.gui_slowfast.show() 
               self.gui_slowfast.setCaption(str(self.speed)) 
            else:
               self.gui_slowfast.hide() 

         if self.isKeyDown(Key.SLOWER)  and  self.timeSince >= 0.25:
            self.speed /= 2 
            self.World.setTimeModifer(self.speed) 
            self.timeSince = 0.0 

            if (self.speed < 0.25):
               self.speed = 0.25 

            if (self.speed != 0) :
               self.gui_slowfast.show() 
               self.gui_slowfast.setCaption(str(self.speed)) 
            else:
               self.gui_slowfast.hide() 

         if self.isKeyDown(Key.PAUSE)  and  self.timeSince >= 0.5:

            if (nxogre.self.World.isPaused()) :
               self.World.resume() 
               self.gui_pause.hide() 
            else :
               self.World.pause() 
               self.gui_pause.show() 

            self.timeSince = 0.0 

         if self.firstFrame:
            if self.MouseMode == MouseMode.PICK:
               self.Camera.setPosition(10,10,10)  
               self.Camera.lookAt(0,1,0) 
            self.firstFrame = False 

         if (self.hasTargetBody == False) :
            self.targetNode.setVisible(False) 
         else :
            p = self.targetBody.getGlobalPosition() 
            p.y = 0.01 
            self.targetNode.setPosition(p)   
            self.targetNode.setVisible(True) 

         return True 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def mmToString(self,mm):
         if mm == MouseMode.CAMERA_FOLLOW:
               return "Camera (Following)" 
         if mm == MouseMode.CAMERA_CONTROL:
               return "Camera (Free)" 
         if mm == MouseMode.PICK:
               return "Mouse picking" 
         return "?" 


      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def frameEnded( self, evt):
         
         if (self.ShutdownNow):
            return False 
            
         return True 


      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def ApplicationStart(self):

         self.Camera.setPosition(10,10,10) 
         self.Camera.lookAt(0,0,0) 

         self.targetBody = None 
         self.hasTargetBody = False 

         self.targetNode = self.SceneMgr.getRootSceneNode().createChildSceneNode("target") 
         _entity = self.SceneMgr.createEntity("nx.bodyguide.ent", "nx.bodyguide.mesh") 
         _entity.setCastShadows(False) 
         self.targetNode.attachObject(_entity) 
         self.targetNode.setScale(0.5,0.5,0.5) 
         self.targetNode.setVisible(False) 
         
         self.firstFrame = True 
         self.timeSince = 0 
         self.speed = 1 

         self.prestart() 
         self.start() 
         print dir (self.World)
         s = self.World.getScenes()
         print s
         print dir(s)
         print s.count()
         print s.items
         print s['Main']
         print dir(s['Main'])
         self.RayCaster=nxogre.RayCaster(ogre.Vector3.ZERO, ogre.Vector3.ZERO, 200,self.World.getScenes()["Main"])
         print dir(self.RayCaster)
         
      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def ApplicationStop(self):
         self.prestop() 
         self.stop() 

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def getSetup(self):
         self.getTutorialSettings()
         print "\n\n",self.TutorialName , "\n\n"

      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def mouseMoveOrGrab(self, x,  y,  move_or_grab):

         r = self.Camera.getCameraToViewportRay(x / self.Window.getWidth(),
                        y / self.Window.getHeight() ) 

         self.RayCaster.setOrigin(r.getOrigin()) 
         self.RayCaster.setDirection(r.getDirection()) 

# # # # # # # # # #          if not self.RayCaster.castShape():
# # # # # # # # # #             return 

         ## Move it
         if move_or_grab:
            if self.hasTargetBody:
               p  = ogre.Plane()
               p.normal = r.getDirection() 
               p.d = -self.RayCaster.mHitPos.dotProduct(p.normal) 

               res = r.intersects(p) 
               objPoint = ogre.Vector3 () 

               if res.first:
                  force = r.getPoint(res.second) - self.targetBody.getGlobalPosition() 
                  force *= 10 
                  force -= physx.convert(self.targetBody.mActor.getPointVelocity(\
                                             physx.convert(self.targetBody.getGlobalPosition()))) 
                  self.targetBody.addForceAtPos(force, self.targetBody.getGlobalPosition()) 

            return 

         ## Grab it
         if not move_or_grab:

            t = self.RayCaster.mHitBody 

            if t.isStatic():
               self.targetBody = None 
               self.hasTargetBody = False 
               return 

            ## Can't add forces to Kinematics.
            if t.isKinematic():
               return 

            if t == self.targetBody:
               return 

            self.targetBody = t 
            self.hasTargetBody = True 


      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      def getTutorialSettings(self):
         print "\n\n**** WRONG\n"
         pass
      def start(self):
         pass 
      def stop(self):
         pass 
      def prestart(self):
         pass 
      def prestop(self):
         pass 
      def newFrame(self, _time):
         pass 
      def GUIbuttonPressed(self,ref):
         pass 
      
      ##//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if __name__ == '__main__':
    import exceptions,sys
    try:
        application = SimpleTutorial()
        application.go()
    except ogre.OgreException, e:
        print e
