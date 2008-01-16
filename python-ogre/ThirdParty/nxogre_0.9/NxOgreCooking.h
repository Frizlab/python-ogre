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

#ifndef __NXOGRE_COOKING_H__
#define __NXOGRE_COOKING_H__

#include "NxOgrePrerequisites.h"

namespace NxOgre {

	// Convex
	NxExport NxConvexMesh* NxGenerateConvexMeshFromOgreMesh(const NxString& meshname, NxScene *scene, NxVec3 scale = NxVec3(1,1,1), const NxString& meshGroup = "General");
	NxExport NxConvexMesh* NxGenerateConvexMeshFromVertices(NxVec3 *verts, NxU32 nbVerts, NxScene *scene);
	
	// TriangleMeshShape
	NxExport NxTriangleMesh* NxGenerateTriangleMeshFromOgreMesh(const NxString&, NxScene *scene, NxVec3 scale = NxVec3(1,1,1));
	NxExport NxTriangleMesh* NxGenerateTriangleMeshFromVertices(NxVec3 *verts, NxU32 nbVerts, NxU32* indices, NxU32 nbIndices, NxScene*);

	// Heightfields
	NxExport NxHeightField* NxGenerateHeightFieldFromImage(const NxString& imageFilename, NxScene*);
	NxExport NxHeightField*	NxGenerateHeightFieldFromRaw(const NxString& imageFilename, NxScene*);

};

#endif
