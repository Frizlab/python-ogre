/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2008 Caelum team. See Contributors.txt for details.

Caelum is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Caelum is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Caelum. If not, see <http://www.gnu.org/licenses/>.
*/

#include "CaelumPrecompiled.h"
#include "CaelumSystem.h"
#include "CloudSystem.h"
#include "Moon.h"
#include "PointStarfield.h"
#include "UniversalClock.h"
#include "ImageStarfield.h"
#include "GroundFog.h"
#include "SkyDome.h"
#include "PrecipitationController.h"
#include "Sun.h"
#include "ImageHelper.h"
#include "FlatCloudLayer.h"
#include "Astronomy.h"

namespace Caelum
{
    const String CaelumSystem::DEFAULT_SKY_GRADIENTS_IMAGE = "EarthClearSky2.png";
    const String CaelumSystem::DEFAULT_SUN_COLOURS_IMAGE = "SunGradient.png";

    CaelumSystem::CaelumSystem
    (
        Ogre::Root *root, 
        Ogre::SceneManager *sceneMgr,
        CaelumComponent componentsToCreate/* = CAELUM_COMPONENTS_DEFAULT*/
    ):
        mOgreRoot (root),
        mSceneMgr (sceneMgr),
        mCleanup (false)
    {
        Ogre::LogManager::getSingleton().logMessage ("Caelum: Initialising Caelum system...");

        Ogre::String uniqueId = Ogre::StringConverter::toString ((size_t)this);
        mCaelumCameraNode = mSceneMgr->getRootSceneNode ()->createChildSceneNode ("Caelum/CameraNode/" + uniqueId);
        mCaelumGroundNode = mSceneMgr->getRootSceneNode ()->createChildSceneNode ("Caelum/GroundNode/" + uniqueId);
        mUniversalClock.reset(new UniversalClock ());

        autoConfigure (componentsToCreate);
    }

    void CaelumSystem::destroySubcomponents (bool destroyEverything)
    {
        // Destroy sub-components
        setSkyDome (0);
        setSun (0);
        setImageStarfield (0);
        setPointStarfield (0);
        setCloudSystem (0);
        setPrecipitationController (0);
        setGroundFog (0);
        setMoon (0);   
        mSkyGradientsImage.reset ();
        mSunColoursImage.reset ();

        // These things can't be rebuilt.
        if (destroyEverything)
        {
            mUniversalClock.reset ();
            if (mCaelumCameraNode) {
                static_cast<Ogre::SceneNode*>(mCaelumCameraNode->getParent())->
                        removeAndDestroyChild(mCaelumCameraNode->getName());
                mCaelumCameraNode = 0;
            }
            if (mCaelumGroundNode) {
                static_cast<Ogre::SceneNode*>(mCaelumGroundNode->getParent())->
                        removeAndDestroyChild(mCaelumGroundNode->getName());
                mCaelumGroundNode = 0;
            }
        }
    }
 

    CaelumSystem::~CaelumSystem () {
        destroySubcomponents (true);
        Ogre::LogManager::getSingleton ().logMessage ("Caelum: CaelumSystem destroyed.");
    }

    void CaelumSystem::clear() {
        destroySubcomponents (false);

        mAutoMoveCameraNode = true;
        mAutoNotifyCameraChanged = true;
        mAutoAttachViewports = true;
        mAutoViewportBackground = true;

        mManageSceneFog = false;

        mGlobalFogDensityMultiplier = 1;
        mSceneFogDensityMultiplier = 1;
        mSceneFogColourMultiplier = Ogre::ColourValue(0.7, 0.7, 0.7, 0.7);
        mGroundFogDensityMultiplier = 1;
        mGroundFogColourMultiplier = Ogre::ColourValue(1.0, 1.0, 1.0, 1.0);

        mManageAmbientLight = false;
        mMinimumAmbientLight = Ogre::ColourValue::Black;
        mEnsureSingleLightSource = false;
        mEnsureSingleShadowSource = false;

        mObserverLatitude = Ogre::Degree(45);
        mObserverLongitude = Ogre::Degree(0);
        mUniversalClock->setJulianDay (Astronomy::J2000);
    }

