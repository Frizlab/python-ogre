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
#include "NxOgreRaycaster.h"
#include "NxOgreUserData.h"
#include "NxOgreHelpers.h"		// For conversions
#include "NxOgreScene.h"		// So I can raycast
#include "NxOgreContainer.h"	// For container access.
#include "NxOgreActor.h"		// For getName()
#if (NX_USE_CHARACTER_API == 1)
#	include "NxOgreCharacter.h"
#endif
namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////

RayCaster::RayCaster(Ogre::Vector3 origin, Ogre::Vector3 direction, NxReal maxDistance, Scene* s)
:	mRayDistance(maxDistance),
	mScene(s),
	mType(RayCaster::RCT_ANY)
{
	mRay = NxRay(toNxVec3(origin), toNxVec3(direction));
	
	mHitNormal.zero();
	mHitImpact.zero();

}

////////////////////////////////////////////////////////////////////////////////

RayCaster::RayCaster(Ogre::Vector3 origin, Ogre::Vector3 direction, NxReal maxDistance, RayCastType t, Scene* s)
:	mRayDistance(maxDistance),
	mScene(s),
	mType(t)
{
	mRay = NxRay(toNxVec3(origin), toNxVec3(direction));
	
	mHitNormal.zero();
	mHitImpact.zero();
	
}

////////////////////////////////////////////////////////////////////////////////

RayCaster::~RayCaster() {

}

////////////////////////////////////////////////////////////////////////////////

