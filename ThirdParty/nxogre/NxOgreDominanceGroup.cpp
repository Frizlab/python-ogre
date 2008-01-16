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
#include "NxOgreDominanceGroup.h"


#if (NX_SDK_VERSION_NUMBER >= 272) 

#include "NxOgreScene.h"			// For: getActorGroups(), getActorGroupsByIndex(), ... in NxScene
#include "NxOgreContainer.h"		// For: Getting ID's for containers and such.

namespace NxOgre {

//////////////////////////////////////////////////////////////////////

#define NxMaxDominanceGroups 0x1F			// 31

NxCollisionGroup DominanceGroup::mNextFreeID = 0;

//////////////////////////////////////////////////////////////////////
// DominanceGroup
//
//
//////////////////////////////////////////////////////////////////////

DominanceGroup::DominanceGroup(const NxString& identifier, Scene* scene) : mScene(scene), mName(identifier) {

	mGroupID = mNextFreeID;

	if (scene->getDominanceGroupsByIndex()->has(mNextFreeID)) {
		DominanceGroupsByIndex* ag = scene->getDominanceGroupsByIndex();
		for (NxU32 i=0; i < NxMaxDominanceGroups;i++) {
			if (ag->has(i))
				continue;
			mGroupID = i;
			break;
		}
	}
	mNextFreeID = mGroupID + 1;
	scene->_registerDominanceGroup(mName, this);
#ifdef NX_DEBUG
	std::stringstream ss;
	ss << "Created DominanceGroup '" << identifier << "' with GroupID as " << mGroupID << std::endl;
	NxDebug(ss.str());
#endif
}

//////////////////////////////////////////////////////////////////////

DominanceGroup::DominanceGroup(NxCollisionGroup GroupID, const NxString& identifier, Scene* scene) : mScene(scene), mName(identifier), mGroupID(GroupID) {
	scene->_registerDominanceGroup(mName, this);
	
	// Default ID.
	if (mGroupID == 0)
		mNextFreeID = 1;
#ifdef NX_DEBUG
	std::stringstream ss;
	ss << "Created DominanceGroup '" << identifier << "' manually with GroupID as " << mGroupID << std::endl;
	NxDebug(ss.str());
#endif
}

//////////////////////////////////////////////////////////////////////

DominanceGroup::~DominanceGroup() {
	mScene->_unregisterDominanceGroup(mName);
	mNextFreeID = mGroupID;

#ifdef NX_DEBUG
	std::stringstream ss;
	ss << "Destroyed DominanceGroup '" << mName << "'." << std::endl;
	NxDebug(ss.str());
#endif
}

//////////////////////////////////////////////////////////////////////

#undef NxMaxDominanceGroups

//////////////////////////////////////////////////////////////////////

#endif

}; //End of NxOgre namespace.
