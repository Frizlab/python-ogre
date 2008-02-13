/** \file    NxOgreWheelSet.cpp
 *  \see     NxOgreWheelSet.h
 *  \version 1.0-20
 *
 *  \licence NxOgre a wrapper for the PhysX physics library.
 *           Copyright (C) 2005-8 Robin Southern of NxOgre.org http://www.nxogre.org
 *           This library is free software; you can redistribute it and/or
 *           modify it under the terms of the GNU Lesser General Public
 *           License as published by the Free Software Foundation; either
 *           version 2.1 of the License, or (at your option) any later version.
 *           
 *           This library is distributed in the hope that it will be useful,
 *           but WITHOUT ANY WARRANTY; without even the implied warranty of
 *           MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *           Lesser General Public License for more details.
 *           
 *           You should have received a copy of the GNU Lesser General Public
 *           License along with this library; if not, write to the Free Software
 *           Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "NxOgreStable.h"

#if 0

#include "NxOgreWheelSet.h"
#include "NxOgreMachine.h"
#include "NxOgreEngine.h"
#include "NxOgreActor.h"
#include "NxOgreHelpers.h"

namespace NxOgre {

/////////////////////////////////////////////////////////////

WheelSet::WheelSet(NxOgre::Actor *actor) : Machine(actor->getScene()), mActor(actor), mEngine(0)  {

}

/////////////////////////////////////////////////////////////

WheelSet::~WheelSet() {
	NxDelete(mEngine);
}

/////////////////////////////////////////////////////////////

WheelSet* WheelSet::createFourWheelSet(
				Actor* actor,
				const Ogre::Vector3& frontLeft,
				const Ogre::Vector3& backLeft,
				NxReal wheelRadius,
				ShapeParams shape_params,
				WheelParams wheel_params,
				NodeRenderableParams left_wheels_renderable,
				NodeRenderableParams right_wheels_renderable,
				NxDirection direction
) {

	if (actor == 0)
		return 0;

	WheelSet* wheelSet = new WheelSet(actor);

	shape_params.mLocalPose.t = NxConvert<NxVec3, Ogre::Vector3>(frontLeft);
	wheelSet->mWheels.insert(FRONT_LEFT, 
		static_cast<Wheel*>(
		actor->addShape(
			new WheelShape(wheelRadius, shape_params,  wheel_params, left_wheels_renderable)
		)
		));
		

	if (direction == NX_DIRECTION_X) {
		shape_params.mLocalPose.t.x = -shape_params.mLocalPose.t.x;
	}
	else if (direction == NX_DIRECTION_Y) {
		shape_params.mLocalPose.t.y = -shape_params.mLocalPose.t.y;
	}
	else {
		shape_params.mLocalPose.t.z = -shape_params.mLocalPose.t.z;
	}

	wheelSet->mWheels.insert(FRONT_RIGHT,
		static_cast<Wheel*> (
			actor->addShape(
				new WheelShape(wheelRadius, shape_params, wheel_params, right_wheels_renderable
				)
			)
		))
	;


	shape_params.mLocalPose.t = NxConvert<NxVec3, Ogre::Vector3>(backLeft);
	wheelSet->mWheels.insert(BACK_LEFT,
		static_cast<Wheel*> (
			actor->addShape(
				new WheelShape(wheelRadius, shape_params,  wheel_params, left_wheels_renderable
				)
			)
		))
	;

	if (direction == NX_DIRECTION_X) {
		shape_params.mLocalPose.t.x = -shape_params.mLocalPose.t.x;
	}
	else if (direction == NX_DIRECTION_Y) {
		shape_params.mLocalPose.t.y = -shape_params.mLocalPose.t.y;
	}
	else {
		shape_params.mLocalPose.t.z = -shape_params.mLocalPose.t.z;
	}

	wheelSet->mWheels.insert(BACK_RIGHT,
		static_cast<Wheel*> (
			actor->addShape(
				new WheelShape(wheelRadius, shape_params, wheel_params, right_wheels_renderable
				)
			)
		))
	;

	

	return wheelSet;
}

/////////////////////////////////////////////////////////////

void WheelSet::simulate(NxReal deltaTime) {

}

/////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.

#endif
