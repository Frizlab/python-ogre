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

#ifndef __NXOGRE_MESH_MANAGER_H__
#define __NXOGRE_MESH_MANAGER_H__

#include "NxOgrePrerequisites.h"
#include "NxOgrePose.h"

namespace NxOgre {

	class NxExport MeshManager {

		class CStringList {

			public:

				CStringList() {}

				void push_back(const char* str) {
					strings.push_back(str);
				}

				void dump() {
					for (std::list<const char*>::iterator it = strings.begin();it != strings.end();it++) {
						std::cout << (*it) << std::endl;
					}
				}

				std::list<const char*> strings;

		};


		/** NxIntermediaryConvex 
			@brief
				A class that sits in betwen an Ogre::Mesh and a NxConvexMesh that represents the collision data.
				Allowing an Ogre::Mesh to only be read once, and every instance of a new NxConvexMesh using the
				same data uses the pre-loaded NxIntermerdiaryConvex data instead.

		*/
		class NxIntermediaryConvex {
			
			public:

				NxIntermediaryConvex() : nbVertices(0), nbIndices(0) {}
				NxIntermediaryConvex(NxU32 nv, NxU32 ni) : nbVertices(nv), nbIndices(ni) {}
				~NxIntermediaryConvex() {
					if (nbVertices)		delete [] vertices;
					if (nbIndices)		delete [] indices;
				}

				NxU32	nbVertices;
				NxVec3* vertices;
				NxU32	nbIndices;
				NxU32*	indices;
				NxU32	flags;
		
		};

		public:

			MeshManager(PhysXDriver*);
			~MeshManager();

			
			static MeshManager* getInstance()			{
															return mMeshManager;
														}


			NxTriangleMesh*			getTriangleMesh(NxString identifier, Ogre::Vector3 scale = Ogre::Vector3(1,1,1));
			NxConvexMesh*			getConvexMesh(NxString identifier, Ogre::Vector3 scale = Ogre::Vector3(1,1,1));
			NxSoftBodyMesh*			getSoftBodyMesh(NxString identifier);
			NxClothMesh*			getClothMesh(NxString identifier);
			NxTriangleMesh*			getTriangleMesh(NxString identifier);

			void					loadTriangleMesh(NxString filename);
			void					loadConvexMesh(NxString filename);
			void					loadSoftBodyMesh(NxString filename);
			void					loadClothMesh(NxString filename);
			void					saveTriangleMesh(NxString identifier, NxString filename);
			void					saveConvexMesh(NxString identifier, NxString filename);
			void					saveSoftBodyMesh(NxString identifier, NxString filename);
			void					saveClothMesh(NxString identifier, NxString filename);

			Skeleton*				getSkeletonFromName(NxString identifier);
			Skeleton*				getSkeletonFromID(NxSkeletonID hashIdentifier);
			Skeleton*				getCubeSkeleton(Ogre::Vector3 dimensions, Pose pose = Pose());
			Skeleton*				getCubeSkeleton(NxVec3 dimensions, NxMat33 pose);
			Skeleton*				getSphereSkeleton(NxReal radius, NxMat33 pose);

			NxTriangleMesh*			__cookTriangleMesh(NxString identifier, NxVec3 scale = NxVec3(1,1,1));
			NxConvexMesh*			__cookConvexMesh(NxString identifier, NxVec3 scale = NxVec3(1,1,1));
		
		protected:

		

			/// Storage for names of meshes.
			/// @note
			///		Names in NxXXMeshes contain the pointer to the const char*, not the string itself. 
			CStringList								mNames;


			/// Identifiers of meshes are combined on the filename (minus extension) and the scale (optional):
			///	
			///		ogrehead				<no scale mentioned because it's 1 1 1>
			///		cube.1m.mesh:4.2 1 2	<scale mentioned because it's not 1 1 1>
			///
			NxString getMeshIdentifier(NxString filename, NxVec3 scale);


			/// NxTriangleMeshes active in the Scene.
			std::map<NxString, NxTriangleMesh*>			mTriangleMeshes;
			

			/// Loaded Convex meshes that are not in the NxScene without scale.
			std::map<NxString, NxIntermediaryConvex*>	mIntermediaryConvexes;


			/// NxConvexMeshes active in a Scene, that have some scale (or not).
			std::map<NxString, NxConvexMesh*>			mConvexMeshes;


			/// NxClothMeshes active in a Scene, that have some scale (or not).
			std::map<NxString, NxClothMesh*>			mClothMeshes;
			
			
			/// NxSoftBodyMeshes active in a Scene, that have some scale (or not).
			std::map<NxString, NxSoftBodyMesh*>			mSoftBodyMeshes;


			/// All Skeletons
			/// ID is based of hash of dimensions/vertices/etc. All Skeletons are owned
			/// by MeshManager, and mSkeletons "owns" the pointer.
			///
			std::map<NxSkeletonID,		Skeleton*>				mSkeletons;

			std::map<NxString,			Skeleton*>				mSkeletonsByName;

			// SkeletonHashID
			//	For fast look up of skeletons
			//	It is checked for real if x,y,z and type match as well.

			
			enum SK_Types_Hash {
				SKH_Other = 41,
				SKH_Box = 79, 
				SKH_Sphere = 83, 
				SKH_Capsule = 89,
				SKH_Convex = 97,
				SKH_Triangle = 101
			};

			// GeneratedMeshHash
			//
			// @params dimensions Size of the mesh/shape. In the case of convex or trianglemesh
			//												Bounding boxes, or number of vertices/indices
			//												can be the dimensions.
			//
			// @params ht		  Type of Shape.
			//
			// @notes	  Undoubtibly there will be collisions, but it won't be important
			//			  it is expected (in the case of Skeletons; size/type) that 
			//			  additional data will be used to check if the mesh is really the mesh.

		
			NxSkeletonID generateMeshHash(NxVec3 dimensions, SK_Types_Hash ht) {
			
				char* chash = new char[sizeof(NxVec3)];
				memcpy(chash, &dimensions, sizeof(NxVec3));

				// djb2
				unsigned int hash = 5381;
				for (int i=0;i < sizeof(NxVec3);i++)
					hash = ((hash << 5) + hash) + chash[i];

				delete [] chash;

				hash = (unsigned int) ht + (hash << 6) + (hash << 16) - hash;

				return hash;
			}

		private:


			PhysXDriver *mDriver;
			static MeshManager* mMeshManager;

	};

};

#endif
