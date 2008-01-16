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

#ifndef __NXOGRE_POSE_H__
#define __NXOGRE_POSE_H__

#include "NxOgrePrerequisites.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"

namespace NxOgre {

	class NxExport Pose {

		public:

			Pose();
			Pose(const Ogre::Vector3&);
			Pose(const Ogre::Quaternion&);
			Pose(const Ogre::Vector3&, const Ogre::Quaternion&);
			Pose(const NxVec3&, const NxQuat&);
			Pose(const NxVec3&);
			Pose(const NxQuat&);
			Pose(NxReal _x, NxReal _y, NxReal _z);
			Pose(NxReal _w, NxReal _x, NxReal _y, NxReal _z);

			////////////////////////////////////////////////////////////////////////////////

			void				id();
			void				set(const Ogre::Vector3& _v);
			void				set(const Ogre::Quaternion& _q);
			bool				isZero();

			void						fromMat34(const NxMat34&);
			NxMat34						toMat34() const;
			void						fromMat33(const NxMat33&);
			NxMat33						toMat33() const;

				Ogre::Vector3		getVector3() const;
				NxVec3				getVec3() const;
				Ogre::Quaternion	getQuaternion() const;
				NxQuat				getQuat() const;

							NxString	toString() const;

			////////////////////////////////////////////////////////////////////////////////

			operator Ogre::Vector3 (void) const {
				return Ogre::Vector3(v.x,v.y,v.z);
			}

			operator Ogre::Quaternion(void) const {
				return Ogre::Quaternion(q.w,q.x,q.y,q.z);
			}

			operator NxVec3 (void) const {
				return v;
			}

			operator NxQuat(void) const {
				return q;
			}

			operator Ogre::String(void) const {
				return toString();
			}

			Pose& operator= (const Ogre::Vector3& _v) {
				v.set(_v.x,_v.y,_v.z);
				return *this;
			}

			Pose& operator= (const Ogre::Quaternion& _q) {
				q.setWXYZ(_q.w,_q.x,_q.y,_q.z);
				return *this;
			}

			////////////////////////////////////////////////////////////////////

			NxVec3 v;
			NxQuat q;

	};

};

#endif