    void CaelumSystem::autoConfigure
    (
        CaelumComponent componentsToCreate/* = CAELUM_COMPONENTS_DEFAULT*/
    )
    {
        // Clear everything
        clear();

        Ogre::LogManager::getSingleton ().logMessage ("Caelum: Creating caelum sub-components.");

        // Default lookups.
        setSkyGradientsImage(DEFAULT_SKY_GRADIENTS_IMAGE);
        setSunColoursImage(DEFAULT_SUN_COLOURS_IMAGE);

        // Create default components; as requested.
        if (componentsToCreate & CAELUM_COMPONENT_SKY_DOME) {
            this->setSkyDome (new SkyDome (mSceneMgr, getCaelumCameraNode ()));
        }
        if (componentsToCreate & CAELUM_COMPONENT_SUN) {
            this->setSun (new SpriteSun (mSceneMgr, getCaelumCameraNode ()));
            this->getSun ()->setAmbientMultiplier (Ogre::ColourValue (0.5, 0.5, 0.5));
            this->getSun ()->setDiffuseMultiplier (Ogre::ColourValue (3, 3, 2.7));
            this->getSun ()->setSpecularMultiplier (Ogre::ColourValue (5, 5, 5));

            this->getSun ()->setAutoDisable (true);
            this->getSun ()->setAutoDisableThreshold (0.05);
        }
        if (componentsToCreate & CAELUM_COMPONENT_MOON) {
            this->setMoon (new Moon (mSceneMgr, getCaelumCameraNode ()));
            this->getMoon ()->setAutoDisable (true);
            this->getMoon ()->setAutoDisableThreshold (0.05);
        }
        if (componentsToCreate & CAELUM_COMPONENT_IMAGE_STARFIELD) {
            this->setImageStarfield (new ImageStarfield (mSceneMgr, getCaelumCameraNode ()));
        }
        if (componentsToCreate & CAELUM_COMPONENT_POINT_STARFIELD) {
            this->setPointStarfield (new PointStarfield (mSceneMgr, getCaelumCameraNode ()));
        }
        if (componentsToCreate & CAELUM_COMPONENT_GROUND_FOG) {
            this->setGroundFog (new GroundFog (mSceneMgr, getCaelumCameraNode ()));
        }
        if (componentsToCreate & CAELUM_COMPONENT_CLOUDS) {
            this->setCloudSystem (new CloudSystem (mSceneMgr, getCaelumGroundNode ()));
            getCloudSystem ()->createLayerAtHeight (3000);		
            getCloudSystem ()->getLayer (0)->setCloudCover (0.3);
        }
        if (componentsToCreate & CAELUM_COMPONENT_PRECIPITATION) {
            this->setPrecipitationController (new PrecipitationController (mSceneMgr));
        }

        setManageSceneFog (true);
        setManageAmbientLight (true);
        setMinimumAmbientLight (Ogre::ColourValue (0.1, 0.1, 0.3));

        Ogre::LogManager::getSingleton ().logMessage ("Caelum: DONE initializing");
    }

    void CaelumSystem::shutdown (const bool cleanup) {
        Ogre::LogManager::getSingleton ().logMessage ("Caelum: Shutting down Caelum system...");

        destroySubcomponents (true);

        if (cleanup) {
            mOgreRoot->removeFrameListener (this);
            delete this;
        } else {
            // We'll delete later. Make sure we're registered as a frame listener, or we'd leak.
            mOgreRoot->addFrameListener(this);
            mCleanup = true;
        }
    }

    void CaelumSystem::attachRenderTarget (Ogre::RenderTarget* rt)
    {
        rt->addListener(this);
        if (getAutoAttachViewports ()) {
            int numViewports = rt->getNumViewports();
            for (int i = 0; i < numViewports; i++) {
                attachViewport(rt->getViewport(i));
            }
        }
    }

    void CaelumSystem::detachRenderTarget (Ogre::RenderTarget* rt)
    {
        rt->removeListener(this);
        if (getAutoAttachViewports ()) {
            int numViewports = rt->getNumViewports();
            for (int i = 0; i < numViewports; i++) {
                detachViewport(rt->getViewport(i));
            }
        }
    }

    void CaelumSystem::viewportAdded (const Ogre::RenderTargetViewportEvent &e)
    {
        if (getAutoAttachViewports ()) {
            attachViewport(e.source);
        }
    }

