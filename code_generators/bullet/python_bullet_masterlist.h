
//#undef _MSC_VER

// Common headers
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

// COLLISION

// broadphase collision
#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.h"
#include "BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h"
#include "BulletCollision/BroadphaseCollision/btDbvt.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletCollision/BroadphaseCollision/btDispatcher.h"
#include "BulletCollision/BroadphaseCollision/btMultiSapBroadphase.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCache.h"
#include "BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h"
#include "BulletCollision/BroadphaseCollision/btQuantizedBvh.h"
#include "BulletCollision/BroadphaseCollision/btSimpleBroadphase.h"

// collision dispatch
#include "BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btBoxBoxDetector.h"
#include "BulletCollision/CollisionDispatch/btCollisionConfiguration.h"
#include "BulletCollision/CollisionDispatch/btCollisionCreateFunc.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/CollisionDispatch/btManifoldResult.h"
#include "BulletCollision/CollisionDispatch/btSimulationIslandManager.h"
#include "BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btUnionFind.h"
#include "BulletCollision/CollisionDispatch/SphereTriangleDetector.h"

// collision shapes
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btBox2dShape.h"
#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/CollisionShapes/btCollisionMargin.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/CollisionShapes/btConcaveShape.h"
#include "BulletCollision/CollisionShapes/btConeShape.h"
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "BulletCollision/CollisionShapes/btConvexInternalShape.h"
#include "BulletCollision/CollisionShapes/btConvexPointCloudShape.h"
#include "BulletCollision/CollisionShapes/btConvexShape.h"
#include "BulletCollision/CollisionShapes/btConvex2dShape.h"
#include "BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btCylinderShape.h"
#include "BulletCollision/CollisionShapes/btEmptyShape.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "BulletCollision/CollisionShapes/btMaterial.h"
#include "BulletCollision/CollisionShapes/btMinkowskiSumShape.h"
#include "BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "BulletCollision/CollisionShapes/btOptimizedBvh.h"
#include "BulletCollision/CollisionShapes/btPolyhedralConvexShape.h"
#include "BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "BulletCollision/CollisionShapes/btStridingMeshInterface.h"
#include "BulletCollision/CollisionShapes/btTetrahedronShape.h"
#include "BulletCollision/CollisionShapes/btTriangleBuffer.h"
#include "BulletCollision/CollisionShapes/btTriangleCallback.h"
#include "BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"
#include "BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.h"
#include "BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btTriangleShape.h"
#include "BulletCollision/CollisionShapes/btUniformScalingShape.h"


// NarrowPhaseCollision
#include "BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.h"
#include "BulletCollision/NarrowPhaseCollision/btConvexCast.h"
#include "BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btDiscreteCollisionDetectorInterface.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkConvexCast.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkEpa2.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h"
#include "BulletCollision/NarrowPhaseCollision/btManifoldPoint.h"
#include "BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h"
#include "BulletCollision/NarrowPhaseCollision/btPersistentManifold.h"
#include "BulletCollision/NarrowPhaseCollision/btPointCollector.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "BulletCollision/NarrowPhaseCollision/btSimplexSolverInterface.h"
#include "BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.h"
#include "BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h"

//Gimpact
#include "BulletCollision/Gimpact/btBoxCollision.h"
#include "BulletCollision/Gimpact/btClipPolygon.h"
#include "BulletCollision/Gimpact/btContactProcessing.h"
#include "BulletCollision/Gimpact/btGenericPoolAllocator.h"
#include "BulletCollision/Gimpact/btGeometryOperations.h"
#include "BulletCollision/Gimpact/btGImpactBvh.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#include "BulletCollision/Gimpact/btGImpactMassUtil.h"
#include "BulletCollision/Gimpact/btGImpactQuantizedBvh.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/Gimpact/btQuantization.h"
#include "BulletCollision/Gimpact/btTriangleShapeEx.h"
/*
#include "BulletCollision/Gimpact/gim_array.h"
#include "BulletCollision/Gimpact/gim_basic_geometry_operations.h"
#include "BulletCollision/Gimpact/gim_bitset.h"
//#include "BulletCollision/Gimpact/gim_box_collision.h"
#include "BulletCollision/Gimpact/gim_box_set.h"
#include "BulletCollision/Gimpact/gim_clip_polygon.h"
// previous decl class GIM_CONTACT
//#include "BulletCollision/Gimpact/gim_contact.h"
#include "BulletCollision/Gimpact/gim_geom_types.h"
#include "BulletCollision/Gimpact/gim_geometry.h"
#include "BulletCollision/Gimpact/gim_hash_table.h"
#include "BulletCollision/Gimpact/gim_linear_math.h"
#include "BulletCollision/Gimpact/gim_math.h"
#include "BulletCollision/Gimpact/gim_memory.h"
#include "BulletCollision/Gimpact/gim_radixsort.h"
#include "BulletCollision/Gimpact/gim_tri_collision.h"
*/

