#include "CaelumPrecompiled.h"
#include "CaelumSystem.h"

namespace caelum {

// Default resource group name is Caelum
Ogre::String RESOURCE_GROUP_NAME = "Caelum";
				
CaelumSystem::CaelumSystem
(
    Ogre::Root *root, 
    Ogre::SceneManager *sceneMgr, 
	bool createSkyColourModel,
    bool createSkyDome,
    bool createSun,
    bool createStarfield,
    bool createClouds,
    bool manageResGroup, 
    const Ogre::String &resGroupName
):
    mCleanup(false)
{
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
        while (it != iend && *it != resGroupName) {
			++it;
        }

		if (it == iend) {
			Ogre::ResourceGroupManager::getSingleton ().createResourceGroup (RESOURCE_GROUP_NAME);
			mManageResourceGroup = true;
			LOG (Ogre::String ("Created Caelum resource group (") + RESOURCE_GROUP_NAME + ")");
		} else {
			mManageResourceGroup = false;
		}
	} else {
		mManageResourceGroup = false;
	}

	// Clock
	mUniversalClock = new UniversalClock ();

	// Set-up attributes
	mManageFog = false;

	LOG ("System attributes set up.");

    assert(getSun() == 0);
    assert(getClouds() == 0);
	// Create basic elements
    if (createSkyColourModel) {
        this->setSkyColourModel (new SkyColourModel ());
    }
    if (createSkyDome) {
		this->setSkyDome (new SkyDome (mSceneMgr));
    }
    if (createSun) {
		this->setSun (new Sun (mSceneMgr));
    }
    if (createClouds) {
		this->setClouds (new LayeredClouds (mSceneMgr));
    }
    if (createStarfield) {
		this->setStarfield (new Starfield (mSceneMgr));
    }

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
	setSkyDome (0);
	setSun (0);
	setStarfield (0);
	setClouds (0);

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

    if (cleanup) {
		delete this;
    } else {
		mCleanup = true;
    }
}

void CaelumSystem::addListener (CaelumListener *listener) {
	mListeners.insert (listener);
}

void CaelumSystem::removeListener (CaelumListener *listener) {
	mListeners.erase (listener);
}

void CaelumSystem::preViewportUpdate (const Ogre::RenderTargetViewportEvent &e) {
	Ogre::Camera *cam = e.source->getCamera ();
	
	if (getSkyDome ()) {
		getSkyDome ()->notifyCameraChanged (cam);
	}

	if (getSun ()) {
		getSun ()->notifyCameraChanged (cam);
	}

	if (getStarfield ()) {
		getStarfield ()->notifyCameraChanged (cam);
	}

	if (getClouds ()) {
		getClouds ()->notifyCameraChanged (cam);
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

        // Get relative time of the day.
        float relDayTime = mUniversalClock->getCurrentRelativeDayTime ();

        // Get the sun's direction.
        Ogre::Vector3 sunDir;
        if (getSun ()) {
            sunDir = getSun ()->getSunDirection();
        } else {
            sunDir = Ogre::Vector3::UNIT_Y;
        }
           
		if (getSun ()) {
			getSun ()->update (relDayTime);
		}

		if (getStarfield ()) {
			getStarfield ()->update (relDayTime);
		}

		if (getSkyDome ()) {
			getSkyDome ()->setSunDirection (sunDir);
		}

		if (mManageFog) {
			// TODO: Fog stuff here!!!
			if (getSkyColourModel ()) {
				mSceneMgr->setFog (Ogre::FOG_EXP2,
				        mSkyColourModel->getFogColour (relDayTime, sunDir) * 0.7,
				        mSkyColourModel->getFogDensity (relDayTime, sunDir));
			}
		}

        // Set sun colours.
		if (getSun () && getSkyColourModel ()) {
			mSun->setSunSphereColour (getSkyColourModel ()->getSunSphereColour (relDayTime, sunDir));
			mSun->setSunLightColour (getSkyColourModel ()->getSunLightColour (relDayTime, sunDir));
		}

		// Call every listener before quiting
        if (!fireFinishedEvent (e)) {
			return false;
        }
	}

	// This has to be moved out of here, but Universal clock doesn't calculate deltaT
    if (getClouds()) {
		double time = mUniversalClock->getCurrentRelativeDayTime ();
		Ogre::Vector3 sunDir = getSun() ? mSun->getSunDirection() : Ogre::Vector3::UNIT_Y;
		mClouds->update (e.timeSinceLastFrame * mUniversalClock->getTimeScale(),
				getSun () ? mSun->getSunDirection() : Ogre::Vector3::UNIT_Y,
				getSun () ? mSun->getSunLightColour() : Ogre::ColourValue::White,
				getSkyColourModel() ? mSkyColourModel->getFogColour (time, sunDir) : Ogre::ColourValue::Black);
	}

	return true;
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
