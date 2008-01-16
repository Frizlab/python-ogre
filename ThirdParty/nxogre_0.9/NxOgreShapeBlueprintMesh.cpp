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
#include "NxOgreCooking.h"			// For: Cooking the TriMesh shapes
#include "NxOgreUserStream.h"		// For: Loading in previously cooked shapes.

namespace NxOgre {






//									Mesh





TriangleMeshShape::TriangleMeshShape(const NxString& meshName, const ShapeParams& p) : ShapeBlueprint(p) {


	mShapeDescription.setToDefault();
	meshname = meshName;

}

////////////////////////////////////////////////////////////////////////////////////////////////

TriangleMeshShape::~TriangleMeshShape() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* TriangleMeshShape::_bindToActorDescription(Actor* actor, NxU32 id, NxArray<NxShapeDesc*>& shapes) {

	if (Ogre::StringUtil::endsWith(meshname, ".nxs")) {
		UserStream rbuf(meshname.c_str(), true);
		mShapeDescription.meshData = actor->getNxScene()->getPhysicsSDK().createTriangleMesh(rbuf);
		fclose(rbuf.fp);
	}
	else {
		mShapeDescription.meshData = NxGenerateTriangleMeshFromOgreMesh(meshname, actor->getNxScene(), mParams.mMeshScale);
	}

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
