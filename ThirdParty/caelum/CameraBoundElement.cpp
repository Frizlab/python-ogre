#include "CaelumPrecompiled.h"
#include "CameraBoundElement.h"

namespace caelum {
    const Ogre::Real CameraBoundElement::CAMERA_FAR_DISTANCE_MULTIPLIER = 0.99;
    
    const Ogre::Real CameraBoundElement::CAMERA_NEAR_DISTANCE_MULTIPLIER = 50;

    CameraBoundElement::CameraBoundElement(): mAutoRadius(true) {
    }

    void CameraBoundElement::notifyCameraChanged (Ogre::Camera *cam) {
	    if (mAutoRadius) {
            if (cam->getFarClipDistance () > 0) {
                setFarRadius(cam->getFarClipDistance () * CAMERA_FAR_DISTANCE_MULTIPLIER);
            } else {
                setFarRadius(cam->getNearClipDistance () * CAMERA_NEAR_DISTANCE_MULTIPLIER);
            }
	    }
    }

    void CameraBoundElement::forceFarRadius (Ogre::Real radius) {
        if (radius > 0) {
            mAutoRadius = false;
            setFarRadius(radius);
        } else {
            mAutoRadius = true;
        }
    }

    bool CameraBoundElement::getAutoRadius () const {
        return mAutoRadius;
    }

    void CameraBoundElement::setFarRadius(Ogre::Real radius) {
        // Pass
    }

} // namespace caelum
