

import ogre.renderer.OGRE as ogre
import ogre.io.OIS as ois


class TestApp(object):

    def __init__(self):

        self.root = ogre.Root('plugins.cfg')
        self.rgm = ogre.ResourceGroupManager.getSingleton()
        self.loadResources('resources.cfg')
        self.root.showConfigDialog()
        self.window = self.root.initialise(True, "addChild Bug")
        self.scenemanager = self.root.createSceneManager(ogre.ST_GENERIC)
        self.camera = self.scenemanager.createCamera("camera")
        self.camera.setPosition(0, 5, 5)
        self.camera.lookAt(ogre.Vector3(0, 0, 0))
        self.camera.setNearClipDistance(0.1)
        self.camera.setFarClipDistance(10.0)
        self.viewport = self.window.addViewport(self.camera)
        self.rgm.initialiseAllResourceGroups()
        self.scenemanager.setAmbientLight(ogre.ColourValue(0.5, 0.5, 0.5))
        self.light = self.scenemanager.createLight("KeyLight")
        self.light.setPosition(5, 5, 5)
        self.light.setType(ogre.Light.LT_POINT)
        
        # scenegraph construction
        self.rootnode = self.scenemanager.getRootSceneNode()

        # works
        self.childnode1 = self.scenemanager.createSceneNode('childnode1')
        self.childnode1.setPosition(ogre.Vector3(-2,0,0))
        self.rootnode.addChild(self.childnode1)
        self.entity1 = self.scenemanager.createEntity('cube1', 'ogrehead.mesh')
        self.childnode1.attachObject(self.entity1)

        # doesn't work
#         self.childnode2 = self.rootnode.createChildSceneNode('childnode2')
#         self.childnode2.setPosition(ogre.Vector3(+2,0,0))
#         self.entity2 = self.scenemanager.createEntity('cube2', 'ogrehead.mesh')
#         self.childnode2.attachObject(self.entity2)
        
        # per-frame callback
        self.frameListener = FrameListener(self)
        self.root.addFrameListener(self.frameListener)


    
    def loadResources(self, resourcepath):
        config = ogre.ConfigFile()
        config.load(resourcepath)
        section_iter = config.getSectionIterator()
        while section_iter.hasMoreElements():
            section_name = section_iter.peekNextKey()
            settings = section_iter.getNext()
            for item in settings:
                self.rgm.addResourceLocation(item.value, item.key, section_name)



#------------------------------------------------------------------------------------

class FrameListener(ogre.FrameListener):

    def __init__(self, application):
        ogre.FrameListener.__init__(self)        

        self._keeprendering = True
        self._framenumber = 0
        self._bufferedkeys = True
        self.application = application

        windowhandle = self.application.window.getCustomAttributeInt("WINDOW")
        self._inputmanager = ois.createPythonInputSystem([("WINDOW",str(windowhandle))])
        self.keyboard = self._inputmanager.createInputObjectKeyboard(ois.OISKeyboard, self._bufferedkeys)


    def frameStarted(self, frameEvent):
        self._framenumber += 1

        # update input devices
        self.keyboard.capture()    

        if self.keyboard.isKeyDown(ois.KC_ESCAPE):
            self._keeprendering = False
            return self._keeprendering

        return self._keeprendering


    def frameEnded(self, frameEvent):
        return True


#--------------------------------------------------------------------------------

if __name__ == "__main__":

    import sys
    app = TestApp()
    app.root.startRendering()
    app.root.shutdown()
