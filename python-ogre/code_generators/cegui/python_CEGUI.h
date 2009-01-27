
#include <vector>
#include <string>
#include <map>
#include <iostream>
#if defined __APPLE_CC__
    #include "strings.h"
    #include "Carbon/Carbon.h"
#endif
#include "CEGUI.h"
#include "CEGUIXMLParser.h"     // needed for System
#include "CEGUIXMLHandler.h"
#include "CEGUIXMLAttributes.h" // needed for FontManager
#include "CEGUIXMLSerializer.h"
#include "OgreCEGUIRenderer.h"

#if ( CEGUI_VERSION_MINOR > 5 )
    #include "OgreCEGUIResourceProvider.h"
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


