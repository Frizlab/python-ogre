#if 0
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
#include "NxOgreShapeBlueprint.h"
#include "NxOgreShapeBlueprintPrimitives.h"			// For defaultShape (new CubeShape(1))
#include "NxOgreShape.h"							// For ShapeParams
#include "NxOgreScene.h"							// For fetching material and group indexes
#include "NxOgreGroup.h"							// For ShapeGroups
#include "NxOgreSkeleton.h"							// For Skeletons

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

ShapeBlueprint::ShapeBlueprint() {
	mParams.setToDefault();
	mShared = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////

ShapeBlueprint::ShapeBlueprint(const ShapeParams& p) {
	mParams = p;
	mShared = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////

ShapeBlueprint* ShapeBlueprint::getDefaultShape() {
	return new CubeShape(1);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ShapeBlueprint::__genericParamsToDescription(NxShapeDesc& desc, const ShapeParams& p, Scene* s) {
	

	desc.density = p.mDensity;

	if (p.mGroupAsIndex != 0)
		desc.group = p.mGroupAsIndex;
	else if (p.mGroupAsName != "")
		desc.group = s->getShapeGroup(p.mGroupAsName)->getGroupID();

	desc.groupsMask = p.mGroupsMask;

	desc.localPose = p.mLocalPose;
	desc.mass = p.mMass;

	if (p.mMaterialAsIndex != 0)
		desc.materialIndex = p.mMaterialAsIndex;
	else if (mParams.mMaterialAsName.length() != 0)
		desc.materialIndex = s->getMaterialIndex(p.mMaterialAsName);
	
	desc.skinWidth = p.mSkinWidth;
	desc.shapeFlags = p.mFlags;

	if (p.mDynamicDynamicCCD) {
		desc.shapeFlags |= NX_SF_DYNAMIC_DYNAMIC_CCD;
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
#endif