
#include "Ogre.h"
#include "OgreTerrain.h"
#include "OgreTerrainQuadTreeNode.h"
#include "OgreTerrainPaging.h"
#include "OgreTerrainGroup.h"
#include "OgreTerrainMaterialGeneratorA.h"
#include "OgreTerrainPagedWorldSection.h"


// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ogreterrain_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ogreterrain{ namespace details{
inline void instantiate(){
 using namespace Ogre;
 #include "python_ogreterrain_sizeof.h"
 
} } }


