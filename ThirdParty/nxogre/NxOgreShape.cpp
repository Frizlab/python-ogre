/** \file    NxOgreShape.cpp
 *  \see     NxOgreShape.h
 *  \version 1.0-20
 *
 *  \licence NxOgre a wrapper for the PhysX physics library.
 *           Copyright (C) 2005-8 Robin Southern of NxOgre.org http://www.nxogre.org
 *           This library is free software; you can redistribute it and/or
 *           modify it under the terms of the GNU Lesser General Public
 *           License as published by the Free Software Foundation; either
 *           version 2.1 of the License, or (at your option) any later version.
 *           
 *           This library is distributed in the hope that it will be useful,
 *           but WITHOUT ANY WARRANTY; without even the implied warranty of
 *           MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *           Lesser General Public License for more details.
 *           
 *           You should have received a copy of the GNU Lesser General Public
 *           License along with this library; if not, write to the Free Software
 *           Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "NxOgreStable.h"
#include "NxOgreShape.h"
#include "NxOgreHelpers.h"
#include "NxOgreSkeleton.h"
#include "NxOgreActor.h"
#include "NxOgreScene.h"
#include "NxOgreGroup.h"
#include "NxOgreDualIdentifier.h"
#include "NxOgreUserData.h"

#if NX_USE_OGRE
#	include "OgreStringConverter.h"
#endif
namespace NxOgre {

/////////////////////////////////////////////////////////////

void ShapeParams::setToDefault() {
	mLocalPose.id();
	mGenerateCCD = false;
	mGenerateCCDDelta = 0.8f;
	mDynamicDynamicCCD = false;
	mCCDSkeleton = NULL;
	mMaterial = MaterialIdentifier(0);
	mGroupAsIndex = 0;
	mGroupAsName = "";
	mSkinWidth = -1.0f;
	mDensity = 1.0f;
	mMass = -1.0f;
	mFlags = NX_SF_VISUALIZATION;
	mTrigger = false;
	mTriggerCallback = 0;
}

/////////////////////////////////////////////////////////////

void ShapeParams::parse(Parameters params) {

	setToDefault();

	for (Parameter* parameter = params.Begin(); parameter = params.Next();) {

		//if (Set("material", parameter, mMaterial)) continue;
		//if (Set("material-index", parameter, mMaterial)) continue;
		if (Set("offset", parameter, mLocalPose.t)) continue;

		if (parameter->i == "orientation") {
			NxQuat q = NxConvert<NxQuat, Ogre::Quaternion>(Ogre::StringConverter::parseQuaternion(parameter->j));
			mLocalPose.M.fromQuat(q);
		}

		if (Set("generateccd", parameter, mGenerateCCD)) continue;
		if (Set("generateccdelta", parameter, mGenerateCCDDelta)) continue;
		if (Set("ccddynamicflag", parameter, mDynamicDynamicCCD)) continue;
		if (Set("skin-width", parameter, mSkinWidth)) continue;
		if (Set("group", parameter, mGroupAsName)) continue;
		if (Set("group-index", parameter, mGroupAsIndex)) continue;
		if (Set("mass", parameter, mMass)) continue;
		if (Set("density", parameter, mDensity)) continue;

	}

}

/////////////////////////////////////////////////////////////

Shape::Shape(NxShapeDesc* shape_description, const ShapeParams& shape_params)
: mShapeIndex(0), mParams(shape_params), mActor(NULL), mNxShape(NULL), mUserData(0), mSkeleton(NULL), mTriggerCallback(0)
{

	if (shape_params.mCCDSkeleton)
		shape_description->ccdSkeleton = shape_params.mCCDSkeleton->getSkeleton();

	shape_description->density = shape_params.mDensity;
	shape_description->group = shape_params.mGroupAsIndex;
	shape_description->groupsMask = shape_params.mGroupsMask;
	shape_description->localPose = shape_params.mLocalPose;
	shape_description->mass = shape_params.mMass;
	
	if (shape_params.mMaterial.hasFirst())
		shape_description->materialIndex = shape_params.mMaterial.getFirst();
	// else
	//	convert string to material index.
	
	shape_description->shapeFlags = shape_params.mFlags;
	shape_description->skinWidth = shape_params.mSkinWidth;

}

/////////////////////////////////////////////////////////////

Shape::~Shape() {
	if (mUserData)
		delete mUserData;
}

/////////////////////////////////////////////////////////////

void Shape::setNxShape(NxShape *shape) {

	if (shape == NULL) {
		NxThrow(NxString("NULL or invalid shape was attempted to be assigned to this shape '" + getType() + "'.").c_str());
	}

	mNxShape = shape;
	mActor = &shape->getActor();

}

/////////////////////////////////////////////////////////////

void Shape::extendedParamsToDescription(Scene* scene, const ShapeParams& params, NxShapeDesc* shape_description) {
	
	if (params.mGroupAsName.size() && params.mGroupAsIndex == 0)
		shape_description->group = scene->getShapeGroup(params.mGroupAsName)->getGroupID();

	if (params.mMaterial.hasSecond())
		shape_description->materialIndex = scene->getMaterialIndex(params.mMaterial.getSecond());

}

/////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
