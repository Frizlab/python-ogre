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
#include "NxOgreOgreResourceSystem.h"
#include "NxOgreResourceManager.h"
#include "NxOgreMaterial.h"

#include "OgreResourceGroupManager.h"
namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

OgreResourceSystem::OgreResourceSystem()
: mResourceManager(Ogre::ResourceGroupManager::getSingletonPtr()) {}

////////////////////////////////////////////////////////////////////////////////////////////////

OgreResourceSystem::~OgreResourceSystem() {}

////////////////////////////////////////////////////////////////////////////////////////////////

void OgreResourceSystem::init() {

}

////////////////////////////////////////////////////////////////////////////////////////////////

ResourceStream* OgreResourceSystem::getStream(NxOgre::ResourceIdentifier i) {
	return new OgreResourceStream(i);
}

////////////////////////////////////////////////////////////////////////////////////////////////

ResourceStream* OgreResourceSystem::getStream(NxOgre::ResourceIdentifier i, void* d) {
	return new OgreResourceStream(i,d);
}

////////////////////////////////////////////////////////////////////////////////////////////////

ResourceAccessList OgreResourceSystem::getAllNXS() {
	ResourceAccessList list;

	Ogre::StringVectorPtr resources = mResourceManager->findResourceNames(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "*.nxs");
	
	for (Ogre::StringVector::iterator it = resources->begin(); it != resources->end();it++) {
		list.insert(std::pair<NxString, NxString> (
			(*it).substr(0, (*it).length() - 4),
			"ogre://" + (*it)
		));
	}

	return list;
}

////////////////////////////////////////////////////////////////////////////////////////////////

ResourceAccessList OgreResourceSystem::getAllMESH() {
	ResourceAccessList list;

	return list;
}

////////////////////////////////////////////////////////////////////////////////////////////////

ResourceAccessList OgreResourceSystem::getAllNXM() {
	ResourceAccessList list;

	return list;
}

////////////////////////////////////////////////////////////////////////////////////////////////

