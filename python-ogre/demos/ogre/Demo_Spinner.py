# NO_UNITTEST
# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http://python-ogre.org/
#
# It is likely based on original code from OGRE and/or PyOgre
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
import ogre.io.OIS as OIS


class MyApp(object):

   def __init__(self, plugins_path='../plugins.cfg',
                resource_path='../resources.cfg'):

       # keep track of the paths that are used

       self.plugins_path = plugins_path
       self.resource_path = resource_path


       # gotta have a Root object :)
       self.root = ogre.Root(plugins_path)
       self._choose_render_engine()
       self.root.initialise(False)

       self.sm = self.root.createSceneManager(ogre.ST_GENERIC)

       self.sm.setAmbientLight(ogre.ColourValue(1, 1, 1))

       self.window = self.root.createRenderWindow("Woot", 800, 600,False)
       self.camera = self.sm.createCamera("camera")
       self.camera.setPosition(0, 100, 500)

       self.camera.lookAt(ogre.Vector3(0, 25, 0))

       self.camera.setNearClipDistance(15)
       self.viewport = self.window.addViewport(self.camera)
       self.viewport.setBackgroundColour(ogre.ColourValue(0, 0, 0))


       # keep track of the resource manager

       self.rgm = ogre.ResourceGroupManager.getSingleton()
       self._load_resources(self.resource_path)
       
       ## initalize resource here
       self.rgm.initialiseAllResourceGroups()
 
       self._build_scene()

       window_hnd = self.window.getCustomAttributeInt("WINDOW")
       self.input_manager = InputManager(self, window_hnd)


   def _choose_render_engine(self):

       # for now this forces D3D
       rend_list = self.root.getAvailableRenderers()
       print rend_list
       self.root.setRenderSystem(rend_list[-1])

   def _build_scene(self):

       s_root = self.sm.getRootSceneNode()
       self.ninja = UberSpinningNinja(self, s_root, ogre.Vector3(0,0,0))


   def _load_resources(self, resource_path):
        config = ogre.ConfigFile()
        config.load(resource_path)
        section_iter = config.getSectionIterator()
        while section_iter.hasMoreElements():
            section_name = section_iter.peekNextKey()
            settings = section_iter.getNext()
            for item in settings:
                self.rgm.addResourceLocation(item.value, item.key, section_name)


   def key_down(self, key):
       if key == OIS.KC_LEFT:
           self.ninja.start_spinning(-1)
       elif key == OIS.KC_RIGHT:
           self.ninja.start_spinning(1)

   def key_up(self, key):
       if key == OIS.KC_LEFT:
           self.ninja.stop_spinning()
       elif key == OIS.KC_RIGHT:
           self.ninja.stop_spinning()
       elif key == OIS.KC_ESCAPE:
           self.window.destroy()

   def render_one(self):
       self.frame_started()
       self.root.renderOneFrame()
       self.frame_ended()

   def frame_started(self):
       self.ninja.update()

   def frame_ended(self):
       self.input_manager.capture()

   def shutdown(self):
       self.input_manager.shutdown()
       self.root.shutdown()


class InputManager(OIS.KeyListener, OIS.MouseListener):
   def __init__(self, app, hnd):
       OIS.KeyListener.__init__(self)
       OIS.MouseListener.__init__(self)
       self.app = app
       self.input_mgr = OIS.createPythonInputSystem([("WINDOW", str(hnd))])

       # Setup Unbuffered Keyboard and Buffered Mouse Input
       self.keyboard = \
           self.input_mgr.createInputObjectKeyboard(OIS.OISKeyboard,True)
       self.keyboard.setEventCallback(self)
       #self.mouse = \
       #    self.input_mgr.createInputObjectMouse(OIS.OISMouse, True)
       #self.mouse.setEventCallback(self)

   def capture(self):
       self.keyboard.capture()
       #self.mouse.capture()


   def keyPressed(self, evt):
       # evt.device is a reference to the device that emitted this signal
       # evt.key is an OIS.KeyCode object (e.g. KC_SPACE)
       # evt.text appears to be a decimal keycode (e.g. 115)
       print "DOWN: %s" % evt.key
       self.app.key_down(evt.key)

   def keyReleased(self, evt):
       print "UP: %s" % evt.key
       self.app.key_up(evt.key)

   def mouseMoved(self, evt):
       return True

   def mousePressed(self, evt, button):
       print evt, button
       return True

   def mouseReleased(self, evt, button):
       print evt, button
       return True

   def shutdown(self):
       #self.input_mgr.destroyInputObjectMouse(self.mouse)

       self.input_mgr.destroyInputObjectKeyboard(self.keyboard)

       OIS.InputManager.destroyInputSystem(self.input_mgr)

class UberSpinningNinja(object):
   """Create a ninja!"""
   def __init__(self, app, node, start_coords):
       self.entity = app.sm.createEntity('ninja', 'ninja.mesh')
       self.node = node.createChildSceneNode("ninja_node", start_coords)
       self.node.attachObject(self.entity)
       
       src = self.node.Orientation * (ogre.Vector3().UNIT_Z) 
       directionToGo = ogre.Vector3(0,0,-1) 
       quat = src.getRotationTo(directionToGo)
       self.node.Orientation=quat 
       
       self.spinning_x = 0 # 1 for clockwise, -1 for counter clockwise

   def start_spinning(self, clock_wise=1):
       self.spinning_x = clock_wise

   def stop_spinning(self):
       self.spinning_x = 0

   def update(self):
       self.node.yaw(.002 * self.spinning_x)


def main():
   app = MyApp(plugins_path='../plugins.cfg.nt')
   while not app.window.isClosed():
       ogre.WindowEventUtilities().messagePump()
       if app.window.isActive():
          # all eyes are on us, render away
           app.render_one()
           time.sleep(.0001)
       else:
           # we got minimized or de-focused, so slow it down and stop rendering
           # until we get focus back
           time.sleep(1)

   app.shutdown()

if __name__ == "__main__":
   main()
