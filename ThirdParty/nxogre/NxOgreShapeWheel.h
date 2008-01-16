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

#ifndef __NXOGRE_SHAPE_WHEEL_H__
#define __NXOGRE_SHAPE_WHEEL_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreShape.h"				// For: Primitives inherits Shape
#include "NxOgreParams.h"				// For: WheelParams
#include "NxOgreContainer.h"			// For: List<Wheel>/Wheels
#include "NxOgreRenderableSource.h"
#include "NxOgreNodeRenderable.h"

namespace NxOgre {
		
	//////////////////////////////////////////////////////////////////////////////////////

	class NxExport WheelParams : public Params {

		public:

			WheelParams() {}
			WheelParams(const char* p) {process(p);}
			WheelParams(NxString p) {process(p);}

			void			setToDefault();
			void			setToNormal();
			void			parse(Parameters);	

			NxReal			power_factor,
							steer_factor,
							steer_limit,
							break_factor,

							motor_torque,
							brake_torque,
							density,
							mass,
							radius,
							inverseWheelMass,

							lateral_tire_extremumSlip,
							lateral_tire_extremumValue,
							lateral_tire_asymptoteSlip,
							lateral_tire_asymptoteValue,	
							lateral_tire_stiffnessFactor,
						
							longitudal_tire_extremumSlip,
							longitudal_tire_extremumValue,
							longitudal_tire_asymptoteSlip,
							longitudal_tire_asymptoteValue,
							longitudal_tire_stiffnessFactor,

							suspension_spring,
							suspension_damper,
							suspension_target,
							suspension_travel;

			Ogre::Radian	steer_angle;
					
			bool			axle_speed_override,
							clamped_friction,
							emulate_legacy_wheel,
							input_lat_slipvelocity,
							input_long_slipvelocity,
							unscaled_spring_behavior,
							axis_contact_normal;

	};
	

	
	//////////////////////////////////////////////////////////////////////////////////////

	class NxExport Wheel : public Shape, public RenderableSource {
	
		friend class WheelShape;

		public:

			Wheel(WheelShape, WheelParams, NodeRenderableParams, Actor*);
			~Wheel();

			NxShortHashIdentifier	getType() const {return NxHashes_Wheel;	/* "NxOgre-Wheel" */}
			NxString				getStringType() const {return "NxOgre-Wheel";}

			void turn(Ogre::Radian angle);
			void setRadius(NxReal);
			void setSuspensionTravel(NxReal);
			void setMotorTorque(NxReal);
			void setBrakeTorque(NxReal);
			void setSteeringAngle(Ogre::Radian);


			NxWheelShape*		getNxWheelShape(void) { return mWheel; }

		
		protected:

			Pose					__calculatePositionHigh();
			Pose					__calculatePositionMedium();
			Pose					__calculatePositionLow();

			void					__renderSelf();

			virtual bool isDynamic() {return true;}
			virtual bool isStaticOnly(){return false;}

			void _bindNxShapeToShape(NxShape*);

			NxWheelShape *mWheel;

			NxReal  mWheelRollAngle;
			NxMat34 mWheelLocalPose;
			//NxMat34 mLastPose;

		private:
	};

	//////////////////////////////////////////////////////////////////////////////////////
#if 0
	
	class NxExport WheelSet {
    public:
 
		static WheelSet createThreeWheelSet(
			Actor*,
			Ogre::Vector3 centerWheel,
			Ogre::Vector3 backLeft,
			NxReal radius = 0.5f,
			ShapeParams sp = "",
			WheelParams wp = "use-params:normal"
		);

		static WheelSet createFourWheelSet(
			Actor*,
			Ogre::Vector3 frontLeft,
			Ogre::Vector3 backLeft,
			NxReal radius = 0.5f,
			ShapeParams sp = "",
			WheelParams wp = "use-params:normal"
		);

		static WheelSet createSixWheelSet(
			Actor*,
			Ogre::Vector3 frontLeft,
			Ogre::Vector3 middleLeft,
			Ogre::Vector3 backLeft,
			NxReal radius = 0.5f,
			ShapeParams sp = "",
			WheelParams wp = "use-params:normal"
		);

		WheelSet() {}
		~WheelSet() {}

		Wheel* attachNewWheel(NxReal radius, ShapeParams = "", WheelParams = "");
		void turn(Ogre::Radian angle);
		void setMotorTorque(NxReal torque);
		void setBrakeTorque(NxReal torque);
		void addMeshes(Ogre::String meshName, Ogre::Vector3 offset = Ogre::Vector3(0,0,0));	
		void attachDriveShaft(DriveShaft*);

		Wheel	*frontLeft,
				*frontRight,
				*backLeft,
				*backRight,
				*middleLeft,
				*middleRight;
	
		DriveShaft*	mDriveShaft;
	
		Wheels		mWheels;
		Wheels		Drive;
		Wheels		Steering;

		//////////////////////////////////////////////////////////////////////////////

	protected:

		Actor* mActor;

	};

#endif
};

#endif