    void CaelumSystem::viewportRemoved (const Ogre::RenderTargetViewportEvent &e)
    {
        if (getAutoAttachViewports ()) {
            detachViewport(e.source);
        }
    }

    void CaelumSystem::attachViewport (Ogre::Viewport* vp)
    {
        if (getPrecipitationController ()) {
            getPrecipitationController ()->createViewportInstance(vp);
        }
    }

    void CaelumSystem::detachViewport (Ogre::Viewport* vp)
    {
        if (getPrecipitationController ()) {
            getPrecipitationController ()->destroyViewportInstance(vp);
        }
    }

    void CaelumSystem::preViewportUpdate (const Ogre::RenderTargetViewportEvent &e) {
        Ogre::Viewport *viewport = e.source;
        Ogre::Camera *camera = viewport->getCamera ();

        if (getAutoViewportBackground ()) {
            viewport->setBackgroundColour (Ogre::ColourValue::Black);
        }
        if (getAutoNotifyCameraChanged ()) {
            this->notifyCameraChanged (camera);
        }
    }

    void CaelumSystem::notifyCameraChanged(Ogre::Camera* cam)
    {
        // Move camera node.
        if (getAutoMoveCameraNode ()) {
            mCaelumCameraNode->setPosition (cam->getDerivedPosition());
            mCaelumCameraNode->_update (true, true);
        }

        if (getSkyDome ()) {
            getSkyDome ()->notifyCameraChanged (cam);
        }

        if (getSun ()) {
            getSun ()->notifyCameraChanged (cam);
        }

        if (getMoon ()) {
            getMoon ()->notifyCameraChanged (cam);
        }

        if (getImageStarfield ()) {
            getImageStarfield ()->notifyCameraChanged (cam);
        }

        if (getPointStarfield ()) {
            getPointStarfield ()->notifyCameraChanged (cam);
        }
        
        if (getGroundFog ()) {
            getGroundFog ()->notifyCameraChanged (cam);
        }
    }
                    
    bool CaelumSystem::frameStarted (const Ogre::FrameEvent &e) {
        if (mCleanup) {
            // Delayed destruction.
            mOgreRoot->removeFrameListener (this);
            delete this;
            return true;
        }

        updateSubcomponents(e.timeSinceLastFrame);

        return true;
    }

