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
#include "NxOgreActorBlueprint.h"
#include "NxOgreSceneBlueprint.h"
#include "NxOgreEnumsBlueprint.h"
#include "NxOgreContainer.h"
#include "NxOgreActor.h"
#include "NxOgreBody.h"
#include "NxOgreScene.h"
#include "NxOgreShape.h"
#include "NxOgreShapeBlueprint.h"
#include "NxOgrePose.h"

namespace NxOgre {
namespace Blueprints {

////////////////////////////////////////////////////////////////////////////////////////////////

ActorBlueprint::ActorBlueprint() : 
	mFlags(EnumsBlueprint::FT_NxActorFlag),
	mBodyFlags(EnumsBlueprint::FT_NxBodyFlag)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////

ActorBlueprint::~ActorBlueprint() {
	empty();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ActorBlueprint::empty() {

	// Reset Descriptions
	NxActorDesc desc;
	desc.setToDefault();
	fromNxActorDesc(desc);
	mBody = false;
	
	NxBodyDesc bdesc;
	bdesc.setToDefault();
	fromNxBodyDesc(bdesc);

	// Clear shapes.

	mShapes.destroyAndEraseAll();

}

////////////////////////////////////////////////////////////////////////////////////////////////

void ActorBlueprint::serialise(Actor* actor, Scene* scene) {
	//
	empty();

	mName = actor->getName();
	
	NxActorDesc desc;
	actor->getNxActor()->saveToDesc(desc);
	fromNxActorDesc(desc);

	if (actor->getNxActor()->isDynamic()) {
		mBody = true;
		NxBodyDesc bdesc;
		actor->getNxActor()->saveBodyToDesc(bdesc);
		fromNxBodyDesc(bdesc);
	}
	else {
		mBody = false;
	}

	mCustom = actor->saveCustom();

	// shapes and stuff here...

	CollisionModel shapes;
	actor->mCollisionModel.CopyTo(shapes);
	
	NxShapeIndex index = 0;
	for(Shape* shape = shapes.begin();shape = shapes.next();) {
		ShapeBlueprint* shapeb = shape->getBlueprint();
		shapeb->setShared(true);
		mShapes.insert(index, shapeb);
		index++;
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

void ActorBlueprint::serialise(Actor* actor, SceneBlueprint* bp) {
	empty();

	mName = actor->getName();
	
	NxActorDesc desc;
	actor->getNxActor()->saveToDesc(desc);
	fromNxActorDesc(desc);

	if (actor->getNxActor()->isDynamic()) {
		mBody = true;
		NxBodyDesc bdesc;
		actor->getNxActor()->saveBodyToDesc(bdesc);
		fromNxBodyDesc(bdesc);
	}
	else {
		mBody = false;
	}

	mCustom = actor->saveCustom();

	// shapes and stuff here...

	CollisionModel shapes;
	actor->mCollisionModel.CopyTo(shapes);
	
	std::cout << "Collision Model size = " << shapes.count() << std::endl;

	NxShapeIndex index = 0;
	for(Shape* shape = shapes.begin();shape = shapes.next();) {
		ShapeBlueprint* shapeb = shape->getBlueprint();
		shapeb->setShared(true);
		mShapes.insert(index, shapeb);
		index++;
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

Actor* ActorBlueprint::unserialise(NxString name, Scene* s) {

	NxString type = "Actor";
	if (mCustom.hasKey("ActorType")) {
		type = mCustom.getFirstOf("ActorType");
	}

	
	ActorParams params;
	params.setToDefault();

	NxActorDesc ad;
	toNxActorDesc(ad);
	params.fromNxActorDesc(ad);

	if (mDensity == 0 && mMass == 0) {
		ad.body = NULL;
	}
	else {
		NxBodyDesc bd;
		toNxBodyDesc(bd);
		params.fromNxBodyDesc(bd);
	}


	Actor *a;
	NxOgre::Pose actorPose;
	actorPose.fromMat34(mGlobalPose);

	if (type == "Actor") {
		a = s->createActor(name, mShapes.getFirst(), actorPose, params); 
	}
	else if (type == "Body") {
		a = s->createBody(name, mShapes.getFirst(), actorPose, params);
	}

	if (a->isDead()) {
		NxThrow_Error("Actor unserialisation failed!");
		s->destroyActor(a->getName());
		return NULL;
	}

	a->restoreCustom(mCustom);

	return a;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ActorBlueprint::fromNxActorDesc(NxActorDesc& desc) {

	mGlobalPose = desc.globalPose;
	mDensity = desc.density;
	mFlags.setFlag( EnumsBlueprint::FT_NxActorFlag, desc.flags);
	mGroup = desc.group;

#if 0
	#if NX_SDK_VERSION_NUMBER >= 272
		desc.dominanceGroup = dominanceGroup;
	#endif
#endif

}

////////////////////////////////////////////////////////////////////////////////////////////////

void ActorBlueprint::fromNxBodyDesc(NxBodyDesc& desc) {


	mMassLocalPose = desc.massLocalPose;
	mMassSpaceInertia = desc.massSpaceInertia;
	mMass = desc.mass;
	mLinearVelocity = desc.linearVelocity;
	mAngularVelocity = desc.angularVelocity;
	mWakeUpCounter = desc.wakeUpCounter;
	mLinearDamping = desc.linearDamping;
	mAngularDamping = desc.angularDamping;
	mMaxAngularVelocity = desc.maxAngularVelocity;
	mCCDMotionThreshold = desc.CCDMotionThreshold;
	mBodyFlags.setFlag(EnumsBlueprint::FT_NxBodyFlag, desc.flags);
	mSleepLinearVelocity = desc.sleepLinearVelocity;
	mSleepAngularVelocity = desc.sleepAngularVelocity;
	mSolverIterationCount = desc.solverIterationCount;
	
	#if NX_SDK_VERSION_NUMBER >= 250
		mSleepEnergyThreshold = desc.sleepEnergyThreshold;
	#endif

	#if NX_SDK_VERSION_NUMBER >= 250
		mSleepDamping = desc.sleepDamping;
	#endif

}

////////////////////////////////////////////////////////////////////////////////////////////////

void ActorBlueprint::toNxActorDesc(NxActorDesc& desc) {

	//	desc.name = 0; Some Global const char* storage here.
	desc.globalPose = mGlobalPose;
	desc.density = mDensity;
	desc.flags = mFlags.getFlag();
	desc.group = mGroup;

#if 0
	#if NX_SDK_VERSION_NUMBER >= 272
		desc.dominanceGroup = mDominanceGroup;
	#endif
#endif

}

////////////////////////////////////////////////////////////////////////////////////////////////

void ActorBlueprint::toNxBodyDesc(NxBodyDesc& desc) {
 
	desc.massLocalPose = mMassLocalPose;
	desc.massSpaceInertia = mMassSpaceInertia;
	desc.mass = mMass;
	desc.linearVelocity = mLinearVelocity;
	desc.angularVelocity = mAngularVelocity;
	desc.wakeUpCounter = mWakeUpCounter;
	desc.linearDamping = mLinearDamping;
	desc.angularDamping = mAngularDamping;
	desc.maxAngularVelocity = mMaxAngularVelocity;
	desc.CCDMotionThreshold = mCCDMotionThreshold;
	desc.flags = mBodyFlags.getFlag();
	desc.sleepLinearVelocity = mSleepLinearVelocity;
	desc.sleepAngularVelocity = mSleepAngularVelocity;
	desc.solverIterationCount = mSolverIterationCount;

#if NX_SDK_VERSION_NUMBER >= 250
	desc.sleepEnergyThreshold = mSleepEnergyThreshold;
#endif

#if NX_SDK_VERSION_NUMBER >= 250
	desc.sleepDamping = mSleepDamping;
#endif

}

////////////////////////////////////////////////////////////////////////////////////////////////

}  //End of Serialiser namespace.
}  //End of NxOgre namespace.
