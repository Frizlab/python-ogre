
#include "ETTerrainInfo.h"
#include "ETTerrainManager.h"
#include "ETBrush.h"
#include "ETSplattingManager.h"
#include "Ogre.h"
#include "OgreColourValue.h"
#include "OgreImage.h"

// include following for ET::Impl Namespace if you must
//#include "ETIndexHandler.h"
//#include "ETOptions.h"
//#include "ETTerrainImpl.h"
//#include "ETTile.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_et_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ET{ namespace details{
inline void instantiate(){
 using namespace ET;
 //using namespace ET::Impl;
 #include "python_et_sizeof.h"
 
} } }


