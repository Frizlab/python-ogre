# NO_UNITTEST
# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http://python-ogre.org/
#
# It is likely based on original code from OGRE
# For the latest info, see http://www.ogre3d.org/
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------
import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import time

import ogre.renderer.OGRE as ogre 

class OGREMain(ogre.Root):
    def __init__(self, plugins_path='../plugins.cfg',resource_path='../resources.cfg'):
       ogre.Root.__init__(self, plugins_path)
       self.plugins_path = plugins_path
       self.resource_path = resource_path
       self.sm = None #: scene manager
       self.s_root = None #: root scene node
       self.rgm = ogre.ResourceGroupManager.getSingleton()
    
       self._load_resources(self.resource_path)
       self._choose_render_engine()
    
       self.initialise(False)
       self.initialisePlugins()
       
       
       self.sm = self.createSceneManager("TerrainSceneManager" ) #ogre.ST_GENERIC)
       self.window = self.createRenderWindow("Woot", 800, 600, False)
       
       self.sm.setAmbientLight(ogre.ColourValue(1, 1, 1))
       self.camera = self.sm.createCamera("camera")
       self.camera.setPosition(0, 100, 500)
       self.camera.lookAt(ogre.Vector3(0, 0, 0))
       self.camera.setNearClipDistance(15)
       self.viewport = self.window.addViewport(self.camera)
       self.viewport.setBackgroundColour(ogre.ColourValue(0, 0, 0))
       
       ## initalize resource here
       self.rgm.initialiseAllResourceGroups()
       
       self._build_scene()

    def _choose_render_engine(self):
        rend_list = self.getAvailableRenderers()
        for ren in rend_list:
            print ren.getName()
            cap = ren.getCapabilities()
            if cap:
                print "MaxPointSize:", cap.getMaxPointSize()
                print "Stencil stuff:", cap.getStencilBufferBitDepth()
                opts = ren.getConfigOptions()
                print "Opts:", opts
                print "Opts keys:",opts.keys()
                for i in opts:
                    print "Key:",i.key
                    print i.value.currentValue
                    print i.value.name
                    print i.value.immutable
                    for v in i.value.possibleValues:
                        print "Posible Value", v
                    
                print dir(opts)
                print "Opts", opts['Video Mode']
                print"Viewo Mode:",  ren.getConfigOptions()['Video Mode']
        self.setRenderSystem(rend_list[-1])

    def _build_scene(self):
        self.s_root = self.sm.getRootSceneNode()
        
        ninja = self.sm.createEntity('ninja', 'ninja.mesh')
        node = self.s_root.createChildSceneNode("ninja_node", ogre.Vector3(100, 0, 0))
        node.attachObject(ninja)
    
    def _load_resources(self, resource_path):
        config = ogre.ConfigFile()
        config.load(resource_path)
        section_iter = config.getSectionIterator()
        while section_iter.hasMoreElements():
            section_name = section_iter.peekNextKey()
            settings = section_iter.getNext()
            ### settings_tuples = config.getMultiMapSettings(settings)
            ##for resource_type, resource in settings_tuples:
            ##    self.rgm.addResourceLocation(resource, resource_type, section_name)
            for item in settings:
                self.rgm.addResourceLocation(item.value, item.key, section_name)


def main():
    import os
    if os.name == 'nt':
        root = OGREMain(plugins_path='../plugins.cfg.nt')
    else:
        root = OGREMain(plugins_path='../plugins.cfg.linux')
    weu = ogre.WindowEventUtilities()
    while not root.window.isClosed():
        weu.messagePump()
        if root.window.isActive():
            # all eyes are on us, render away
            root.renderOneFrame()
            time.sleep(.0001)
        else:
            # we got minimized or de-focused, so slow it down and stop
            # rendering until we get focus back
            time.sleep(1)
    root.shutdown()
    sys.exit(0)

if __name__ == "__main__":
   main()
