#include "Ogre.h"  // need to include this
#include "QuickGUI.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_quickgui_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_QuickGUI{ namespace details{
inline void instantiate(){
 using namespace QuickGUI;
 #include "python_quickgui_sizeof.h"
 
} } }


