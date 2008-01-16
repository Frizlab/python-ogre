
#include "NxOgre.h"
#include "NxOgreSkeleton.h"  //missing ??
#include "NxOgreResourceMesh.h" // not included anywhere by default...
#include "Ogre.h"
#include "OgrePlugin.h"
#include <NxPhysics.h>

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_nxogre_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_nxogre{ namespace details{
inline void instantiate(){
 using namespace NxOgre;
 #include "python_nxogre_sizeof.h"
 
} } }


