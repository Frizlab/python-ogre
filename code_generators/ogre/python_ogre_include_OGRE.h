/* this is a "standard" way to include OGRE when building a python-ogre module
   so we have a centralised way to resolve compiler issues etc
*/
#if defined __APPLE_CC__
    #include "strings.h"
    #include "Carbon/Carbon.h"
#endif

#include "OgrePlatform.h"
#if (OGRE_COMPILER == OGRE_COMPILER_GNUC ) && OGRE_THREAD_PROVIDER
#define OGRE_COMP_VER 420
#endif

// Solve an issue on MSVC
// disable thread support so gccxml can handle atomicwrappers correctly
#if (OGRE_COMPILER == OGRE_COMPILER_MSVC ) && defined (__PYTHONOGRE_BUILD_CODE )
 #include "OgrePrerequisites.h"
 #define _HOLD_STATUS OGRE_THREAD_SUPPORT  // need to backup current state
 #define OGRE_THREAD_SUPPORT 0  // disable threading
 #include "OgreAtomicWrappers.h"   // include the problem definition
 #define OGRE_THREAD_SUPPORT _HOLD_STATUS // set back to the original state
#endif

#include "Ogre.h"