ResourceAccessList OgreResourceSystem::getAllEndingWith(NxString) {
	ResourceAccessList list;

	return list;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void OgreResourceSystem::loadThese(ResourceAccessList) {

}

////////////////////////////////////////////////////////////////////////////////////////////////






































//////////////////////////////////////////////////////////////////////

OgreResourceStream::OgreResourceStream(ResourceIdentifier identifier) : ResourceStream(identifier) {
	reuse(identifier);
}

//////////////////////////////////////////////////////////////////////

OgreResourceStream::OgreResourceStream(ResourceIdentifier identifier, void* ds) : ResourceStream(identifier) {
	// Temp: Check first.
	ds = static_cast<Ogre::DataStream*>(ds);
}

//////////////////////////////////////////////////////////////////////

OgreResourceStream::~OgreResourceStream() {
	close();
}

//////////////////////////////////////////////////////////////////////

void OgreResourceStream::close() {
	data->close();
}

//////////////////////////////////////////////////////////////////////

void OgreResourceStream::seek(long p)	 {
	data->seek(p);
}

//////////////////////////////////////////////////////////////////////

void OgreResourceStream::skip(long p) {
	data->skip(p);
}

//////////////////////////////////////////////////////////////////////

size_t OgreResourceStream::tell() const	{
	return data->tell();
}					

//////////////////////////////////////////////////////////////////////

NxU32 OgreResourceStream::size() const {
	return data->size();
}

//////////////////////////////////////////////////////////////////////

void OgreResourceStream::rewind() {
	seek(0);
}

//////////////////////////////////////////////////////////////////////

void OgreResourceStream::reuse(ResourceIdentifier fileName) {
	
	if (!data.isNull())
		close();


	// Remove the ogre:// bit
	fileName = fileName.substr(7, fileName.length() - 7);
	Ogre::StringUtil::trim(fileName);

	data = Ogre::ResourceGroupManager::getSingleton().openResource(fileName);

	if (data.isNull())
		NxThrow_Error("Ogre DataStream '" + fileName + "' could not be opened for reading");

}

//////////////////////////////////////////////////////////////////////

unsigned char OgreResourceStream::getUChar() const {
	unsigned char d=0;data->read(&d, sizeof(unsigned char));
	return d;
}

//////////////////////////////////////////////////////////////////////

NxU16 OgreResourceStream::getShort() const {
	NxU16 d=0;data->read(&d, sizeof(NxU16));
	return d;
}

//////////////////////////////////////////////////////////////////////

unsigned int OgreResourceStream::getUInt() const{
	unsigned int d=0;data->read(&d, sizeof(unsigned int));
	return d;
}

//////////////////////////////////////////////////////////////////////

float OgreResourceStream::getFloat() const{
	float d=0;data->read(&d, sizeof(float));
	return d;
}

//////////////////////////////////////////////////////////////////////

double OgreResourceStream::getDouble() const{
	double d=0;data->read(&d, sizeof(double));
	return d;
}

//////////////////////////////////////////////////////////////////////

std::string OgreResourceStream::getString()	const {

	std::stringstream ss;
	char c;

	while (!data->eof()) {
		data->read(&c, sizeof(NxU8));
		if (c == 0) break;
		ss << c;
	}

	return ss.str();
}

//////////////////////////////////////////////////////////////////////

void OgreResourceStream::get(void* buffer, size_t size) const {
	data->read(buffer, size);
}

//////////////////////////////////////////////////////////////////////



#if 0

//////////////////////////////////////////////////////////////////////

NxString OgreResourceStream::readString()	const	{
	return NxString();
}

//////////////////////////////////////////////////////////////////////

NxStream& OgreResourceStream::storeString(NxString)	{

	return *this;
}

//////////////////////////////////////////////////////////////////////
#endif
#if 0
ResourceSystem::ResourceType OgreResourceStream::getResourceType() {
	
	if (data->size() < 8) {
		NxDebug("Resource is invalid! (Data Length less than 9 bytes)");
		return ResourceSystem::RT_INVALID;
	}

	// Read the header "NXS" and check.
	{
		char header[4] = {0,0,0,0};
		data->read(&header, 3);
		std::string hs(header);

		if (hs != NXS_HEADER)  {
			NxDebug("Resource is invalid! (Header is invalid)");
			return ResourceSystem::RT_INVALID;
		}
	}
	// Skip foward a byte (should be 01)
	data->skip(1);

	ResourceSystem::ResourceType rt = ResourceSystem::RT_UNKNOWN;
	// Read mesh type (4 bytes)
	{
		char header[5] = {0,0,0,0,0};
		data->read(&header, 4);
		std::string hs(header);

		if (hs == NXS_CONVEX_HEADER)
			rt = ResourceSystem::RT_CONVEX;

		if (hs == NXS_TRIANGLEMESH_HEADER)
			rt = ResourceSystem::RT_TRIANGLEMESH;
	}


	// Go back to the beginning of the file.
	data->skip(0 - int(data->tell()));

	return rt;
}
#endif




#if 0
//////////////////////////////////////////////////////////////////////

NxU8 OgreResourceStream::readByte() const {
	NxU8 b;

	size_t r = data->read(&b, sizeof(NxU8));
	NX_ASSERT(r);
	return b;
}

//////////////////////////////////////////////////////////////////////

NxU16 OgreResourceStream::readWord() const {
	NxU16 w;
	size_t r = data->read(&w, sizeof(NxU16));
	NX_ASSERT(r);
	return w;
}

//////////////////////////////////////////////////////////////////////

NxU32 OgreResourceStream::readDword() const {
	NxU32 d;
	size_t r = data->read(&d, sizeof(NxU32));
	NX_ASSERT(r);
	return d;
}

//////////////////////////////////////////////////////////////////////

Ogre::Real OgreResourceStream::readFloat() const {
	NxReal f;
	size_t r = data->read(&f, sizeof(NxReal));;
	NX_ASSERT(r);
	return f;
}

//////////////////////////////////////////////////////////////////////

double OgreResourceStream::readDouble() const {
	NxF64 f;
	size_t r = data->read(&f, sizeof(NxF64));
	NX_ASSERT(r);
	return f;
}

//////////////////////////////////////////////////////////////////////

void OgreResourceStream::readBuffer(void* buffer, NxU32 size) const {
	size_t w = data->read(buffer, size);
	NX_ASSERT(w);
}

//////////////////////////////////////////////////////////////////////

NxStream& OgreResourceStream::storeByte(NxU8 b) {
	NxThrow_Warning("Cannot use the Ogre Resource System for writing!");
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& OgreResourceStream::storeWord(NxU16 w) {
	NxThrow_Warning("Cannot use the Ogre Resource System for writing!");
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& OgreResourceStream::storeDword(NxU32 d) {
	NxThrow_Warning("Cannot use the Ogre Resource System for writing!");
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& OgreResourceStream::storeFloat(NxReal f) {
	NxThrow_Warning("Cannot use the Ogre Resource System for writing!");
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& OgreResourceStream::storeDouble(NxF64 f) {
	NxThrow_Warning("Cannot use the Ogre Resource System for writing!");
	return *this;
}

//////////////////////////////////////////////////////////////////////

NxStream& OgreResourceStream::storeBuffer(const void* buffer, NxU32 size) {
	NxThrow_Warning("Cannot use the Ogre Resource System for writing!");
	return *this;
}

//////////////////////////////////////////////////////////////////////
#endif

#undef NXS_HEADER
#undef NXS_CONVEX_HEADER

}; //End of NxOgre namespace.
