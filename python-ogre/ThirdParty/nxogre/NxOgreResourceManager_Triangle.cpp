/** \file    NxOgreResourceManager_Triangle.cpp
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

NxTriangleMesh*	ResourceManager::getTriangleMesh(MeshIdentifier identifier) {
	if (mTriangleMeshes.has(identifier)) {
		return mTriangleMeshes.get(identifier);
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::loadTriangleMesh(MeshIdentifier identifier, ResourceStreamPtr resource) {
	
	NxTriangleMesh* triangleMesh = 0;
	NxPhysicsSDK*	sdk = NxGetPhysicsSDK();
	if (sdk == 0) {
		NxThrow("PhysX SDK has not been started");	
		return false; 
	}
	triangleMesh = sdk->createTriangleMesh(*resource);
	
	if (triangleMesh) {
		mTriangleMeshes.insert(identifier, triangleMesh);
	}
	else {
		NxThrow(NxString("TriangleMesh '" + identifier + "' could not be loaded.").c_str());
		return false;
	}

	resource->close();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::loadTriangleMesh(MeshIdentifier identifier, NxTriangleMesh* triangleMesh) {
	
	if (triangleMesh) {
		mTriangleMeshes.insert(identifier, triangleMesh);
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::saveTriangleMesh(MeshIdentifier identifier, ResourceStreamPtr resource) {
	
	if (!mTriangleMeshes.has(identifier)) {
		NxDebug(NxString("TriangleMesh with name '" + identifier + "' could not be found.").c_str());
		return false;
	}

	NxTriangleMesh* triangleMesh = mTriangleMeshes.get(identifier);
	NxTriangleMeshDesc description;
	triangleMesh->saveToDesc(description);
	
	bool status = NxCookTriangleMesh(description, *resource);
	resource->close();
	// Free description?

	return status;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::cookTriangleMesh(TriangleMeshIntermediary* tmi, ResourceStreamPtr resource) {
	
	if (!tmi->mDescription.isValid()) {

		std::string ss("Triangle Mesh is invalid. Reason(s) are: \n");
		
		if(tmi->mDescription.numVertices < 3)
			ss.append("+ Vertex count less than three");
		
		if((!tmi->mDescription.triangles) && (tmi->mDescription.numVertices%3))
			ss.append("+ Number of vertices cannot be divided by three, no triangles can be made.");

		if(tmi->mDescription.materialIndices && tmi->mDescription.materialIndexStride < sizeof(NxMaterialIndex))
			ss.append("+ Material index stride is not the size of a material index");


		NxThrow_AsWarning(ss.c_str());

		return false;

	}

	bool status = NxCookTriangleMesh(tmi->mDescription, *resource);
	
	return status;
}

////////////////////////////////////////////////////////////////////////////////////////////////

TriangleMeshIntermediary* ResourceManager::generateTriangleMeshDescription(Ogre::MeshPtr meshPtr, MaterialAlias* alias, NxVec3 scale) {

	Ogre::Mesh*		mesh			= meshPtr.getPointer();
	unsigned int	nbVertices		= 0,
					nbIndices		= 0;
	bool			added_shared	= false;
	bool			use32bitindexes	= false;


	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {

		Ogre::SubMesh* submesh = mesh->getSubMesh(i);
		
		if(submesh->useSharedVertices) {
			if(!added_shared) {
				nbVertices += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else {
			nbVertices += submesh->vertexData->vertexCount;
		}
		nbIndices  += submesh->indexData->indexCount;
	}

	TriangleMeshIntermediary* tmi = new TriangleMeshIntermediary();
	tmi->setToNormal();
	
	//NxVec3*				vertices				= new NxVec3[nbVertices];
	tmi->createVertices(nbVertices);

	//NxU32*				triangles				= new NxU32[nbIndices];
	tmi->createIndices(nbIndices);

	//NxMaterialIndex*	materialIndexes			= new NxMaterialIndex[nbIndices];
	tmi->createMaterialIndices(nbIndices);

	NxMaterialIndex		currentMaterialIndex	= 0;
	added_shared								= false;

	size_t				current_offset			= 0, 
						shared_offset			= 0,
						next_offset				= 0,
						index_offset			= 0;

	for (unsigned short i=0; i < mesh->getNumSubMeshes();i++) {

		Ogre::SubMesh*		submesh		= mesh->getSubMesh(i);
		Ogre::VertexData*	vertex_data	= submesh->useSharedVertices	?	  mesh->sharedVertexData
																			: submesh->vertexData;

		if (alias) {
			currentMaterialIndex = alias->get(submesh->getMaterialName());
		}
		else {
			// Perhaps the mesh name could be looked up as a material.
			currentMaterialIndex = 0;
		}

		if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared)) {

			if(submesh->useSharedVertices) {
				added_shared	= true;
				shared_offset	= current_offset;
			}

			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			Ogre::Real* pReal;

			for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) {
				posElem->baseVertexPointerToElement(vertex, &pReal);
				tmi->mVertices[current_offset + j] = NxVec3(pReal[0]*scale.x, pReal[1]*scale.y, pReal[2]*scale.z); 
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}

		Ogre::IndexData* index_data = submesh->indexData;

		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);


		if ( use32bitindexes )	{
			unsigned int*  pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

			for (size_t k = 0; k < numTris*3; ++k) {
				tmi->mIndices[index_offset] = pInt[k] + static_cast<unsigned int>(offset);			
				tmi->mMaterialIndices[index_offset++] = currentMaterialIndex;
			}
		}
		else {

			unsigned short* pShort = reinterpret_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

			for ( size_t k = 0; k < numTris*3; ++k) {
				tmi->mIndices[index_offset] = static_cast<unsigned int>(pShort[k]) + static_cast<unsigned int>(offset);
				tmi->mMaterialIndices[index_offset++] = currentMaterialIndex;
			}

		}

		ibuf->unlock();
		current_offset = next_offset;
	}


/*	NxTriangleMeshDesc desc;

	desc.numVertices				= nbVertices;
	desc.points						= vertices;							
	desc.pointStrideBytes			= sizeof(NxVec3);
	desc.numTriangles				= nbIndices / 3;
	desc.triangles					= triangles;
	desc.triangleStrideBytes		= 3 * sizeof(NxU32);
	desc.materialIndexStride		= sizeof(NxMaterialIndex);
	desc.materialIndices			= materialIndexes;

	if (!desc.isValid()) {
		NxThrow_Warning("Ogre Mesh '" + mesh->getName() + "' produces an invalid NxTriangleMeshDescription!");
	}
*/
	tmi->verticesToDescription();
	tmi->indicesToDescription();
	tmi->materialIndicesToDescription();

	return tmi;

}

////////////////////////////////////////////////////////////////////////////////////////////////

TriangleMeshIntermediary* ResourceManager::generateTriangleMeshDescription(NxTriangleMesh*, MaterialConversionList, NxVec3 scale) {

#if 0
	NxTriangleMeshDesc desc;

	return desc;
#endif 

	return 0;

}

////////////////////////////////////////////////////////////////////////////////////////////////


#undef NX_TEMP_COOKINGFILENAME
#undef NX_TEMP_COOKINGFILENAME_WRITE

}; //End of NxOgre namespace.
