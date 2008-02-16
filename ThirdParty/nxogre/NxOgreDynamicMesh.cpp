/** \file    NxOgreDynamicMesh.cpp
 *  \see     NxOgreDynamicMesh.h
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
#include "NxOgreDynamicMesh.h"
#include "NxOgreResourceMesh.h"

namespace NxOgre {

/////////////////////////////////////////////////////////////////////

DynamicMesh::DynamicMesh() 
:	mNbVertices(0), mVertices(0), mNbNormals(0),
	mNormals(0), mNbIndices(0), mIndices(0),
	mNbTextureCoordinates(0), mTextureCoordinates(0)
{
	// Empty Constructor
}

/////////////////////////////////////////////////////////////////////

DynamicMesh::~DynamicMesh() {
	if (mNbTextureCoordinates)
		free(mTextureCoordinates);

	if (mNbNormals)
		free(mNormals);
}

/////////////////////////////////////////////////////////////////////

void DynamicMesh::replaceVertices(NxVec3* vertices, NxU32 nbVertices) {
	
	if (mVertices == 0) {
		mVertices = (NxVec3*) malloc(nbVertices * sizeof(NxVec3));
		mNbVertices = nbVertices;
	}
	else if (nbVertices != mNbVertices) {
		free(mVertices);
		mVertices = (NxVec3*) malloc(nbVertices * sizeof(NxVec3));
		mNbVertices = nbVertices;
	}

	memcpy(mVertices, vertices, sizeof(NxVec3) * nbVertices);

}

/////////////////////////////////////////////////////////////////////

void DynamicMesh::replaceNormals(NxVec3* normals, NxU32 nbNormals) {
	
	if (mNormals == 0) {
		mNormals = (NxVec3*) malloc(nbNormals * sizeof(NxVec3));
		mNbNormals = nbNormals;
	}
	else if (nbNormals != mNbNormals) {
		free(mNormals);
		mNormals = (NxVec3*) malloc(nbNormals * sizeof(NxVec3));
		mNbNormals = nbNormals;
	}

	memcpy(mNormals, normals, sizeof(NxVec3) * nbNormals);

}

/////////////////////////////////////////////////////////////////////

void DynamicMesh::replaceIndices(NxU32* indices, NxU32 nbIndices) {

	if (mIndices == 0) {
		mIndices = (NxU32*) malloc(nbIndices * sizeof(NxU32));
		mNbIndices = nbIndices;
	}
	else if (nbIndices != mNbIndices) {
		free(mIndices);
		mIndices = (NxU32*) malloc(nbIndices * sizeof(NxU32));
		mNbIndices = nbIndices;
	}

	memcpy(mIndices, indices, sizeof(NxU32) * nbIndices);

}

/////////////////////////////////////////////////////////////////////

void DynamicMesh::replaceTextureCoordinates(float2* coords, NxU32 nbCoords) {

	if (mTextureCoordinates == 0) {
		mTextureCoordinates = (float2*) malloc(nbCoords * sizeof(float2));
		mNbTextureCoordinates = nbCoords;
	}
	else if (nbCoords != mNbTextureCoordinates) {
		free(mTextureCoordinates);
		mTextureCoordinates = (float2*) malloc(nbCoords * sizeof(float2));
		mNbTextureCoordinates = nbCoords;
	}

	memcpy(mTextureCoordinates, coords, sizeof(float2) * nbCoords);

}

/////////////////////////////////////////////////////////////////////

void	DynamicMesh::referenceVertices(NxVec3* vertices, NxU32 nbVertices, bool gc) {
	
	if (gc && mVertices)
		free(mVertices);

	mVertices = vertices;
	mNbVertices = nbVertices;

}

/////////////////////////////////////////////////////////////////////

void	DynamicMesh::referenceNormals(NxVec3* normals, NxU32 nbNormals, bool gc) {

	if (gc && mNormals)
		free(mNormals);

	mNormals = normals;
	mNbNormals = nbNormals;

}

/////////////////////////////////////////////////////////////////////

void	DynamicMesh::referenceIndices(NxU32* indices, NxU32 nbIndices, bool gc) {

	if (gc && mIndices)
		free(mIndices);

	mIndices = indices;
	mNbIndices = nbIndices;

}

/////////////////////////////////////////////////////////////////////

void	DynamicMesh::referenceTextureCoordinates(float2* coords, NxU32 nbCoords, bool gc) {

	if (gc && mTextureCoordinates)
		free(mTextureCoordinates);

	mTextureCoordinates = coords;
	mNbTextureCoordinates = nbCoords;

}

/////////////////////////////////////////////////////////////////////


DynamicClothMesh::DynamicClothMesh(NxClothMesh* meshPtr)
: DynamicMesh() {
	mMeshPtr = meshPtr;
}

/////////////////////////////////////////////////////////////////////

DynamicClothMesh::~DynamicClothMesh() {
	// Empty. Parent constructor will do the clean up.
}

/////////////////////////////////////////////////////////////////////

NxClothMesh*	DynamicClothMesh::getMeshPtr() {
	return mMeshPtr;
}

/////////////////////////////////////////////////////////////////////

void DynamicClothMesh::fromClothMeshIntermediary(ClothMeshIntermediary* cmi) {
	std::cout << "Vertices '" << cmi->mVertices << "' , size " << cmi->mNbVertices << std::endl;
	replaceVertices(cmi->mVertices, cmi->mNbVertices);
	std::cout << mVertices << std::endl;
	std::cout << "Indices '" << cmi->mIndices << "' , size " << cmi->mNbIndices << std::endl;
	replaceIndices(cmi->mIndices, cmi->mNbIndices);
}

/////////////////////////////////////////////////////////////////////

void DynamicClothMesh::toClothMeshIntermediary(ClothMeshIntermediary* cmi) {

}

/////////////////////////////////////////////////////////////////////

} //End of NxOgre namespace.

////#endif
