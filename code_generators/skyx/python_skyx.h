#include "../ogre/python_ogre_include_OGRE.h"
#include "SkyX.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_skyx_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_skyx{ namespace details{
inline void instantiate(){
 using namespace SkyX;
 #include "python_skyx_sizeof.h"
 
} } }


