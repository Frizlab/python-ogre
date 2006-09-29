## This code is in the Public Domain

## This currently only works with wx 2.5!
## If you are using wx 2.4, comment out the MainLoop function.
import wx, sys
from wx import Frame
import Ogre as ogre
import time

def getPluginPath():
    """Return the absolute path to a valid plugins.cfg file.""" 
    import sys
    import os
    import os.path
    
    paths = [os.path.join(os.getcwd(), 'plugins.cfg'),
             '/etc/OGRE/plugins.cfg',
             os.path.join(os.path.dirname(os.path.abspath(__file__)),
                              'plugins.cfg')]
    for path in paths:
        if os.path.exists(path):
            return path

    sys.stderr.write("\n"
        "** Warning: Unable to locate a suitable plugins.cfg file.\n"
        "** Warning: Please check your ogre installation and copy a\n"
        "** Warning: working plugins.cfg file to the current directory.\n\n")
    raise ogre.Exception(0, "can't locate the 'plugins.cfg' file", "")

class MyFrame(wx.Frame):
    def __init__(
            self, parent, ID, title, pos=wx.DefaultPosition,
            size=wx.DefaultSize, style=wx.DEFAULT_FRAME_STYLE
           ):
        size = wx.Size(800,600)
        wx.Frame.__init__(self, parent, ID, title, pos, size)
        self.Bind(wx.EVT_CLOSE, self.OnCloseWindow)
        self.Bind(wx.EVT_ERASE_BACKGROUND, self.OnEraseBackground)
        self.Bind(wx.EVT_SIZE, self.OnSize)

    def OnSize(self, event):
        size = self.GetClientSize()
        print 'resizing', size
        if getattr(self, 'app', None):
            self.app.update()
        event.Skip()

    def OnEraseBackground(self, event):
        pass # Do nothing, to avoid flashing on MSW.

#     def OnCloseMe(self, event):
#         if getattr(self, 'app', None):
#             self.app.keepGoing = False
#         self.Close(True)

    def OnCloseWindow(self, event):
        print "IN CLOSE WINDOW"
        self.app.keepGoing = False
#         self.app.sm.clearScene()
#         self.Destroy()

class App(wx.App):
    """Application class."""

    def OnInit(self):
        print "OnInit"
        self.frame = MyFrame(None, -1, 'PopupButton Test')
        self.frame.app = self   # the frame needs to know about the app for window closing etc
        self.frame.Show(True)
        self.SetTopWindow(self.frame)
        self.keepGoing = True
        return True

    def initializeOgre(self):    
        print "INIT"
        root = ogre.Root(getPluginPath())
        self.root = root

        # setup resources
        config = ogre.ConfigFile()
        config.load('resources.cfg', '', False )
