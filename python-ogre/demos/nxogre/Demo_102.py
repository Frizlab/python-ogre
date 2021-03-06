"""

"""
import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.physics.PhysX as physx
import ogre.physics.NxOgre_09 as nxogre
import CakeFramework as cf
    
class NxTutorial ( cf.Cake ):
   def __init__ ( self ):
        cf.Cake.__init__(self)
   
   ##////////////////////////////////////////////////////////////////////////////////////////////////

   def start(self ):
      self.World = nxogre.World("Log: html") 
      
      self.Scene = self.World.createScene("Main", self.SceneMgr,"gravity: yes, floor: yes") 
      
      self.Scene.createBody('sphere.50cm.mesh',nxogre.SphereShape(0.5),
                    ogre.Vector3(3,1.5,0), "mass: 10" )
                    
      pose = nxogre.Pose(ogre.Vector3(-3,0.5,3), nxogre.Pose(ogre.Vector3(-3,0.5,3))) #,arg1=quat)
      self.Scene.createBody("cube.1m.mesh", nxogre.CubeShape(1), pose,  "mass: 1" )
      ## That's it!


   ##////////////////////////////////////////////////////////////////////////////////////////////////
   def stop(self):
      ## Time to go, better tell NxOgre we are leaving.
      del self.World 

   ##////////////////////////////////////////////////////////////////////////////////////////////////
   
   def onFrame(self, _time) :
      if not self.mTargetActor:
         return 
      if self.InputHandler.isKeyDown(cf.Action.Z):
         self.mTargetActor.addTorque(ogre.Vector3(0,0,150)) 
      if self.InputHandler.isKeyDown(cf.Action.NEG_Z):
         self.mTargetActor.addTorque(ogre.Vector3(0,0,-150)) 
      if self.InputHandler.isKeyDown(cf.Action.X):
         self.mTargetActor.addTorque(ogre.Vector3(150,0,0)) 
      if self.InputHandler.isKeyDown(cf.Action.NEG_X):
         self.mTargetActor.addTorque(ogre.Vector3(-150,0,0)) 
      if self.InputHandler.isKeyDown(cf.Action.Y):
         self.mTargetActor.addTorque(ogre.Vector3(0,150,0)) 
      if self.InputHandler.isKeyDown(cf.Action.NEG_Y):
         self.mTargetActor.addTorque(ogre.Vector3(0,-150,0)) 
         
   def getWorld(self):
        return self.World
   ##////////////////////////////////////////////////////////////////////////////////////////////////

   def getTutorialSettings( self ):
      self.TutorialName = "101" 
      self.TutorialDescription = "Cube on a plane" 

   ##////////////////////////////////////////////////////////////////////////////////////////////////

if __name__ == '__main__':
    import exceptions,sys
    try:
        application = NxTutorial ()
        application.pre()
        application.start()
        application.WorldInstance = application.getWorld()
        application.startRendering(application)
        application.stop()
        application.post()

    except ogre.OgreException, e:
        print e

