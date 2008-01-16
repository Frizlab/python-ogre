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
#include "NxOgreShapeBlueprintPrimitives.h"
#include "NxOgreActor.h"			// For: Actor binding
#include "NxOgreShapePrimitives.h"	// For: Ground, Cube Binding
#include "NxOgreHelpers.h"			// For: Conversions
#include "NxOgreScene.h"			// For: Materials
#include "NxOgreGroup.h"			// For: ShapeGroups
#include "NxOgreContainer.h"		// For: mShapeBlueprints in CompoundShape;
#include "NxOgreMeshManager.h"		// For: Creating/Getting Skeletons
#include "NxOgreSkeleton.h"			// For: Skeleton::getSkeleton()

namespace NxOgre {






//									Ground





GroundShape::GroundShape(NxReal distance, Ogre::Vector3 normal, const ShapeParams& p) : ShapeBlueprint(p) {

	mShapeDescription.setToDefault();
	mShapeDescription.d = distance;
	mShapeDescription.normal = toNxVec3(normal);

}

////////////////////////////////////////////////////////////////////////////////////////////////

GroundShape::~GroundShape() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* GroundShape::_bindToActorDescription(Actor* actor, NxU32 id, NxArray<NxShapeDesc*>& shapes) {

	__paramsToDescription(mShapeDescription, mParams, actor->getScene());

	Ground* ground = new Ground(*this, actor, shapes);
	actor->mCollisionModel.insert(id, ground);
	actor->mCollisionModel.lock(id, true);
	ground->setIndex(id);

	shapes.push_back(&mShapeDescription);

	return ground;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* GroundShape::_bindToNxActor(Actor* actor, NxShapeIndex id) {
	
	__paramsToDescription(mShapeDescription, mParams, actor->getScene());

	Ground* ground = new Ground(*this, actor);
	actor->mCollisionModel.insert(id, ground);
	actor->mCollisionModel.lock(id, true);
	ground->setIndex(id);

	NxShape* shape = actor->getNxActor()->createShape(mShapeDescription);
	ground->_bindNxShapeToShape(shape);

	return ground;
}

////////////////////////////////////////////////////////////////////////////////////////////////


//									Cube





////////////////////////////////////////////////////////////////////////////////////////////////

CubeShape::CubeShape(NxReal width, NxReal height, NxReal length, const ShapeParams& p) : ShapeBlueprint(p) {
	_buildDescription(width,height,length);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CubeShape::CubeShape(NxReal cubed, const ShapeParams& p) : ShapeBlueprint(p) {
	_buildDescription(cubed,cubed,cubed);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CubeShape::~CubeShape() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CubeShape::_buildDescription(NxReal w,NxReal h,NxReal d) {
	mShapeDescription.setToDefault();
	mShapeDescription.dimensions.set(w * 0.5f,h * 0.5f, d * 0.5f);
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* CubeShape::_bindToActorDescription(Actor* actor, NxU32 id, NxArray<NxShapeDesc*>& shapes) {
    
	__paramsToDescription(mShapeDescription, mParams, actor->getScene());

	Skeleton* skeleton = 0;

	if (mParams.mCCDSkeleton) {
		skeleton = mParams.mCCDSkeleton;
		mShapeDescription.ccdSkeleton = mParams.mCCDSkeleton->getSkeleton();
	}
	else if (mParams.mGenerateCCD) {
		skeleton = NxOgre::MeshManager::getInstance()->getCubeSkeleton(mShapeDescription.dimensions * mParams.mGenerateCCDDelta, NxMat33());
		mShapeDescription.ccdSkeleton = skeleton->getSkeleton();
	}

	Cube* cube = new Cube(*this, actor, shapes, skeleton);
	actor->mCollisionModel.insert(id, cube);
	actor->mCollisionModel.lock(id, true);
	cube->setIndex(id);

	shapes.push_back(&mShapeDescription);

	return cube;

}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* CubeShape::_bindToNxActor(Actor* actor, NxShapeIndex id) {
	
	__paramsToDescription(mShapeDescription, mParams, actor->getScene());

	Cube* cube = new Cube(*this, actor);
	actor->mCollisionModel.insert(id, cube);
	actor->mCollisionModel.lock(id, true);
	cube->setIndex(id);

	NxShape* shape = actor->getNxActor()->createShape(mShapeDescription);
	cube->_bindNxShapeToShape(shape);
	
	return cube;
}

////////////////////////////////////////////////////////////////////////////////////////////////


//									Sphere





////////////////////////////////////////////////////////////////////////////////////////////////

SphereShape::SphereShape(NxReal radius, const ShapeParams& p) : ShapeBlueprint(p) {

	mShapeDescription.setToDefault();
	mShapeDescription.radius = radius;
}

////////////////////////////////////////////////////////////////////////////////////////////////

SphereShape::~SphereShape() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* SphereShape::_bindToActorDescription(Actor* actor, NxU32 id, NxArray<NxShapeDesc*>& shapes) {

	__paramsToDescription(mShapeDescription, mParams, actor->getScene());

	if (mParams.mCCDSkeleton == 0 && mParams.mGenerateCCD) {
		Skeleton* skeleton = NxOgre::MeshManager::getInstance()->getSphereSkeleton(mShapeDescription.radius * mParams.mGenerateCCDDelta, NxMat33());
		mShapeDescription.ccdSkeleton = skeleton->getSkeleton();
	}

	Sphere* sphere = new Sphere(*this, actor, shapes);
	actor->mCollisionModel.insert(id, sphere);
	actor->mCollisionModel.lock(id, true);
	sphere->setIndex(id);

	shapes.push_back(&mShapeDescription);
	return sphere;

}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* SphereShape::_bindToNxActor(Actor* actor, NxShapeIndex id) {
	
	__paramsToDescription(mShapeDescription, mParams, actor->getScene());

	Sphere* sphere = new Sphere(*this, actor);
	actor->mCollisionModel.insert(id, sphere);
	actor->mCollisionModel.lock(id, true);
	sphere->setIndex(id);

	actor->getNxActor()->createShape(mShapeDescription);

	return sphere;

}

////////////////////////////////////////////////////////////////////////////////////////////////





//									Capsule





////////////////////////////////////////////////////////////////////////////////////////////////

CapsuleShape::CapsuleShape(NxReal radius, NxReal height,  const ShapeParams& p) : ShapeBlueprint(p) {

	mShapeDescription.setToDefault();
	mShapeDescription.radius = radius;
	mShapeDescription.height = height;

}

////////////////////////////////////////////////////////////////////////////////////////////////

CapsuleShape::~CapsuleShape() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* CapsuleShape::_bindToActorDescription(Actor* actor, NxU32 id, NxArray<NxShapeDesc*>& shapes) {

	__paramsToDescription(mShapeDescription, mParams, actor->getScene());

	Capsule* capsule = new Capsule(*this, actor, shapes);
	actor->mCollisionModel.insert(id, capsule);
	actor->mCollisionModel.lock(id, true);
	capsule->setIndex(id);

	shapes.push_back(&mShapeDescription);
	return capsule;

}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* CapsuleShape::_bindToNxActor(Actor* actor, NxShapeIndex id) {
	
	__paramsToDescription(mShapeDescription, mParams, actor->getScene());

	Capsule* capsule = new Capsule(*this, actor);
	actor->mCollisionModel.insert(id, capsule);
	actor->mCollisionModel.lock(id, true);
	capsule->setIndex(id);

	NxShape* shape = actor->getNxActor()->createShape(mShapeDescription);
	capsule->_bindNxShapeToShape(shape);

	return capsule;
}

////////////////////////////////////////////////////////////////////////////////////////////////





//									Compound





////////////////////////////////////////////////////////////////////////////////////////////////

CompoundShape::CompoundShape(
							 ShapeBlueprint* a, 
							 ShapeBlueprint* b
							 ) 
{
	NxShapeIndex id = mShapeBlueprints.count(); 
	mShapeBlueprints.insert(id, a);
	mShapeBlueprints.insert(++id, b);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CompoundShape::CompoundShape(
							 ShapeBlueprint* a,
							 ShapeBlueprint* b,
							 ShapeBlueprint* c
							 )
{
	NxShapeIndex id = mShapeBlueprints.count(); 
	mShapeBlueprints.insert(id, a);
	mShapeBlueprints.insert(++id, b);
	mShapeBlueprints.insert(++id, c);

}

////////////////////////////////////////////////////////////////////////////////////////////////

CompoundShape::CompoundShape(
							 ShapeBlueprint* a, 
							 ShapeBlueprint* b, 
							 ShapeBlueprint* c, 
							 ShapeBlueprint* d
							 )
{
	NxShapeIndex id = mShapeBlueprints.count(); 
	mShapeBlueprints.insert(id, a);
	mShapeBlueprints.insert(id++, b);
	mShapeBlueprints.insert(id++, c);
	mShapeBlueprints.insert(id++, d);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CompoundShape::CompoundShape(
							 ShapeBlueprint* a, 
							 ShapeBlueprint* b, 
							 ShapeBlueprint* c, 
							 ShapeBlueprint* d, 
							 ShapeBlueprint* e
							 )
{
	NxShapeIndex id = mShapeBlueprints.count(); 
	mShapeBlueprints.insert(id, a);
	mShapeBlueprints.insert(id++, b);
	mShapeBlueprints.insert(id++, c);
	mShapeBlueprints.insert(id++, d);
	mShapeBlueprints.insert(id++, e);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CompoundShape::CompoundShape(
							 ShapeBlueprint* a, 
							 ShapeBlueprint* b, 
							 ShapeBlueprint* c, 
							 ShapeBlueprint* d, 
							 ShapeBlueprint* e, 
							 ShapeBlueprint* f
							 )
{
	NxShapeIndex id = mShapeBlueprints.count(); 
	mShapeBlueprints.insert(id, a);
	mShapeBlueprints.insert(id++, b);
	mShapeBlueprints.insert(id++, c);
	mShapeBlueprints.insert(id++, d);
	mShapeBlueprints.insert(id++, e);
	mShapeBlueprints.insert(id++, f);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CompoundShape::CompoundShape(
							 ShapeBlueprint* a, 
							 ShapeBlueprint* b, 
							 ShapeBlueprint* c, 
							 ShapeBlueprint* d, 
							 ShapeBlueprint* e, 
							 ShapeBlueprint* f, 
							 ShapeBlueprint* g
							 )
{
	NxShapeIndex id = mShapeBlueprints.count(); 
	mShapeBlueprints.insert(id, a);
	mShapeBlueprints.insert(id++, b);
	mShapeBlueprints.insert(id++, c);
	mShapeBlueprints.insert(id++, d);
	mShapeBlueprints.insert(id++, e);
	mShapeBlueprints.insert(id++, f);
	mShapeBlueprints.insert(id++, g);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CompoundShape::CompoundShape(
							 ShapeBlueprint* a, 
							 ShapeBlueprint* b, 
							 ShapeBlueprint* c, 
							 ShapeBlueprint* d, 
							 ShapeBlueprint* e, 
							 ShapeBlueprint* f, 
							 ShapeBlueprint* g, 
							 ShapeBlueprint* h)
{
	NxShapeIndex id = mShapeBlueprints.count(); 
	mShapeBlueprints.insert(id, a);
	mShapeBlueprints.insert(id++, b);
	mShapeBlueprints.insert(id++, c);
	mShapeBlueprints.insert(id++, d);
	mShapeBlueprints.insert(id++, e);
	mShapeBlueprints.insert(id++, f);
	mShapeBlueprints.insert(id++, g);
	mShapeBlueprints.insert(id++, h);
}

////////////////////////////////////////////////////////////////////////////////////////////////

CompoundShape::~CompoundShape() {
	// ShapeBlueprints should be already deleted when bound to an actor.

	// temp
	std::cout << "Compound Shape destructor" << std::endl;
	mShapeBlueprints.destroyAndEraseAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void CompoundShape::add(ShapeBlueprint* sb) {
	mShapeBlueprints.insert(mShapeBlueprints.count(), sb);
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* CompoundShape::_bindToActorDescription(Actor* actor, NxU32 id, NxArray<NxShapeDesc*>& desc) {
	

	for (ShapeBlueprint* shape = mShapeBlueprints.begin();shape = mShapeBlueprints.next();) {
		shape->_bindToActorDescription(actor, id, desc);
		id++;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* CompoundShape::_bindToNxActor(Actor*, NxShapeIndex) {
	NxUnderConstruction;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
