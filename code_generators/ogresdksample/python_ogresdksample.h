
#include "Ogre.h"
#include "SdkSample.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ogresdksample_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ogresdksample{ namespace details{
inline void instantiate(){
 using namespace OgreBites;
 #include "python_ogresdksample_sizeof.h"
 
} } }


