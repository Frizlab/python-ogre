/** \file    NxOgreResourceMesh.cpp
 *  \see     NxOgreResourceMesh.h
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
#include "NxOgreResourceMesh.h"

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

ConvexMeshIntermediary::ConvexMeshIntermediary() : mNbVertices(0), mVertices(0) {
	mDescription.setToDefault();
}

////////////////////////////////////////////////////////////////////////////////////////////////

ConvexMeshIntermediary::~ConvexMeshIntermediary() {
	destroyVertices();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ConvexMeshIntermediary::createVertices(NxU32 length) {
	mVertices = new NxVec3[length];
	mNbVertices = length;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ConvexMeshIntermediary::destroyVertices() {
	delete [] mVertices;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ConvexMeshIntermediary::replaceVertices(NxVec3* vertices, NxU32 length) {
	destroyVertices();
	mVertices = new NxVec3[length];
	mNbVertices = length;
	memcpy(mVertices, vertices, sizeof(NxVec3) * length);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ConvexMeshIntermediary::setToNormal() {
	mDescription.flags = NX_CF_COMPUTE_CONVEX;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ConvexMeshIntermediary::verticesToDescription() {
	mDescription.points = mVertices;
	mDescription.numVertices = mNbVertices;
	mDescription.pointStrideBytes = sizeof(NxVec3);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ConvexMeshIntermediary::descriptionToVertices() {
//	mVertices = static_cast<NxVec3*>(mDescription.points);
//	mNbVertices = mDescription.numVertices;
}

////////////////////////////////////////////////////////////////////////////////////////////////

TriangleMeshIntermediary::TriangleMeshIntermediary() {
	mDescription.setToDefault();
}

////////////////////////////////////////////////////////////////////////////////////////////////

TriangleMeshIntermediary::~TriangleMeshIntermediary() {
	destroyVertices();
	destroyIndices();
	destroyMaterialIndices();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMeshIntermediary::setToNormal() {
	// What is normal anyway?
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMeshIntermediary::createVertices(NxU32 length) {
	mVertices = new NxVec3[length];
	mNbVertices = length;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMeshIntermediary::destroyVertices() {
	delete [] mVertices;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMeshIntermediary::replaceVertices(NxVec3* vertices, NxU32 length) {
	destroyVertices();
	mVertices = new NxVec3[length];
	mNbVertices = length;
	memcpy(mVertices, vertices, sizeof(NxVec3) * length);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMeshIntermediary::verticesToDescription() {
	mDescription.points = mVertices;
	mDescription.numVertices = mNbVertices;
	mDescription.pointStrideBytes = sizeof(NxVec3);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMeshIntermediary::createIndices(NxU32 length) {
	mIndices = new NxU32[length];
	mNbIndices = length;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMeshIntermediary::destroyIndices() {
	delete [] mIndices;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMeshIntermediary::replaceIndices(NxU32* indices, NxU32 length) {
	destroyIndices();
	mIndices = new NxU32[length];
	mNbIndices = length;
	memcpy(mIndices, indices, sizeof(NxU32) * length);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMeshIntermediary::indicesToDescription() {
	mDescription.triangles = mIndices;
	mDescription.numTriangles = mNbIndices / 3;
	mDescription.triangleStrideBytes = 3 * sizeof(NxU32);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMeshIntermediary::createMaterialIndices(NxU32 length) {
	mMaterialIndices = new NxMaterialIndex[length];
	mNbMaterialIndices = length;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMeshIntermediary::destroyMaterialIndices() {
	delete [] mMaterialIndices;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMeshIntermediary::replaceMaterialIndices(NxU32* material_indices, NxU32 length) {
	destroyMaterialIndices();
	mMaterialIndices = new NxMaterialIndex[length];
	mNbMaterialIndices = length;
	memcpy(mMaterialIndices, material_indices, sizeof(NxU32) * length);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void TriangleMeshIntermediary::materialIndicesToDescription() {
	mDescription.materialIndices = mMaterialIndices;
	mDescription.materialIndexStride = sizeof(NxMaterialIndex);
}

////////////////////////////////////////////////////////////////////////////////////////////////

ClothMeshIntermediary::ClothMeshIntermediary()
: mNbVertices(0), mVertices(0), mNbIndices(0), mIndices(0) {
	mDescription.setToDefault();
}

////////////////////////////////////////////////////////////////////////////////////////////////

ClothMeshIntermediary::~ClothMeshIntermediary() {
	destroyVertices();
	destroyIndices();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ClothMeshIntermediary::createVertices(NxU32 length)  {
	std::cout << "Creating vertices of length '" << length << "'." << std::endl; 
	mVertices = (NxVec3*) malloc(sizeof(NxVec3) * length);
	mNbVertices = length;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ClothMeshIntermediary::destroyVertices() {
	free(mVertices);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ClothMeshIntermediary::replaceVertices(NxVec3* vertices, NxU32 length) {
	destroyVertices();
	createVertices(length);
	memcpy(mVertices, vertices, sizeof(NxVec3) * length);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ClothMeshIntermediary::verticesToDescription() {
	mDescription.points = mVertices;
	mDescription.numVertices = mNbVertices;
	mDescription.pointStrideBytes = sizeof(NxVec3);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ClothMeshIntermediary::createIndices(NxU32 length) {
	std::cout << "Creating indices of length '" << length << "'." << std::endl;
	mIndices = (NxU32*) malloc(sizeof(NxU32) * length);
	mNbIndices = length;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ClothMeshIntermediary::destroyIndices() {
	free(mIndices);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ClothMeshIntermediary::replaceIndices(NxU32* indices, NxU32 length) {
	destroyIndices();
	createIndices(length);
	memcpy(mIndices, indices, sizeof(NxU32) * length);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ClothMeshIntermediary::indicesToDescription() {
	mDescription.triangles = mIndices;
	mDescription.numTriangles = mNbIndices / 3;
	mDescription.triangleStrideBytes = 3 * sizeof(NxU32);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ClothMeshIntermediary::setToNormal() {

}

////////////////////////////////////////////////////////////////////////////////////////////////


}; //End of NxOgre namespace.
