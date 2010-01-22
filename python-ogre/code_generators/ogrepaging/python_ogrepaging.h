
#include "OgrePaging.h"
#include "OgrePageContentCollectionFactory.h"
#include "OgrePageContentFactory.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ogrepaging_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ogrepaging{ namespace details{
inline void instantiate(){
 using namespace Ogre;
 #include "python_ogrepaging_sizeof.h"
 
} } }


