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
#include "NxOgreShape.h"
#include "NxOgreHelpers.h"
#include "NxOgreSkeleton.h"

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

void ShapeParams::setToDefault() {
	mLocalPose.id();
	mGenerateCCD = false;
	mGenerateCCDDelta = 0.8f;
	mDynamicDynamicCCD = false;
	mCCDSkeleton = NULL;
	mMaterialAsIndex = 0;
	mMaterialAsName = "";
	mGroupAsIndex = 0;
	mGroupAsName = "";
	mSkinWidth = -1.0f;
	mMeshScale.set(1,1,1);
	mDensity = 1.0f;
	mMass = -1.0f;
	mFlags = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ShapeParams::parse(Parameters P) {
	
	setToDefault();

	for (Parameters::iterator p = P.begin(); p != P.end();p++) {
		if (Set("material", (*p), mMaterialAsName)) continue;
		if (Set("material-index", (*p), mMaterialAsIndex)) continue;
		if (Set("offset", (*p), mLocalPose.t)) continue;
		if ((*p).first == "orientation") {
			NxQuat q = toNxQuat(Ogre::StringConverter::parseQuaternion((*p).second));
			mLocalPose.M.fromQuat(q);
		}
		if (Set("generateccd", (*p), mGenerateCCD)) continue;
		if (Set("generateccdelta", (*p), mGenerateCCDDelta)) continue;
		if (Set("ccddynamicflag", (*p), mDynamicDynamicCCD)) continue;
		if (Set("skin-width", (*p), mSkinWidth)) continue;
		if (Set("group", (*p), mGroupAsName)) continue;
		if (Set("group-index", (*p), mGroupAsIndex)) continue;
		if (Set("mesh-scale", (*p), mMeshScale)) continue;
		if (Set("mass", (*p), mMass)) continue;
		if (Set("density", (*p), mDensity)) continue;
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape::Shape(Actor* a) : mActor(a), mBaseShape(0), mSkeleton(0) {

}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape::~Shape() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Shape::__descriptionToParams(NxShapeDesc& desc, ShapeParams& p) {

// Temp. Commented out, need to store SkeletonHashIdentifier in skeleton.

//	if (desc.ccdSkeleton)
//		p.mCCDSkeleton = desc.ccdSkeleton;

	p.mDensity = desc.density;
	p.mGroupAsIndex = desc.group;
	p.mGroupsMask = desc.groupsMask;
	p.mLocalPose = desc.localPose;
	p.mMass = desc.mass;
	p.mMaterialAsIndex = desc.materialIndex;
	p.mSkinWidth = desc.skinWidth;
	p.mFlags = desc.shapeFlags;

}

////////////////////////////////////////////////////////////////////////////////////////////////

Skeleton* Shape::getSkeleton() {
	return mSkeleton;
}

////////////////////////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
