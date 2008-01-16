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


namespace NxOgre {

 NxVec3 toNxVec3(const Ogre::Vector3& v) {
	return NxVec3(v.x,v.y,v.z);
}

 Ogre::Vector3 toVector3(const NxVec3& v) {
	return Ogre::Vector3(v.x, v.y, v.z);
}

 Ogre::Quaternion toQuaternion(const NxQuat& q) {
	return Ogre::Quaternion(q.w, q.x, q.y, q.z);
}

 NxQuat toNxQuat(const Ogre::Quaternion& q) {
	NxQuat Q;
	Q.setXYZW(q.x,q.y,q.z,q.w);
	return Q;
}

 NxExtendedVec3 toNxExtendedVec3(const Ogre::Vector3& v) {
	return NxExtendedVec3(v.x,v.y,v.z);
}

// Generates a 8 digit ID, based on groupCount and type of object (typeHash)
 NxString NxCreateID(unsigned int groupCount, NxString typeHash) {
	std::stringstream ss;
	ss << typeHash << "-" << std::hex << (rand() % 1024) << "-" << groupCount;
	return ss.str();
}

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

 NxMat34 toNxMat34(const Ogre::Matrix4& matrix) {
	
	NxUnderConstruction;

	NxMat34 m;
	m.id();
	
	return m;
}

//////////////////////////////////////////////////////////////////////////////////


}; //End of NxOgre namespace.
