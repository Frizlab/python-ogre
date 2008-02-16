/** \file    NxOgreResourceManager_Cloth.cpp
 *  \see     NxOgreResourceManager.h
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

#if (NX_USE_CLOTH_API == 1)

#include "NxOgreResourceManager.h"
#include "NxOgreResourceSystem.h"
#include "NxOgreMaterial.h"
#include "NxOgreMemoryStream.h"
#include "NxOgreHelpers.h"
#include "NxOgreResourceMesh.h"
#include "NxOgreDynamicMesh.h"

#include "NxCooking.h"

#include "OgreMesh.h"
#include "OgreSubMesh.h"

namespace NxOgre {

#define NX_TEMP_COOKINGFILENAME "file://NxOgre.tmp"
#define NX_TEMP_COOKINGFILENAME_WRITE "file://NxOgre.tmp +write"

////////////////////////////////////////////////////////////////////////////////////////////////
	
DynamicClothMesh* ResourceManager::getDynamicClothMesh(MeshIdentifier identifier) {
	if (mClothMeshes.has(identifier)) {
		return mClothMeshes.get(identifier);
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////

NxClothMesh* ResourceManager::getClothMesh(MeshIdentifier identifier) {
	if (mClothMeshes.has(identifier)) {
		return mClothMeshes.get(identifier)->getMeshPtr();
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::loadClothMesh(MeshIdentifier identifier, ResourceStreamPtr resource) {
	
	NxClothMesh* clothMesh = 0;
	NxPhysicsSDK* sdk = NxGetPhysicsSDK();
	
	if (sdk == 0) {
		NxThrow("PhysX SDK has not been started");	
		return false; 
	}

	clothMesh = sdk->createClothMesh(*resource);

	if (clothMesh) {
		
		DynamicClothMesh* dcm = new DynamicClothMesh(clothMesh);
		mClothMeshes.insert(identifier, dcm);

	}
	else {
		NxThrow(NxString("ClothMesh '" + identifier + "' could not be loaded.").c_str());
		return false;
	}

	resource->close();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::loadClothMesh(MeshIdentifier identifier, DynamicClothMesh* dcm) {
	
	if (dcm) {
		mClothMeshes.insert(identifier, dcm);
		return true;
	}
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::loadClothMesh(MeshIdentifier identifier, NxClothMesh* clothMesh) {
	if (clothMesh) {
		DynamicClothMesh* dcm = new DynamicClothMesh(clothMesh);
		mClothMeshes.insert(identifier, dcm);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::saveClothMesh(MeshIdentifier identifier, ResourceStreamPtr resource) {
	
	if (!mClothMeshes.has(identifier)) {
		NxThrow_AsWarning(NxString("ClothMesh with name '" + identifier + "' could not be found.").c_str());
		return false;
	}

	NxClothMesh* clothMesh = mClothMeshes.get(identifier)->getMeshPtr();
	NxClothMeshDesc description;
	clothMesh->saveToDesc(description);
	
	bool status = NxCookClothMesh(description, *resource);
	resource->close();
	// Free description?

	return status;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::cookClothMesh(ClothMeshIntermediary* cmi, ResourceStreamPtr resource) {
	
	if (!cmi->mDescription.isValid()) {
		std::string ss("ClothMesh is invalid. Reason(s) are: \n");

		if(cmi->mDescription.vertexMasses && (cmi->mDescription.vertexMassStrideBytes < sizeof(NxReal)))
			ss.append("+ Vertex Mass stride size is less than the size of NxReal");

		if(cmi->mDescription.vertexFlags && (cmi->mDescription.vertexFlagStrideBytes < sizeof(NxU32)))
			ss.append("+ Vertex Flag stride size is less than the size of NxReal");
		
		NxThrow_AsWarning(ss.c_str());
	}

	bool status = NxCookClothMesh(cmi->mDescription, *resource);
	
	return status;

}

////////////////////////////////////////////////////////////////////////////////////////////////


ClothMeshIntermediary* ResourceManager::generateClothMesh(NxReal width, NxReal height, NxReal verticesPerRow, NxReal verticesPerColumn)  {

	ClothMeshIntermediary* cmi = new ClothMeshIntermediary();
	cmi->setToNormal();

	NxU32 nbX = (NxU32) NxReal(width * verticesPerRow) + 1;
	NxU32 nbY = (NxU32) NxReal(height * verticesPerColumn) + 1;
	NxU32 nbVertices = (nbX + 1) * (nbY + 1);
	NxU32 nbTriangles = nbX * nbY * 2;
	
	cmi->createVertices(nbVertices);
	cmi->createIndices(nbTriangles * 3);

	// Vertices
	{
		NxU32 i = 0, j = 0;
		NxVec3* p = cmi->mVertices;
		
		for (i = 0; i <= nbY; i++) {
			for (j = 0; j <= nbX; j++) {
				p->set(j * verticesPerRow, 0, i * verticesPerColumn);
				*p++;
			}
		}
		cmi->verticesToDescription();
	}

	// Triangles
	{
		NxU32 i = 0, j = 0, i0 = 0, i1 = 0, i2 = 0, i3 = 0;
		NxU32 *p = cmi->mIndices;
		for (i = 0; i < nbY; i++) {
			for (j = 0; j < nbX; j++) {

				i0 = i * (nbX + 1) + j;
				i1 = i0 + 1;
				i2 = i0 + (nbX + 1);
				i3 = i2 + 1;

				if ( (j+1) % 2) {
					*p++ = i0; *p++ = i2; *p++ = i1;
					*p++ = i1; *p++ = i2; *p++ = i3;
				}
				else {
					*p++ = i0; *p++ = i2; *p++ = i3;
					*p++ = i0; *p++ = i3; *p++ = i1;
				}
			}
		}

		cmi->indicesToDescription();
	}

	return cmi;
}

////////////////////////////////////////////////////////////////////////////////////////////////

#if (NX_USE_OGRE == 1)

ClothMeshIntermediary* ResourceManager::generateClothMesh(Ogre::MeshPtr*, NxVec3 scale)  {
	
	// TEMP.
	
	return NULL;

}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////



#undef NX_TEMP_COOKINGFILENAME
#undef NX_TEMP_COOKINGFILENAME_WRITE


}; //End of NxOgre namespace.

#endif
