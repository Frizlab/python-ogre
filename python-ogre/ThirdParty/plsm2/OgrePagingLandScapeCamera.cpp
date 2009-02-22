/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright � 2000-2004 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
/***************************************************************************
PagingLandScapeCamera.cpp  -  description
-------------------
begin                : Fri Sep 27 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team

***************************************************************************/

#include "OgrePagingLandScapePrecompiledHeaders.h"



#include "OgreCamera.h"

#include "OgrePagingLandScapeCamera.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeSceneManager.h"

namespace Ogre
{
    //-----------------------------------------------------------------------
    PagingLandScapeCamera::PagingLandScapeCamera(const String& name, SceneManager* sm) : 
        PagingLandScapeOctreeCamera(name, sm),

        mCurrentCameraPageX(0), 
        mCurrentCameraPageZ(0),
        mCurrentCameraTileX(0), 
        mCurrentCameraTileZ(0),
        mIniX(0),
        mFinX(0),
		mIniZ(0),
        mFinZ(0),
        mPreIniX(0),
        mPreFinX(0),
        mPreIniZ(0),
        mPreFinZ(0),
	    mLastCameraPos (Vector3(std::numeric_limits<Real>::max (), 
                                0.0f, 
                                std::numeric_limits<Real>::max ()))
    {
        // initialized after the Camera::Camera().
        mLastViewport = 0;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeCamera::~PagingLandScapeCamera()
    {  
       
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeCamera::resetPaging()
    {
        mCurrentCameraPageX = 0; 
        mCurrentCameraPageZ = 0;
        mCurrentCameraTileX = 0; 
        mCurrentCameraTileZ = 0;
        mIniX = 0;
        mFinX = 0;
		mIniZ = 0;
        mFinZ = 0;
        mPreIniX = 0;
        mPreFinX = 0;
        mPreIniZ = 0;
        mPreFinZ = 0;
        mLastCameraPos = Vector3 (std::numeric_limits<Real>::min (), 
                                  0.0f, 
                                  std::numeric_limits<Real>::min ());
        updatePaging (0, 0);
//        Real dist = Camera::getDerivedPosition().squaredLength()
//            + mParent->getOptions()->cameraThreshold
//        mLastCameraPos.x = dist;
//        mLastCameraPos.y = dist;
//        mLastCameraPos.z = dist;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeCamera::updatePaging (const unsigned int x, const unsigned int z)
    {
        // We must load the next visible LandScape pages, 
        // check the LandScape boundaries	

        mCurrentCameraPageX = x;
        mCurrentCameraPageZ = z;
        const PagingLandScapeOptions * const opt = 
            static_cast <PagingLandScapeSceneManager*> (mSceneMgr)->getOptions ();
        const unsigned int w = opt->world_width;
        const unsigned int h = opt->world_height;
        const unsigned int adjpages = opt->max_adjacent_pages;
        const unsigned int prepages = opt->max_preload_pages;

        // Load Windowing
        unsigned int lx = x;
        if (static_cast<int> (x - adjpages) >= 0)
        {
            mIniX = x - adjpages;
        }
        else
        {
            mIniX = 0;
            lx -= x - adjpages;
        }
        if (lx + adjpages < w)
        {
            mFinX = lx + adjpages;
        }
        else
        {
            mFinX = w - 1;
            mIniX = (static_cast <int> (mIniX + (w - (lx + adjpages))) > 0)? mIniX + (w - (lx + adjpages)):0;
        }

        unsigned int lz = z;
        if (static_cast<int> (z - adjpages) >= 0)
        {
            mIniZ = z - adjpages;
        }
        else
        {
            mIniZ = 0;
            lz -= z - adjpages;
        }
        if (lz + adjpages < h)
        {
            mFinZ = lz + adjpages;
        }
        else
        {
            mFinZ = h - 1;
            mIniZ = (static_cast <int> (mIniZ + (h - (lz + adjpages))) > 0)? mIniZ + (h - (lz + adjpages)):0;
        }

        // Pre-load Windowing
        lx = x;
        if (static_cast<int> (x - prepages) > 0)
        {
            mPreIniX = x - prepages;
        }
        else
        {
            mPreIniX = 0;
            lx -= x - prepages;
        }
        if (x + prepages < w)
        {
            mPreFinX = x + prepages;
        }
        else
        {
            mPreFinX = w - 1;
            mPreIniX = (static_cast <int> (mPreIniX + (w - (lx + prepages))) > 0)? mPreIniX + (w - (lx + prepages)):0;
        }

        lz = z;
        if (static_cast<int> (z - prepages) > 0)
        {
            mPreIniZ = z - prepages;
        }
        else
        {
            mPreIniZ = 0;
            lz -= z - prepages;
        }
        if (lz + prepages < h)
        {
            mPreFinZ = lz + prepages;
        }
        else
        {
            mPreFinZ = h - 1;
            mPreIniZ = (static_cast <int> (mPreIniZ + (h - (lz + prepages))) > 0)? mPreIniZ  + (h - (lz + prepages)):0;
        }
        //(static_cast <PagingLandScapeSceneManager*> (getSceneManager())->resize());
    }
    //-----------------------------------------------------------------------

    bool PagingLandScapeCamera::isVisible(const AxisAlignedBox &bound) const
    {

		// Null boxes always invisible
		PLSM2_ASSERT (!bound.isNull());
		// Infinite boxes always visible
		PLSM2_ASSERT (!bound.isInfinite());
	

		// Get centre of the box
		const Vector3 &centre (bound.getCenter());
		// Get the half-size of the box
		const Vector3 &halfSize =(bound.getHalfSize());

		// For each plane, see if all points are on the negative side
		// If so, object is not visible
		const bool infinite_far_clip = (mFarDist == 0);
		for (unsigned int plane = 0; plane < 6; ++plane)
		{
			// Skip far plane if infinite view frustum
			if (plane == FRUSTUM_PLANE_FAR && infinite_far_clip)
				continue;

			const Plane::Side side = mFrustumPlanes[plane].getSide(centre, halfSize);
			if (side == Plane::NEGATIVE_SIDE)
			{
				// ALL corners on negative side therefore out of view
				return false;
			}

		}
		return true;
    }
 
}




