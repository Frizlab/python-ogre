#ifndef __python_ogrear_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__
#define __python_ogrear_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__

//See best practices section in Py++ documentation
#include "UtilsOgreDshow.h"
#include "Ogre.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ogredshow_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ogrear{ namespace details{
inline void instantiate(){
 using namespace OgreUtils;
 #include "python_ogredshow_sizeof.h"
 
} } }



#endif//__python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__
