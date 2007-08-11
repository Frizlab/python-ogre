"""

"""

import ogre.renderer.OGRE as ogre
import ogre.physics.PhysX as physx
import ogre.physics.NxOgre as nxogre
import CakeFramework as cf
    
class NxTutorial ( cf.Cake ):
   def __init__ ( self ):
        cf.Cake.__init__(self)
   
   ##////////////////////////////////////////////////////////////////////////////////////////////////

   def start(self ):
      self.World = nxogre.World("Log: html") 
      
      self.Scene = self.World.createScene("Main", self.SceneMgr,"gravity: yes, floor: yes") 
      
      self.Scene.createBody("capsule.50cmx1m.mesh",nxogre.CapsuleShape(0.5,1),  ogre.Vector3(0,2,0), "mass: 10" )

      self.myCuboid= self.Scene.createBody("", 
            nxogre.CubeShape(1,2,1), 
                nxogre.Pose(
                    ogre.Vector3(4,0.5,0),                          ## Offset it.
                    ogre.Quaternion(0.92388, -0.382683, -0, -0)     ## Rotate it (-45 Degrees on the X axis)
                    )
                ,
            "mass: 10"
        )

#       print dir( self.myCuboid )
#       self.myCuboid.addShape("cube.1m-2m-1m.mesh") #, 
# #             nxogre.Pose(                                                
# #                 ogre.Vector3(4,0.5,0),
# #                 ogre.Quaternion(0.92388, -0.382683, -0, -0)
# #             )
# #         )

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

