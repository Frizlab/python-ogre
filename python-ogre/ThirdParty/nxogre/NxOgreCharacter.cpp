/** \file    NxOgreCharacter.cpp
 *  \see     NxOgreCharacter.h
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

#if (NX_USE_LEGACY_NXCONTROLLER == 0)

#include "NxOgreCharacter.h"
#include "NxOgreScene.h"
#include "NxOgreNxActorController.h"

#if (NX_USE_CHARACTER_API == 1)
#	include "NxOgreNxCharacterController.h"
#endif

namespace NxOgre {

/////////////////////////////////////////////////////////////

void CharacterParams::setToDefault() {

#if (NX_USE_CHARACTER_API == 1)
	mControllerType = CT_NXCONTROLLER;
#else
	mControllerType = CT_NXACTOR;
#endif

	mStepOffset = 0.0f;
	mSlopeLimit = 0.0f;
	mUpDirection = NX_AXIS_PLUS_Y;
	//mGroupMask;
	mControllerCollisionShape = 0;

}

/////////////////////////////////////////////////////////////

Character::Character(NxString identifier, Pose pose, CharacterModel* model, CharacterParams params, CharacterController* character_controller, Scene* scene) {
	
	if (model == 0) {
		NxThrow("Character Model pointer is null!");
		mDead = true;
		return;
	}

	if (params.mControllerType == CharacterParams::CT_CUSTOM && character_controller == 0) {
		NxThrow("ControllerType is Custom but pointer is null!");
		mDead = true;
		return;
	}

	mName = identifier;
	
	if (params.mControllerType == CharacterParams::CT_NXACTOR) {
		mController = new NxActorController(pose, params.mControllerCollisionShape, scene->getNxScene());
	}
#if (NX_USE_CHARACTER_API == 1)
	else if (params.mControllerType == CharacterParams::CT_NXCONTROLLER) {
		mController = new NxCharacterController(pose, params.mControllerCollisionShape, scene->getNxScene());

	}
#endif
	else {
		mController = character_controller;
	}

}

/////////////////////////////////////////////////////////////

Character::~Character() {

}

/////////////////////////////////////////////////////////////

}

#endif
