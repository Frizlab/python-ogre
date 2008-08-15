
#include "PROJECT MASTER.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_PROJECT_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_PROJECT{ namespace details{
inline void instantiate(){
 using namespace PROJECT;
 #include "python_PROJECT_sizeof.h"
 
} } }


