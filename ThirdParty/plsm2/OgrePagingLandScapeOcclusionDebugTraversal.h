#ifndef __PagingLandScapeOcclusionTreeOverlayDebugTraversal_H
#define __PagingLandScapeOcclusionTreeOverlayDebugTraversal_H



#include "OgrePagingLandScapePrerequisites.h"

#ifdef _VISIBILITYDEBUG

#include "OgrePagingLandScapeOctreeCamera.h"

#include "OgrePagingLandScapeOcclusionTraversal.h"

namespace Ogre
{
    //------------------------------------------------------
    //
    class TreeOverlayDebug : public TraversalConst
    {
	    public:
		    TreeOverlayDebug(Occlusion& o, PagingLandScapeOctreeSceneManager *scnMgr);
		    virtual void onTree(PagingLandScapeOctree& n, VisibleObjectsBoundsInfo * const visibleBounds ) const ;
		    virtual void onLeaf(PagingLandScapeOctreeNode& n, VisibleObjectsBoundsInfo * const visibleBounds ) const ;

	    private:
		    Occlusion& occlusion;
            PagingLandScapeOctreeSceneManager *mScnMrg;

	};
}

#endif //_VISIBILITYDEBUG

#endif //__PagingLandScapeOcclusionTreeOverlayDebugTraversal_H
