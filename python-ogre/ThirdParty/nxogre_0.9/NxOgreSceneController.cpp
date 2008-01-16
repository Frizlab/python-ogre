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
#include "NxOgreSceneController.h"
#include "NxOgreScene.h"

namespace NxOgre {

///////////////////////////////////////////////////////////////////////

SceneController::SceneController(Scene* scene) : mScene(scene) {

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
	mNxScene->simulate(deltaTime);
	mNxScene->flushStream();
	while (!mNxScene->fetchResults(NX_RIGID_BODY_FINISHED, false));
	return true;
}

///////////////////////////////////////////////////////////////////////

bool SceneController::Render(NxReal deltaTime) {
	return true;
}

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
	mNxScene->simulate(deltaTime);
	mNxScene->flushStream();
	while (!mNxScene->fetchResults(NX_RIGID_BODY_FINISHED, false));
	return true;
}

///////////////////////////////////////////////////////////////////////

bool VariableSceneController::Render(NxReal deltaTime) {
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
#ifdef NX_DEBUG
	NxDebug("FixedSceneController Initalised");
#endif
	mNxScene = nxscene;

}

///////////////////////////////////////////////////////////////////////

void FixedSceneController::setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {
	
	mTiming_MaxStep = maxTimestep;
	mTiming_CurrentStep = 0.0f;

	mRenderFrame = false;
}

///////////////////////////////////////////////////////////////////////

void FixedSceneController::setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {
	
	mTiming_MaxStep = maxTimestep;
	mTiming_CurrentStep = 0.0f;

	desc.maxTimestep = mTiming_MaxStep;
	desc.maxIter = matIter;
	desc.timeStepMethod = NX_TIMESTEP_FIXED;

	mRenderFrame = false;
}

///////////////////////////////////////////////////////////////////////

bool FixedSceneController::Simulate(NxReal deltaTime) {
	
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
	mTiming_CurrentStep=0;

	return true;
}

///////////////////////////////////////////////////////////////////////

bool FixedSceneController::Render(NxReal deltaTime) {
	return mRenderFrame;
}

///////////////////////////////////////////////////////////////////////
#if (NX_UNSTABLE == 1)
SuperFixedSceneController::SuperFixedSceneController(Scene* scene) : SceneController(scene) {
	
}

///////////////////////////////////////////////////////////////////////

SuperFixedSceneController::~SuperFixedSceneController() {
	delete mTimer;
}

///////////////////////////////////////////////////////////////////////

void SuperFixedSceneController::init(NxScene* nxscene) {
	NxDebug("SuperFixedSceneController Initalised");
	mNxScene = nxscene;
	
#ifdef NX_DEBUG
	std::stringstream ss;
	ss	<< "SuperFixedSceneController Init" << std::endl
		<< "	-> MaxStep				= " << mTiming_MaxStep << std::endl;
	NxDebug(ss.str());
#endif
/*
	mNxScene->simulate(mTiming_CurrentStep);
	mNxScene->flushStream();
	mNxScene->fetchResults(NX_RIGID_BODY_FINISHED, true);*/

	mTiming_CurrentStep = 0;
	mFrameCount = 0;
	mMargin = 0;

}

///////////////////////////////////////////////////////////////////////


void SuperFixedSceneController::setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {
	
	mTimer = new Ogre::Timer();
	mTimer2 = new Ogre::Timer();
	mTiming_MaxStep = maxTimestep;
	mTiming_CurrentStep = 0.0f;
	mMargin = 0;
	mNbSimulations =0;
	mNbSimulationsLast = 1;
	mNbSimulationsFramesLast = 1;
	mTiming_Multiplier = 1;

	mRenderFrame = false;
}

///////////////////////////////////////////////////////////////////////

void SuperFixedSceneController::setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {
	
	mTimer = new Ogre::Timer();
	mTimer2 = new Ogre::Timer();

	mTiming_MaxStep = maxTimestep;
	mTiming_CurrentStep = 0.0f;
	mMargin = 0;
	mNbSimulations =0;
	mNbSimulationsLast = 1;
	mNbSimulationsFramesLast = 1;
	desc.maxTimestep = mTiming_MaxStep;
	desc.maxIter = matIter;
	desc.timeStepMethod = NX_TIMESTEP_FIXED;
	mTiming_Multiplier = 1;
}

