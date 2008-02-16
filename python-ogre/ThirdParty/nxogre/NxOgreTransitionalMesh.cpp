/** \file    NxOgreTransitionalMesh.cpp
 *  \see     NxOgreTransitionalMesh.h
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
#include "NxOgreTransitionalMesh.h"
#include "NxOgreResourceStream.h"
#include "BetajaenCC.h"

namespace NxOgre {

/////////////////////////////////////////////////////////////

TransitionalMesh::TransitionalMesh(ResourceStreamPtr resourceStream)
: mStream(resourceStream),
mNbVertices(0),
mNbIndices(0),
mNbNormals(0),
mNbTextureCoordinates(0),
mNbIndexMaterials(0)
{
}

/////////////////////////////////////////////////////////////

TransitionalMesh::~TransitionalMesh()
{
	mStream->close();
	empty();
}

/////////////////////////////////////////////////////////////

bool TransitionalMesh::isEmpty() const {
	
	if ((mNbVertices + mNbNormals + mNbTextureCoordinates + mNbIndices) != 0)
		return false;
	
	return true;
}

/////////////////////////////////////////////////////////////

void TransitionalMesh::empty() const {

	if (mNbVertices)
		delete [] mVertices;

	if (mNbIndices)
		delete [] mIndices;

	if (mNbNormals)
		delete [] mNormals;

	if (mNbTextureCoordinates)
		delete [] mTextureCoordinates;

	if (mNbIndexMaterials)
		delete [] mIndexMaterials;

}

/////////////////////////////////////////////////////////////

float3* TransitionalMesh::getVertices() {
	return mVertices;
}

/////////////////////////////////////////////////////////////

unsigned int TransitionalMesh::getNbVertices() const {
	return mNbVertices;
}

/////////////////////////////////////////////////////////////

unsigned int* TransitionalMesh::getIndices() {
	return mIndices;
}

/////////////////////////////////////////////////////////////

unsigned int TransitionalMesh::getNbIndices() const {
	return mNbIndices;
}

/////////////////////////////////////////////////////////////

float3* TransitionalMesh::getNormals() {
	return mNormals;
}

/////////////////////////////////////////////////////////////

unsigned int TransitionalMesh::getNbNormals() const {
	return mNbNormals;
}

/////////////////////////////////////////////////////////////

float2* TransitionalMesh::getTextureCoordinates() {
	return mTextureCoordinates;
}

/////////////////////////////////////////////////////////////

NxU32	TransitionalMesh::getNbTextureCoordinates() const {
	return mNbTextureCoordinates;
}

/////////////////////////////////////////////////////////////

unsigned int TransitionalMesh::getNbIndexMaterials() const {
	return mNbIndexMaterials;
}

/////////////////////////////////////////////////////////////

unsigned char* TransitionalMesh::getIndexMaterials() {
	return mIndexMaterials;
}

/////////////////////////////////////////////////////////////

void TransitionalMesh::_reserveVertices(unsigned int nbVertices) {

	if (mNbVertices)
		delete [] mVertices;

	mNbVertices = nbVertices;
	mVertices = new float3[mNbVertices];

}

void TransitionalMesh::_reserveNormals(unsigned int nbNormals) {

	if (mNbNormals)
		delete [] mNormals;

	mNbNormals = nbNormals;
	mNormals = new float3[mNbNormals];

}

void TransitionalMesh::_reserveIndices(unsigned int nbIndices) {

	if (mNbIndices)
		delete [] mIndices;

	mNbIndices = nbIndices;
	mIndices = new unsigned int[mNbIndices];

}

/////////////////////////////////////////////////////////////

void TransitionalMesh::_reserveTextureCoordinates(unsigned int nbTextureCoordinates) {
	
	if (mNbTextureCoordinates)
		delete [] mTextureCoordinates;

	mNbTextureCoordinates = nbTextureCoordinates;
	mTextureCoordinates = new float2[mNbTextureCoordinates];

}

/////////////////////////////////////////////////////////////

void TransitionalMesh::_reserveIndexMaterials(unsigned int nbIndexMaterials) {
	
	if (mNbIndexMaterials)
		delete [] mIndexMaterials;

	mNbIndexMaterials = nbIndexMaterials;
	mIndexMaterials = new unsigned char[mNbIndexMaterials];

}

/////////////////////////////////////////////////////////////

Betajaen::Vector2< float3 > TransitionalMesh::getBoundingBox() const {

	Betajaen::Vector2<float3> bb;
	bb.i = float3(0,0,0);
	bb.j = float3(0,0,0);

	float3* verts = mVertices;

	for(unsigned int i = 0; i < mNbVertices; i++) {
		
		if (verts->i < bb.i.i)
			bb.i.i = verts->i;

		if (verts->j < bb.i.j)
			bb.i.j = verts->j;

		if (verts->k < bb.i.k)
			bb.i.k = verts->k;

		if (verts->i > bb.j.i)
			bb.j.i = verts->i;

		if (verts->j > bb.j.j)
			bb.j.j = verts->j;

		if (verts->k > bb.j.k)
			bb.j.k = verts->k;

		*verts++;
	}
	
	return bb;
}

/////////////////////////////////////////////////////////////

void TransitionalMesh::_cleanMesh() {

}

/////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
