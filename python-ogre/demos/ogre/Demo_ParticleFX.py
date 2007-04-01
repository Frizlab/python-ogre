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
import ogre.renderer.OGRE as ogre
import SampleFramework as sf

class ParticleApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera
        
        sceneManager.ambientLight = [0.5, 0.5, 0.5]

        entity = sceneManager.createEntity('head', 'ogrehead.mesh')
        
        ## Add entity to the root scene node
        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(entity)
        
        ## green numbus around Ogre
        particleSystem1 = sceneManager.createParticleSystem('Nimbus', 'Examples/GreenyNimbus')
        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(particleSystem1)


#         ## Fireworks  
#         ## WARNING -- needs a reasonably good graphics card or causes a crash
#         particleSystem1 = sceneManager.createParticleSystem('Fireworks', 'Examples/Fireworks')
#         sceneManager.getRootSceneNode().createChildSceneNode().attachObject(particleSystem1)

        self.fountainNode = sceneManager.getRootSceneNode().createChildSceneNode()

        particleSystem2 = sceneManager.createParticleSystem('fountain1', 'Examples/PurpleFountain')
        node = self.fountainNode.createChildSceneNode()
        node.translate(200.0, -100.0, 0.0)
        node.rotate(ogre.Vector3.UNIT_Z, ogre.Degree(20))
        node.attachObject(particleSystem2)

        particleSystem3 = sceneManager.createParticleSystem('fountain2', 'Examples/PurpleFountain')
        node = self.fountainNode.createChildSceneNode()
        node.translate(-200.0, -100.0, 0.0)
        node.rotate(ogre.Vector3.UNIT_Z, ogre.Degree(-20))
        node.attachObject(particleSystem3)

        particleSystem4 = sceneManager.createParticleSystem('rain', 'Examples/Rain')
        node = sceneManager.getRootSceneNode().createChildSceneNode()
        node.translate(0.0, 1000.0, 0.0)
        node.attachObject(particleSystem4)
        particleSystem4.fastForward(5)
#         
        # Aureola around Ogre perpendicular to the ground
        ## AJM This media is part of OgreSDK not in the current python_ogre set..
        try:
            particleSystem5 = sceneManager.createParticleSystem('Aureola', 'Examples/Aureola')
            sceneManager.getRootSceneNode().createChildSceneNode().attachObject(particleSystem5)
        except:
            pass

		# Set nonvisible timeout
        ogre.ParticleSystem.defaultNonVisibleUpdateTimeout=5

        
    def _createFrameListener(self):
        self.frameListener = ParticleListener(self.renderWindow, self.camera, self.fountainNode)
        self.root.addFrameListener(self.frameListener)

class ParticleListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, fountainNode):
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.fountainNode = fountainNode

    def frameStarted(self, frameEvent):
        self.fountainNode.yaw(ogre.Degree(frameEvent.timeSinceLastFrame * 30))
# #         dd = ogre.Degree(frameEvent.timeSinceLastFrame * 30.0)
# #         rad = ogre.Radian(dd)
# #         self.fountainNode.yaw(rad, ogre.Node.TS_LOCAL)
        return sf.FrameListener.frameStarted(self, frameEvent)

if __name__ == '__main__':
    try:
        application = ParticleApplication()
        application.go()
    except ogre.Exception, e:
        print e
