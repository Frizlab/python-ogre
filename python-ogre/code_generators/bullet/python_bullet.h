
#undef _MSC_VER
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"


#include "LinearMath/btStackAlloc.h"
#include "LinearMath/btPoolAllocator.h"
#include "BulletCollision/CollisionDispatch/btSimulationIslandManager.h"
#include "BulletDynamics/Character/btCharacterControllerInterface.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"

#include "BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h"

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

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_bullet_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_bullet{ namespace details{
inline void instantiate(){
// //  using namespace e;
 #include "python_bullet_sizeof.h"
 
} } }


