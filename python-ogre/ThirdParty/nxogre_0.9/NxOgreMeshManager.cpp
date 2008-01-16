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
#include "NxOgreMeshManager.h"
#include "NxOgreSkeleton.h"
#include "NxOgreHelpers.h"

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

MeshManager* MeshManager::mMeshManager = 0;

////////////////////////////////////////////////////////////////////////////////////////////////

MeshManager::MeshManager(PhysXDriver* driver) {

	if (mMeshManager) {
		NxThrow_Error("MeshManager has already created.");
		return;
	}

	mMeshManager = this;
	mDriver = driver;
}

////////////////////////////////////////////////////////////////////////////////////////////////

MeshManager::~MeshManager() {

	// For every Skeleton
		// delete

	for (std::map<NxSkeletonID, Skeleton*>::iterator it = mSkeletons.begin();
		 it != mSkeletons.end();
		 it++) {
			 delete (*it).second;
	}

	mSkeletons.empty();
	mSkeletonsByName.empty();

	// For every Triangle Mesh
		// delete
	// For every Convex Mesh
		// delete
	// For every soft body
		// delete
	// For every cloth
		// delete
}

////////////////////////////////////////////////////////////////////////////////////////////////

Skeleton* MeshManager::getSkeletonFromName(NxString identifier) {
	if (mSkeletonsByName[identifier]) {
		return mSkeletonsByName[identifier];
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Skeleton* MeshManager::getSkeletonFromID(NxSkeletonID hashIdentifier) {
	if (mSkeletons[hashIdentifier]) {
		return mSkeletons[hashIdentifier];
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Skeleton* MeshManager::getCubeSkeleton(Ogre::Vector3 dimensions, Pose pose)  {
	NxVec3 d = toNxVec3(dimensions);
	NxMat33 m = pose.toMat34().M;
	return getCubeSkeleton(d,m);
}

////////////////////////////////////////////////////////////////////////////////////////////////

Skeleton* MeshManager::getCubeSkeleton(NxVec3 dimensions, NxMat33 pose)  {
	NxSkeletonID skeletonID = generateMeshHash(dimensions, SKH_Box);
	Skeleton* skeleton = getSkeletonFromID(skeletonID);
	
	if (skeleton)
		return skeleton;

	skeleton = new CubeSkeleton(dimensions, pose);
	// should check this.
	mSkeletons[skeletonID] = skeleton;


	return skeleton;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Skeleton* MeshManager::getSphereSkeleton(NxReal radius, NxMat33 pose)  {
	NxVec3 dimensions;
	dimensions.set(radius, 0, 0);
	NxSkeletonID skeletonID = generateMeshHash(dimensions, SKH_Sphere);
	Skeleton* skeleton = getSkeletonFromID(skeletonID);
	
	if (skeleton)
		return skeleton;

	skeleton = new SphereSkeleton(radius, pose);
	// should check this.
	mSkeletons[skeletonID] = skeleton;


	return skeleton;
}

////////////////////////////////////////////////////////////////////////////////////////////////


NxTriangleMesh* MeshManager::__cookTriangleMesh(NxString identifier, NxVec3 scale)  {

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

NxConvexMesh* MeshManager::__cookConvexMesh(NxString identifier, NxVec3 scale)  {

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////


}; //End of NxOgre namespace.
