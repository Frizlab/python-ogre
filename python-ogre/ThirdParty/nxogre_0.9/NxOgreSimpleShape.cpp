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
#include "NxOgreSimpleShape.h"

namespace NxOgre {

//////////////////////////////////////////////////////////////////////////////////////

SimpleBox* SimpleShape::getAsBox() {
	return static_cast<SimpleBox*>(this);
}

//////////////////////////////////////////////////////////////////////////////////////

SimpleSphere* SimpleShape::getAsSphere() {
	return static_cast<SimpleSphere*>(this);
}

//////////////////////////////////////////////////////////////////////////////////////

SimpleCapsule* SimpleShape::getAsCapsule() {
	return static_cast<SimpleCapsule*>(this);
}

//////////////////////////////////////////////////////////////////////////////////////

SimpleConvex* SimpleShape::getAsConvex() {
	return static_cast<SimpleConvex*>(this);
}

//////////////////////////////////////////////////////////////////////////////////////

SimplePlane* SimpleShape::getAsPlane() {
	return static_cast<SimplePlane*>(this);
}

//////////////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
