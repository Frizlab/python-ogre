/** \file    NxOgreScene.cpp
 *  \see     NxOgreScene.h
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
#include "NxOgreScene.h"
///////////#include "NxOgreSceneBlueprint.h"
#include "NxOgreSceneController.h"				// For: Controlling the TimeStep of the Scene
#include "NxOgreSceneRenderer.h"
#include "NxOgreSceneContactController.h"		// For: SceneContactController
#include "NxOgreSceneTriggerController.h"		// For: SceneTriggerController
#include "NxOgreContainer.h"					// For: Scene::mActors
#include "NxOgreActor.h"						// For: Actor operations
////////////#include "NxOgreActorBlueprint.h"				// For: Saving Actors
#include "NxOgreBody.h"							// For: Body operations
#include "NxOgreTrigger.h"						// For: Trigger operations
#include "NxOgreForceField.h"					// For: ForceField operations
#include "NxOgreWorld.h"						// For: Scene::mOwner
#include "NxOgreHelpers.h"						// For: simple conversions
#include "NxOgreShapePrimitives.h"				// For: hasFloor and Floor params (Ground)
#include "NxOgrePose.h"							// For: hasFloor (Ground)
#include "NxOgreMaterial.h"						// For: Material operations
#include "NxOgreJoint.h"						// For: Creation of abstract joints and releasing them.
#include "NxOgreJointSet1.h"					// For: Creation of Joints
#include "NxOgreJointSet2.h"					// For: Creation of Joints

#if (NX_USE_LEGACY_NXCONTROLLER == 1)
#	include "NxOgreLegacyCharacterController.h"			// For: Creation of Characters
#	include "NxOgreLegacyCharacter.h"					// For: Character Simulation and Render
#else
#	include "NxOgreCharacterSystem.h"
#	include "NxOgreCharacter.h"							// For: Character Simulation and Render
#endif

//#include "NxOgreHelper.h"						// For: Helpers
#include "NxOgreCloth.h"						// For: Cloths
#include "NxOgreFluid.h"						// For: Fluids
#include "NxOgreSoftBody.h"						// For: SoftBodies
#include "NxOgreGroup.h"						// For: ActorGroups and ShapeGroups
#include "NxOgreDominanceGroup.h"				// For: Dominance Groups
#include "NxOgreIntersection.h"					// For: Intersections (mSceneIntersection)
#include "NxOgreRaycaster.h"					// For: Raycasting (mSceneRayCaster)
#include "NxOgreSimpleShape.h"					// For: Intersections (SimpleBox in getActorsFromLastRegion)
#include "NxOgreMachine.h"						// For: Machines
#include "NxOgreUserData.h"						// For: UserData in NxTransformCode
#include "NxOgreSleepCallback.h"				// For: delete mSleepCallback
#include "NxOgreEffectsSystem.h"				// For: delete an EffectsSystem.

#include "NxOgreNodeRenderable.h"

#include "NxOgreOgreSceneRenderer.h"
#include "OgreStringConverter.h"
#include "OgreSceneManager.h"

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

void SceneParams::setToDefault() {

	gravity.zero();
	floor								= false;
	defaultMaterialRestitution			= 0.0f;
	defaultMaterialStaticFriction		= 0.5f;
	defaultMaterialDynamicFriction		= 0.5f;
	max_timestep						= 1.0f / 60.0f;
	max_iter							= 8;
	controller							= CN_FIXED;	// fixed, variable, accumulator, async
	renderer							= RN_OGRE;
	max_bounds							= NULL;
	limits								= NULL;
	sim_type							= NX_SIMULATION_SW;
	bounds_planes						= false;
	flags								= NX_SF_SIMULATE_SEPARATE_THREAD | NX_SF_DISABLE_SCENE_MUTEX;
	internal_thread_count				= 0;
	background_thread_count				= 0;
	custom_scheduler					= NULL;
	thread_mask							= 0x55555554;
	background_thread_mask				= 0x55555554;
	sim_thread_stack_size				= 0;
	sim_thread_priority					= NX_TP_NORMAL;
	worker_thread_stack_size			= 0;
	worker_thread_priority				= NX_TP_NORMAL;
	up_axis								= 0;
	
	subdivision_level					= 5;
	static_structure					= NX_PRUNING_DYNAMIC_AABB_TREE;
	dynamic_structure					= NX_PRUNING_NONE;


}

////////////////////////////////////////////////////////////////////////////////////////////////

void SceneParams::parse(Parameters params) {

	for (Parameter* parameter = params.Begin(); parameter = params.Next();) {

		if (Set("floor", parameter, floor)) continue;
		if (Set("default-material-restitution", parameter, defaultMaterialRestitution)) continue;
		if (Set("default-material-dynamic-friction", parameter, defaultMaterialDynamicFriction)) continue;
		if (Set("default-material-static-friction", parameter, defaultMaterialStaticFriction)) continue;
		if (Set("max-timestep", parameter, max_timestep)) continue;


		if (parameter->i == "gravity") {

			// TODO: Use Native String functions
			if (parameter->j.substr(0,1) == "y" || parameter->j.substr(0,1) == "Y") {
				gravity.set(0,-9.80665,0);
			}
			else {
				gravity.set(NxConvert<NxVec3, Ogre::Vector3>(Ogre::StringConverter::parseVector3(parameter->j)));
				continue;
			}
		}

		if (parameter->i == "controller") {
			
			if(parameter->j.substr(0,1) == "f" || parameter->j.substr(0,1) == "F") {
				controller = CN_FIXED;
			}
			else if (parameter->j.substr(0,1) == "v" || parameter->j.substr(0,1) == "V") {
				controller = CN_VARIABLE;
			}
#if (NX_UNSTABLE == 1)
			else if(parameter->j.substr(0,1) == "a" || parameter->j.substr(0,1) == "A") {
				controller = CN_ACCUMULATOR;
			}
			else if(parameter->j.substr(0,1) == "n" || parameter->j.substr(0,1) == "N") {
				controller = CN_NULL;
			}
#endif
			else {
				controller = CN_FIXED;
			}

		}


		if (parameter->i == "renderer") {
			
			if (parameter->j.substr(0,1) == "o" || parameter->j.substr(0,1) == "O") {
				renderer = RN_OGRE;
			}
			else {
				renderer = RN_NULL;
			}

		}
	}

}

///////////////////////////////////////////////////////////////////////

void Scene::_paramsToDescription(SceneParams sp) {
	
	mDescription.workerThreadStackSize = sp.worker_thread_stack_size;
	mDescription.workerThreadPriority = sp.worker_thread_priority;
	mDescription.upAxis = sp.up_axis;
	mDescription.threadMask = sp.thread_mask;
	mDescription.subdivisionLevel = sp.subdivision_level;
	mDescription.staticStructure = sp.static_structure;
	mDescription.simType = sp.sim_type;
	mDescription.simThreadStackSize = sp.sim_thread_stack_size;
	mDescription.simThreadPriority = sp.sim_thread_priority;

	mDescription.maxBounds = sp.max_bounds;
	mDescription.limits = sp.limits;
	mDescription.internalThreadCount = sp.internal_thread_count;
	mDescription.flags = sp.flags;
	mDescription.dynamicStructure = sp.dynamic_structure;
	mDescription.customScheduler = sp.custom_scheduler;
	mDescription.boundsPlanes = sp.bounds_planes;
	mDescription.backgroundThreadCount = sp.background_thread_count;
	mDescription.backgroundThreadMask= sp.background_thread_mask;
	mDescription.gravity = sp.gravity;
}

///////////////////////////////////////////////////////////////////////

Scene::Scene(const NxString& name, World* world, SceneParams p) :
mName(name),
mNxID(mName.c_str()),
mOwner(world),
mStaticGeometry(0),
mBatchProcessSimulate(false),
mSceneRayCaster(0),
mSceneIntersection(0),
mSleepCallback(0),
mSleepCallbackPolicy(GC_Never_Delete)
{
	mOwner->_registerScene(mName, this);

	mDescription.setToDefault();
	_paramsToDescription(p);

#if (NX_UNSTABLE_USE_SCENE_ACTIVE_TRANSFORM == 1)
	mDescription.flags |= NX_SF_ENABLE_ACTIVETRANSFORMS;
#endif

	if (p.controller == p.CN_FIXED) {
		mSceneController = NxNew(FixedSceneController) FixedSceneController(this);
	}
	else if (p.controller == p.CN_VARIABLE) {
		mSceneController = NxNew(VariableSceneController) VariableSceneController(this);
	}
	else if (p.controller == p.CN_ACCUMULATOR) {
		mSceneController = NxNew(AccumulatorSceneController) AccumulatorSceneController(this);
	}
	else if (p.controller == p.CN_NULL) {
		mSceneController = NxNew(NullSceneController) NullSceneController(this);
	}
	else {
		mSceneController = NxNew(FixedSceneController) FixedSceneController(this);
	}

	mSceneController->setTiming(mDescription, p.max_timestep, p.max_iter,0);

	mSceneContactController = NxNew(SceneContactController) SceneContactController(this);
	mSceneTriggerController = NxNew(SceneTriggerController) SceneTriggerController(this);

	mDescription.userContactReport = mSceneContactController;
	mDescription.userTriggerReport = mSceneTriggerController;
	mDescription.userNotify        = this;

	mScene = mOwner->getSDK()->createScene(mDescription);

	if (!mScene) {
		NxThrow(NxString("Scene creation of '" + name + "' failed.").c_str());
		return;
	}

#if (NX_DEBUG == 1)
	NxDebug(NxString("Created Scene '" + name + "'.").c_str());
#endif
	
	mScene->userData = (void*) this;

	if (p.renderer == p.RN_OGRE) {

		mSceneRenderer = NxNew(OgreSceneRenderer) OgreSceneRenderer(this, p.rendererUserData);

	}
	else {
		mSceneRenderer = NxNew(NullSceneRenderer) NullSceneRenderer(this, p.rendererUserData);
	}

	_createDefaultActorGroup();
	_createDefaultShapeGroup();
	_createDefaultDominanceGroup();
	_createDefaultMaterial(p.defaultMaterialRestitution,p.defaultMaterialDynamicFriction,p.defaultMaterialStaticFriction);

	
	// SceneParams -> Floor: Yes
	if (p.floor) {
		addFloor();
	}
	

	mSceneController->init(mScene);

	
	NxLabelContainer(Scene, mActors, Actors);
	NxLabelContainer(Scene, mActorGroups, ActorGroups);
	NxLabelContainer(Scene, mActorGroupsByIndex, ActorGroupsByIndex);
	NxLabelContainer(Scene, mJoints, Joints);
	NxLabelContainer(Scene, mTriggers, Triggers);


#if (NX_USE_CLOTH_API == 1)
	NxLabelContainer(Scene, mCloths, Cloths);
#endif

#if (NX_USE_CHARACTER_API == 1)
	NxLabelContainer(Scene, mCharacters, Characters);
#endif

#if (NX_USE_FLUID_API == 1)
	NxLabelContainer(Scene, mFluids, Fluids);
#endif

#if (NX_USE_FORCEFIELD_API == 1)
	NxLabelContainer(Scene, mForceFields, ForceFields);
#endif

#if (NX_USE_SOFTBODY_API == 1)
	NxLabelContainer(Scene, mSoftBodies, SoftBodies);
#endif

	NxLabelContainer(Scene, mMaterials, MaterialList);
	NxLabelContainer(Scene, mMaterialsByIndex, MaterialListByIndex);
	NxLabelContainer(Scene, mMaterialPair, MaterialPair);
	NxLabelContainer(Scene, mMachines, Machines);

	NxLabelContainer(Scene, mShapeGroups, ShapeGroups);
	NxLabelContainer(Scene, mShapeGroupsByIndex, ShapeGroupsByIndex);

#if (NX_SDK_VERSION_NUMBER >= 272) 
	NxLabelContainer(Scene, mDominanceGroups, DominanceGroups);
	NxLabelContainer(Scene, mDominanceGroupsByIndex, DominanceGroupsByIndex);
#endif

}

///////////////////////////////////////////////////////////////////////

Scene::Scene(const NxString& identifier, World* world, NxScene* nxscene)
: mName(identifier), mNxID(mName.c_str()), mOwner(world), mBatchProcessSimulate(false)  {

	mOwner->_registerScene(mName, this);
	mScene = nxscene;

	mScene->userData = (void*) this;
	
	NxReal mxts;
	NxU32 mxit;
	NxTimeStepMethod tsm;
	NxU32 nbss;
	mScene->getTiming(mxts, mxit, tsm, &nbss);


	// TODO: Accumulator and thingy. Perhaps via a the userData string thing
	if (tsm == NX_TIMESTEP_FIXED) {
		mSceneController = NxNew(FixedSceneController) FixedSceneController(this);
	}
	else {
		mSceneController = NxNew(VariableSceneController) VariableSceneController(this);
	}
	mSceneController->setTiming(mxts, mxit, nbss);

	mSceneContactController = NxNew(SceneContactController) SceneContactController(this);
	mSceneTriggerController = NxNew(SceneTriggerController) SceneTriggerController(this);

	mScene->setUserContactReport(mSceneContactController);
	mScene->setUserTriggerReport(mSceneTriggerController);

	mSceneController->init(mScene);

	// Temp
	mSceneRenderer = NxNew(OgreSceneRenderer) OgreSceneRenderer(this, "#first");

	_createDefaultActorGroup();
	_createDefaultShapeGroup();
	_createDefaultDominanceGroup();

	NxMaterial* dm = mScene->getMaterialFromIndex(0);	
	_createDefaultMaterial(dm->getRestitution(),dm->getDynamicFriction(),dm->getStaticFriction());

}

///////////////////////////////////////////////////////////////////////

Scene::~Scene() {

	mOwner->_unregisterScene(mName);
	
	NxString stats;

	#if (NX_DEBUG == 0)
		std::stringstream ss;
		ss << "End of scene '" << mName << "' stats" << std::endl << "\t";
	#endif

	if (mSleepCallback)
		if (mSleepCallbackPolicy == GC_Delete)
			delete mSleepCallback;

	//////////////////////////////
	#if (NX_USE_FORCEFIELD_API == 1)

		#if (NX_DEBUG == 1)
			if (mForceFields.count())
				mForceFields.dumpToConsole();
		#else
			if (mForceFields.count() != 0)
				ss << "ForceFields (" << mForceFields.count() << "), ";
		#endif

		
		mForceFields.destroyAllOwned();

	#endif

	//////////////////////////////

	#if (NX_USE_FLUID_API == 1)

		#if (NX_DEBUG == 1)
			if (mFluids.count())
				mFluids.dumpToConsole();
		#else
			if (mFluids.count() != 0)
				ss << "Fluids (" << mFluids.count() << "), ";
		#endif

		mFluids.destroyAllOwned();

	#endif

	//////////////////////////////

	#if (NX_USE_CLOTH_API == 1)

		#if (NX_DEBUG == 1)
		if (mCloths.count())
			mCloths.dumpToConsole();
		#else
			if (mCloths.count() != 0)
				ss << "Cloths (" << mCloths.count() << "), ";
		#endif
			
		
		
		mCloths.destroyAllOwned();

	#endif

	//////////////////////////////

	#if (NX_USE_SOFTBODY_API == 1)

		#if (NX_DEBUG == 1)
		if (mSoftBodies.count())
			mSoftBodies.dumpToConsole();
		#else
			if (mSoftBodies.count() != 0)
				ss << "SoftBodies (" << mSoftBodies.count() << "), ";
		#endif
	
	#endif

	//////////////////////////////
	
	#if (NX_DEBUG == 1)
		if (mJoints.count())
			mJoints.dumpToConsole();
	#else
		if (mJoints.count() != 0)
				ss << "Joints (" << mJoints.count() << "), ";
	#endif

	mJoints.destroyAllOwned();

	//////////////////////////////

	#if (NX_USE_CHARACTER_API == 1)

		#if (NX_DEBUG == 1)
			if (mCharacters.count())
				mCharacters.dumpToConsole();
		#else
			if (mCharacters.count() != 0)
				ss << "Characters (" << mCharacters.count() << "), ";
		#endif
		
		mCharacters.destroyAllOwned();
	
	#endif

	//////////////////////////////

	#if (NX_DEBUG == 1)
		if (mTriggers.count())
			mTriggers.dumpToConsole();
	#else
		if (mTriggers.count() != 0)
			ss << "Triggers (" << mTriggers.count() << "), ";
	#endif
	
	mSimpleActors.DestroyAll();

	//////////////////////////////

	#if (NX_DEBUG == 1)
		if (mActors.count())
			mActors.dumpToConsole();
	#else
		if (mActors.count() != 0)
			ss << "Actors (" << mActors.count() << "), ";
	#endif

	mActors.destroyAllOwned();

	//////////////////////////////

	#if (NX_DEBUG == 1)
		if (mMaterials.count())
			mMaterials.dumpToConsole();
	#else
		if (mMaterials.count() != 0)
			ss << "Materials (" << mMaterials.count() << ").";
	#endif

	mMaterials.destroyAllOwned();
	mMaterialsByIndex.empty();

	mActorGroups.destroyAllOwned();
	mActorGroupsByIndex.empty();

	mShapeGroups.destroyAllOwned();
	mShapeGroupsByIndex.empty();
	
	mDominanceGroups.destroyAllOwned();
	mDominanceGroups.empty();

	if (mSceneIntersection)
		NxDelete(mSceneIntersection);

	if (mSceneRayCaster)
		NxDelete(mSceneRayCaster);


	NxDelete(mSceneRenderer);
	NxDelete(mSceneController);

	//////////////////////////////

	mOwner->getSDK()->releaseScene(*mScene);

	NxDelete(mSceneContactController);
	NxDelete(mSceneTriggerController);

	//////////////////////////////

	
#if (NX_DEBUG == 1)
	NxDebug(NxString("Destroyed Scene '" + mName + "'.").c_str());
#endif
	
}

///////////////////////////////////////////////////////////////////////

void Scene::setName(const NxOgre::NxString &name) {
	mOwner->mScenes.rename(mName, name);
	mName = name;
}

///////////////////////////////////////////////////////////////////////

void Scene::setSceneController(SceneController* controller) {
	
	if (mSceneController)
		delete mSceneController;
	mSceneController = controller;
}

///////////////////////////////////////////////////////////////////////

void Scene::setSceneRenderer(SceneRenderer* renderer) {
	
	if (mSceneRenderer)
		delete mSceneRenderer;
	
	mSceneRenderer = renderer;

}

///////////////////////////////////////////////////////////////////////

NxReal Scene::getLastDeltaTime() const {
	return mSceneController->getDeltaTime();
}

///////////////////////////////////////////////////////////////////////

NxReal Scene::getLastAlphaValue() const {
	return mSceneController->getAlphaValue();
}

///////////////////////////////////////////////////////////////////////

void Scene::batchCreateActors(const NxString& identifierPattern, NxU32 nbActors, Shape* collisionModel, const Pose& pose, const Pose& poseDelta, BatchActionTimeFrame batchTime, ActorParams actorParams) {

	BatchActorBlueprint bp;
	bp.collisionModel = collisionModel;
	bp.identifierPattern = identifierPattern;
	bp.visualData = "";
	bp.nbActors = nbActors;
	bp.params = actorParams;
	bp.poseDelta = poseDelta;
	bp.poseOrigin = pose;

	if (batchTime == BT_NOW) {
		_batchActorCreate(bp);
	}
	else {
		mBatchActorCreation.push_back(std::pair<BatchActionTimeFrame, BatchActorBlueprint>(batchTime, bp));
		if (batchTime == BT_NEXT_SIMULATION)
			mBatchProcessSimulate = true;
	}

}
		
///////////////////////////////////////////////////////////////////////

void Scene::batchCreateBodies(const NxString& identifierPattern, const NxString& visualData, NxU32 nbActors, Shape* collisionModel, const Pose& pose, const Pose& poseDelta, BatchActionTimeFrame batchTime, ActorParams actorParams) {

	BatchActorBlueprint bp;
	bp.collisionModel = collisionModel;
	bp.identifierPattern = identifierPattern;
	bp.visualData = visualData;
	bp.nbActors = nbActors;
	bp.params = actorParams;
	bp.poseDelta = poseDelta;
	bp.poseOrigin = pose;

	if (batchTime == BT_NOW) {
		_batchBodyCreate(bp);
	}
	else {
		mBatchActorCreation.push_back(std::pair<BatchActionTimeFrame, BatchActorBlueprint>(batchTime, bp));
		if (batchTime == BT_NEXT_SIMULATION)
			mBatchProcessSimulate = true;
	}

}
		
///////////////////////////////////////////////////////////////////////

void Scene::_batchActorCreate(BatchActorBlueprint& bp) {
#if 0
	if (bp.visualData.length() != 0) {
		_batchBodyCreate(bp);
		return;
	}

	Pose poseNext = bp.poseOrigin;
//	bp.collisionModel->setShared(true);

	NxString actorIdentifier = ">" + bp.identifierPattern;
	Actor* actor;
	for (NxU32 i=0;i < bp.nbActors;i++) {
	
		actor = NxNew(Actor) Actor(
				actorIdentifier + Ogre::StringConverter::toString(i),
				this,
				bp.collisionModel,
				poseNext,
				bp.params
		);
		mActors.lock(actor->getName(), true);

		poseNext.v += bp.poseDelta.v;
		// TODO: Quaternions.
	}

//	delete bp.collisionModel;
#endif
}
		
///////////////////////////////////////////////////////////////////////
	
void Scene::_batchBodyCreate(BatchActorBlueprint& bp) {
#if 0
	Pose poseNext = bp.poseOrigin;
	bp.collisionModel->setShared(true);

	NxString actorIdentifier = ">" + bp.identifierPattern;
	Actor* actor;
	for (NxU32 i=0;i < bp.nbActors;i++) {
		
		actor = NxNew(Body) Body(
				actorIdentifier + Ogre::StringConverter::toString(i) + ";" + bp.visualData,
				this,
				bp.collisionModel,
				poseNext,
				bp.params
		);
		mActors.lock(actor->getName(), true);

		poseNext.v += bp.poseDelta.v;
		// TODO: Quaternions.
	}

	delete bp.collisionModel;
#endif
}

///////////////////////////////////////////////////////////////////////

void Scene::batchDestroyActor(const NxOgre::NxString &identifier, NxOgre::Scene::BatchActionTimeFrame tf) {
	Actors a;
	a.insert(identifier, mActors.get(identifier));
	mBatchActorDestruction.push_back(std::pair<BatchActionTimeFrame, Actors>(tf, a)); 
}

///////////////////////////////////////////////////////////////////////

void Scene::batchDestroyActors(Actors a, BatchActionTimeFrame tf) {
	mBatchActorDestruction.push_back(std::pair<BatchActionTimeFrame, Actors>(tf, a)); 
	
	// TODO: Consider to limit to 64 is a good acount, or should be depedant on FPS.
	for (Actor* actor = a.begin();actor = a.next();) {
			actor->disable();
		}
	
}

///////////////////////////////////////////////////////////////////////

void Scene::_batchActorDestroy(Actors& actors) {
	
	for (Actor* actor = actors.begin();actor = actors.next();) {
		if (mActors.isLocked(actor->getName()))
			delete actor;
	}

}
		
///////////////////////////////////////////////////////////////////////

Actor* Scene::createActor(const NxString& identifier, Shape *firstShapeDescription, const Pose& pose, ActorParams params) {
	Actor* actor = NxNew(Actor) Actor(identifier, this, firstShapeDescription, pose, params);
	mActors.lock(actor->getName(), true);
	return actor;
}

///////////////////////////////////////////////////////////////////////

void Scene::destroyActor(const NxString& name) {
	Actor* a = mActors.get(name);
	if (mActors.isLocked(name))
		delete a;

}

///////////////////////////////////////////////////////////////////////

Actors Scene::getActorsFromIdentifier(NxStringOperator op, const NxString& pattern) {
	
	Actors returnActors;
	Actors sceneActors;
	mActors.CopyTo(sceneActors);

	NxString lPattern = pattern;
	Ogre::StringUtil::toLowerCase(lPattern);

	switch (op) {

		case SO_STARTS_WITH:
			for (Actor* actor = sceneActors.begin();actor = sceneActors.next();) {
				if (Ogre::StringUtil::startsWith(actor->getName(), lPattern, true))
					returnActors.insert(actor->getName(),actor);
			}
		break;

		case SO_ENDS_WITH:
			for (Actor* actor = sceneActors.begin();actor = sceneActors.next();) {
				if (Ogre::StringUtil::endsWith(actor->getName(), lPattern, true))
					returnActors.insert(actor->getName(),actor);
			}
		break;

	}
	
	return returnActors;
}

///////////////////////////////////////////////////////////////////////

Actors Scene::getActorsFromFrame(NxComparisonOperator op, NxU32 frame) {
	
	// Yes. This is messy, but it's a lot faster than having one switch statement used once than
	// hundreds in a loop.

	Actors returnActors;
	Actors sceneActors;
	mActors.CopyTo(sceneActors);
	
	switch(op) {

		case CO_EXACTLY:
			for (Actor* actor = sceneActors.begin();actor = sceneActors.next();) {
				if (actor->getBirthFrame() == frame)
					returnActors.insert(actor->getName(),actor);
			}
		break;

		case CO_MORE_THAN:
			for (Actor* actor = sceneActors.begin();actor = sceneActors.next();) {
				if (actor->getBirthFrame() > frame)
					returnActors.insert(actor->getName(),actor);
			}
		break;

		case CO_LESS_THAN:
			for (Actor* actor = sceneActors.begin();actor = sceneActors.next();) {
				if (actor->getBirthFrame() < frame)
					returnActors.insert(actor->getName(),actor);
			}
		break;

		case CO_NOT:
			for (Actor* actor = sceneActors.begin();actor = sceneActors.next();) {
				if (actor->getBirthFrame() != frame)
					returnActors.insert(actor->getName(),actor);
			}
		break;

	}

	return returnActors;
}

///////////////////////////////////////////////////////////////////////

Actors& Scene::getActorsFromRegion(SimpleShape* shape, NxShapesType shapesType) {
	
	if (mSceneIntersection == 0) {
		mSceneIntersection = NxNew(Intersection) Intersection(this, shape, shapesType);
		return *mSceneIntersection;
	}
	
	mSceneIntersection->setShape(shape);
	mSceneIntersection->intersect();

	return *mSceneIntersection;
}

///////////////////////////////////////////////////////////////////////

Actors& Scene::getActorsFromLastRegion(NxShapesType shapesType) {
	
	if (mSceneIntersection == 0) {
		mSceneIntersection = NxNew(Intersection) Intersection(this, new SimpleBox(1), shapesType);
		return *mSceneIntersection;
	}

	mSceneIntersection->intersect();
	return *mSceneIntersection;
}

///////////////////////////////////////////////////////////////////////

Actors Scene::getActorsFromRaycast(Ogre::Vector3 position, Ogre::Vector3 normal, NxReal range) {

	if (mSceneRayCaster == 0) {
		mSceneRayCaster = NxNew(RayCaster) RayCaster(position, normal, range, RayCaster::RCT_ALL, this);	
	}
	else {
		mSceneRayCaster->setOrigin(position);
		mSceneRayCaster->setDirection(normal);
		mSceneRayCaster->setMaxDistance(range);
	}

	Actors actors;
	mSceneRayCaster->castShape(RayCaster::AF_NONE);
	
	std::cout << "Count" << mSceneRayCaster->mReport.count() << std::endl;
	
	if (mSceneRayCaster->mReport.count() > 0) {
	for (RayCastHit hit = mSceneRayCaster->mReport._begin();!mSceneRayCaster->mReport._atEnd();hit = mSceneRayCaster->mReport._next()) {
			actors.insert(hit.mActor->getName(), hit.mActor);
		}
	}
	

	return actors;
}

///////////////////////////////////////////////////////////////////////

Actors* Scene::getActors() {
	return &mActors;
}

///////////////////////////////////////////////////////////////////////

NxU32 Scene::getNbActors() const {
	return mActors.count();
}

///////////////////////////////////////////////////////////////////////

Body* Scene::createBody(const NxString& identifier, Shape *firstShapeDescription, const Pose& pose, ActorParams params) {
	Body* body = NxNew(Body) Body(identifier, this, firstShapeDescription, pose, params);
	mActors.lock(body->getName(), true);
	return body;
}

///////////////////////////////////////////////////////////////////////

Body* Scene::createBody(const NxString& identifier, Shape* firstShapeDescription, const Pose& pose, NodeRenderableParams nrparams, ActorParams params) {
	Body* body = NxNew(Body) Body(identifier, this, firstShapeDescription, pose, nrparams, params);
	mActors.lock(body->getName(), true);
	return body;
}
			

///////////////////////////////////////////////////////////////////////

void Scene::destroyBody(const NxString& name) {
	Actor* body = mActors.get(name);
	if (mActors.isLocked(name))
		delete body;
}

#if (NX_USE_LEGACY_NXCONTROLLER == 0)

///////////////////////////////////////////////////////////////////////

Character* Scene::createCharacter(const NxString &identifier, Pose pose, CharacterModel*, CharacterParams params) {
	NxUnderConstruction;
	return NULL;
}

///////////////////////////////////////////////////////////////////////

void Scene::destroyCharacter(const NxString& identifer) {
	NxUnderConstruction;
}

///////////////////////////////////////////////////////////////////////

#else

///////////////////////////////////////////////////////////////////////

Character* Scene::createCharacter(const NxString &name, Pose p, CharacterParams cp) {
	return mOwner->getCharacterController()->create(name, p, cp, this);
}

///////////////////////////////////////////////////////////////////////

void Scene::destroyCharacter(const NxString &name) {
	mOwner->getCharacterController()->destroy(name);
}

///////////////////////////////////////////////////////////////////////

#endif

Material* Scene::createMaterial(const NxString& name) {
	Material *m = NxNew(Material) Material(name, this);
	mMaterials.lock(m->getName(), true);
	mMaterialsByIndex.lock(m->getMaterialIndex(), true);
	return m;
}

///////////////////////////////////////////////////////////////////////

void Scene::destroyMaterial(const NxString& name) {
	Material* material = mMaterials.get(name);
	if (mMaterials.isLocked(name))
		delete material;
}

///////////////////////////////////////////////////////////////////////

void Scene::_createDefaultShapeGroup() {
	ShapeGroup* sag = NxNew(ShapeGroup) ShapeGroup(0, "Default", this);
	mShapeGroups.lock(sag->getName(), true);
	mShapeGroupsByIndex.lock(sag->getGroupID(), true);
}

///////////////////////////////////////////////////////////////////////

void Scene::_createDefaultActorGroup() {
	ActorGroup* dag = NxNew(ActorGroup) ActorGroup(0, "Default", this);
	mActorGroups.lock(dag->getName(), true);
	mActorGroupsByIndex.lock(dag->getGroupID(), true);
}
 

///////////////////////////////////////////////////////////////////////

void Scene::_createDefaultDominanceGroup() {
	DominanceGroup* ddg = NxNew(DominanceGroup) DominanceGroup(0, "Default", this);
	mDominanceGroups.lock(ddg->getName(), true);
	mDominanceGroupsByIndex.lock(ddg->getGroupID(), true);
}

///////////////////////////////////////////////////////////////////////

ActorGroup*	Scene::createActorGroup(const NxString& name) {
	ActorGroup* ag = NxNew(ActorGroup) ActorGroup(name, this);
	mActorGroups.lock(ag->getName(), true);
	mActorGroupsByIndex.lock(ag->getGroupID(), true);
	return ag;
}

///////////////////////////////////////////////////////////////////////

void Scene::destroyActorGroup(const NxString& name) {
	ActorGroup* ag = mActorGroups.get(name);
	if (mActorGroups.isLocked(name))
		delete ag;
}

///////////////////////////////////////////////////////////////////////

NxU32 Scene::getNbActorGroups() const {
	return mActorGroups.count();
}

///////////////////////////////////////////////////////////////////////

ActorGroups* Scene::getActorGroups() {
	return &mActorGroups;
}

///////////////////////////////////////////////////////////////////////

ActorGroupsByIndex* Scene::getActorGroupsByIndex() {
	return &mActorGroupsByIndex;
}

///////////////////////////////////////////////////////////////////////

ShapeGroup*	Scene::createShapeGroup(const NxString& name) {
	ShapeGroup* ag = NxNew(ShapeGroup) ShapeGroup(name, this);
	mShapeGroups.lock(ag->getName(), true);
	mShapeGroupsByIndex.lock(ag->getGroupID(), true);
	return ag;	
}

///////////////////////////////////////////////////////////////////////

void Scene::destroyShapeGroup(const NxString& name) {
	ShapeGroup* ag = mShapeGroups.get(name);
	if (mShapeGroups.isLocked(name))
		delete ag;
}

///////////////////////////////////////////////////////////////////////

NxU32 Scene::getNbShapeGroups() const {
	return mShapeGroups.count();
}

///////////////////////////////////////////////////////////////////////

ShapeGroups* Scene::getShapeGroups() {
	return &mShapeGroups;
}

///////////////////////////////////////////////////////////////////////

ShapeGroupsByIndex* Scene::getShapeGroupsByIndex() {
	return &mShapeGroupsByIndex;
}

#if (NX_SDK_VERSION_NUMBER >= 272)

///////////////////////////////////////////////////////////////////////

DominanceGroup*	Scene::createDominanceGroup(const NxString& name) {
	DominanceGroup* ag = NxNew(DominanceGroup) DominanceGroup(name, this);
	mDominanceGroups.lock(ag->getName(), true);
	mDominanceGroupsByIndex.lock(ag->getGroupID(), true);
	return ag;	
}

///////////////////////////////////////////////////////////////////////

void Scene::destroyDominanceGroup(const NxString& name) {
	DominanceGroup* ag = mDominanceGroups.get(name);
	if (mDominanceGroups.isLocked(name))
		delete ag;
}

///////////////////////////////////////////////////////////////////////

void Scene::setDominanceGroupPair(DominanceGroup* AGroup, DominanceGroup* BGroup, NxReal ADominance, NxReal BDominance) {

	NxConstraintDominance domConstaint(ADominance, BDominance);
	
	mScene->setDominanceGroupPair(AGroup->getGroupID(), BGroup->getGroupID(), domConstaint);

}


///////////////////////////////////////////////////////////////////////

void Scene::setDominanceGroupPair(const NxString& AGroup, const NxString& BGroup, NxReal ADominance, NxReal BDominance) {
	
	DominanceGroup* agroup = mDominanceGroups.get(AGroup);
	if (agroup == 0)
		return;

	DominanceGroup* bgroup = mDominanceGroups.get(BGroup);
	if (bgroup == 0)
		return;

	setDominanceGroupPair(agroup, bgroup, ADominance, BDominance);
}

///////////////////////////////////////////////////////////////////////

NxU32 Scene::getNbDominanceGroups() const {
	return mDominanceGroups.count();
}

///////////////////////////////////////////////////////////////////////

DominanceGroups* Scene::getDominanceGroups() {
	return &mDominanceGroups;
}

///////////////////////////////////////////////////////////////////////

DominanceGroupsByIndex* Scene::getDominanceGroupsByIndex() {
	return &mDominanceGroupsByIndex;
}

///////////////////////////////////////////////////////////////////////

DominanceGroup*	Scene::getDominanceGroup(const NxString& identifier) {
	return mDominanceGroups.get(identifier);
}

///////////////////////////////////////////////////////////////////////

#endif

void Scene::_registerActor(const NxString& name, Actor* a) {
	mActors.insert(name, a);
}

///////////////////////////////////////////////////////////////////////

void Scene::_unregisterActor(const NxString& name) {
	mActors.remove(name);
}

///////////////////////////////////////////////////////////////////////

void Scene::_registerMaterial(const NxString& name, Material* m) {
	mMaterials.insert(name, m);
	mMaterialsByIndex.insert(m->getMaterialIndex(), m);
}

///////////////////////////////////////////////////////////////////////

void Scene::_unregisterMaterial(const NxString& n) {
	Material *m = mMaterials.get(n);
	NxMaterialIndex i = m->getMaterialIndex();
	mMaterials.remove(n);
	mMaterialsByIndex.remove(i);
}

///////////////////////////////////////////////////////////////////////

void Scene::_registerActorGroup(const NxString& name, ActorGroup* ag) {
	mActorGroups.insert(name, ag);
	mActorGroupsByIndex.insert(ag->getGroupID(), ag);
}

///////////////////////////////////////////////////////////////////////

void Scene::_unregisterActorGroup(const NxString& name) {
	ActorGroup *ag = mActorGroups.get(name);
	NxActorGroup i = ag->getGroupID();
	mActorGroups.remove(name);
	mActorGroupsByIndex.remove(i);
}

///////////////////////////////////////////////////////////////////////

void Scene::_registerShapeGroup(const NxString& name, ShapeGroup* sg) {
	mShapeGroups.insert(name, sg);
	mShapeGroupsByIndex.insert(sg->getGroupID(), sg);
}

///////////////////////////////////////////////////////////////////////

void Scene::_unregisterShapeGroup(const NxString& name) {
	ShapeGroup *sg = mShapeGroups.get(name);
	NxCollisionGroup i = sg->getGroupID();
	mShapeGroups.remove(name);
	mShapeGroupsByIndex.remove(i);
}

#if (NX_SDK_VERSION_NUMBER >= 272)
///////////////////////////////////////////////////////////////////////

void Scene::_registerDominanceGroup(const NxString& name, DominanceGroup* sg) {
	mDominanceGroups.insert(name, sg);
	mDominanceGroupsByIndex.insert(sg->getGroupID(), sg);
}

///////////////////////////////////////////////////////////////////////

void Scene::_unregisterDominanceGroup(const NxString& name) {
	DominanceGroup *sg = mDominanceGroups.get(name);
	NxCollisionGroup i = sg->getGroupID();
	mDominanceGroups.remove(name);
	mDominanceGroupsByIndex.remove(i);
}

///////////////////////////////////////////////////////////////////////

#endif

void Scene::_createDefaultMaterial(NxReal r, NxReal d, NxReal s) {
	Material *m = NxNew(Material) Material(0, "Default", this);
	m->setAll(r,d,s);
	mMaterials.lock(m->getName(), true);
	mMaterialsByIndex.lock(0, true);
}

///////////////////////////////////////////////////////////////////////

void Scene::simulate(NxReal time) {

	if (!mSceneController->Simulate(time))
		return;
	
	NxReal deltaTime = mSceneController->getDeltaTime();
	
#if (NX_USE_LEGACY_NXCONTROLLER)
#	if (NX_USE_CHARACTER_API == 1)
	mScene->getTiming(mControllers_MaxTimestep, mControllers_MaxIter, mControllers_Method, &mControllers_NumSubSteps);
	
	if (mControllers_NumSubSteps) {	
		for(Character* character = mCharacters.begin();character = mCharacters.next();) {
			character->simulate(time);
		}
	}
	mOwner->getCharacterController()->getNxControllerManager()->updateControllers();
#	endif
#else
	
	// ...

#endif
	
	for (Machine* machine = mMachines.begin();machine = mMachines.next();) {
		machine->simulate(deltaTime);
	}
#if 0
#if (NX_USE_CLOTH_API == 1)
	for(Cloth* cloth = mCloths.begin();cloth = mCloths.next();) {
		cloth->simulate(deltaTime);
	}
#endif
#endif
	if (mBatchProcessSimulate) {

		for (NxU32 i=0;i < mBatchActorCreation.size();i++) {
			if (mBatchActorCreation[i].first == BT_NEXT_SIMULATION) {
				std::cout << "** Batch Create ** " << std::endl;
				_batchActorCreate(mBatchActorCreation[i].second);
				mBatchActorCreation[i].first = BT_NEVER;
				// Will be removed in the next batch time.
			}
		}

		mBatchProcessSimulate = false;
	}

	// Decrease next batch time counter
	// Perform batche for next_batch time
	// Clean up BT_NEVER batches from ActorCreation and ActorDestruction.
	// and so on.

}

///////////////////////////////////////////////////////////////////////

void Scene::idle() {

	// Things to idle with.

	// 1. Quickly run through a portion of the Actors with states (only say 16 at a time).
	//	-- Reset the mStateCheck to 0, and set it to a SC_Partial, with mStateCheckIterator pointed
	//	   to the next actor.
	// 2. Do other things
	//
	// 3. Fiddle thumbs.
	//

}

///////////////////////////////////////////////////////////////////////

void Scene::render() {
	mSceneRenderer->render();
}

///////////////////////////////////////////////////////////////////////

RevoluteJoint* Scene::createRevoluteJoint(Actor* a, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams jp) {
	RevoluteJoint* j = NxNew(RevoluteJoint) RevoluteJoint(a,axis,anchor,jp);
	NxJointID jid = mJoints.count();
	mJoints.insert(jid, j);
	mJoints.lock(jid, true);
	j->setID(jid);
	return j;
}

///////////////////////////////////////////////////////////////////////

RevoluteJoint* Scene::createRevoluteJoint(Actor* a, Actor* b, const Ogre::Vector3 & axis, const Ogre::Vector3 &anchor, JointParams jp) {
	RevoluteJoint* j = NxNew(RevoluteJoint) RevoluteJoint(a,b,axis,anchor,jp);
	NxJointID jid = mJoints.count();
	mJoints.insert(jid, j);
	mJoints.lock(jid, true);
	j->setID(jid);
	return j;
}

///////////////////////////////////////////////////////////////////////

SphericalJoint* Scene::createSphericalJoint(Actor* a, Actor* b, const Ogre::Vector3 &anchor, JointParams jp) {
	SphericalJoint* j = NxNew(SphericalJoint) SphericalJoint(a,b,anchor,jp);
	NxJointID jid = mJoints.count();
	mJoints.insert(jid, j);
	mJoints.lock(jid, true);
	j->setID(jid);
	return j;
}

///////////////////////////////////////////////////////////////////////

SphericalJoint* Scene::createSphericalJoint(Actor* a, const Ogre::Vector3 &anchor, JointParams jp) {
	SphericalJoint* j = NxNew(SphericalJoint) SphericalJoint(a,anchor,jp);
	NxJointID jid = mJoints.count();
	mJoints.insert(jid, j);
	mJoints.lock(jid, true);
	j->setID(jid);
	return j;
}

///////////////////////////////////////////////////////////////////////

PrismaticJoint* Scene::createPrismaticJoint(Actor* a, Actor* b, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams jp) {
	PrismaticJoint* j = NxNew(PrismaticJoint) PrismaticJoint(a,b,axis,anchor,jp);
	NxJointID jid = mJoints.count();
	mJoints.insert(jid, j);
	mJoints.lock(jid, true);
	j->setID(jid);
	return j;
}

///////////////////////////////////////////////////////////////////////

PrismaticJoint* Scene::createPrismaticJoint(Actor* a, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams jp) {
	PrismaticJoint* j = NxNew(PrismaticJoint) PrismaticJoint(a,axis,anchor,jp);
	NxJointID jid = mJoints.count();
	mJoints.insert(jid, j);
	mJoints.lock(jid, true);
	j->setID(jid);
	return j;
}

///////////////////////////////////////////////////////////////////////

CylindricalJoint* Scene::createCylindricalJoint(Actor* a, Actor* b, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams jp) {
	CylindricalJoint* j = NxNew(CylindricalJoint) CylindricalJoint(a,b,axis,anchor,jp);
	NxJointID jid = mJoints.count();
	mJoints.insert(jid, j);
	mJoints.lock(jid, true);
	j->setID(jid);
	return j;
}

///////////////////////////////////////////////////////////////////////

CylindricalJoint* Scene::createCylindricalJoint(Actor* a, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams jp) {
	CylindricalJoint* j = NxNew(CylindricalJoint) CylindricalJoint(a,axis,anchor,jp);
	NxJointID jid = mJoints.count();
	mJoints.insert(jid, j);
	mJoints.lock(jid, true);
	j->setID(jid);
	return j;
}

///////////////////////////////////////////////////////////////////////

FixedJoint* Scene::createFixedJoint(Actor* a, Actor* b, JointParams jp) {
	FixedJoint* j = NxNew(FixedJoint) FixedJoint(a,b,jp);
	NxJointID jid = mJoints.count();
	mJoints.insert(jid, j);
	mJoints.lock(jid, true);
	j->setID(jid);
	return j;
}


///////////////////////////////////////////////////////////////////////

FixedJoint* Scene::createFixedJoint(Actor* a, JointParams jp) {
	FixedJoint* j = NxNew(FixedJoint) FixedJoint(a, jp);
	NxJointID jid = mJoints.count();
	mJoints.insert(jid, j);
	mJoints.lock(jid, true);
	j->setID(jid);
	return j;
}
		
///////////////////////////////////////////////////////////////////////

void Scene::releaseJoint(Joint* j) {
	NxJointID jid = j->getID();
	Joint* joint = mJoints.get(jid);
	if (mJoints.isLocked(jid))
		delete joint;
}

///////////////////////////////////////////////////////////////////////

void Scene::releaseJoint(NxJointID identifier) {
	Joint* joint = mJoints.get(identifier);
	if (mJoints.isLocked(identifier))
		delete joint;
}

///////////////////////////////////////////////////////////////////////

#if (NX_USE_CHARACTER_API == 1)

void Scene::_registerCharacter(const NxString& name, Character* c) {
	NxDebug("Scene Character registered");
	mCharacters.insert(name, c);
}

///////////////////////////////////////////////////////////////////////

void Scene::_unregisterCharacter(const NxString& name) {
	NxDebug("Scene Character removed");
	mCharacters.remove(name);
}

#endif

///////////////////////////////////////////////////////////////////////

void Scene::addFloor() {
	ActorParams ap;
	ap.setToDefault();
	ap.mMass = 0;
	ap.mDensity = 0;
	Actor* actor = NxNew(Actor) Actor("Ground", this, new Ground(), Pose(), ap);
	mActors.lock(actor->getName(), true);
}

///////////////////////////////////////////////////////////////////////

void Scene::setGravity(const Ogre::Vector3& v) {
	mScene->setGravity(NxConvert<NxVec3, Ogre::Vector3>(v));
}

///////////////////////////////////////////////////////////////////////

Ogre::Vector3 Scene::getGravity() const {
	NxVec3 g;
	mScene->getGravity(g);
	return NxConvert<Ogre::Vector3, NxVec3>(g);
}

///////////////////////////////////////////////////////////////////////

NxMaterialIndex Scene::getMaterialIndex(const NxString& name) {
	if (!mMaterials.has(name))
		return 0;

	return mMaterials.get(name)->getMaterialIndex();
}

///////////////////////////////////////////////////////////////////////
#if 0
#if (NX_USE_CLOTH_API == 1)

///////////////////////////////////////////////////////////////////////

Actor*	Scene::createDeformableActor(const NxString& meshName, ShapeBlueprint* roughShape, const Pose& pose, ActorParams ap, ClothParams cp, NxReal impulseThreshold, NxReal penetrationDepth) {

	NxString id = NxCreateID(mActors.count(), meshName);
	Actor* actor = createActor(id, roughShape, pose, ap);
	Cloth* cloth = createCloth(id, meshName, pose, cp);
	cloth->attachToCore(actor->getNxActor(), impulseThreshold, penetrationDepth);

	return actor;			
}

///////////////////////////////////////////////////////////////////////

Cloth*	Scene::createCloth(const NxString& identifier, const Pose& p, ClothParams cp) {
	Cloth *c = new Cloth(identifier, this, p, cp);
	mCloths.lock(c->getName(), true);
	return c;
}

///////////////////////////////////////////////////////////////////////

Cloth*	Scene::createCloth(const NxString& identifier, const NxString& meshName, const Pose& p, ClothParams cp) {
	Cloth *c = new Cloth(identifier, meshName, this, p, cp);
	mCloths.lock(c->getName(), true);
	return c;
}

///////////////////////////////////////////////////////////////////////

void	Scene::destroyCloth(const NxString& name) {
	Cloth* cloth = mCloths.get(name);
	if (mCloths.isLocked(name))
		delete cloth;
}

///////////////////////////////////////////////////////////////////////

void Scene::_registerCloth(const NxString& name, Cloth* c) {
	mCloths.insert(name, c);
}

///////////////////////////////////////////////////////////////////////

void Scene::_unregisterCloth(const NxString& name) {
	mCloths.remove(name);
}

///////////////////////////////////////////////////////////////////////

Cloths*	Scene::getCloths() {
	return &mCloths;
}

///////////////////////////////////////////////////////////////////////

NxU32 Scene::getNbCloths() {
	return mCloths.count();
}

///////////////////////////////////////////////////////////////////////

#endif
#endif
#if (NX_USE_FLUID_API == 1)

///////////////////////////////////////////////////////////////////////

Fluid*	Scene::createFluid(const Pose& p, FluidParams fp) {
	Fluid *f = NxNew(Fluid) Fluid(this, p, fp);
	mFluids.lock(f->getName(), true);
	return f;
}

///////////////////////////////////////////////////////////////////////

void Scene::destroyFluid(const NxString& name) {
	Fluid* fluid = mFluids.get(name);
	if (mFluids.isLocked(name))
		delete fluid;
}

///////////////////////////////////////////////////////////////////////

void Scene::_registerFluid(const NxString& name, Fluid* c) {
	mFluids.insert(name, c);
}

///////////////////////////////////////////////////////////////////////

void Scene::_unregisterFluid(const NxString& name) {
	mFluids.remove(name);
}

///////////////////////////////////////////////////////////////////////

NxU32 Scene::getNbFluids() const {
	return mFluids.count();
}

///////////////////////////////////////////////////////////////////////

Fluids* Scene::getFluids() {
	return &mFluids;
}

///////////////////////////////////////////////////////////////////////

#endif

#if (NX_USE_SOFTBODY_API == 1)

///////////////////////////////////////////////////////////////////////

SoftBody* Scene::createSoftBody(const NxString& TetAndMeshFilePrefix, const Pose& pose, SoftBodyParams sp) {
	SoftBody* softbody = NxNew(SoftBody) SoftBody(TetAndMeshFilePrefix, this, pose, sp);
	mSoftBodies.lock(softbody->getName(), true);
	return softbody;
}

///////////////////////////////////////////////////////////////////////

SoftBody* Scene::createSoftBody(NxReal width, NxReal height, NxReal depth, const Pose& pose, SoftBodyParams sbp) {
	SoftBody* softbody = NxNew(SoftBody) SoftBody(width, height, depth, this, pose, sbp);
	mSoftBodies.lock(softbody->getName(), true);
	return softbody;
}

///////////////////////////////////////////////////////////////////////

void Scene::destroySoftBody(const NxString& name) {
	SoftBody* softbody = mSoftBodies.get(name);
	if (mSoftBodies.isLocked(name))
		delete softbody;
}

///////////////////////////////////////////////////////////////////////

NxU32 Scene::getNbSoftBodies() {
	return mSoftBodies.count();
}

///////////////////////////////////////////////////////////////////////

SoftBodies*	Scene::getSoftBodies() {
	return &mSoftBodies;
}

///////////////////////////////////////////////////////////////////////

void Scene::_registerSoftBody(const NxString& name, SoftBody* c) {
	mSoftBodies.insert(name, c);
}

///////////////////////////////////////////////////////////////////////

void Scene::_unregisterSoftBody(const NxString& name) {
	mSoftBodies.remove(name);
}

///////////////////////////////////////////////////////////////////////

#endif

ActorGroup*	Scene::getActorGroup(const NxString& identifier) {
	return mActorGroups.get(identifier);
}

///////////////////////////////////////////////////////////////////////

Actor* Scene::getActor(const NxString& identifier) {
	return mActors.get(identifier);
}

///////////////////////////////////////////////////////////////////////

ShapeGroup*	Scene::getShapeGroup(const NxString& identifier) {
	return mShapeGroups.get(identifier);
}

///////////////////////////////////////////////////////////////////////

Trigger* Scene::createTrigger(SimpleShape* shape, TriggerContactCallback* callback) {
	Trigger* trigger = NxNew(Trigger) Trigger(shape, callback, this);
	mSimpleActors.Insert(trigger);
	return trigger;
}

///////////////////////////////////////////////////////////////////////

void Scene::destroyTrigger(Trigger*) {
//	Trigger* trigger = mTriggers.get(name);
//	if (mTriggers.isLocked(name))
//		delete trigger;
}

///////////////////////////////////////////////////////////////////////

NxU32	 Scene::getNbTriggers() {
//	return mTriggers.count();
	return 0;
}

///////////////////////////////////////////////////////////////////////

void Scene::_registerTrigger(const NxString& name, Trigger* t) {
	mTriggers.insert(name, t);
}

///////////////////////////////////////////////////////////////////////

void Scene::_unregisterTrigger(const NxString& name) {
	mTriggers.remove(name);
}

///////////////////////////////////////////////////////////////////////

#if (NX_USE_FORCEFIELD_API == 1)

ForceField* Scene::createForceField() {
//	ForceField* forcefield = new ForceField(identifier, this, pose, actorShape, aparams, fparams);
//	mForceFields.lock(identifier, true);
//	return forcefield;

	return NULL;
}

///////////////////////////////////////////////////////////////////////

void Scene::destroyForceField() {
//	ForceField* forcefield = mForceFields.get(name);
//	if (mForceFields.isLocked(name))
//		delete forcefield;
}

///////////////////////////////////////////////////////////////////////

NxU32 Scene::getNbForceFields() {
	return mForceFields.count();
}

///////////////////////////////////////////////////////////////////////

ForceFields* Scene::getForceFields() {
	return &mForceFields;
}

///////////////////////////////////////////////////////////////////////

void Scene::_registerForceField(const NxString& name, ForceField* ff) {
	mForceFields.insert(name, ff);
}

///////////////////////////////////////////////////////////////////////

void Scene::_unregisterForceField(const NxString& name) {
	mForceFields.remove(name);
}

///////////////////////////////////////////////////////////////////////

#endif

Material* Scene::getMaterial(const NxString& name) {
	return mMaterials.get(name);
}

///////////////////////////////////////////////////////////////////////

Material* Scene::getMaterial(NxMaterialIndex index) {
	return mMaterialsByIndex.get(index);
}

///////////////////////////////////////////////////////////////////////

MaterialList* Scene::getMaterials() {
	return &mMaterials;
}

///////////////////////////////////////////////////////////////////////

NxU32 Scene::getNbMaterials() {
	return mMaterials.count();
}

///////////////////////////////////////////////////////////////////////

void Scene::setStaticGeometry(Ogre::StaticGeometry* sg) {
	mStaticGeometry = sg;
}

///////////////////////////////////////////////////////////////////////

Ogre::StaticGeometry* Scene::getStaticGeometry() {
	return mStaticGeometry;
}

///////////////////////////////////////////////////////////////////////

void Scene::removeStaticGeometry() {
	mStaticGeometry = 0;
}

///////////////////////////////////////////////////////////////////////

Renderables Scene::getRenderable(NxOgre::Scene::RenderableType type) {
	
	Renderables r;
	
	switch (type) {
		
		case RT_EVERYTHING:
			mActors.CopyTo(r.actors);
			// ...
			// Characters, etc.

		break;

		case RT_MOVED:
		{	
			for (Actor* actor = mActors.begin();actor = mActors.next();) {
//				if (actor->hasVisualChanged())
//					r.actors.insert(actor->getName(), actor);
			}
		}
		break;

		case RT_TRANSFORM:
		{
				NxU32 nbActors = 0;
				NxActiveTransform *actors = mScene->getActiveTransforms(nbActors);

				if(nbActors == 0)
					break;

				NxUserData* aud;
				for(NxU32 i = 0; i < nbActors; ++i) {

					if (actors[i].actor == 0)
						continue;
				
					aud = static_cast<NxUserData*>(actors[i].userData);

						switch (aud->getType()) {
							case NxUserData::T_Actor:
								{
								Actor* a = aud->toActor();
								r.actors.insert(a->getName(), a);
								}
							break;

							case NxUserData::T_Character:
							//	aud->toCharacter()->render(time);
							break;
					}
				}
		}
		break;

	};

	return r;
}

///////////////////////////////////////////////////////////////////////

void Scene::registerMachine(Machine *machine) {
	NxMachineID id = mMachines.count() + 1;
	machine->mMachineID = id;
	mMachines.insert(id, machine);
}

///////////////////////////////////////////////////////////////////////

void Scene::unregisterMachine(Machine* machine) {
	mMachines.remove(machine->mMachineID);
}

///////////////////////////////////////////////////////////////////////

void Scene::setSleepCallback(SleepCallback* callback, NxOgre::GarbageCollectionPolicy gcp) {
	mSleepCallback = callback;
	mSleepCallbackPolicy = gcp;
}

///////////////////////////////////////////////////////////////////////

SleepCallback* Scene::getSleepCallback() {
	return mSleepCallback;
}

///////////////////////////////////////////////////////////////////////

bool Scene::onJointBreak(NxReal,NxJoint &) {

	// Call the JointCallback here.

	// (Delete the joint here.
	return false; // False, doesn't delete the joint. Just been deleted there.
				  // But may have to delete the Joint, but not release it from the scene.
				  // then return true..... Some testing needed.
}

///////////////////////////////////////////////////////////////////////

void Scene::onWake(NxActor **actors_iterator, NxU32 count) {
	
	if (mSleepCallback == 0)
		return;

	while(count--) {
		NxActor* actor = *actors_iterator;

		if (actor->userData == 0)
			return;

		NxUserData* ud = static_cast<NxUserData*>(actor->userData);
		mSleepCallback->ActorWake(ud->toActor());

		actors_iterator++;
	}

}

///////////////////////////////////////////////////////////////////////

void Scene::onSleep(NxActor **actors_iterator, NxU32 count) {
	
	if (mSleepCallback == 0)
		return;

	while(count--) {
		NxActor* actor = *actors_iterator;

		if (actor->userData == 0)
			return;

		NxUserData* ud = static_cast<NxUserData*>(actor->userData);
		mSleepCallback->ActorSleep(ud->toActor());

		actors_iterator++;
	}

}

///////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
