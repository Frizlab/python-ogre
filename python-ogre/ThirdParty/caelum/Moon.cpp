/*
This file is part of Caelum.
See http://www.ogre3d.org/wiki/index.php/Caelum 

Copyright (c) 2006-2007 Caelum team. See Contributors.txt for details.

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
#include "Moon.h"

namespace caelum {
const Ogre::String Moon::MOON_MATERIAL_NAME = "CaelumPhaseMoon";

Moon::Moon (
		Ogre::SceneManager *sceneMgr,
		Ogre::SceneNode *caelumRootNode, 
		const Ogre::String &moonTextureName,
		const Ogre::Degree& moonTextureAngularSize
):
		BaseSkyLight(sceneMgr, caelumRootNode),
		mMoonTextureAngularSize(moonTextureAngularSize)
{
    mMaterial = Ogre::MaterialManager::getSingletonPtr()->getByName(MOON_MATERIAL_NAME);
	assert(!mMaterial.isNull());
	mMaterial = mMaterial->clone(MOON_MATERIAL_NAME + Ogre::StringConverter::toString((size_t)this));
    mMaterial->load();
    assert(!mMaterial.isNull());
	assert(!getFpParams().isNull());
	getFpParams()->setIgnoreMissingParams(true);
    setMoonTexture(moonTextureName);

	mMoonBillboardSet = sceneMgr->createBillboardSet("CaelumSpriteMoon", 2);
	mMoonBillboardSet->setMaterialName (mMaterial->getName());
	mMoonBillboardSet->setCastShadows (false);
	mMoonBillboardSet->setRenderQueueGroup (CAELUM_RENDER_QUEUE_MOON);
	mMoonBillboardSet->setDefaultDimensions(1.0f, 1.0f);
	mMoonBillboardSet->createBillboard(Ogre::Vector3::ZERO);

	mNode->attachObject (mMoonBillboardSet);
}

Moon::~Moon () {
	if (mNode) {
		mNode->detachObject (mMoonBillboardSet);
	}
	if (mMoonBillboardSet) {
		mMoonBillboardSet->_getManager ()->destroyBillboardSet (mMoonBillboardSet);
		mMoonBillboardSet = 0;
	}
	Ogre::MaterialManager::getSingletonPtr()->remove(mMaterial->getHandle());
}

void Moon::setBodyColour (const Ogre::ColourValue &colour) {
	BaseSkyLight::setBodyColour(colour);

	// Set moon material colour.
	mMoonBillboardSet->getBillboard(0)->setColour(colour);
}

void Moon::setMoonTexture (const Ogre::String &textureName) {
	// Update the moon material
	assert(mMaterial->getBestTechnique ());
	assert(mMaterial->getBestTechnique ()->getPass (0));
	assert(mMaterial->getBestTechnique ()->getPass (0)->getTextureUnitState (0));
	mMaterial->getBestTechnique ()->getPass (0)->getTextureUnitState (0)->setTextureName (textureName);
}

void Moon::setMoonTextureAngularSize(const Ogre::Degree& moonTextureAngularSize){
	mMoonTextureAngularSize = moonTextureAngularSize;
}

void Moon::notifyCameraChanged (Ogre::Camera *cam) {
    // This calls setFarRadius
    BaseSkyLight::notifyCameraChanged(cam);

    // Set moon position.
    Ogre::Real moonDistance = mRadius - mRadius * Ogre::Math::Tan(mMoonTextureAngularSize);
	mNode->setPosition(-mDirection * moonDistance);

    // Set moon scaling in [1.0 ~ 1.2] range
    float factor = 1.2f - mBodyColour.b * 0.2f;
    float scale = factor * moonDistance * Ogre::Math::Tan(mMoonTextureAngularSize);
    mNode->setScale (Ogre::Vector3::UNIT_SCALE * scale);
}

Ogre::GpuProgramParametersSharedPtr Moon::getFpParams() {
    assert(!mMaterial.isNull());
    assert(mMaterial->getBestTechnique());
    assert(mMaterial->getBestTechnique()->getPass(0));
    assert(!mMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters().isNull());
	return mMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
}

void Moon::setPhase (Ogre::Real phase) {
	getFpParams()->setNamedConstant("phase", phase);	
}

} // namespace caelum
