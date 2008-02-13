#if 0
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
#include "NxOgreShapeMesh.h"
#include "NxOgreShapeBlueprintMesh.h"		// For: TriangleMeshShape
#include "NxOgreActor.h"

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

TriangleMesh::TriangleMesh(TriangleMeshShape& desc, Actor* actor, NxArray<NxShapeDesc*>& shapes) : Shape(actor) {
	desc.mShapeDescription.userData = this;
}

////////////////////////////////////////////////////////////////////////////////////////////////

TriangleMesh::~TriangleMesh() {
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMesh::_bindNxShapeToShape(NxShape* s) {
		
	mBaseShape = s;

	if (s->isTriangleMesh()) {
		mShape = static_cast<NxTriangleMeshShape*>(s);
	}
	else {
		NxThrow_Warning("Attempted to bind wrong NxShape to NxOgre shape");
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
#endif
