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
#include "NxOgreMotor.h"
#include "NxOgreShapeWheel.h"
#include "NxOgreContainer.h"

namespace NxOgre {

//////////////////////////////////////////////////////////////////////////////////////

Motor::Motor(const NxString& name, Scene* scene, Wheels* w) : DriveShaft(name,scene,w) {
	mCurrentGear = 1;
	mEngineTorque = 1000;
	mAcceleration = 0.0f;
	mRollingFriction = 0.015f;
	mGear[0] = 3.82f;
	mGear[1] = 3.82f;
    mGear[2] = 2.20f;
    mGear[3] = 1.52f;
    mGear[4] = 1.22f;
    mGear[5] = 1.02f;
    mGear[6] = 0.84f;
	mFinalGear = 3.44f;
	mRedline = 7200;
}

//////////////////////////////////////////////////////////////////////////////////////

void Motor::simulate(float deltaTime) {
	
	NxReal torque;
 /*
	NxReal b, d;
    if ( mEngineTorque  <= 1000.0 ) {
      b = 0.0;
      d = 220.0;
    }
    else if ( mEngineTorque  < 4600.0 ) {
      b = 0.025;
      d = 195.0; 
    }
    else {
      b = -0.032;
      d = 457.2;
    }
*/

	torque = ((mAcceleration * 7200.0f) * mGear[mCurrentGear] * mFinalGear) / (0.5f * 2000.0f);

	for(Wheels::Iterator i = Drive->items.begin();i != Drive->items.end();++i) {
			(*i)->setMotorTorque(torque);
	}

}

//////////////////////////////////////////////////////////////////////////////////////

void Motor::setAcceleration(NxReal acceleration) {			// 0..1
	mAcceleration = acceleration;
}

//////////////////////////////////////////////////////////////////////////////////////

void Motor::setBreaking(NxReal brake) {						// 0..1
	mBrake = brake;
}

//////////////////////////////////////////////////////////////////////////////////////

void Motor::shiftGear(unsigned int gear) {					// -5..0..5

	if (mCurrentGear + gear > 6)
		return;

	if (mCurrentGear + gear < 0)
		return;
	
	NxReal ogr = mGear[mCurrentGear];
	mCurrentGear+=gear;
    
	mEngineTorque += mGear[mCurrentGear] / ogr;

}

//////////////////////////////////////////////////////////////////////////////////////

int Motor::getGear() const {
	return mCurrentGear;
}

//////////////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
