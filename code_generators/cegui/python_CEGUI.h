
#include <vector>
#include <string>
#include <map>
#include <iostream>
#if defined __APPLE_CC__
    #include "strings.h"
    #include "Carbon/Carbon.h"
#endif

#include "OgrePlatform.h"
#if (OGRE_COMPILER == OGRE_COMPILER_GNUC ) && defined (__PYTHONOGRE_BUILD_CODE )
#define OGRE_COMP_VER 420
#endif

#include "CEGUI.h"

#include "CEGUIXMLParser.h"     // needed for System
#include "CEGUIXMLHandler.h"
#include "CEGUIXMLAttributes.h" // needed for FontManager
#include "CEGUIXMLSerializer.h"

#if defined VERSION_0_7_1
    #include "CEGUIOgreRenderer.h"
    #include "CEGUIOgreResourceProvider.h"
#else 
    #include "OgreCEGUIRenderer.h"
    #if ( CEGUI_VERSION_MINOR > 5 )
        #include "OgreCEGUIResourceProvider.h"
    #endif
#endif

#include "Ogre.h"   // we need this to fix unexposed stuff..

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_CEGUI_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_CEGUI{ namespace details{
inline void instantiate(){
 using namespace CEGUI;
 #include "python_CEGUI_sizeof.h"
 
} } }


