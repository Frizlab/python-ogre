//
//	NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org
//
//	This library is free software; you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation; either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef __NXOGRE_RENDERABLE_SOURCE_H__
#define __NXOGRE_RENDERABLE_SOURCE_H__

#include "NxOgrePrerequisites.h"
#include "NxOgrePose.h"

namespace NxOgre {

	class NxExport RenderableSource {

		/* best */ friend class SceneRenderer;

		public:


			/**@brief Level Of Detail
			*/
			enum LevelOfDetail {
				LOD_Low,		// Pose only.
				LOD_Medium,		// Pose and "half" of the detail.
				LOD_High		// Pose and all of the detail possible.
			};


			/**@brief Type of Render (used with Level of Detail). This is considered
					  advice by the RenderableSource and may not be used.
			*/
			enum Interpolation {
				I_Absolute,		// Render as absolute position
				I_Linear,		// Render as a blend between this and last frame.
				I_Linear_x4,	// Render as a blend between this and every four frames.
				I_Linear_x8		// Render as a blend between this and every eight frames.
			};

			/**@brief Set the type of Render, absolute or blend.
			*/
			void	setInterpolation(Interpolation i)		{
																mInterpolation = i;
																mLastPoseCount = 0;
															}


			/**@brief Gets the type of Render, absolute or blend
			*/
			Interpolation	getInterpolation() const		{
																return mInterpolation;
															}


			/**@brief Sets the Level of Detail that the RenderableSource will use.
			*/
			void	setLevelOfDetail(LevelOfDetail lod)		{
																mLOD = lod;
															}


			LevelOfDetail getLevelOfDetail() const			{
																return mLOD;
															}


			/**@brief Replaces the current Renderable with a new one.
			   @note This function can be quite dangerous.
			*/
			void	setRenderable(Renderable*);


			/** @brief Hash Identifier type of this Renderable, must implemented
				by the class inheriting it.
			*/
			virtual NxShortHashIdentifier	getType() const = 0;


			/** @brief String Identifier tpy of this Renderable, must be implemented
				by the class inheriting it.
			*/
			virtual NxString				getStringType() const = 0;


			/** @brief Get a copy of the Renderable
			*/
			Renderable* getRenderable();


			/** @brief What exactly the Renderable is.
			*/
			NxString	getRenderableType();


			/** @brief What exactly the Renderable is as a hash
			*/
			NxShortHashIdentifier getRenderableTypeHash();


		protected:

			virtual void __renderSelf() = 0;

			Renderable*				mRenderable;
			NxRenderableSourceID	mRenderableSourceID;
			LevelOfDetail			mLOD;
			Interpolation			mInterpolation;
			Pose					mLastPose;
			Pose					mDeltaPose;
			NxU32					mLastPoseCount;

	};


};

#endif
