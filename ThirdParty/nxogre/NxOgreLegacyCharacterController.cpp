/** \file    NxOgreLegacyCharacterController.cpp
 *  \see     NxOgreLegacyCharacterController.h
 *  \version 1.0-20
 *
 *  \licence NxOgre a wrapper for the PhysX physics library.
 *           Copyright (C) 2005-8 Robin Southern of NxOgre.org http://www.nxogre.org
 *           This library is free software; you can redistribute it and/or
 *           modify it under the terms of the GNU Lesser General Public
 *           License as published by the Free Software Foundation; either
 *           version 2.1 of the License, or (at your option) any later version.
 *           
 *           This library is distributed in the hope that it will be useful,
 *           but WITHOUT ANY WARRANTY; without even the implied warranty of
 *           MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *           Lesser General Public License for more details.
 *           
 *           You should have received a copy of the GNU Lesser General Public
 *           License along with this library; if not, write to the Free Software
 *           Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "NxOgreStable.h"


#if (NX_USE_LEGACY_NXCONTROLLER == 1)
#if (NX_USE_CHARACTER_API == 1)

#include "NxOgreLegacyCharacterController.h"
#include "NxOgreLegacyCharacter.h"			// Obviously
#include "NxOgreLegacyCharacterHitReport.h"	// For HitReports
#include "NxOgreUserAllocator.h"		// NxCreateControllerManager needs an allocator.
#include "NxOgreContainer.h"			// For mHitReports
#include "NxOgrePose.h"					// For conversions
#include "NxOgreHelpers.h"				// Name generation for duplicat names
#include "NxOgreActor.h"				// For Actor::getGroup
#include "NxOgreScene.h"				// For registerCharacter and unregisterCharacter in scene
#include "NxOgreUserData.h"

namespace NxOgre {


////////////////////////////////////////////////////////////////////////////////////////////////

void CharacterParams::setToDefault() {

	mDimensions.set(0.5f,1.0f,0.5f);
	mUpAxis = NX_Y;
	mSlopeLimit = 0.707f;
	mStepOffset = 0.5f;
	mSkinWidth = 0.1f;
	mType = CT_Box;
	mActiveGroupsMask = 1;
	mSharpness = 1.0f;
	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CharacterParams::parse(Parameters P) {

	for (Parameters::iterator p = P.begin(); p != P.end();p++) {
		if (Set("dimensions", (*p), mDimensions)) continue;
		if (Set("up", (*p), mUpAxis)) continue;
		if (Set("slopelimit", (*p), mSlopeLimit)) continue;
		if (Set("stepoffset", (*p), mStepOffset)) continue;
		if (Set("skinwidth", (*p), mSkinWidth)) continue;
		if ((*p).first == "type") {
			if ((*p).second.substr(0,1) == "b" || (*p).second.substr(0,1) == "B") {
				mType = CT_Box;
			}
			else {
				mType = CT_Capsule;
			}
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

CharacterController::CharacterController() {

	mAllocator = NxCreateAllocator("Character Controller");
	mControllerManager = NxCreateControllerManager(mAllocator);

	mBaseHitReport = new BaseCharacterHitReport();
}

////////////////////////////////////////////////////////////////////////////////////////////////

CharacterController::~CharacterController() {
/*
#if (NX_DEBUG == 1)
	mCharacters.dumpToConsole();
#endif
	mCharacters.destroyAllOwned();*/

	NxReleaseControllerManager(mControllerManager);
	delete mAllocator;
	delete mBaseHitReport;

}

////////////////////////////////////////////////////////////////////////////////////////////////

Character* CharacterController::create(const NxString &name, Pose p, CharacterParams cp, Scene* s) {

	if (mCharacters.has(name)) {
		NxString newname = NxCreateID(mCharacters.count(), name);
		Character* c = new Character(newname, s, this, p, cp);
		NxThrow_Conflict("Character name conflict '" + name + "' is now '" + newname + "'.");
		mCharacters.lock(newname, true);
		return c;
	}
	
	Character* c = new Character(name, s, this, p, cp);
	
	mCharacters.lock(name, true);
	s->mCharacters.lock(name, true);

	return c;

}

////////////////////////////////////////////////////////////////////////////////////////////////

void CharacterController::destroy(const NxString &name) {
	Character* character = mCharacters.get(name);
	if (mCharacters.isLocked(name))
		delete character;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Character* CharacterController::get(const NxString &name) {
	return mCharacters.get(name);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CharacterController::_registerCharacter(const NxString& name, Character* c) {
	mCharacters.insert(name, c);
	c->getScene()->_registerCharacter(name, c);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CharacterController::_unregisterCharacter(const NxString& name) {
	Character* c = mCharacters.get(name);
	c->getScene()->_unregisterCharacter(name);
	mCharacters.remove(name);
}

////////////////////////////////////////////////////////////////////////////////////////////////

NxControllerAction CharacterController::onShapeHit(const NxControllerShapeHit &hit) {

	if (hit.shape->getActor().userData == 0)
		return (NxControllerAction) CharacterHitReport::RS_None;

	NxUserData* aud = static_cast<NxUserData*>(hit.shape->getActor().userData);
	if (aud->getType() != NxUserData::T_Actor)
		return (NxControllerAction) CharacterHitReport::RS_None;

	CharacterHitReport::Response response = CharacterHitReport::RS_None;
	Character* character = static_cast<Character*>(hit.controller->getUserData());
	
	Actor* actor = aud->toActor();
	ActorGroup* group = actor->getGroup();
	Shape* shape = static_cast<Shape*>(hit.shape->userData);

	// Go through any existing reporters.
	
	for(CharacterHitReports::Iterator it = mHitReports.items.begin();it != mHitReports.items.end();++it) {
		
		CharacterHitReport::Response r = (*it)->onActor(character, actor, shape, group, hit);
	
		if (r != CharacterHitReport::RS_None) {
			return (NxControllerAction) r;
		}

	}

	// Fire up the final base reporter.
	return (NxControllerAction) mBaseHitReport->onActor(character, actor, shape, group, hit);

}

////////////////////////////////////////////////////////////////////////////////////////////////

NxControllerAction CharacterController::onControllerHit(const NxControllersHit &) {

	CharacterHitReport::Response response = CharacterHitReport::RS_None;

	// Go through any existing reporters.

	// Fire up the final reporter.
	//if (response == CharacterHitReport::RS_None)
		/// fire it up


	return (NxControllerAction) response;

}

////////////////////////////////////////////////////////////////////////////////////////////////


}  //End of NxOgre namespace.
#endif
#endif