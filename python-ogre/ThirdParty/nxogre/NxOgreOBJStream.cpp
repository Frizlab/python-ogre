/** \file    NxOgreOBJStream.cpp
 *  \see     NxOgreOBJStream.h
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
#include "NxOgreOBJStream.h"
#include "NxOgreHelpers.h"
#include "NxOgreResourceStream.h"
#include "NxOgreResourceStreamPtr.h"
#include "NxOgreContainer.h"

namespace NxOgre {

/////////////////////////////////////////////////////////////

OBJStream::OBJStream(ResourceStreamPtr stream_ptr) 
: mNbVertices(0),
  mVertices(0),  
  mNormals(0),
  mNbNormals(0),
  mTextureCoordinates(0),
  mNbTextureCoordinates(0),
  mIndices(0),
  mNbIndices(0)
{
	mStream = stream_ptr;
}

/////////////////////////////////////////////////////////////

OBJStream::~OBJStream() {
	__freeAll();
	mStream->close();
}

/////////////////////////////////////////////////////////////

void OBJStream::load() {
	
	if (mStream->size() == 0)
		return;

	static const NxU32 maxLineLength = 256;
	
	__freeAll();

	mNbVertices = 0;
	mNbNormals = 0;
	mNbTextureCoordinates = 0;
	mNbIndices = 0;
	
	NxU32	szVertices = 10,
			szNormals = 10,
			szTextureCoordinates = 10,
			szIndices = 30;

	mVertices = new NxVec3[szVertices];
	mTextureCoordinates = new float2[szTextureCoordinates];
	mNormals = new NxVec3[szNormals];
	mIndices = new NxU32[szIndices];


	char line[maxLineLength];

	for (NxU32 i=0;i < maxLineLength;i++) line[i] = 0;

	NxU32 lineLength = 0;
	NxU32 lineNb = 0;
	while (mStream->tell() < mStream->size()) {
		
		lineLength = 0;
		while (mStream->tell() < mStream->size()) {
			unsigned char c;
			c = mStream->getUChar();
			if (c == '\r') continue;
			if (c == '\n') break;
			line[lineLength++] = c;
		}
		
		if (lineLength == 0) continue;
		if (line[0] == ' ') continue;
		if (line[0] == '#') continue;
		line[lineLength + 1] = 0;

		NxString lineStr(line);
		NxStringTrim(lineStr);

		Container<NxU32, NxString> strings = NxStringTokenize(lineStr, " \t", true);
		if (strings.count() <= 1) continue;

		if (strings[0] == "v" && strings.count() == 4) {

			if (mNbVertices + 1 > szVertices) {
				szVertices *= 2;
				NxVec3* tVertices = new NxVec3[szVertices];
				memcpy(tVertices, mVertices, mNbVertices * sizeof(NxVec3));
				delete [] mVertices;
				mVertices = tVertices;
			}

			NxVec3 vertex;
			vertex.x = NxStringToReal(strings[1]);
			vertex.y = NxStringToReal(strings[2]);
			vertex.z = NxStringToReal(strings[3]);
			
			mVertices[mNbVertices] = vertex;
			mNbVertices++;
		}

		if (strings[0] == "vn" && strings.count() == 4) {
			
			if (mNbNormals + 1 > szNormals) {
				szNormals *= 2;
				NxVec3* tNormals = new NxVec3[szNormals];
				memcpy(tNormals, mNormals, mNbNormals * sizeof(NxVec3));
				delete [] mNormals;
				mNormals = tNormals;
			}

			NxVec3 normal;
			normal.x = NxStringToReal(strings[1]);
			normal.y = NxStringToReal(strings[2]);
			normal.z = NxStringToReal(strings[3]);
			
			mNormals[mNbNormals] = normal;
			mNbNormals++;
		}

		if (strings[0] == "vt" && strings.count() >= 3) {
			
			if (mNbTextureCoordinates + 1 > szTextureCoordinates) {
				szTextureCoordinates *= 2;
				float2* tTextureCoordinates = new float2[szTextureCoordinates];
				memcpy(tTextureCoordinates, mTextureCoordinates, mNbTextureCoordinates * sizeof(float2));
				delete [] mTextureCoordinates;
				mTextureCoordinates = tTextureCoordinates;
			}

			float2 coords;
			coords.i = NxStringToReal(strings[1]);
			coords.j = NxStringToReal(strings[2]);

			mTextureCoordinates[mNbTextureCoordinates] = coords;
			mNbTextureCoordinates++;

		}

		lineNb++;
	}

}

/////////////////////////////////////////////////////////////

void OBJStream::save() {
	
}

/////////////////////////////////////////////////////////////

NxVec3* OBJStream::getVertices() {
	return mVertices;
}

/////////////////////////////////////////////////////////////

NxU32	OBJStream::getNbVertices() {
	return mNbVertices;
}

/////////////////////////////////////////////////////////////

NxU32*	OBJStream::getIndices() {
	return mIndices;
}

/////////////////////////////////////////////////////////////

NxU32	OBJStream::getNbIndices() {
	return mNbIndices;
}

/////////////////////////////////////////////////////////////

NxVec3*	OBJStream::getNormals() {
	return mNormals;
}

/////////////////////////////////////////////////////////////

float2*	OBJStream::getTextureCoordinates() {
	return mTextureCoordinates;
}

/////////////////////////////////////////////////////////////

void OBJStream::setVertices(NxVec3*, NxU32 nbVertices) {
	
}

/////////////////////////////////////////////////////////////

void OBJStream::setNormals(NxVec3*, NxU32 nbNormals) {
	
}

/////////////////////////////////////////////////////////////

void OBJStream::setTextureCoodinates(float2*, NxU32 nbCoords) {
	
}

/////////////////////////////////////////////////////////////

void OBJStream::setIndices(NxU32*, NxU32 nbIndices) {
	
}

/////////////////////////////////////////////////////////////

void OBJStream::__freeAll() {

	if (mVertices)
		delete [] mVertices;

	if (mNormals)
		delete [] mNormals;

	if (mTextureCoordinates)
		delete [] mTextureCoordinates;

	if (mIndices)
		delete [] mIndices;

}

/////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
