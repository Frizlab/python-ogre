#if defined __APPLE_CC__
    #include "strings.h"
    #include "Carbon/Carbon.h"
#endif

#include <vector>
#include <string>
#include <map>
#include "OgreNewt.h"


// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ogrenewt_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ogrenewt{ namespace details{
inline void instantiate(){
 using namespace Ogre;
 #include "python_ogrenewt_sizeof.h"
 
} } }

        

        