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
#include "NxOgreScene.h"			// For: Materials
#include "NxOgreGroup.h"			// For: ShapeGroups
#include "NxOgreResourceManager.h"	// For: Convex Fetching/Cooking/Storage.
#include "NxOgreResourceSystem.h"
#include "NxOgreResourceMesh.h"
#include "NxOgreResourceStream.h"
#include "NxOgreResourceStreamPtr.h"

#include "OgreMeshManager.h"

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


	NxString meshIdentifier = ResourceManager::getSingleton()->getMeshIdentifier(meshname, mParams.mMeshScale);

	mShapeDescription.meshData = ResourceManager::getSingleton()->getConvexMesh(meshIdentifier);

	if (mShapeDescription.meshData == 0) {
		
		if (Ogre::StringUtil::endsWith(meshname, ".mesh")) {

			Ogre::MeshManager::getSingleton().load(meshname, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			Ogre::MeshPtr meshPtr = Ogre::MeshManager::getSingleton().getByName(meshname);

			if (meshPtr.isNull()) {
				NxThrow_Error("Ogre Mesh '" + meshname + "' could not be found.");
			}

			ConvexMeshIntermediary* cmi = ResourceManager::getSingleton()->generateConvexMeshDescription(meshPtr, mParams.mMeshScale);
			
#if (NX_USE_TEMPORARYCOOKING_TO_DISK == 1)
			if (ResourceManager::getSingleton()->cookConvexMesh(cmi, ResourceStreamPtr(
				"file:"
				NX_USE_TEMPORARYCOOKING_TO_DISK_PATH
				" +write"))) {
				ResourceManager::getSingleton()->loadConvexMesh(meshIdentifier, ResourceStreamPtr(
					"file:"
					NX_USE_TEMPORARYCOOKING_TO_DISK_PATH
				));
				mShapeDescription.meshData = ResourceManager::getSingleton()->getConvexMesh(meshIdentifier);
			}
			else {
				NxThrow_Warning("Convex mesh '" + meshname +  "' could not be used with ConvexShape. Cooking process failed.");
			}
#else

			ResourceStreamPtr memoryStream(NxMemoryStreamIdentifier);


			if (ResourceManager::getSingleton()->cookConvexMesh(cmi, memoryStream)) {
				memoryStream->rewind();

				ResourceManager::getSingleton()->loadConvexMesh(meshIdentifier, memoryStream);
				mShapeDescription.meshData = ResourceManager::getSingleton()->getConvexMesh(meshIdentifier);
			}
			else {
				NxThrow_Warning("Convex mesh '" + meshname +  "' could not be used with ConvexShape. Cooking process failed.");
			}

			memoryStream->close();
#endif

			delete cmi;

		}
		else {
			NxThrow_Warning("Convex mesh '" + meshname +  "' could not be used with ConvexShape. It could not be found.");
		}

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

	std::stringstream ss;
	ss << "Prism-" << r << "-" << h << "-" << s;
	NxString prismIdentifier = ss.str();

	NxConvexMesh* mesh = ResourceManager::getSingleton()->getConvexMesh(prismIdentifier);
	
	if (mesh == 0) {

		ConvexMeshIntermediary* cmi = new ConvexMeshIntermediary();
		cmi->setToNormal();
		cmi->createVertices(s * 2);
		NxReal Angle,Cosine,Sine;
		NxU32 j = 0;

		for (unsigned int i = 0; i < s; i++)  {
			Angle = 360 * ( i / (NxReal) s);
			Cosine = NxMath::cos(Angle*(NxPi/180))*r;
			Sine = NxMath::sin(Angle*(NxPi/180))*r;
			cmi->mVertices[j++] = NxVec3(Sine, - h / 2, Cosine);
			cmi->mVertices[j++] = NxVec3(Sine, h / 2, Cosine);
		}

		cmi->verticesToDescription();

#if (NX_USE_TEMPORARYCOOKING_TO_DISK == 1)
			if (ResourceManager::getSingleton()->cookConvexMesh(cmi, ResourceStreamPtr(
				"file:"
				NX_USE_TEMPORARYCOOKING_TO_DISK_PATH
				" +write"))) {
				ResourceManager::getSingleton()->loadConvexMesh(prismIdentifier, ResourceStreamPtr(
					"file:"
					NX_USE_TEMPORARYCOOKING_TO_DISK_PATH
				));
				mShapeDescription.meshData = ResourceManager::getSingleton()->getConvexMesh(prismIdentifier);
			}
			else {
				NxThrow_Warning("Prism could not be used with ConvexShape. Cooking process failed.");
			}
#else

			ResourceStreamPtr memoryStream(NxMemoryStreamIdentifier);

			if (ResourceManager::getSingleton()->cookConvexMesh(cmi, memoryStream)) {
				memoryStream->rewind();

				ResourceManager::getSingleton()->loadConvexMesh(prismIdentifier, memoryStream);
				mShapeDescription.meshData = ResourceManager::getSingleton()->getConvexMesh(prismIdentifier);
			}
			else {
				NxThrow_Warning("Prism could not be used with ConvexShape. Cooking process failed.");
			}

			memoryStream->close();
#endif


		delete cmi;

	}
	else {
		mShapeDescription.meshData = mesh;
	}

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
