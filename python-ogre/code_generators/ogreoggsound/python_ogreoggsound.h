#include "../ogre/python_ogre_include_OGRE.h"

#include "OgreOggSound.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ogreoggsound_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ogreoggsound{ namespace details{
inline void instantiate(){
 using namespace OgreOggSound;
 #include "python_ogreoggsound_sizeof.h"
 
} } }


