import os
def header_files( version ):
    ret= [ 'btBulletDynamicsCommon.h'
            ,'btBulletCollisionCommon.h'
            ,'LinearMath/btStackAlloc.h'
            ,'LinearMath/btPoolAllocator.h'
            ,'BulletCollision/CollisionDispatch/btSimulationIslandManager.h'
            ,'BulletCollision/CollisionDispatch/btGhostObject.h'
            ,'BulletDynamics/Character/btCharacterControllerInterface.h'
            ,'BulletDynamics/Character/btKinematicCharacterController.h'

            ,'BulletCollision/BroadphaseCollision/btBroadphaseInterface.h'
            
            ,'BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h'
            ,'BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h'
            
            ,'BulletMultiThreaded/SpuGatheringCollisionDispatcher.h'
            ,'BulletMultiThreaded/PlatformDefinitions.h'
        ]
    if os.sys.platform == 'Darwin':
        ret.append('BulletMultiThreaded/SpuLibspe2Support.h')  ## Don't know about this one ?????
    elif os.name == 'nt':
        ret.append('BulletMultiThreaded/Win32ThreadSupport.h')
        ret.append('BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h')
    elif os.name == 'posix':
        ret.append('BulletMultiThreaded/PosixThreadSupport.h')
        ret.append('BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h')
    return ret
     
def huge_classes( version ):
    return []
   
