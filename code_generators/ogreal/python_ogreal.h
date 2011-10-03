#include "../ogre/python_ogre_include_OGRE.h"
#include "OgreAL.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ogreal_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ogreal{ namespace details{
inline void instantiate(){
 //using namespace OgreAL;
 #include "python_ogreal_sizeof.h"
 
} 
} 
}
