
#include "PROJECT MASTER.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_awesomium_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_awesomium{ namespace details{
inline void instantiate(){
 using namespace PROJECT;
 #include "python_awesomium_sizeof.h"
 
} } }


