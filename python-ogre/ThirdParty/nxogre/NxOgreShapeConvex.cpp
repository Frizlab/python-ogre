/** \file    NxOgreShapeConvex.cpp
 *  \see     NxOgreShapeConvex.h
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
#include "NxOgreShapeConvex.h"
#include "NxOgreActor.h"
#include "NxOgreHelpers.h"
#include "NxOgreResourceManager.h"
namespace NxOgre {

/////////////////////////////////////////////////////////////

Convex::Convex(NxConvexMesh* convexMeshPtr, const ShapeParams& params)
: Shape(&mShapeDescription, params)
{
	mMesh = convexMeshPtr;
	mMeshIdentifier = NxString();
}

/////////////////////////////////////////////////////////////

Convex::Convex(const NxString& convexMeshIdentifier, const ShapeParams& params)
: Shape(&mShapeDescription, params)
{
	mMeshIdentifier = ResourceManager::getSingleton()->getMeshIdentifier(convexMeshIdentifier, NxVec3(1,1,1));
	mMesh = ResourceManager::getSingleton()->getConvexMesh(mMeshIdentifier);
}

/////////////////////////////////////////////////////////////

#if NX_USE_OGRE

Convex::Convex(const NxString& convexMeshIdentifier, Ogre::Vector3 scale, const ShapeParams& params)
: Shape(&mShapeDescription, params)
{
	mMeshIdentifier = ResourceManager::getSingleton()->getMeshIdentifier(convexMeshIdentifier, NxConvert<NxVec3, Ogre::Vector3>(scale));
	mMesh = ResourceManager::getSingleton()->getConvexMesh(mMeshIdentifier);
}

#endif

/////////////////////////////////////////////////////////////

Convex::Convex(const NxString& convexMeshIdentifier, NxVec3 scale, const ShapeParams& params)
: Shape(&mShapeDescription, params)
{
	mMeshIdentifier = ResourceManager::getSingleton()->getMeshIdentifier(convexMeshIdentifier, scale);
	mMesh = ResourceManager::getSingleton()->getConvexMesh(mMeshIdentifier);
}

/////////////////////////////////////////////////////////////

Convex::~Convex() {
	// Shape does not get released, as most of the time the destruction of
	// this shape, shortly results in the shape's actor.
}

/////////////////////////////////////////////////////////////

void Convex::createShape(NxActor* actor, NxShapeIndex index, Scene* scene) {
	extendedParamsToDescription(scene, mParams, &mShapeDescription);
	mShapeIndex = index;
	mActor = actor;
	mShapeDescription.userData = (void*) this;
	NxShape* shape = actor->createShape(mShapeDescription);
	setNxShape(shape);
}

/////////////////////////////////////////////////////////////

void Convex::createShape(NxArray<NxShapeDesc*>& shapes, NxShapeIndex index, Scene* scene) {
	extendedParamsToDescription(scene, mParams, &mShapeDescription);
	mShapeDescription.userData = (void*) this;
	shapes.push_back(&mShapeDescription);
	mShapeIndex = index;
}

/////////////////////////////////////////////////////////////

void Convex::releaseShape() {
	mActor->releaseShape(*mNxShape);
}

/////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.

