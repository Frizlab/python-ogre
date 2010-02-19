#include "OgrePlatform.h"
#if (OGRE_COMPILER == OGRE_COMPILER_GNUC ) && defined (__PYTHONOGRE_BUILD_CODE )
#define OGRE_COMP_VER 420
#endif
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


