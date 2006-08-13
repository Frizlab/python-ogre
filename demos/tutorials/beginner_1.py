# this code is in the public domain
# http://www.ogre3d.org/wiki/index.php/PyOgre_Beginner_Tutorial_1
#from ogre import Ogre
import Ogre as ogre
import SampleFramework

class TutorialApplication(SampleFramework.Application):
    def _createScene(self):
        # first edit
        sceneManager = self.sceneManager
        sceneManager.ambientLight = ogre.ColourValue(1, 1, 1)

        ent1 = sceneManager.createEntity("Robot", "robot.mesh")
        
        node1 = sceneManager
        node1= node1.getRootSceneNode().createChildSceneNode("RobotNode") 
        node1.attachObject(ent1)

        ent2 = sceneManager.createEntity("Robot2", "robot.mesh")
        node2 = sceneManager.getRootSceneNode().createChildSceneNode("RobotNode2", ogre.Vector3(50, 0, 0))
        node2.attachObject(ent2)


##        # second edit
##        sceneManager = self.sceneManager
##        sceneManager.ambientLight = (1, 1, 1)
##
##        ent1 = sceneManager.createEntity("Robot", "robot.mesh")
##        node1 = sceneManager.rootSceneNode.createChildSceneNode("RobotNode")
##        node1.attachObject(ent1)
##
##        ent2 = sceneManager.createEntity("Robot2", "robot.mesh")
##        node2 = sceneManager.rootSceneNode.createChildSceneNode("RobotNode2", (50, 0, 0))
##        node2.attachObject(ent2)
##
##
##        # third edit (scale)
##       sceneManager = self.sceneManager
##       sceneManager.ambientLight = (1, 1, 1)
##x
##       ent = sceneManager.createEntity("Robot", "robot.mesh")
##       node = sceneManager.rootSceneNode.createChildSceneNode("RobotNode")
##       node.attachObject(ent)
##       node.scale = (.5, 1, 2)
##       
##       ent = sceneManager.createEntity("Robot2", "robot.mesh")
##       node = sceneManager.rootSceneNode.createChildSceneNode("RobotNode2", (50, 0, 0))
##       node.attachObject(ent)
##       node.scale = (0.5, 0.2, 1)
##       node.scaleBy(2, 10, 1)     # these two lines same as node.scale = (1, 2, 1)##
##
##        # fourth edit (rotation)
##        sceneManager = self.sceneManager
##        sceneManager.ambientLight = (1, 1, 1)
##
##        ent1 = sceneManager.createEntity("Robot", "robot.mesh")
##        node1 = sceneManager.rootSceneNode.createChildSceneNode("RobotNode")
##        node1.attachObject(ent1)
##        node1.yaw(ogre.Degree(-90))
##
##        ent2 = sceneManager.createEntity("Robot2", "robot.mesh")
##        node2 = sceneManager.rootSceneNode.createChildSceneNode("RobotNode2", ogre.Vector3(50, 0, 0))
##        node2.attachObject(ent2)
##        node2.pitch(ogre.Degree(-90))
##
##        ent3 = sceneManager.createEntity("Robot3", "robot.mesh")
##        node3 = sceneManager.rootSceneNode.createChildSceneNode("RobotNode3", ogre.Vector3(100, 0, 0))
##        node3.attachObject(ent3)
##        node3.roll(ogre.Degree(-90))

        

if __name__ == '__main__':
    ta = TutorialApplication()
    ta.go()
