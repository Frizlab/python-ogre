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

#ifndef __NXOGRE_SCENE_BLUEPRINT_H__
#define __NXOGRE_SCENE_BLUEPRINT_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreBlueprint.h"
#include "NxOgreScene.h"
#include "NxOgreEnumsBlueprint.h"		// For: Flags

namespace NxOgre {

	namespace Blueprints {

		class NxExport SceneBlueprint : public Blueprint {

			public:

				struct ShapeGroupBlueprint {
					NxString ID;
					NxCollisionGroup Group;
				};

				typedef NxArray<ShapeGroupBlueprint> ShapeGroupBlueprints ;

				struct ActorGroupBlueprint {
					NxString ID;
					NxActorGroup Group;
				};

				typedef NxArray<ActorGroupBlueprint> ActorGroupBlueprints ;

				/////////////////////////////////////////////////////////////

				void empty();
				void serialise(Scene*);
				void unserialise(World*);

				/////////////////////////////////////////////////////////////

				SceneBlueprint();
				~SceneBlueprint();
			    
				/////////////////////////////////////////////////////////////

				NxString						mName;
				EnumsBlueprint					mFilterOp0;
				EnumsBlueprint					mFilterOp1;
				EnumsBlueprint					mFilterOp2;
				NxGroupsMask					mGroupMask0;
				NxGroupsMask					mGroupMask1;
				NxVec3							mGravity;
				NxF32							mMaxTimestep;
				NxU32							mMaxIter;
				EnumsBlueprint					mTimeStepMethod;
				NxBounds3						mMaxBounds;
				NxSceneLimits					mLimits;
				EnumsBlueprint					mSimType;
				bool							mGroundPlane;
				bool							mBoundsPlanes;
				EnumsBlueprint					mFlags;
				NxU32							mInternalThreadCount;
				NxU32							mBackgroundThreadCount;
				NxU32							mThreadMask;
				NxU32							mBackgroundThreadMask;

#if NX_SDK_VERSION_NUMBER >= 250
				NxU32							mSimThreadStackSize;
#endif

#if NX_SDK_VERSION_NUMBER >= 260
				EnumsBlueprint					mSimThreadPriority;
#endif

#if NX_SDK_VERSION_NUMBER >= 250
				NxU32							mWorkerThreadStackSize;
#endif

#if NX_SDK_VERSION_NUMBER >= 250
				EnumsBlueprint					mWorkerThreadPriority;
#endif

#if NX_SDK_VERSION_NUMBER >= 262
				NxU32							mUpAxis;
#endif

#if NX_SDK_VERSION_NUMBER >= 262
				NxU32							mSubdivisionLevel;
#endif

#if NX_SDK_VERSION_NUMBER >= 262
				EnumsBlueprint					mStaticStructure;
#endif
#if NX_SDK_VERSION_NUMBER >= 262
				EnumsBlueprint					mDynamicStructure;
#endif

				// NxArray< NxMaterialDesc * >                    mMaterials;
				
//////////				List<Blueprints::ActorBlueprint*>				  mActors;

				// NxArray< NxJointDesc *>                        mJoints;
				
				// NxArray< NxPairFlagDesc *>                     mPairFlags;

				// NxArray< NxSpringAndDamperEffectorDesc *>      mEffectors;

#if NX_SDK_VERSION_NUMBER >= 270
				// NxArray< NxForceFieldDesc *>                   mForceFields;
#endif

				// NxArray< NxCollisionGroupDesc *>               mCollisionGroups;

				ShapeGroupBlueprints							  mShapeGroups;
				ActorGroupBlueprints							  mActorGroups;
				// NxArray< NxActorGroupPair *>                   mActorGroupPairs;

#if NX_SDK_VERSION_NUMBER >= 260
				// NxArray< NxCompartmentDesc *>                  mCompartments;
#endif

#if NX_USE_FLUID_API
				// NxArray< NxFluidDesc *>                        mFluids;
#endif

				// NxArray< NxClothDesc *>                        mCloths;

#if NX_USE_SOFTBODY_API
				// NxArray< NxSoftBodyDesc *>                     mSoftBodies;
#endif

#if NX_SDK_VERSION_NUMBER >= 272
				// NxArray< NxConstraintDominanceDesc * >         mDominanceGroupPairs;
#endif

				/////////////////////////////////////////////////////////////

				void parseNxSceneDesc(NxSceneDesc&);
				void toNxSceneDesc(NxSceneDesc&);

				/////////////////////////////////////////////////////////////

				World* mWorld;

		};// End of SceneBlueprint Class
	
	};// End of Serialiser namespace

};// End of namespace

#endif
