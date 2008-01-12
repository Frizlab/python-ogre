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
#include "NxOgreHelpers.h"
#include "NxOgrePose.h"

#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "OgreMatrix3.h"
#include "OgreMatrix4.h"

namespace NxOgre {

//////////////////////////////////////////////////////////////////////////////////

NxExtendedVec3 toNxExtendedVec3(const Ogre::Vector3& v) {
	return NxExtendedVec3(v.x,v.y,v.z);
}

//////////////////////////////////////////////////////////////////////////////////

// Generates a 8 digit ID, based on groupCount and type of object (typeHash)
NxString NxCreateID(unsigned int groupCount, NxString typeHash) {
	std::stringstream ss;
	ss << typeHash << "-" << std::hex << (rand() % 1024) << "-" << groupCount;
	return ss.str();
}

//////////////////////////////////////////////////////////////////////////////////

NxMat33 toNxMat33(const Ogre::Matrix3& matrix) {
	
	NxReal* c = new NxReal[9];
	c[0] = matrix[0][0];	c[1] = matrix[1][0];		c[2] = matrix[2][0];
	c[3] = matrix[0][1];	c[4] = matrix[1][1];		c[5] = matrix[2][1];
	c[6] = matrix[0][2];	c[7] = matrix[1][2];		c[8] = matrix[2][2];

	NxMat33 m;
	m.id();
	m.setColumnMajor(c);

	delete [] c;

	return m;
}

//////////////////////////////////////////////////////////////////////////////////

NxMat34 toNxMat34(const Ogre::Matrix4& matrix) {
	
	NxUnderConstruction;

	NxMat34 m;
	m.id();
	
	return m;
}

//////////////////////////////////////////////////////////////////////////////////

NxVec3 operator* (const NxQuat& q, const NxVec3& v) {
	return q.rot(v);
}

////////////////////////////////////////////////////////////////

std::vector<NxString> NxStringSplit(const NxString& string, char delimiter, unsigned int maxSplits, bool trim) {

	std::vector<NxString> strings;
	std::istringstream stream(string);
	NxString element;

	int i=0;
	while (std::getline(stream, element, delimiter)) {
		
		if (maxSplits != 0 && i == maxSplits)
			break;

		if (trim) {
			static const NxString delims = " \t\r";
			element.erase(element.find_last_not_of(delims)+1);		// trim right
			element.erase(0, element.find_first_not_of(delims));	// trim left
		}

		strings.push_back(element);
		
		i++;
	}

	return strings;

}

////////////////////////////////////////////////////////////////

template <typename Type>
	NxString NxToString(const Type& type)
{
	std::stringstream stream;
	stream << type;
	return stream.str();
}

//////////////////////////////////////////////////////////////////////////////////

template <> NxString
	NxToString<NxVec3>(const NxVec3& vec3)
{
	std::stringstream stream;
	stream << vec3.x << " " << vec3.y << " " << vec3.z;
	return stream.str();
}

////////////////////////////////////////////////////////////////

template <> NxString
	NxToString<NxQuat>(const NxQuat& quat)
{
	std::stringstream stream;
	stream << quat.w << " " << quat.x << " " << quat.y << " " << quat.z;
	return stream.str();
}

////////////////////////////////////////////////////////////////

template <> NxString
	NxToString<Ogre::Vector3>(const Ogre::Vector3& vec3)
{
	std::stringstream stream;
	stream << vec3.x << " " << vec3.y << " " << vec3.z;
	return stream.str();
}

////////////////////////////////////////////////////////////////

template <> NxString
	NxToString<Ogre::Quaternion>(const Ogre::Quaternion& quat)
{
	std::stringstream stream;
	stream << quat.w << " " << quat.x << " " << quat.y << " " << quat.z;
	return stream.str();
}

////////////////////////////////////////////////////////////////

template <typename Type>
	Type NxFromString(const NxString& string)
{
	std::istringstream str(string);
	Type val = 0;
	str >> val;
	return val;
}

////////////////////////////////////////////////////////////////

template <> 
  NxVec3 NxFromString<NxVec3>(const NxString &string) {
	
	NxVec3 vector(0,0,0);
	std::vector<NxString> units = NxStringSplit(string, ' ', 3, true);
	
	if (units.size() != 3)
		return vector;

	vector.x = NxFromString<NxReal>(units[0]);
	vector.y = NxFromString<NxReal>(units[1]);
	vector.z = NxFromString<NxReal>(units[2]);

	return vector;
}

////////////////////////////////////////////////////////////////

template <>
  NxQuat NxFromString<NxQuat>(const NxString &string) {
	
	NxQuat quaternion;
	quaternion.id();
	std::vector<NxString> units = NxStringSplit(string, ' ', 4, true);
	
	if (units.size() != 4)
		return quaternion;

	quaternion.w = NxFromString<NxReal>(units[0]);
	quaternion.x = NxFromString<NxReal>(units[1]);
	quaternion.y = NxFromString<NxReal>(units[2]);
	quaternion.z = NxFromString<NxReal>(units[3]);

	return quaternion;

}

////////////////////////////////////////////////////////////////

template <>
  Ogre::Vector3 NxExport NxFromString<Ogre::Vector3>(const NxString& string) {

	Ogre::Vector3 vector(0,0,0);
	std::vector<NxString> units = NxStringSplit(string, ' ', 3, true);
	
	if (units.size() != 3)
		return vector;

	vector.x = NxFromString<NxReal>(units[0]);
	vector.y = NxFromString<NxReal>(units[1]);
	vector.z = NxFromString<NxReal>(units[2]);

	return vector;
}

////////////////////////////////////////////////////////////////

template <>	
  Ogre::Quaternion NxExport NxFromString<Ogre::Quaternion>(const NxString& string) {

	Ogre::Quaternion quat(1,0,0,0);
	std::vector<NxString> units = NxStringSplit(string, ' ', 4, true);
	
	if (units.size() != 4)
		return quat;

	quat.w = NxFromString<NxReal>(units[0]);
	quat.x = NxFromString<NxReal>(units[1]);
	quat.y = NxFromString<NxReal>(units[2]);
	quat.z = NxFromString<NxReal>(units[3]);

	return quat;

}

////////////////////////////////////////////////////////////////

NxHashIdentifier NxCreateHashFromString(const NxString& str) {
	// djb2 by Dan Bernstein
	NxHashIdentifier hash = 5381;
	int c; 
	for (NxU32 i=0;i < str.length();i++) {
		c = str[i];
		hash = ((hash << 5) + hash) + c; // hash*33 + c
	}
	return hash;
}

////////////////////////////////////////////////////////////////

NxShortHashIdentifier NxCreateShortHashFromString(const NxString& str) {
	// djb2 by Dan Bernstein
	NxShortHashIdentifier hash = 5381;
	int c; 
	for (NxU32 i=0;i < str.length();i++) {
		c = str[i];
		hash = ((hash << 5) + hash) + c; // hash*33 + c
	}
	return hash;
}

////////////////////////////////////////////////////////////////

Pose NxInterpolate(Pose First, Pose Second, NxReal c) {

	Pose r;
	
	r.v.x = First.v.x * (1 - c) + Second.v.x * c;
	r.v.y = First.v.y * (1 - c) + Second.v.y * c;
	r.v.z = First.v.z * (1 - c) + Second.v.z * c;
	r.q.slerp(c, First.q, Second.q);

	return r;
}

////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
