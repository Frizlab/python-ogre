#include "../ogre/python_ogre_include_OGRE.h"

#include "Hikari.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_hikari_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_hikari{ namespace details{
inline void instantiate(){
 using namespace Hikari;
 #include "python_hikari_sizeof.h"
 
} } }


