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
#include "NxOgreShapePrimitives.h"
#include "NxOgreShapeBlueprintPrimitives.h"	// For CubeShape
#include "NxOgreActor.h"
#include "NxOgreHelpers.h"

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

Ground::Ground(GroundShape& desc, Actor* actor, NxArray<NxShapeDesc*>& shapes) : Shape(actor) {
	desc.mShapeDescription.userData = this;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Ground::Ground(GroundShape& desc, Actor* actor) : Shape(actor)  {
	desc.mShapeDescription.userData = this;	
}

////////////////////////////////////////////////////////////////////////////////////////////////

Ground::~Ground() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Ground::_bindNxShapeToShape(NxShape* s) {
	if (s->isPlane()) {
		mShape = static_cast<NxPlaneShape*>(s);
	}
	else {
		NxThrow_Warning("Attempted to bind wrong NxShape to NxOgre shape");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Ground::releaseShape() {
	mActor->getNxActor()->releaseShape(*mShape);
}

////////////////////////////////////////////////////////////////////////////////////////////////

ShapeBlueprint*	Ground::getBlueprint() {
	
	
	NxPlaneShapeDesc desc;
	ShapeParams params;
	params.setToDefault();

	mShape->saveToDesc(desc);	
	__descriptionToParams(desc, params);

	GroundShape* shape = new GroundShape(desc.d, NxConvert<Ogre::Vector3, NxVec3>(desc.normal), params);

	return shape;
}


////////////////////////////////////////////////////////////////////////////////////////////////

void Ground::copyTo(Actor* a) {
	// ...
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Ground::moveTo(Actor*) {
	// ...
}

////////////////////////////////////////////////////////////////////////////////////////////////

Cube::Cube(CubeShape& desc, Actor* actor, NxArray<NxShapeDesc*>& shapes, Skeleton* skeleton) : Shape(actor), mShape(0) {
	
	desc.mShapeDescription.userData = this;
	mSkeleton = skeleton;
	
}

////////////////////////////////////////////////////////////////////////////////////////////////

Cube::Cube(CubeShape& desc, Actor* actor, Skeleton* skeleton) : Shape(actor), mShape(0)  {
	
	desc.mShapeDescription.userData = this;	
	mSkeleton = skeleton;

}

////////////////////////////////////////////////////////////////////////////////////////////////

Cube::~Cube() {

}

////////////////////////////////////////////////////////////////////////////////////////////////

void Cube::_bindNxShapeToShape(NxShape* s) {

	mBaseShape = s;

	
	if (s->isBox()) {
		mShape = s->isBox();
	}
#ifdef NX_DEBUG
	else {
		NxThrow_Warning("Attempted to bind wrong NxShape to NxOgre shape");
	}
#endif

}

////////////////////////////////////////////////////////////////////////////////////////////////

void Cube::releaseShape() {
	mActor->getNxActor()->releaseShape(*mBaseShape);
}

////////////////////////////////////////////////////////////////////////////////////////////////

ShapeBlueprint*	Cube::getBlueprint() {
		
	NxBoxShapeDesc desc;
	ShapeParams params;
	params.setToDefault();
	
	mShape->saveToDesc(desc);	
	__descriptionToParams(desc, params);

	CubeShape* shape = new CubeShape(desc.dimensions.x * 2, desc.dimensions.y * 2, desc.dimensions.z * 2, params);

	return shape;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Cube::copyTo(Actor* a) {

}

////////////////////////////////////////////////////////////////////////////////////////////////

void Cube::moveTo(Actor*) {
	// ...
}

////////////////////////////////////////////////////////////////////////////////////////////////

Sphere::Sphere(SphereShape& desc, Actor* actor, NxArray<NxShapeDesc*>& shapes) : Shape(actor) {
	desc.mShapeDescription.userData = this;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Sphere::Sphere(SphereShape& desc, Actor* actor) : Shape(actor) {
	desc.mShapeDescription.userData = this;	
}

////////////////////////////////////////////////////////////////////////////////////////////////

Sphere::~Sphere() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Sphere::_bindNxShapeToShape(NxShape* s) {
	
	mBaseShape = s;

	if (s->isSphere()) {
		mShape = static_cast<NxSphereShape*>(s);
	}
	else {
		NxThrow_Warning("Attempted to bind wrong NxShape to NxOgre shape");
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

ShapeBlueprint*	Sphere::getBlueprint() {
	
	NxSphereShapeDesc desc;
	ShapeParams params;
	params.setToDefault();
	
	mShape->saveToDesc(desc);	
	__descriptionToParams(desc, params);

	CubeShape* shape = new CubeShape(desc.radius, params);

	return shape;

}

////////////////////////////////////////////////////////////////////////////////////////////////

Capsule::Capsule(CapsuleShape& desc, Actor* actor, NxArray<NxShapeDesc*>& shapes) : Shape(actor)  {
	desc.mShapeDescription.userData = this;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Capsule::Capsule(CapsuleShape& desc, Actor* actor) : Shape(actor)  {
	desc.mShapeDescription.userData = this;	
}

////////////////////////////////////////////////////////////////////////////////////////////////

Capsule::~Capsule() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

void Capsule::_bindNxShapeToShape(NxShape* s) {
	
	mBaseShape = s;

	if (s->isCapsule()) {
		mShape = static_cast<NxCapsuleShape*>(s);
	}
	else {
		NxThrow_Warning("Attempted to bind wrong NxShape to NxOgre shape");
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

ShapeBlueprint*	Capsule::getBlueprint() {
	
	NxCapsuleShapeDesc desc;
	ShapeParams params;
	params.setToDefault();
	
	mShape->saveToDesc(desc);	
	__descriptionToParams(desc, params);

	CapsuleShape* shape = new CapsuleShape(desc.radius, desc.height, params);

	return shape;

}

}; //End of NxOgre namespace.
