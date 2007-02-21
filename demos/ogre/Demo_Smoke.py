# This code is in the Public Domain
import  Ogre as ogre
import SampleFramework as sf
import ctypes, math

class SmokeApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        sceneManager.setSkyDome(True, 'Examples/CloudySky', 5.0, 8.0)

        self.fountainNode = sceneManager.getRootSceneNode().createChildSceneNode()
        
        psm = ogre.ParticleSystemManager.getSingleton()
        particleSystem2 = sceneManager.createParticleSystem('fountain1', 'Examples/Smoke')
        node = self.fountainNode.createChildSceneNode()
        node.attachObject(particleSystem2)
        
        
if __name__ == '__main__':
    try:
        application = SmokeApplication()
        application.go()
    except ogre.Exception, e:
        print e
