import wx 
import ogre.renderer.OGRE as ogre 
import ogre.io.OIS as ois 

class MyFrame1(wx.Frame): 
    def __init__(self, *args, **kwds): 
        kwds["style"] = wx.DEFAULT_FRAME_STYLE 
        wx.Frame.__init__(self, *args, **kwds) 
        self.pnl = wx.Panel(self, -1) 
        szr = wx.BoxSizer(wx.HORIZONTAL) 
        szr.Add(self.pnl, 1, wx.EXPAND, 0) 
        self.SetSizer(szr) 
        self.Layout() 
        self.SetSize((640,480))        
    def GetOgrePanelHandle(self): 
        return str(self.pnl.GetHandle()) 
    def GetOgrePanelSize(self): 
        return self.pnl.GetSize() 
    def GetMainWindowHandle(self): 
        return str(self.GetHandle())  

class OgreWrapper: 
    def __init__( self, externalHandle, mainHandle, (w,h) ): 
        self.root = ogre.Root("plugins.cfg") 
        if self.root == None: 
            print "Could not initialize root from plugins.cfg" 
            exit(1) 
            
        if( not self.root.restoreConfig() ): 
            self.root.showConfigDialog() 
        self.root.initialise(False) 
        self.sceneManager = self.root.createSceneManager(ogre.ST_GENERIC) 

        renderParameters = ogre.NameValuePairList() 
        renderParameters['externalWindowHandle'] = str(externalHandle) 
        self.renderWindow = self.root.createRenderWindow('Example',w,h,False,renderParameters ) 
        self.renderWindow.active = True 
        
        self.camera = self.sceneManager.createCamera("camera") 
        self.camera.position = (0,0,500) 
        self.camera.lookAt((0,0,-100)) 
        self.camera.nearClipDistance = 10 
        self.viewport = self.renderWindow.addViewport(self.camera) 
        self.viewport.backgroundColor = (0,0,0)        
        
        params = [("WINDOW",str(mainHandle)), 
                ("w32_mouse", "DISCL_NONEXCLUSIVE") ,
                ("w32_mouse", "DISCL_FOREGROUND"), 
                ] 
        self.inputManager = ois.createPythonInputSystem( params ) 
        self.m_Mouse = self.inputManager.createInputObject(ois.OISMouse,True)

    def Update(self): 
        self.root.renderOneFrame() 
        
               
if __name__ == "__main__": 
    app = wx.PySimpleApp(0) 
    wx.InitAllImageHandlers() #you may or may not need this 

    frame = MyFrame1(None,-1,"") 
    app.SetTopWindow(frame) 
    o = OgreWrapper( frame.GetOgrePanelHandle(),frame.GetMainWindowHandle(),  frame.GetOgrePanelSize() ) 
    frame.Show() 
    o.Update() 
    app.MainLoop() 
    
   

