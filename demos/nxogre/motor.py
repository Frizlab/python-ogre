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
      
      pose = nxogre.Pose(ogre.Vector3(0,1,0), ogre.Quaternion(0,0,0,0)) #,arg1=quat)
      self.bodymesh = nxogre.TriangleMeshShape ("racecar.mesh",  "mass: 20" )

      self.cab = self.Scene.createBody("racecar.mesh", self.bodymesh, pose,  "mass: 10" )
      self.cab.setAngularDamping(0.7)
      
      wheelmeshname = "wheel50cmx10cmx50cm.mesh"
                    
      self.wheelset = nxogre.WheelSet().createFourWheelSet ( self.cab, 
                    frontLeft = (0.5,.6,0.8), 
                    backLeft=(0.5,.6,-1),
                    radius = 0.5 )
      self.wheels = self.wheelset.mWheels

            
      print "\n\n", self.wheels,"\n",\
                dir(self.wheels),"\n",\
                self.wheels.count(),"\n",\
                self.wheels.items
      print self.wheels.items
      print dir(self.wheels.items)
      for i in self.wheels.items:
        i.springRestitution = 7000000
        i.springDamping = 800
        i.springBias = 10000
      print dir ( i)
#  wheelDesc.wheelApproximation = 10;
# 	wheelDesc.wheelRadius = 0.5;
# 	wheelDesc.wheelWidth = 0.1;
# 	wheelDesc.wheelSuspension = 0.2;
# 	wheelDesc.springRestitution = 7000;
# 	wheelDesc.springDamping = 800;
# 	wheelDesc.springBias = 0;
# 	wheelDesc.maxBrakeForce = 1;
# 	wheelDesc.frictionToFront = 0.1;
# 	wheelDesc.frictionToSide = 0.99;
# 	wheelDesc.position = NxVec3(0,-0.4,0);
# 	wheelDesc.wheelFlags = NX_WF_USE_WHEELSHAPE | NX_WF_BUILD_LOWER_HALF | NX_WF_ACCELERATED | 
# 		                   NX_WF_AFFECTED_BY_HANDBRAKE | NX_WF_STEERABLE_INPUT | NX_WF_BUILD_LOWER_HALF;        
      
#       self.driveshaft = nxogre.DriveShaft ( "driveshaft", self.Scene, self.wheels )
#       
      self.wheelset.addMeshes (wheelmeshname )
#       self.wheelset.attachDriveShaft ( self.driveshaft )
            
      self.motor = nxogre.Motor ("motor", self.Scene, self.wheels )

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

