## ===============================================================================
##						  AGEIA PHYSX SDK TRAINING PROGRAMS
##			                  LESSON 701: WHEEL SHAPES
##
##						    Written by Bob Schade, 5-1-06
## ===============================================================================



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
      
      self.Scene.createBody('sphere.50cm.mesh',nxogre.SphereShape(0.5),
                    ogre.Vector3(3,1.5,0), "mass: 10" )
                    

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

        

# # # def PrintControls():
# # # 
# # # 	print("\n Flight Controls:\n ----------------\n w = forward, s = back\n a = strafe left, d = strafe right\n q = up, z = down\n") 
# # #     print("\n Force Controls:\n ---------------\n i = +z, k = -z\n j = +x, l = -x\n u = +y, m = -y\n") 
# # # 	print("\n Miscellaneous:\n --------------\n p = Pause\n r = Select Next Actor\n f = Toggle Force Mode\n b = Toggle Debug Wireframe Mode\n x = Toggle Shadows\n t = Move Focus Actor to (0,5,0)\n") 
# # # 
# # # 
# # # def RenderActors(bool shadows)
# # # 
# # #     ## Render all the actors in the scene
# # #     NxU32 nbActors = self.Scene.getNbActors() 
# # #     NxActor** actors = self.Scene.getActors() 
# # #     while (nbActors--)
# # # 
# # #         NxActor* actor = *actors++ 
# # # 	    DrawActor(actor, self.SelectedActor, False) 
# # # 
# # #         ## Handle shadows
# # #         if (shadows):
# # # 
# # # 			DrawActorShadow(actor, False) 
# # # 



def UpdateWheelShapeUserData(self):
    ## Look for wheel shapes
    nbActors = self.Scene.getNbActors() 
    actors = self.Scene.getActors() 
    while nbActors > 0:
        nbActors -= 1
		nbShapes = actors[nbActors].getNbShapes() 
	    shapes = actors[nbActors].getShapes() 
		while nbShapes > 0:
		    nbShapes -= 1

			shape = shapes[nbShapes] 
			if (shape.getType() == nxogre.NX_SHAPE_WHEEL):

# 				NxWheelShape* ws = (NxWheelShape*)shape 
# 				ShapeUserData* sud = (ShapeUserData*)(shape.userData) 
				ws = shape 
				sud = shape.userData
				if sud:
					## Need to save away roll angle in wheel shape user data
					rollAngle = sud.wheelShapeRollAngle 
##					rollAngle += ws.getAxleSpeed() * 1.0/60.0f 
					rollAngle += ws.getAxleSpeed() * gDeltaTime 
					while (rollAngle > nxogre.NxTwoPi)	##normally just 1x
						rollAngle -= nxogre.NxTwoPi 
					while (rollAngle < -nxogre.NxTwoPi)	##normally just 1x
						rollAngle += nxogre.NxTwoPi 

					## We have the roll angle for the wheel now
					sud.wheelShapeRollAngle = rollAngle 


					pose = ws.getGlobalPose() 

					wcd = nxogre.NxWheelContactData ()
					s = ws.getContact(wcd) 	

					r = ws.getRadius() 
					st = ws.getSuspensionTravel() 
					steerAngle = ws.getSteerAngle() 

##					NxWheelShapeDesc state 	
##					ws.saveToDesc(state) 

					dir_ = NxVec3 () 
# # 					/*
# # 					getWorldSegmentFast(seg) 
# # 					seg.computeDirection(dir) 
# # 					dir.normalize() 
# # 					*/
					p0 = pose.t   ##cast from shape origin
					pose.M.getColumn(1, dir_) 
					dir_ = -dir_	##cast along -Y.
					castLength = r + st 	##cast ray self long

##					renderer.addArrow(p0, dir, castLength, 1.0) 
	
					##have ground contact?
					## This code is from WheelShape.cpp in SDKs/core/common/src
					## if (contactPosition != nxogre.NX_MAX_REAL)  :
					if s and (wcd.contactForce > -1000) :

##						pose.t = p0 + dir * wcd.contactPoint 
##						pose.t -= dir * state.radius 	//go from contact pos to center pos.
						pose.t = wcd.contactPoint 
						pose.t -= dir_ * r 	##go from contact pos to center pos.

						rot = nxogre.NxMat33()
						axisRot = nxogre.NxMat33 ()
						rot.rotY(steerAngle) 
						axisRot.rotY(0) 

##						NxReal rollAngle = ((ShapeUserData*)(wheel.userData)).rollAngle 

						rollRot = nxogre.NxMat33 ()
						rollRot.rotX(rollAngle) 

						pose.M = rot * pose.M * axisRot * rollRot 

						sud.wheelShapePose = pose 


					else:

						pose.t = p0 + dir * st 
						sud.wheelShapePose = pose 



def AddWheelToActor(self, actor, wheelDesc):

	wheelShapeDesc = nxogre.NxWheelShapeDesc ()

	## Create a shared car wheel material to be used by all wheels
	if (!self.wsm):

		m = nxogre.NxMaterialDesc ()
		m.flags |= nxogre.NX_MF_DISABLE_FRICTION 
		self.wsm = self.Scene.createMaterial(m) 

	wheelShapeDesc.materialIndex = self.wsm.getMaterialIndex() 

	wheelShapeDesc.localPose.t = wheelDesc.position 
	q = nxogre.NxQuat ()
	q.fromAngleAxis(90, nxogre.NxVec3(0,1,0)) 
	wheelShapeDesc.localPose.M.fromQuat(q) 

	heightModifier = (wheelDesc.wheelSuspension + wheelDesc.wheelRadius) / wheelDesc.wheelSuspension 

	wheelShapeDesc.suspension.spring = wheelDesc.springRestitution*heightModifier 
	wheelShapeDesc.suspension.damper = 0   ##wheelDesc.springDamping*heightModifier;
	wheelShapeDesc.suspension.targetValue = wheelDesc.springBias*heightModifier 

	wheelShapeDesc.radius = wheelDesc.wheelRadius 
	wheelShapeDesc.suspensionTravel = wheelDesc.wheelSuspension  
	wheelShapeDesc.inverseWheelMass = 0.1 	##not given!? TODO

	## wheelShapeDesc.lateralTireForceFunction =  	//TODO
	## wheelShapeDesc.longitudalTireForceFunction =  	//TODO

