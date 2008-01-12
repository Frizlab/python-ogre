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
#include "NxOgreWorld.h"
#include "NxOgreScene.h"				// For: Scene operations
#include "NxOgreHelpers.h"				// For: Incase of Scene duplicate names.

#if (NX_USE_LEGACY_NXCONTROLLER == 1)
#	include "NxOgreLegacyCharacterController.h"
#else
#	include "NxOgreCharacterSystem.h"	// For: Setting up character controller
#endif

#include "NxOgreDebugRenderer.h"		// For: Times when it's really needed.
#include "NxOgreSerialiser.h"			// For: Serialisation
#include "time.h"						// For: Making sure NxGenerateID is random.

#include "NxOgreResourceManager.h"
#include "NxOgreFileResourceSystem.h"
#include "NxOgreOgreResourceSystem.h"

#include "OgreRoot.h"
#include "OgreSingleton.h"

namespace NxOgre {

///////////////////////////////////////////////////////////////////////

World::World(PhysXDriverParams driverParams) {

	mRoot = Ogre::Root::getSingletonPtr();
	mDriver = new PhysXDriver(this, driverParams);

	srand((unsigned)time(0));

	mDriver->getSDK()->setParameter(NX_VISUALIZATION_SCALE, 1.0f);
	mDriver->getSDK()->setParameter(NX_SKIN_WIDTH,0.01f);
	mDriver->getSDK()->setParameter(NX_VISUALIZE_WORLD_AXES, true);
	mDriver->getSDK()->setParameter(NX_CONTINUOUS_CD, true);
	mDriver->getSDK()->setParameter(NX_CCD_EPSILON, 0.01);

#if (NX_USE_LEGACY_NXCONTROLLER == 0)
	mCharacterSystem = new CharacterSystem(this);
#else
	mCharacterController = new CharacterController();
#endif


#if (NX_USE_DEBUG_RENDERER_API == 1)
	mDebugRenderer = 0;
#endif

//	mSerialiser = new Serialiser(this);

	mResourceManager = new ResourceManager(mDriver);
	
	if (driverParams.mInitResources) {
		mResourceManager->initAllResourceSystems();
	}

}

///////////////////////////////////////////////////////////////////////

World::~World() {
	shutdown();
}

///////////////////////////////////////////////////////////////////////

void World::shutdown() {

	#ifdef NX_DEBUG
		NxDebug("NxOgre shutting down.");
	#endif

	//	NxDelete(mSerialiser);

	#if (NX_USE_DEBUG_RENDERER_API == 1)
		NxDelete(mDebugRenderer);
	#endif

	#ifdef NX_DEBUG
		mScenes.dumpToConsole();
	#endif

	mScenes.destroyAllOwned();
#if (NX_USE_LEGACY_NXCONTROLLER == 1)
#	if (NX_USE_CHARACTER_API == 1)
		NxDelete(mCharacterController);
#	endif
#else
		NxDelete(mCharacterSystem);
#endif
	delete mResourceManager;
	NxDebug("Destroyed ResourceManager.");

	NxDelete(mDriver);
}

///////////////////////////////////////////////////////////////////////

void World::_registerScene(const NxString& name, Scene* scene) {

	if (mScenes.has(name)) {
		NxString newname = NxCreateID(mScenes.count(), name);
		NxThrow_Conflict("Tried to register a scene with a duplicate name '" + scene->getName() + "', name is now '" + newname + "'.");
		mScenes.insert(newname, scene);
		return;
	}

	mScenes.insert(name, scene);
}

///////////////////////////////////////////////////////////////////////

void World::_unregisterScene(const NxString& name) {
	mScenes.remove(name);
}
///////////////////////////////////////////////////////////////////////

Scene* World::createScene(const NxString& name, Ogre::SceneManager* s, SceneParams params) {
	params.rendererUserData = s->getName();
	return createScene(name, params);
}

///////////////////////////////////////////////////////////////////////

Scene* World::createScene(const NxString& name, SceneParams p) {

	Scene* scene;

	if (mScenes.has(name)) {
		NxString newname = NxCreateID(mScenes.count(), name);
		NxThrow_Conflict("Tried to create a scene with a duplicate name '" + name + "', name is now '" + newname + "'.");
		scene = new Scene(NxCreateID(mScenes.count(), newname), this, p);
	}
	else {
		scene = new Scene(name, this, p);
	}

	mScenes.lock(scene->getName(), true);
	return scene;
}

///////////////////////////////////////////////////////////////////////

void World::destroyScene(const NxString& name) {
	Scene* s = mScenes.get(name);
	if (mScenes.isLocked(name))
		delete s;
}

///////////////////////////////////////////////////////////////////////

void World::destroyAllScenes() {
	mScenes.destroyAllOwned();
}

///////////////////////////////////////////////////////////////////////

NxPhysicsSDK* World::getSDK() {
	return mDriver->getSDK();
}

///////////////////////////////////////////////////////////////////////

void World::simulate(NxReal time) {

	for(Scene* scene = mScenes.begin();scene = mScenes.next();) {
		scene->simulate(time);
		
	}

	#if (NX_USE_DEBUG_RENDERER_API == 1)
		if (mDebugRenderer) {

			mDebugRenderer->start();

			for(Scene* scene = mScenes.begin();scene = mScenes.next();) {
				mDebugRenderer->render(*scene->getNxScene()->getDebugRenderable());
			}
			
			mDebugRenderer->stop();
		}
	#endif

}

///////////////////////////////////////////////////////////////////////

void World::render(NxReal time) {

	for(Scene* scene = mScenes.begin();scene = mScenes.next();) {
		scene->render();
	}

}

///////////////////////////////////////////////////////////////////////

Scenes*	World::getScenes() {
	return &mScenes;
}

///////////////////////////////////////////////////////////////////////

#if (NX_USE_DEBUG_RENDERER_API == 1)

void World::createDebugRenderer(Ogre::SceneManager* s) {
	if (mDebugRenderer)
		return;

	NxPhysicsSDK* sdk = mDriver->getSDK();

	sdk->setParameter(NX_VISUALIZATION_SCALE, 1.0f);
	sdk->setParameter(NX_VISUALIZE_WORLD_AXES, true);
	sdk->setParameter(NX_VISUALIZE_BODY_AXES, true);
	sdk->setParameter(NX_VISUALIZE_BODY_MASS_AXES, true);
	sdk->setParameter(NX_VISUALIZE_BODY_LIN_VELOCITY, true);
	sdk->setParameter(NX_VISUALIZE_BODY_ANG_VELOCITY, true);
	sdk->setParameter(NX_VISUALIZE_BODY_JOINT_GROUPS, true);
	sdk->setParameter(NX_VISUALIZE_JOINT_LOCAL_AXES, true);
	sdk->setParameter(NX_VISUALIZE_JOINT_WORLD_AXES, true);
	sdk->setParameter(NX_VISUALIZE_JOINT_LIMITS, true);
	sdk->setParameter(NX_VISUALIZE_CONTACT_POINT, true);
	sdk->setParameter(NX_VISUALIZE_CONTACT_NORMAL, true);
	sdk->setParameter(NX_VISUALIZE_CONTACT_ERROR, true);
	sdk->setParameter(NX_VISUALIZE_CONTACT_FORCE, true);
	sdk->setParameter(NX_VISUALIZE_ACTOR_AXES, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_AABBS, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_SHAPES, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_AXES, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_COMPOUNDS, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_VNORMALS, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_FNORMALS, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_EDGES, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_SPHERES, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_SAP, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_STATIC, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_DYNAMIC, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_FREE, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_CCD, true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_SKELETONS, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_EMITTERS, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_POSITION, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_VELOCITY, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_KERNEL_RADIUS, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_BOUNDS, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_PACKETS, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_MOTION_LIMIT, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_DYN_COLLISION, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_STC_COLLISION, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_MESH_PACKETS, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_DRAINS, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_PACKET_DATA, true);
	sdk->setParameter(NX_VISUALIZE_CLOTH_MESH, true);
	sdk->setParameter(NX_VISUALIZE_CLOTH_COLLISIONS, true);
	sdk->setParameter(NX_VISUALIZE_CLOTH_SELFCOLLISIONS, true);
	sdk->setParameter(NX_VISUALIZE_CLOTH_WORKPACKETS, true);
	sdk->setParameter(NX_VISUALIZE_CLOTH_SLEEP, true);
	sdk->setParameter(NX_VISUALIZE_CLOTH_TEARABLE_VERTICES, true);
	sdk->setParameter(NX_VISUALIZE_CLOTH_TEARING, true);
	sdk->setParameter(NX_VISUALIZE_CLOTH_ATTACHMENT, true);
	sdk->setParameter(NX_VISUALIZE_SOFTBODY_MESH, true);
	sdk->setParameter(NX_VISUALIZE_SOFTBODY_COLLISIONS, true);
	sdk->setParameter(NX_VISUALIZE_SOFTBODY_WORKPACKETS, true);
	sdk->setParameter(NX_VISUALIZE_SOFTBODY_SLEEP, true);
	sdk->setParameter(NX_VISUALIZE_SOFTBODY_TEARABLE_VERTICES, true);
	sdk->setParameter(NX_VISUALIZE_SOFTBODY_TEARING, true);
	sdk->setParameter(NX_VISUALIZE_SOFTBODY_ATTACHMENT, true); 


	mDebugRenderer = new DebugRenderer(s);
}

///////////////////////////////////////////////////////////////////////

void World::destroyDebugRenderer() {
	NxDelete(mDebugRenderer);
}

#endif

///////////////////////////////////////////////////////////////////////

void World::setCCD(bool enabled, NxReal epsilon) {
	mDriver->getSDK()->setParameter(NX_CONTINUOUS_CD, enabled);
	if (enabled)
		mDriver->getSDK()->setParameter(NX_CCD_EPSILON, epsilon);
}

///////////////////////////////////////////////////////////////////////

bool World::getCCD() {
	return !!mDriver->getSDK()->getParameter(NX_CONTINUOUS_CD);
}

///////////////////////////////////////////////////////////////////////

NxReal World::getCCDEpsilon() {
	return mDriver->getSDK()->getParameter(NX_CCD_EPSILON);
}

///////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
