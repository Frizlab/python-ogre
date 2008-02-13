/** \file    NxOgreResourceStreamPtr.cpp
 *  \see     NxOgreResourceStreamPtr.h
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
#include "NxOgreResourceStreamPtr.h"
#include "NxOgreResourceSystem.h"
#include "NxOgreWorld.h"
#include "NxOgreResourceManager.h"
#include "NxOgreMemoryStream.h"

namespace NxOgre {

	bool	str_split(std::string& s, std::string& a, std::string& b) {
		for (unsigned int i=0;i < s.length();i++) {
			if (s[i] == ':') {
				a = s.substr(0, i);
				b = s.substr(i + 3, s.length() - i - 3);
				return true;
			}
		}

		return false;
	}
	
	ResourceStreamPtr::ResourceStreamPtr() : pStream(0), pUseCount(new unsigned int(1)) {
		OGRE_SET_AUTO_SHARED_MUTEX_NULL
	}

	ResourceStreamPtr::ResourceStreamPtr(ResourceIdentifier identifier) : pStream(0), pUseCount(new unsigned int(1)) {

		OGRE_SET_AUTO_SHARED_MUTEX_NULL
		OGRE_NEW_AUTO_SHARED_MUTEX
		createStream(identifier);
	}

	void ResourceStreamPtr::createStream(NxString identifier) {
		destroyStream();

		if (identifier == NxMemoryStreamIdentifier) {
			pStream = new MemoryStream(NxMemoryStreamIdentifier);
			return;
		}

		std::string resourceType, resourceIdentifier;

		bool hasResourceType = str_split(identifier, resourceType, resourceIdentifier);

		Ogre::StringUtil::trim(resourceType);
		Ogre::StringUtil::trim(resourceIdentifier);
		Ogre::StringUtil::toLowerCase(resourceType);
		
		ResourceSystem* resourceSystem = ResourceManager::getSingleton()->getResourceSystem(resourceType);
		if (resourceSystem == 0) {
			NxThrow(NxString("Could not find Resource System '" + resourceType + "'!").c_str());
		}

		pStream  = resourceSystem->getStream(identifier);

	}

	void ResourceStreamPtr::destroyStream() {
		if (pStream) {
			delete pStream;
			pStream = 0;
		}
	}

	void ResourceStreamPtr::destroy(void) {
        // IF YOU GET A CRASH HERE, YOU FORGOT TO FREE UP POINTERS
        // BEFORE SHUTTING OGRE DOWN
        // Use setNull() before shutdown or make sure your pointer goes
        // out of scope before OGRE shuts down to avoid this.
        destroyStream();
        delete pUseCount;
		OGRE_DELETE_AUTO_SHARED_MUTEX
    }

}; //End of NxOgre namespace.
