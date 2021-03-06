#include "../ogre/python_ogre_include_OGRE.h"

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


