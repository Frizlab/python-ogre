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
#include "NxOgreCharacterController.h"	// For: Setting up character controller
#include "NxOgreDebugRenderer.h"		// For: Times when it's really needed.
#include "NxOgreSerialiser.h"			// For: Serialisation
#include "time.h"						// For: Making sure NxGenerateID is random.


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

#if (NX_USE_CHARACTER_API == 1)
	mCharacterController = new CharacterController();
#endif

#if (NX_USE_DEBUG_RENDERER_API == 1)
	mDebugRenderer = 0;
#endif

//	mSerialiser = new Serialiser(this);

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

	#if (NX_USE_CHARACTER_API == 1)
		NxDelete(mCharacterController);
	#endif


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

Scene* World::createScene(const NxString& name, Ogre::SceneManager* s,SceneParams p) {

	Scene* scene;

	if (mScenes.has(name)) {
		NxString newname = NxCreateID(mScenes.count(), name);
		NxThrow_Conflict("Tried to create a scene with a duplicate name '" + name + "', name is now '" + newname + "'.");
		scene = new Scene(NxCreateID(mScenes.count(), newname), s, this, p);
	}
	else {
		scene = new Scene(name, s, this, p);
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
		scene->render(time);
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
	sdk->setParameter(NX_VISUALIZE_BODY_AXES,true);
	sdk->setParameter(NX_VISUALIZE_CONTACT_POINT,true);
	sdk->setParameter(NX_VISUALIZE_JOINT_LOCAL_AXES,true);
	sdk->setParameter(NX_VISUALIZE_COLLISION_SHAPES,true);
	sdk->setParameter(NX_VISUALIZE_FLUID_DRAINS, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_EMITTERS, true);
	sdk->setParameter(NX_VISUALIZE_FLUID_POSITION, true);
	sdk->setParameter(NX_VISUALIZE_CLOTH_MESH, true);
	sdk->setParameter(NX_VISUALIZE_SOFTBODY_MESH, true);
	sdk->setParameter(NX_VISUALIZE_SOFTBODY_WORKPACKETS, true);
	sdk->setParameter(NX_VISUALIZE_SOFTBODY_COLLISIONS, true);
	sdk->setParameter(NX_VISUALIZE_FORCE_FIELDS, true);

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
	return mDriver->getSDK()->getParameter(NX_CONTINUOUS_CD);
}

///////////////////////////////////////////////////////////////////////

NxReal World::getCCDEpsilon() {
	return mDriver->getSDK()->getParameter(NX_CCD_EPSILON);
}

///////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
