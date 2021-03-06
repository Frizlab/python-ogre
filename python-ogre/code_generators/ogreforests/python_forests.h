#include <string>

#include "../ogre/python_ogre_include_OGRE.h"

#include "PagedGeometry.h"
#include "GrassLoader.h"

#include "TreeLoader3D.h"
#include "TreeLoader2D.h"
#include "BatchPage.h"
#include "ImpostorPage.h"
#include "WindBatchPage.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_forests_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_forests{ namespace details{
inline void instantiate(){
 using namespace Forests;
 #include "python_forests_sizeof.h"
 
} } }