bool RayCaster::castShape(ActorFilter f) {

	mReport.empty();

	if (mType == RCT_ANY)
		return _castAny(f);
	else if (mType == RCT_CLOSEST)
		return _castClosest(f);
	else
		return !!_castAll(f);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

bool RayCaster::_castAny(ActorFilter f) {
	return mScene->getNxScene()->raycastAnyShape(mRay, (NxShapesType) f, -1, mRayDistance);
}

////////////////////////////////////////////////////////////////////////////////

bool RayCaster::_castClosest(ActorFilter f) {

	NxRaycastHit hit;
	NxShape* hitShape = mScene->getNxScene()->raycastClosestShape(mRay, (NxShapesType) f, hit,-1, mRayDistance);

#if (NX_USE_CHARACTER_API == 0)
	if (hitShape) {

		mHit = RayCastHit();
		mHit.mRaycastHit = hit;
		
		if (hitShape->getActor().userData)
			mHit.mActor = (static_cast<NxActorUserData*>(hitShape->getActor().userData))->toActor();
		else
			return false;

		mHit.mWorldImpact = toVector3(hit.worldImpact);
		mHit.mWorldNormal = toVector3(hit.worldNormal);
		
		mReport.insert(mHit.mActor->getName(), mHit);
		
		return true;
	}
	return false;
#else

	if (hitShape) {

		if (hitShape->getActor().userData == 0)
			return false; 

		mHit = RayCastHit();
		mHit.mRaycastHit = hit;

		switch( static_cast< NxActorUserData* >( hitShape->getActor().userData )->getType()) {
		
			case NxActorUserData::T_Actor:
				mHit.mActor = static_cast<NxActorUserData*>(hitShape->getActor().userData)->toActor();
				mHit.mCharacter = 0;
				mHit.mWorldImpact = toVector3(mHit.mRaycastHit.worldImpact);
				mHit.mWorldNormal = toVector3(mHit.mRaycastHit.worldNormal);

				mReport.insert(mHit.mActor->getName(), mHit);
			break;
			
			case NxActorUserData::T_Character:
				mHit.mActor = 0;
				mHit.mCharacter = static_cast< NxActorUserData* >(hitShape->getActor().userData)->toCharacter();
				mHit.mWorldImpact = toVector3(mHit.mRaycastHit.worldImpact);
				mHit.mWorldNormal = toVector3(mHit.mRaycastHit.worldNormal);
				mReport.insert(mHit.mCharacter->getName(), mHit);
			break;

			default:
				return false;
			break;
		}

 

		return true;
	}

	return false;
#endif
}

////////////////////////////////////////////////////////////////////////////////

NxU32 RayCaster::_castAll(ActorFilter f) {
	return mScene->getNxScene()->raycastAllShapes(mRay, *this, (NxShapesType) f,-1,mRayDistance);
}

////////////////////////////////////////////////////////////////////////////////

Actor* RayCaster::getClosestActor() {
	if (mReport.count() == 0)
		return 0;

	RayCastReport::Iterator mIterator = mReport.items.begin();
	return (*mIterator).second.t.mActor;

}

////////////////////////////////////////////////////////////////////////////////

RayCastHit RayCaster::getClosestRaycastHit() {
	if (mReport.count() == 0) {
		return RayCastHit();
	}

	RayCastReport::Iterator mIterator = mReport.items.begin();
	return (*mIterator).second.t;
}

////////////////////////////////////////////////////////////////////////////////

bool RayCaster::onHit(const NxRaycastHit& hit)  {
	
	std::cout << hit.distance << std::endl;

	RayCastHit raycastHit;
	
#if (NX_USE_CHARACTER_API == 0)
	
	if (hit.shape) {

		raycastHit = RayCastHit();
		raycastHit.mRaycastHit = hit;
		
		if (hit.shape->getActor().userData)
			raycastHit.mActor = (static_cast<NxActorUserData*>(hit.shape->getActor().userData))->toActor();
		else
			return true;

		raycastHit.mWorldImpact = toVector3(hit.worldImpact);
		raycastHit.mWorldNormal = toVector3(hit.worldNormal);
		
		mReport.insert(raycastHit.mActor->getName(), raycastHit);
		
	}

#else

	if (hit.shape) {

		if (hit.shape->getActor().userData == 0)
			return true; 

		raycastHit = RayCastHit();
		raycastHit.mRaycastHit = hit;

		switch( static_cast< NxActorUserData* >( hit.shape->getActor().userData )->getType()) {
		
			case NxActorUserData::T_Actor:
				raycastHit.mActor = static_cast<NxActorUserData*>(hit.shape->getActor().userData)->toActor();
				raycastHit.mCharacter = 0;
				raycastHit.mWorldImpact = toVector3(raycastHit.mRaycastHit.worldImpact);
				raycastHit.mWorldNormal = toVector3(raycastHit.mRaycastHit.worldNormal);

				mReport.insert(raycastHit.mActor->getName(), raycastHit);
			break;
			
			case NxActorUserData::T_Character:
				raycastHit.mActor = 0;
				raycastHit.mCharacter = static_cast< NxActorUserData* >(hit.shape->getActor().userData)->toCharacter();
				raycastHit.mWorldImpact = toVector3(raycastHit.mRaycastHit.worldImpact);
				raycastHit.mWorldNormal = toVector3(raycastHit.mRaycastHit.worldNormal);
				mReport.insert(raycastHit.mCharacter->getName(), raycastHit);
			break;

		}

 
	}
#endif

	return true;
}

////////////////////////////////////////////////////////////////////////////////

void RayCaster::setOrigin(const Ogre::Vector3& o) {
	mRay.orig = toNxVec3(o);
}

////////////////////////////////////////////////////////////////////////////////

void RayCaster::setDirection(const Ogre::Vector3& d) {
	mRay.dir = toNxVec3(d);
}

////////////////////////////////////////////////////////////////////////////////

void RayCaster::setMaxDistance(NxReal d) {
	mRayDistance = d;
}

////////////////////////////////////////////////////////////////////////////////

#if (NX_USE_CHARACTER_API == 1)

Character * RayCaster::getClosestCharacter() {
	
	if (mReport.count() == 0)
		return 0;
	
	RayCastReport::Iterator mIterator = mReport.items.begin();

	return (*mIterator).second.t.mCharacter;
}

////////////////////////////////////////////////////////////////////////////////

#endif

}; //End of NxOgre namespace.
