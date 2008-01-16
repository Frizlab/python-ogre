//
//	NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org
//
//	This library is free software{ you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation{ either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY{ without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library{ if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "NxOgreStable.h"
#include "NxOgreTesting.h"
#include "NxOgreWorld.h"
#include "NxOgreScene.h"
#include "NxOgreContainer.h"
#include "NxOgreActor.h"
#include "NxOgreShape.h"
#include "NxOgreShapeBlueprintPrimitives.h"
#include "NxOgrePose.h"
#include "NxOgrePhysXDriver.h"

namespace NxOgre {

//////////////////////////////////////////////////////////////////////

Test::Test(World* w, Ogre::SceneManager* s) : mWorld(w), mSceneMgr(s) {
}

//////////////////////////////////////////////////////////////////////

Test::~Test() {

}

//////////////////////////////////////////////////////////////////////

void Test::addTestCases() {
	mTestCases.push_back(new TestCase_NxOgreCore());
	mTestCases.push_back(new TestCase_RigidActors());
	///		mTestCases.push_back(new TestCase_Cooking());
}

//////////////////////////////////////////////////////////////////////

void Test::addTestCase(TestCase* tc) {
	mTestCases.push_back(tc);
}

//////////////////////////////////////////////////////////////////////

void Test::startTesting() {
//	mStopTime = 0;
//	mTimer = new Ogre::Timer();
//	mTimer->reset();

	NxPhysicsSDK* sdk = NxGetPhysicsSDK();

	for (NxU32 i=0;i < mTestCases.size();i++) {
		TestResult result;
		mTestCases[i]->Start(mWorld, sdk, mSceneMgr);
		mTestCases[i]->Stop();

		result.mCaseResult = mTestCases[i]->getResult();
		result.mCaseID = mTestCases[i]->getID();
		result.mCaseComments = mTestCases[i]->getComments();
		
		mTestResults.push_back(result);
	}
}

//////////////////////////////////////////////////////////////////////

void Test::stopTesting() {

	//mStopTime = mTimer->getMicrosecondsCPU();
//	delete mTimer;

	for (NxU32 i=0;i < mTestCases.size();i++) {
		delete mTestCases[i];
	}

	mTestCases.clear();
}

//////////////////////////////////////////////////////////////////////

void Test::saveToLog() {

	std::stringstream ss;

	ss << "Test Results" << std::endl << std::endl;

	for (NxU32 i=0;i < mTestResults.size();i++) {
		ss << "TestCase ID: " << mTestResults[i].mCaseID << std::endl;
		ss << "\t Result: ";
		if (mTestResults[i].mCaseResult)
			ss << " Pass";
		else
			ss << " Failed";
		ss << std::endl << " \t Comments: " << mTestResults[i].mCaseComments << std::endl << std::endl;
	}

	//ss << "Completed Test in " << mStopTime << " microseconds." << std::endl;

	NxDebug(ss.str());

}

//////////////////////////////////////////////////////////////////////

void Test::save(NxString filename) {
	NxUnderConstruction;
}

//////////////////////////////////////////////////////////////////////

void Test::TestCase_NxOgreCore::Start(World* world, NxPhysicsSDK* sdk, Ogre::SceneManager* scenemgr) {
	
	mSDK = sdk;
	mWorld = world;
	mComments = "";
	mMaxScenes = 63;

	if (mSDK == 0) {
		mResult = false;
		mComments += "(NxCore:NoSDK) PhysicsSDK is unavailable.\n";
		return;
	}

	if (mWorld == 0) {
		mResult = false;
		mComments += "(NxCore:NoWorld) World Pointer is invalid or unavailable.\n";
		return;
	}

	mMaxScenes = 63 - mSDK->getNbScenes();
	NxU32 nbNxScenes = mSDK->getNbScenes();
	NxU32 nbScenes = mWorld->getScenes()->count();

	for (NxU32 i=0;i < mMaxScenes;i++) {
		
		NxU32 nbLastKnownNxScenes = mSDK->getNbScenes();
		NxU32 nbLastKnownScenes = mWorld->getScenes()->count();
		// sceneIdentifier
		//		Type:			NxString
		//		Pattern:		TestCase-Scene-<i>
		//		For:			Scene Identifier
		NxString sceneIdentifier = "TestCase-Scene-";
		sceneIdentifier.append(Ogre::StringConverter::toString(i));
		
		// sceneParams
		//		Type:			SceneParams
		//		Value:			Default as per setToDefault
		//		For:			Parameters of the Scene

		SceneParams sceneParams;
		sceneParams.setToDefault();
		
		Scene* scene = mWorld->createScene(sceneIdentifier, scenemgr, sceneParams);
		
		if (scene == 0) {
			mResult = false;
			mComments += "(NxCore:NullScene) Scene wasn't created. Pointer is NULL.\n";
			return;
		}

		mScenes.push_back(scene);
		nbNxScenes = mSDK->getNbScenes();
		nbScenes = mWorld->getScenes()->count();

		if (nbNxScenes != nbLastKnownNxScenes + 1) {
			mResult = false;
			mComments += "(NxCore:NoNxScene) Scene #" + Ogre::StringConverter::toString(i) + " wasn't created.\n";
			return;
		}

		if (nbScenes != nbLastKnownScenes + 1) {
			mResult = false;
			mComments += "(NxCore:SceneCountDiffer) Scene #" + Ogre::StringConverter::toString(i) + " wasn't created.\n";
			return;
		}

		if (nbScenes != nbNxScenes) {
			mResult = false;
			mComments += "(NxCore:SceneNENxScene) Scene #" + Ogre::StringConverter::toString(i) + " wasn't created. Universe will explode.\n";
			return;
		}

		if (scene->getName() != sceneIdentifier) {
			mResult = false;
			mComments += "(NxCore:IdentifierDiffer) Scene Identifier assigned is different '" + sceneIdentifier +"' vs. '" + scene->getName() + "'.\n";
			return;
		}

		
		mWorld->getPhysXDriver()->simulate(1.0f / 60.0f);
		mWorld->getPhysXDriver()->render(1.0f / 60.0f);
	}
		
	mResult = true;
	mComments += "Scene (" + Ogre::StringConverter::toString(mMaxScenes) + ") creation passed.\n";

}

void Test::TestCase_NxOgreCore::Stop() {

	if (mWorld == 0)
		return;

	for (NxU32 i=0;i < mScenes.size();i++) {
		if (mScenes[i] == 0) {
			mResult = false;
			mComments += "(NxCore:NullPointerClean) During cleanup Scene '" + Ogre::StringConverter::toString(i) + "' in mScenes has a NULL pointer.\n"; 
			continue;
		}

		mWorld->destroyScene(mScenes[i]->getName());


	}

}

//////////////////////////////////////////////////////////////////////

void Test::TestCase_RigidActors::Start(World* world, NxPhysicsSDK* sdk, Ogre::SceneManager* scenemgr) {
	
	mSDK = sdk;
	mWorld = world;
	mComments = "";
	mMaxScenes = 8;

	if (mSDK == 0) {
		mResult = false;
		mComments += "(NxRigid:NoSDK) PhysicsSDK is unavailable.\n";
		return;
	}

	if (mWorld == 0) {
		mResult = false;
		mComments += "(NxRigid:NoWorld) World Pointer is invalid or unavailable.\n";
		return;
	}

	mMaxScenes = 8 - mSDK->getNbScenes();
	NxU32 nbNxScenes = mSDK->getNbScenes();
	NxU32 nbScenes = mWorld->getScenes()->count();

	NxVec3* averageActorEight = new NxVec3[mMaxScenes + 1];

	for (NxU32 i=0;i < mMaxScenes;i++) {
		
		NxU32 nbLastKnownNxScenes = mSDK->getNbScenes();
		NxU32 nbLastKnownScenes = mWorld->getScenes()->count();
		// sceneIdentifier
		//		Type:			NxString
		//		Pattern:		TestCase-Scene-<i>
		//		For:			Scene Identifier
		NxString sceneIdentifier = "TestCase-Scene-";
		sceneIdentifier.append(Ogre::StringConverter::toString(i));
		
		// sceneParams
		//		Type:			SceneParams
		//		Value:			Default as per setToDefault
		//		For:			Parameters of the Scene

		SceneParams sceneParams;
		sceneParams.setToDefault();
		
		Scene* scene = mWorld->createScene(sceneIdentifier, scenemgr, sceneParams);
		
		if (scene == 0) {
			mResult = false;
			mComments += "(NxRigid:NullScene) Scene wasn't created. Pointer is NULL.\n";
			return;
		}

		mScenes.push_back(scene);
		nbNxScenes = mSDK->getNbScenes();
		nbScenes = mWorld->getScenes()->count();

		if (nbNxScenes != nbLastKnownNxScenes + 1) {
			mResult = false;
			mComments += "(NxRigid:NoNxScene) Scene #" + Ogre::StringConverter::toString(i) + " wasn't created.\n";
			return;
		}

		if (nbScenes != nbLastKnownScenes + 1) {
			mResult = false;
			mComments += "(NxRigid:SceneCountDiffer) Scene #" + Ogre::StringConverter::toString(i) + " wasn't created.\n";
			return;
		}

		if (nbScenes != nbNxScenes) {
			mResult = false;
			mComments += "(NxRigid:SceneNENxScene) Scene #" + Ogre::StringConverter::toString(i) + " wasn't created. Universe will explode.\n";
			return;
		}

		if (scene->getName() != sceneIdentifier) {
			mResult = false;
			mComments += "(NxRigid:IdentifierDiffer) Scene Identifier assigned is different '" + sceneIdentifier +"' vs. '" + scene->getName() + "'.\n";
			return;
		}

		Actor* actor8;
		

		for (NxU32 j=0;j < mMaxActors;j++) {

			NxU32 nxactorcount = scene->getNxScene()->getNbActors();

			Actor* actor;
			
			NxString actorIdentifier = "TestCase-Actor-";
			actorIdentifier.append(Ogre::StringConverter::toString(j));

			ActorParams actorParams;
			actorParams.setToDefault();
			actorParams.mMass = 10.0f;

			NxOgre::Pose actorPose(NxVec3(0, 0.5f + i, 0));
			actor = scene->createActor(sceneIdentifier, new CubeShape(1), actorPose, actorParams);

			if (actor == 0) {
				mResult = false;
				mComments += "(NxRigid:NullActor) Scene wasn't created. Pointer is NULL.\n";
				delete [] averageActorEight;
				return;
			}

			if (actor->isDead()) {
				mResult = false;
				mComments += "(NxRigid:DeadActor) NxActor in Actor has a NULL pointer.\n";
				delete [] averageActorEight;
				return;
			}

			if (scene->getNxScene()->getNbActors() != nxactorcount + 1) {
				mResult = false;
				mComments += "(NxRigid:ActorCount) Actor count is inconsistant.\n";
				delete [] averageActorEight;
				return;
			}


			if (j == 8)
				actor8 = actor;

			
		}

		// Simulate/Render for 1 second.
		for (int z=0;z < 60.0f;z++) {
			mWorld->getPhysXDriver()->simulate(1.0f / 60.0f);
			mWorld->getPhysXDriver()->render(1.0f / 60.0f);
		}

		// Pick out Actor eight, add it average.
		averageActorEight[i] = actor8->getNxActor()->getGlobalPosition();

	}
		
	NxReal mAvg = 0, mMax = 0, mMin = averageActorEight[0].magnitude();
	
	for (NxU32 i=0;i < mMaxScenes;i++) {
		mAvg += averageActorEight[i].magnitude();
		
		if (averageActorEight[i].magnitude() > mMax)
			mMax = averageActorEight[i].magnitude();

		if (averageActorEight[i].magnitude() < mMin)
			mMin = averageActorEight[i].magnitude();
	}

	mAvg /= mMaxScenes;

	NxReal mDif = NxMath::abs(mMax - mMin);

	std::stringstream ss;
	ss << "Av:" << mAvg << ", Mx:" << mMax << ", Mn:" << mMin << ", Diff: " << mDif << std::endl;

	delete [] averageActorEight;

	mComments += ss.str();

	mResult = true;
	mComments += "Scene(" + Ogre::StringConverter::toString(mMaxScenes) + ") and Actors(" + Ogre::StringConverter::toString(mMaxActors) + ") creation passed.\n";



}

//////////////////////////////////////////////////////////////////////

void Test::TestCase_RigidActors::Stop() {

	if (mWorld == 0)
		return;

	

	for (NxU32 i=0;i < mScenes.size();i++) {
		
		if (mScenes[i] == 0) {
			mResult = false;
			mComments += "(NxCore:NullPointerClean) During cleanup Scene '" + Ogre::StringConverter::toString(i) + "' in mScenes has a NULL pointer.\n"; 
			continue;
		}

	
		mWorld->destroyScene(mScenes[i]->getName());


	}

}

//////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
