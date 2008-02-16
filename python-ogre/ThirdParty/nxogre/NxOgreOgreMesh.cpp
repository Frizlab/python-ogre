/** \file    NxOgreOgreMeshStream.cpp
 *  \see     NxOgreOgreMeshStream.h
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
#include "NxOgreOgreMesh.h"
#include "NxOgreTransitionalMesh.h"
#include "NxOgreResourceStream.h"
#include "NxOgreOgreResourceStream.h"
#include "BetajaenCC.h"

#include "OgreMesh.h"
#include "OgreMeshManager.h"
#include "OgreSubMesh.h"

namespace NxOgre {

/////////////////////////////////////////////////////////////

OgreMesh::OgreMesh(ResourceStreamPtr resourceStream) : TransitionalMesh(resourceStream) {
	
	// Ogre works a little funny, or not really the same that NxOgre does. So the
	// DataStream that Ogre provide cannot be turned into the Resource that a mesh
	// would be.

	OgreResourceStream* ogre_stream = static_cast<OgreResourceStream*>(mStream.getPointer());
	NxString OgreMeshName = ogre_stream->data->getName();
	mMesh = Ogre::MeshManager::getSingletonPtr()->getByName(OgreMeshName);
	
}

/////////////////////////////////////////////////////////////

OgreMesh::~OgreMesh() {
	// Parent Destructor will close the stream and mMesh does not need to be deleted.
}

/////////////////////////////////////////////////////////////

void OgreMesh::load(bool indices, bool textureCoords, bool normals, bool materials) {

	empty();

	size_t currentOffset = 0, sharedOffset = 0, nextOffset = 0;

	if (!mMesh->isLoaded())
		mMesh->load();

	if (mMesh->getSubMesh(0)->useSharedVertices) {
		mNbVertices = mMesh->sharedVertexData->vertexCount;
	}
	else {
		for (Ogre::ushort i = 0;i < mMesh->getNumSubMeshes(); ++i) {
			Ogre::SubMesh* subMesh = mMesh->getSubMesh(i);
			if (!subMesh->useSharedVertices) {
				mNbVertices += subMesh->vertexData->vertexCount;
			}
		}
	}

	_reserveVertices(mNbVertices);

	bool addedShared = false;

	for (Ogre::uint i = 0; i < mMesh->getNumSubMeshes();i++) {
		
		Ogre::SubMesh* subMesh = mMesh->getSubMesh(i);
		Ogre::VertexData* vertexData = subMesh->useSharedVertices ? mMesh->sharedVertexData : subMesh->vertexData;
		
		if ((!subMesh->useSharedVertices) || (subMesh->useSharedVertices && !addedShared)) {
			
			if (subMesh->useSharedVertices) {
				addedShared = true;
				sharedOffset = currentOffset;
			}


			const Ogre::VertexElement* vPosElem = vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			Ogre::HardwareVertexBufferSharedPtr vBuf = vertexData->vertexBufferBinding->getBuffer(vPosElem->getSource());
			unsigned char* vertex = static_cast<unsigned char*>(vBuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			// Normal:-
			//		nPosElement
			//		vertexData->vertexDeclaration->findElementBySemantic
			
			// Texture Coordinates
			//		tPosElement
			//		....

			Ogre::Real* pReal;

			for(size_t j=0;j < vertexData->vertexCount; ++j, vertex += vBuf->getVertexSize()) {
				vPosElem->baseVertexPointerToElement(vertex, &pReal);
				mVertices[currentOffset + j].i = pReal[0];
				mVertices[currentOffset + j].j = pReal[1];
				mVertices[currentOffset + j].k = pReal[2];

				// Normals
				// ...
				// Texture Coordinates
				// ...
			}

			vBuf->unlock();
			nextOffset += vertexData->vertexCount;

		}
	
		currentOffset = nextOffset;

	}


	

}

/////////////////////////////////////////////////////////////

void OgreMesh::save(bool indices, bool textureCoords, bool normals, bool materials) {

}

/////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