    void CaelumSystem::updateSubcomponents (double timeSinceLastFrame) {
        if (!mUniversalClock->update (timeSinceLastFrame)) {
            return;
        }

        // Get current julian day.
        LongReal julDay = mUniversalClock->getJulianDay ();
        LongReal relDayTime = fmod(julDay, 1);

        // Get astronomical parameters.
        Ogre::Vector3 sunDir = getSunDirection(julDay);    
        Ogre::Vector3 moonDir = getMoonDirection(julDay);  
        Real moonPhase = getMoonPhase(julDay);    

        // Get parameters from sky colour model.
        Real fogDensity = getFogDensity (relDayTime, sunDir);           
        Ogre::ColourValue fogColour = getFogColour (relDayTime, sunDir);                  
        Ogre::ColourValue sunLightColour = getSunLightColour (relDayTime, sunDir);
        Ogre::ColourValue sunSphereColour = getSunSphereColour (relDayTime, sunDir);
        Ogre::ColourValue moonLightColour = getMoonLightColour (moonDir);
        Ogre::ColourValue moonBodyColour = getMoonBodyColour (moonDir); 

        fogDensity *= mGlobalFogDensityMultiplier;

        // Update image starfield
        if (getImageStarfield ()) {
            getImageStarfield ()->update (relDayTime);
            getImageStarfield ()->setInclination (-getObserverLatitude ());
        }

        // Update point starfield
        if (getPointStarfield ()) {
            getPointStarfield ()->setObserverLatitude (getObserverLatitude ());
            getPointStarfield ()->setObserverLongitude (getObserverLongitude ());
            getPointStarfield ()->_update (relDayTime);
        }

        // Update skydome.
        if (getSkyDome ()) {
            getSkyDome ()->setSunDirection (sunDir);
            getSkyDome ()->setHazeColour (fogColour * mSceneFogColourMultiplier);
        }

        // Update scene fog.
        if (getManageSceneFog ()) {
            mSceneMgr->setFog (Ogre::FOG_EXP2,
                    fogColour * mSceneFogColourMultiplier,
                    fogDensity * mSceneFogDensityMultiplier);
        }

        // Update ground fog.
        if (getGroundFog ()) {
            getGroundFog ()->setColour (fogColour * mGroundFogColourMultiplier);
            getGroundFog ()->setDensity (fogDensity * mGroundFogDensityMultiplier);
        }

        // Update sun
        if (getSun ()) {
            mSun->update (sunDir, sunLightColour, sunSphereColour);
        }

        // Update moon.
        if (getMoon ()) {
            mMoon->update (
                    moonDir,
                    moonLightColour,
                    moonBodyColour);
            mMoon->setPhase (moonPhase);
        }

        // Update clouds
        if (getCloudSystem ()) {
            getCloudSystem ()->update (
                    mUniversalClock->getJulianSecondDifference(),
                    sunDir, sunLightColour, fogColour, sunSphereColour);
        }

        if (getPrecipitationController ()) {
            getPrecipitationController ()->update(mUniversalClock->getJulianSecondDifference());
        }

        // Update ambient lighting.
        if (getManageAmbientLight ()) {
            Ogre::ColourValue ambient = Ogre::ColourValue::Black;
            if (getMoon ()) {
                ambient += getMoon ()->getLightColour () * getMoon() ->getAmbientMultiplier ();
            }
            if (getSun ()) {
                ambient += getSun ()->getLightColour () * getSun() ->getAmbientMultiplier ();
            }
            ambient.r = std::max(ambient.r, mMinimumAmbientLight.r);
            ambient.g = std::max(ambient.g, mMinimumAmbientLight.g);
            ambient.b = std::max(ambient.b, mMinimumAmbientLight.b);
            ambient.a = std::max(ambient.a, mMinimumAmbientLight.a);
            mSceneMgr->setAmbientLight (ambient);
        }

        if (getSun() && getMoon ()) {
            Ogre::Real moonBrightness = moonLightColour.r + moonLightColour.g + moonLightColour.b + moonLightColour.a;
            Ogre::Real sunBrightness = sunLightColour.r + sunLightColour.g + sunLightColour.b + sunLightColour.a;
            bool sunBrighterThanMoon = (sunBrightness > moonBrightness);

            if (getEnsureSingleLightSource ()) {
                getMoon ()->setForceDisable (sunBrighterThanMoon);
                getSun ()->setForceDisable (!sunBrighterThanMoon);
            }
            if (getEnsureSingleShadowSource ()) {
                getMoon ()->getMainLight ()->setCastShadows (!sunBrighterThanMoon);
                getSun ()->getMainLight ()->setCastShadows (sunBrighterThanMoon);
            }
        }
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

    double CaelumSystem::getSceneFogDensityMultiplier () const {
        return mSceneFogDensityMultiplier;
    }

    void CaelumSystem::setGroundFogDensityMultiplier (double value) {
        mGroundFogDensityMultiplier = value;
    }

    double CaelumSystem::getGroundFogDensityMultiplier () const {
        return mGroundFogDensityMultiplier;
    }

    void CaelumSystem::setGlobalFogDensityMultiplier (double value) {
        mGlobalFogDensityMultiplier = value;
    }

    double CaelumSystem::getGlobalFogDensityMultiplier () const {
        return mGlobalFogDensityMultiplier;
    }

    void CaelumSystem::setSkyGradientsImage (const Ogre::String &filename) {
        mSkyGradientsImage.reset(new Ogre::Image ());
        mSkyGradientsImage->load (filename, RESOURCE_GROUP_NAME);
    }

    void CaelumSystem::setSunColoursImage (const Ogre::String &filename) {
        mSunColoursImage.reset(new Ogre::Image ());
        mSunColoursImage->load (filename, RESOURCE_GROUP_NAME);
    }

    Ogre::ColourValue CaelumSystem::getFogColour (float time, const Ogre::Vector3 &sunDir) {
        if (!mSkyGradientsImage.get()) {
            return Ogre::ColourValue::Black;
        }

        float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y) * 0.5 + 0.5;
        Ogre::ColourValue col = getInterpolatedColour (elevation, 1, mSkyGradientsImage.get(), false);
        return col;
    }

