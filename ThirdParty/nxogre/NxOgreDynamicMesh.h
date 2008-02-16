/** \file    NxOgreDynamicMesh.h
 *  \brief   Header for the DynamicMesh, DynamicClothMesh class.
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

#ifndef __NXOGRE_DYNAMIC_MESH_H__
#define __NXOGRE_DYNAMIC_MESH_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreResourceManager.h"

namespace NxOgre {

	////////////////////////////////////////////////////////////////////

	/** \brief They live up to their namesake. A mesh which changes quite a lot.
	*/
	class NxPublicClass DynamicMesh {

		public:

			DynamicMesh();
			virtual ~DynamicMesh();

			void	fromNxMeshData(NxMeshData*) {}	// TEMP
			void	toNxMeshData(NxMeshData*) {}	// TEMP

			NxU32	mNbVertices;
			NxVec3* mVertices;
			
			NxU32	mNbNormals;
			NxVec3* mNormals;

			NxU32	mNbIndices;
			NxU32*	mIndices;

			NxU32	mNbTextureCoordinates;
			float2*	mTextureCoordinates;

			void	replaceVertices(NxVec3* vertices, NxU32 nbVertices);
			void	replaceNormals(NxVec3* normals, NxU32 nbNormals);
			void	replaceIndices(NxU32* indices, NxU32 nbIndices);
			void	replaceTextureCoordinates(float2* coords, NxU32 nbCoords);

			void	referenceVertices(NxVec3* vertices, NxU32 nbVertices, bool gc = true);
			void	referenceNormals(NxVec3* normals, NxU32 nbNormals, bool gc = true);
			void	referenceIndices(NxU32* indices, NxU32 nbIndices, bool gc = true);
			void	referenceTextureCoordinates(float2* coords, NxU32 nbCoords, bool gc = true);

	};

	////////////////////////////////////////////////////////////////////

	class NxPublicClass DynamicClothMesh : public DynamicMesh {

		public:

			DynamicClothMesh(NxClothMesh*);
			DynamicClothMesh() {}
			~DynamicClothMesh();

			NxClothMesh*	getMeshPtr();
			void			fromClothMeshIntermediary(ClothMeshIntermediary*);
			void			toClothMeshIntermediary(ClothMeshIntermediary*);

			NxClothMesh*	mMeshPtr;

	};

	////////////////////////////////////////////////////////////////////

};

#endif