#         for section, key, path in config.values:
#             ogre.ResourceGroupManager.getSingleton().addResourceLocation(path, key, section)            
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/packs/OgreCore.zip", "Zip", "Bootstrap", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media", "FileSystem", "General", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/fonts", "FileSystem", "General", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/materials/programs", "FileSystem", "General", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/materials/scripts", "FileSystem", "General", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/materials/textures", "FileSystem", "General", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/models", "FileSystem", "General", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/overlays", "FileSystem", "General", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/particle", "FileSystem", "General", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/packs/cubemap.zip", "Zip", "General", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/packs/cubemapsJS.zip", "Zip", "General", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/packs/dragon.zip", "Zip", "General", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/packs/fresneldemo.zip", "Zip", "General", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/packs/ogretestmap.zip", "Zip", "General", False)
        ogre.ResourceGroupManager.getSingleton().addResourceLocation("../media/packs/skybox.zip", "Zip", "General", False)

        #configure
        carryOn = root.showConfigDialog()
        if not carryOn:
            sys.exit('Quit from Config Dialog') 


        root.initialise(False)
        renderParameters = ogre.NameValuePairList()
        renderParameters['externalWindowHandle'] = str(self.frame.GetHandle())
        renderWindow = root.createRenderWindow('TestOgre render window', 640,
                                               480, False, renderParameters)
        ogre.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()

        # chooseSceneManager
        ##sceneManager = root.getSceneManager(ogre.ST_GENERIC)
        sceneManager = root.createSceneManager(ogre.ST_GENERIC,"ExampleSMInstance")
        self.sm = sceneManager
        # createCamera
        camera = sceneManager.createCamera('PlayerCam')
        camera.setPosition (ogre.Vector3(0, 0, 500))
        camera.lookAt(ogre.Vector3(0, 0, -300))
        camera.nearClipDistance = 5
        print "1"

        # createViewports
        viewport = renderWindow.addViewport(camera, 0, 0.0, 0.0, 1.0, 1.0)
        viewport.setBackgroundColour (ogre.ColourValue(0, 0, 0))

        # set mipmaps ignored...

        # create scene
        sceneManager.setAmbientLight (ogre.ColourValue(0.2, 0.2, 0.2))
        sceneManager.setSkyDome(True, 'Examples/CloudySky', 4.0, 8.0)

        light = sceneManager.createLight('MainLight')
        light.setPosition (ogre.Vector3(20, 80, 50))

        plane = ogre.Plane()
        plane.normal = ogre.Vector3(0, 1, 0)
        plane.d = 200
        ogre.MeshManager.getSingleton().createPlane('FloorPlane', "General",
                                                    plane, 200000, 200000,
                                                    20, 20, True, 1, 50, 50,
                                                    ogre.Vector3(0, 0, 1),
               ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, 
               True,True )

        # create floor entity
        entity = sceneManager.createEntity('floor', 'FloorPlane')
        entity.setMaterialName('Examples/RustySteel')
        sceneManager.rootSceneNode.createChildSceneNode().attachObject(entity)

        # create head entity
        headNode = sceneManager.rootSceneNode.createChildSceneNode()
        entity = sceneManager.createEntity('head', 'ogrehead.mesh')
        headNode.attachObject(entity)

        # make sure the camera track this node
        camera.setAutoTracking(True, headNode)

        # create the camera node & attach camera
        cameraNode = sceneManager.rootSceneNode.createChildSceneNode()
        cameraNode.attachObject(camera)

        # set up spline animation of node
        animation = sceneManager.createAnimation('CameraTrack', 10)
        animation.interpolationMode = ogre.Animation.IM_SPLINE
        animationTrack = animation.createNodeTrack(0, cameraNode)
        key = animationTrack.createNodeKeyFrame(0)
        key = animationTrack.createNodeKeyFrame(2.5)
        key.translation = (500.0, 500.0, -1000.0)
        key = animationTrack.createNodeKeyFrame(5)
        key.translation = (-1500.0, -1500.0, -600.0)
        key = animationTrack.createNodeKeyFrame(7.5)
        key.translation = (0.0, -100.0, 0.0)
        key = animationTrack.createNodeKeyFrame(10.0)
        key.translation = (0.0, 0.0, 0.0)
        animationState = sceneManager.createAnimationState('CameraTrack')
        animationState.setEnabled (True)
        
        # add some fog
        sceneManager.setFog(ogre.FOG_EXP, ogre.ColourValue.White, 0.0002)
        self.frame.app = self
        
        renderWindow.active = True
        self.update()

    def update(self):
        self.root.renderOneFrame()

    def MainLoop(self):
        print "mainloop"
        self.keepGoing = True
        evtloop = wx.EventLoop()
        old = wx.EventLoop.GetActive()
        wx.EventLoop.SetActive(evtloop)
        
        while self.keepGoing:
            self.update()

            while evtloop.Pending():
                evtloop.Dispatch()

            time.sleep(0.10)
            self.ProcessIdle()

            while self.Pending():
                self.Dispatch()
#             self.update()
        print "TIME TO KILL IT ALL"
        self.sm.clearScene()    # need to stop ogre rendering etc first
        self.root.shutdown()
        print "Now destroy the frame"
        self.frame.Destroy()    # now kill the frame
        print "Frame destroyed"
#         self.root.shutdown()    


def main():
    try:
        app = App(False)
        app.initializeOgre()
        app.MainLoop()
    except ogre.Exception, e:
        print e

if __name__ == '__main__':
    main()
