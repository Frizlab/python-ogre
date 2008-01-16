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
#include "NxOgreForceField.h"
#include "NxOgreActor.h"			// ForceField Inherits Actors
#include "NxOgreScene.h"			// Scene owns ForceFields
#include "NxOgreUserData.h"			// For NxActorUserData for NxActor
#include "NxOgreShapeBlueprint.h"
#include "NxOgreShape.h"
#include "NxOgreGroup.h"
#include "NxOgreDominanceGroup.h"
#include "NxOgrePose.h"
#include "NxOgreCooking.h"
#include "NxOgreHelpers.h"
#include "NxOgreSimpleShape.h"

namespace NxOgre {
	
//////////////////////////////////////////////////////////////////////

void ForceFieldParams::setToDefault() {

	pose.id();

	coordinates = NX_FFC_CARTESIAN;

	group				= 0;
	groupsMask.bits0	= 0;
	groupsMask.bits1	= 0;
	groupsMask.bits2	= 0;
	groupsMask.bits3	= 0;

	constant.zero(); 
	positionMultiplier.zero();
	positionTarget.zero();
	velocityMultiplier.zero();
	velocityTarget.zero();
	falloffLinear.zero();
	falloffQuadratic.zero();
	noise.zero();
	torusRadius = 1.0f;

	flags = 0;

	fluidScale		= 1.0f;
	clothScale		= 1.0f;
	softBodyScale	= 1.0f;
	rigidBodyScale	= 1.0f;

}

//////////////////////////////////////////////////////////////////////

void ForceFieldParams::parseFormula(const ForceFieldFormula &ff) {
	coordinates = ff.coords;
	constant = ff.K;
	positionMultiplier = ff.Mp;
	positionTarget = ff.P;
	velocityMultiplier = ff.Mv;
	velocityTarget = ff.V;	
	noise = ff.Anoise;
	falloffLinear = ff.l;
	falloffQuadratic = ff.q;
}

//////////////////////////////////////////////////////////////////////

void ForceFieldFormula::asExplosive(NxReal explosiveConstant, Ogre::Vector3 velocityTarget, Ogre::Vector3 noise) {
	/*
	NxForceFieldDesc desc;
	desc.coordinates = NX_FFC_SPHERICAL;

	//constant force of 100 outwards
	desc.constant = NxVec3(100, 0, 0);

	//The forces do not depend on where the objects are positioned
	NxMat33 m;
	m.zero();
	desc.positionMultiplier = m;
	desc.noise = NxVec3(5,5,5); //adds a random noise on the forces to make the objects a little more chaotic

	//Set target velocity along the radius to 20
	desc.velocityTarget = NxVec3(20,0,0);
	m.diagonal(NxVec3(1,0,0)); //Acts with a force relative to the current velocity to reach the
							   //target velocities. 0 means that those components won't be affected
	desc.velocityMultiplier = m;

	//Attach the force field to an actor (kinematic) so that we can move it around 
	// (spawn the explosions in different places)
	desc.actor = actor;

	//Create the force field around origo
	desc.pose.id();

	gForceField = scene->createForceField(desc);
	*/
	
	coords = NX_FFC_SPHERICAL;
	K.set(explosiveConstant,0,0);
	V = toNxVec3(velocityTarget);
	Anoise = toNxVec3(noise);
	Mv.zero();
	Mv.diagonal(NxVec3(1,0,0));

}

//////////////////////////////////////////////////////////////////////

void ForceFieldParams::parse(Parameters P) {

	for (Parameters::iterator p = P.begin(); p != P.end();p++) {
	

	}

}


//////////////////////////////////////////////////////////////////////

ForceField::ForceField(const NxString& Identifier, Scene* scene) : Actor(Identifier, scene, false) {
	mActor = 0;
	mForceField = 0;
	mOwner->_registerForceField(mName, this);
}

//////////////////////////////////////////////////////////////////////

ForceField::ForceField(const NxString& identifier, Scene* scene, const Pose& pose, ShapeBlueprint* actorShape, ActorParams aparams, ForceFieldParams fparams) : Actor(identifier, scene) {
	_createActor(actorShape, pose, aparams);
	_createForceField(fparams);
	mOwner->_registerForceField(mName, this);
}

//////////////////////////////////////////////////////////////////////

ForceField::~ForceField() {
	mOwner->_unregisterForceField(mName);
	_destroyForceField();
	_destroyActor();
}

//////////////////////////////////////////////////////////////////////

void ForceField::_createActor(ShapeBlueprint *shape, const Pose& pose, ActorParams params) {


	NxActorDesc ad;
	NxBodyDesc bd;

	mNxActorUserData = new NxActorUserData(this, NxActorUserData::T_ForceField);
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

	mActor = mOwner->mScene->createActor(ad);

	
    delete shape;

}

//////////////////////////////////////////////////////////

void ForceField::_destroyActor() {
	mOwner->getNxScene()->releaseActor(*mActor);
	mActor=0;
	delete mNxActorUserData;
	mCollisionModel.destroyAllOwned();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ForceField::_createForceField(ForceFieldParams params) {

	NxForceFieldDesc desc;
	desc.setToDefault();
	desc.actor = mActor;
	desc.clothScale = params.clothScale;
	desc.constant = params.constant;
	desc.coordinates = params.coordinates;
	desc.falloffLinear = params.falloffLinear;
	desc.falloffQuadratic = params.falloffQuadratic;
	desc.flags = params.flags;
	desc.fluidScale = params.fluidScale;
	desc.group = params.group;
	desc.groupsMask = params.groupsMask;
	desc.noise = params.noise;
	desc.pose = mActor->getGlobalPose();
	desc.positionMultiplier = desc.positionMultiplier;
	desc.positionTarget = desc.positionTarget;
	desc.rigidBodyScale = desc.rigidBodyScale;
	desc.softBodyScale = desc.softBodyScale;
	desc.torusRadius = desc.torusRadius;
	desc.velocityMultiplier = desc.velocityMultiplier;
	desc.velocityTarget = desc.velocityTarget;
	
	mForceField = mOwner->getNxScene()->createForceField(desc);

}

////////////////////////////////////////////////////////////////////////////////////////////////

void ForceField::_destroyForceField() {
	mOwner->getNxScene()->releaseForceField(*mForceField);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ForceField::addForceFieldShape(SimpleShape* sh, bool Inclusion) {
	//sh->__bindToForceField(mForceField, Inclusion);
	
	switch(sh->getType()) {

		case SimpleShape::SST_Box: {
			SimpleBox* sb = sh->getAsBox();
			NxBoxForceFieldShapeDesc desc;
			desc.dimensions = sb->getDimensions();
			desc.pose = sb->getPose();
			
			if (sb->getInverse())
				desc.flags = NX_FFS_EXCLUDE;

		} break;

		case SimpleShape::SST_Sphere: {
			SimpleSphere* ss = sh->getAsSphere();
			NxSphereForceFieldShapeDesc desc;
			desc.radius = ss->getRadius();
			desc.pose = ss->getPose();
			
			if (ss->getInverse())
				desc.flags = NX_FFS_EXCLUDE;

		} break;

		case SimpleShape::SST_Capsule: {
			SimpleCapsule* sc = sh->getAsCapsule();
			NxCapsuleForceFieldShapeDesc desc;
			desc.radius = sc->getRadius();
			desc.height = sc->getHeight();
			desc.pose = sc->getPose();
			
			if (sc->getInverse())
				desc.flags = NX_FFS_EXCLUDE;

		} break;

	}

	delete sh;
}

#if 0
////////////////////////////////////////////////////////////////////////////////////////////////

NxForceFieldShape* ForceFieldCubeShape::__bindToForceField(NxForceField* ff, bool Inclusion) {

	if (!Inclusion)
		mDescription.flags = NX_FFS_EXCLUDE;

	return ff->createShape(mDescription);
	
}

////////////////////////////////////////////////////////////////////////////////////////////////

NxForceFieldShape* ForceFieldSphereShape::__bindToForceField(NxForceField* ff, bool Inclusion) {

	if (!Inclusion)
		mDescription.flags = NX_FFS_EXCLUDE;

	return ff->createShape(mDescription);

}

////////////////////////////////////////////////////////////////////////////////////////////////

NxForceFieldShape* ForceFieldCapsuleShape::__bindToForceField(NxForceField* ff, bool Inclusion) {

	if (!Inclusion)
		mDescription.flags = NX_FFS_EXCLUDE;

	return ff->createShape(mDescription);

}

////////////////////////////////////////////////////////////////////////////////////////////////

ForceFieldConvexShape::ForceFieldConvexShape(NxString meshName, Scene* scene, const Pose& p, const Ogre::Vector3& scale) {

	mDescription.setToDefault();
	mDescription.pose = p.toMat34();
	mDescription.meshData = NxGenerateConvexMeshFromOgreMesh(meshName, scene->getNxScene(), toNxVec3(scale));

}

////////////////////////////////////////////////////////////////////////////////////////////////

NxForceFieldShape* ForceFieldConvexShape::__bindToForceField(NxForceField* ff, bool Inclusion) {

	if (!Inclusion)
		mDescription.flags = NX_FFS_EXCLUDE;

	return ff->createShape(mDescription);

}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////

StringPairList ForceField::saveCustom() {

	StringPairList l;
	l.insert("ActorType", "ForceField");
	return l;
	
}

//////////////////////////////////////////////////////////

void ForceField::restoreCustom(StringPairList spl) {


}

////////////////////////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
