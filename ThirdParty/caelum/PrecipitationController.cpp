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

	PrecipitationController::PrecipitationController(
            Ogre::SceneManager *sceneMgr)
    {
		Ogre::String uniqueId = Ogre::StringConverter::toString((size_t)this);
		mSceneMgr = sceneMgr;

		setWindSpeed(Ogre::Vector3(0, -0.1, 0.1));
		setCoverage(0.25);
		setType(PRECIPITATION_SNOW);
		setSpeed(0.1);
		mTime = 0.0;

		update(0.0f);
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

	void PrecipitationController::setCoverage(Real coverage) {
		mCoverage = coverage;
	}

	Real PrecipitationController::getCoverage() {
		return mCoverage;
	}

	void PrecipitationController::setType(PrecipitationType type) {
		mType = type;
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

	void PrecipitationController::update(Real secondsSinceLastFrame) {
		mTime += secondsSinceLastFrame;
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
		Ogre::Vector3 camDir = cam->getDerivedDirection();
		Ogre::Vector3 camUp = cam->getDerivedUp();
		Ogre::Vector3 camRight = cam->getDerivedRight();
		camDir.normalise();
		camUp.normalise();
		camRight.normalise();
		// 4523.893416f is divisible with all the sine periods in the shader
        Real appTime = static_cast<Real>(fmod(mTime, 4523.893416f));

		Ogre::GpuProgramParametersSharedPtr fpParams =
				mat->getBestTechnique ()->getPass (0)->getFragmentProgramParameters ();

        fpParams->setIgnoreMissingParams(true);
		fpParams->setNamedConstant("coverage", mCoverage);	
		fpParams->setNamedConstant("camera_vector", camSpeed);
		fpParams->setNamedConstant("wind_vector", mWindSpeed);
		fpParams->setNamedConstant("camera_direction", camDir);
		fpParams->setNamedConstant("camera_up", camUp);
		fpParams->setNamedConstant("camera_right", camRight);
		fpParams->setNamedConstant("dropSpeed", mSpeed);
		fpParams->setNamedConstant("appTime", appTime);
		mat->getTechnique(0)->getPass(0)->getTextureUnitState(1)->setCurrentFrame(mType);
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
                mCameraSpeed = camPos - mLastCameraPosition;
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
