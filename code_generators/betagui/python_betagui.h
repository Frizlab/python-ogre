typedef float NxReal;
#include "BetaGUI2.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_betagui_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_BetaGUI2{ namespace details{
inline void instantiate(){
 using namespace BetaGUI2;
 #include "python_betagui_sizeof.h"
 
} } }


