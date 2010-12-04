
#include <vector>
#include <string>
#include <map>
#include <iostream>

#include "../ogre/python_ogre_include_OGRE.h"

#include "CEGUI.h"

#include "CEGUIXMLParser.h"     // needed for System
#include "CEGUIXMLHandler.h"
#include "CEGUIXMLAttributes.h" // needed for FontManager
#include "CEGUIXMLSerializer.h"

//#if defined VERSION_0_7_1
    #include "CEGUIOgreRenderer.h"
    #include "CEGUIOgreResourceProvider.h"
/*#else 
    #include "OgreCEGUIRenderer.h"
    #if ( CEGUI_VERSION_MINOR > 5 )
        #include "OgreCEGUIResourceProvider.h"
    #endif
#endif
*/

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


