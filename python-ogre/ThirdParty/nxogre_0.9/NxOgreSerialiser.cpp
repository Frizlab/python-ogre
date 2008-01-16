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
#include "NxOgreSerialiser.h"

#if (NX_USE_NXUSTREAM == 1)
#	include "NxOgreNxuStreamSerialiser.h"
#endif

#include "NxOgreNxSerialiser.h"


namespace NxOgre {
namespace Serialiser {

//////////////////////////////////////////////////////////////////////

Serialiser::SerialiserBase* SerialiserBase::mSerialiser = 0;

//////////////////////////////////////////////////////////////////////

void exportWorldToFile(NxOgre::Serialiser::BlueprintsFormat f, NxString n) {
	SerialiserBase::exportWorldToFile(f,n);
}

//////////////////////////////////////////////////////////////////////

void importWorldFromFile(NxOgre::Serialiser::BlueprintsFormat f, NxString n) {
	SerialiserBase::importWorldFromFile(f, n);
}

//////////////////////////////////////////////////////////////////////

NxString exportWorldToString(NxOgre::Serialiser::BlueprintsFormat f) {
	return SerialiserBase::exportWorldToString(f);
}

//////////////////////////////////////////////////////////////////////

void importWorldFromString(NxOgre::Serialiser::BlueprintsFormat f, NxString s) {
	SerialiserBase::importWorldFromString(f, s);
}

//////////////////////////////////////////////////////////////////////

void SerialiserBase::exportWorldToFile(NxOgre::Serialiser::BlueprintsFormat f, NxString s) {
	
	if (mSerialiser == 0)
		return;

	switch(f) {

#if (NX_USE_NXUSTREAM == 1)
		case SF_Ageia_XML:					break;
#endif


#if (NX_USE_NXUSTREAM == 1)
		case SF_Ageia_COLLADA:				break;
#endif

#if (NX_USE_NXUSTREAM == 1)
		case SF_Ageia_Binary:				break;
#endif

#if (NX_USE_JSON == 1)
		case SF_JSON:						break;
#endif

#if (NX_USE_TINYXML == 1)
		case SF_XML:						break;
#endif

		default:
		case SF_Binary:						break;

	}

}

//////////////////////////////////////////////////////////////////////

void SerialiserBase::importWorldFromFile(NxOgre::Serialiser::BlueprintsFormat f, NxString s) {
	
	if (mSerialiser == 0)
		return;

	switch(f) {

#if (NX_USE_NXUSTREAM == 1)
		case SF_Ageia_XML:					break;
#endif


#if (NX_USE_NXUSTREAM == 1)
		case SF_Ageia_COLLADA:				break;
#endif

#if (NX_USE_NXUSTREAM == 1)
		case SF_Ageia_Binary:				break;
#endif

#if (NX_USE_JSON == 1)
		case SF_JSON:				mSerialiser->mNxOgreSerialiser->importJSONFromFile(s);		break;
#endif

#if (NX_USE_TINYXML == 1)
		case SF_XML:				mSerialiser->mNxOgreSerialiser->importXMLFromFile(s);		break;
#endif

		default:
		case SF_Binary:				mSerialiser->mNxOgreSerialiser->importBinaryFromFile(s);	break;

	}

}

//////////////////////////////////////////////////////////////////////

NxString SerialiserBase::exportWorldToString(NxOgre::Serialiser::BlueprintsFormat f) {
	return "";
}

//////////////////////////////////////////////////////////////////////

void SerialiserBase::importWorldFromString(NxOgre::Serialiser::BlueprintsFormat f, NxString s) {


}

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////

}; //End of Serialiser namespace.
}; //End of NxOgre namespace.
