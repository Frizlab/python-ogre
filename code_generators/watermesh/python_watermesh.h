
#include "WaterMesh.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_watermesh_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_watermesh{ namespace details{
inline void instantiate(){
 using namespace Ogre;
 #include "python_watermesh_sizeof.h"
 
} } }


