# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http:##python-ogre.org/
#
# It is likely based on original code from OGRE and/or PyOgre
# For the latest info, see http:##www.ogre3d.org/
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------
import ogre.renderer.OGRE as ogre
import ogre.gui.navi as navi
import ogre.io.OIS as OIS
import SampleFramework as sf
import random

class NaviApplication(sf.Application, ogre.WindowEventListener):
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera
        self.naviMgr = navi.NaviManager.Get()
        self.shouldQuit = False # flag to allow the app to do a shutdown
        
        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        sceneManager.setSkyDome(True, 'CloudySky', 5.0, 6, 8,)
        light = sceneManager.createLight( "Sun" )
        light.setType( ogre.Light.LT_DIRECTIONAL )
        light.setDiffuseColour( ogre.ColourValue( .82, .81, .7 ) )
        light.setSpecularColour( ogre.ColourValue( .82, .81, .7 ) )
        light.setDirection( ogre.Vector3( 0, -1, -1 ) ) 

        ## Create the Blue Knot
        ent = sceneManager.createEntity( "BlueMetalKnot", "knot.mesh" )
        ent.setMaterialName("Knot")
        ent.setCastShadows( True )
    
        ## Position and scale the Blue Knot
        node = sceneManager.getRootSceneNode().createChildSceneNode("KnotNode", ogre.Vector3(0, 0, 400))
        node.attachObject( ent )
        node.scale=(.2, .2, .2)
        
        ## Startup, create, and manage Navis 
        self.naviMgr.Startup(self.renderWindow)
    
        self.naviMgr.createNavi("menubar", "local://menubar.html", navi.NaviPosition(navi.BottomCenter), 1024, 128, False)
        self.naviMgr.setNaviMask("menubar", "navimenu_bg.png")
        self.naviMgr.bind("menubar", "turnOn", self, "turnOn", ["name"] )
        self.naviMgr.bind("menubar", "turnOff", self, "turnOff", ["name"] )
        self.naviMgr.bind("menubar", "hpChange", self,"hpChange", ["direction"] )
    
        self.naviMgr.createNavi("status", "local://status.html", navi.NaviPosition(0, 0), 512, 256, True, False)
        self.naviMgr.setNaviMask("status", "status_bg.png")
        self.naviMgr.bind("status", "levelChanged", self,"levelChanged", ["#level"])
    
        self.naviMgr.createNavi("chat", "http://navi.agelessanime.com/chat", navi.NaviPosition(navi.BottomLeft, 40, -150), 512, 256, True, False)
        self.naviMgr.setNaviMask("chat", "navichat_bg.png")
        self.naviMgr.setNaviColorKey("chat", "#19001a", 0.7)
        self.naviMgr.bind("chat", "messageSent", self, "messageSent", ["nick","message"])
    
        self.naviMgr.createNavi("equip", "local://equip.html", navi.NaviPosition(navi.Right), 256, 512, True, False)
        self.naviMgr.setNaviMask("equip", "naviequip_bg.png")
        self.naviMgr.bind("equip", "itemEquipped", self, "itemEquipped", ["name"])
    
        ## Startup NaviMouse and create the cursors
        mouse = self.naviMgr.StartupMouse()
        mouse.createCursor("defaultCursor", 3, 2).addFrame(0, "cursor1.png")
        mouse.setDefaultCursor("defaultCursor")
    
        ## If you define a "move" cursor, NaviMouse will automatically use that when you move a Navi
        moveCursor = mouse.createCursor("move", 19, 19)
        moveCursor.addFrame(0, "cursorMove.png")
        self.curHP = 100
     
    def turnOn(self, naviData):
        naviName = naviData["name"].str()
        if(naviName == "quit"):
            self.shouldQuit = True
        else:
            self.naviMgr.resetNaviPosition(naviName)
            self.naviMgr.showNavi(naviName, True)
            
    def turnOff(self, naviData):
        self.naviMgr.hideNavi(naviData["name"].str(), True)

    def hpChange(self,naviData):
        print "HPCHANGE"
        if naviData["direction"].str() == "up":
            self.curHP += (random.random() % 30) + 1
            if self.curHP > 100: self.curHP = 100
            self.naviMgr.naviEvaluateJS("status", "setHP(" + str(self.curHP)+")" )
        else:
            self.curHP -= (random.random() % 30) + 1
            if self.curHP < 0: self.curHP = 0
            self.naviMgr.naviEvaluateJS("status", "setHP(" + str(self.curHP)+")" )

        ogre.LogManager.getSingleton().logMessage("Navi Menubar: Current HP is now " + str(self.curHP)+"%")

    def messageSent(self, naviData):
        self.naviMgr.naviEvaluateJS("status", "$('playerName').innerHTML = '" + naviData["nick"].str() + "'")
        ogre.LogManager.getSingleton().logMessage("Navi Chat: " + naviData["message"].str())

    def itemEquipped(self, naviData):
        ogre.LogManager.getSingleton().logMessage("Navi Equip: Item '" + naviData["name"].str() + "' equipped!")

    def levelChanged(seld, naviData):
        level = naviData["level"].toInt()
#         levelMsg << "Navi Status: Level has been changed to: " << level << ", double that is: " << level * 2
#         LogManager.getSingleton().logMessage(levelMsg.str())
        
    def _createFrameListener(self):
        self.frameListener = frameListener(self.renderWindow, self.camera, self)
        self.root.addFrameListener(self.frameListener)

class frameListener(sf.FrameListener, OIS.MouseListener, OIS.KeyListener):
    def __init__(self, renderWindow, camera,app):
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.showDebugOverlay(False) # don't show the overlay 
        OIS.KeyListener.__init__(self)
        OIS.MouseListener.__init__(self)
        self.app = app
        self.Mouse.setEventCallback(self)
        self.MenuMode = True # stops the mouse moving the background
        self.Keyboard.setEventCallback(self)
                      
    def mouseMoved(self,arg):
        if(arg.get_state().Z.rel != 0):
             navi.NaviManager.Get().injectMouseWheel(arg.get_state().Z.rel)
        return navi.NaviManager.Get().injectMouseMove(arg.get_state().X.abs, arg.get_state().Y.abs)

    def mousePressed(self, arg, id):
        return navi.NaviManager.Get().injectMouseDown(id)
    
    def mouseReleased(self, arg, id):
        return navi.NaviManager.Get().injectMouseUp(id)
               
    def frameStarted(self, frameEvent):
        navi.NaviManager.Get().Update()
        ret = sf.FrameListener.frameStarted(self, frameEvent)
        if self.app.shouldQuit == True: ret = False # force the exit
        if not ret: # we've been asked to exit
            navi.NaviManager.Get().Shutdown()  
        return ret
                       
if __name__ == '__main__':
    try:
        application = NaviApplication()
        application.go()
    except ogre.OgreException, e:
        print e
