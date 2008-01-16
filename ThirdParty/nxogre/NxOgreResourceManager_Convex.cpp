

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
#include "NxOgreResourceManager.h"
#include "NxOgreResourceSystem.h"
#include "NxOgreMaterial.h"
#include "NxOgreMemoryStream.h"
#include "NxOgreHelpers.h"
#include "NxOgreResourceMesh.h"

#include "NxCooking.h"

#include "OgreMesh.h"
#include "OgreSubMesh.h"

namespace NxOgre {

#define NX_TEMP_COOKINGFILENAME "file://NxOgre.tmp"
#define NX_TEMP_COOKINGFILENAME_WRITE "file://NxOgre.tmp +write"

////////////////////////////////////////////////////////////////////////////////////////////////

NxConvexMesh* ResourceManager::getConvexMesh(MeshIdentifier identifier) {
	if (mConvexMeshes.has(identifier)) {
		return mConvexMeshes.get(identifier);
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::loadConvexMesh(MeshIdentifier identifier, ResourceStreamPtr resource) {
	
	NxConvexMesh* convexMesh = 0;
	NxPhysicsSDK* sdk = NxGetPhysicsSDK();
	
	if (sdk == 0) {
		NxThrow_Error("PhysX SDK has not been started");	
		return false; 
	}

	convexMesh = sdk->createConvexMesh(*resource);

	if (convexMesh) {
		mConvexMeshes.insert(identifier, convexMesh);
	}
	else {
		NxThrow_Error("ConvexMesh '" + identifier + "' could not be loaded.");
		return false;
	}

	resource->close();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::loadConvexMesh(MeshIdentifier identifier, NxConvexMesh* convexMesh) {
	if (convexMesh) {
		mConvexMeshes.insert(identifier, convexMesh);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::saveConvexMesh(MeshIdentifier identifier, ResourceStreamPtr resource) {
	
	if (!mConvexMeshes.has(identifier)) {
		NxDebug("ConvexMesh with name '" + identifier + "' could not be found.");
		return false;
	}

	NxConvexMesh* convexMesh = mConvexMeshes.get(identifier);
	NxConvexMeshDesc description;
	convexMesh->saveToDesc(description);
	
	bool status = NxCookConvexMesh(description, *resource);
	resource->close();
	// Free description?

	return status;
}

////////////////////////////////////////////////////////////////////////////////////////////////

ConvexMeshIntermediary* ResourceManager::generateConvexMeshDescription(Ogre::MeshPtr meshPtr, NxVec3 scale) {
	
	Ogre::Mesh* mesh = meshPtr.getPointer();

	ConvexMeshIntermediary* cmi = new ConvexMeshIntermediary();
	cmi->setToNormal();

	NxU32	RawVertexCount = 0;
	NxVec3  *RawVertices;

	bool added_shared = false;
	size_t current_offset = 0, shared_offset = 0, next_offset = 0;

	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
		
		Ogre::SubMesh* submesh = mesh->getSubMesh( i );

		if(submesh->useSharedVertices) {
			if( !added_shared ) {
				RawVertexCount += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else {
			RawVertexCount += submesh->vertexData->vertexCount;
		}

	}

	RawVertices = new NxVec3[RawVertexCount];

	for ( unsigned short i = 0; i < mesh->getNumSubMeshes();i++) {

		Ogre::SubMesh* submesh = mesh->getSubMesh(i);
		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

		if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared)) {

			if(submesh->useSharedVertices) {
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			Ogre::Real* pReal;

			for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) {
				posElem->baseVertexPointerToElement(vertex, &pReal);
				RawVertices[current_offset + j] = NxVec3(pReal[0] * scale.x,pReal[1] * scale.y, pReal[2] * scale.z);
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}

		current_offset = next_offset;
	}

	// Removed duplicate vertices.
	cmi->mVertices = new NxVec3[RawVertexCount];
	
	NxVec3 vertex;
	bool d=false;
	
	for (NxU32 i=0;i < RawVertexCount;++i) {
		
		vertex = RawVertices[i];
		d = false;
		
		for (NxU32 j=0;j < cmi->mNbVertices;++j) {
			if (vertex == cmi->mVertices[j]) {
				d = true;
			}
		}

		if (!d)
			cmi->mVertices[cmi->mNbVertices++] = vertex;

	}


	delete []RawVertices;

	// Scale vertices.
	if (scale != NxVec3(1,1,1)) {
		for (NxU32 i=0;i < cmi->mNbVertices;++i) {
			cmi->mVertices[i].x = cmi->mVertices[i].x * scale.x;
			cmi->mVertices[i].y = cmi->mVertices[i].y * scale.x;
			cmi->mVertices[i].z = cmi->mVertices[i].z * scale.x;
		}
	}
	
	cmi->verticesToDescription();

	return cmi;

}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::cookConvexMesh(ConvexMeshIntermediary* cmi, ResourceStreamPtr resource) {
		
	if (!cmi->mDescription.isValid()) {
		std::string ss;
		
		if(cmi->mDescription.numVertices < 3 ||	(cmi->mDescription.numVertices > 0xffff && cmi->mDescription.flags & NX_CF_16_BIT_INDICES))
			ss.append("+ Vertex count less than three");
		
		if(!cmi->mDescription.points)
			ss.append("+ No Vertices");

		if(cmi->mDescription.pointStrideBytes < sizeof(NxPoint))
			ss.append("+ PointStride is less than NxPoint");


		NxThrow_Warning("ConvexMesh is invalid. Reason(s) are: \n" + ss);
	}

	bool status = NxCookConvexMesh(cmi->mDescription, *resource);
	
	return status;

}

////////////////////////////////////////////////////////////////////////////////////////////////

NxConvexMesh* ResourceManager::cookConvexMesh(NxConvexMesh* mesh, NxReal scale) {

	NxConvexMeshDesc description;
	mesh->saveToDesc(description);

	ResourceStreamPtr originalCookingStream(NxMemoryStreamIdentifier);
	NxCookConvexMesh(description, *originalCookingStream);
	originalCookingStream->rewind();

	ResourceStreamPtr scaledCookingStream(NxMemoryStreamIdentifier);
	NxScaleCookedConvexMesh(*originalCookingStream, scale, *scaledCookingStream); 
	scaledCookingStream->rewind();

	NxPhysicsSDK* sdk = NxGetPhysicsSDK();
	NxConvexMesh* scaledConvex = sdk->createConvexMesh(*scaledCookingStream);

	originalCookingStream->close();
	scaledCookingStream->close();

	return scaledConvex;
}

////////////////////////////////////////////////////////////////////////////////////////////////

#undef NX_TEMP_COOKINGFILENAME
#undef NX_TEMP_COOKINGFILENAME_WRITE

}; //End of NxOgre namespace.
