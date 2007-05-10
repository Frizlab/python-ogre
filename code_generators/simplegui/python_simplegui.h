
#include "SimpleGUI.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_simplegui_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_SimpleGUI{ namespace details{
inline void instantiate(){
 using namespace SimpleGUI;
 #include "python_simplegui_sizeof.h"
 
} } }


