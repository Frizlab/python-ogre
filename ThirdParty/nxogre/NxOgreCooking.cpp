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
#include "NxOgreCooking.h"
#include "NxOgreUserStream.h"
#include "NxCooking.h"
#include "NxOgreHelpers.h"

namespace NxOgre {

NxConvexMesh* NxGenerateConvexMeshFromOgreMesh(const NxString& meshname, NxScene *scene, NxVec3 scale, const NxString& meshGroup) {

	NxU32	RawVertexCount = 0;
	NxU32	SafeVertexCount = 0;
	NxVec3*	RawVertices;
	NxVec3* SafeVertices;

	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;

	
	Ogre::Root::getSingleton().getMeshManager()->load(meshname, meshGroup);
	Ogre::Mesh* mesh = reinterpret_cast<Ogre::Mesh *>(Ogre::Root::getSingleton().getMeshManager()->getByName(meshname).get() );

	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
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

	// Safe-Vertices
	SafeVertices = new NxVec3[RawVertexCount];
	{
		NxVec3 vertex;
		bool d=false;
		for (NxU32 i=0;i < RawVertexCount;++i) {
			vertex = RawVertices[i];
			d = false;
			for (NxU32 j=0;j < SafeVertexCount;++j) {
				if (vertex == SafeVertices[j]) {
					d = true;
				}
			}
			if (!d)	SafeVertices[SafeVertexCount++] = vertex;
		}
	}

	delete []RawVertices;

	NxConvexMeshDesc mMeshDesc;

	mMeshDesc.numVertices = SafeVertexCount;
	mMeshDesc.pointStrideBytes = sizeof(NxVec3);
	mMeshDesc.points = SafeVertices;
	mMeshDesc.flags = NX_CF_COMPUTE_CONVEX;// | NX_CF_USE_LEGACY_COOKER;
	
	NxConvexMesh* cm;

#ifndef NX_DEBUG

	MemoryWriteBuffer buf;
	bool status = NxCookConvexMesh(mMeshDesc,buf);

	if (!status)
		NxDebug("Convex '" + meshname + "' failed to cook.");

	cm = scene->getPhysicsSDK().createConvexMesh(MemoryReadBuffer(buf.data));

#else

	NxString filename;
	if (Ogre::StringUtil::endsWith(meshname, ".mesh")) {
		filename = meshname.substr(0, meshname.length() - 5) + ".Convex.nxs";	
	}
	else {
		filename = meshname + ".Convex.nxs";
	}


	UserStream buf(filename.c_str(),false);
	bool status = NxCookConvexMesh(mMeshDesc, buf);

	if (!status) {
		std::stringstream s;
		s	<< "Convex '" << meshname << "' failed to cook" << std::endl
			<< "Vertices = " << SafeVertexCount;

		NxThrow_Error(s.str());
	}

	fclose(buf.fp);

	UserStream rbuf(filename.c_str(), true);

	cm = scene->getPhysicsSDK().createConvexMesh(rbuf);

	fclose(rbuf.fp);

	
#endif

	delete []SafeVertices;

	return cm;

}

////////////////////////////////////////////////////////////////////////////////////////////////////

NxConvexMesh* NxGenerateConvexMeshFromVertices(NxVec3 *verts, unsigned int nbVerts, NxScene *scene) {

	// Create descriptor for gear center convex mesh

	NxConvexMeshDesc mMeshDesc;
	mMeshDesc.numVertices			= nbVerts;
	mMeshDesc.pointStrideBytes		= sizeof(NxVec3);
	mMeshDesc.points				= verts;
	mMeshDesc.flags					= NX_CF_COMPUTE_CONVEX;// | NX_CF_USE_LEGACY_COOKER;

#ifndef NX_DEBUG

	MemoryWriteBuffer buf;
	bool status = NxCookConvexMesh(mMeshDesc,buf);

	if (!status)
		NxDebug("Convex failed to cook.");

	return scene->getPhysicsSDK().createConvexMesh(MemoryReadBuffer(buf.data));

#else

	NxString filename;
	
	filename = Ogre::StringConverter::toString(nbVerts) + "-" + Ogre::StringConverter::toString(int(verts[0].x)) + "-" + Ogre::StringConverter::toString(int(verts[0].z)) + ".ConvexShape.nxs";

	UserStream buf(filename.c_str(),false);
	bool status = NxCookConvexMesh(mMeshDesc, buf);

	if (!status)
		NxDebug("Convex failed to cook.");

	fclose(buf.fp);

	UserStream rbuf(filename.c_str(), true);

	NxConvexMesh* cm = scene->getPhysicsSDK().createConvexMesh(rbuf);

	fclose(rbuf.fp);

	return cm;
#endif

}

////////////////////////////////////////////////////////////////////////////////////////////////////

NxTriangleMesh* NxGenerateTriangleMeshFromOgreMesh(const NxString& meshName, NxScene *scene, NxVec3 scale) {


	unsigned int	mVertexCount = 0, mIndexCount  = 0;
	size_t			vertex_count = 0;
	Ogre::Vector3*	vertices;
	size_t			index_count = 0;
	unsigned long*	indices;

	bool added_shared = false;

	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	Ogre::Root::getSingleton().getMeshManager()->load(meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::Mesh* mesh = reinterpret_cast<Ogre::Mesh *>(Ogre::Root::getSingleton().getMeshManager()->getByName(meshName).get());

	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
		Ogre::SubMesh* submesh = mesh->getSubMesh( i );
		if(submesh->useSharedVertices) {
			if( !added_shared ) {
				mVertexCount += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else {
			mVertexCount += submesh->vertexData->vertexCount;
		}

		mIndexCount += submesh->indexData->indexCount;
	}


	vertices = new Ogre::Vector3[mVertexCount];
	indices = new unsigned long[mIndexCount];

	NxVec3* mMeshVertices = new NxVec3[mVertexCount];
	NxU32* mMeshFaces = new NxU32[mIndexCount];
	NxMaterialIndex* mMaterials = new NxMaterialIndex[mIndexCount];

	NxMaterialIndex currentMaterialIndex = 0;
	added_shared = false;
	bool use32bitindexes;

	for (unsigned short i=0;i<mesh->getNumSubMeshes();i++) {

		Ogre::SubMesh* submesh = mesh->getSubMesh(i);
		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

		// Material Aliases..

		//currentMaterialIndex = _scene->findMaterialIndex(submesh->getMaterialName());
		// Temp...
		currentMaterialIndex = 0;

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
				mMeshVertices[current_offset + j] = NxVec3(pReal[0]*scale.x, pReal[1]*scale.y, pReal[2]*scale.z); 
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

			for ( size_t k = 0; k < numTris*3; ++k) {
				mMeshFaces[index_offset] = pInt[k] + static_cast<unsigned int>(offset);			
				mMaterials[index_offset++] = currentMaterialIndex;

			}
		}
		else {

			unsigned short* pShort = reinterpret_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

			for ( size_t k = 0; k < numTris*3; ++k) {
				mMeshFaces[index_offset] = static_cast<unsigned int>(pShort[k]) + static_cast<unsigned int>(offset);
				mMaterials[index_offset++] = currentMaterialIndex;

			}

		}

		ibuf->unlock();
		current_offset = next_offset;
	}


	NxTriangleMeshDesc mTriangleMeshDescription;

	// Vertices
	mTriangleMeshDescription.numVertices				= mVertexCount;
	mTriangleMeshDescription.points						= mMeshVertices;							
	mTriangleMeshDescription.pointStrideBytes			= sizeof(NxVec3);
	// Triangles
	mTriangleMeshDescription.numTriangles				= mIndexCount / 3;
	mTriangleMeshDescription.triangles					= mMeshFaces;
	mTriangleMeshDescription.triangleStrideBytes		= 3 * sizeof(NxU32);
	// Materials
	//#if 0
	mTriangleMeshDescription.materialIndexStride		= sizeof(NxMaterialIndex);

	mTriangleMeshDescription.materialIndices			= mMaterials;
	//#endif
	//mTriangleMeshDescription.flags					= NX_MF_HARDWARE_MESH;

	NxTriangleMesh* trimesh;

#ifndef NX_DEBUG

	MemoryWriteBuffer buf;
	if (!NxCookTriangleMesh(mTriangleMeshDescription, buf)) {
		std::stringstream s;
		s	<< "Mesh '" << meshName << "' failed to cook"
			<< "V(" << mMeshVertices << ") F(" << mMeshFaces << ")";

		NxThrow_Error(s.str());
	}
	trimesh = scene->getPhysicsSDK().createTriangleMesh(MemoryReadBuffer(buf.data));

#else

	NxString filename;
	if (Ogre::StringUtil::endsWith(meshName, ".mesh")) {
		filename = meshName.substr(0, meshName.length() - 5) + ".TriangleMeshShape.nxs";	
	}
	else {
		filename = meshName + ".TriangleMeshShape.nxs";
	}

	UserStream buf(filename.c_str(),false);

	if (!NxCookTriangleMesh(mTriangleMeshDescription, buf)) {
		std::stringstream s;
		s	<< "Mesh '" << meshName << "' failed to cook"
			<< "V(" << mMeshVertices << ") F(" << mMeshFaces << ")";

		NxThrow_Error(s.str());
	}

	fclose(buf.fp);

	UserStream rbuf(filename.c_str(), true);

	trimesh = scene->getPhysicsSDK().createTriangleMesh(rbuf);

	fclose(rbuf.fp);



#endif

	delete []vertices;
	delete []indices;

	delete []mMeshVertices;
	delete []mMeshFaces;
	delete []mMaterials;

	return trimesh;

}

////////////////////////////////////////////////////////////////////////////////////////////////////

NxTriangleMesh* NxGenerateTriangleMeshFromVertices(NxVec3 *verts, NxU32 nbVerts, NxU32* indices, NxU32 nbIndices, NxScene* scene) {

	NxTriangleMeshDesc mTriangleMeshDescription;

	// Vertices
	mTriangleMeshDescription.numVertices				= nbVerts;
	mTriangleMeshDescription.points						= verts;							
	mTriangleMeshDescription.pointStrideBytes			= sizeof(NxVec3);
	// Triangles
	mTriangleMeshDescription.numTriangles				= nbIndices / 3;
	mTriangleMeshDescription.triangles					= indices;
	mTriangleMeshDescription.triangleStrideBytes		= 3 * sizeof(NxU32);
	// Materials

	NxTriangleMesh* trimesh;

#ifndef NX_DEBUG

	MemoryWriteBuffer buf;
	if (!NxCookTriangleMesh(mTriangleMeshDescription, buf)) {
		std::stringstream s;
		s	<< "Mesh  failed to cook"
			<< "V(" << nbVerts << ") F(" << nbIndices << ")";

		NxThrow_Error(s.str());
	}
	trimesh = scene->getPhysicsSDK().createTriangleMesh(MemoryReadBuffer(buf.data));

#else

	NxString filename;
	
	filename = Ogre::StringConverter::toString(nbVerts) + "-" + Ogre::StringConverter::toString(int(verts[0].x)) + "-" + Ogre::StringConverter::toString(int(verts[0].z)) + ".TriangleMeshShape.nxs";

	UserStream buf(filename.c_str(),false);

	if (!NxCookTriangleMesh(mTriangleMeshDescription, buf)) {
		std::stringstream s;
		s	<< "Mesh  failed to cook"
			<< "V(" << nbVerts << ") F(" << nbIndices << ")";
		NxThrow_Error(s.str());
	}

	fclose(buf.fp);

	UserStream rbuf(filename.c_str(), true);
	trimesh = scene->getPhysicsSDK().createTriangleMesh(rbuf);
	fclose(rbuf.fp);

#endif

	return trimesh;
}

#if 0
////////////////////////////////////////////////////////////////////////////////////////////////

NxHeightField* NxGenerateHeightFieldFromImage(const NxString& imageFilename, unsigned int nbRows,NxScene *scene) {

	Ogre::Image img;
	img.load(imageFilename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	unsigned int imageSize = img.getWidth();

	NxHeightFieldDesc heightFieldDesc;
	heightFieldDesc.nbColumns         = nbRows;
	heightFieldDesc.nbRows            = nbRows;
	heightFieldDesc.verticalExtent      = -1000;
	heightFieldDesc.convexEdgeThreshold = 0;

	// Allocate storage for heightfield samples
	heightFieldDesc.samples         = new NxU32[imageSize * imageSize];
	heightFieldDesc.sampleStride    = sizeof(NxU32);

	char* currentByte = (char*) heightFieldDesc.samples;
	int gMatrixSize = 3;
	NxReal sixtyFourKb = 65536.0;
	NxReal thirtyTwoKb = 32768.0;

	NxMaterialIndex gMatrix[9][3] = {
		// {tesselation, material0, material1}
		{0,1,1}, {0,1,1}, {0,1,1},
		{0,1,1}, {0,1,1}, {0,1,1},
		{0,1,1}, {0,1,1}, {0,1,1}
	};
	const Ogre::uchar* pSrc = img.getData();

	for (NxU32 row = 0; row < nbRows; row++) {
		for (NxU32 column = 0; column < nbRows; column++) {

			NxU32 matrixOffset = (row % gMatrixSize) * gMatrixSize + (column % gMatrixSize);

			NxReal s = NxReal(row) / NxReal(nbRows);
			NxReal t = NxReal(column) / NxReal(nbRows);

			NxHeightFieldSample* currentSample = (NxHeightFieldSample*)currentByte;
			float colorValue = img.getColourAt (row, column,0).r;

			currentSample->height = img.getColourAt (row, column,0).r * 32768.0f;

			currentSample->materialIndex0 = gMatrix[matrixOffset][1];
			currentSample->materialIndex1 = gMatrix[matrixOffset][2];
			currentSample->tessFlag = gMatrix[matrixOffset][0];

			currentByte += heightFieldDesc.sampleStride;
		}
	}

	NxHeightField *hf = scene->getPhysicsSDK().createHeightField(heightFieldDesc);

	delete [] heightFieldDesc.samples;

	return hf;

}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////

NxHeightField* NxGenerateHeightFieldFromImage(const NxString& imageFilename, NxScene* scene) {

	Ogre::Image img;
	img.load(imageFilename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	NxU32 imageWidth = img.getWidth();
	NxU32 imageHeight = img.getHeight();

	NxHeightFieldDesc heightFieldDesc;
	
	heightFieldDesc.nbRows					= imageWidth;
	heightFieldDesc.nbColumns				= imageHeight;
	heightFieldDesc.verticalExtent			= -1000;
	heightFieldDesc.convexEdgeThreshold		= 0;

	// Allocate storage for heightfield samples
	heightFieldDesc.samples         = new NxU32[imageWidth * imageHeight];
	heightFieldDesc.sampleStride    = sizeof(NxU32);

	char* currentByte = (char*) heightFieldDesc.samples;
	int gMatrixSize = 3;
	
	NxMaterialIndex gMatrix[9][3] = {
		// {tesselation, material0, material1}
		{0,1,1}, {0,1,1}, {0,1,1},
		{0,1,1}, {0,1,1}, {0,1,1},
		{0,1,1}, {0,1,1}, {0,1,1}
	};
	const Ogre::uchar* pSrc = img.getData();
	NxReal colourValue = 0;

	for (NxU32 row = 0; row < imageWidth; row++) {
		for (NxU32 column = 0; column < imageHeight; column++) {

			NxU32 matrixOffset = (row % gMatrixSize) * gMatrixSize + (column % gMatrixSize);

			NxReal s = NxReal(row) / NxReal(imageWidth);
			NxReal t = NxReal(column) / NxReal(imageHeight);

			NxHeightFieldSample* currentSample = (NxHeightFieldSample*) currentByte;
			colourValue = img.getColourAt (row, column,0).r;

			currentSample->height = colourValue * 32768.0f;

			currentSample->materialIndex0 = gMatrix[matrixOffset][1];
			currentSample->materialIndex1 = gMatrix[matrixOffset][2];
			currentSample->tessFlag = gMatrix[matrixOffset][0];

			currentByte += heightFieldDesc.sampleStride;
		}
	}

	NxHeightField *hf = scene->getPhysicsSDK().createHeightField(heightFieldDesc);

#ifdef NX_LINUX
	delete [] (NxU32*) heightFieldDesc.samples;
#else
	delete [] heightFieldDesc.samples;
#endif

	return hf;

}

////////////////////////////////////////////////////////////////////////////////////////////////

NxHeightField* NxGenerateHeightFieldFromRaw(const NxString& imageFilename, NxScene* scene) {

	Ogre::MemoryDataStreamPtr mRawData;
	mRawData.setNull();

	Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(imageFilename);
	mRawData = Ogre::MemoryDataStreamPtr(new Ogre::MemoryDataStream(imageFilename, stream));

	// Work out the Image size by the size of the file. This algorithm only works if the texture is square. But since
	// Ogre terrain works this way as well, it isn't much of a problem.
	NxU32 fileLength = mRawData->size();
	NxReal width = NxMath::sqrt(NxReal(fileLength));
	bool  is8bit = true;

	// Check to see if the image is 16-bit. The square root of all image filesizes which are 16-bit, produce a non-integer
	// number. To get the real imageWidth, the fileLength is halfed and square-rooted again, which produces an integer.
	if (NxU32(NxMath::floor(width)) != width) {
		width = NxMath::sqrt(NxReal(fileLength * 0.5f));
		is8bit = false;
	}

	/// And here we go.

	NxHeightFieldDesc heightFieldDesc;
	heightFieldDesc.nbColumns			= width;
	heightFieldDesc.nbRows				= width;
	heightFieldDesc.verticalExtent      = -1000;
	heightFieldDesc.convexEdgeThreshold = 0;

	// Allocate storage for heightfield samples
	heightFieldDesc.samples         = new NxU32[heightFieldDesc.nbRows * heightFieldDesc.nbColumns];
	heightFieldDesc.sampleStride    = sizeof(NxU32);

	char* currentByte = (char*) heightFieldDesc.samples;
	NxU32 gMatrixSize = 3;

	NxMaterialIndex gMatrix[9][3] = {
		// {tesselation, material0, material1}

		{0,1,1},	{0,1,1},	{0,1,1},

		{0,1,1},	{0,1,1},	{0,1,1},

		{0,1,1},	{0,1,1},	{0,1,1}

	};

	const Ogre::uchar* pSrc = mRawData->getPtr();

	NxU32 i = 0;
	NxReal height = 0;
	NxI16 short_height = 0;

	for (NxU32 row = 0; row < width; row++) {
		for (NxU32 column = 0; column < width; column++) {
		
			NxU32 matrixOffset = (row % gMatrixSize) * gMatrixSize + (column % gMatrixSize);

			NxReal s = NxReal(row) / NxReal(width);
			NxReal t = NxReal(column) / NxReal(width);

			NxHeightFieldSample* currentSample = (NxHeightFieldSample*)currentByte;

			i = row + (column * width);

			if (is8bit) {
				height = pSrc[i];
				height *= 257.0f;
				short_height = height;
				short_height -= 32768;
			}
			else {
				i*=2;
				short_height  =	pSrc[i];
				short_height += pSrc[i + 1] << 8;
				short_height -= 32768;
			}
			
			currentSample->height = short_height;    
			currentSample->materialIndex0 = gMatrix[matrixOffset][1];
			currentSample->materialIndex1 = gMatrix[matrixOffset][2];
			currentSample->tessFlag		  = gMatrix[matrixOffset][0];

			currentByte += heightFieldDesc.sampleStride;
		}
	}

	NxHeightField *hf = scene->getPhysicsSDK().createHeightField(heightFieldDesc);

#ifdef NX_LINUX
	delete [] (NxU32*) heightFieldDesc.samples;
#else
	delete [] heightFieldDesc.samples;
#endif

	return hf;
}

////////////////////////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.

