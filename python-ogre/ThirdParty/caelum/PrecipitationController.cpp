/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2008 Caelum team. See Contributors.txt for details.

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
#include "PrecipitationController.h"

using namespace Ogre;

namespace Caelum
{
	const String PrecipitationController::COMPOSITOR_NAME = "Caelum/PrecipitationCompositor";
	const String PrecipitationController::MATERIAL_NAME =   "Caelum/PrecipitationMaterial";

	const PrecipitationPresetParams PrecipitationPresets[] = {
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.95, "precipitation_drizzle.png" },
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.85, "precipitation_rain.png" },
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.12, "precipitation_snow.png" },
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.33, "precipitation_snowgrains.png" },
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.70, "precipitation_icecrystals.png" },
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.78, "precipitation_icepellets.png" },
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.74, "precipitation_hail.png" },
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.70, "precipitation_smallhail.png" }
	};

	PrecipitationController::PrecipitationController(
            Ogre::SceneManager *sceneMgr)
    {
		Ogre::String uniqueId = Ogre::StringConverter::toString((size_t)this);
		mSceneMgr = sceneMgr;

        setAutoDisableThreshold (0.001);

        setIntensity (0);
		setWindSpeed (Ogre::Vector3(0, 0, 0));
		mInternalTime = 0;
		mSecondsSinceLastFrame = 0;
		
		setPresetType (PRECTYPE_RAIN);
		
		update (0, Ogre::ColourValue(0, 0, 0, 0));
	}

	PrecipitationController::~PrecipitationController () {
        destroyAllViewportInstances ();
	}

	void PrecipitationController::setTextureName (Ogre::String textureName) {
        mPresetType = PRECTYPE_CUSTOM;
		mTextureName = textureName;
	}
	
	Ogre::String PrecipitationController::getTextureName () {
		return mTextureName;
	}

	void PrecipitationController::setSpeed (Real speed) {
        mPresetType = PRECTYPE_CUSTOM;
		mSpeed = speed;
	}

	Real PrecipitationController::getSpeed () {
		return mSpeed;
	}

	void PrecipitationController::setColor (Ogre::ColourValue color) {
        mPresetType = PRECTYPE_CUSTOM;
		mColor = color;
	}
	
	Ogre::ColourValue PrecipitationController::getColor () {
		return mColor;
	}

	bool PrecipitationController::isPresetType (PrecipitationType type) {
        return PRECTYPE_DRIZZLE <= type && type <= PRECTYPE_SMALLHAIL;
    }

	const PrecipitationPresetParams& PrecipitationController::getPresetParams (PrecipitationType type) {
        assert(isPresetType(type));
        return PrecipitationPresets[type];
    }

	void PrecipitationController::setParams (const PrecipitationPresetParams& params) {
		setColor (params.Color);
		setSpeed (params.Speed);
		setTextureName (params.Name);
    }

	void PrecipitationController::setPresetType (PrecipitationType type) {
        setParams (getPresetParams (type));
		mPresetType = type;
	}

	PrecipitationType PrecipitationController::getPresetType () const {
        return mPresetType;
	}

	void PrecipitationController::setWindSpeed (const Ogre::Vector3& value) {
		mWindSpeed = value;
	}

	const Ogre::Vector3 PrecipitationController::getWindSpeed () {
		return mWindSpeed;
	}

	void PrecipitationController::setIntensity (Real intensity) {
		mIntensity = intensity;
	}

	Real PrecipitationController::getIntensity () {
		return mIntensity;
	}

	void PrecipitationController::update (Real secondsSinceLastFrame, Ogre::ColourValue colour) {
        mSecondsSinceLastFrame = secondsSinceLastFrame;
        mInternalTime += mSecondsSinceLastFrame;
        mSceneColor = colour;

        ViewportInstanceMap::const_iterator it;
        ViewportInstanceMap::const_iterator begin = mViewportInstanceMap.begin ();
        ViewportInstanceMap::const_iterator end = mViewportInstanceMap.end ();
        for (it = begin; it != end; ++it) {
            it->second->_update ();
        }
	}

	void PrecipitationController::setManualCameraSpeed (const Ogre::Vector3& value) {
        ViewportInstanceMap::const_iterator it;
        ViewportInstanceMap::const_iterator begin = mViewportInstanceMap.begin();
        ViewportInstanceMap::const_iterator end = mViewportInstanceMap.end();
        for (it = begin; it != end; ++it) {
            it->second->setManualCameraSpeed(value);
        }
	}

	void PrecipitationController::setAutoCameraSpeed() {
        ViewportInstanceMap::const_iterator it;
        ViewportInstanceMap::const_iterator begin = mViewportInstanceMap.begin();
        ViewportInstanceMap::const_iterator end = mViewportInstanceMap.end();
        for (it = begin; it != end; ++it) {
            it->second->setAutoCameraSpeed();
        }
	}

	void PrecipitationController::_updateMaterialParams(
            const Ogre::MaterialPtr& mat,
            const Ogre::Camera* cam,
            const Ogre::Vector3& camSpeed) 
	{		
		// 4523.893416f is divisible with all the sine periods in the shader
        Real appTime = static_cast<Real>(fmod(mInternalTime, 4523.893416f));

		Ogre::GpuProgramParametersSharedPtr fpParams =
				mat->getBestTechnique ()->getPass (0)->getFragmentProgramParameters ();

        fpParams->setIgnoreMissingParams(true);
		Real sceneLum = (mSceneColor.r + mSceneColor.g + mSceneColor.b) / 3;
		mSceneColor.r = mSceneColor.g = mSceneColor.b = sceneLum;			
		fpParams->setNamedConstant("precColor", mSceneColor * mColor);
		fpParams->setNamedConstant("intensity", mIntensity);
		fpParams->setNamedConstant("dropSpeed", 0);		

		Ogre::Vector3 corner1, corner2, corner3, corner4;

		corner1 = cam->getCameraToViewportRay(0, 0).getDirection();
		corner2 = cam->getCameraToViewportRay(1, 0).getDirection();
		corner3 = cam->getCameraToViewportRay(0, 1).getDirection();
		corner4 = cam->getCameraToViewportRay(1, 1).getDirection();

		Ogre::Vector3 precDir = Ogre::Vector3(0, -mSpeed, 0) + mWindSpeed - camSpeed;
		Ogre::Quaternion quat = precDir.getRotationTo(Ogre::Vector3(0, -1, 0));

		corner1 = quat * corner1;
		corner2 = quat * corner2;
		corner3 = quat * corner3;
		corner4 = quat * corner4;

		fpParams->setNamedConstant("corner1", corner1);
		fpParams->setNamedConstant("corner2", corner2);
		fpParams->setNamedConstant("corner3", corner3);
		fpParams->setNamedConstant("corner4", corner4);		
		
		float fallSpeed = precDir.length();

		fpParams->setNamedConstant("deltaX",
			Ogre::Vector3(sin(appTime) + 4.33, cos(appTime * 1.5) + 5.26,
				cos(appTime * 2.5)) * fallSpeed / 10 + 88.001);
		fpParams->setNamedConstant("deltaY",
			Ogre::Vector3(0.6, 1.0, 1.4) * fallSpeed * appTime);

        if (mat->getTechnique(0)->getPass(0)->getTextureUnitState(1)->getTextureName() != mTextureName) {
			mat->getTechnique(0)->getPass(0)->getTextureUnitState(1)->setTextureName(mTextureName);
        }
	}

    PrecipitationInstance::PrecipitationInstance
    (
        PrecipitationController* parent,
        Ogre::Viewport* viewport
    ):
        mParent(parent),
        mViewport(viewport),
        mCompInst(0),
        mLastCamera(0),
        mLastCameraPosition(Vector3::ZERO),
        mCameraSpeed(Vector3::ZERO),
        mAutoCameraSpeed(true)
    {
        createCompositor ();
    }

    PrecipitationInstance::~PrecipitationInstance ()
    {
        destroyCompositor();
    }

    void PrecipitationInstance::createCompositor ()
    {
        // Check if nothing to do.
        if (mCompInst) {
            return;
        }

        Ogre::CompositorManager* compMgr = Ogre::CompositorManager::getSingletonPtr();
		
        // Create the precipitation compositor.
        mCompInst = compMgr->addCompositor(mViewport, PrecipitationController::COMPOSITOR_NAME);
        assert(mCompInst);
		mCompInst->setEnabled (false);
		mCompInst->addListener (this);
    }

    void PrecipitationInstance::destroyCompositor ()
    {
        // Check if nothing to do.
        if (mCompInst == 0) {
            return;
        }

        Ogre::CompositorManager* compMgr = Ogre::CompositorManager::getSingletonPtr();

        // Remove the precipitation compositor.
		mCompInst->removeListener (this);
        compMgr->removeCompositor(mViewport, PrecipitationController::COMPOSITOR_NAME);
        mCompInst = 0;
    }

	void PrecipitationInstance::notifyMaterialSetup(uint pass_id, Ogre::MaterialPtr &mat)
	{
        // Nothing to do here.
	}

	void PrecipitationInstance::notifyMaterialRender(uint pass_id, Ogre::MaterialPtr &mat)
	{
        if (mAutoCameraSpeed) {
			Ogre::Camera* cam = mViewport->getCamera();
			Ogre::Vector3 camPos = cam->getDerivedPosition();
            if (cam != mLastCamera) {
                mCameraSpeed = Ogre::Vector3::ZERO;
            } else {
                Real timeDiff = mParent->getSecondsSinceLastFrame ();
                Ogre::Vector3 posDiff = camPos - mLastCameraPosition;
                mCameraSpeed = posDiff / timeDiff;

                /*
                LogManager::getSingletonPtr ()->logMessage (
                        "Caelum::PrecipitationInstance:"
                        " posDiff = " + StringConverter::toString(posDiff) +
                        " timeDiff = " + StringConverter::toString(mParent->getSecondsSinceLastFrame (), 10) +
                        " speed = " + StringConverter::toString(mCameraSpeed));
                */
            }
            mLastCamera = cam;
            mLastCameraPosition = camPos;
        }
		mParent->_updateMaterialParams (mat, mViewport->getCamera(), mCameraSpeed);
	}

    bool PrecipitationInstance::getAutoCameraSpeed () {
        return mAutoCameraSpeed;
    }

    void PrecipitationInstance::setAutoCameraSpeed () {
        mAutoCameraSpeed = true;
        mLastCamera = 0;
    }

    void PrecipitationInstance::setManualCameraSpeed (const Ogre::Vector3& value) {
        mAutoCameraSpeed = false;
        mCameraSpeed = value;
    }

    const Ogre::Vector3 PrecipitationInstance::getCameraSpeed () {
        return mCameraSpeed;
    }

    bool PrecipitationInstance::shouldBeEnabled () const {
        return mParent->getAutoDisableThreshold () < 0 ||
               mParent->getIntensity () > mParent->getAutoDisableThreshold ();
    }

    void PrecipitationInstance::_update ()
    {
        mCompInst->setEnabled (shouldBeEnabled ());
    }

    PrecipitationInstance* PrecipitationController::createViewportInstance (Ogre::Viewport* vp)
    {
        ViewportInstanceMap::const_iterator it = mViewportInstanceMap.find (vp);
        if (it == mViewportInstanceMap.end()) {
            std::auto_ptr<PrecipitationInstance> inst (new PrecipitationInstance(this, vp));
            mViewportInstanceMap.insert (std::make_pair (vp, inst.get()));
            // hold instance until successfully added to map.
            return inst.release();
        } else {
            return it->second;
        }
    }
    
    PrecipitationInstance* PrecipitationController::getViewportInstance(Ogre::Viewport* vp) {
        ViewportInstanceMap::iterator it = mViewportInstanceMap.find (vp);
        if (it != mViewportInstanceMap.end ()) {
            return it->second;
        } else {
            return 0;
        }
    }

    void PrecipitationController::destroyViewportInstance (Viewport* vp)
    {
        ViewportInstanceMap::iterator it = mViewportInstanceMap.find (vp);
        if (it != mViewportInstanceMap.end ()) {
            PrecipitationInstance* inst = it->second;
            delete inst;
            mViewportInstanceMap.erase (it);
        }
    }

    void PrecipitationController::destroyAllViewportInstances () {
        ViewportInstanceMap::const_iterator it;
        ViewportInstanceMap::const_iterator begin = mViewportInstanceMap.begin();
        ViewportInstanceMap::const_iterator end = mViewportInstanceMap.end();
        for (it = begin; it != end; ++it) {
            assert(it->first == it->second->getViewport ());
            delete it->second;
        }
        mViewportInstanceMap.clear ();
    }
}