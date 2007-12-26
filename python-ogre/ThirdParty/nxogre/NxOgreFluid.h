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

#ifndef __NXOGRE_FLUID_H__
#define __NXOGRE_FLUID_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreContainer.h"		// For: mCollisionList
#include "NxOgreParams.h"

#if (NX_USE_FLUID_API == 1)

namespace NxOgre {

	///	Params
	///		->	Kinematic: <bool>

	class NxExport FluidParams : Params {

		public:


			FluidParams() {}
			FluidParams(const char* p) {process(p);}
			FluidParams(NxString p) {process(p);}

			void			setToDefault();
			void			parse(Parameters);			
			
			NxU32						maxParticles;
			NxU32						numReserveParticles;
			NxReal						restParticlesPerMeter;
			NxReal						restDensity;
			NxReal						kernelRadiusMultiplier;
			NxReal						motionLimitMultiplier;
			NxReal						collisionDistanceMultiplier;
			NxU32						packetSizeMultiplier;
			NxReal						stiffness;
			NxReal						viscosity;
			NxReal						damping;
			NxReal						fadeInTime;
			NxVec3						externalAcceleration;
			NxReal						staticCollisionRestitution;
			NxReal						staticCollisionAdhesion;
			NxReal						staticCollisionAttraction;
			NxReal						dynamicCollisionRestitution;
			NxReal						dynamicCollisionAdhesion;
			NxReal						dynamicCollisionAttraction;
			NxReal						collisionResponseCoefficient;
			NxU32						simulationMethod;
			NxU32						collisionMethod;
			NxCollisionGroup			collisionGroup;
			NxGroupsMask				groupsMask;
			NxU32						flags;

#if NX_SDK_VERSION_NUMBER >= 260
			NxCompartment*				compartment;
#endif


	}; // End of ActorParams class

	class NxExport Fluid {

		public:

			Fluid(Scene*, const Pose&, FluidParams = "");
			virtual ~Fluid();

			//////////////////////////////////////////////////////////

			Scene*		getScene() {return mOwner;}
			NxScene*	getNxScene();
			NxFluid*	getNxFluid() {return mFluid;}
			FluidMesh*	getFluidMesh() {return mFluidMesh;}
			NxU32		getNbParticles() {return mNbParticles;}
			void		createFluidMesh(Ogre::Camera*);
			void		destroyFluidMesh();

			//////////////////////////////////////////////////////////

			void						setName(NxString);
			NxString					getName() {return mName;}


			FluidEmitters				mEmitters;
			FluidDrains					mDrains;

		protected:

			Scene					   *mOwner;
			NxActor*					mActor;
			NxString					mName;

			enum FLUID_CONSTANTS {
				FC_MAX_PARTICLES = 10000
			};

			NxVec3*						mParticles;
			NxU32						mNbParticles;
			NxFluid*					mFluid;
			FluidMesh*					mFluidMesh;
			

		private:

	};// End of Actor Class

};// End of namespace
#endif
#endif
