#include "CaelumPrecompiled.h"
#include "Sun.h"
#include "SimpleSunPositionModel.h"
#include "EarthSunPositionModel.h"

namespace caelum {

const Ogre::String Sun::SUN_MATERIAL_NAME = "CaelumSunMaterial";

Sun::Sun (Ogre::SceneManager *sceneMgr): mScene(sceneMgr) {
	mSunSphereColour = Ogre::ColourValue::White;
	mSunLightColour = Ogre::ColourValue::White;
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
	mSunEntity->setRenderQueueGroup (CAELUM_RENDER_QUEUE_SUN);
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
    // This calls setFarRadius
    CameraBoundElement::notifyCameraChanged(cam);

    // Set sun position.
    Ogre::Real sunRadius = -mRadius * Ogre::Math::Tan (Ogre::Degree (0.01));
	mSunNode->setPosition (cam->getRealPosition () - mSunDirection * (mRadius + sunRadius));

    // Set sun scaling.
    float factor = 2 - mSunSphereColour.b / 3;
    float scale = factor * (mRadius + sunRadius) * Ogre::Math::Tan (Ogre::Degree (0.01));
    mSunNode->setScale (Ogre::Vector3::UNIT_SCALE * scale);
}

void Sun::setFarRadius (Ogre::Real radius) {
    CameraBoundElement::setFarRadius(radius);
	mRadius = radius;
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

void Sun::setSunSphereColour (const Ogre::ColourValue &colour) {
	// Store this last colour
	mSunSphereColour = colour;

	// Set sun material colour.
	mSunMaterial->setSelfIllumination (colour);
}

Ogre::ColourValue Sun::getSunSphereColour () {
	return mSunSphereColour;
}

void Sun::setSunLightColour (const Ogre::ColourValue &colour) {
	// Store this last colour
	mSunLightColour = colour;

	// Set light colours.
	mMainLight->setDiffuseColour (colour * mDiffuseMultiplier);
	mMainLight->setSpecularColour (colour * mSpecularMultiplier);
	if (isManagingAmbientLight()) {
		mScene->setAmbientLight(colour * mAmbientMultiplier);
	}
}

Ogre::ColourValue Sun::getSunLightColour () {
	return mSunLightColour;
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
		mat = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton ().create (SUN_MATERIAL_NAME, RESOURCE_GROUP_NAME));
		mat->setReceiveShadows (false);
		LOG ("\t\tMaterial [OK]");
		Ogre::Pass *pass = mat->getTechnique (0)->getPass (0);
        pass->setSceneBlending (Ogre::SBT_TRANSPARENT_COLOUR);
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
