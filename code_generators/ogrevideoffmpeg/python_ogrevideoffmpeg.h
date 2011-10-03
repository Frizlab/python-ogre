#include "../ogre/python_ogre_include_OGRE.h"

#include "videoPlayer.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ogrevideoffmpeg_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_videoplayer{ namespace details{
inline void instantiate(){
 //using namespace VideoPlayer;
 #include "python_ogrevideoffmpeg_sizeof.h"
 
} } }


