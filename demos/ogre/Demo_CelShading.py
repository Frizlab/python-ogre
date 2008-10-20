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
import SampleFramework as sf

# Custom parameter bindings
CUSTOM_SHININESS = 1
CUSTOM_DIFFUSE = 2
CUSTOM_SPECULAR = 3

class CelShadingApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager

        capabilities = ogre.Root.getSingleton().getRenderSystem().capabilities
        if not capabilities.hasCapability(ogre.RSC_VERTEX_PROGRAM) or not capabilities.hasCapability(ogre.RSC_FRAGMENT_PROGRAM):
            raise ogre.Exception(111, 'Your card does not support vertex and fragment programs, so cannot run this demo. Sorry!', 'celshadingdemo.py')
        
        # Accept default settings: point light, white diffuse, just set position
        light = sceneManager.createLight('MainLight')
        self.rotationNode = sceneManager.getRootSceneNode().createChildSceneNode()
        self.rotationNode.createChildSceneNode((20,40,50)).attachObject(light)

        # create head entity
        entity = sceneManager.createEntity('head', 'ogrehead.mesh')
        self.camera.setPosition (20, 0, 100)
        self.camera.lookAt(0, 0, 0)

        # eyes
        sub = entity.getSubEntity(0)
        sub.materialName = 'Examples/CelShading'
        sub.setCustomParameter(CUSTOM_SHININESS, (35.0, 0.0, 0.0, 0.0))
        sub.setCustomParameter(CUSTOM_DIFFUSE, (1.0, 0.3, 0.3, 1.0))
        sub.setCustomParameter(CUSTOM_SPECULAR, (1.0, 0.6, 0.6, 1.0))

        # skin
        sub = entity.getSubEntity(1)
        sub.materialName = 'Examples/CelShading'
        sub.setCustomParameter(CUSTOM_SHININESS, (10.0, 0.0, 0.0, 0.0))
        sub.setCustomParameter(CUSTOM_DIFFUSE, (0.0, 0.5, 0.0, 1.0))
        sub.setCustomParameter(CUSTOM_SPECULAR, (0.3, 0.5, 0.3, 1.0))

        # earring
        sub = entity.getSubEntity(2)
        sub.materialName = 'Examples/CelShading'
        sub.setCustomParameter(CUSTOM_SHININESS, (25.0, 0.0, 0.0, 0.0))
        sub.setCustomParameter(CUSTOM_DIFFUSE, (1.0, 1.0, 0.0, 1.0))
        sub.setCustomParameter(CUSTOM_SPECULAR, (1.0, 1.0, 0.7, 1.0))

        # teeth
        sub = entity.getSubEntity(3)
        sub.materialName = 'Examples/CelShading'
        sub.setCustomParameter(CUSTOM_SHININESS, (20.0, 0.0, 0.0, 0.0))
        sub.setCustomParameter(CUSTOM_DIFFUSE, (1.0, 1.0, 0.7, 1.0))
        sub.setCustomParameter(CUSTOM_SPECULAR, (1.0, 1.0, 1.0, 1.0))

        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(entity)
        self.renderWindow.getViewport(0).backgroundColour = (1, 1, 1)

    def _createFrameListener(self):
        self.frameListener = CelShadingListener(self.renderWindow, self.camera, self.rotationNode)
        self.root.addFrameListener(self.frameListener)

class CelShadingListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, rotationNode):
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.rotationNode = rotationNode

    def frameStarted(self, frameEvent):
        self.rotationNode.yaw(ogre.Radian(ogre.Degree(frameEvent.timeSinceLastFrame * 30)))
        return sf.FrameListener.frameStarted(self, frameEvent)

if __name__ == '__main__':
    try:
        application = CelShadingApplication()
        application.go()
    except ogre.OgreException, e:
        print e
