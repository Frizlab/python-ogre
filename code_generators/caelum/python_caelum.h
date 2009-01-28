#if defined __APPLE_CC__
    #include "strings.h"
    #include "Carbon/Carbon.h"
#endif
#include "Caelum.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_caelum_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_Caelum{ namespace details{
inline void instantiate(){
 using namespace Caelum;
 #include "python_caelum_sizeof.h"
 
} } }