///////////////////////////////////////////////////////////////////////

bool SuperFixedSceneController::Simulate(NxReal deltaTime) {
	
	mFrameCount++;
	double dt = (mTimer->getMicrosecondsCPU()) * 0.000001;		// Convert to seconds.

	mTiming_CurrentStep += dt;


	if (mTiming_CurrentStep < mTiming_MaxStep * mTiming_Multiplier ) {
		mRenderFrame = false;
		return false;
	}

	if (mTimer2->getMilliseconds() >= 1000) {
		mTimer2->reset();
		mNbSimulationsLast = mNbSimulations;
		mNbSimulationsFramesLast = mFrameCount;
	}

	mNxScene->simulate(1.0f / 60.0f);
	mNxScene->flushStream();
	mNxScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
	mTimer->reset();
	mNbSimulations++;
	mMargin += dt;
	mTiming_CurrentStep = 0;
	
	if (mNbSimulations > 60)
		mTiming_Multiplier = (mMargin / mFrameCount) / (1.0f / 60.f);

	mRenderFrame = true;
	return true;
}

///////////////////////////////////////////////////////////////////////

bool SuperFixedSceneController::Render(NxReal deltaTime) {
	return mRenderFrame;
}

///////////////////////////////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////

ASyncSceneController::ASyncSceneController(Scene* scene) : SceneController(scene) {
	mTimer = new Ogre::Timer();
}


///////////////////////////////////////////////////////////////////////

ASyncSceneController::~ASyncSceneController() {
	delete mTimer;
}

///////////////////////////////////////////////////////////////////////

void ASyncSceneController::init(NxScene* nxscene) {
#ifdef NX_DEBUG
	NxDebug("ASyncSceneController Initalised");
#endif
	mNxScene = nxscene;
	mTimer->reset();
}

///////////////////////////////////////////////////////////////////////

void ASyncSceneController::setTiming(NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {
	
	mTiming_MaxStep = maxTimestep;
	mTiming_CurrentStep = 0.0f;

	mRenderFrame = false;
}

///////////////////////////////////////////////////////////////////////

void ASyncSceneController::setTiming(NxSceneDesc &desc, NxReal maxTimestep, NxU32 matIter, NxU32 numSubSteps) {
	
	mTiming_MaxStep = maxTimestep;
	mTiming_CurrentStep = 0.0f;

	desc.maxTimestep = mTiming_MaxStep;
	desc.maxIter = matIter;
	desc.timeStepMethod = NX_TIMESTEP_FIXED;

	mRenderFrame = false;
	mRenderNow = false;
	mUnfinished = false;
}

///////////////////////////////////////////////////////////////////////

bool ASyncSceneController::Simulate(NxReal) {
	
	mTiming_CurrentStep += (mTimer->getMicrosecondsCPU()) * 0.000001f;
	mTimer->reset();

	if (mUnfinished) {
		if (mNxScene->checkResults(NX_RIGID_BODY_FINISHED, false)) {
			mNxScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
			mRenderNow = true;
			std::cout << "+" << std::endl;
			return true;
		}
		else {
			std::cout << "-" << std::endl;
			return false;
		}
	}

	if (mTiming_CurrentStep < mTiming_MaxStep) {
		mRenderFrame = false;
		return false;
	}

	mNxScene->simulate(mTiming_MaxStep);
	mNxScene->flushStream();

	mRenderFrame = true;
	mTiming_CurrentStep-=mTiming_MaxStep;

	return true;
}

///////////////////////////////////////////////////////////////////////

bool ASyncSceneController::Render(NxReal deltaTime) {
	
	if (mRenderNow)
		return true;

	if (!mRenderFrame)
		return false;

	if (!mNxScene->checkResults(NX_RIGID_BODY_FINISHED, false)) {
		mUnfinished = true;
		return false;
	}

	mNxScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
	
	return true;
}


#endif

}; //End of NxOgre namespace.
