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
#ifndef __NXOGRE_ENGINE_H__
#define __NXOGRE_ENGINE_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreMachine.h"

namespace NxOgre {

	class NxExport Engine : public Machine {
		
		public:
		
			Engine(Scene* scene) : Machine(scene) {}
			
			NxString getIdentifierType() {return "NxOgre-Engine";}
			NxShortHashIdentifier getIdentifierTypeHash() const {return 4987;}
			
			virtual void simulate(NxReal deltaTime) = 0;
			virtual void setAcceleration(NxReal pressure) = 0;

	};

	///class NxExport ElectricMotor : public Engine {
	///		
	///};

	class NxExport CombustionEngine : public Engine {
	public:

		CombustionEngine(Scene* scene, Wheels* DrivingWheels) : Engine(scene), mWheels(DrivingWheels) {}

		NxString getIdentifierType() {return "NxOgre-CombustionEngine";}
		NxShortHashIdentifier getIdentifierTypeHash() const {return 57758;}

		virtual void simulate(NxReal deltaTime) = 0;
		virtual void setAcceleration(NxReal pressure) = 0;
		virtual void changeGear(NxU32 gear) = 0;
		virtual NxU32 getGear() const = 0;


		Wheels* mWheels;

	};


	class NxExport InternalCombustionEngine : public CombustionEngine {
	public:

		InternalCombustionEngine(Scene*, Wheels* DrivingWheels);

		NxString getIdentifierType() {return "NxOgre-InternalCombustionEngine";}
		NxShortHashIdentifier getIdentifierTypeHash() const {return 22427;}

		void simulate(NxReal deltaTime);
		void setAcceleration(NxReal pressure);
		void changeGear(NxU32 gear);
		NxU32 getGear() const;

		unsigned int gear;

	};

};

#endif

#if 0
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
