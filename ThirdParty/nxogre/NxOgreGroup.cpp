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
#include "NxOgreGroup.h"
#include "NxOgreScene.h"			// For: getActorGroups(), getActorGroupsByIndex(), ... in NxScene
#include "NxOgreContainer.h"		// For: Getting ID's for containers and such.

namespace NxOgre {

//////////////////////////////////////////////////////////////////////

#define NxMaxActorGroups 0x7fff			// 32767
#define NxMaxShapeGroups 0x1F			// 31

NxActorGroup ActorGroup::mNextFreeID = 0;
NxCollisionGroup ShapeGroup::mNextFreeID = 0;

//////////////////////////////////////////////////////////////////////

//
// ActorGroup
//
//////////////////////////////////////////////////////////////////////

ActorGroup::ActorGroup(const NxString& identifier, Scene* scene) : mScene(scene), mName(identifier), mCallback(0) {

	mGroupID = mNextFreeID;

	if (scene->getActorGroupsByIndex()->has(mNextFreeID)) {
		ActorGroupsByIndex* ag = scene->getActorGroupsByIndex();
		for (NxU32 i=0; i < NxMaxActorGroups;i++) {
			if (ag->has(i))
				continue;
			mGroupID = i;
			break;
		}
	}
	mNextFreeID = mGroupID + 1;
	scene->_registerActorGroup(mName, this);
#ifdef NX_DEBUG
	std::stringstream ss;
	ss << "Created ActorGroup '" << identifier << "' with GroupID as " << mGroupID << std::endl;
	NxDebug(ss.str());
#endif
}

//////////////////////////////////////////////////////////////////////

ActorGroup::ActorGroup(NxActorGroup GroupID, const NxString& identifier, Scene* scene) : mScene(scene), mName(identifier), mGroupID(GroupID), mCallback(0) {
	
	scene->_registerActorGroup(mName, this);
	
	// Default ID.
	if (mGroupID == 0)
		mNextFreeID = 1;
#ifdef NX_DEBUG
	std::stringstream ss;
	ss << "Created ActorGroup '" << identifier << "' with GroupID as " << mGroupID << std::endl;
	NxDebug(ss.str());
#endif
}

//////////////////////////////////////////////////////////////////////

ActorGroup::~ActorGroup() {
	mScene->_unregisterActorGroup(mName);
	mNextFreeID = mGroupID;

	if (mCallbackOwned)
		delete mCallback;
#ifdef NX_DEBUG
	std::stringstream ss;
	ss << "Destroyed ActorGroup '" << mName << "'." << std::endl;
	NxDebug(ss.str());
#endif
}

//////////////////////////////////////////////////////////////////////

void ActorGroup::setCollisionCallback(ActorGroup* b, NxContactPairFlag flag, bool ForBGroup) {

	mScene->getNxScene()->setActorGroupPairFlags(mGroupID, b->getGroupID(), flag);
	if (ForBGroup)
		mScene->getNxScene()->setActorGroupPairFlags(b->getGroupID(), mGroupID, flag);	
}

//////////////////////////////////////////////////////////////////////

void ActorGroup::onStartTouch(Actor* a, Actor* b) {
	if (mCallback)
		mCallback->onStartTouch(a,b);
}

//////////////////////////////////////////////////////////////////////

void ActorGroup::onEndTouch(Actor* a, Actor* b) {
	if (mCallback)
		mCallback->onEndTouch(a,b);
}

//////////////////////////////////////////////////////////////////////

void ActorGroup::onTouch(Actor* a, Actor* b) {
	if (mCallback)
		mCallback->onTouch(a,b);
}

//////////////////////////////////////////////////////////////////////

// ShapeGroup
//
//
//////////////////////////////////////////////////////////////////////

ShapeGroup::ShapeGroup(const NxString& identifier, Scene* scene) : mScene(scene), mName(identifier) {

	mGroupID = mNextFreeID;

	if (scene->getShapeGroupsByIndex()->has(mNextFreeID)) {
		ShapeGroupsByIndex* ag = scene->getShapeGroupsByIndex();
		for (NxU32 i=0; i < NxMaxShapeGroups;i++) {
			if (ag->has(i))
				continue;
			mGroupID = i;
			break;
		}
	}
	mNextFreeID = mGroupID + 1;
	scene->_registerShapeGroup(mName, this);
#ifdef NX_DEBUG
	std::stringstream ss;
	ss << "Created ShapeGroup '" << identifier << "' with GroupID as " << mGroupID << std::endl;
	NxDebug(ss.str());
#endif
}

//////////////////////////////////////////////////////////////////////

ShapeGroup::ShapeGroup(NxCollisionGroup GroupID, const NxString& identifier, Scene* scene) : mScene(scene), mName(identifier), mGroupID(GroupID) {
	scene->_registerShapeGroup(mName, this);
	
	// Default ID.
	if (mGroupID == 0)
		mNextFreeID = 1;
#ifdef NX_DEBUG
	std::stringstream ss;
	ss << "Created ShapeGroup '" << identifier << "' with GroupID as " << mGroupID << std::endl;
	NxDebug(ss.str());
#endif
}

//////////////////////////////////////////////////////////////////////

ShapeGroup::~ShapeGroup() {
	mScene->_unregisterShapeGroup(mName);
	mNextFreeID = mGroupID;

#ifdef NX_DEBUG
	std::stringstream ss;
	ss << "Destroyed ShapeGroup '" << mName << "'." << std::endl;
	NxDebug(ss.str());
#endif
}

//////////////////////////////////////////////////////////////////////

void ShapeGroup::setCollisionResponse(ShapeGroup* B, CollisionResponse cr) {
	if (cr == ShapeGroup::CR_No_Collision)
		mScene->getNxScene()->setGroupCollisionFlag(mGroupID, B->getGroupID(), false);
	else
		mScene->getNxScene()->setGroupCollisionFlag(mGroupID, B->getGroupID(), true);
}

//////////////////////////////////////////////////////////////////////

#undef NxMaxActorGroups
#undef NxMaxShapeGroups

//////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
