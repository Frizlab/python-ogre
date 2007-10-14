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

#ifndef __NXOGRE_JOINTSET2_H__
#define __NXOGRE_JOINTSET2_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreJoint.h"

namespace NxOgre {



	/** Cylindrical Joints
		A Cylindrical joint is the simplest kind of joint. It constrains two points on two different bodies 
		from coinciding. This point, located in world space, is the only parameter that has to be specified 
		(the other parameters are optional). Specifying the anchor point (point that is forced to coincide) 
		in world space guarantees that the point in the local space of each body will coincide when the 
		point is transformed back from local into world space. 
	
		@note Params specific to CylindricalJoint


	*/

	class NxExport CylindricalJoint : public Joint {
		
	public:

		CylindricalJoint(Actor*, Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
		CylindricalJoint(Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
		~CylindricalJoint();

		NxCylindricalJoint* getJoint() {return mCylindricalJoint;}

	protected:

		void __createJoint(const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams&);

		NxCylindricalJoint*		mCylindricalJoint;
		NxCylindricalJointDesc	mDescription;

	};

	/** Fixed Joints
		The fixed joint effectively glues two bodies together with no remaining degrees of freedom 
		for relative motion.

		@note Params specific to FixedJoint


	*/

	class NxExport FixedJoint : public Joint {
		
	public:

		FixedJoint(Actor*, Actor*, JointParams = JointParams());
		FixedJoint(Actor*, JointParams = JointParams());
		~FixedJoint();

		NxFixedJoint* getJoint() {return mFixedJoint;}

	protected:

		void __createJoint(JointParams&);

		NxFixedJoint*		mFixedJoint;
		NxFixedJointDesc	mDescription;

	};
};

#endif