# #     NxWheelShape* wheelShape 
# # 	wheelShape = static_cast<NxWheelShape *>(actor.createShape(wheelShapeDesc)) 


def CreateCab( self, pos):
    """ create the cab from a box """
	actor = self.CreateBox(pos + nxogre.NxVec3(0,1,0), nxogre.NxVec3(1,0.5,0.5), 10) 
	actor.raiseBodyFlag(nxogre.NX_BF_FROZEN_ROT_X) 
	actor.raiseBodyFlag(nxogre.NX_BF_FROZEN_ROT_Z) 
	actor.setAngularDamping(0.5) 

	wheelDesc  =nxogre.NxWheelDesc
	##wheelDesc.wheelAxis.set(0,0,1) 
	##wheelDesc.downAxis.set(0,-1,0) 
    wheelDesc.wheelApproximation = 10 
	wheelDesc.wheelRadius = 0.5 
	wheelDesc.wheelWidth = 0.1 
	wheelDesc.wheelSuspension = 0.2 
	wheelDesc.springRestitution = 7000 
	wheelDesc.springDamping = 800 
	wheelDesc.springBias = 0 
	wheelDesc.maxBrakeForce = 1 
	wheelDesc.frictionToFront = 0.1 
	wheelDesc.frictionToSide = 0.99 
	wheelDesc.position = nxogre.NxVec3(0,-0.4,0) 
	wheelDesc.wheelFlags = nxogre.NX_WF_USE_WHEELSHAPE | nxogre.NX_WF_BUILD_LOWER_HALF | nxogre.NX_WF_ACCELERATED | 
		                   nxogre.NX_WF_AFFECTED_BY_HANDBRAKE | nxogre.NX_WF_STEERABLE_INPUT | nxogre.NX_WF_BUILD_LOWER_HALF 
    self.AddWheelToActor(actor, wheelDesc) 
	return actor 


def InitNx()

	## Create a memory allocator
    gAllocator = new UserAllocator 

    ## Create the physics SDK
    self.gPhysicsSDK = NxCreatePhysicsSDK(nxogre.NX_PHYSICS_SDK_VERSION, gAllocator) 
	if (!self.gPhysicsSDK)  return :

	## Set the physics parameters
	self.gPhysicsSDK.setParameter(nxogre.NX_SKIN_WIDTH, 0.05) 

	## Set the debug visualization parameters
	self.gPhysicsSDK.setParameter(nxogre.NX_VISUALIZATION_SCALE, 1) 
	self.gPhysicsSDK.setParameter(nxogre.NX_VISUALIZE_ACTOR_AXES, 1) 	
	self.gPhysicsSDK.setParameter(nxogre.NX_VISUALIZE_COLLISION_SHAPES, 1) 

	self.gPhysicsSDK.setParameter(nxogre.NX_VISUALIZE_CONTACT_POINT, 1) 
	self.gPhysicsSDK.setParameter(nxogre.NX_VISUALIZE_CONTACT_NORMAL, 1) 

    ## Create the scene
    sceneDesc = nxogre.NxSceneDesc ()
    sceneDesc.gravity = gDefaultGravity 
	sceneDesc.simType = nxogre.NX_SIMULATION_HW 
	self.Scene = self.gPhysicsSDK.createScene(sceneDesc) 	
 if(!self.Scene){ :
		sceneDesc.simType				= nxogre.NX_SIMULATION_SW  
		self.Scene = self.gPhysicsSDK.createScene(sceneDesc)   
		if(!self.Scene) return :


	## Create the default material
	NxMaterial* defaultMaterial = self.Scene.getMaterialFromIndex(0)  
	defaultMaterial.setRestitution(0.5) 
	defaultMaterial.setStaticFriction(0.5) 
	defaultMaterial.setDynamicFriction(0.5) 

	## Create the objects in the scene
	groundPlane = CreateGroundPlane() 

	self.box = CreateBox(NxVec3(5,0,0), NxVec3(0.5,1,0.5), 20) 
	self.sphere = CreateSphere(NxVec3(0,0,5), 1, 10) 
	self.capsule = CreateCapsule(NxVec3(-5,0,0), 2, 0.5, 10) 

	## Reset wheel material
	self.wsm = None 

	self.cab = CreateCab(NxVec3(0,0,0)) 

	self.SelectedActor = cab 
	self.gForceStrength = 35000 

	## Turn on all contact notifications
	self.Scene.setActorGroupPairFlags(0, 0, NX_NOTIFY_ON_TOUCH) 

    AddUserDataToActors(self.Scene) 

	## Initialize HUD
	InitializeHUD() 

	## Get the current time
	UpdateTime() 

	## Start the first frame of the simulation
	if (self.Scene)  StartPhysics() :


int main(int argc, char** argv)

	PrintControls() 
	InitGlut(argc, argv, "Lesson 701: Wheel Shapes") 
    InitNx() 
    glutMainLoop() 
	ReleaseNx() 
	return 0 




