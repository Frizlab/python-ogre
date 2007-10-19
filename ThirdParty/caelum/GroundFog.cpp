#include "CaelumPrecompiled.h"
#include "GroundFog.h"
#include "CaelumExceptions.h"

namespace caelum {
	const Ogre::String GroundFog::DEFAULT_PASS_NAME = "CaelumGroundFog";

	GroundFog::GroundFog(
			Ogre::SceneManager *scene,
			const Ogre::String &domeMaterialName,
			const Ogre::String &domeEntityName):
			mScene(scene)
	{
		// Create dome entity, using a prefab sphere.
        // The prefab sphere has a radius of 50 units.
        // If this changes in future version of ogre this might break.
        mDomeEntity = mScene->createEntity(domeEntityName, Ogre::SceneManager::PT_SPHERE);
        mDomeEntity->setMaterialName(domeMaterialName);
        mDomeEntity->setCastShadows(false);
        mDomeEntity->setRenderQueueGroup (CAELUM_RENDER_QUEUE_GROUND_FOG);

		mDomeNode = mScene->getRootSceneNode ()->createChildSceneNode ();
		mDomeNode->attachObject (mDomeEntity);

		// Maybe it would be better to create the material at runtime instead.
		// More consistent with the rest of Caelum.
		mDomeMaterial = static_cast<Ogre::MaterialPtr>(
				Ogre::MaterialManager::getSingleton().getByName(domeMaterialName));
		mDomeMaterial->load();
		if (mDomeMaterial->getBestTechnique() == 0) {
			throw new UnsupportedException (0, "GroundFog dome material not supported.",
					"GroundFog", "GroundFog.cpp", -1);
		}
        mDomeMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters()->setIgnoreMissingParams(true);
        mDomeMaterial->getBestTechnique()->getPass(0)->getVertexProgramParameters()->setIgnoreMissingParams(true);

		// Initialize default fog parameters
		mDensity = 0.1;
		mVerticalDecay = 0.2;
		mGroundLevel = 5;
        mFogColour = Ogre::ColourValue::Black;

        forceUpdate();
	}

	GroundFog::~GroundFog() {
		mDomeNode->detachObject(mDomeEntity);
		mScene->destroyEntity(mDomeEntity);
		mDomeEntity = 0;
		static_cast<Ogre::SceneNode*>(mDomeNode->getParent())->
				removeAndDestroyChild(mDomeNode->getName());
		mDomeNode = 0;
		mScene = 0;
	}

	GroundFog::PassSet& GroundFog::getPasses() {
		return mPasses;
	}

	const GroundFog::PassSet& GroundFog::getPasses() const {
		return mPasses;
	}

	void GroundFog::findFogPassesByName (const Ogre::String& passName) {
		Ogre::MaterialManager *matManager = Ogre::MaterialManager::getSingletonPtr();
		Ogre::MaterialManager::ResourceMapIterator matIt = matManager->getResourceIterator();
		while (matIt.hasMoreElements()) {
			Ogre::MaterialPtr mat = matIt.getNext();
			Ogre::Material::TechniqueIterator techIt = mat->getTechniqueIterator();
			while (techIt.hasMoreElements()) {
				Ogre::Technique *tech = techIt.getNext();
				Ogre::Technique::PassIterator passIt = tech->getPassIterator();
				while (passIt.hasMoreElements()) {
					Ogre::Pass *pass = passIt.getNext();
					if (pass->getName() == passName) {
						mPasses.insert(pass);
					}
				}
			}
		}
		forceUpdate();
	}

	void GroundFog::setDensity (Ogre::Real density) {
		if (Ogre::Math::Abs(mDensity - density) > 0.000001) {
			for (PassSet::iterator it = mPasses.begin(); it != mPasses.end(); ++it) {
				Ogre::GpuProgramParametersSharedPtr params = (*it)->getFragmentProgramParameters();
				params->setNamedConstant("fogDensity", density);
			}
			mDensity = density;
		}
		updateSkyFogging();
	}

	Ogre::Real GroundFog::getDensity () {
		return mDensity;
	}

	void GroundFog::setColour (const Ogre::ColourValue &colour) {
		for (PassSet::iterator it = mPasses.begin(); it != mPasses.end(); ++it) {
			Ogre::GpuProgramParametersSharedPtr params = (*it)->getFragmentProgramParameters();
			params->setNamedConstant("fogColour", colour);
		}
		mFogColour = colour;
		updateSkyFogging();
	}

	Ogre::ColourValue GroundFog::getColour () {
        return mFogColour;
	}

	void GroundFog::setVerticalDecay (Ogre::Real verticalDecay) {
		if (Ogre::Math::Abs(mVerticalDecay - verticalDecay) > 0.000001) {
			for (PassSet::iterator it = mPasses.begin(); it != mPasses.end(); ++it) {
				Ogre::GpuProgramParametersSharedPtr params = (*it)->getFragmentProgramParameters();
				params->setNamedConstant("fogVerticalDecay", verticalDecay);
			}
			mVerticalDecay = verticalDecay;
		}
		updateSkyFogging();
	}

	Ogre::Real GroundFog::getVerticalDecay () {
		return mVerticalDecay;
	}

	void GroundFog::setGroundLevel (Ogre::Real groundLevel) {
		if (Ogre::Math::Abs(mGroundLevel - groundLevel) > 0.000001) {
			for (PassSet::iterator it = mPasses.begin(); it != mPasses.end(); ++it) {
				Ogre::GpuProgramParametersSharedPtr params = (*it)->getFragmentProgramParameters();
				params->setNamedConstant("fogGroundLevel", groundLevel);
			}
			mGroundLevel = groundLevel;
		}
		updateSkyFogging();
	}

	Ogre::Real GroundFog::getGroundLevel () {
		return mGroundLevel;
	}

	void GroundFog::forceUpdate () {
		for (PassSet::iterator it = mPasses.begin(); it != mPasses.end(); ++it) {
			Ogre::GpuProgramParametersSharedPtr params = (*it)->getFragmentProgramParameters();
			params->setNamedConstant("fogDensity", mDensity);
			params->setNamedConstant("fogVerticalDecay", mVerticalDecay);
			params->setNamedConstant("fogGroundLevel", mGroundLevel);
			params->setNamedConstant("fogColour", mFogColour);
		}
		updateSkyFogging();
	}

	void GroundFog::notifyCameraChanged (Ogre::Camera *cam)
	{
		// Move and sky to fill the sky.
        // Adjust for radius 50.
		mDomeNode->setPosition (cam->getRealPosition ());
		if (cam->getFarClipDistance () > 0) {
			mDomeNode->setScale (Ogre::Vector3::UNIT_SCALE * (1 / 50.0) *
                    (cam->getFarClipDistance () - CAMERA_DISTANCE_MODIFIER));
		} else {
			mDomeNode->setScale (Ogre::Vector3::UNIT_SCALE * (1 / 50.0) *
                    (cam->getNearClipDistance () + CAMERA_DISTANCE_MODIFIER));
		}

		// Send camera height to shader.
		Ogre::GpuProgramParametersSharedPtr params = 
			mDomeMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
		params->setNamedConstant("cameraHeight", cam->getDerivedPosition().y);
	}

	void GroundFog::updateSkyFogging() {
		Ogre::GpuProgramParametersSharedPtr params = 
			mDomeMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
		params->setNamedConstant("fogDensity", getDensity());
		params->setNamedConstant("fogColour", getColour());
		params->setNamedConstant("fogVerticalDecay", getVerticalDecay());
		params->setNamedConstant("fogGroundLevel", getGroundLevel());
	}
}