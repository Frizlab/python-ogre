/** \file    NxOgreOgreResourceSystem.cpp
 *  \see     NxOgreOgreResourceSystem.h
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
#include "NxOgreOgreResourceSystem.h"
#include "NxOgreOgreResourceStream.h"
#include "NxOgreResourceManager.h"

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
