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
#include "NxOgreSceneContactController.h"
#include "NxOgreScene.h"
#include "NxOgreGroup.h"
#include "NxOgreUserData.h"

namespace NxOgre {

//////////////////////////////////////////////////////////////////////////////

SceneContactController::SceneContactController(Scene* scene) : mScene(scene) {
	mNxScene = mScene->getNxScene();
	mActors = mScene->getActors();
	mActorGroups = mScene->getActorGroups();
	mActorGroupsIndexed = mScene->getActorGroupsByIndex();
}

//////////////////////////////////////////////////////////////////////////////

SceneContactController::~SceneContactController() {

}

//////////////////////////////////////////////////////////////////////////////

void SceneContactController::onContactNotify(NxContactPair &pair, NxU32 events) {

	Actor *a, *b;
	
	if (pair.actors[0]->userData && pair.actors[1]->userData) {
		a = (static_cast<NxActorUserData*>(pair.actors[0]->userData))->toActor();
		b = (static_cast<NxActorUserData*>(pair.actors[1]->userData))->toActor();
	}
	else {
		return;
	}

	NxActorGroup agid = pair.actors[0]->getGroup();
	NxActorGroup bgid = pair.actors[1]->getGroup();

	ActorGroup *ag, *bg;

	switch(events) {

		case NX_NOTIFY_ON_TOUCH:
			
			ag = mActorGroupsIndexed->get(agid);
			bg = mActorGroupsIndexed->get(bgid);
			
			if (ag)
				ag->onTouch(a, b);

			if (bg)
				bg->onTouch(b, a);

		break;

		case NX_NOTIFY_ON_START_TOUCH:
			
			ag = mActorGroupsIndexed->get(agid);
			bg = mActorGroupsIndexed->get(bgid);
			
			if (ag)
				ag->onStartTouch(a, b);

			if (bg)
				bg->onStartTouch(b, a);

		break;

		case NX_NOTIFY_ON_END_TOUCH:

			ag = mActorGroupsIndexed->get(agid);
			bg = mActorGroupsIndexed->get(bgid);
			
			if (ag)
				ag->onEndTouch(a, b);

			if (bg)
				bg->onEndTouch(b, a);

		break;
	}

}

//////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
