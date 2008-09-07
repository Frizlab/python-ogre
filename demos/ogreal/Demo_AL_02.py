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

import ogre.renderer.OGRE as ogre
import ogre.sound.OgreAL as OgreAL

import SampleFramework as sf


class SkyBoxApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        sceneManager.setSkyBox(True, "Examples/SpaceSkyBox")
        
        # Need a light 
        light = sceneManager.createLight('MainLight')
        light.setPosition (20, 80, 50)
        # Also add a nice starship in
        ent = sceneManager.createEntity( "razor", "razor.mesh" )

        node = sceneManager.getRootSceneNode().createChildSceneNode()
        node.attachObject( ent )
        
        self.soundManager  = OgreAL.SoundManager() ## "Generic Software")

        sound = self.soundManager.createSound("Roar", "roar.wav", True) ## "6chan.ogg", True) ## "roar.wav", True)
        node.attachObject(sound)
        sound.play()
#         print "*** ", sound
# #         print "*** ", OgreAL.SoundManager.getSingleton().getSound("Roar").play()
#         print "*** ", self.soundManager

    def __del__ ( self ):
        del self.soundManager
        sf.Application.__del__(self)
        

    def _createFrameListener(self):
        self.frameListener = SkyBoxListener(self.renderWindow, self.camera, self.sceneManager)
        self.root.addFrameListener(self.frameListener)

class SkyBoxListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, sceneManager):
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.sceneManager = sceneManager
         
    def frameStarted(self, frameEvent):
        return sf.FrameListener.frameStarted(self, frameEvent)

if __name__ == '__main__':
    try:
        application = SkyBoxApplication()
        application.go()
    except ogre.OgreException, e:
        print e
