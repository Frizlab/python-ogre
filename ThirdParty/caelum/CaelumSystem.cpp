#include "CaelumPrecompiled.h"
#include "CaelumSystem.h"

namespace caelum {

// Default resource group name is Caelum
Ogre::String RESOURCE_GROUP_NAME = "Caelum";
				
CaelumSystem::CaelumSystem
(
    Ogre::Root *root, 
	Ogre::SceneManager *sceneMgr, 
	CaelumComponent componentsToCreate/* = CAELUM_COMPONENTS_DEFAULT*/, 
	bool manageResGroup/* = true*/, 
	const Ogre::String &resGroupName/* = RESOURCE_GROUP_NAME*/
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
	mManageSceneFog = false;
	mGlobalFogDensityMultiplier = 1;
	mSceneFogDensityMultiplier = 1;
	mGroundFogDensityMultiplier = 1;

	LOG ("System attributes set up.");

	// Create default components; as requested.
    if (componentsToCreate & CAELUM_COMPONENT_SKY_COLOUR_MODEL) {
        this->setSkyColourModel (new SkyColourModel ());
    }
    if (componentsToCreate & CAELUM_COMPONENT_SKY_DOME) {
		this->setSkyDome (new SkyDome (mSceneMgr));
    }
    if (componentsToCreate & CAELUM_COMPONENT_SUN) {
		this->setSun (new Sun (mSceneMgr));
    }
    if (componentsToCreate & CAELUM_COMPONENT_STARFIELD) {
		this->setStarfield (new Starfield (mSceneMgr));
    }
    if (componentsToCreate & CAELUM_COMPONENT_CLOUDS) {
		this->setClouds (new LayeredClouds (mSceneMgr));
    }
    if (componentsToCreate & CAELUM_COMPONENT_GROUND_FOG) {
		this->setGroundFog (new GroundFog (mSceneMgr));
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
    setGroundFog (0);
    setSkyColourModel (0);

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

	if (getGroundFog ()) {
		getGroundFog ()->notifyCameraChanged (cam);
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
           
        // Update sun.
		if (getSun ()) {
			getSun ()->update (relDayTime);
		}

        // Update starfield
		if (getStarfield ()) {
			getStarfield ()->update (relDayTime);
		}

        // Update skydome.
		if (getSkyDome ()) {
			getSkyDome ()->setSunDirection (sunDir);
		}

        // Init fog properties from sky colour model.
        double fogDensity;
        Ogre::ColourValue fogColour;
        if (getSkyColourModel ()) {
            fogDensity = getSkyColourModel ()->getFogDensity (relDayTime, sunDir);
            fogDensity *= mGlobalFogDensityMultiplier;
            fogColour = mSkyColourModel->getFogColour (relDayTime, sunDir);
        } else {
            fogDensity = 0;
            fogColour = Ogre::ColourValue::Black;
        }

        // Update scene fog.
        if (getManageSceneFog ()) {
			mSceneMgr->setFog (Ogre::FOG_EXP2,
                    fogColour * 0.7,
                    fogDensity * mSceneFogDensityMultiplier);
        }

        // Update ground fog.
        if (getGroundFog ()) {
			getGroundFog ()->setColour (fogColour);
		    getGroundFog ()->setDensity (fogDensity * mGroundFogDensityMultiplier);
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

    // Update clouds
	// This has to be moved out of here, but Universal clock doesn't calculate deltaT
    // Maybe it's better to update everything every frame?
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

void CaelumSystem::setManageSceneFog (bool value) {
	mManageSceneFog = value;
    // Prevent having some stale values around.
    if (!value) {
        mSceneMgr->setFog (Ogre::FOG_NONE);
    }
}

bool CaelumSystem::getManageSceneFog () const {
	return mManageSceneFog;
}

void CaelumSystem::setSceneFogDensityMultiplier (double value) {
    mSceneFogDensityMultiplier = value;
}

double  CaelumSystem::getSceneFogDensityMultiplier () const {
    return mSceneFogDensityMultiplier;
}

void CaelumSystem::setGroundFogDensityMultiplier (double value) {
    mGroundFogDensityMultiplier = value;
}

double  CaelumSystem::getGroundFogDensityMultiplier () const {
    return mGroundFogDensityMultiplier;
}

void CaelumSystem::setGlobalFogDensityMultiplier (double value) {
    mGlobalFogDensityMultiplier = value;
}

double  CaelumSystem::getGlobalFogDensityMultiplier () const {
    return mGlobalFogDensityMultiplier;
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
