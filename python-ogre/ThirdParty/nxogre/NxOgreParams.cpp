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
#include "NxOgreParams.h"
#include "NxOgreHelpers.h"

#include "OgreString.h"
#include "OgreStringConverter.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"

namespace NxOgre {

/////////////////////////////////////////////////////////////

void Params::process(const NxString& p) {
	
	setToDefault();

	if (p == "")
		return;

	Parameters pm = fromString(p);
	parse(pm);

}

/////////////////////////////////////////////////////////////

Params::Parameters Params::fromString(const NxString& p) {
	Parameters pm;
	std::vector<NxString> d = Ogre::StringUtil::split(p,",");

	for(std::vector<NxString>::iterator it = d.begin();it != d.end();++it) {
		Parameter pa;
		toParameter((*it), pa);
		pm.push_back(pa);
	}
	return pm;
}

/////////////////////////////////////////////////////////////

bool Params::Set(const NxString& i, Parameter &d, int &t) {
	if (d.first == i) {
		t = Ogre::StringConverter::parseInt(d.second);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////

bool Params::Set(const NxString& i, Parameter &d, unsigned int &t) {
	if (d.first == i) {
		t = Ogre::StringConverter::parseUnsignedInt(d.second);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////

bool Params::Set(const NxString& i, Parameter &d, NxMaterialIndex &t) {
	if (d.first == i) {
		t = Ogre::StringConverter::parseUnsignedInt(d.second);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////

bool Params::Set(const NxString& i, Parameter &d, bool &t) {
	if (d.first == i) {
		t = Ogre::StringConverter::parseBool(d.second);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////

bool Params::Set(const NxString& i, Parameter &d, NxReal &t) {
	if (d.first == i) {
		t = Ogre::StringConverter::parseReal(d.second);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////

bool Params::Set(const NxString& i, Parameter &d, NxVec3& t) {
	if (d.first == i) {
		t = NxConvert<NxVec3, Ogre::Vector3>(Ogre::StringConverter::parseVector3(d.second));
		return true;
	}
	return false;
}

bool Params::Set(const NxString &i , Parameter &d, NxQuat& t) {
	if (d.first == i) {
		Ogre::Quaternion q = Ogre::StringConverter::parseQuaternion(d.second);
		t.setXYZW(q.x,q.y,q.z,q.z);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////

bool Params::Set(const NxString& i, Parameter &d, Ogre::Vector3 &t) {
	if (d.first == i) {
		t = Ogre::StringConverter::parseVector3(d.second);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////

bool Params::Set(const NxString& i, Parameter &d, Ogre::Quaternion &t) {
	if (d.first == i) {
		t = Ogre::StringConverter::parseQuaternion(d.second);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////

bool Params::Set(const NxString& i, Parameter &d, NxString &t) {
	if (d.first == i) {
		t = d.second;
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////

bool Params::Set(const NxOgre::NxString& i, Parameter &d, NxHeightFieldAxis &t) {
	if (d.first == i) {
		std::string tp = d.second.substr(0,1);
		std::transform(tp.begin(),tp.end(),tp.begin(),tolower);
		if (tp == "x") {
			t = NX_X;
			return true;
		}
		else if (tp == "y") {
			t = NX_Y;
			return true;
		}
		else if (tp == "z") {
			t = NX_Z;
			return true;
		}

		return false;
	}
	return false;
}

/////////////////////////////////////////////////////////////

void Params::toParameter(const NxString& s, Parameter &d) {
	int dp = s.find_first_of(":");

	// Split
	d.first = s.substr(0,dp);
	d.second = s.substr(dp+1, s.length() - dp - 1);
	
	// Trim
	static const std::string delims = " \t\r";
    d.first.erase( d.first.find_last_not_of(delims)+1);
    d.first.erase(0,  d.first.find_first_not_of(delims));
	
    d.second.erase( d.second.find_last_not_of(delims)+1);
    d.second.erase(0,  d.second.find_first_not_of(delims));

	
	// Lower case first.
	std::transform(d.first.begin(),d.first.end(),d.first.begin(),tolower);
}

/////////////////////////////////////////////////////////////

void Params::toLower(NxString& s) {
	std::transform(s.begin(),s.end(),s.begin(),tolower);
}

/////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.

