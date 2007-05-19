import ogre.renderer.OGRE as ogre
import ogre.io.OIS as ogreio
import SampleFramework

import ode

import lasseworld


class TutorialFrameListener(SampleFramework.FrameListener):
    def __init__(self, renderWindow, camera, sceneManager, world):
        SampleFramework.FrameListener.__init__(self, renderWindow, camera)
        self.mouseDown = False
        self.sceneManager = sceneManager
        self.world = world

    def frameStarted(self, evt):
        ms = self.Mouse.getMouseState()
        currMouse = ms.buttonDown(ogreio.MB_Left)
        if currMouse and not self.mouseDown:
           light = self.sceneManager.getLight("Light1")
           light.visible = not light.visible
        self.mouseDown = currMouse
        self.world.advanceFrame(evt.timeSinceLastFrame)
        return SampleFramework.FrameListener.frameStarted(self, evt)



class TutorialApplication(SampleFramework.Application):
    def _createScene(self):
        self.world = lasseworld.World(self.sceneManager)
        self.world.setupObjects()
        
        sceneManager = self.sceneManager
        sceneManager.ambientLight = (0.25, 0.3, 0.35)
        sceneManager.shadowTechnique = ogre.SHADOWTYPE_STENCIL_ADDITIVE

        for i in range(0):
            for j in range(0):
                name = str(i) + "_" + str(j)
                ent = sceneManager.createEntity("Robot_" + name, "robot.mesh")
                node = sceneManager.rootSceneNode.createChildSceneNode("RobotNode_" + name, (25*i, 0, 25*j))
                node.attachObject(ent)

 
        light = self.sceneManager.createLight("Light1")
        light.type = ogre.Light.LT_POINT
        light.position = (0, 150, 250)
        light.diffuseColour = (1.0, 0.8, 0.8)
        light.specularColour = (1.0, 0.2, 0.2)



    def _createCamera(self):
        print self
        self.camera = self.sceneManager.createCamera("PlayerCam")
        self.camera.position = (-200, 50, -200)
        self.camera.lookAt((0,0,0))
        self.camera.nearClipDistance = 5


    def _createFrameListener(self):
        self.frameListener = TutorialFrameListener(self.renderWindow, self.camera, self.sceneManager, self.world)
        self.root.addFrameListener(self.frameListener)



if __name__ == '__main__':
    ta = TutorialApplication()
    ta.go()
