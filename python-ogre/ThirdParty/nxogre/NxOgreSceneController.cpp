/** \file    NxOgreSceneController.cpp
 *  \see     NxOgreSceneController.h
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
#include "NxOgreSceneController.h"
#include "NxOgreScene.h"

#include "OgreTimer.h"

#include "Nx.h"

#ifdef WIN32 
	#define NOMINMAX
	#include <windows.h>
#endif

namespace NxOgre {

///////////////////////////////////////////////////////////////////////

SceneController::SceneController(Scene* scene) : mScene(scene) {
	mRenderTime = 0;
	mIPS = 0.0f;
	mDeltaTime = 0;
}


///////////////////////////////////////////////////////////////////////

SceneController::~SceneController() {

}
///////////////////////////////////////////////////////////////////////

void SceneController::setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {

}

///////////////////////////////////////////////////////////////////////

void SceneController::setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {

}

///////////////////////////////////////////////////////////////////////

void SceneController::init(NxScene* nxscene) {
	NxDebug("SceneController Initalised");
}

///////////////////////////////////////////////////////////////////////

bool SceneController::Simulate(NxReal deltaTime) {
	mDeltaTime = deltaTime;
	mNxScene->simulate(deltaTime);
	mNxScene->flushStream();
	while (!mNxScene->fetchResults(NX_RIGID_BODY_FINISHED, false));
	return true;
}

///////////////////////////////////////////////////////////////////////

NullSceneController::NullSceneController(Scene* scene) : SceneController(scene) {
	
}


///////////////////////////////////////////////////////////////////////

NullSceneController::~NullSceneController() {

}

///////////////////////////////////////////////////////////////////////

void NullSceneController::init(NxScene* nxscene) {
	mNxScene = nxscene;
}
///////////////////////////////////////////////////////////////////////

void NullSceneController::setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {

}

///////////////////////////////////////////////////////////////////////

void NullSceneController::setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {
	desc.maxIter = 8;
	desc.maxTimestep = 1.0f/60.0f;
}

///////////////////////////////////////////////////////////////////////

bool NullSceneController::Simulate(NxReal deltaTime) {
	mDeltaTime = deltaTime;
	return true;
}

///////////////////////////////////////////////////////////////////////










///////////////////////////////////////////////////////////////////////

VariableSceneController::VariableSceneController(Scene* scene) : SceneController(scene) {
	
}


///////////////////////////////////////////////////////////////////////

VariableSceneController::~VariableSceneController() {

}

///////////////////////////////////////////////////////////////////////

void VariableSceneController::init(NxScene* nxscene) {
	NxDebug("VariableSceneController Initalised");
	mNxScene = nxscene;
}
///////////////////////////////////////////////////////////////////////

void VariableSceneController::setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {

}

///////////////////////////////////////////////////////////////////////

void VariableSceneController::setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {
	desc.maxIter = matIter;
	desc.timeStepMethod = NX_TIMESTEP_VARIABLE;
}

///////////////////////////////////////////////////////////////////////

bool VariableSceneController::Simulate(NxReal deltaTime) {
	mDeltaTime = deltaTime;
	mNxScene->simulate(deltaTime);
	mNxScene->flushStream();
	while (!mNxScene->fetchResults(NX_RIGID_BODY_FINISHED, false));
	mRenderTime = deltaTime;
	return true;
}

///////////////////////////////////////////////////////////////////////

FixedSceneController::FixedSceneController(Scene* scene) : SceneController(scene) {
}


///////////////////////////////////////////////////////////////////////

FixedSceneController::~FixedSceneController() {

}

///////////////////////////////////////////////////////////////////////

void FixedSceneController::init(NxScene* nxscene) {

#if (NX_DEBUG == 1)
	NxDebug("FixedSceneController Initalised");
#endif
	mNxScene = nxscene;

}

///////////////////////////////////////////////////////////////////////

void FixedSceneController::setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {
	
	mTiming_MaxStep = maxTimestep;
	mTiming_CurrentStep = 0.0f;
	mRenderTime = 0;
	mRenderFrame = false;
}

///////////////////////////////////////////////////////////////////////

void FixedSceneController::setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {
	
	mTiming_MaxStep = maxTimestep;
	mTiming_CurrentStep = 0.0f;

	desc.maxTimestep = mTiming_MaxStep;
	desc.maxIter = matIter;
	desc.timeStepMethod = NX_TIMESTEP_FIXED;
	mRenderTime = 0;
	mRenderFrame = false;
}

///////////////////////////////////////////////////////////////////////

bool FixedSceneController::Simulate(NxReal deltaTime) {
	mDeltaTime = deltaTime;
	mTiming_CurrentStep += deltaTime;

	if (mTiming_CurrentStep < mTiming_MaxStep) {
		mRenderFrame = false;
		return false;
	}

	mNxScene->simulate(mTiming_MaxStep - (mTiming_MaxStep - mTiming_CurrentStep));
	mNxScene->flushStream();

	while(!mNxScene->checkResults(NX_RIGID_BODY_FINISHED, false)) {}

	mNxScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
	
	mRenderFrame = true;
	mRenderTime = deltaTime;		
	mTiming_CurrentStep=0;

	return true;
}

///////////////////////////////////////////////////////////////////////

AccumulatorSceneController::AccumulatorSceneController(Scene* scene) : SceneController(scene) {
	
}

///////////////////////////////////////////////////////////////////////

AccumulatorSceneController::~AccumulatorSceneController() {

}

///////////////////////////////////////////////////////////////////////

void AccumulatorSceneController::init(NxScene* nxscene) {

#if (NX_DEBUG == 1)
	NxDebug("AccumulatorSceneController Initalised");
#endif
	mNxScene = nxscene;
	mSecond = 0.0f;
	mSimCount = 0;
	
}

///////////////////////////////////////////////////////////////////////


void AccumulatorSceneController::setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {
	
	mDt = maxTimestep;
	mSecond = 0.0f;
	mRenderTime = 0;
	mSimCount = 0;
	mIPS = 0;
	mAccumulator = 0;

}

///////////////////////////////////////////////////////////////////////

void AccumulatorSceneController::setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {
	

	desc.maxTimestep = maxTimestep;
	desc.maxIter = matIter;
	desc.timeStepMethod = NX_TIMESTEP_FIXED;
	
	mDt = maxTimestep;
	mSecond = 0.0f;
	mRenderTime = 0;
	mSimCount = 0;
	mIPS = 0;
	mAccumulator = 0;
	mPreviousTime = getTime();
}

///////////////////////////////////////////////////////////////////////
#ifdef WIN32

unsigned long AccumulatorSceneController::getTime() {
	QueryPerformanceFrequency(&mFreq);
	unsigned long long ticksPerMillisecond = mFreq.QuadPart / 1000;

	QueryPerformanceCounter(&mCounter);
	return (unsigned long)(mCounter.QuadPart/ticksPerMillisecond);
}

#else

unsigned long AccumulatorSceneController::getTime() {
	// Some fancy code for Linux goes in here.
	return 0;
}

#endif
///////////////////////////////////////////////////////////////////////

bool AccumulatorSceneController::Simulate(NxReal deltaTime) {
	
	unsigned long currentTime = getTime();
	unsigned long elapsedTime = currentTime - mPreviousTime;
	mPreviousTime = currentTime;
	mDeltaTime = (float)(elapsedTime)*0.001f;

	mAccumulator += mDeltaTime;

	while (mAccumulator>=mDt) {
		mNxScene->simulate(mDt);
		mNxScene->flushStream();
		mSecond += mDt;
		mSimCount++;
		//while(!mNxScene->checkResults(NX_RIGID_BODY_FINISHED, false)) {}
		mNxScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
		mAccumulator -= mDt;
	}

	if (mSecond >= 1.0f) {
		mIPS =  mDt * mSimCount;
		mSecond = 0.0f;
		mSimCount = 0;
	}

	mAlpha = mAccumulator / mDt;

	return true;

}

///////////////////////////////////////////////////////////////////////


}; //End of NxOgre namespace.
