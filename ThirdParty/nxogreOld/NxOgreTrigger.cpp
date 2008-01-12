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
#include "NxOgreTrigger.h"
#include "NxOgreActor.h"			// Trigger Inherits Actors
#include "NxOgreScene.h"			// Scene owns Triggers
#include "NxOgreUserData.h"			// For NxActorUserData for NxActor
#include "NxOgreShapeBlueprint.h"
#include "NxOgreShape.h"
#include "NxOgreGroup.h"
#include "NxOgreDominanceGroup.h"
#include "NxOgrePose.h"

namespace NxOgre {

//////////////////////////////////////////////////////////////////////

Trigger::Trigger(const NxString& identifier, Scene* scene, ShapeBlueprint* shape, const Pose& pose, ActorParams params) : Actor(identifier, scene, false) {
	_createActor(shape, pose, params);
	mOwner->_registerTrigger(mName, this);
}

//////////////////////////////////////////////////////////////////////

Trigger::~Trigger() {
	mOwner->_unregisterTrigger(mName);
	_destroyActor();
}

//////////////////////////////////////////////////////////////////////

void Trigger::_createActor(ShapeBlueprint *shape, const Pose& pose, ActorParams params) {


	NxActorDesc ad;
	NxBodyDesc bd;

	mNxActorUserData = new NxActorUserData(this, NxActorUserData::T_Trigger);
#if NX_SDK_VERSION_NUMBER >= 260
	ad.compartment = params.mCompartment;
#endif
	ad.density = params.mDensity;

#if NX_SDK_VERSION_NUMBER >= 272 
	if (params.mDominanceGroupAsIndex != 0)
		ad.dominanceGroup = params.mDominanceGroupAsIndex;
	else if (params.mDominanceGroupAsName.length() > 0)
		ad.dominanceGroup = mOwner->getDominanceGroup(params.mDominanceGroupAsName)->getGroupID();
#endif

	ad.flags = params.mActorFlags;	
	
	ad.globalPose.t = pose.getVec3();
	ad.globalPose.M.fromQuat(pose.getQuat());
	
	if (params.mGroupAsIndex != 0)
		ad.group = params.mGroupAsIndex;
	else if (params.mGroupAsName.length() > 0)
		ad.group = mOwner->getActorGroup(params.mGroupAsName)->getGroupID();

	ad.userData = mNxActorUserData;

	if (params.mDensity == 0 && params.mMass == 0) {
		ad.body = NULL;
	}
	else {
		bd.angularDamping = params.mAngularDamping;
		bd.angularVelocity = params.mAngularVelocity;
		bd.CCDMotionThreshold = params.mCCDMotionThreshold;
		bd.flags = params.mBodyFlags;
		bd.linearDamping = params.mLinearDamping;
		bd.linearVelocity = params.mLinearVelocity;
		bd.mass = params.mMass;
		bd.massLocalPose = params.mMassLocalPose;
		bd.massSpaceInertia = params.mMassSpaceInertia;
		bd.maxAngularVelocity = params.mMaxAngularVelocity;
		bd.sleepAngularVelocity = params.mSleepAngularVelocity;
		bd.sleepDamping = params.mSleepDamping;
		bd.sleepEnergyThreshold = params.mSleepEnergyThreshold;
		bd.sleepLinearVelocity = params.mSleepLinearVelocity;
		bd.solverIterationCount = params.mSolverIterationCount;
		bd.wakeUpCounter = params.mWakeUpCounter;
		ad.body = &bd;
	}
	
	shape->_bindToActorDescription(this, 0, ad.shapes);

	// Set the Shapes to Triggers the really naughty way.
	for (NxU32 i=0;i < ad.shapes.size();i++)
		ad.shapes[i]->shapeFlags |= NX_TRIGGER_ENABLE;


	mActor = mOwner->mScene->createActor(ad);
	
    delete shape;

}

//////////////////////////////////////////////////////////

void Trigger::_destroyActor() {
	mOwner->mScene->releaseActor(*mActor);
	mActor=0;
	delete mNxActorUserData;
	mCollisionModel.destroyAllOwned();
}

//////////////////////////////////////////////////////////

void Trigger::onEnter(Actor* a) {
	if (mCallback) {
		mCallback->onEnter(this, a);
	}
}

//////////////////////////////////////////////////////////

void Trigger::onLeave(Actor* a) {
	if (mCallback)
		mCallback->onLeave(this, a);
}

//////////////////////////////////////////////////////////

void Trigger::onInside(Actor* a) {
	if (mCallback)
		mCallback->onInside(this, a);
}

//////////////////////////////////////////////////////////

void Trigger::onEnter(Character* a) {
	if (mCallback) {
		mCallback->onEnter(this, a);
	}
}

//////////////////////////////////////////////////////////

void Trigger::onLeave(Character* a) {
	if (mCallback)
		mCallback->onLeave(this, a);
}

//////////////////////////////////////////////////////////

void Trigger::onInside(Character* a) {
	if (mCallback)
		mCallback->onInside(this, a);
}

//////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
