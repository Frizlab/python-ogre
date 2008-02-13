/** \file    NxOgreFluidMesh.h
 *  \legacy
 *  \version 1.0-20
 *
 *  \licence NxOgre a wrapper for the PhysX physics library.
 *           Copyright (C) 2005-8 Robin Southern of NxOgre.org http://www.nxogre.org
 *           This library is free software; you can redistribute it and/or
 *           modify it under the terms of the GNU Lesser General Public
 *           License as published by the Free Software Foundation; either
 *           version 2.1 of the License, or (at your option) any later version.
 *           
 *           This library is distributed in the hope that it will be useful,
 *           but WITHOUT ANY WARRANTY; without even the implied warranty of
 *           MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *           Lesser General Public License for more details.
 *           
 *           You should have received a copy of the GNU Lesser General Public
 *           License along with this library; if not, write to the Free Software
 *           Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __NXOGRE_FLUID_MESH_H__
#define __NXOGRE_FLUID_MESH_H__

#include "NxOgrePrerequisites.h"

#if (NX_USE_FLUID_API == 1)

#include "NxOgreContainer.h"		// For: mCollisionList
#include "NxOgreParams.h"
#include "OgreMesh.h"
#include "OgreMatrix4.h"

namespace NxOgre {

	///	Params
	///		->	Kinematic: <bool>

	class NxPublicClass FluidMesh {

		public:

			FluidMesh(Fluid*, Scene*, Ogre::Camera*);
			virtual ~FluidMesh();

			//////////////////////////////////////////////////////////

			Scene*				getScene()		{return mOwner;}
			Ogre::MeshPtr		getMesh()		{return mMesh;}
			Ogre::SubMesh*		getSubMesh()	{return mSubMesh;}
			Ogre::Entity*		getEntity()		{return mEntity;}
			Ogre::SceneNode*	getNode()		{return mNode;}
			NxImplicitScreenMesh* getFluidMesh() {return mFluidMesh;}

			void				__preRender();
			void				__createMesh();
			void				__createOgreMesh();
			void				__addToScene();

			void				render(NxReal);
				
			
			

		protected:

			enum CONSTANTS {
				CN_MAX_VERTS_AND_INDICES = 100000
			};


			Scene					   *mOwner;
			Ogre::Camera*				mCamera;
			Fluid*						mFluid;
			NxFluid*					mNxFluid;
			NxImplicitScreenMesh*		mFluidMesh;
			Ogre::MeshPtr				mMesh;
			Ogre::SubMesh*				mSubMesh;
			
			Ogre::IndexData*			mIndexData;
			Ogre::VertexData*			mVertexData;
			Ogre::Entity*				mEntity;
			Ogre::SceneNode*			mNode;

			NxVec3*						mVertices;
			NxU32						mNbVertices;
			
			NxVec3*						mNormals;

			NxU32*						mIndices;
			NxU32						mNbIndices;
			


		private:

	};// End of Actor Class

};// End of namespace

#endif
#endif
