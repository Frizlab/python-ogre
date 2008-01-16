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

#ifndef __NXOGRE_CLOTH_H__
#define __NXOGRE_CLOTH_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreParams.h"

//#if (NX_USE_CLOTH_API == 1)

namespace NxOgre {


	class NxExport ClothParams : public Params {

		public:


			ClothParams() {}
			ClothParams(const char* p)	{setToDefault();process(p);}
			ClothParams(NxString p)		{setToDefault();process(p);}

			void			setToDefault();
			void			parse(Parameters);			

			NxReal width;
			NxReal height;
			NxReal vertexDistance;
			NxString meshMaterial;

			NxReal thickness;
			NxReal density;
			NxReal bendingStiffness;
			NxReal stretchingStiffness;
			NxReal dampingCoefficient;
			NxReal friction;
			NxReal pressure;
			NxReal tearFactor;
			NxReal collisionResponseCoefficient;
			NxReal attachmentResponseCoefficient;
			NxReal attachmentTearFactor;
			NxReal toFluidResponseCoefficient;
			NxReal fromFluidResponseCoefficient;
			NxU32  solverIterations;
			NxVec3 externalAcceleration;
			NxReal wakeUpCounter;
			NxReal sleepLinearVelocity;
			NxCollisionGroup collisionGroup;
			NxGroupsMask groupsMask;
			NxBounds3 validBounds;
			NxU32 flags;
			
			bool tear;
			NxU32 tear_buffer_multiplier;

			bool bending;
			bool bending_ortho;
			bool gravity;
			bool collision_twoway;
			bool collision_self;
			bool is_static;
			bool pressurised;


	}; // End of ClothParams class

	class NxExport Cloth {

		public:

			Cloth(const NxString& identifier, Scene*, const Pose&, ClothParams = "");
			Cloth(const NxString& identifier, const NxString& meshName, Scene*, const Pose&, ClothParams = "");
			Cloth(const NxString& identifier, NxClothDesc, NxMeshData, Ogre::MeshPtr, const Pose&);
			~Cloth();

			//////////////////////////////////////////////////////////

			NxString		getName() const{return mName;}
			Scene*			getScene() {return mOwner;}
			NxScene*		getNxScene();


			Cloth*			duplicate(const NxString& identifier, const Pose&);

			void			simulate(NxReal);
			void			render(NxReal);

			NxClothMesh*	getClothMesh();
			void			setBendingStiffness(NxReal stiffness);
			NxReal			getBendingStiffness() const;
			void			setStretchingStiffness(NxReal stiffness);
			NxReal			getStretchingStiffness() const;
			void			setDampingCoefficient(NxReal dampingCoefficient);
			NxReal			getDampingCoefficient() const;
			void			setFriction(NxReal friction);
			NxReal			getFriction() const;
			void			setPressure(NxReal pressure);
			NxReal			getPressure() const;
			void			setTearFactor(NxReal factor);
			NxReal			getTearFactor() const;
			void			setAttachmentTearFactor(NxReal factor);
			NxReal			getAttachmentTearFactor() const;
			void			setThickness(NxReal thickness);
			NxReal			getThickness() const;
			NxReal			getDensity() const;
			NxU32			getSolverIterations() const;
			void			setSolverIterations(NxU32 iterations);
			void			getWorldBounds(NxBounds3& bounds) const;
			void			attachToShape(const NxShape *shape, NxU32 attachmentFlags);
			void			attachToCollidingShapes(NxU32 attachmentFlags);
			void			detachFromShape(const NxShape *shape);
			void			attachVertexToShape(NxU32 vertexId, const NxShape *shape, const NxVec3 &localPos, NxU32 attachmentFlags);
			void			attachVertexToGlobalPosition(const NxU32 vertexId, const NxVec3 &pos);
			void			freeVertex(const NxU32 vertexId);
			void			attachToCore(NxActor *actor, NxReal impulseThreshold, NxReal penetrationDepth = 0.0f);
			bool			tearVertex(const NxU32 vertexId, const NxVec3 &normal);
			bool			raycast(const NxRay& worldRay, NxVec3 &hit, NxU32 &vertexId);
			void			setGroup(NxCollisionGroup collisionGroup);
			NxCollisionGroup	getGroup() const;
			void			setGroupsMask(const NxGroupsMask& groupsMask);
			const NxGroupsMask getGroupsMask() const;
			void			setMeshData(NxMeshData& meshData);
			NxMeshData		getMeshData();
			void			setValidBounds(const NxBounds3& validBounds);
			void			getValidBounds(NxBounds3& validBounds) const;
			void			setPositions(void* buffer, NxU32 byteStride = sizeof(NxVec3));
			void			getPositions(void* buffer, NxU32 byteStride = sizeof(NxVec3));
			void			setVelocities(void* buffer, NxU32 byteStride = sizeof(NxVec3));
			void			getVelocities(void* buffer, NxU32 byteStride = sizeof(NxVec3));
			NxU32			getNumberOfParticles();
			NxU32			queryShapePointers();
			NxU32			getStateByteSize();
			void			getShapePointers(NxShape** shapePointers,NxU32 *flags=0);
			void			setShapePointers(NxShape** shapePointers,unsigned int numShapes);
			void			saveStateToStream(NxStream& stream);
			void			loadStateFromStream(NxStream& stream);
			void			setCollisionResponseCoefficient(NxReal coefficient);
			NxReal			getCollisionResponseCoefficient() const;
			void			setAttachmentResponseCoefficient(NxReal coefficient);
			NxReal			getAttachmentResponseCoefficient() const;
			void			setFromFluidResponseCoefficient(NxReal coefficient);
			NxReal			getFromFluidResponseCoefficient() const;
			void			setToFluidResponseCoefficient(NxReal coefficient);
			NxReal			getToFluidResponseCoefficient() const;
			void			setExternalAcceleration(NxVec3 acceleration);
			NxVec3			getExternalAcceleration() const;
			bool			isSleeping() const ;
			NxReal			getSleepLinearVelocity() const;
			void			setSleepLinearVelocity(NxReal threshold);
			void			wakeUp(NxReal wakeCounterValue = NX_SLEEP_INTERVAL);
			void			putToSleep();
			void			setFlags(NxU32 flags);
			NxU32			getFlags() const;
			void			addForceAtVertex(const Ogre::Vector3& force, NxU32 vertexId, NxForceMode mode = NX_FORCE);
			void			addForceAtPos(const NxVec3& position, NxReal magnitude, NxReal radius, NxForceMode mode = NX_FORCE);
			bool			overlapAABBTriangles(const NxBounds3& bounds, NxU32& nb, const NxU32*& indices) const;
#if NX_SDK_VERSION_NUMBER >= 260
			NxCompartment*	getCompartment() const;
#endif
			unsigned int	getNbVertices();

