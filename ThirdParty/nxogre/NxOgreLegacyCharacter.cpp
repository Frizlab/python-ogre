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


#if (NX_USE_LEGACY_NXCONTROLLER == 1 && NX_USE_CHARACTER_API == 1)

#include "NxOgreLegacyCharacter.h"
#include "NxOgrePose.h"						// For: Character position and orientation
#include "NxOgreLegacyCharacterController.h"		// For: HitReport and Character creation
#include "NxOgreScene.h"					// For: getNxScene in Scene
#include "NxOgreHelpers.h"					// For: NxVec3 to Vector3 conversion
#include "NxOgreUserData.h"					// For: Actor's UserData

#include "NxCharacter.h"
#include "NxBoxController.h"
#include "NxCapsuleController.h"

#include "OgreSceneNode.h"
#include "OgreMovableObject.h"
#include "OgreSceneMAnager.h"
#include "OgreEntity.h"

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

Character::Character(const NxString &name, Scene* scene, CharacterController* cc, Pose p , CharacterParams cp) 
: mName(name), mScene(scene), mCharacterController(cc), mNode(0), mEntity(0) {
	
	mMoveVectorController = mCharacterController;
	mCharacterController->_registerCharacter(mName, this);
	mType = cp.mType;
	mDimensions = cp.mDimensions;
	mUpAxis = cp.mUpAxis;
	mSlopeLimit = cp.mSlopeLimit;
	mStepOffset = cp.mStepOffset;
	mSkinWidth = cp.mSkinWidth;
	mMove_ActiveGroupsMask = cp.mActiveGroupsMask;
	mMove_Sharpness = cp.mSharpness;
	mMove_GroupMask = cp.mGroupMask;
	mNextMovementDirection.zero();
	mDirection.id();
	mMass = 100;			/// TEMP.
	mCharacter = 0;

	if (mType == CharacterParams::CT_Box) {

		NxBoxControllerDesc desc;
		desc.setToDefault();
		desc.position.set(p.v.x,p.v.y,p.v.z); // 0,0,0
		desc.extents.set(mDimensions.x * 0.5f, mDimensions.y * 0.5f, mDimensions.z * 0.5f); // 0.5, 1, 0.5
		desc.upDirection = mUpAxis;		// NX_Y
		desc.slopeLimit = mSlopeLimit; // 0.707
		desc.stepOffset = mStepOffset; // 0.5
		desc.skinWidth = mSkinWidth; // 0.1
		desc.callback = mCharacterController;
		desc.userData = this;
	
		mCharacter = mCharacterController->mControllerManager->createController(mScene->getNxScene(), desc);

	}
	else {

		NxCapsuleControllerDesc desc;
		desc.setToDefault();
		desc.radius = mDimensions.x;
		desc.height = mDimensions.y;
		desc.position.set(p.v.x,p.v.y,p.v.z); // 0,0,0
		desc.upDirection = mUpAxis;		// NX_Y
		desc.slopeLimit = mSlopeLimit; // 0.707
		desc.stepOffset = mStepOffset; // 0.5
		desc.skinWidth = mSkinWidth; // 0.1
		desc.callback = mCharacterController;
		desc.userData = this;
		

		mCharacter = mCharacterController->mControllerManager->createController(mScene->getNxScene(), desc);

	}
	
	if (mCharacter == 0) {
		NxThrow_Error("Character '" + mName + "' could not be created.");
		return;
	}

	mNxActorUserData = new NxActorUserData(this, NxActorUserData::T_Character);
	mCharacter->getActor()->setGroup(0);	
	mCharacter->getActor()->userData = mNxActorUserData;
	mCharacter->setCollision(true);

}

////////////////////////////////////////////////////////////////////////////////////////////////

