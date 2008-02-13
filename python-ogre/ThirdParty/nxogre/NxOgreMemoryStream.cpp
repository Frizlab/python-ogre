/** \file    NxOgreMemoryStream.cpp
 *  \see     NxOgreMemoryStream.h
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
#include "NxOgreMemoryStream.h"

namespace NxOgre {

	
MemoryStream::MemoryStream(ResourceIdentifier ri)
: ResourceStream(ri), currentSize(0), maxSize(0), data(NULL), readBuffer(NULL) {

}

//////////////////////////////////////////////////////////////////////

MemoryStream::MemoryStream(ResourceIdentifier ri, void* v)
: ResourceStream(ri, v), currentSize(0), maxSize(0), data(NULL), readBuffer(NULL) {

}

//////////////////////////////////////////////////////////////////////

MemoryStream::~MemoryStream() {
	close();
}

//////////////////////////////////////////////////////////////////////

void	MemoryStream::close()		{

	if(data) {
		delete [] data;
		data = NULL;
		readBuffer = NULL;
	}

}

//////////////////////////////////////////////////////////////////////

size_t MemoryStream::tell() const {
	return 0;
}

//////////////////////////////////////////////////////////////////////

void	MemoryStream::writeUChar(unsigned char b) {
	storeBuffer(&b, sizeof(unsigned char));
}

//////////////////////////////////////////////////////////////////////

void	MemoryStream::writeUShort(unsigned short w) {
	storeBuffer(&w, sizeof(unsigned short));
}

//////////////////////////////////////////////////////////////////////

void	MemoryStream::writeUInt(unsigned int d) {
	storeBuffer(&d, sizeof(unsigned int));
}

//////////////////////////////////////////////////////////////////////

void	MemoryStream::writeFloat(float f) {
	storeBuffer(&f, sizeof(float));
}

//////////////////////////////////////////////////////////////////////

void	MemoryStream::writeDouble(double f) {
	storeBuffer(&f, sizeof(double));
}

//////////////////////////////////////////////////////////////////////

void	MemoryStream::writeString(std::string) {
	NxThrow_AsWarning("Write std::string not supported in MemoryStream");
}

//////////////////////////////////////////////////////////////////////

void	MemoryStream::write(const void* buffer, size_t size) {
	
	NxU32 expectedSize = currentSize + size;

	if(expectedSize > maxSize) {
		
		maxSize = expectedSize + 4096;
		NxU8* newData = new NxU8[maxSize];

		if(data) {
			memcpy(newData, data, currentSize);
			if(data) {
				delete [] data;
				data = NULL;
			}
		}
		data = newData;
		readBuffer = data;
	}

	memcpy(data+currentSize, buffer, size);

	currentSize += size;

}

//////////////////////////////////////////////////////////////////////

void MemoryStream::rewind() {
	readBuffer = static_cast<NxU8*>(data);
}
	
//////////////////////////////////////////////////////////////////////

unsigned char MemoryStream::getUChar()	const {
	unsigned char b;
	memcpy(&b, readBuffer, sizeof(unsigned char));
	std::cout << b;
	readBuffer += sizeof(unsigned char);
	return b;
}

//////////////////////////////////////////////////////////////////////

NxU16 MemoryStream::getShort()	const {
	NxU16 w;
	memcpy(&w, readBuffer, sizeof(short));
	readBuffer += sizeof(short);
	return w;
}

//////////////////////////////////////////////////////////////////////

unsigned int MemoryStream::getUInt() const {
	unsigned int d;
	memcpy(&d, readBuffer, sizeof(unsigned int));
	readBuffer += sizeof(unsigned int);
	return d;
}

//////////////////////////////////////////////////////////////////////

float MemoryStream::getFloat() const {
	float f;
	memcpy(&f, readBuffer, sizeof(float));
	readBuffer += sizeof(float);
	return f;	
}

//////////////////////////////////////////////////////////////////////

double MemoryStream::getDouble() const {
	double f;
	memcpy(&f, readBuffer, sizeof(double));
	readBuffer += sizeof(double);
	return f;
}

//////////////////////////////////////////////////////////////////////

std::string	MemoryStream::getString()	const {
	NxThrow_AsWarning("Read std::string not supported in MemoryStream");
	return std::string();
}

//////////////////////////////////////////////////////////////////////

void MemoryStream::get(void* dest, size_t size) const {
	memcpy(dest, readBuffer, size);
	readBuffer += size;
}

//////////////////////////////////////////////////////////////////////

}; // End of NxOgre namespace.

