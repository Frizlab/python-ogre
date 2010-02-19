#if defined __APPLE_CC__
    #include "strings.h"
    #include "Carbon/Carbon.h"
#endif
#include "OgrePlatform.h"
#if (OGRE_COMPILER == OGRE_COMPILER_GNUC ) && defined (__PYTHONOGRE_BUILD_CODE )
#define OGRE_COMP_VER 420
#endif
#include "Ogre.h"
#include "OgrePortal.h"
#include "OgrePCZSceneNode.h"
#include "OgrePCZone.h"
#include "OgrePCZSceneManager.h"
#include "OgrePCZSceneQuery.h"
#include "OgrePCZLight.h"
#include "OgrePCPlane.h"
#include "OgrePCZFrustum.h"


// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ogrepcz_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ogrepcz{ namespace details{
inline void instantiate(){
 using namespace Ogre;
 #include "python_ogrepcz_sizeof.h"
 
} } }


