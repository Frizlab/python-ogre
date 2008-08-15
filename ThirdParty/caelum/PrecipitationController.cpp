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

	PrecipitationParams PrecipitationPresets[]={
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.95, "precipitation_drizzle.png" }, //Drizzle
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.85, "precipitation_rain.png" }, //Rain
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.12, "precipitation_snow.png" }, //Snow
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.33, "precipitation_snowgrains.png" }, //Snow grains
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.70, "precipitation_icecrystals.png" }, //Ice crystals
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.78, "precipitation_icepellets.png" }, //Ice pellets
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.74, "precipitation_hail.png" }, //Hail
        { Ogre::ColourValue(0.8, 0.8, 0.8, 1), 0.70, "precipitation_smallhail.png" } //Small hail		
	};

	PrecipitationController::PrecipitationController(
            Ogre::SceneManager *sceneMgr)
    {
		Ogre::String uniqueId = Ogre::StringConverter::toString((size_t)this);
		mSceneMgr = sceneMgr;

		setWindSpeed(Ogre::Vector3(0, -0.1, 0.0));
		setIntensity(0.25);
		setSpeed(0.1);
		setColor(Ogre::ColourValue(0.8, 0.8, 0.8, 1));
		mInternalTime = 0.0;
		mSecondsSinceLastFrame = 0.0;
		
		setType(PRECIPITATION_SNOW);
		
		update(0.0f,Ogre::ColourValue(0,0,0,0));
	}

	PrecipitationController::~PrecipitationController() {
        destroyAllViewportInstances();
	}

	void PrecipitationController::setWindSpeed(const Ogre::Vector3& value) {
		mWindSpeed = value;
	}

	const Ogre::Vector3 PrecipitationController::getWindSpeed() {
		return mWindSpeed;
	}

	void PrecipitationController::setTextureName(Ogre::String textureName) {
		mTextureName=textureName;
	}
	
	Ogre::String PrecipitationController::getTextureName() {
		return mTextureName;
	}
	
	void PrecipitationController::setIntensity(Real intensity) {
		mIntensity = intensity;
	}

	Real PrecipitationController::getIntensity() {
		return mIntensity;
	}

	void PrecipitationController::setType(PrecipitationType type) {
		mType = type;
		setColor(PrecipitationPresets[type].mColor);
		setSpeed(PrecipitationPresets[type].mSpeed);
		setTextureName(PrecipitationPresets[type].mName);
	}

	PrecipitationType PrecipitationController::getType() {
		return mType;
	}

	void PrecipitationController::setSpeed(Real speed) {
		mSpeed = speed;
	}

	Real PrecipitationController::getSpeed() {
		return mSpeed;
	}

	void PrecipitationController::setColor(Ogre::ColourValue color) {
		mColor = color;
	}
	
	Ogre::ColourValue PrecipitationController::getColor() {
		return mColor;
	}

	void PrecipitationController::update(Real secondsSinceLastFrame, Ogre::ColourValue colour) {
        mSecondsSinceLastFrame = secondsSinceLastFrame;
        mInternalTime += mSecondsSinceLastFrame;
        mSceneColor = colour;
	}

	void PrecipitationController::setManualCameraSpeed(const Ogre::Vector3& value) {
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

		Ogre::Vector3 camDir = cam->getDerivedDirection();
		Ogre::Vector3 camUp = - cam->getDerivedUp();
		Ogre::Vector3 camRight = cam->getDerivedRight();

		Ogre::Vector3 corner1, corner2, corner3, corner4;
		
		corner1 = camDir - camRight - camUp;
		corner2 = camDir + camRight - camUp;
		corner3 = camDir - camRight + camUp;
		corner4 = camDir + camRight + camUp;

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
        Ogre::CompositorManager* compMgr = Ogre::CompositorManager::getSingletonPtr();
		
        // Create the precipitation compositor.
        mCompInst = compMgr->addCompositor(mViewport, PrecipitationController::COMPOSITOR_NAME);
        assert(mCompInst);
		mCompInst->setEnabled (true);
		mCompInst->addListener (this);
    }

    PrecipitationInstance::~PrecipitationInstance()
    {
        Ogre::CompositorManager* compMgr = Ogre::CompositorManager::getSingletonPtr();

        // Remove the precipitation compositor.
		mCompInst->removeListener (this);
        compMgr->removeCompositor(mViewport, PrecipitationController::COMPOSITOR_NAME);
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
		mParent->_updateMaterialParams(mat, mViewport->getCamera(), mCameraSpeed);
	}

    bool PrecipitationInstance::getAutoCameraSpeed() {
        return mAutoCameraSpeed;
    }

    void PrecipitationInstance::setAutoCameraSpeed() {
        mAutoCameraSpeed = true;
        mLastCamera = 0;
    }

    void PrecipitationInstance::setManualCameraSpeed(const Ogre::Vector3& value) {
        mAutoCameraSpeed = false;
        mCameraSpeed = value;
    }

    const Ogre::Vector3 PrecipitationInstance::getCameraSpeed() {
        return mCameraSpeed;
    }

    PrecipitationInstance* PrecipitationController::createViewportInstance(Ogre::Viewport* vp)
    {
        ViewportInstanceMap::const_iterator it = mViewportInstanceMap.find(vp);
        // Throwing here would be justified.
        assert(it == mViewportInstanceMap.end());

        std::auto_ptr<PrecipitationInstance> inst(new PrecipitationInstance(this, vp));
        mViewportInstanceMap.insert(std::make_pair(vp, inst.get()));
        // hold instance until successfully added to map.
        return inst.release();
    }
    
    PrecipitationInstance* PrecipitationController::getViewportInstance(Ogre::Viewport* vp) {
        ViewportInstanceMap::iterator it = mViewportInstanceMap.find(vp);
        if (it != mViewportInstanceMap.end()) {
            return it->second;
        } else {
            return 0;
        }
    }

    void PrecipitationController::destroyViewportInstance(Viewport* vp)
    {
        ViewportInstanceMap::iterator it = mViewportInstanceMap.find(vp);
        // Throwing here would be justified.
        assert(it != mViewportInstanceMap.end());

        PrecipitationInstance* inst = it->second;
        delete inst;
        mViewportInstanceMap.erase(it);
    }

    void PrecipitationController::destroyAllViewportInstances() {
        ViewportInstanceMap::const_iterator it;
        ViewportInstanceMap::const_iterator begin = mViewportInstanceMap.begin();
        ViewportInstanceMap::const_iterator end = mViewportInstanceMap.end();
        for (it = begin; it != end; ++it) {
            assert(it->first == it->second->getViewport());
            delete it->second;
        }
        mViewportInstanceMap.clear();
    }
}
