# Viewer.py 

# Viewer.py 

import ogre.renderer.OGRE as ogre 
import sys 

class Viewer: 
    def __init__(self): 
        self.root          = None 
        self.sceneManager  = None 
        self.renderWindow  = None 
        self.cameras       = [] 
        self.viewports     = [] 
        
    def setUp(self): 
        
        self.root = ogre.Root("./plugins.cfg") 
                
        #set up resources 
        config = ogre.ConfigFile() 
        config.load("./resources.cfg") 
        seci = config.getSectionIterator() 
        while seci.hasMoreElements(): 
            SectionName = seci.peekNextKey() 
            Section = seci.getNext() 
            for item in Section: 
                ogre.ResourceGroupManager.getSingleton().\
                    addResourceLocation(item.value, item.key, SectionName) 

        # configure 
        carryOn = self.root.showConfigDialog() 
        if not carryOn: 
            return 

        try:  ############## DIES HERE ################## 
            self.renderWindow = self.root.initialise(True, "OGRE Render Window") 
        except Exception, e:         ###note ogre.OgreException doesn't catch anything 
            print '## EXCEPTION ##' 
            print str(e)                   ###prints "unidentifiable C++ exception" 
            return 
        
        self.sceneManager = self.root.createSceneManager(ogre.ST_GENERIC,"ExampleSMInstance") 
    
        self.camera = PlayerCam(self.sceneManager, "player1") 
        self.viewport = PlayerViewport(self.renderWindow, self.camera) 
              
        ogre.ResourceGroupManager.getSingleton().initialiseAllResourceGroups() 

class PlayerViewport: 
    def __init__(self, renderWindow, camera): 
        self.viewport = renderWindow.addViewport(camera.getRealCamera()) 
        self.viewport.BackgroundColour = ogre.ColourValue(0.0,1.0,1.0) 
    pass 

class PlayerCam: 
    def __init__(self, sceneManager, name): 
        self.name = name 
        self.camera = sceneManager.createCamera(name) 

        # temp 
        self.camera.setPosition(ogre.Vector3(0, 0, 500)) 
        self.camera.lookAt(ogre.Vector3(0, 0, -300)) 
        self.camera.NearClipDistance = 5        

    pass 


#Testing code 
v = Viewer() 
v.setUp() 
print "done"