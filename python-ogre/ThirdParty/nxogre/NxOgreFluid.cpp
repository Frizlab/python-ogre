/** \file    NxOgreFluid.cpp
 *  \see     NxOgreFluid.h
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
#include "NxOgreFluid.h"

#if (NX_USE_FLUID_API == 1)
#include "NxOgrePose.h"					// For conversions
#include "NxOgreHelpers.h"				// For conversions
#include "NxOgreScene.h"				// For Actor::mOwner
#include "NxOgreFluidMesh.h"			// For Fluid Mesh

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////


void FluidParams::setToDefault() {
	maxParticles				= 32767;
	numReserveParticles			= 0;
	restParticlesPerMeter		= 50.0f;
	restDensity					= 1000.0f;
	kernelRadiusMultiplier		= 1.2f;
	motionLimitMultiplier		= 3.0f * kernelRadiusMultiplier;
	collisionDistanceMultiplier = 0.1f * kernelRadiusMultiplier;
	packetSizeMultiplier		= 16;
	stiffness					= 20.0f;
	viscosity					= 6.0f;
	damping						= 0.0f;
	fadeInTime					= 0.0f;
	externalAcceleration.zero();
	staticCollisionRestitution	= 0.5f;
	staticCollisionAdhesion		= 0.05f;
	staticCollisionAttraction	= 0.0f;
	dynamicCollisionRestitution	= 0.5f;
	dynamicCollisionAdhesion	= 0.5f;
	dynamicCollisionAttraction	= 0.0f;
	collisionResponseCoefficient = 0.2f;

	simulationMethod			= NX_F_SPH;
	collisionMethod				= NX_F_STATIC|NX_F_DYNAMIC;
	collisionGroup				= 0;	
	groupsMask.bits0 = 0;
	groupsMask.bits1 = 0;
	groupsMask.bits2 = 0;
	groupsMask.bits3 = 0;

	// NX_FF_VISUALIZATION|
	flags						= NX_FF_ENABLED|NX_FF_HARDWARE;
#if NX_SDK_VERSION_NUMBER >= 260
	compartment					= NULL;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////

void FluidParams::parse(Parameters params) {

	setToDefault();

	for (Parameter* parameter = params.Begin(); parameter = params.Next();) {

	}

}

//////////////////////////////////////////////////////////

Fluid::Fluid(Scene* scene, const Pose& pose, FluidParams fp) : mOwner(scene) {
	
	mName = NxCreateID(mOwner->getNbFluids(), "Fluid");
	mOwner->_registerFluid(mName, this);

	//Create fluid
	NxFluidDesc fluidDesc;
	fluidDesc.setToDefault();  
    fluidDesc.simulationMethod = NX_F_SPH;
	fluidDesc.staticCollisionRestitution = 0.1;
	fluidDesc.staticCollisionAdhesion = 0.5;
	fluidDesc.maxParticles = FC_MAX_PARTICLES;
	fluidDesc.restParticlesPerMeter = 50;
	fluidDesc.stiffness = 1;
	fluidDesc.viscosity = 6;
    
	mParticles = new NxVec3[fluidDesc.maxParticles];
	fluidDesc.particlesWriteData.bufferPos = &mParticles[0].x;
	fluidDesc.particlesWriteData.bufferPosByteStride = sizeof(NxVec3);
	fluidDesc.particlesWriteData.numParticlesPtr = &mNbParticles; 

	mFluid = mOwner->getNxScene()->createFluid(fluidDesc);

	NxFluidEmitterDesc emitterDesc;
    emitterDesc.setToDefault();
    emitterDesc.dimensionX = 1;
    emitterDesc.dimensionY = 1;
    emitterDesc.relPose.id();
    NxReal mat[] = {1,0,0,0, 0,0,1,0, 0,-1,0,0, 0,2,0,1};
    emitterDesc.relPose.setColumnMajor44(mat);
    emitterDesc.rate = 500.0;
    emitterDesc.randomAngle = 0.1f;
    emitterDesc.fluidVelocityMagnitude = 6.5f;
    emitterDesc.maxParticles = FC_MAX_PARTICLES;
    emitterDesc.particleLifetime = 100.0f;
    emitterDesc.type = NX_FE_CONSTANT_FLOW_RATE;
    emitterDesc.shape = NX_FE_ELLIPSE;
    mFluid->createEmitter(emitterDesc);


	
}

//////////////////////////////////////////////////////////

Fluid::~Fluid() {
	mOwner->_unregisterFluid(mName);
	
	if (mFluidMesh)
		delete mFluidMesh;

	// Destroy all emitters

	// Destroy all drains

	mOwner->getNxScene()->releaseFluid(*mFluid);
}

//////////////////////////////////////////////////////////

void Fluid::createFluidMesh(Ogre::Camera* camera) {
	mFluidMesh = new FluidMesh(this, mOwner, camera);
}

//////////////////////////////////////////////////////////

void Fluid::destroyFluidMesh() {
	delete mFluidMesh;
	mFluidMesh = 0;
}

//////////////////////////////////////////////////////////

}  //End of NxOgre namespace.
#endif
