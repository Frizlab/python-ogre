
#include "NxPhysics.h"
#include "NxCooking.h"
#include "NxFoundation.h"
#include "NxCharacter.h"
// #include "NxExtensions.h"
#include "PhysXLoader.h"
#include "NxStream.h" // not included by default

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_physx_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_physx{ namespace details{
inline void instantiate(){
// //  using namespace e;
 #include "python_physx_sizeof.h"
 
} } }


