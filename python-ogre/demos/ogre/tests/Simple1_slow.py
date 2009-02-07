import ogre.renderer.OGRE as ogre 
import ogre.io.OIS as OIS 

class AppFrameListener(ogre.FrameListener): 
  
    def __init__(self, keyboard, rw): 
        ogre.FrameListener.__init__(self) 
        self.keyboard = keyboard 
        self.rw=rw
        self.count=0
  
    def frameStarted(self, evt): 
        self.keyboard.capture() 
        self.count+=1
        if self.count > 100:
            print self.rw.getLastFPS()
            self.count=0
        return not self.keyboard.isKeyDown(OIS.KC_ESCAPE) 

class Application(object): 
    
    def __init__(self): 
        self.root = None 
        self.appFrameListener = None 
        self.sceneManager = None 
        self.camera = None 
        self.inputManager = None 
        self.keyboard = None 
        
    def __del__(self): 
        del self.keyboard 
        del self.inputManager 
        del self.camera 
        del self.sceneManager 
        del self.appFrameListener 
        del self.root 
    
    def go(self): 
        self.createRoot() 
        self.defineResources() 
        self.setupRenderSystem() 
        self.createRenderWindow() 
        self.initializeResourceGroups() 
        self.setupScene() 
        self.setupInputSystem() 
        #self.setupCEGUI() 
        self.createFrameListener() 
        self.startRenderLoop() 
        self.cleanUp() 
  
    def createRoot(self): 
        self.root = ogre.Root ( ) 
  
    def defineResources(self): 
        cf = ogre.ConfigFile() 
        cf.load("resources.cfg") 
        seci = cf.getSectionIterator() 
        while seci.hasMoreElements(): 
            secName = seci.peekNextKey() 
            settings = seci.getNext() 
            for item in settings: 
                typeName = item.key 
                archName = item.value 
                ogre.ResourceGroupManager.getSingleton().addResourceLocation(archName, typeName, secName) 
  
    def setupRenderSystem(self): 
        renderSystem = self.root.getRenderSystemByName("Direct3D9 Rendering Subsystem") 
        self.root.setRenderSystem(renderSystem); 
        renderSystem.setConfigOption("Full Screen", "No") 
        renderSystem.setConfigOption("Video Mode", "800 x 600 @ 32-bit colour") 
        renderSystem.setConfigOption("VSync", "No") 
        
        print dir(renderSystem)
  
    def createRenderWindow(self): 
        self.root.initialise(True, "Prova TitleManager") 
        """ 
        # Do not add this to the application 
        self.root.initialise(False) 
        hWnd = 0  # Get the hWnd of the application! 
        misc = ogre.NameValuePairList() 
        misc["externalWindowHandle"] = str(int(hWnd)) 
        renderWindow = self.root.createRenderWindow("Main RenderWindow", 800, 600, False, misc) 
        """ 
  
    def initializeResourceGroups(self): 
        ogre.TextureManager.getSingleton().setDefaultNumMipmaps(5) 
        ogre.ResourceGroupManager.getSingleton().initialiseAllResourceGroups() 
  
    def setupScene(self): 
        self.sceneManager = self.root.createSceneManager(ogre.ST_GENERIC, "Default SceneManager") 
        self.camera = self.sceneManager.createCamera("Camera") 
        viewPort = self.root.getAutoCreatedWindow().addViewport(self.camera) 
        
        overlayManager = ogre.OverlayManager.getSingleton() 
        overlay = overlayManager.create ( "overlay" ) 
        panel = overlayManager.createOverlayElement ( "Panel", "container1" ) 
        panel.setDimensions ( 1, 1 ) 
        panel.setPosition ( 0, 0 ) 
        overlay.add2D ( panel ) 
        overlay.show ( ) 
        
        textBox = overlayManager.createOverlayElement ( "TextArea", "txt1" ) 
        textBox.setDimensions ( 500.0, 300.0 ) 
        textBox.setMetricsMode ( ogre.GMM_PIXELS ) 
        textBox.setPosition ( 100.0, 20.0 ) 
#         textBox.setParameter ( "font_name", "MyFont" ) 
        textBox.setParameter ( "char_height", "40" ) 
        textBox.setCaption ( "Ciao Mondo!" ) 
        
        panel.addChild ( textBox ) 
        
  
    def setupInputSystem(self): 
        windowHandle = 0 
        self.renderWindow = self.root.getAutoCreatedWindow() 
        windowHandle = self.renderWindow.getCustomAttributeInt("WINDOW") 
        paramList = [("WINDOW", str(windowHandle))] 
        self.inputManager = OIS.createPythonInputSystem(paramList) 
        try: 
            self.keyboard = self.inputManager.createInputObjectKeyboard(OIS.OISKeyboard, False) 
        except Exception, e: 
            raise e 
  
    def setupCEGUI(self): 
        pass 
  
    def createFrameListener(self): 
        self.appFrameListener = AppFrameListener ( self.keyboard, self.renderWindow ) 
        self.root.addFrameListener(self.appFrameListener) 
  
    def startRenderLoop(self): 
        self.root.startRendering() 
  
    def cleanUp(self): 
        self.inputManager.destroyInputObjectKeyboard(self.keyboard) 
        OIS.InputManager.destroyInputSystem(self.inputManager) 
        self.inputManager = None 

if __name__ == '__main__': 
    try: 
        ta = Application() 
        ta.go() 
    except ogre.OgreException, e: 
        print e 