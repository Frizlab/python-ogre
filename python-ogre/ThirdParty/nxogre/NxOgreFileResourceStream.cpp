/** \file    NxOgreResourceStream.cpp
 *  \see     NxOgreResourceStream.h
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
#include "NxOgreFileResourceStream.h"
#include "NxOgreHelpers.h"

namespace NxOgre {

//////////////////////////////////////////////////////////////////////

FileResourceStream::FileResourceStream(ResourceIdentifier identifier, void*)
: ResourceStream(identifier), fp(0), mSize(0)
{}


FileResourceStream::FileResourceStream(ResourceIdentifier ri)
: ResourceStream(ri), fp(0), mSize(0) {
	reuse(ri);
}

//////////////////////////////////////////////////////////////////////

FileResourceStream::~FileResourceStream() {
	close();
}

//////////////////////////////////////////////////////////////////////

void FileResourceStream::close() {
	if(fp)	fclose(fp);
	fp = 0;
}

//////////////////////////////////////////////////////////////////////

void FileResourceStream::seek(long pos) {
	fseek(fp, pos, SEEK_SET);
}

//////////////////////////////////////////////////////////////////////

void FileResourceStream::skip(long pos) {
	fseek(fp, pos, SEEK_CUR);
}

//////////////////////////////////////////////////////////////////////

size_t FileResourceStream::tell() const {
	return ftell(fp);
}

//////////////////////////////////////////////////////////////////////

void FileResourceStream::reuse(ResourceIdentifier fileName) {
	
	if (fp)
		close();

		bool reading = true;

	// Remove the file:// bit
	fileName = fileName.substr(7, fileName.length() - 7);
	NxStringTrim(fileName);

	std::cout << "1: Reading =>" << reading << ", Filename => " << fileName << std::endl;

	if (NxStringEndsWith(fileName, "+write")) {
		reading = false;
		fileName = fileName.substr(0, fileName.length() - 6);
		NxStringTrim(fileName);
	}
	
	std::cout << "2: Reading =>" << reading << ", Filename => " << fileName << std::endl;

	#ifdef NX_PLATFORM_WINDOWS
		fopen_s(&fp,fileName.c_str(), reading ? "rb" : "wb");
	#else
		fp = fopen(fileName.c_str(), reading ? "rb" : "wb");
	#endif

	std::cout << "3: FP => " << fp << std::endl;

	if (fp == 0)
		NxThrow(NxString("File '" + fileName + "' could not be opened for " + (reading ? "Reading" : "Writing")).c_str());

	if (reading) {
		fseek(fp, 0, SEEK_END);
		mSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	}

	std::cout << "File size of '" << fileName << "' is " << mSize << std::endl;

}

//////////////////////////////////////////////////////////////////////

NxU32 FileResourceStream::size() const {
	return mSize;
}

//////////////////////////////////////////////////////////////////////

void FileResourceStream::rewind() {
	fseek(fp, 0, SEEK_SET);
}

//////////////////////////////////////////////////////////////////////

void FileResourceStream::writeUChar(unsigned char data) {
	size_t w = fwrite(&data, sizeof(unsigned char), 1, fp);
	// NX_ASSERT(w);
}

//////////////////////////////////////////////////////////////////////

void FileResourceStream::writeUShort(NxU16 data) {
	size_t w = fwrite(&data, sizeof(NxU16), 1, fp);
	// NX_ASSERT(w);
}

//////////////////////////////////////////////////////////////////////

void FileResourceStream::writeUInt(unsigned int data) {
	size_t w = fwrite(&data, sizeof(unsigned int), 1, fp);
	// NX_ASSERT(w);
}

//////////////////////////////////////////////////////////////////////

void FileResourceStream::writeFloat(float data) {
	size_t w = fwrite(&data, sizeof(float), 1, fp);
	// NX_ASSERT(w);
}

//////////////////////////////////////////////////////////////////////

void FileResourceStream::writeDouble(double data) {
	size_t w = fwrite(&data, sizeof(double), 1, fp);
	// NX_ASSERT(w);
}

//////////////////////////////////////////////////////////////////////

void FileResourceStream::writeString(std::string s) {
	for (NxU32 i=0;i < s.length();i++) {
		writeUChar(s[i]);
	}
	writeUChar(0);
}

//////////////////////////////////////////////////////////////////////

void FileResourceStream::write(const void* data, size_t l) {
	size_t w = fwrite(data, l, 1, fp);
	// NX_ASSERT(w);
}

//////////////////////////////////////////////////////////////////////

unsigned char FileResourceStream::getUChar() const {
	unsigned char d=0;fread(&d, sizeof(unsigned char), 1, fp);
	return d;
}

//////////////////////////////////////////////////////////////////////

NxU16 FileResourceStream::getShort() const {
	NxU16 d=0;fread(&d, sizeof(NxU16), 1, fp);
	return d;
}

//////////////////////////////////////////////////////////////////////

unsigned int FileResourceStream::getUInt() const{
	unsigned int d=0;fread(&d, sizeof(unsigned int), 1, fp);
	return d;
}

//////////////////////////////////////////////////////////////////////

float FileResourceStream::getFloat() const{
	float d=0;fread(&d, sizeof(float), 1, fp);
	return d;
}

//////////////////////////////////////////////////////////////////////

double FileResourceStream::getDouble() const{
	double d=0;fread(&d, sizeof(double), 1, fp);
	return d;
}

//////////////////////////////////////////////////////////////////////

std::string FileResourceStream::getString()	const {

	std::stringstream ss;
	char c;

	while (!feof(fp)) {
		fread(&c, sizeof(NxU8), 1, fp);
		if (c == 0) break;
		ss << c;
	}

	return ss.str();
}

//////////////////////////////////////////////////////////////////////

void FileResourceStream::get(void* buffer, size_t size) const {
	fread(buffer, size, 1, fp);
}

//////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
