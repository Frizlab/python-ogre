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
#include "NxOgreSceneTriggerController.h"
#include "NxOgreScene.h"
#include "NxOgreTrigger.h"
#include "NxOgreUserData.h"

namespace NxOgre {

//////////////////////////////////////////////////////////////////////////////

SceneTriggerController::SceneTriggerController(Scene* scene) : mScene(scene) {
	mNxScene = mScene->getNxScene();
	mTriggers = mScene->getTriggers();
}

//////////////////////////////////////////////////////////////////////////////

SceneTriggerController::~SceneTriggerController() {

}

//////////////////////////////////////////////////////////////////////////////

void SceneTriggerController::onTrigger(NxShape& ts, NxShape& as, NxTriggerFlag tf) {

	if (!ts.getActor().userData && !as.getActor().userData) {
		return;
	}

	NxActorUserData* aud = static_cast<NxActorUserData*>(as.getActor().userData);
	Trigger *t = (static_cast<NxActorUserData*>(ts.getActor().userData))->toTrigger();

#if (NX_USE_CHARACTER_API == 1)	

	switch (aud->getType()) {

		case NxActorUserData::T_Actor: 
			{
					Actor *a = aud->toActor();
					switch(tf) {
						case NX_TRIGGER_ON_ENTER:
							t->onEnter(a);
						break;

						case NX_TRIGGER_ON_LEAVE:
							t->onLeave(a);
						break;

						case NX_TRIGGER_ON_STAY:
							t->onInside(a);
						break;
					}
			}
		break;

		case NxActorUserData::T_Character:
			{
				Character* c = aud->toCharacter();
				switch(tf) {
					case NX_TRIGGER_ON_ENTER:
						t->onEnter(c);
					break;

					case NX_TRIGGER_ON_LEAVE:
						t->onLeave(c);
					break;

					case NX_TRIGGER_ON_STAY:
						t->onInside(c);
					break;
				}
			}
		break;
	};

#else

	Actor *a = aud->toActor();

	switch(tf) {
		case NX_TRIGGER_ON_ENTER:
			t->onEnter(a);
		break;

		case NX_TRIGGER_ON_LEAVE:
			t->onLeave(a);
		break;

		case NX_TRIGGER_ON_STAY:
			t->onInside(a);
		break;
	}

#endif
}

//////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