    float CaelumSystem::getFogDensity (float time, const Ogre::Vector3 &sunDir)
    {
        if (!mSkyGradientsImage.get()) {
            return 0;
        }

        float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y) * 0.5 + 0.5;
        Ogre::ColourValue col = getInterpolatedColour (elevation, 1, mSkyGradientsImage.get(), false);
        return col.a;
    }

    Ogre::ColourValue CaelumSystem::getSunSphereColour (float time, const Ogre::Vector3 &sunDir)
    {
        if (!mSunColoursImage.get()) {
            return Ogre::ColourValue::White;
        }

        float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y);
        elevation = elevation * 2 + 0.4;
        return getInterpolatedColour (elevation, 1, mSunColoursImage.get(), false);
    }

    Ogre::ColourValue CaelumSystem::getSunLightColour (float time, const Ogre::Vector3 &sunDir)
    {
        if (!mSkyGradientsImage.get()) {
            return Ogre::ColourValue::White;
        }
        float elevation = sunDir.dotProduct (Ogre::Vector3::UNIT_Y) * 0.5 + 0.5;

        // Hack: return averaged sky colours.
        // Don't use an alpha value for lights, this can cause nasty problems.
        Ogre::ColourValue col = getInterpolatedColour (elevation, elevation, mSkyGradientsImage.get(), false);
        double val = (col.r + col.g + col.b) / 3;
        col = Ogre::ColourValue(val, val, val, 1.0);
        assert(Ogre::Math::RealEqual(col.a, 1));
        return col;
    }

    Ogre::ColourValue CaelumSystem::getMoonBodyColour (const Ogre::Vector3 &moonDir) {
        return Ogre::ColourValue::White;
    }

    Ogre::ColourValue CaelumSystem::getMoonLightColour (const Ogre::Vector3 &moonDir)
    {
        if (!mSkyGradientsImage.get()) {
            return Ogre::ColourValue::Blue;
        }
        // Scaled version of getSunLightColor
        float elevation = moonDir.dotProduct (Ogre::Vector3::UNIT_Y) * 0.5 + 0.5;
        Ogre::ColourValue col = getInterpolatedColour (elevation, elevation, mSkyGradientsImage.get(), false);
        double val = (col.r + col.g + col.b) / 3;
        col = Ogre::ColourValue(val / 2.5f, val / 2.5f, val / 2.5f, 1.0);
        assert(Ogre::Math::RealEqual(col.a, 1));
        return col;
    }

    const Ogre::Vector3 CaelumSystem::makeDirection (
            Ogre::Degree azimuth, Ogre::Degree altitude)
    {
        Ogre::Vector3 res;
        res.z = -Ogre::Math::Cos (azimuth) * Ogre::Math::Cos (altitude);  // North 
        res.x =  Ogre::Math::Sin (azimuth) * Ogre::Math::Cos (altitude);  // East
        res.y = -Ogre::Math::Sin (altitude); // Zenith
        return res;
    }

    const Ogre::Vector3 CaelumSystem::getSunDirection (LongReal jday)
    {
		int fpmode = Astronomy::enterHighPrecissionFloatingPointMode ();
                  
        Ogre::Degree azimuth;
        Ogre::Degree altitude; 
		Astronomy::getHorizontalSunPosition(jday,
                getObserverLongitude(), getObserverLatitude(),
                azimuth, altitude);		
        Ogre::Vector3 res = makeDirection(azimuth, altitude);

        Astronomy::restoreFloatingPointMode(fpmode);
        return res;
    }

	const Ogre::Vector3 CaelumSystem::getMoonDirection (LongReal jday) {
		int fpmode = Astronomy::enterHighPrecissionFloatingPointMode ();

        Ogre::Degree azimuth, altitude;
        Astronomy::getHorizontalMoonPosition(jday,
                getObserverLongitude (), getObserverLatitude (),
                azimuth, altitude);
	
        Ogre::Vector3 res = makeDirection(azimuth, altitude);

		Astronomy::restoreFloatingPointMode(fpmode);
		return res;
	}

    const Ogre::Real CaelumSystem::getMoonPhase (LongReal jday)
    {
        // Calculates julian days since January 22, 2008 13:36 (full moon)
        // and divides by the time between lunations (synodic month)
        LongReal T = (jday - 2454488.0665L) / 29.531026L;

        T = fabs(fmod(T, 1));
        return -fabs(-4 * T + 2) + 2;
    }
}
