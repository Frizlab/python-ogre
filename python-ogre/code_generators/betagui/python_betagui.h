#include "BetaGUI.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_betagui_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_BetaGUI{ namespace details{
inline void instantiate(){
 using namespace BetaGUI;
 #include "python_betagui_sizeof.h"
 
} } }


