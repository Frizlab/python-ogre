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
#include "NxOgreShapeBlueprintConvex.h"
#include "NxOgreActor.h"			// For: Actor binding
#include "NxOgreShapeConvex.h"		// For: Convex
#include "NxOgreHelpers.h"			// For: Conversions
#include "NxOgreCooking.h"			// For: Cooking the convex shapes
#include "NxOgreScene.h"			// For: Materials
#include "NxOgreUserStream.h"		// For: Loading in .nxs files
#include "NxOgreGroup.h"			// For: ShapeGroups

namespace NxOgre {






//									Convex





ConvexShape::ConvexShape(const NxString& meshName, const ShapeParams& p) : ShapeBlueprint(p) {


	mShapeDescription.setToDefault();
	mShapeDescription.skinWidth = mParams.mSkinWidth;
	mShapeDescription.localPose = mParams.mLocalPose;
	
	meshname = meshName;

}

////////////////////////////////////////////////////////////////////////////////////////////////

ConvexShape::~ConvexShape() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* ConvexShape::_bindToActorDescription(Actor* actor, NxU32 id, NxArray<NxShapeDesc*>& shapes) {

	if (mParams.mMaterialAsIndex != 0)
		mShapeDescription.materialIndex = mParams.mMaterialAsIndex;
	else if (mParams.mMaterialAsName.length() != 0)
		mShapeDescription.materialIndex = actor->getScene()->getMaterialIndex(mParams.mMaterialAsName);

	if (mParams.mGroupAsIndex != 0)
		mShapeDescription.group = mParams.mGroupAsIndex;
	else if (mParams.mGroupAsName != "")
		mShapeDescription.group = actor->getScene()->getShapeGroup(mParams.mGroupAsName)->getGroupID();

	if (Ogre::StringUtil::endsWith(meshname, ".nxs")) {
		UserStream rbuf(meshname.c_str(), true);
		mShapeDescription.meshData = actor->getNxScene()->getPhysicsSDK().createConvexMesh(rbuf);
		fclose(rbuf.fp);
	}
	else {
		mShapeDescription.meshData = NxGenerateConvexMeshFromOgreMesh(meshname, actor->getNxScene(), mParams.mMeshScale);
	}

	Convex* convex = new Convex(*this, actor, shapes);
	actor->mCollisionModel.insert(id, convex);
	actor->mCollisionModel.lock(id, true);

	shapes.push_back(&mShapeDescription);
	return convex;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* ConvexShape::_bindToNxActor(Actor* actor, NxShapeIndex id) {
	NxUnderConstruction;
	return 0;
}

//									PrismShape





PrismShape::PrismShape(NxReal radius, NxReal height, unsigned int nbSides, const ShapeParams& p) : ShapeBlueprint(p) {


	mShapeDescription.setToDefault();
	mShapeDescription.skinWidth = mParams.mSkinWidth;
	mShapeDescription.localPose = mParams.mLocalPose;
	r = radius;
	h = height;
	s = nbSides;

}

////////////////////////////////////////////////////////////////////////////////////////////////

PrismShape::~PrismShape() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* PrismShape::_bindToActorDescription(Actor* actor, NxU32 id, NxArray<NxShapeDesc*>& shapes) {

	NxVec3* verts = new NxVec3[s * 2];

	NxReal Angle,Cosine,Sine;
	NxU32 j = 0;

	for (unsigned int i = 0; i < s; i++)  {
		Angle = 360 * ( i / (NxReal) s);
		Cosine = NxMath::cos(Angle*(NxPi/180))*r;
		Sine = NxMath::sin(Angle*(NxPi/180))*r;
		verts[j++] = NxVec3(Sine, - h / 2, Cosine);
		verts[j++] = NxVec3(Sine, h / 2, Cosine);
	}

	mShapeDescription.meshData = NxGenerateConvexMeshFromVertices(verts, s * 2, actor->getNxScene());

	delete []verts;

	if (mParams.mMaterialAsIndex != 0)
		mShapeDescription.materialIndex = mParams.mMaterialAsIndex;
	else if (mParams.mMaterialAsName.length() != 0)
		mShapeDescription.materialIndex = actor->getScene()->getMaterialIndex(mParams.mMaterialAsName);

	if (mParams.mGroupAsIndex != 0)
		mShapeDescription.group = mParams.mGroupAsIndex;
	else if (mParams.mGroupAsName != "")
		mShapeDescription.group = actor->getScene()->getShapeGroup(mParams.mGroupAsName)->getGroupID();


	Prism* nss = new Prism(*this, actor, shapes);
	actor->mCollisionModel.insert(id, nss);
	actor->mCollisionModel.lock(id, true);

	shapes.push_back(&mShapeDescription);

	return nss;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* PrismShape::_bindToNxActor(Actor* actor, NxShapeIndex id) {
	NxUnderConstruction;
	return 0;
}

}; //End of NxOgre namespace.
