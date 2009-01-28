#if defined __APPLE_CC__
    #include "strings.h"
    #include "Carbon/Carbon.h"
#endif
#include "CTParameters.h"
#include "CTSerializer.h"
//#include "CTSection.h"
#include "CTStem.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_cadunetree_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_cadunetree{ namespace details{
inline void instantiate(){
 using namespace CaduneTree;
 #include "python_cadunetree_sizeof.h"
 
} } }


