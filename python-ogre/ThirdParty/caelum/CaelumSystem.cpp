#include "CaelumPrecompiled.h"
#include "CaelumSystem.h"

namespace caelum {

Ogre::String CaelumSystem::RESOURCE_GROUP_NAME = "Caelum";
				
CaelumSystem::CaelumSystem (Ogre::Root *root, 
															Ogre::SceneManager *sceneMgr, 
															bool manageResGroup, 
															const Ogre::String &resGroupName, 
															bool createSkyDome, bool createSun, bool createStarfield) {
	LOG ("Initialising Caelum system...");
	mOgreRoot = root;
	mSceneMgr = sceneMgr;

	mCleanup = false;

	RESOURCE_GROUP_NAME = resGroupName;

	// Create resource group
	if (manageResGroup) {
		// Search for the resource group
		Ogre::StringVector resGroups = Ogre::ResourceGroupManager::getSingleton ().getResourceGroups ();
		Ogre::StringVector::iterator it = resGroups.begin (), iend = resGroups.end ();
		while (it != iend && *it != resGroupName)
			++it;

		if (it == iend) {
			Ogre::ResourceGroupManager::getSingleton ().createResourceGroup (RESOURCE_GROUP_NAME);
			mManageResourceGroup = true;
			LOG (Ogre::String ("Created Caelum resource group (") + RESOURCE_GROUP_NAME + ")");
		}
		else {
			mManageResourceGroup = false;
		}
	}
	else {
		mManageResourceGroup = false;
	}

	// Clock
	mUniversalClock = new UniversalClock ();

	// Set-up attributes
	mManageFog = false;

	mSkyDome = 0;
	mSun = 0;
	mStarfield = 0;
	LOG ("System attributes set up.");

	// Create basic elements
	if (createSkyDome)
		this->createSkyDome ();
	if (createSun)
		this->createSun ();
	if (createStarfield)
		this->createStarfield ();

	// Auto-register itself as a frame listener
	mOgreRoot->addFrameListener (this);

	LOG ("DONE");
}

CaelumSystem::~CaelumSystem () {
	LOG ("Caelum system destroyed.");
}

void CaelumSystem::shutdown (const bool cleanup) {
	LOG ("Shutting down Caelum system...");
	// Remove itself as a frame listener
	mOgreRoot->removeFrameListener (this);

	// Unregister all the caelum listeners
	mListeners.clear ();

	// Destroy the elements
	destroySkyDome ();
	destroySun ();
	destroyStarfield ();

	// Destroy the clock
	if (mUniversalClock) {
		delete mUniversalClock;
		mUniversalClock = 0;
	}

	// Remove resource group
	if (mManageResourceGroup) {
		Ogre::ResourceGroupManager::getSingleton ().destroyResourceGroup (RESOURCE_GROUP_NAME);
		LOG ("Destroyed Caelum resource group");
	}

	if (cleanup)
		delete this;
	else
		mCleanup = true;
}

void CaelumSystem::addListener (CaelumListener *listener) {
	mListeners.insert (listener);
}

void CaelumSystem::removeListener (CaelumListener *listener) {
	mListeners.erase (listener);
}

void CaelumSystem::preViewportUpdate (const Ogre::RenderTargetViewportEvent &e) {
	Ogre::Camera *cam = e.source->getCamera ();
	
	if (mSkyDome) {
		mSkyDome->notifyCameraChanged (cam);
	}

	if (mSun) {
		mSun->notifyCameraChanged (cam);
	}

	if (mStarfield) {
		mStarfield->notifyCameraChanged (cam);
	}
}

UniversalClock *CaelumSystem::getUniversalClock () const {
	return mUniversalClock;
}
				
bool CaelumSystem::frameStarted (const Ogre::FrameEvent &e) {
	// First of all, check if a cleanup has been requested or not, and if so, self-destruction
	if (mCleanup) {
		delete this;
		return true;
	}

	if (mUniversalClock->update (e.timeSinceLastFrame)) {
		// Call every listener before doing anything
		if (!fireStartedEvent (e))
			return false;

		if (mSun) {
			mSun->update (mUniversalClock->getCurrentRelativeDayTime ());
		}

		if (mStarfield) {
			mStarfield->update (mUniversalClock->getCurrentRelativeDayTime ());
		}

		if (mSkyDome) {
			mSkyDome->updateSkyDomeMaterialTime (mSkyColourModel, mUniversalClock->getCurrentRelativeDayTime (), mSun);

			mSkyDome->setSunDirection (mSun->getSunDirection ());
		}

		if (mManageFog) {
			// TODO: Fog stuff here!!!
			if (mSkyColourModel) {
				mSceneMgr->setFog (Ogre::FOG_EXP2,
													mSkyColourModel->getFogColour (mUniversalClock->getCurrentRelativeDayTime (), mSun ? mSun->getSunDirection() : Ogre::Vector3::UNIT_Y) * 0.7,
													mSkyColourModel->getFogDensity (mUniversalClock->getCurrentRelativeDayTime (), mSun ? mSun->getSunDirection() : Ogre::Vector3::UNIT_Y));
			}
		}

		if (mSun) {
			if (mSkyColourModel)
				mSun->setSunColour (mSkyColourModel->getSunColour (mUniversalClock->getCurrentRelativeDayTime (), mSun->getSunDirection ()));
			else
				mSun->setSunColour (mSceneMgr->getFogColour ());
		}

		// Call every listener before quiting
		if (!fireFinishedEvent (e))
			return false;
	}

	return true;
}

SkyDome *CaelumSystem::createSkyDome () {
	if (!mSkyDome) {
		mSkyDome = new SkyDome (mSceneMgr);
		LOG ("Sky Dome created.");
	}

	return mSkyDome;
}

SkyDome *CaelumSystem::getSkyDome () const {
	return mSkyDome;
}

void CaelumSystem::destroySkyDome () {
	if (mSkyDome) {
		delete mSkyDome;
		mSkyDome = 0;
		LOG ("Sky Dome destroyed.");
	}
}

Sun *CaelumSystem::createSun () {
	if (!mSun) {
		mSun = new Sun (mSceneMgr);
		LOG ("Sun created.");
	}

	return mSun;
}

Sun *CaelumSystem::getSun () const {
	return mSun;
}

void CaelumSystem::destroySun () {
	if (mSun) {
		delete mSun;
		mSun = 0;
		LOG ("Sun destroyed.");
	}
}

Starfield *CaelumSystem::createStarfield (const Ogre::String &mapName) {
	if (!mStarfield) {
		mStarfield = new Starfield (mSceneMgr);
		LOG ("Starfield created.");
	}

	mStarfield->updateMaterial (mapName);

	return mStarfield;
}

Starfield *CaelumSystem::getStarfield () const {
	return mStarfield;
}

void CaelumSystem::destroyStarfield () {
	if (mStarfield) {
		delete mStarfield;
		mStarfield = 0;
		LOG ("Starfield destroyed.");
	}
}

void CaelumSystem::setSkyColourModel (SkyColourModel *model) {
	mSkyColourModel = model;
	if (mSkyDome) {
		Ogre::TextureUnitState *temp = mSkyDome->getTextureUnitState ();
		if (temp)
			mSkyColourModel->setSkyGradientsTextureUnitState (temp);
	}
}

void CaelumSystem::setManageFog (bool manage) {
	mManageFog = manage;
}

bool CaelumSystem::isFogManaged () const {
	return mManageFog;
}

bool CaelumSystem::fireStartedEvent (const Ogre::FrameEvent &e) {
	std::set<CaelumListener *>::iterator it, iend = mListeners.end ();
	bool flag = true;

	it = mListeners.begin ();

	while (it != iend && flag) {
		flag &= (*it)->caelumStarted (e, this);

		++it;
	}

	return flag;
}

bool CaelumSystem::fireFinishedEvent (const Ogre::FrameEvent &e) {
	std::set<CaelumListener *>::iterator it, iend = mListeners.end ();
	bool flag = true;

	it = mListeners.begin ();

	while (it != iend && flag) {
		flag &= (*it)->caelumFinished (e, this);

		++it;
	}

	return flag;
}

} // namespace caelum
