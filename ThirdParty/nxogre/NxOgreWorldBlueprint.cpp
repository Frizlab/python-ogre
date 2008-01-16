
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
#include "NxOgreWorldBlueprint.h"
#include "NxOgreSceneBlueprint.h"
#include "NxOgreContainer.h"
#include "NxOgreWorld.h"
#include "NxOgreScene.h"

namespace NxOgre {
namespace Blueprints {

////////////////////////////////////////////////////////////////////////////////////////////////

WorldBlueprint::WorldBlueprint(World* w) : mWorld(w) {
	toDefault();
}

////////////////////////////////////////////////////////////////////////////////////////////////

WorldBlueprint::~WorldBlueprint() {

}

////////////////////////////////////////////////////////////////////////////////////////////////

void WorldBlueprint::toDefault() {
	mSDKBlueprint.toDefault();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void WorldBlueprint::empty() {
	
	for(NxU32 i=0;i < mScenes.size();i++) {
		mScenes[i]->empty();
		delete mScenes[i];
	}

	toDefault();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void WorldBlueprint::unserialise() {

}

////////////////////////////////////////////////////////////////////////////////////////////////

void WorldBlueprint::serialise() {

	Scenes scenes;
	mWorld->getScenes()->CopyTo(scenes);

	for (Scene* scene = scenes.begin();scene = scenes.next();) {
		SceneBlueprint* bp = new SceneBlueprint();
		bp->serialise(scene);
		mScenes.push_back(bp);
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

std::map<NxString, float> WorldBlueprint::ParametersBlueprint::toStrings() {
	std::map<NxString, float> list;

	
	return list;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void WorldBlueprint::ParametersBlueprint::fromStrings(std::map<NxString, float>) {

}

////////////////////////////////////////////////////////////////////////////////////////////////

void WorldBlueprint::ParametersBlueprint::serialise() {
	
	NxPhysicsSDK* sdk = NxGetPhysicsSDK();
	mParameters.clear();
	
	for	(NxU32 i = NX_PENALTY_FORCE; i < NX_PARAMS_NUM_VALUES; i++) {
		mParameters[i] = sdk->getParameter( (NxParameter) i);
	}
	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void WorldBlueprint::ParametersBlueprint::unserialise() {

	NxPhysicsSDK* sdk = NxGetPhysicsSDK();

	for (std::map<NxU32, NxReal>::iterator it=mParameters.begin();it!=mParameters.end();it++) {
		sdk->setParameter(	(NxParameter) (*it).first, (*it).second);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

NxString WorldBlueprint::ParametersBlueprint::getStringFor(NxParameter) {

	return "";
}

////////////////////////////////////////////////////////////////////////////////////////////////

NxU32	 WorldBlueprint::ParametersBlueprint::getParameterFor(NxString) {

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

}  //End of Blueprints namespace.
}  //End of NxOgre namespace.
