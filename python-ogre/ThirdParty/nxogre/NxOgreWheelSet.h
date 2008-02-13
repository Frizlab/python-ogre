/** \file    NxOgreWheelSet.h
 *  \brief   Header for the WheelSet class.
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

#if 0

#ifndef __NXOGRE_WHEELSET_H__
#define __NXOGRE_WHEELSET_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreMachine.h"
#include "NxOgreContainer.h"
#include "NxOgreShape.h"
#include "NxOgreShapeBlueprintWheel.h"
#include "NxOgreNodeRenderable.h"

namespace NxOgre {
	
	//////////////////////////////////////////////////////////////////////////////////////
	
	class NxPublicClass WheelSet : public Machine {
		
		public:
			
			NxString getIdentifierType() {return "NxOgre-WheelSet";}
			NxShortHashIdentifier getIdentifierTypeHash() const {return 24166;}
			
			enum WheelIdentifier {
				FRONT			= 0,
				FRONT_LEFT		= 1,
				FRONT_RIGHT		= 2,
				BACK			= 3,
				BACK_LEFT		= 4,
				BACK_RIGHT		= 5,
				MIDDLE_LEFT		= 6,
				MIDDLE_RIGHT	= 7
			};
			
			static WheelSet* createFourWheelSet(
				Actor*,
				const Ogre::Vector3& frontLeft,
				const Ogre::Vector3& backLeft,
				NxReal wheelRadius = 0.5f,
				ShapeParams shape_params = ShapeParams(),
				WheelParams wheel_params = "use-params:normal",
				NodeRenderableParams left_wheels = NodeRenderableParams(),
				NodeRenderableParams right_wheels = NodeRenderableParams(),
				NxDirection forwardDirection = NX_DIRECTION_X
			);
			
			void simulate(NxReal deltaTime);
			
			void turn(NxRadian);
			void turn(const Ogre::Radian&);
			
			void setMotorTorque(NxReal motorTorqueValue);
			void setBrakeTorque(NxReal brakeTorqueValue);
			
			Wheels				mWheels;
			Wheels				mDrive;
			Wheels				mSteering;
			CombustionEngine*	mEngine;
			
		protected:
			
			WheelSet(Actor*);
			~WheelSet();

			Actor* mActor;
		
	};
	
	
};
#endif
#endif