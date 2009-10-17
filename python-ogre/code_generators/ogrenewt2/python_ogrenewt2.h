
#include "OgreNewt.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ogrenewt2_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ogrenewt{ namespace details{
inline void instantiate(){
 using namespace OgreNewt;
 #include "python_ogrenewt2_sizeof.h"
 
} } }
