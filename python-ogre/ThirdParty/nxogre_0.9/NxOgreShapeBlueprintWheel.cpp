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
#include "NxOgreShapeBlueprintWheel.h"
#include "NxOgreActor.h"			// For: Actor binding
#include "NxOgreShapeWheel.h"		// For: Convex
#include "NxOgreHelpers.h"			// For: Conversions
#include "NxOgreScene.h"			// For: Materials

namespace NxOgre {






//									Wheel





WheelShape::WheelShape(NxReal radius, const ShapeParams& p, const WheelParams& wp) : ShapeBlueprint(p) {

	mShapeDescription.setToDefault();
	mShapeDescription.radius = radius;
	
	wheelParams = wp;
}

////////////////////////////////////////////////////////////////////////////////////////////////

WheelShape::~WheelShape() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* WheelShape::_bindToActorDescription(Actor* actor, NxU32 id, NxArray<NxShapeDesc*>& shapes) {
	NxUnderConstruction;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

Shape* WheelShape::_bindToNxActor(Actor* actor, NxShapeIndex id) {
	
	__paramsToDescription(mShapeDescription, mParams, actor->getScene());

	Wheel* wheel = new Wheel(*this, wheelParams, actor);
	
	actor->mDynamicCollisionModel.insert(actor->mDynamicCollisionModel.count(), wheel);
	actor->mCollisionModel.insert(id, wheel);
	actor->mCollisionModel.lock(id, true);
	
	NxShape* s = actor->getNxActor()->createShape(mShapeDescription);

	wheel->_bindNxShapeToShape(s);

	return wheel;
}



}; //End of NxOgre namespace.
