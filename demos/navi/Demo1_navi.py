# For the latest inf
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------
import ogre.renderer.OGRE as ogre
import ogre.gui.navi as navi
import ogre.io.OIS as OIS
import SampleFramework as sf


naviMgr = navi.NaviManager.Get()

class NaviDemo():
    def __init__(self):
        self.shouldQuit = False
        self.renderWin = 0
        self.sceneMgr = 0


    def Startup( self ):
        root = ogre.Root("./plugins.cfg", "./ogre.cfg", "./ogre.log")
        if not root.restoreConfig():
            self.shouldQuit = not root.showConfigDialog()
            if self.shouldQuit:
                return
        root.saveConfig()
        self.root = root
        self.renderWin = root.initialise(True, "NaviDemo")
        
        self.sceneMgr = root.createSceneManager("TerrainSceneManager", "NaviDemoSceneMgr")
        self.sceneMgr.setAmbientLight(ogre.ColourValue(1, 1, 1))
        self.sceneMgr.setShadowTechnique(ogre.SHADOWTYPE_TEXTURE_ADDITIVE)
        self.sceneMgr.setFog( ogre.FOG_LINEAR, ogre.ColourValue(0.9, 0.9, 1), 0.0, 0, 1550 )
    
        camera = self.sceneMgr.createCamera("MainCam")
        camera.setPosition(0,10,-15)
        camera.lookAt(0, 0, 30)
        viewport = self.renderWin.addViewport(camera)
        viewport.setBackgroundColour(ogre.ColourValue(0.5, 0.5, 0.5))
        camera.setAspectRatio(viewport.getActualWidth() / viewport.getActualHeight())
        camera.setFarClipDistance( 1000 )
        camera.setNearClipDistance( 1 )
        camNode = self.sceneMgr.getRootSceneNode().createChildSceneNode("camNode")
        camNode.attachObject(camera)
    
        if (root.getRenderSystem().getCapabilities().hasCapability(ogre.RSC_INFINITE_FAR_PLANE)):
            camera.setFarClipDistance(0)
    
        light = self.sceneMgr.createLight( "Sun" )
        light.setType( ogre.Light.LT_DIRECTIONAL )
        light.setDiffuseColour( ogre.ColourValue( .82, .81, .7 ) )
        light.setSpecularColour( ogre.ColourValue( .82, .81, .7 ) )
        light.setDirection( ogre.Vector3( 0, -1, 1 ) ) 
            
        self.parseResources()
    
        self.sceneMgr.setSkyDome( True, "CloudySky", 5, 6, 8 )
    
    
        ## Startup, create, and manage Navis 
        naviMgr.Startup(self.renderWin)
    
        naviMgr.createNavi("menubar", "local://menubar.html", navi.NaviPosition(navi.BottomCenter), 1024, 128, False)
        naviMgr.setNaviMask("menubar", "navimenu_bg.png")
#     #   naviMgr.bind("menubar", "turnOn", NaviDelegate(this, &NaviDemo.turnOn), Strings("name"))
#     #   naviMgr.bind("menubar", "turnOff", NaviDelegate(this, &NaviDemo.turnOff), Strings("name"))
#     #   naviMgr.bind("menubar", "hpChange", NaviDelegate(this, &NaviDemo.hpChange), Strings("direction"))
#     
#         naviMgr.createNavi("status", "local:##status.html", navi.NaviPosition(0, 0), 512, 256, True, False)
#         naviMgr.setNaviMask("status", "status_bg.png")
#     #   naviMgr.bind("status", "levelChanged", NaviDelegate(this, &NaviDemo.levelChanged), Strings("#level"))
#     
#         naviMgr.createNavi("chat", "http:##navi.agelessanime.com/chat", navi.NaviPosition(navi.BottomLeft, 40, -150), 512, 256, True, False)
#         naviMgr.setNaviMask("chat", "navichat_bg.png")
#         naviMgr.setNaviColorKey("chat", "#19001a", 0.7)
#     #   naviMgr.bind("chat", "messageSent", NaviDelegate(this, &NaviDemo.messageSent), Strings("nick")("message"))
#     
#         naviMgr.createNavi("equip", "local:##equip.html", navi.NaviPosition(navi.Right), 256, 512, True, False)
#         naviMgr.setNaviMask("equip", "naviequip_bg.png")
#     #   naviMgr.bind("equip", "itemEquipped", NaviDelegate(this, &NaviDemo.itemEquipped), Strings("name"))
#     
#         ## Startup NaviMouse and create the cursors
#         mouse = naviMgr.StartupMouse()
#         mouse.createCursor("defaultCursor", 3, 2).addFrame(0, "cursor1.png")
#         mouse.setDefaultCursor("defaultCursor")
#     
#         ## If you define a "move" cursor, NaviMouse will automatically use that when you move a Navi
#         moveCursor = mouse.createCursor("move", 19, 19)
#         moveCursor.addFrame(0, "cursorMove.png")
#     
        ## Create the ground
        plane = ogre.Plane ( ogre.Vector3.UNIT_Y, 0)
        ogre.MeshManager.getSingleton().createPlane("ground",
            ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, plane,
            1500,1500,1,1,True,1,9,9,ogre.Vector3.UNIT_Z)
        ent = self.sceneMgr.createEntity( "GroundEntity", "ground" )
        self.sceneMgr.getRootSceneNode().createChildSceneNode("GroundNode").attachObject(ent)
        ent.setMaterialName("Terrain")
        ent.setCastShadows(False)
    
        ## Create the Blue Knot
        ent = self.sceneMgr.createEntity( "BlueMetalKnot", "knot.mesh" )
        ent.setMaterialName("Knot")
        ent.setCastShadows( True )
    
        ## Position and scale the Blue Knot
        node = self.sceneMgr.getRootSceneNode().createChildSceneNode("KnotNode", ogre.Vector3(0, 12, 45))
        node.attachObject( ent )
        node.scale=(.07, .07, .07)
        print "Done Startup"
    
        ##loadInputSystem()
    
        ##Ogre.WindowEventUtilities.addWindowEventListener(self.renderWin, this)

    def turnOn(naviData):
        naviName = naviData["name"].str()
    
        if(naviName == "quit"):
            self.shouldQuit = True
        else:
            naviMgr.resetNaviPosition(naviName)
            naviMgr.showNavi(naviName, True)

    def turnOff(naviData):
        naviMgr.hideNavi(naviData["name"].str(), True)

