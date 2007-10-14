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
#include "NxOgreJointSet2.h"
#include "NxOgreJoint.h"			// For: These joints inherit Joint
#include "NxOgreActor.h"			// For: a::getNxActor, b::getNxActor
#include "NxOgreHelpers.h"			// For: Vector3 to NxVec3 conversion
#include "NxOgreScene.h"			// For: mScene::getNxScene()

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////





// CylindricalJoint




////////////////////////////////////////////////////////////////////////////////////

CylindricalJoint::CylindricalJoint(Actor *a ,Actor *b, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams jp) : Joint(a,b){
	mDescription.setToDefault();
	mDescription.jointFlags = 0;
	ActorsToDescription(a, b, mDescription);
	ParamsToDescription(jp, mDescription);
	__createJoint(axis,anchor,jp);
}

////////////////////////////////////////////////////////////////////////////////////

CylindricalJoint::CylindricalJoint(Actor *a, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams jp) : Joint (a) {
	mDescription.setToDefault();
	mDescription.jointFlags = 0;
	ActorToDescription(a, mDescription);
	ParamsToDescription(jp, mDescription);
	__createJoint(axis,anchor, jp);
}

////////////////////////////////////////////////////////////////////////////////////

void CylindricalJoint::__createJoint(const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams& jp) {
	
	mCallback = 0;
	mDescription.setGlobalAnchor(toNxVec3(anchor));
	mDescription.setGlobalAxis(toNxVec3(axis));
	mDescription.userData = this;

	mCallback = jp.mBreakableCallback;
	mJoint = mScene->getNxScene()->createJoint(mDescription);
	mCylindricalJoint = static_cast<NxCylindricalJoint*>(mJoint);
	

}

////////////////////////////////////////////////////////////////////////////////////

CylindricalJoint::~CylindricalJoint() {

	if (mJoint)
		mScene->getNxScene()->releaseJoint(*mJoint);

	mCylindricalJoint = 0;
	if (mCallback)
		delete mCallback;
}

////////////////////////////////////////////////////////////////////////////////////





// FixedJoint




////////////////////////////////////////////////////////////////////////////////////

FixedJoint::FixedJoint(Actor *a ,Actor *b, JointParams jp) : Joint(a,b){
	mDescription.setToDefault();
	mDescription.jointFlags = 0;
	ActorsToDescription(a, b, mDescription);
	ParamsToDescription(jp, mDescription);
	__createJoint(jp);
}

////////////////////////////////////////////////////////////////////////////////////

FixedJoint::FixedJoint(Actor *a, JointParams jp) : Joint (a) {
	mDescription.setToDefault();
	mDescription.jointFlags = 0;
	ActorToDescription(a, mDescription);
	ParamsToDescription(jp, mDescription);
	__createJoint(jp);
}

////////////////////////////////////////////////////////////////////////////////////

void FixedJoint::__createJoint(JointParams& jp) {
	
	mCallback = 0;

	mDescription.userData = this;

	mCallback = jp.mBreakableCallback;
	mJoint = mScene->getNxScene()->createJoint(mDescription);
	mFixedJoint = static_cast<NxFixedJoint*>(mJoint);
	

}

////////////////////////////////////////////////////////////////////////////////////

FixedJoint::~FixedJoint() {
	
	if (mJoint)
		mScene->getNxScene()->releaseJoint(*mJoint);

	mFixedJoint = 0;
	if (mCallback)
		delete mCallback;
}

////////////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
