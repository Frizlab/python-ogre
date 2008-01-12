//
//	NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org
//
//	This library is free software{ you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation{ either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY{ without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library{ if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "NxOgreStable.h"
#include "NxOgreNxSerialiser.h"
#include "NxOgreWorldBlueprint.h"

#if (NX_USE_JSON == 1)
#	include "NxOgreJSONCollection.h"
#endif

namespace NxOgre {
namespace Serialiser {
namespace NxSerialiser {

//////////////////////////////////////////////////////////////////////

Serialiser::Serialiser(World* w) : mWorld(w) {

}

//////////////////////////////////////////////////////////////////////

Serialiser::~Serialiser() {

}


//////////////////////////////////////////////////////////////////////

#if (NX_USE_JSON == 1)

bool Serialiser::importJSONFromFile(NxString filename) {

	Blueprints::WorldBlueprint* mWorldBlueprint = new Blueprints::WorldBlueprint(mWorld);

//	mWorldBlueprint->
//	JSONCollection* mCollection = new JSONCollection(mWorld);
//	mCollection->importCollection(mWorldBlueprint, filename);


	return true;
}

#endif

//////////////////////////////////////////////////////////////////////

#if (NX_USE_TINYXML == 1)

bool Serialiser::importXMLFromFile(NxString filename) {

	return true;
}

#endif

//////////////////////////////////////////////////////////////////////

bool Serialiser::importBinaryFromFile(NxString filename) {

	return true;
}
			
//////////////////////////////////////////////////////////////////////

}; //End of NxSerialiser namespace.
}; //End of Serialiser namespace.
}; //End of NxOgre namespace.
