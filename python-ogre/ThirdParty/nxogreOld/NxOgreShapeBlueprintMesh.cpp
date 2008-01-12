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
#include "NxOgreShapeBlueprintMesh.h"
#include "NxOgreActor.h"			// For: Actor binding
#include "NxOgreShapeMesh.h"		// For: Convex
#include "NxOgreHelpers.h"			// For: Conversions
#include "NxOgreResourceManager.h"	// For: Triangle Fetching/Cooking/Storage.
#include "NxOgreResourceSystem.h"
#include "NxOgreResourceMesh.h"
#include "NxOgreResourceStream.h"
#include "NxOgreResourceStreamPtr.h"

#include "OgreMeshManager.h"

namespace NxOgre {






//									Mesh





TriangleMeshShape::TriangleMeshShape(const NxString& meshName, const ShapeParams& p) : ShapeBlueprint(p) {
	mShapeDescription.setToDefault();
	mShapeDescription.density  = 0.0f;
	meshname = meshName;
}

////////////////////////////////////////////////////////////////////////////////////////////////

TriangleMeshShape::~TriangleMeshShape() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* TriangleMeshShape::_bindToActorDescription(Actor* actor, NxU32 id, NxArray<NxShapeDesc*>& shapes) {

	NxString meshIdentifier = ResourceManager::getSingleton()->getMeshIdentifier(meshname, mParams.mMeshScale);

	mShapeDescription.meshData = ResourceManager::getSingleton()->getTriangleMesh(meshIdentifier);

	if (mShapeDescription.meshData == 0) {
		
		if (Ogre::StringUtil::endsWith(meshname, ".mesh")) {

			Ogre::MeshManager::getSingleton().load(meshname, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			Ogre::MeshPtr meshPtr = Ogre::MeshManager::getSingleton().getByName(meshname);

			if (meshPtr.isNull()) {
				NxThrow_Error("Ogre Mesh '" + meshname + "' could not be found.");
			}

			TriangleMeshIntermediary* tmi = ResourceManager::getSingleton()->generateTriangleMeshDescription(meshPtr, 0, mParams.mMeshScale);
			
#if (NX_USE_TEMPORARYCOOKING_TO_DISK == 1)
			if (ResourceManager::getSingleton()->cookTriangleMesh(tmi, ResourceStreamPtr(
				"file:"
				NX_USE_TEMPORARYCOOKING_TO_DISK_PATH
				" +write"))) {
				ResourceManager::getSingleton()->loadTriangleMesh(meshIdentifier, ResourceStreamPtr(
					"file:"
					NX_USE_TEMPORARYCOOKING_TO_DISK_PATH
				));
				mShapeDescription.meshData = ResourceManager::getSingleton()->getTriangleMesh(meshIdentifier);
			}
			else {
				NxThrow_Warning("Convex mesh '" + meshname +  "' could not be used with ConvexShape. Cooking process failed.");
			}
#else

			ResourceStreamPtr memoryStream(NxMemoryStreamIdentifier);


			if (ResourceManager::getSingleton()->cookTriangleMesh(tmi, memoryStream)) {
				memoryStream->rewind();

				ResourceManager::getSingleton()->loadTriangleMesh(meshIdentifier, memoryStream);
				mShapeDescription.meshData = ResourceManager::getSingleton()->getTriangleMesh(meshIdentifier);
			}
			else {
				NxThrow_Warning("Triangle mesh '" + meshname +  "' could not be used with TriangleMeshShape. Cooking process failed.");
			}

			memoryStream->close();
#endif

			delete tmi;

		}
		else {
			NxThrow_Warning("Triangle mesh '" + meshname +  "' could not be used with TriangleMeshShape. It could not be found.");
		}

	}

#if 0
	if (Ogre::StringUtil::endsWith(meshname, ".nxs")) {
		UserStream rbuf(meshname.c_str(), true);
		mShapeDescription.meshData = actor->getNxScene()->getPhysicsSDK().createTriangleMesh(rbuf);
		fclose(rbuf.fp);
	}
	else {
		mShapeDescription.meshData = NxGenerateTriangleMeshFromOgreMesh(meshname, actor->getNxScene(), mParams.mMeshScale);
	}
#endif
	
///////////////////	mShapeDescription.meshData = ResourceManager::getSingleton()->getTriangleMesh(meshname, mParams.mMeshScale);

	

	__paramsToDescription(mShapeDescription, mParams, actor->getScene());

	TriangleMesh* mesh = new TriangleMesh(*this, actor, shapes);
	actor->mCollisionModel.insert(id, mesh);
	actor->mCollisionModel.lock(id, true);

	shapes.push_back(&mShapeDescription);
	return mesh;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* TriangleMeshShape::_bindToNxActor(Actor* actor, NxShapeIndex id) {
	NxUnderConstruction;
	return 0;
}

}; //End of NxOgre namespace.
