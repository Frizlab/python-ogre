#include "navi.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_navi_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_plib{ namespace details{
inline void instantiate(){
  using namespace NaviLibrary;

 #include "python_navi_sizeof.h"
 
} } }


