#include "Ogre.h"
#include "OgreColourValue.h"
#include "OgreWireBoundingBox.h"
//#include "OgrePagingLandScapePreCompiledHeaders.h"

#include "OgrePagingLandScapePrerequisites.h"

#ifndef _PLSM_OCTREE
	#include "OgreOctreeSceneManager.h"
#endif

// -------------------------------------------------------
// Octree Scene Manager (  occlusion management inside )
// -------------------------------------------------------

#include "OgrePagingLandScapeOctreeSceneManager.h"
#include "OgrePagingLandScapeOctreeNode.h"
#include "OgrePagingLandScapeOctree.h"
#include "OgrePagingLandScapeOctreeCamera.h"

#include "OgrePagingLandScapeOctreeAxisAlignedBoxSceneQuery.h"
#include "OgrePagingLandScapeOctreeIntersectionSceneQuery.h"
#include "OgrePagingLandScapeOctreePlaneBoundedVolumeListSceneQuery.h"
#include "OgrePagingLandScapeOctreeRaySceneQuery.h"
#include "OgrePagingLandScapeOctreeSphereSceneQuery.h"

//hardware occlusion management


#include "OgreOcclusionBoundingBox.h"
#include "OgrePagingLandScapeOcclusionQuerySet.h"
#include "OgrePagingLandScapeOcclusionSorter.h"

#include "OgrePagingLandScapeOcclusionTraversal.h"
#include "OgrePagingLandScapeOcclusionCameraTraversal.h"
#include "OgrePagingLandScapeOcclusionDebugTraversal.h"

#include "OgrePagingLandScapeOcclusionCHCTraversal.h"
#include "OgrePagingLandScapeOcclusionSWTraversal.h"
#include "OgrePagingLandScapeOcclusionVFTraversal.h"


#include "OgrePagingLandScapeOcclusion.h"
#include "OgrePagingLandScapeOcclusionElement.h"
#include "OgrePagingLandScapeOcclusionVisibilityData.h"


// -------------------------------------------------------
// Paging Scene Manager
// -------------------------------------------------------
#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeCamera.h"

#include "OgrePagingLandScapeRaySceneQuery.h"
#include "OgrePagingLandScapeIntersectionSceneQuery.h"
#include "OgrePagingLandScapeAxisAlignedBoxSceneQuery.h"

// Horizon Culling 
#include "OgrePagingLandScapeHorizon.h"

// Queues


// pages
#include "OgrePagingLandScapePage.h"
#include "OgrePagingLandScapePageRenderable.h"
#include "OgrePagingLandScapePageManager.h"


// Tile Management (page is constituted of tiles.)
#include "OgrePagingLandScapeTile.h"
#include "OgrePagingLandScapeTileInfo.h"
#include "OgrePagingLandScapeTileManager.h"

// IndexBuffer Caching to share it across tiles and LOD
#include "OgrePagingLandScapeIndexBuffer.h"
// Texture coordinates buffer cache to share it across pages
#include "OgrePagingLandScapeTextureCoordinatesManager.h"

// Renderable that constitutes tiles
#include "OgrePagingLandScapeRenderable.h"
#include "OgrePagingLandScapeRenderableManager.h"


// Terrain Data Source management
#include "OgrePagingLandScapeData2D.h"

#include "OgrePagingLandScapeData2D_HeightField.h"
#include "OgrePagingLandScapeData2D_HeightFieldTC.h"
#include "OgrePagingLandScapeData2D_HeightFieldN.h"
#include "OgrePagingLandScapeData2D_HeightFieldNTC.h"

#include "OgrePagingLandScapeData2D_HeightFieldRaw.h"
#include "OgrePagingLandScapeData2D_HeightFieldRawTC.h"

#include "OgrePagingLandScapeData2D_Spline.h"

#include "OgrePagingLandScapeData2DManager.h"


// Terrain Texture Source management
#include "OgrePagingLandScapeTexture.h"
#include "OgrePagingLandScapeTexture_Splatting.h"
#include "OgrePagingLandScapeTexture_BaseTexture.h"

#include "OgrePagingLandScapeTextureManager.h"

// User Call back system using listener pattern
#include "OgrePagingLandScapeListenerManager.h"
#include "OgrePagingLandScapeListener.h"

//===================================================
// TODO GCCXML crashes on these headers
//===================================================
#include "OgrePagingLandScapeCallback.h"
#include "OgrePagingLandScapeCallbackEvent.h"

//decals 

#include "OgrePagingLandScapeMeshDecal.h"

//utils 

#include "OgrePagingLandscapePoolSet.h"
#include "OgrePagingLandscapeQueue.h"

#include "OgreDebugRectangle2D.h"
#include "fileutils.h"

// test worka around fast delegates
#include "PagingLandScape2TerrainListener.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_plsm2_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ogrepcz{ namespace details{
inline void instantiate(){
 using namespace Ogre;
 #include "python_plsm2_sizeof.h"
 
} } }


