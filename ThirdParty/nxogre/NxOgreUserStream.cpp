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
#include "NxOgreUserStream.h"
#include <stdio.h>
#include <iostream>

#include <OgreNoMemoryMacros.h>

//////////////////////////////////////////////////////////////////////

UserStream::UserStream(const char* filename, bool load) : fp(NULL) {
	// Swap these two lines around if you get a compiling error.
#ifdef NX_LINUX
	fp = fopen(filename, load ? "rb" : "wb");
#else
	fopen_s(&fp,filename, load ? "rb" : "wb");
#endif

}

//////////////////////////////////////////////////////////////////////

UserStream::~UserStream() {
	if(fp)
		fclose(fp);
}

//////////////////////////////////////////////////////////////////////

NxU8 UserStream::readByte() const {
	NxU8 b;
	size_t r = fread(&b, sizeof(NxU8), 1, fp);
	NX_ASSERT(r);
	return b;
}

//////////////////////////////////////////////////////////////////////

NxU16 UserStream::readWord() const {

	NxU16 w;
	size_t r = fread(&w, sizeof(NxU16), 1, fp);
	NX_ASSERT(r);
	return w;
}

//////////////////////////////////////////////////////////////////////

NxU32 UserStream::readDword() const {
	NxU32 d;
	size_t r = fread(&d, sizeof(NxU32), 1, fp);
	NX_ASSERT(r);
	return d;
}

//////////////////////////////////////////////////////////////////////

Ogre::Real UserStream::readFloat() const {

	NxReal f;
	size_t r = fread(&f, sizeof(NxReal), 1, fp);
	NX_ASSERT(r);
	return f;
}

//////////////////////////////////////////////////////////////////////

double UserStream::readDouble() const {
	NxF64 f;
	size_t r = fread(&f, sizeof(NxF64), 1, fp);
	NX_ASSERT(r);
	return f;
}

//////////////////////////////////////////////////////////////////////

void UserStream::readBuffer(void* buffer, NxU32 size) const {
	size_t w = fread(buffer, size, 1, fp);
	NX_ASSERT(w);
}

//////////////////////////////////////////////////////////////////////

NxStream& UserStream::storeByte(NxU8 b) {
	size_t w = fwrite(&b, sizeof(NxU8), 1, fp);
	NX_ASSERT(w);
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& UserStream::storeWord(NxU16 w) {
	size_t ww = fwrite(&w, sizeof(NxU16), 1, fp);
	NX_ASSERT(ww);
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& UserStream::storeDword(NxU32 d) {
	size_t w = fwrite(&d, sizeof(NxU32), 1, fp);
	NX_ASSERT(w);
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& UserStream::storeFloat(NxReal f) {
	size_t w = fwrite(&f, sizeof(NxReal), 1, fp);
	NX_ASSERT(w);
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& UserStream::storeDouble(NxF64 f) {
	size_t w = fwrite(&f, sizeof(NxF64), 1, fp);
	NX_ASSERT(w);
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& UserStream::storeBuffer(const void* buffer, NxU32 size) {
	size_t w = fwrite(buffer, size, 1, fp);
	NX_ASSERT(w);
	return *this;
}

#if 0
//////////////////////////////////////////////////////////////////////

MemoryWriteBuffer::MemoryWriteBuffer() : currentSize(0), maxSize(0), data(NULL) {

}

//////////////////////////////////////////////////////////////////////

MemoryWriteBuffer::~MemoryWriteBuffer() {
	if(data) {
		NxGetPhysicsSDKAllocator()->free(data);
		data = NULL;
	}
}

//////////////////////////////////////////////////////////////////////

NxStream& MemoryWriteBuffer::storeByte(NxU8 b) {
	storeBuffer(&b, sizeof(NxU8));
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& MemoryWriteBuffer::storeWord(NxU16 w) {
	storeBuffer(&w, sizeof(NxU16));
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& MemoryWriteBuffer::storeDword(NxU32 d) {
	storeBuffer(&d, sizeof(NxU32));
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& MemoryWriteBuffer::storeFloat(NxReal f) {
	storeBuffer(&f, sizeof(NxReal));
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& MemoryWriteBuffer::storeDouble(NxF64 f) {
	storeBuffer(&f, sizeof(NxF64));
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& MemoryWriteBuffer::storeBuffer(const void* buffer, NxU32 size) {

	NxU32 expectedSize = currentSize + size;

	if(expectedSize > maxSize) {
		maxSize = expectedSize + 4096;
		#ifdef NX_LINUX
            NxU8* newData = new NxU8[maxSize];
		#else
            NxU8* newData = (NxU8*)NX_ALLOC(maxSize);
		#endif
		if(data) {
			memcpy(newData, data, currentSize);
			if(data) {
				NxGetPhysicsSDKAllocator()->free(data);
				data = NULL;
			}
		}
		data = newData;
	}
	memcpy(data+currentSize, buffer, size);
	currentSize += size;
	return *this;
}

//////////////////////////////////////////////////////////////////////

MemoryReadBuffer::MemoryReadBuffer(const NxU8* data) : buffer(data) {
}

//////////////////////////////////////////////////////////////////////

MemoryReadBuffer::~MemoryReadBuffer() {
	// We don't own the data => no delete
}

//////////////////////////////////////////////////////////////////////

NxU8 MemoryReadBuffer::readByte() const {
	NxU8 b;
	memcpy(&b, buffer, sizeof(NxU8));
	buffer += sizeof(NxU8);
	return b;
}

//////////////////////////////////////////////////////////////////////

NxU16 MemoryReadBuffer::readWord() const {
	NxU16 w;
	memcpy(&w, buffer, sizeof(NxU16));
	buffer += sizeof(NxU16);
	return w;
}

//////////////////////////////////////////////////////////////////////

NxU32 MemoryReadBuffer::readDword() const {
	NxU32 d;
	memcpy(&d, buffer, sizeof(NxU32));
	buffer += sizeof(NxU32);
	return d;
}

//////////////////////////////////////////////////////////////////////

Ogre::Real MemoryReadBuffer::readFloat() const {
	Ogre::Real f;
	memcpy(&f, buffer, sizeof(Ogre::Real));
	buffer += sizeof(Ogre::Real);
	return f;
}

//////////////////////////////////////////////////////////////////////

double MemoryReadBuffer::readDouble() const {
	double f;
	memcpy(&f, buffer, sizeof(double));
	buffer += sizeof(double);
	return f;
}

//////////////////////////////////////////////////////////////////////

void MemoryReadBuffer::readBuffer(void* dest, NxU32 size) const {
	memcpy(dest, buffer, size);
	buffer += size;
}

//////////////////////////////////////////////////////////////////////
#endif
#include <OgreMemoryMacros.h>
