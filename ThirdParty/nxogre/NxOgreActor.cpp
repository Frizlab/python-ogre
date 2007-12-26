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
#include "NxOgreActor.h"
#include "NxOgreActorBlueprint.h"
#include "NxOgreUserData.h"				// For NxActorUserData for NxActor
#include "NxOgrePose.h"					// For conversions
#include "NxOgreHelpers.h"				// For conversions
#include "NxOgreScene.h"				// For Actor::mOwner
#include "NxOgreShapeBlueprint.h"		// For ShapeBlueprint System
#include "NxOgreDominanceGroup.h"		// For Dominance Groups
#include "NxOgreGroup.h"				// For ActorGroup and ShapeGroups
#include "NxOgreContainer.h"			// For Getting an ActorGroup
#include "NxOgreActorBlueprint.h"		// For Quick Duplication.

#include "OgreStringConverter.h"
#include "OgreRoot.h"
#include "OgreSingleton.h"
#include "OgreMatrix3.h"

namespace NxOgre {

//////////////////////////////////////////////////////////

void ActorParams::setToDefault() {


	mDensity				 = 0.0f;
	mActorFlags				 = 0;
	mGroupAsIndex			 = 0;
	mGroupAsName			 = "";
#if NX_SDK_VERSION_NUMBER >= 272 
	mDominanceGroupAsIndex	 = 0;
	mDominanceGroupAsName	 = "";
#endif
#if NX_SDK_VERSION_NUMBER >= 260
	mCompartment			 = NULL;
#endif
	mMassLocalPose			 . id();
	mMassSpaceInertia		 . zero();
	mLinearVelocity			 . zero();
	mAngularVelocity		 . zero();
	mWakeUpCounter			 = 20.0f * 0.02f;
	mMass					 = 10.0f;
	mLinearDamping			 = 0.0f;
	mAngularDamping			 = 0.05f;
	mMaxAngularVelocity		 = -1.0f;
	mBodyFlags				 = NX_BF_VISUALIZATION;
	mBodyFlags				|= NX_BF_ENERGY_SLEEP_TEST;
	mSleepLinearVelocity	 = -1.0f;
	mSleepAngularVelocity	 = -1.0f;
	mCCDMotionThreshold		 = 0.0f;
	mSolverIterationCount    = 4;
	mSleepEnergyThreshold	 = -1.0f;
	mSleepDamping			 = 0.0f;

	mNodeScale				 = Ogre::Vector3(1,1,1);
	mNodeName				 = "";
	mNodeShadows			 = true;
	mNodePose				 . id();

}

//////////////////////////////////////////////////////////

void ActorParams::parse(Parameters P) {

	for (Parameters::iterator p = P.begin(); p != P.end();p++) {

		if ((*p).first == "static") {
			if ( (*p).second.substr(0,1) == "y" || (*p).second.substr(0,1) == "Y") {
				mMass = 0;
				mDensity = 0;
				continue;
			}
			else {
				mMass = 10;
				mDensity = 0;
			}
		}
		
		if ((*p).first == "kinematic") {
			if ( (*p).second.substr(0,1) == "y" || (*p).second.substr(0,1) == "Y") {
				mBodyFlags |= NX_BF_KINEMATIC;
				if (mMass == 0 || mDensity == 0) {
					mMass = 1;
					mDensity = 0;
				}
			}
		}
		
		if (Set("density", (*p), mDensity))	continue;
		if (Set("group", (*p), mGroupAsName))	continue;
		if (Set("group-index", (*p), mGroupAsIndex))	continue;
#if NX_SDK_VERSION_NUMBER >= 272 
		if (Set("dominancegroup", (*p), mDominanceGroupAsName)) continue;
		if (Set("dominancegroup-index", (*p), mDominanceGroupAsIndex))	continue;
#endif
		if (Set("massspaceinertia", (*p), mMassSpaceInertia))	continue;
		if (Set("linearvelocity", (*p), mLinearVelocity)) continue;
		if (Set("angularvelocity", (*p), mAngularVelocity)) continue;
		if (Set("wakeupcounter", (*p), mWakeUpCounter)) continue;
		if (Set("mass", (*p), mMass)) continue;
		if (Set("lineardamping", (*p), mLinearDamping)) continue;
		if (Set("angulardamping", (*p), mAngularDamping)) continue;
		if (Set("maxangularvelocity", (*p), mMaxAngularVelocity)) continue;
		if (Set("sleepangularvelocity", (*p), mSleepAngularVelocity)) continue;
		if (Set("ccdmotionthreshold", (*p), mCCDMotionThreshold)) continue;
		if (Set("solveriterationcount", (*p), mSolverIterationCount)) continue;
		if (Set("sleepenergythreshold", (*p), mSleepEnergyThreshold)) continue;
		if (Set("sleepdamping", (*p), mSleepDamping)) continue;
		if (Set("node-scale", (*p), mNodeScale)) continue;
		if (Set("node-shadows", (*p), mNodeShadows)) continue;
		if (Set("node", (*p), mNodeName)) continue;
		if (Set("node-orientation", (*p), mNodePose.q)) continue;
		if (Set("node-offset", (*p), mNodePose.v)) continue;

	}

}

//////////////////////////////////////////////////////////

void ActorParams::fromNxActorDesc(NxActorDesc& desc) {
	
	mDensity				= desc.density;
	mActorFlags				= desc.flags;
	mGroupAsIndex			= desc.group;
#if NX_SDK_VERSION_NUMBER >= 272 
	mDominanceGroupAsIndex	= desc.dominanceGroup;
#endif
#if NX_SDK_VERSION_NUMBER >= 260
	mCompartment			= desc.compartment;
#endif
}

//////////////////////////////////////////////////////////

void ActorParams::fromNxBodyDesc(NxBodyDesc& desc) {

	mMassLocalPose			= desc.massLocalPose;
	mMassSpaceInertia		= desc.massSpaceInertia;
	mLinearVelocity			= desc.linearVelocity;
	mAngularVelocity		= desc.angularVelocity;
	mWakeUpCounter			= desc.wakeUpCounter;
	mMass					= desc.mass;
	mLinearDamping			= desc.linearDamping;
	mAngularDamping			= desc.angularDamping;
	mMaxAngularVelocity		= desc.maxAngularVelocity;
	mBodyFlags				= desc.flags;
	mSleepLinearVelocity	= desc.sleepLinearVelocity;
	mSleepAngularVelocity	= desc.sleepAngularVelocity;
	mCCDMotionThreshold		= desc.CCDMotionThreshold;
	mSolverIterationCount   = desc.solverIterationCount;
	mBodyFlags				= desc.flags;
	mSleepEnergyThreshold	= desc.sleepEnergyThreshold;
	mSleepDamping			= desc.sleepDamping;

}

//////////////////////////////////////////////////////////

Actor::Actor(const NxString& Identifier, Scene* scene, bool isActorBased) : mName(Identifier), mOwner(scene) {
	mActor = 0;
	if (isActorBased)
		mOwner->_registerActor(mName, this);
}

//////////////////////////////////////////////////////////

Actor::Actor(const NxString& name, Scene* scene, ShapeBlueprint *shape, const Pose& pose, ActorParams params)
: mName(name), mOwner(scene) {

	if (name.length() == 0) {
		mName = NxCreateID(scene->getNbActors(), "Actor");
	}
	else {

		std::vector<NxString> splitID = Ogre::StringUtil::split(name, ";", 2);

		NxString identifier;

		if (splitID.size() == 1) {
			identifier = name;
		}
		else {
			identifier = splitID[0];
		}

		Ogre::StringUtil::trim(identifier);

		if (identifier.substr(0,1) == ">") {
			mName = identifier.substr(1, identifier.length() - 1);
			if (scene->getActors()->has(mName)) {
				NxThrow_Error("Duplicate Actor with identifier '" + mName + "' found!");
			}

		}
		else if (scene->getActors()->has(identifier)) {
			// Generate a identifier based on the the suggested name.
			// i.e.
			//			myBody	-> myBody 1
			//			myBody10 -> myBody 11

			Actors actors;
			scene->getActors()->CopyTo(actors);

			NxU32 highest = 0;
			NxString workingName;
			for(Actor* a = actors.begin();a = actors.next();) {
				if (Ogre::StringUtil::startsWith(a->getName(), identifier, false)) {
					workingName = a->getName();
					workingName = workingName.substr(identifier.length(), workingName.length() - identifier.length());
					std::stringstream ss(workingName);
					NxU32 current = 0;
					ss >> current;
					if (current > highest)
						highest = current;
				}
			}

			highest++;
			mName = identifier;
			mName.append(Ogre::StringConverter::toString(highest));
		}
		else {
			mName = identifier;
		}
	}

	_createActor(shape,pose,params);
	mOwner->_registerActor(mName, this);
}

//////////////////////////////////////////////////////////

Actor::~Actor() {

	mOwner->_unregisterActor(mName);

	if (!isDead())
		_destroyActor();

}

//////////////////////////////////////////////////////////

void Actor::_createActor(ShapeBlueprint *shape, const Pose& pose, ActorParams params) {
	
	NxActorDesc ad;
	NxBodyDesc bd;
	ad.setToDefault();
	bd.setToDefault();

	mNxActorUserData = new NxActorUserData(this, NxActorUserData::T_Actor);
#if NX_SDK_VERSION_NUMBER >= 260
	ad.compartment = params.mCompartment;
#endif
	if (params.mMass == 0)
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
		if (ad.density == 0)
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

	if (!mActor) {
		std::stringstream ss;
		ss << "Creation of Actor with the identifier '" << mName << "' failed." << std::endl;
		NxThrow_Error(ss.str());
		return;
	}

	if (!shape->isShared())
		delete shape;

	NxU32 nbShapes = mActor->getNbShapes();
	NxShape*const* shapes = mActor->getShapes();

	int i=0;
	while (nbShapes--) {
		mCollisionModel[i]->_bindNxShapeToShape(shapes[nbShapes]);
		i++;
	}

#if (OGRE_VERSION_MAJOR >= 1) && (OGRE_VERSION_MINOR >= 5)
	mBirthFrame = Ogre::Root::getSingletonPtr()->getNextFrameNumber() - 1;
#else
	mBirthFrame = Ogre::Root::getSingletonPtr()->getCurrentFrameNumber();
#endif
}

//////////////////////////////////////////////////////////

void Actor::_destroyActor() {
	mCollisionModel.destroyAllOwned();
	delete mNxActorUserData;
	mOwner->mScene->releaseActor(*mActor);
	mActor = 0;
}

//////////////////////////////////////////////////////////

void Actor::setName(NxString name) {
	mOwner->mActors.rename(mName, name);
	mName = name;
}

//////////////////////////////////////////////////////////

NxString Actor::getName() const {
	return mName;
}

//////////////////////////////////////////////////////////

NxScene* Actor::getNxScene() {
	return mOwner->getNxScene();
}

//////////////////////////////////////////////////////////

Actor*	Actor::duplicate(const NxString& name, const Pose& p) {
	Blueprints::ActorBlueprint* ab = new Blueprints::ActorBlueprint();
	ab->serialise(this, mOwner);
	Actor* a = ab->unserialise(name, mOwner);
	a->setGlobalPose(p);
	delete ab;
	return a;
}

//////////////////////////////////////////////////////////

void Actor::setGlobalPose(const Pose& pose) {
	mActor->setGlobalPose(pose.toMat34());
}

//////////////////////////////////////////////////////////

Pose Actor::getGlobalPose() {
	static Pose p;
	p.fromMat34(mActor->getGlobalPose());
	return p;
}

//////////////////////////////////////////////////////////

Ogre::Quaternion Actor::getGlobalOrientation() {
	return NxConvert<Ogre::Quaternion, NxQuat>(mActor->getGlobalOrientationQuat());
}

//////////////////////////////////////////////////////////

void Actor::setGlobalOrientation(const Ogre::Quaternion& quat) {
	mActor->setGlobalOrientation(NxConvert<NxQuat, Ogre::Quaternion>(quat));
}

//////////////////////////////////////////////////////////

Ogre::Vector3 Actor::getGlobalPosition() {
	return NxConvert<Ogre::Vector3, NxVec3>(mActor->getGlobalPosition());
}

//////////////////////////////////////////////////////////

void Actor::setGlobalPosition(const Ogre::Vector3& vec) {
	mActor->setGlobalPosition(NxConvert<NxVec3, Ogre::Vector3>(vec));
}

//////////////////////////////////////////////////////////

void Actor::moveGlobalPose(const Pose& pose) {
	mActor->moveGlobalPose(pose.toMat34());
}

//////////////////////////////////////////////////////////

void Actor::moveGlobalPosition(const Ogre::Vector3& vec) {
	mActor->moveGlobalPosition(NxConvert<NxVec3, Ogre::Vector3>(vec));
}

//////////////////////////////////////////////////////////

void Actor::moveGlobalOrientation(const Ogre::Quaternion& quat) {
	mActor->moveGlobalOrientationQuat(NxConvert<NxQuat, Ogre::Quaternion>(quat));
}

////////////////////////////////////////////////////////

void Actor::moveTowards(const Pose& p, float force) {
	NxVec3 fc = p.v - mActor->getGlobalPosition();
	fc *= force;
	fc -= mActor->getPointVelocity(mActor->getGlobalPosition());
	mActor->addForceAtPos(fc, mActor->getGlobalPosition());
}

////////////////////////////////////////////////////////

#if 0

const ShapeBlueprint& Actor::createShapeDescription(const ShapeBlueprint& s) {
	NxUnderConstruction;
	return s;
}

//////////////////////////////////////////////////////////

void Actor::destroyShapeDescription(const ShapeBlueprint&) {
	NxUnderConstruction;
}

//////////////////////////////////////////////////////////

Ogre::uint Actor::getNbShapeDescriptions() {
	return mActor->getNbShapeDescriptions();
}

//////////////////////////////////////////////////////////

std::vector<const ShapeBlueprint&> Actor::getAllShapeDescriptions() {
	NxUnderConstruction;
	std::vector<const ShapeBlueprint&> s;
	return s;
}

//////////////////////////////////////////////////////////

ShapeBlueprint Actor::getFirstShapeDescription() {
	NxUnderConstruction;
	return NULL;
}

//////////////////////////////////////////////////////////

ShapeBlueprint Actor::getShapeDescription(Ogre::uint id) {
	NxUnderConstruction;
	return NULL;
}

#endif

//////////////////////////////////////////////////////////

void Actor::setGroup(ActorGroup* ag) {
	mActor->setGroup(ag->getGroupID());
}

//////////////////////////////////////////////////////////

void Actor::setGroup(const NxString& GroupIdentifier) {
	mActor->setGroup(mOwner->getActorGroups()->get(GroupIdentifier)->getGroupID());
}

//////////////////////////////////////////////////////////

void Actor::setGroup(NxActorGroup agid) {
	mActor->setGroup(agid);
}

//////////////////////////////////////////////////////////

ActorGroup* Actor::getGroup() const {
	NxActorGroup agid = mActor->getGroup();
	return mOwner->getActorGroupsByIndex()->get(agid);
}

//////////////////////////////////////////////////////////

void Actor::raiseActorFlag(NxActorFlag f) {
	mActor->raiseActorFlag(f);
}

//////////////////////////////////////////////////////////

void Actor::clearActorFlag(NxActorFlag f) {
	mActor->clearActorFlag(f);
}

//////////////////////////////////////////////////////////

bool Actor::hasActorFlag(NxActorFlag f)	const {
	return mActor->readActorFlag(f);
}
//////////////////////////////////////////////////////////

bool Actor::isDynamic()	const {
	return mActor->isDynamic();
}

//////////////////////////////////////////////////////////

void Actor::setCMassOffsetLocalPose(const Pose& pose) {
	mActor->setCMassOffsetLocalPose(pose.toMat34());
}

//////////////////////////////////////////////////////////

void Actor::setCMassOffsetLocalPosition(const Ogre::Vector3& vec) {
	mActor->setCMassOffsetLocalPosition(NxConvert<NxVec3, Ogre::Vector3>(vec));
}

//////////////////////////////////////////////////////////

void Actor::setCMassOffsetLocalOrientation(const NxMat33& m) {
	mActor->setCMassOffsetLocalOrientation(m);
}

//////////////////////////////////////////////////////////

void Actor::setCMassOffsetLocalOrientation(const Ogre::Matrix3& vec) {
	NxUnderConstruction;
}

//////////////////////////////////////////////////////////

void Actor::setCMassOffsetGlobalPose(const Pose& pose) {
	mActor->setCMassOffsetGlobalPose(pose.toMat34());
}

//////////////////////////////////////////////////////////

void Actor::setCMassOffsetGlobalPosition(const Ogre::Vector3& vec) {
	mActor->setCMassOffsetGlobalPosition(NxConvert<NxVec3, Ogre::Vector3>(vec));
}

//////////////////////////////////////////////////////////

void Actor::setCMassOffsetGlobalOrientation(const NxMat33& m) {
	mActor->setCMassOffsetGlobalOrientation(m);
}

//////////////////////////////////////////////////////////

void Actor::setCMassOffsetGlobalOrientation(const Ogre::Matrix3& quat) {
	NxUnderConstruction;
	NxMat33 m;
	mActor->setCMassOffsetGlobalOrientation(m);
}

//////////////////////////////////////////////////////////

void Actor::setCMassGlobalPose(const Pose& pose) {
	mActor->setCMassGlobalPose(pose.toMat34());
}

//////////////////////////////////////////////////////////

void Actor::setCMassGlobalPosition(const Ogre::Vector3& vec) {
	mActor->setCMassGlobalPosition(NxConvert<NxVec3, Ogre::Vector3>(vec));
}

//////////////////////////////////////////////////////////

void Actor::setCMassGlobalOrientation(const Ogre::Quaternion& quat) {
	NxUnderConstruction;
	NxMat33 m;
	mActor->setCMassGlobalOrientation(m);
}
//////////////////////////////////////////////////////////


Pose Actor::getCMassLocalPose()	const {
	Pose pose;
	pose.fromMat34(mActor->getCMassLocalPose());
	return pose;
}

//////////////////////////////////////////////////////////

Ogre::Vector3 Actor::getCMassLocalPosition()	const {
	return NxConvert<Ogre::Vector3, NxVec3>(mActor->getCMassLocalPosition());
}

//////////////////////////////////////////////////////////

Ogre::Quaternion Actor::getCMassLocalOrientation() const {
	return NxConvert<Ogre::Quaternion, NxQuat>(mActor->getCMassLocalOrientation());
}

//////////////////////////////////////////////////////////

Pose Actor::getCMassGlobalPose() const {
	Pose pose;
	pose.fromMat34(mActor->getCMassGlobalPose());
	return pose;
}

//////////////////////////////////////////////////////////

Ogre::Vector3 Actor::getCMassGlobalPosition() const {
	return NxConvert<Ogre::Vector3, NxVec3>(mActor->getCMassGlobalPosition());
}

//////////////////////////////////////////////////////////

Ogre::Quaternion Actor::getCMassGlobalOrientation()	const {
	NxQuat quat;
	mActor->getCMassGlobalOrientation().toQuat(quat);
	return NxConvert<Ogre::Quaternion, NxQuat>(quat);
}

//////////////////////////////////////////////////////////


void Actor::setMass(NxReal r) {
	mActor->setMass(r);
}

//////////////////////////////////////////////////////////

NxReal Actor::getMass() const {
	return mActor->getMass();
}

//////////////////////////////////////////////////////////


void Actor::setMassSpaceInertiaTensor(const Ogre::Vector3& vec) {
	mActor->setMassSpaceInertiaTensor(NxConvert<NxVec3, Ogre::Vector3>(vec));
}

//////////////////////////////////////////////////////////

Ogre::Vector3 Actor::getMassSpaceInertiaTensor()	const {
	return NxConvert<Ogre::Vector3, NxVec3>(mActor->getMassSpaceInertiaTensor());
}

//////////////////////////////////////////////////////////

Ogre::Matrix3 Actor::getGlobalInertiaTensor() const {
	NxUnderConstruction;
	Ogre::Matrix3 m;
	return m;
}

//////////////////////////////////////////////////////////

Ogre::Matrix3 Actor::getGlobalInertiaTensorInverse()	const {
	NxUnderConstruction;
	Ogre::Matrix3 m;
	return m;
}

//////////////////////////////////////////////////////////


void Actor::updateMassFromShapeFromDensity(NxReal r) {
	mActor->updateMassFromShapes(r,0);
}

//////////////////////////////////////////////////////////

void Actor::updateMassFromShapeAsMass(NxReal r) {
	mActor->updateMassFromShapes(0,r);
}

//////////////////////////////////////////////////////////

void Actor::setLinearDamping(NxReal r) {
	mActor->setLinearDamping(r);
}

//////////////////////////////////////////////////////////

NxReal Actor::getLinearDamping() const {
	return mActor->getLinearDamping();
}

//////////////////////////////////////////////////////////


void Actor::setAngularDamping(NxReal r) {
	mActor->setAngularDamping(r);
}

//////////////////////////////////////////////////////////

NxReal Actor::getAngularDamping() const {
	return mActor->getAngularDamping();
}

//////////////////////////////////////////////////////////

void Actor::setLinearVelocity(const Ogre::Vector3& vec) {
	mActor->setLinearVelocity(NxConvert<NxVec3, Ogre::Vector3>(vec));
}

//////////////////////////////////////////////////////////

void Actor::setAngularVelocity(const Ogre::Vector3& vec) {
	mActor->setAngularVelocity(NxConvert<NxVec3, Ogre::Vector3>(vec));
}

//////////////////////////////////////////////////////////


Ogre::Vector3 Actor::getLinearVelocity()	const {
	return NxConvert<Ogre::Vector3, NxVec3>(mActor->getLinearVelocity());
}

//////////////////////////////////////////////////////////

Ogre::Vector3 Actor::getAngularVelocity() const {
	return NxConvert<Ogre::Vector3, NxVec3>(mActor->getAngularVelocity());
}

//////////////////////////////////////////////////////////


void Actor::setMaxAngularVelocity(NxReal r) {
	mActor->setMaxAngularVelocity(r);
}

//////////////////////////////////////////////////////////

NxReal Actor::getMaxAngularVelocity()	const {
	return mActor->getMaxAngularVelocity();
}

//////////////////////////////////////////////////////////

void Actor::setCCDMotionThreshold(NxReal r) {
	mActor->setCCDMotionThreshold(r);
}

//////////////////////////////////////////////////////////

NxReal Actor::getCCDMotionThreshold()	const {
	return mActor->getCCDMotionThreshold();
}

//////////////////////////////////////////////////////////

void Actor::setLinearMomentum(const Ogre::Vector3& vec) {
	mActor->setLinearMomentum(NxConvert<NxVec3, Ogre::Vector3>(vec));
}

//////////////////////////////////////////////////////////

void Actor::setAngularMomentum(const Ogre::Vector3& vec) {
	mActor->setAngularMomentum(NxConvert<NxVec3, Ogre::Vector3>(vec));
}

//////////////////////////////////////////////////////////

Ogre::Vector3 Actor::getLinearMomentum()		const {
	return NxConvert<Ogre::Vector3, NxVec3>(mActor->getLinearMomentum());
}

//////////////////////////////////////////////////////////

Ogre::Vector3 Actor::getAngularMomentum()	const {
	return NxConvert<Ogre::Vector3, NxVec3>(mActor->getAngularMomentum());
}

//////////////////////////////////////////////////////////

void Actor::addForceAtPos(const Ogre::Vector3& force, const Ogre::Vector3& pos, NxForceMode mode, bool wakeup) {
	mActor->addForceAtPos(NxConvert<NxVec3, Ogre::Vector3>(force), NxConvert<NxVec3, Ogre::Vector3>(pos), mode, wakeup);
}

//////////////////////////////////////////////////////////

void Actor::addForceAtLocalPos(const Ogre::Vector3& force, const Ogre::Vector3& pos, NxForceMode mode, bool wakeup) {
	mActor->addForceAtLocalPos(NxConvert<NxVec3, Ogre::Vector3>(force), NxConvert<NxVec3, Ogre::Vector3>(pos), mode, wakeup);
}

//////////////////////////////////////////////////////////

void Actor::addLocalForceAtPos(const Ogre::Vector3& force, const Ogre::Vector3& pos, NxForceMode mode, bool wakeup) {
	mActor->addLocalForceAtPos(NxConvert<NxVec3, Ogre::Vector3>(force), NxConvert<NxVec3, Ogre::Vector3>(pos), mode, wakeup);
}

//////////////////////////////////////////////////////////

void Actor::addLocalForceAtLocalPos(const Ogre::Vector3& force, const Ogre::Vector3& pos, NxForceMode mode, bool wakeup) {
	mActor->addLocalForceAtLocalPos(NxConvert<NxVec3, Ogre::Vector3>(force), NxConvert<NxVec3, Ogre::Vector3>(pos), mode, wakeup);
}

//////////////////////////////////////////////////////////

void Actor::addForce(const Ogre::Vector3& force, NxForceMode mode, bool wakeup) {
	mActor->addForce(NxConvert<NxVec3, Ogre::Vector3>(force), mode, wakeup);
}

//////////////////////////////////////////////////////////

void Actor::addForce(const NxVec3& force, NxForceMode mode, bool wakeup) {
	mActor->addForce(force, mode, wakeup);
}

//////////////////////////////////////////////////////////

void Actor::addForce(NxReal x, NxReal y, NxReal z, NxForceMode mode, bool wakeup) {
	mActor->addForce(NxVec3(x,y,z), mode, wakeup);
}

//////////////////////////////////////////////////////////

void Actor::addLocalForce(const Ogre::Vector3& force, NxForceMode mode, bool wakeup) {
	mActor->addLocalForce(NxConvert<NxVec3, Ogre::Vector3>(force), mode, wakeup);
}

//////////////////////////////////////////////////////////

void Actor::addTorque(const NxVec3& torque, NxForceMode mode, bool wakeup) {
	mActor->addTorque(torque, mode, wakeup);
}

//////////////////////////////////////////////////////////

void Actor::addTorque(const Ogre::Vector3& torque, NxForceMode mode, bool wakeup) {
	mActor->addTorque(NxConvert<NxVec3, Ogre::Vector3>(torque), mode, wakeup);
}

//////////////////////////////////////////////////////////

void Actor::addLocalTorque(const Ogre::Vector3& torque, NxForceMode mode, bool wakeup) {
	mActor->addLocalTorque(NxConvert<NxVec3, Ogre::Vector3>(torque), mode, wakeup);
}

//////////////////////////////////////////////////////////

NxReal Actor::computeKineticEnergy() const {
	return mActor->computeKineticEnergy();
}

//////////////////////////////////////////////////////////

Ogre::Vector3 Actor::getPointVelocity(const Ogre::Vector3& vec) const {
	return NxConvert<Ogre::Vector3, NxVec3>(mActor->getPointVelocity(NxConvert<NxVec3, Ogre::Vector3>(vec)));
}

//////////////////////////////////////////////////////////

Ogre::Vector3 Actor::getLocalPointVelocity(const Ogre::Vector3& vec) const {
	return NxConvert<Ogre::Vector3, NxVec3>(mActor->getLocalPointVelocity(NxConvert<NxVec3, Ogre::Vector3>(vec)));
}

//////////////////////////////////////////////////////////

bool Actor::isGroupSleeping() const {
	return mActor->isGroupSleeping();
}

//////////////////////////////////////////////////////////

bool Actor::isSleeping() const {
	return mActor->isSleeping();
}

//////////////////////////////////////////////////////////

NxReal Actor::getSleepLinearVelocity() const {
	return mActor->getSleepLinearVelocity();
}

//////////////////////////////////////////////////////////

void Actor::setSleepLinearVelocity(NxReal threshold) {
	mActor->setSleepLinearVelocity(threshold);
}

//////////////////////////////////////////////////////////

NxReal Actor::getSleepAngularVelocity() const {
	return mActor->getSleepAngularVelocity();
}

//////////////////////////////////////////////////////////

void Actor::setSleepAngularVelocity(NxReal threshold) {
	mActor->setSleepAngularVelocity(threshold);
}

//////////////////////////////////////////////////////////

NxReal Actor::getSleepEnergyThreshold() const {
	return mActor->getSleepEnergyThreshold();
}

//////////////////////////////////////////////////////////

void Actor::setSleepEnergyThreshold(NxReal threshold) {
	mActor->setSleepEnergyThreshold(threshold);
}

//////////////////////////////////////////////////////////

void Actor::wakeUp(NxReal wakeCounterValue)	{
	mActor->wakeUp(wakeCounterValue);
}

//////////////////////////////////////////////////////////

void Actor::putToSleep() {
	mActor->putToSleep();
}

//////////////////////////////////////////////////////////

void Actor::raiseBodyFlag(NxBodyFlag f) {
	mActor->raiseBodyFlag(f);
}

//////////////////////////////////////////////////////////

void Actor::clearBodyFlag(NxBodyFlag f) {
	mActor->clearBodyFlag(f);
}

//////////////////////////////////////////////////////////

bool Actor::hasBodyFlag(NxBodyFlag f) const {
	return mActor->readBodyFlag(f);
}

//////////////////////////////////////////////////////////

void Actor::setSolverIterationCount(NxU32 c) {
	mActor->setSolverIterationCount(c);
}

//////////////////////////////////////////////////////////

NxU32 Actor::getSolverIterationCount() const {
	return mActor->getSolverIterationCount();
}

//////////////////////////////////////////////////////////

#if NX_SUPPORT_SWEEP_API

NxU32 Actor::linearSweep(const Ogre::Vector3& motion, NxU32 flags, void* userData, NxU32 nbShapeDescriptions, NxSweepQueryHit* shapes, NxUserEntityReport<NxSweepQueryHit>* callback, const NxSweepCache* sweepCache) {
	return mActor->linearSweep(NxConvert<NxVec3, Ogre::Vector3>(motion), flags, userData, nbShapeDescriptions, shapes, callback, sweepCache);
}

#endif

//////////////////////////////////////////////////////////

#if NX_SDK_VERSION_NUMBER >= 260

Compartment* Actor::getCompartment() const {
	NxUnderConstruction;
	return NULL;
}

#endif

//////////////////////////////////////////////////////////

Shape* Actor::addShape(ShapeBlueprint* sd) {


	NxShapeIndex id = mCollisionModel.count();
	sd->_bindToNxActor(this, id);
	delete sd;
	Shape* s = mCollisionModel.get(id);
	

	return s;

}

//////////////////////////////////////////////////////////

void Actor::removeShape(Shape* sh) {
	

	for (Shape* shape = mCollisionModel.begin();shape = mCollisionModel.next();) {
		if (shape == sh) {
			mActor->releaseShape(*sh->getNxShape());
			mCollisionModel.remove(sh->getIndex());		
			
			if (mDynamicCollisionModel.has(sh->getIndex()))
				mDynamicCollisionModel.remove(sh->getIndex());

			delete shape;
			
			return;
		}
	}

}

//////////////////////////////////////////////////////////

void Actor::removeShape(NxShapeIndex id) {
	
	Shape* shape = mCollisionModel[id];

	if (shape == 0)
		return;

	shape->releaseShape();
	mCollisionModel.remove(id);
	
	delete shape;
}

//////////////////////////////////////////////////////////

NxU32 Actor::getNbShapes() const {
	return mCollisionModel.count();
}

//////////////////////////////////////////////////////////

CollisionModel* Actor::getCollisionModel() {
	return &mCollisionModel;
}

//////////////////////////////////////////////////////////

CollisionModel* Actor::getDynamicCollisionModel() {
	return &mDynamicCollisionModel;
}

//////////////////////////////////////////////////////////

void* Actor::getNxActorUserData() {
	return mActor->userData;
}

//////////////////////////////////////////////////////////

NxActorUserData* Actor::getUserData() {
	return mNxActorUserData;
}

//////////////////////////////////////////////////////////

StringPairList Actor::saveCustom() {
	StringPairList l;
	l.insert("ActorType", "Actor");
	
	return l;	
}

//////////////////////////////////////////////////////////

void Actor::disable() {

	mActor->raiseActorFlag(NX_AF_DISABLE_COLLISION);
	mActor->raiseActorFlag(NX_AF_DISABLE_RESPONSE);
	mActor->raiseActorFlag(NX_AF_FLUID_DISABLE_COLLISION);
	mActor->raiseBodyFlag(NX_BF_FROZEN);
	mActor->raiseBodyFlag(NX_BF_FROZEN_ROT);
	mActor->setAngularVelocity(NxVec3(0,0,0));
	mActor->setAngularMomentum(NxVec3(0,0,0));
	mActor->setLinearMomentum(NxVec3(0,0,0));
	mActor->setLinearVelocity(NxVec3(0,0,0));

//	disableVisualisation();
}

//////////////////////////////////////////////////////////

FixedJoint* Actor::joinWith(Actor* actorB, JointParams jp) {
	return mOwner->createFixedJoint(this, actorB, jp);
}

//////////////////////////////////////////////////////////

}  //End of NxOgre namespace.
