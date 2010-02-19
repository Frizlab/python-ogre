#if defined __APPLE_CC__
    #include "strings.h"
    #include "Carbon/Carbon.h"
#endif
#include "OgrePlatform.h"
#if (OGRE_COMPILER == OGRE_COMPILER_GNUC ) && defined (__PYTHONOGRE_BUILD_CODE )
#define OGRE_COMP_VER 420
#endif
#include "Hydrax.h"
#include "ProjectedGrid.h"
#include "SimpleGrid.h"
#include "Perlin.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_hydrax_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_hydrax{ namespace details{
inline void instantiate(){
 using namespace Hydrax;
 #include "python_hydrax_sizeof.h"
 
} } }


