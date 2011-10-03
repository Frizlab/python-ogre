/** \file    NxOgreMesh.h
 *  \brief   Header for the Mesh class.
 *  \version 1.0-22T2
 *
 *  \licence NxOgre a wrapper for the PhysX physics library.
 *           Copyright © 2005-2008 Robin Southern of NxOgre.org http://www.nxogre.org
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


#ifndef __NXOGRE_MESH_H__
#define __NXOGRE_MESH_H__

#include "NxOgrePrerequisites.h"

namespace NxOgre {
namespace Resources {

	class NxPublicClass Mesh {

		friend class ResourceSystem;
		friend class ManualMesh;
		friend class ManualMeshUtil;
		friend class Convex;
		friend class TriangleMesh;

		public:

			Mesh();
			Mesh(const ResourceIdentifier&);
			Mesh(Resource*);
			~Mesh();

      /** \brief Get's the name of this mesh.
          \note  If it hasn't been added to the resource system, then it doesn't have one.
      */
      NxString getName(void) const;

			void load(Resource*);
			void load(const ResourceIdentifier&);
#ifndef NX_SMALL
			void save(Resource*);
#endif

#ifndef NX_SMALL
			void save(const ResourceIdentifier&);
#endif
			MeshType  getType() const;
			NxU32     getReferenceCount() const;
			MeshType  getMeshTypeFromResource(Resource*);

			enum Reason {
				R_HasMesh         = 0,
				R_InvalidResource = 1,
        R_InvalidHeader   = 2
			};

			void acquire(MeshType, void* data);
			void acquire(NxConvexMesh*);
			void acquire(NxTriangleMesh*);
			void acquire(NxCCDSkeleton*);
			void acquire(NxClothMesh*);
			void acquire(NxSoftBodyMesh*);
			void release();

			void increaseReference();
			void decreaseReference();

			/** \brief A class representing the ever changing mesh data; with a primitive locking state
			           for avoiding crashing or corrupt data when working with it.
			*/
			/// Some how this will need a callback function for every simulation to update the new
			/// mesh data. All MeshData of this Mesh will all originate from the same source. Locking
			/// one will lock them all, etc. Since the user may be working in a threaded environment
			/// then a locking is required, so when the callback is triggered it can decide or not
			/// to update it.
			/// 
			///   Global callback class may be needed - like a machine but more abstract  
			///     class SimulationEvent
			///     {
			///        virtual void onSimulate(const TimeStep&);
			///     };
			class MutableMeshData
			{
				public:

					/** \brief Flagged when new data is written. User/App should set it to false once
					           to track any changes, and don't cause useless updates if the mesh has not
					           changed.
					*/
					bool    mNewData;
					
					/** \brief Vertices; castable into NxVec3
					*/
					float3* mVertices;

					/** \brief Normals
					*/
					float3* mNormals;
					
					/** \brief TextureCoordinates
					*/
					float2* mTextureCoords;
					
					/** \brief Number of vertices/normals/texture coordinates
					*/
					NxU32*  mNbVertices;
					
					/** \brief Indicies 
					*/
					NxU32*  mIndicies;
					
					/** \brief Number of indicies (triangles = indicies / 3)
					*/
					NxU32*  mNbIndices;

					/** \brief Place on the lock; Do not write to the Mesh, user/app is using it.
					*/
					void lock();
					
					/** \brief Remove the lock; Mesh can use it. User/App is forbidden to use it.
					*/
					void unlock();
					
					/** \brief Lock status
					*/
					bool isLocked() const;

				protected:

					Mesh*    _mesh;

			};

			MutableMeshData*   getMutableMesh(void) const;

			NxTriangleMesh*    getAsTriangleMesh(void);
			NxConvexMesh*      getAsConvexMesh(void);
			NxCCDSkeleton*     getAsSkeletonMesh(void);
			NxClothMesh*       getAsClothMesh(void);
			NxSoftBodyMesh*    getAsSoftBodyMesh(void);

		protected:

      void setName(const NxString&);

			/// \internal
			/// XNX<42>TXCO<coord_size 2 or 3><length (int) = coord_size * size_of float><data>
			void _load_TextureCoords(Resource*);

			/// \internal
			/// XNX<42>TXCO<coord_size 2 or 3><length (int) = coord_size * size_of float><data>
			void _write_TextureCoords(bool uv_or_uvw, Betajaen::Flat<float3>*, Resource*);

			/// \internal
			/// XNX<32>NRML<length = vertices * size_of float * 3><data>
			void _load_Normals(Resource*);

			/// \internal
			/// XNX<32>NRML<length = vertices * size_of float * 3><data>
			void _write_Normals(Betajaen::Flat<float3>*, Resource*);

			/// \internal
			void _copyTextureCoords(Betajaen::Flat<float3>*);

			/// \internal
			void _copyNormals(Betajaen::Flat<float3>*);


			NxTriangleMesh*                mTriangleMesh;
			NxConvexMesh*                  mConvexMesh;
			NxCCDSkeleton*                 mSkeletonMesh;
			NxClothMesh*                   mClothMesh;
			NxSoftBodyMesh*                mSoftBodyMesh;
			MeshType                       mType;
			NxU32                          mNbReferences;
			//Betajaen::Flat<float3>*        mTextureCoordinates;
			NxArray<float3>*               mTextureCoordinates;
			//Betajaen::Flat<float3>*        mNormals;
			NxArray<float3>*               mNormals;
			bool                           mIsPairTextureCoordinates;
			MaterialAlias*                 mMaterialAlias;
      NxString                       mName;

	};

}; // End of Resources namespaces.
}; // End of NxOgre namespaces.

#endif
