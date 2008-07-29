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


