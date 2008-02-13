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

#ifndef __NXOGRE_ACTOR_BLUEPRINT_H__
#define __NXOGRE_ACTOR_BLUEPRINT_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreBlueprint.h"
#include "NxOgreContainer.h"
#include "NxOgreEnumsBlueprint.h"			// For: mFlags and mBodyFlags
namespace NxOgre {

	namespace Blueprints {


		class NxExport ActorFactory {

			public:

				ActorFactory();
				~ActorFactory();

				void registerFactory(NxString actorType);

				virtual bool createActor(NxString ID, Blueprints::ActorBlueprint*, NxActor*, Scene*) {return false;}

		}; // ActorFactory

		class NxExport ActorBlueprint : public Blueprint {

			public:

				ActorBlueprint();
				~ActorBlueprint();

				void empty();
				// Serialises the Actor. No mesh storage.
				void serialise(Actor*, Scene*);
				// Serialise the actor w/shapes, mesh storage
				void serialise(Actor*, SceneBlueprint*);

				
				// Serialise to a current scene with a new name
				Actor* unserialise(NxString, Scene*);

				// Serialise to a scene with the current name.
				void unserialise(Scene*);

				void fromNxActorDesc(NxActorDesc&);
				void fromNxBodyDesc(NxBodyDesc&);

				void toNxActorDesc(NxActorDesc&);
				void toNxBodyDesc(NxBodyDesc&);
				
				////////////////////////////////////////////////

				NxString					mName;
				StringPairList				mCustom;
				NxMat34						mGlobalPose;
				bool						mBody;
				NxF32						mDensity;
				EnumsBlueprint				mFlags;
				NxActorGroup				mGroup;

#if NX_SDK_VERSION_NUMBER >= 260
				NxString					mCompartment;
#endif

				CollisionModel	mShapes;

#if NX_SDK_VERSION_NUMBER >= 272
				NxU16						mDominanceGroup;
#endif


				//////////////////////////////////////////////////
				
				NxMat34						mMassLocalPose;
				NxVec3						mMassSpaceInertia;
				NxF32						mMass;
				NxVec3						mLinearVelocity;
				NxVec3						mAngularVelocity;
				NxF32						mWakeUpCounter;
				NxF32						mLinearDamping;
				NxF32						mAngularDamping;
				NxF32						mMaxAngularVelocity;
				NxF32						mCCDMotionThreshold;
				EnumsBlueprint				mBodyFlags;
				NxF32						mSleepLinearVelocity;
				NxF32						mSleepAngularVelocity;
				NxU32						mSolverIterationCount;

#if NX_SDK_VERSION_NUMBER >= 250
				NxF32						mSleepEnergyThreshold;
#endif

#if NX_SDK_VERSION_NUMBER >= 250
				NxF32						mSleepDamping;
#endif

				//////////////////////////////////////////////////

				StringPairList mCustomData;

		};// End of ActorBlueprint Class
	
	};
};// End of namespace

#endif
