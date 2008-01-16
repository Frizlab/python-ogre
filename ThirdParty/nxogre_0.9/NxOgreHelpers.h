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

#ifndef __NXOGRE_HELPERS_H__
#define __NXOGRE_HELPERS_H__

#include "NxOgrePrerequisites.h"
#include "NxExtended.h"

namespace NxOgre {

	NxVec3 NxExport toNxVec3(const Ogre::Vector3& v);
	Ogre::Vector3 NxExport toVector3(const NxVec3& v);
	Ogre::Quaternion NxExport toQuaternion(const NxQuat& q);
	NxQuat NxExport toNxQuat(const Ogre::Quaternion& q);
	NxExtendedVec3 NxExport toNxExtendedVec3(const Ogre::Vector3& v);
	NxMat33 toNxMat33(const Ogre::Matrix3&);
	NxMat34 toNxMat34(const Ogre::Matrix4&);
	NxString NxExport NxCreateID(unsigned int groupCount, NxString typeHash);

	////////////////////////////////////////////////////////////////////////////////////////

	void NxExport NxCookTriMeshToDisk(const Ogre::String& meshName);
	void NxExport NxCookConvexToDisk(const Ogre::String& meshName);

};

#endif
