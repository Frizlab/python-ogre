import os
def header_files( version ):
    ret = ['python_bullet_masterlist.h'
        ]
    ret_NOTUSED= [ 

                ## Common headers
                'btBulletDynamicsCommon.h'
                ,'btBulletCollisionCommon.h'

                ## COLLISION

                ## broadphase collision
                ,'BulletCollision/BroadphaseCollision/btAxisSweep3.h'
                ,'BulletCollision/BroadphaseCollision/btBroadphaseInterface.h'
                ,'BulletCollision/BroadphaseCollision/btBroadphaseProxy.h'
                ,'BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h'
                ,'BulletCollision/BroadphaseCollision/btDbvt.h'
                ,'BulletCollision/BroadphaseCollision/btDbvtBroadphase.h'
                ,'BulletCollision/BroadphaseCollision/btDispatcher.h'
                ,'BulletCollision/BroadphaseCollision/btMultiSapBroadphase.h'
                ,'BulletCollision/BroadphaseCollision/btOverlappingPairCache.h'
                ,'BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h'
                ,'BulletCollision/BroadphaseCollision/btQuantizedBvh.h'
                ,'BulletCollision/BroadphaseCollision/btSimpleBroadphase.h'

                ## collision dispatch
                ,'BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.h'
                ,'BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.h'
                ,'BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h'
                ,'BulletCollision/CollisionDispatch/btBoxBoxDetector.h'
                ,'BulletCollision/CollisionDispatch/btCollisionConfiguration.h'
                ,'BulletCollision/CollisionDispatch/btCollisionCreateFunc.h'
                ,'BulletCollision/CollisionDispatch/btCollisionDispatcher.h'
                ,'BulletCollision/CollisionDispatch/btCollisionObject.h'
                ,'BulletCollision/CollisionDispatch/btCollisionWorld.h'
                ,'BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.h'
                ,'BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.h'
                ,'BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.h'
                ,'BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h'
                ,'BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.h'
                ,'BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h'
                ,'BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h'
                ,'BulletCollision/CollisionDispatch/btGhostObject.h'
                ,'BulletCollision/CollisionDispatch/btManifoldResult.h'
                ,'BulletCollision/CollisionDispatch/btSimulationIslandManager.h'
                ,'BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h'
                ,'BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h'
                ,'BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h'
                ,'BulletCollision/CollisionDispatch/btUnionFind.h'
                ,'BulletCollision/CollisionDispatch/SphereTriangleDetector.h'

                ## collision shapes
                ,'BulletCollision/CollisionShapes/btBoxShape.h'
                ,'BulletCollision/CollisionShapes/btBox2dShape.h'
                ,'BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h'
                ,'BulletCollision/CollisionShapes/btCapsuleShape.h'
                ,'BulletCollision/CollisionShapes/btCollisionMargin.h'
                ,'BulletCollision/CollisionShapes/btCollisionShape.h'
                ,'BulletCollision/CollisionShapes/btCompoundShape.h'
                ,'BulletCollision/CollisionShapes/btConcaveShape.h'
                ,'BulletCollision/CollisionShapes/btConeShape.h'
                ,'BulletCollision/CollisionShapes/btConvexHullShape.h'
                ,'BulletCollision/CollisionShapes/btConvexInternalShape.h'
                ,'BulletCollision/CollisionShapes/btConvexPointCloudShape.h'
                ,'BulletCollision/CollisionShapes/btConvexShape.h'
                ,'BulletCollision/CollisionShapes/btConvex2dShape.h'
                ,'BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h'
                ,'BulletCollision/CollisionShapes/btCylinderShape.h'
                ,'BulletCollision/CollisionShapes/btEmptyShape.h'
                ,'BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h'
                ,'BulletCollision/CollisionShapes/btMaterial.h'
                ,'BulletCollision/CollisionShapes/btMinkowskiSumShape.h'
                ,'BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.h'
                ,'BulletCollision/CollisionShapes/btMultiSphereShape.h'
                ,'BulletCollision/CollisionShapes/btOptimizedBvh.h'
                ,'BulletCollision/CollisionShapes/btPolyhedralConvexShape.h'
                ,'BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h'
                ,'BulletCollision/CollisionShapes/btShapeHull.h'
                ,'BulletCollision/CollisionShapes/btSphereShape.h'
                ,'BulletCollision/CollisionShapes/btStaticPlaneShape.h'
                ,'BulletCollision/CollisionShapes/btStridingMeshInterface.h'
                ,'BulletCollision/CollisionShapes/btTetrahedronShape.h'
                ,'BulletCollision/CollisionShapes/btTriangleBuffer.h'
                ,'BulletCollision/CollisionShapes/btTriangleCallback.h'
                ,'BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h'
                ,'BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.h'
                ,'BulletCollision/CollisionShapes/btTriangleMesh.h'
                ,'BulletCollision/CollisionShapes/btTriangleMeshShape.h'
                ,'BulletCollision/CollisionShapes/btTriangleShape.h'
                ,'BulletCollision/CollisionShapes/btUniformScalingShape.h'


                ## NarrowPhaseCollision
                ,'BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.h'
                ,'BulletCollision/NarrowPhaseCollision/btConvexCast.h'
                ,'BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h'
                ,'BulletCollision/NarrowPhaseCollision/btDiscreteCollisionDetectorInterface.h'
                ,'BulletCollision/NarrowPhaseCollision/btGjkConvexCast.h'
                ,'BulletCollision/NarrowPhaseCollision/btGjkEpa2.h'
                ,'BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h'
                ,'BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h'
                ,'BulletCollision/NarrowPhaseCollision/btManifoldPoint.h'
                ,'BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h'
                ,'BulletCollision/NarrowPhaseCollision/btPersistentManifold.h'
                ,'BulletCollision/NarrowPhaseCollision/btPointCollector.h'
                ,'BulletCollision/NarrowPhaseCollision/btRaycastCallback.h'
                ,'BulletCollision/NarrowPhaseCollision/btSimplexSolverInterface.h'
                ,'BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.h'
                ,'BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h'

                ## Gimpact
                ,'BulletCollision/Gimpact/btBoxCollision.h'
                ,'BulletCollision/Gimpact/btClipPolygon.h'
                ,'BulletCollision/Gimpact/btContactProcessing.h'
                ,'BulletCollision/Gimpact/btGenericPoolAllocator.h'
                ,'BulletCollision/Gimpact/btGeometryOperations.h'
                ,'BulletCollision/Gimpact/btGImpactBvh.h'
                ,'BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h'
                ,'BulletCollision/Gimpact/btGImpactMassUtil.h'
                ,'BulletCollision/Gimpact/btGImpactQuantizedBvh.h'
                ,'BulletCollision/Gimpact/btGImpactShape.h'
                ,'BulletCollision/Gimpact/btQuantization.h'
                ,'BulletCollision/Gimpact/btTriangleShapeEx.h'

                #,'BulletCollision/Gimpact/gim_array.h'
                #,'BulletCollision/Gimpact/gim_basic_geometry_operations.h'
                #,'BulletCollision/Gimpact/gim_bitset.h'
                #,'BulletCollision/Gimpact/gim_box_collision.h'
                #,'BulletCollision/Gimpact/gim_box_set.h'
                #,'BulletCollision/Gimpact/gim_clip_polygon.h'
                #,'BulletCollision/Gimpact/gim_contact.h' # previous decl class GIM_CONTACT
                #,'BulletCollision/Gimpact/gim_geom_types.h'
                #,'BulletCollision/Gimpact/gim_geometry.h'
                #,'BulletCollision/Gimpact/gim_hash_table.h'
                #,'BulletCollision/Gimpact/gim_linear_math.h'
                #,'BulletCollision/Gimpact/gim_math.h'
                #,'BulletCollision/Gimpact/gim_memory.h'
                #,'BulletCollision/Gimpact/gim_radixsort.h'
                #,'BulletCollision/Gimpact/gim_tri_collision.h'


                ## DYNAMICS

                ##ConstraintSolver
                ,'BulletDynamics/ConstraintSolver/btConeTwistConstraint.h'
                ,'BulletDynamics/ConstraintSolver/btConstraintSolver.h'
                ,'BulletDynamics/ConstraintSolver/btContactConstraint.h'
                ,'BulletDynamics/ConstraintSolver/btContactSolverInfo.h'
                ,'BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h'
                ,'BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h'
                ,'BulletDynamics/ConstraintSolver/btHinge2Constraint.h'
                ,'BulletDynamics/ConstraintSolver/btHingeConstraint.h'
                ,'BulletDynamics/ConstraintSolver/btJacobianEntry.h'
                ,'BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h'
                ,'BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h'
                ,'BulletDynamics/ConstraintSolver/btSliderConstraint.h'
                ,'BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.h'
                ,'BulletDynamics/ConstraintSolver/btSolverBody.h'
                ,'BulletDynamics/ConstraintSolver/btSolverConstraint.h'
                ,'BulletDynamics/ConstraintSolver/btTypedConstraint.h'
                ,'BulletDynamics/ConstraintSolver/btUniversalConstraint.h'


                ##Dynamics
                ,'BulletDynamics/Dynamics/btActionInterface.h'
                ,'BulletDynamics/Dynamics/btContinuousDynamicsWorld.h'
                ,'BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h'
                ,'BulletDynamics/Dynamics/btDynamicsWorld.h'
                ,'BulletDynamics/Dynamics/btSimpleDynamicsWorld.h'
                ,'BulletDynamics/Dynamics/btRigidBody.h'


                ##Vehicle
                ,'BulletDynamics/Vehicle/btRaycastVehicle.h'
                ,'BulletDynamics/Vehicle/btVehicleRaycaster.h'
                ,'BulletDynamics/Vehicle/btWheelInfo.h'

                ##Character
                ,'BulletDynamics/Character/btCharacterControllerInterface.h'
                ,'BulletDynamics/Character/btKinematicCharacterController.h'


                ## MATHS
                ,'LinearMath/btStackAlloc.h'
                ,'LinearMath/btPoolAllocator.h'

                ## THREADING
                ,'BulletMultiThreaded/SpuGatheringCollisionDispatcher.h'
                ,'BulletMultiThreaded/PlatformDefinitions.h'
        ]
    if False:     
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
   
