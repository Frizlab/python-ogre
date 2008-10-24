##
## Simple test code for Physx
##
import ogre.physics.PhysX as physics
import sys

class  MyOutputStream ( physics.NxUserOutputStream ):
    def __init__( self):
#         return
        physics.NxUserOutputStream.__init__( self )
        
    def reportError (self, code, message,  file, line):
    
        if code < physics.NXE_DB_INFO:
            MessageBox(NULL, message, "SDK Error", MB_OK),
            sys.exit(1)
            
    def reportAssertViolation (self, message, file, line):
#         //this should not get hit by
#         // a properly debugged SDK!
#         assert(0);
        return physics.NX_AR_CONTINUE
            
    def print1 (self,message):
        print "SDK says: %s\n", message
 
# myOutputStream = MyOutputStream ()   

# print myOutputStream     
               
def InitNx ( ) :

    gDefaultGravity = physics.NxVec3(0.0,-9.8,0.0)
    gCookingInitialized = False
    
    # Initialize PhysicsSDK
    desc = physics.NxPhysicsSDKDesc()
    
    gPhysicsSDK, errorCode = physics.NxCreatePhysicsSDK(physics.NX_PHYSICS_SDK_VERSION(),desc=desc) 
    if gPhysicsSDK == None : 
        print "\nSDK create error (%d - %s).\nUnable to initialize the PhysX SDK, exiting the sample.\n\n" % (errorCode,"test")
        return False
    

# #if SAMPLES_USE_VRD
#   # The settings for the VRD host and port are found in SampleCommonCode/SamplesVRDSettings.h
#   if (gPhysicsSDK.getFoundationSDK().getRemoteDebugger() && !gPhysicsSDK.getFoundationSDK().getRemoteDebugger().isConnected())
#       gPhysicsSDK.getFoundationSDK().getRemoteDebugger().connect(SAMPLES_VRD_HOST, SAMPLES_VRD_PORT, SAMPLES_VRD_EVENTMASK)
# #endif

    if not gCookingInitialized:
        gCookingInitialized = True
        if not physics.InitCooking():
            print "\nError: Unable to initialize the cooking library, exiting the sample.\n\n"
            return False
            
    # Set the physics parameters
    gPhysicsSDK.setParameter(physics.NX_SKIN_WIDTH, -0.01)

    # Set the debug visualization parameters
    gPhysicsSDK.setParameter(physics.NX_VISUALIZATION_SCALE, 1)
    gPhysicsSDK.setParameter(physics.NX_VISUALIZE_COLLISION_SHAPES, 1)
    gPhysicsSDK.setParameter(physics.NX_VISUALIZE_JOINT_LIMITS, 1)
    gPhysicsSDK.setParameter(physics.NX_VISUALIZE_JOINT_LOCAL_AXES, 1)


    # Create the scene
    sceneDesc = physics.NxSceneDesc()
    sceneDesc.gravity = gDefaultGravity
    gScene = gPhysicsSDK.createScene(sceneDesc)

    # Create the default material
    m = physic.NxMaterialDesc ()
    m.restitution        = 0.5
    m.staticFriction     = 0.2
    m.dynamicFriction    = 0.2
    mat = gScene.getMaterialFromIndex(0)
    mat.loadFromDesc(m) 

#     switch(gSceneNr) {
#         case 1 : SetupAnimalScene() break
#         case 2 : SetupWheelScene() break
#         case 3 : SetupPalmScene() break
#         case 4 : SetupGrassScene() break
#         case 5 : SetupRegularScene() break
#         case 6 : SetupBunnyScene() break
#         case 7 : SetupPumpkinScene() break
# 
#         # Insert new cases here
#     }
# 
#     if (gSoftBodies.size() > 0)
#         gSelectedSoftBody = gSoftBodies[0].getNxSoftBody()
#     else
#         gSelectedSoftBody = None
# 
#     if (gScene.getNbActors() > 0)
#         gSelectedActor = *gScene.getActors()
#     else
#         gSelectedActor = None
# 
#     UpdateTime()

    return True

if __name__ == "__main__":
   print InitNx()    