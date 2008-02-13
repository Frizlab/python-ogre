/** \file    NxOgreResourceManager.h
 *  \brief   Header for the ResourceManager and Resource classes.
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

#ifndef __NXOGRE_RESOURCE_MANAGER_H__
#define __NXOGRE_RESOURCE_MANAGER_H__

#include "NxOgrePrerequisites.h"
#include "NxOgrePose.h"
#include "NxOgreContainer.h"
#include "NxOgreResourceStreamPtr.h"
#include "NxStream.h"

namespace NxOgre {

	/////////////////////////////////////////////////////////////////////

	/** \brief A lovely little class to handle pretty much anything external to NxOgre and PhysX.

	*/
	class NxPublicClass ResourceManager {

		friend class ResourceSystem;

		public:

			ResourceManager(PhysXDriver*);
			~ResourceManager();

			typedef NxString MeshIdentifier;
			
			static ResourceManager* getSingleton()		{
															return mResourceManager;
														}

			/** \brief Add a Resource System to NxOgre.
				\param identifier		Identifier of the Resource System
				\param system			Instance of the ResourceSystem
				\param owned			NxOgre "owns" this Resource System, and deletes it on shutdown.
			*/
			void					addResourceSystem(const NxString& identifier, ResourceSystem* system, bool owned = true);
			
			/** \brief Gets a Reource System by it's identifier.
				\note Identifiers of Resource Systems are always in lowercase.
			*/
			ResourceSystem*			getResourceSystem(const NxString& identifier);


			void					initAllResourceSystems();
			void					initResourceSystem(const NxString& identifier);
			
			////////////////////////////////////////////////////////////////////////////////////////////////

			// New ones.
			NxString				getMeshIdentifier(NxString filename, NxVec3 scale);
			NxString				getIdentifierFromFilename(NxString filename);

			// Basics
			bool					loadUnknownNXS(MeshIdentifier, ResourceStreamPtr);


			// Triangle Meshes
			NxTriangleMesh*			getTriangleMesh(MeshIdentifier);
			bool					loadTriangleMesh(MeshIdentifier, ResourceStreamPtr);
			bool					loadTriangleMesh(MeshIdentifier, NxTriangleMesh*);
			bool					saveTriangleMesh(MeshIdentifier, ResourceStreamPtr);
			bool					cookTriangleMesh(TriangleMeshIntermediary*, ResourceStreamPtr);
#if (NX_USE_OGRE == 1)
			TriangleMeshIntermediary*	generateTriangleMeshDescription(Ogre::MeshPtr, MaterialAlias*, NxVec3 scale = NxVec3(1,1,1));
#endif
			TriangleMeshIntermediary*	generateTriangleMeshDescription(NxTriangleMesh*, MaterialConversionList, NxVec3 scale = NxVec3(1,1,1));

#if (NX_USE_CLOTH_API == 1)
			// Cloth Meshes
			DynamicClothMesh*		getDynamicClothMesh(MeshIdentifier);
			NxClothMesh*			getClothMesh(MeshIdentifier);
			bool					loadClothMesh(MeshIdentifier, DynamicClothMesh*);
			bool					loadClothMesh(MeshIdentifier, ResourceStreamPtr);
			bool					loadClothMesh(MeshIdentifier, NxClothMesh*);
			bool					saveClothMesh(MeshIdentifier, ResourceStreamPtr);
			bool					cookClothMesh(ClothMeshIntermediary*, ResourceStreamPtr);
#if (NX_USE_OGRE == 1)
			ClothMeshIntermediary*	generateClothMesh(NxReal width, NxReal height, NxReal verticesPerRow = 10, NxReal verticesPerColumn = 10);
#endif
			ClothMeshIntermediary*	generateClothMesh(Ogre::MeshPtr*, NxVec3 scale = NxVec3(1,1,1));

#endif

			// Convex Meshes
			
			/** \brief getConvexMesh based on an identifier.

				\examples
					<code>
						"convexName"
						"file://convex1.obj"
						"ogre://convex1.mesh"
						"ogre://convex1.mesh +scale(2,2,2)"
					</code>
			*/
			NxConvexMesh*			getConvexMesh(const NxString& identifier);
			bool					loadConvexMesh(MeshIdentifier, ResourceStreamPtr);
			bool					loadConvexMesh(MeshIdentifier, NxConvexMesh*);
			bool					saveConvexMesh(MeshIdentifier, ResourceStreamPtr);
#if (NX_USE_OGRE == 1)
			ConvexMeshIntermediary*	generateConvexMeshDescription(Ogre::MeshPtr, NxVec3 scale = NxVec3(1,1,1));
#endif
			bool					cookConvexMesh(ConvexMeshIntermediary*, ResourceStreamPtr);
			NxConvexMesh*			cookConvexMesh(NxConvexMesh*, NxReal scale);

			// Material Alias
			MaterialAlias*			getMaterialAlias(NxString identifier);
			bool					loadMaterialAlias(NxString identifier, ResourceStreamPtr);
			bool					loadMaterialAlias(NxString identifier, MaterialAlias*);
			bool					saveMaterialAlias(NxString identifier, ResourceStreamPtr);
			MaterialAlias*			generateMaterialAliasForTriangleMesh(MeshIdentifier triangleMesh);
			MaterialAlias*			generateMaterialAliasForHeightField(MeshIdentifier heightfield);

			
			// HeightField
			NxHeightField*			getHeightField(MeshIdentifier);
			bool					loadHeightField(MeshIdentifier, ResourceStreamPtr);
			bool					loadHeightField(MeshIdentifier, NxHeightField*);
			bool					saveHeightField(MeshIdentifier, ResourceStreamPtr);
			NxHeightField*			cookHeightField(NxHeightFieldDesc, MaterialAlias*, ResourceStreamPtr);
			



			/////// Heightfield

			NxHeightField*			getHeightfield(NxString identifier);
			NxHeightField*			cookHeightFieldFromImage(NxString imageSource /* bits here */);
			void					saveHeightfield(NxString identifier, ResourceStreamPtr);
			void					storeHeightField(NxString identifier, NxHeightField*);
			
			/** Creates a MaterialAlias for the materials referenced in a Heightfield.
				\param identifier Heightfield to use; run through getMeshIndentifier first.
				\param saveCopy	  Save and own a copy in the ResourceManager.
				\return The material alias
			*/
			MaterialAlias*			createMaterialAliasForHeightfield(NxString identifier, bool saveCopy);


			////// SoftBody
			NxSoftBodyMesh*			getSoftBodyMesh(NxString identifier);
			void					saveSoftBodyMesh(NxString identifier, NxString filename);
			NxSoftBodyMesh*			cookSoftBodyMesh(NxSoftBodyMeshDesc);
			void					storeSoftBodyMesh(NxString identifier, NxSoftBodyMesh*);

			////// Skeleton
			Skeleton*				getSkeletonFromName(NxString identifier);
			Skeleton*				getSkeletonFromID(NxSkeletonID hashIdentifier);
			Skeleton*				getCubeSkeleton(Ogre::Vector3 dimensions, Pose pose = Pose());
			Skeleton*				getCubeSkeleton(NxVec3 dimensions, NxMat33 pose);
			Skeleton*				getSphereSkeleton(NxReal radius, NxMat33 pose);
			

	protected:

			/// MaterialAlias
			Container<NxString, MaterialAlias*>			mMaterialAliases;
		

			/// NxTriangleMeshes active in the Scene.
			Container<NxString, NxTriangleMesh*>		mTriangleMeshes;
			
			/// NxConvexMeshes active in a Scene, that have some scale (or not).
			Container<NxString, NxConvexMesh*>			mConvexMeshes;


			/// NxClothMeshes active in a Scene, that have some scale (or not).
			Container<NxString, DynamicClothMesh*>		mClothMeshes;
			
			
			/// NxSoftBodyMeshes active in a Scene, that have some scale (or not).
			std::map<NxString, NxSoftBodyMesh*>			mSoftBodyMeshes;


			/// All Skeletons
			/// ID is based of hash of dimensions/vertices/etc. All Skeletons are owned
			/// by ResourceManager, and mSkeletons "owns" the pointer.
			///
			std::map<NxSkeletonID,		Skeleton*>				mSkeletons;

			///
			///
			///
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
			// \param dimensions Size of the mesh/shape. In the case of convex or trianglemesh
			//												Bounding boxes, or number of vertices/indices
			//												can be the dimensions.
			//
			// \param ht		  Type of Shape.
			//
			// \notes	  Undoubtibly there will be collisions, but it won't be important
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

			static ResourceManager*		 mResourceManager;
			PhysXDriver					*mDriver;
			ResourceSystems				 mResourceSystems;

	};


	/////////////////////////////////////////////////////////////////////

	class Resource {
		
		public:
			
			Resource(const NxString& id, const NxString& type) : mID(id), mType(type) {} 
			virtual ~Resource() {}

			NxString		getID() const {return mID;}
			NxString		getType() const {return mType;}

		protected:

			NxString mID;
			NxString mType;

	};

	/////////////////////////////////////////////////////////////////////

};

#endif
