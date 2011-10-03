#ifndef __python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__
#define __python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__

//See best practices section in Py++ documentation
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <map>

#include "python_ogre_masterlist.h"

typedef HashMap<Ogre::String, unsigned short> OgreHashMapStringUshortOuter;

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ogre_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ogre{ namespace details{
inline void instantiate(){
 using namespace Ogre;
 #include "python_ogre_sizeof.h"
 
} } }

#endif//__python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__
