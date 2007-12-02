
#undef _MSC_VER
#include "btBulletDynamicsCommon.h"

#include "LinearMath/btStackAlloc.h"
#include "LinearMath/btPoolAllocator.h"
#include "BulletCollision/CollisionDispatch/btSimulationIslandManager.h"
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