Character::~Character() {

	mCharacterController->_unregisterCharacter(mName);
	mCharacterController->mControllerManager->releaseController(*mCharacter);

	if (mNode == 0)
		return;
	
	
	Ogre::SceneNode::ObjectIterator object_it = mNode->getAttachedObjectIterator();
	Ogre::MovableObject *m;
	while(object_it.hasMoreElements()) {
		m = object_it.getNext();
		if (m->getMovableType() == "Entity") {
			mScene->getSceneManager()->destroyEntity((Ogre::Entity*) m);
		}
		else {
			mNode->detachObject(m);
			mScene->getSceneManager()->getRootSceneNode()->attachObject(m);
		}
	}

	mNode->detachAllObjects();
	mNode->removeAndDestroyAllChildren();

	if (mNode)
		mScene->getSceneManager()->destroySceneNode(mNode->getName());

	delete mNxActorUserData;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::simulate(float t) {

	NxVec3 moveVector, gravity;
	moveVector.zero();
	mScene->getNxScene()->getGravity(gravity);

	// Calculate next movement.
	mMoveVectorController->move(moveVector, mNextMovementDirection, mDirection, gravity, t, this);

	// Move it
	mCharacter->move(moveVector, mMove_ActiveGroupsMask, mMove_MinDistance, mCollisionFlags, mMove_Sharpness, &mMove_GroupMask);

	// ...And reset.
	mNextMovementDirection.zero();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::render(float) {
	if (mNode) {
		mNode->setPosition(getGlobalPosition());
		mNode->setOrientation(getGlobalOrientation());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::createNode() {
	if (mNode)
		return;
	mNode = mScene->getSceneManager()->getRootSceneNode()->createChildSceneNode(mName, NxConvert<Ogre::Vector3, NxVec3>(NxVec3(mCharacter->getPosition().x,mCharacter->getPosition().y,mCharacter->getPosition().z)));
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::attachMesh(const NxString& meshName) {

	createNode();

	if (mEntity == 0) {
		mEntity = mScene->getSceneManager()->createEntity(mName, meshName);
		mNode->attachObject(mEntity);
	}
	else {
		Ogre::Entity* entity = mScene->getSceneManager()->createEntity(mName, meshName);
		mNode->attachObject(entity);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::attachSkeletonMesh(const NxString&) {
	NxUnderConstruction;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::bindToAnimation(Movement, const NxString& AnimationName) {
	NxUnderConstruction;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::bindFeet(const NxString& leftBone, const NxString& rightBone) {
	NxUnderConstruction;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::unBindFeet() {
	NxUnderConstruction;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Ogre::Vector3 Character::getGlobalPosition() {
	return NxConvert<Ogre::Vector3, NxVec3>(mCharacter->getPosition().x, mCharacter->getPosition().y,mCharacter->getPosition().z);
}

////////////////////////////////////////////////////////////////////////////////////////////////

Ogre::Quaternion Character::getGlobalOrientation() {
	return NxConvert<Ogre::Quaternion, NxQuat>(mDirection);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::setRawNextMovementDirection(const NxVec3& direction) {
	mNextMovementDirection = direction;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::setDirection(const Ogre::Radian &r) {
	mDirection.fromAngleAxis(r.valueAngleUnits(),NxVec3(0,1,0));
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::setDirection(const Ogre::Quaternion &q) {
	mDirection = NxConvert<NxQuat, Ogre::Quaternion>(q);
	mDirection.x = 0;
	mDirection.z = 0;

	mDirection.normalize();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::addMovement(Direction m) {
	
	if (mUpAxis == NX_Y) {

		switch (m) {
			case DR_Forward:
				mNextMovementDirection -= NxVec3Axis[NX_X];
				return;
			break;

			case DR_Backward:
				mNextMovementDirection += NxVec3Axis[NX_X];
				return;
			break;

			case DR_StepLeft:
				mNextMovementDirection -= NxVec3Axis[NX_Z];
				return;
			break;

			case DR_StepRight:
				mNextMovementDirection += NxVec3Axis[NX_Z];
				return;
			break;

			case DR_Jump_Up:
				// jump inject code
				mNextMovementDirection.zero();
				// jump timer++
				// pass jump timer onto move bit as well.
			break;

			case DR_Ladder_Up:
				mNextMovementDirection += NxVec3Axis[NX_Y];
				return;
			break;

			case DR_Ladder_Down:
				mNextMovementDirection -= NxVec3Axis[NX_Y];
				return;
			break;

		}

		return;
	}
	else if (mUpAxis == NX_X) {
		// TODO
	}
	else if (mUpAxis == NX_Z) {
		// TODO
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::setMovementVectorController(CharacterMovementVectorController* cmvc) {
	mMoveVectorController = cmvc;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Character::setPosition(const Ogre::Vector3& v) {
	mCharacter->setPosition(toNxExtendedVec3(v));	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CharacterMovementVectorController::move(NxVec3 &out, NxVec3 &moveVector, NxQuat &direction, NxVec3 &g, float t, Character*) {
	// TODO: Non NX_Y directions.
	out = ((direction.rot(moveVector) * 1.5) + g) * t;
}

////////////////////////////////////////////////////////////////////////////////////////////////

}  //End of NxOgre namespace.

#endif