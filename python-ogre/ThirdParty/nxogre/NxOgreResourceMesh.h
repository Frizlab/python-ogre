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

#ifndef __NXOGRE_RESOURCEMESH_H__
#define __NXOGRE_RESOURCEMESH_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreResourceManager.h"

namespace NxOgre {

	/** @brief A small class to wrap a NxConvexMeshDescription, as well to look after and store
		vertices for the purposes of garbage collection.

	*/
	class ConvexMeshIntermediary {

		public:

			ConvexMeshIntermediary();
			~ConvexMeshIntermediary();

			void createVertices(NxU32 length);
			void destroyVertices();
			void replaceVertices(NxVec3* vertices, NxU32 length);
			void setToNormal();
			void verticesToDescription();
			void descriptionToVertices();

			NxConvexMeshDesc	mDescription;
			NxVec3*				mVertices;
			NxU32				mNbVertices;

	};

	class TriangleMeshIntermediary {
	
		public:

			TriangleMeshIntermediary();
			~TriangleMeshIntermediary();

			void createVertices(NxU32 length);
			void destroyVertices();
			void replaceVertices(NxVec3* vertices, NxU32 length);
			void verticesToDescription();

			void createIndices(NxU32 length);
			void destroyIndices();
			void replaceIndices(NxU32* indices, NxU32 length);
			void indicesToDescription();

			void createMaterialIndices(NxU32 length);
			void destroyMaterialIndices();
			void replaceMaterialIndices(NxU32* material_indices, NxU32 length);
			void materialIndicesToDescription();

			void setToNormal();
			
			NxTriangleMeshDesc	mDescription;
			
			NxVec3*				mVertices;
			NxU32				mNbVertices;
			
			NxU32*				mIndices;
			NxU32				mNbIndices;

			NxMaterialIndex*	mMaterialIndices;
			NxU32				mNbMaterialIndices;

	};

	class HeightfieldIntermediary {

	};

	class SoftbodyIntermediary {

	};

	class ClothIntermediary {

	};


	////////////////////////////////////////////////////////////////////

	/** A generic abstract Resource for handling/storage of Nx Meshes

	*/
	class MeshResource : public Resource {
		
		public:

			MeshResource(const NxString& id, NxConvexMesh*);
			MeshResource(const NxString& id, NxTriangleMesh*);
			MeshResource(const NxString& id, NxHeightField*);
			MeshResource(const NxString& id, NxSoftBody*);
			MeshResource(const NxString& id, NxCloth*);
			~MeshResource();

	};

};

#endif
