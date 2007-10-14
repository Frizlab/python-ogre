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
#ifndef __NXOGRE_MOTOR_H__
#define __NXOGRE_MOTOR_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreHelper.h"

namespace NxOgre {

	//////////////////////////////////////////////////////////////////////////////////////

	class NxExport DriveShaft : public Helper {

		public:
			
			DriveShaft(const NxString& name, Scene* scene, Wheels* wheels) : Helper(name, scene), Drive(wheels) {}

			virtual void simulate(float deltaTime) {}

		protected:

			Wheels* Drive;
	};

	
	//////////////////////////////////////////////////////////////////////////////////////

	class NxExport Motor : public DriveShaft {

		public:

				Motor(const NxString& name, Scene* scene, Wheels* w);

				virtual void simulate(float deltaTime);

				void setAcceleration(NxReal acceleration);			// 0..1
				void setBreaking(NxReal brake);						// 0..1
				void shiftGear(unsigned int gear);					// -5..0..5
				int getGear() const;					

				int		mCurrentGear;
				NxReal	mEngineTorque;
				NxReal	mGear[6];
				NxReal	mFinalGear;
				NxReal	mRollingFriction;
				NxReal	mRedline;
				NxReal	mAcceleration;
				NxReal	mBrake;

	};

	//////////////////////////////////////////////////////////////////////////////////////

};
#endif
