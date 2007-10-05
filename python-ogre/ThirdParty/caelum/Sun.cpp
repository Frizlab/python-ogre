#include "CaelumPrecompiled.h"
#include "Sun.h"
#include "CaelumSystem.h"
#include "SimpleSunPositionModel.h"
#include "EarthSunPositionModel.h"

namespace caelum {

const Ogre::String Sun::SUN_MATERIAL_NAME = "CaelumSunMaterial";

Sun::Sun (Ogre::SceneManager *sceneMgr): mScene(sceneMgr) {
	mSunColour = Ogre::ColourValue::White;
	mAutoRadius = true;
	mSunPositionModel = 0;

	mDiffuseMultiplier = Ogre::ColourValue(1, 1, 0.9);
	mSpecularMultiplier = Ogre::ColourValue(1, 1, 1);
	mAmbientMultiplier = Ogre::ColourValue(0.2, 0.2, 0.2);
	mManageAmbientLight = false;

	mMainLight = sceneMgr->createLight ("CaelumSun");
	mMainLight->setType (Ogre::Light::LT_DIRECTIONAL);
	// HDR power scale, REALLY bright:
	mMainLight->setPowerScale (10);

	createSunMaterial ();

	mSunEntity = sceneMgr->createEntity ("CaelumSun", "sphere.mesh");
	mSunEntity->setMaterialName (SUN_MATERIAL_NAME);
	mSunEntity->setCastShadows (false);
	mSunEntity->setRenderQueueGroup (Ogre::RENDER_QUEUE_SKIES_EARLY + 3);
	mSunNode = sceneMgr->getRootSceneNode ()->createChildSceneNode ();
	mSunNode->attachObject (mSunEntity);
}

Sun::~Sun () {
	if (mSunNode) {
		mSunNode->detachObject (mSunEntity);
		static_cast<Ogre::SceneNode *>(mSunNode->getParent ())->removeAndDestroyChild (mSunNode->getName ());
		mSunNode = 0;
	}
	if (mSunEntity) {
		mSunEntity->_getManager ()->destroyEntity (mSunEntity);
		mSunEntity = 0;
	}

	destroySunMaterial ();

	if (mSunPositionModel) {
		delete mSunPositionModel;
		mSunPositionModel = 0;
	}

	if (mMainLight) {
		mMainLight->_getManager ()->destroyLight (mMainLight);
		mMainLight = 0;
	}
}

void Sun::notifyCameraChanged (Ogre::Camera *cam) {
	float sunRadius0;
	if (mAutoRadius) {
		if (cam->getFarClipDistance () > 0) {
			mRadius = (cam->getFarClipDistance () - CAMERA_DISTANCE_MODIFIER) * 0.5;
			sunRadius0 = -1;
		}
		else {
			mRadius = (cam->getNearClipDistance () + CAMERA_DISTANCE_MODIFIER) * 2;
			sunRadius0 = 1;
		}
	}
	sunRadius0 *= mRadius * Ogre::Math::Tan (Ogre::Degree (0.01));
	mSunNode->setPosition (cam->getRealPosition () - mSunDirection * (mRadius + sunRadius0));
	mSunNode->setScale (Ogre::Vector3::UNIT_SCALE * (mRadius + sunRadius0) * Ogre::Math::Tan (Ogre::Degree (0.01)));
}

void Sun::setFarRadius (float radius) {
	if (radius > 0) {
		mRadius = radius;
		mAutoRadius = false;
	}
	else {
		mAutoRadius = true;
	}
}

void Sun::update (const float time) {
	Ogre::Vector3 dir = Ogre::Vector3::NEGATIVE_UNIT_Y;

	if (mSunPositionModel) {
		dir = mSunPositionModel->update (time);
	}

	// Update the main light direction
	if (mMainLight != 0) {
		mMainLight->setDirection (dir);
	}

	// Store the latest sun direction.
	mSunDirection = dir;
}

SunPositionModel *Sun::setSunPositionModel (SunPositionModel *model) {
	SunPositionModel *temp = mSunPositionModel;

	mSunPositionModel = model;

	return temp;
}

SunPositionModel *Sun::getSunPositionModel () const {
	return mSunPositionModel;
}

Ogre::Vector3 Sun::getSunDirection () const {
	return mSunDirection;
}

void Sun::setSunColour (const Ogre::ColourValue &colour) {
	// Store this last colour
	mSunColour = colour;

	// Handle light colors.
	mMainLight->setDiffuseColour (colour * mDiffuseMultiplier);
	mMainLight->setSpecularColour (colour * mSpecularMultiplier);
	if (isManagingAmbientLight()) {
		mScene->setAmbientLight(colour * mAmbientMultiplier);
	}

	// Check sun material.
	mSunMaterial->setSelfIllumination (colour + Ogre::ColourValue (.5, .4, .2));
}

Ogre::ColourValue Sun::getSunColour () {
	return mSunColour;
}

void Sun::setDiffuseMultiplier (const Ogre::ColourValue &diffuse) {
	mDiffuseMultiplier = diffuse;
}

Ogre::ColourValue Sun::getDiffuseMultiplier () {
	return mDiffuseMultiplier;
}

void Sun::setSpecularMultiplier (const Ogre::ColourValue &specular) {
	mSpecularMultiplier = specular;
}

Ogre::ColourValue Sun::getSpecularMultiplier () {
	return mSpecularMultiplier;
}

void Sun::setAmbientMultiplier (const Ogre::ColourValue &ambient) {
	mAmbientMultiplier = ambient;
}

Ogre::ColourValue Sun::getAmbientMultiplier () {
	return mAmbientMultiplier;
}

void Sun::setManageAmbientLight (bool manage) {
	mManageAmbientLight = manage;
}

bool Sun::isManagingAmbientLight () {
	return mManageAmbientLight;
}

void Sun::createSunMaterial () {
	Ogre::MaterialPtr mat;

	LOG ("Generating sun material...");
	if (!Ogre::MaterialManager::getSingleton ().resourceExists (SUN_MATERIAL_NAME)) {
		LOG ("\tMaterial not found; creating...");
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().create (SUN_MATERIAL_NAME, CaelumSystem::RESOURCE_GROUP_NAME));
		mat->setReceiveShadows (false);
		LOG ("\t\tMaterial [OK]");
		Ogre::Pass *pass = mat->getTechnique (0)->getPass (0);
		pass->setDepthCheckEnabled (false);
		pass->setDepthWriteEnabled (false);
//		pass->setLightingEnabled (false);
		pass->setFog (true);
		pass->setAmbient (Ogre::ColourValue::Black);
		pass->setDiffuse (Ogre::ColourValue::Black);
		pass->setSpecular (Ogre::ColourValue::Black);
		LOG ("\t\tPass [OK]");
		mat->load ();
		LOG ("\tDONE");
	}
	else {
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().getByName (SUN_MATERIAL_NAME));
	}
	LOG ("DONE");

	mSunMaterial = mat;
}

void Sun::destroySunMaterial () {
	LOG ("Removing sun material...");
	if (Ogre::MaterialManager::getSingleton ().resourceExists (SUN_MATERIAL_NAME)) {
		Ogre::MaterialManager::getSingleton ().remove (SUN_MATERIAL_NAME);
	}
	mSunMaterial.setNull ();
	LOG ("DONE");
}

} // namespace caelum