# # #     def hpChange(const NaviData &naviData)
# # # {
# # #   static short curHP = 100
# # # 
# # #   if(naviData["direction"].str() == "up")
# # #   {
# # #       curHP += (rand() % 30) + 1
# # #       if(curHP > 100) curHP = 100
# # #       std.stringstream setHP
# # #       setHP << "setHP(" << curHP << ")"
# # #       naviMgr.naviEvaluateJS("status", setHP.str())
# # #   }
# # #   else
# # #   {
# # #       curHP -= (rand() % 30) + 1
# # #       if(curHP < 0) curHP = 0
# # #       std.stringstream setHP
# # #       setHP << "setHP(" << curHP << ")"
# # #       naviMgr.naviEvaluateJS("status", setHP.str())
# # #   }
# # # 
# # #   std.stringstream hpMsg
# # #   hpMsg << "Navi Menubar: Current HP is now " << curHP << "%"
# # # 
# # #   LogManager.getSingleton().logMessage(hpMsg.str())
# # # }
# # # 
# # # void NaviDemo.messageSent(const NaviData &naviData)
# # # {
# # #   naviMgr.naviEvaluateJS("status", "$('playerName').innerHTML = '" + naviData["nick"].str() + "'")
# # #   LogManager.getSingleton().logMessage("Navi Chat: " + naviData["message"].str())
# # # }
# # # 
# # # void NaviDemo.itemEquipped(const NaviData &naviData)
# # # {
# # #   LogManager.getSingleton().logMessage("Navi Equip: Item '" + naviData["name"].str() + "' equipped!")
# # # }
# # # 
# # # void NaviDemo.levelChanged(const NaviData &naviData)
# # # {
# # #   int level = naviData["level"].toInt()
# # # 
# # #   std.stringstream levelMsg
# # #   levelMsg << "Navi Status: Level has been changed to: " << level << ", double that is: " << level * 2
# # # 
# # #   LogManager.getSingleton().logMessage(levelMsg.str())
# # # }

    def Update(self):
        navi.NaviManager.Get().Update()
        self.root.renderOneFrame()
        self.updateStats()
        ogre.WindowEventUtilities.messagePump()
        ##InputManager.getSingletonPtr().capture()
    
          

    def Shutdown(self):
        navi.NaviManager.Get().Shutdown()
        ogre.Root.getSingleton().shutdown()

    def parseResources(self):
        config = ogre.ConfigFile()
        config.load('resources.cfg' ) 
        seci = config.getSectionIterator()
        while seci.hasMoreElements():
            SectionName = seci.peekNextKey()
            Section = seci.getNext()
            for item in Section:
                ogre.ResourceGroupManager.getSingleton().\
                    addResourceLocation(item.value, item.key, SectionName)
        ogre.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()


# /* void NaviDemo.loadInputSystem()
# {
#   inputMgr = InputManager.getSingletonPtr()
#     inputMgr.initialise(self.renderWin)
#     inputMgr.addMouseListener(this, "NaviDemoMouseListener")
#   inputMgr.addKeyListener(this, "NaviDemoKeyListener")
# }*/

    def updateStats(self):
        return
        
if __name__ == '__main__':
    try:
        demo = NaviDemo()
        demo.Startup()

        while not demo.shouldQuit:
            demo.Update()
    except ogre.OgreException, e:
        print e

























































































































































