// DYNAMICS

//ConstraintSolver
#include "BulletDynamics/ConstraintSolver/btConeTwistConstraint.h"
#include "BulletDynamics/ConstraintSolver/btConstraintSolver.h"
#include "BulletDynamics/ConstraintSolver/btContactConstraint.h"
#include "BulletDynamics/ConstraintSolver/btContactSolverInfo.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h"
#include "BulletDynamics/ConstraintSolver/btHinge2Constraint.h"
#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"
#include "BulletDynamics/ConstraintSolver/btJacobianEntry.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "BulletDynamics/ConstraintSolver/btSliderConstraint.h"
#include "BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.h"
#include "BulletDynamics/ConstraintSolver/btSolverBody.h"
#include "BulletDynamics/ConstraintSolver/btSolverConstraint.h"
#include "BulletDynamics/ConstraintSolver/btTypedConstraint.h"
#include "BulletDynamics/ConstraintSolver/btUniversalConstraint.h"


//Dynamics
#include "BulletDynamics/Dynamics/btActionInterface.h"
#include "BulletDynamics/Dynamics/btContinuousDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btSimpleDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"


//Vehicle
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"
#include "BulletDynamics/Vehicle/btVehicleRaycaster.h"
#include "BulletDynamics/Vehicle/btWheelInfo.h"

//Character
#include "BulletDynamics/Character/btCharacterControllerInterface.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"


// MATHS
#include "LinearMath/btStackAlloc.h"
#include "LinearMath/btPoolAllocator.h"

//#include "LinearMath/btAabbUtil2.h"
//#include "LinearMath/btAlignedAllocator.h"
//#include "LinearMath/btAlignedObjectArray.h"
//#include "LinearMath/btConvexHull.h"
//#include "LinearMath/btDefaultMotionState.h"
//#include "LinearMath/btGeometryUtil.h"
//#include "LinearMath/btHashMap.h"
//#include "LinearMath/btIDebugDraw.h"
//#include "LinearMath/btList.h"
//#include "LinearMath/btMatrix3x3.h"
//#include "LinearMath/btMinMax.h"
//#include "LinearMath/btMotionState.h"
//#include "LinearMath/btQuadWord.h"
//#include "LinearMath/btQuaternion.h"
//#include "LinearMath/btQuickprof.h"
//#include "LinearMath/btRandom.h"
//#include "LinearMath/btScalar.h"
//#include "LinearMath/btStackAlloc.h"
//#include "LinearMath/btTransform.h"
///#include "LinearMath/btTransformUtil.h"
//#include "LinearMath/btVector3.h"


//THREADING

//#ifdef USE_PARALLEL_DISPATCHER
#include "BulletMultiThreaded/SpuGatheringCollisionDispatcher.h"
#include "BulletMultiThreaded/PlatformDefinitions.h"

#ifdef USE_LIBSPE2
    #include "BulletMultiThreaded/SpuLibspe2Support.h"
#elif defined (WIN32)
    #include "BulletMultiThreaded/Win32ThreadSupport.h"
    #include "BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h"
#elif defined (USE_PTHREADS)
    #include "BulletMultiThreaded/PosixThreadSupport.h"
    #include "BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h"s
#endif //USE_LIBSPE2


//#include "../../Extras/SATConvexCollision/Hull.h"


