
#include "OgreOSMScene.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ofusion_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ofusion{ namespace details{
inline void instantiate(){
//  using namespace ofusion;
 #include "python_ofusion_sizeof.h"
 
} } }


