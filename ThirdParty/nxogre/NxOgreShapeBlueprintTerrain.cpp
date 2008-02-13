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
#include "NxOgreShapeBlueprintTerrain.h"
#include "NxOgreActor.h"			// For: Actor binding
#include "NxOgreShapeTerrain.h"		// For: Convex
#include "NxOgreHelpers.h"			// For: Conversions
#include "NxOgreCooking.h"			// For: Cooking the convex shapes

namespace NxOgre {


////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//									Terrain
//
////////////////////////////////////////////////////////////////////////////////////////////////

TerrainShape::TerrainShape(const NxString& ImageFilename, NxReal maxHeight, const ShapeParams& params) : ShapeBlueprint(params) {

	mShapeDescription.setToDefault();
	mShapeDescription.skinWidth = mParams.mSkinWidth;

	mFilename = ImageFilename;
	mMaxHeight = maxHeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////

TerrainShape::~TerrainShape() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* TerrainShape::_bindToActorDescription(Actor* actor, NxU32 id, NxArray<NxShapeDesc*>& shapes) {
	
	__paramsToDescription(mShapeDescription, mParams, actor->getScene());

	NxU32 mRowCount = 1;
	NxU32 mColumnCount = 1;

	mShapeDescription.shapeFlags    |= NX_SF_FEATURE_INDICES;

#if (NX_USE_DEBUG_RENDERER_API == 1)
	mShapeDescription.shapeFlags	|= NX_SF_VISUALIZATION;
#endif

	mShapeDescription.materialIndexHighBits = 0;
	mShapeDescription.holeMaterial = 2;

	if (Ogre::StringUtil::endsWith(mFilename, ".raw")) {
		mShapeDescription.heightField = NxGenerateHeightFieldFromRaw(mFilename, actor->getNxScene());
		mShapeDescription.heightScale = (mMaxHeight * 0.5f) / (32768.0f);
		mRowCount = mShapeDescription.heightField->getNbRows();
		mColumnCount = mShapeDescription.heightField->getNbColumns();
	}
	else {
		mShapeDescription.heightField = NxGenerateHeightFieldFromImage(mFilename, actor->getNxScene());
		mShapeDescription.heightScale = mMaxHeight / 32768.0f;
		mRowCount = mShapeDescription.heightField->getNbRows();
		mColumnCount = mShapeDescription.heightField->getNbColumns();
	}

	if (mParams.mMeshScale == NxVec3(1,1,1)) {
		mShapeDescription.rowScale      = (mRowCount / NxReal(mRowCount - 1));
		mShapeDescription.columnScale   = (mColumnCount / NxReal(mColumnCount - 1));
	}
	else {
		mShapeDescription.rowScale      = mParams.mMeshScale.x;
		mShapeDescription.columnScale   = mParams.mMeshScale.z;
	}

	Terrain* terrain = new Terrain(*this, actor, shapes);
	actor->mCollisionModel.insert(id, terrain);
	actor->mCollisionModel.lock(id, true);

	shapes.push_back(&mShapeDescription);
	return terrain;
}

////////////////////////////////////////////////////////////////////////////////////////////////

#if 0
Shape* TerrainShape::_bindToActorDescription(Actor* actor, NxU32 id, NxArray<NxShapeDesc*>& shapes) {

	if (Ogre::StringUtil::endsWith(mFilename, ".raw")) {
		std::vector<Ogre::String> d = Ogre::StringUtil::split(mFilename, ";", 3);
		if (d.size() < 2) {
			std::stringstream ss;
			ss << "Raw filename given does not come with the correct syntax for processing" << std::endl
			   << "		->	'" << mFilename << "'" << std::endl;
			 
			if (d.size() == 1)
				ss << "Image depth is required";
			else
				ss << "Filename is malformed";

			NxThrow_Error(ss.str());
		}

		bool is8Bit = Ogre::StringConverter::parseBool(d[0]);
		mFilename = d[1];
		mShapeDescription.heightField   = NxGenerateHeightFieldFromRaw(mFilename, mImageSize, mRows, is8Bit, actor->getNxScene());
		mShapeDescription.heightScale   = (mMaxHeight * 0.5f) / (32768.0f);
	}
	else {
		mShapeDescription.heightField   = NxGenerateHeightFieldFromImage(mFilename,mRows,actor->getNxScene());
		mShapeDescription.heightScale   = mMaxHeight / 32768.0f;
	}

	mShapeDescription.shapeFlags    = NX_SF_FEATURE_INDICES;
	
	mShapeDescription.rowScale      = mSize / NxReal(mRows-1);
	mShapeDescription.columnScale   = mSize / NxReal(mRows-1);
	mShapeDescription.shapeFlags	= 0;

#if (NX_USE_DEBUG_RENDERER_API == 1)
	//mShapeDescription.shapeFlags	|= NX_SF_VISUALIZATION;
#endif

	mShapeDescription.materialIndexHighBits = 0;
	mShapeDescription.holeMaterial = 2;

#if 0
	if (mParams.mMaterial != "")
		mShapeDescriptionm.aterialIndex = b->owner->findMaterialIndex(mParams.mMaterial);

	if (mParams.mGroup != "")
		mShapeDescription.group = b->owner->findGroupIndex(mParams.mGroup);
#endif

	Terrain* terrain = new Terrain(*this, actor, shapes);
	actor->mCollisionModel.insert(id, terrain);
	actor->mCollisionModel.lock(id, true);

	shapes.push_back(&mShapeDescription);
	return terrain;
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////

Shape* TerrainShape::_bindToNxActor(Actor* actor, NxShapeIndex id) {
	NxUnderConstruction;
	return 0;
}

}; //End of NxOgre namespace.
#endif