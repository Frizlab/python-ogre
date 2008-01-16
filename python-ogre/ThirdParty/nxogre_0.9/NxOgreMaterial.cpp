//
//	NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org
//
//	This library is free software; you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation; either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "NxOgreStable.h"
#include "NxOgreMaterial.h"
#include "NxOgreScene.h"			// For: CreateMaterial in NxScene
#include "NxOgreHelpers.h"			// For: Ogre::Vector3 to NxVec3 conversion.

namespace NxOgre {

//////////////////////////////////////////////////////////////////////

Material::Material(const NxString& identifier, Scene* scene) : mName(identifier), mScene(scene) {
	NxMaterialDesc d;
	d.setToDefault();
	mMaterial = scene->getNxScene()->createMaterial(d);
	mMaterialIndex = mMaterial->getMaterialIndex();
	scene->_registerMaterial(identifier, this);
	std::stringstream ss;
	ss << "Created material '" << identifier << "' with Material Index as " << mMaterialIndex << std::endl;
	NxDebug(ss.str());

}

//////////////////////////////////////////////////////////////////////


Material::Material(NxMaterialIndex index, const NxString& identifier, Scene* scene) : mName(identifier), mScene(scene) {
	mMaterialIndex = index;
	mMaterial = mScene->getNxScene()->getMaterialFromIndex(index);
	scene->_registerMaterial(identifier, this);

#ifdef NX_DEBUG
	std::stringstream ss;
	ss << "Created material manually named '" << identifier << "' with Material Index as " << mMaterialIndex << std::endl;
	NxDebug(ss.str());
#endif

}

//////////////////////////////////////////////////////////////////////

Material::~Material() {
	if (mMaterialIndex == 0)
		return;

	mScene->getNxScene()->releaseMaterial(*mMaterial);
	mScene->_unregisterMaterial(mName);
}

//////////////////////////////////////////////////////////////////////

NxMaterialIndex	Material::getMaterialIndex() {
	return mMaterialIndex;
}

//////////////////////////////////////////////////////////////////////

void Material::setAll(NxReal bounce, NxReal dFriction, NxReal sFriction) {
	mMaterial->setRestitution(bounce);
	mMaterial->setDynamicFriction(dFriction);
	mMaterial->setStaticFriction(sFriction);
}

//////////////////////////////////////////////////////////////////////

void Material::setDynamicFriction(NxReal coef) {
	mMaterial->setDynamicFriction(coef);
}

//////////////////////////////////////////////////////////////////////

NxReal Material::getDynamicFriction() const {
	return mMaterial->getDynamicFriction();
}

//////////////////////////////////////////////////////////////////////

void Material::setStaticFriction(NxReal coef) {
	mMaterial->setStaticFriction(coef);
}

//////////////////////////////////////////////////////////////////////

NxReal Material::getStaticFriction() const {
	return mMaterial->getStaticFriction();
}

//////////////////////////////////////////////////////////////////////

void Material::setRestitution(NxReal rest) {
	mMaterial->setRestitution(rest);
}

//////////////////////////////////////////////////////////////////////

NxReal Material::getRestitution() const {
	return mMaterial->getRestitution();
}

//////////////////////////////////////////////////////////////////////

void Material::setDynamicFrictionV(NxReal coef) {
	mMaterial->setDynamicFrictionV(coef);
}

//////////////////////////////////////////////////////////////////////

NxReal Material::getDynamicFrictionV() const {
	return mMaterial->getDynamicFrictionV();
}

//////////////////////////////////////////////////////////////////////

void Material::setStaticFrictionV(NxReal coef) {
	mMaterial->setStaticFrictionV(coef);
}

//////////////////////////////////////////////////////////////////////

NxReal Material::getStaticFrictionV() const {
	return mMaterial->getStaticFrictionV();
}

//////////////////////////////////////////////////////////////////////

void Material::setDirOfAnisotropy(const Ogre::Vector3 &vec) {
	mMaterial->setDirOfAnisotropy(toNxVec3(vec));
}

//////////////////////////////////////////////////////////////////////

NxVec3 Material::getDirOfAnisotropy() const {
	return mMaterial->getDirOfAnisotropy();
}

//////////////////////////////////////////////////////////////////////

void Material::setFlags(NxU32 flags) {
	mMaterial->setFlags(flags);
}

//////////////////////////////////////////////////////////////////////

NxU32 Material::getFlags() const {
	return mMaterial->getFlags();
}

//////////////////////////////////////////////////////////////////////

void Material::setFrictionCombineMode(NxCombineMode combMode) {
	mMaterial->setFrictionCombineMode(combMode);
}

//////////////////////////////////////////////////////////////////////

NxCombineMode Material::getFrictionCombineMode() const {
	return mMaterial->getFrictionCombineMode();
}

//////////////////////////////////////////////////////////////////////

void Material::setRestitutionCombineMode(NxCombineMode combMode) {
	mMaterial->setRestitution(combMode);
}

//////////////////////////////////////////////////////////////////////

NxCombineMode Material::getRestitutionCombineMode() {
	return mMaterial->getRestitutionCombineMode();
}

//////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
