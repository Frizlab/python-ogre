import sys 
sys.path.insert(0,'..') 
import PythonOgreConfig 

import ogre.renderer.OGRE as ogre 
import ogre.io.OIS as OIS 
import SampleFramework as sf 
  
class TutorialApplication(sf.Application): 

   def _createScene(self): 
      sceneManager = self.sceneManager 
      sceneManager.ambientLight = ogre.ColourValue (1, 1, 1) 

      ent1 = sceneManager.createEntity ("Robot", "robot.mesh") 
      node1 = sceneManager.getRootSceneNode().createChildSceneNode("RobotNode") 
      node1.attachObject(ent1) 

if __name__ == '__main__': 
    import exceptions 
    try: 
        application = TutorialApplication() 
        application.go() 
    except ogre.OgreException, e: 
        print e