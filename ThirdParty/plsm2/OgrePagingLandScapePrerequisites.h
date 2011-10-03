/***************************************************************************
  OgrePagingLandScapePrerequisites.h  -  description
  -------------------
	begin                : Sun Oct 26 2003
	copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
	email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __PagingLandScapePrerequisites_H__
#define __PagingLandScapePrerequisites_H__


#include "OgrePrerequisites.h"

#include "OgreHardwareBufferManager.h"
#include "OgreTextureManager.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreAxisAlignedBox.h"

//-----------------------------------------------------------------------
// Options 
//-----------------------------------------------------------------------

//#define _VISIBILITYDEBUG

#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    #define PLSM2_EIHORT 1
#endif

//-----------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------

#include <vector>
#include <map>
#include <queue>
#include <list>
#include <stack>
#include <limits>


namespace Ogre
{


    // Octree Scene management
	
    typedef std::list < MovableObject* > MovableObjectList;
    typedef std::list < Renderable* > RenderableList;

    class PagingLandScapeOctreeSceneManager;
    class PagingLandScapeOctreeNode;
    typedef std::list<PagingLandScapeOctreeNode*> PagingLandscapeOctreeNodeList;
    class PagingLandScapeOctree;
    class PagingLandScapeOctreeCamera;

    #ifdef _VISIBILITYDEBUG
        class DebugRectangle2D;
    #endif //_VISIBILITYDEBUG

    // hardware occlusion management
    class QuerySet;
    class Occlusion;
    class VisibilityData;
    class OcclusionElement;
    class Between;
    class Leaf;


    typedef enum culling_modes
    { 
        // STANDARD_WALK = 0,
        // VIEW_FRUSTUM,
        VIEW_FRUSTUM_DIRECT,
        //STOP_AND_WAIT,
        CHC, 
        CHC_CONSERVATIVE, 
        NUM_CULLING_MODE
    };
    class OcclusionBoundingBox;
    typedef std::list<OcclusionElement*> OcclusionElementList;
    typedef std::list<PagingLandScapeOctreeNode*> PagingLandScapeOctreeNodeList;

    class FrontToBackNodeSorterOperator;
    //priority queue for the nodes to traversal
	typedef std::priority_queue<OcclusionElement*,std::vector<OcclusionElement*>,FrontToBackNodeSorterOperator> FrontToBackNodeSorterPriorityQueue;
	//queue for the nodes with running occlusion queries
	typedef std::queue<OcclusionElement*> QueriedOcclusionElement;  //queue for the nodes with running occlusion queries
    	   

    class Traversal;
    class TraversalConst;
    class ConstTraversalConst;

    class CHCTraversal;
    class SWTraversal;
    class ViewFrustumCullingTraversal;
    class ViewFrustumCullingTraversalDirect;
    class TreeOverlayDebug;





    // Paging Scene Management

    class PagingLandScapeSceneManager;
    class PagingLandScapeOptions;
    class PagingLandScapeCamera;
    typedef std::list< PagingLandScapeCamera* > PagingLandScapeCameraList;

    class PagingLandScapeRaySceneQuery;
    
    // Basic Horizon Culling (based on tile max and min height)
    class PagingLandScapeHorizon;

    // Page Scene Management
    class PagingLandScapePage;
    class PagingLandScapePageRenderable;
    class PagingLandScapePageManager;

#define PAGE_INSIDE  0x00000001
#define PAGE_CHANGE  0x00000002
#define PAGE_OUTSIDE 0x00000004


typedef enum PageState
{
	inited,
	Preloaded,
	Textureloaded,
	Loaded
};

typedef enum PageQueuingState
{
	queuednone,
	queuedPreload,
	queuedTextureLoad,
	queuedLoad	
};

	typedef std::list< PagingLandScapePage* > PagingLandScapePageList;
	typedef std::vector< PagingLandScapePage* > PagingLandScapePageArray;
    typedef PagingLandScapePageArray PagingLandScapePageRow;
    typedef std::vector< PagingLandScapePageRow > PagingLandScapePages;

    // Tile Management (page is constituted of tiles.)
    class PagingLandScapeTile;
    class PagingLandScapeTileInfo;
	class PagingLandScapeTileManager;
	typedef std::list< PagingLandScapeTile* > PagingLandScapeTileList;
    typedef std::vector< PagingLandScapeTile* > PagingLandScapeTileRow;
    typedef std::vector< PagingLandScapeTileRow > PagingLandScapeTiles;


    // IndexBuffer Caching to share it across tiles and LOD
    class PagingLandScapeIndexBufferManager;
    typedef std::map< unsigned int, IndexData* > IndexMap;
    typedef std::vector< IndexData* > IndexArray;
    typedef std::vector< IndexMap* > LevelArray;

    // Renderable that constitutes tiles
    class PagingLandScapeRenderable;
	class PagingLandScapeRenderableSet;
    class PagingLandScapeRenderableManager;
    typedef std::vector< PagingLandScapeRenderable* > PagingLandScapeRenderableVector;

	// Texture coordinates buffer cache
	class PagingLandScapeTextureCoordinatesManager;

    // Terrain Data Source management
    class PagingLandScapeData2D;
	class PagingLandScapeData2DManager;
	typedef std::vector< PagingLandScapeData2D* > PagingLandScapeData2DArray;
	typedef std::list< PagingLandScapeData2D* > PagingLandScapeData2DList;

    typedef std::vector< PagingLandScapeData2D* > PagingLandScapeData2DRow;
    typedef std::vector< PagingLandScapeData2DRow > PagingLandScapeData2DPages;

    typedef std::vector<PagingLandScapeData2D*> PagingLandScapeData2DMap;

    // Terrain Texture Source management
    class PagingLandScapeTexture;
	class PagingLandScapeTextureManager;

	typedef std::vector< PagingLandScapeTexture* > PagingLandScapeTextureArray;
	typedef std::list< PagingLandScapeTexture* > PagingLandScapeTextureList;

    typedef std::vector< PagingLandScapeTexture* > PagingLandScapeTextureRow;
    typedef std::vector< PagingLandScapeTextureRow > PagingLandScapeTexturePages;

    typedef std::vector<PagingLandScapeTexture*> PagingLandScapeTextureMap;


    typedef std::vector< HardwareVertexBufferSharedPtr> HardwareTextureBuffersRow;
    typedef std::vector< HardwareTextureBuffersRow> HardwareTextureBuffersCol;

    // User Call back system using listener pattern
    class PagingLandScapeListenerManager;
    class PagingLandScapeListener;
	class PagingLandscapeEvent;

    // Multi-map management
    typedef std::map< String, String > LandScapeFileNames;

    // Vertex Shader Hardware Morphing
    #define MORPH_CUSTOM_PARAM_ID 77

    // LOD
    enum Neighbor
    {
        NORTH = 0,
        SOUTH,
        EAST,
        WEST
    };

    #define STITCH_NORTH_SHIFT 0
    #define STITCH_SOUTH_SHIFT 8
    #define STITCH_WEST_SHIFT  16
    #define STITCH_EAST_SHIFT  24

    #define STITCH_NORTH  128 << STITCH_NORTH_SHIFT
    #define STITCH_SOUTH  128 << STITCH_SOUTH_SHIFT
    #define STITCH_WEST   128 << STITCH_WEST_SHIFT
    #define STITCH_EAST   128 << STITCH_EAST_SHIFT
    
}
//-----------------------------------------------------------------------
// Windows Settings
//-----------------------------------------------------------------------

//#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 
//#   ifdef PLUGIN_PAGINGLANDSCAPE2_EXPORTS
//#       define _OgrePagingLandScapeExport __declspec(dllexport) 
//#   else 
//#       define _OgrePagingLandScapeExport __declspec(dllimport) 
//#   endif 
//#else 
#   define _OgrePagingLandScapeExport 
//#endif 

//STL trick to call delete in a for_each
struct delete_object
{
    template <typename T>
    void operator()(T *ptr){ delete ptr; ptr = 0;}
};


#ifndef ogre_restrict
#ifdef __INTEL_COMPILER
#define ogre_restrict restrict
#elif _MSC_VER >= 1400
#define ogre_restrict __restrict
#else
#define ogre_restrict
#endif
#endif


// DEBUGGING

#ifdef _DEBUG
#define _PLSM2_RELEASE_ASSERT
#endif 

#ifdef _PLSM2_RELEASE_ASSERT

#define PLSM2_HALT() do { __asm int 3 } while(0)

#define PLSM2_ASSERT(x) \
	do { if ((!(x))) \
	PLSM2_HALT(); } while(0)
#else
#define PLSM2_ASSERT(x) \
	do { (void)sizeof(x); } while(0)
#endif





#endif //__PagingLandScapePrerequisites_H__
