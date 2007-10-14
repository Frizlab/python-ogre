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

#include "NxOgreStable.h"
#include "NxOgreSceneBlueprint.h"
#include "NxOgreScene.h"
#include "NxOgreActorBlueprint.h"
#include "NxOgreEnumsBlueprint.h"
#include "NxOgreContainer.h"

namespace NxOgre {
namespace Blueprints {

////////////////////////////////////////////////////////////////////////////////////////////////

SceneBlueprint::SceneBlueprint() : 
	mFlags(EnumsBlueprint::FT_NxSceneFlags),
	mStaticStructure(EnumsBlueprint::FT_NxPruningStructure), 
	mDynamicStructure(EnumsBlueprint::FT_NxPruningStructure), 
	mWorkerThreadPriority(EnumsBlueprint::FT_NxThreadPriority)
{
	
}

////////////////////////////////////////////////////////////////////////////////////////////////

SceneBlueprint::~SceneBlueprint() {
	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void SceneBlueprint::empty() {

	// Clear actors

	mActors.destroyAndEraseAll();

	mActors.empty();
	mFlags.empty();

}

////////////////////////////////////////////////////////////////////////////////////////////////

void SceneBlueprint::serialise(Scene* s) {
	empty();

	mWorld = s->getWorld();
	mName = s->getName();

	// Pass over Scene
	NxSceneDesc desc;
	s->getNxScene()->saveToDesc(desc);
	parseNxSceneDesc(desc);

	// Actors.
	Actors actors;
	s->getActors()->CopyTo(actors);
	
	for (Actor* actor = actors.begin();actor = actors.next();) {
		ActorBlueprint* bp = new ActorBlueprint();
	}

	// Materials

	// ShapeGroups

	// ActorGroups

	// ForceFields

	// Cloth

	// Fluids

	// ...
}

////////////////////////////////////////////////////////////////////////////////////////////////

void SceneBlueprint::unserialise(World* w) {
	mWorld = w;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void SceneBlueprint::parseNxSceneDesc(NxSceneDesc& desc) {
	
  mGravity = desc.gravity;
  mMaxTimestep = desc.maxTimestep;
  mMaxIter = desc.maxIter;
  mTimeStepMethod.setFlag( EnumsBlueprint::FT_NxTimeStepMethod, desc.timeStepMethod);
  mSimType.setFlag( EnumsBlueprint::FT_NxSimulationType, desc.simType);
  mGroundPlane = desc.groundPlane;
  mBoundsPlanes = desc.boundsPlanes;
  mFlags.setFlag( EnumsBlueprint::FT_NxSceneFlags, desc.flags);
  mInternalThreadCount = desc.internalThreadCount;
  mBackgroundThreadCount = desc.backgroundThreadCount;
  mThreadMask = desc.threadMask;
  mBackgroundThreadMask = desc.backgroundThreadMask;
#if NX_SDK_VERSION_NUMBER >= 250
  mSimThreadStackSize = desc.simThreadStackSize;
#endif
#if NX_SDK_VERSION_NUMBER >= 260
  mSimThreadPriority.setFlag( EnumsBlueprint::FT_NxThreadPriority, desc.simThreadPriority);
#endif
#if NX_SDK_VERSION_NUMBER >= 250
  mWorkerThreadStackSize = desc.workerThreadStackSize;
#endif
#if NX_SDK_VERSION_NUMBER >= 250
  mWorkerThreadPriority.setFlag( EnumsBlueprint::FT_NxThreadPriority, desc.workerThreadPriority);
#endif
#if NX_SDK_VERSION_NUMBER >= 262
  mUpAxis = desc.upAxis;
#endif
#if NX_SDK_VERSION_NUMBER >= 262
  mSubdivisionLevel = desc.subdivisionLevel;
#endif
#if NX_SDK_VERSION_NUMBER >= 262
  mStaticStructure.setFlag( EnumsBlueprint::FT_NxPruningStructure, desc.staticStructure);
#endif
#if NX_SDK_VERSION_NUMBER >= 262
  mDynamicStructure.setFlag( EnumsBlueprint::FT_NxPruningStructure, desc.dynamicStructure);
#endif

}

////////////////////////////////////////////////////////////////////////////////////////////////

void SceneBlueprint::toNxSceneDesc(NxSceneDesc& desc) {
 desc.gravity = mGravity;
  desc.maxTimestep = mMaxTimestep;
  desc.maxIter = mMaxIter;
  desc.timeStepMethod = (NxTimeStepMethod) mTimeStepMethod.getFlag();
  desc.simType = (NxSimulationType) mSimType.getFlag();

  desc.groundPlane = mGroundPlane;
  desc.boundsPlanes = mBoundsPlanes;
  desc.flags = (NxSceneFlags) mFlags.getFlag();

  desc.internalThreadCount = mInternalThreadCount;
  desc.backgroundThreadCount = mBackgroundThreadCount;
  desc.threadMask = mThreadMask;
  desc.backgroundThreadMask = mBackgroundThreadMask;
#if NX_SDK_VERSION_NUMBER >= 250
  desc.simThreadStackSize = mSimThreadStackSize;
#endif

#if NX_SDK_VERSION_NUMBER >= 260
  desc.simThreadPriority = (NxThreadPriority) mSimThreadPriority.getFlag();
#endif

#if NX_SDK_VERSION_NUMBER >= 250
	desc.workerThreadStackSize = mWorkerThreadStackSize;
#endif

#if NX_SDK_VERSION_NUMBER >= 250
	desc.workerThreadPriority = (NxThreadPriority) mWorkerThreadPriority.getFlag();
#endif

#if NX_SDK_VERSION_NUMBER >= 262
	desc.upAxis = mUpAxis;
#endif
#if NX_SDK_VERSION_NUMBER >= 262
	desc.subdivisionLevel = mSubdivisionLevel;
#endif
#if NX_SDK_VERSION_NUMBER >= 262
	desc.staticStructure = (NxPruningStructure) mStaticStructure.getFlag();
#endif
#if NX_SDK_VERSION_NUMBER >= 262
	desc.dynamicStructure = (NxPruningStructure) mDynamicStructure.getFlag();
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////

}  //End of Serialiser namespace.
}  //End of NxOgre namespace.