			enum HardwareBinding {
				HWB_VERTEX = 0,
				HWB_NORMAL = 1,
				HWB_TEXTURE = 2
			};

			enum Tearing {
				TR_FACTOR = 2
			};

			struct VertexCorner {
				NxU32 topLeft;
				NxU32 topRight;
				NxU32 bottomLeft;
				NxU32 bottomRight;
			};


			VertexCorner				mCorners;

		protected:

			void						__createCloth(ClothParams);
			void						__ParamsToDescription(ClothParams params, const Pose& pose);
			void						__destroyCloth();
			NxClothMesh*				__createClothMesh(NxReal width, NxReal height, NxReal vDistance);
			NxClothMesh*				__createClothMeshFromOgreMesh(const NxString& name, const NxString& mesh);
			void						__createMesh(const NxString& name);

			Scene					   *mOwner;
			NxActor*					mActor;
			NxString					mName;
			
			NxCloth*					mCloth;
			NxClothDesc					mClothDescription;
			NxClothMesh*				mClothMesh;
			Ogre::MeshPtr				mMesh;
			Ogre::SubMesh*				mSubMesh;
			NxMeshData					mReceiveBuffers;
			NxBounds3					mNxBounds;
			Ogre::AxisAlignedBox		mAABox;
			bool						mUpdateBB;
			bool						mIsTearable;

			NxU32*						mTriangles;
			NxVec3*						mVertices;
			Ogre::Vector3*				mNormals;
			Ogre::Vector2*				mTextCoords;
			NxU32						mVertexCount;
			NxU32						mTriangleCount;
			NxU32						mTextCoordCount;
			NxU32						mNormalCount;
			NxU32						mTearFactor;
			Ogre::IndexData*			mIndexData;
			Ogre::VertexData*			mVertexData;
			Ogre::SceneNode*			mNode;
			Ogre::Entity*				mEntity;

		private:


	};// End of Cloth Class

	// Cloth Vertex Helper Class
	struct NxExport ClothVertex {
		Cloth*	cloth;
		NxU32	vertex;
		
		Ogre::Vector3 getGlobalPosition() const;
		void setGlobalPosition(const Ogre::Vector3& worldPosition);
		void attachToShape(Shape*, const Ogre::Vector3& localPosition);
		void attachToWorld(const Ogre::Vector3& worldPosition);
		void freeVertex();
		void tear(const Ogre::Vector3& normal);
		
		// Set Velocity of a specific vertex.
		// This function is quite complex. If you need to set velocities of one or more vertexes, set it within the Cloth.
		void setVelocity(const Ogre::Vector3& velocity);
	};


};// End of namespace
//#endif
#endif
