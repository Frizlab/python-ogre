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
sys.path.append(r"c:\python25\lib\site-packages")
import PythonOgreConfig
import ogre.renderer.OGRE as ogre
import SampleFramework as sf


class SmokeApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        sceneManager.setSkyDome(True, 'Examples/CloudySky', 5.0, 8.0)

        self.fountainNode = sceneManager.getRootSceneNode().createChildSceneNode()
        
        psm = ogre.ParticleSystemManager.getSingleton()
        self.particleSystem2 = sceneManager.createParticleSystem('fountain1', 'Examples/Smoke')
        node = self.fountainNode.createChildSceneNode()
        node.attachObject(self.particleSystem2)
        
    def __del__(self):
        del self.particleSystem2
        sf.Application.__del__(self)     
                
if __name__ == '__main__':
    try:
        application = SmokeApplication()
        application.go()
    except ogre.OgreException, e:
        print e
