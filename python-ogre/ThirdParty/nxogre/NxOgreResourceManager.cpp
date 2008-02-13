/** \file    NxOgreResourceManager.cpp
 *  \see     NxOgreResourceManager.h
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
#include "NxOgreResourceManager.h"
#include "NxCooking.h"
#include "NxOgreSkeleton.h"
#include "NxOgreHelpers.h"
#include "NxOgreResourceSystem.h"
#include "NxOgrePhysXDriver.h"
#include "NxOgreWorld.h"
#include "NxOgreMaterial.h"
#include "NxOgreResourceStreamPtr.h"
#include "NxOgreMemoryStream.h"
#include "NxOgreFileResourceSystem.h"
#include "NxOgreOgreResourceSystem.h"
#include "NxOgreContainer.h"
#include "NxOgreResourceMesh.h"
#include "NxOgreDynamicMesh.h"

namespace NxOgre {

#define NX_TEMP_COOKINGFILENAME "file://nxogre.tmp"
#define NX_TEMP_COOKINGFILENAME_WRITE "file://nxogre.tmp +write"

////////////////////////////////////////////////////////////////////////////////////////////////

ResourceManager* ResourceManager::mResourceManager = 0;

////////////////////////////////////////////////////////////////////////////////////////////////

ResourceManager::ResourceManager(PhysXDriver* driver) {

	if (mResourceManager) {
		NxThrow("ResourceManager has already created.");
		return;
	}

	mResourceManager = this;
	mDriver = driver;

	addResourceSystem("file", new FileResourceSystem(), true);
#if (NX_USE_OGRE_RESOURCE_SYSTEM == 1)
	addResourceSystem("ogre", new OgreResourceSystem(), true);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////

ResourceManager::~ResourceManager() {

	// Remove every Skeleton
	mSkeletons.empty();
	mSkeletonsByName.empty();

	// Remove every Triangle Mesh

	// Remove every Convex Mesh
	// PhysX "owns" the convex meshes. Anyway if we randomly start deleting meshes, there is a high
	// probably that a shape is using one, which will cause a big crash. 
	mConvexMeshes.empty();

	// Remove every soft body

	// Remove every cloth
	mClothMeshes.destroyAndEraseAll();

	// Delete every material alias
	mMaterialAliases.destroyAllOwned();

	////////////////////////////////////
	
	#if (NX_DEBUG == 1)
		if (mDriver)
			mResourceSystems.dumpToConsole();
	#endif

	mResourceSystems.destroyAllOwned();
	
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ResourceManager::addResourceSystem(const NxString& identifier, ResourceSystem* rs, bool lock) {
	
	if (mResourceSystems.has(identifier))
		return;

	mResourceSystems.insert(identifier, rs);
	if (lock)
		mResourceSystems.lock(identifier, true);

	if (mDriver)
		NxDebug(NxString("Added " + identifier + " as Resource System").c_str());

}

////////////////////////////////////////////////////////////////////////////////////////////////

ResourceSystem*	ResourceManager::getResourceSystem(const NxString& identifier) {
	return mResourceSystems.get(identifier);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void ResourceManager::initAllResourceSystems() {
	
	for (ResourceSystem* resourcesystem = mResourceSystems.begin();resourcesystem = mResourceSystems.next();) {
		
		resourcesystem->init();
		ResourceAccessList list = resourcesystem->getAllNXS();

		
		for (ResourceAccessList::iterator it = list.begin();it != list.end();++it) {
			std::cout << (*it).first << " -> " << (*it).second << std::endl;
			ResourceStreamPtr resource((*it).second);
			loadUnknownNXS((*it).first, resource);
			resource->close();
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

void ResourceManager::initResourceSystem(const NxString& identifier) {
	ResourceSystem* resourcesystem = mResourceSystems.get(identifier);
	if (resourcesystem)
		resourcesystem->init();
}

////////////////////////////////////////////////////////////////////////////////////////////////

NxString ResourceManager::getIdentifierFromFilename(NxString filename) {
	
	if (Ogre::StringUtil::endsWith(filename, ".nxm")) {
		filename = filename.substr(0, filename.length() - 4);
	}
	
	if (Ogre::StringUtil::endsWith(filename, ".nxs")) {
		filename = filename.substr(0, filename.length() - 4);
	}

	if (Ogre::StringUtil::endsWith(filename, ".mesh")) {
		filename = filename.substr(0, filename.length() - 5);
	}

	return filename;
}

////////////////////////////////////////////////////////////////////////////////////////////////

NxString ResourceManager::getMeshIdentifier(NxString filename, NxVec3 scale) {
	
	std::stringstream ss;
	
	if (Ogre::StringUtil::endsWith(filename, ".nxs")) {
		filename = filename.substr(0, filename.length() - 4);
	}

	if (Ogre::StringUtil::endsWith(filename, ".mesh")) {
		filename = filename.substr(0, filename.length() - 5);
	}

	ss << filename;
	
	if (scale != NxVec3(1,1,1)) {
		ss << ";" << scale.x << ";" << scale.y << ";" << scale.z;
	}

	return ss.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ResourceManager::loadUnknownNXS(MeshIdentifier identifier, ResourceStreamPtr resource) {
	
	// See if it starts with a NXS.
	resource->rewind();
	
	char hdr[4] = {0,0,0,0};
	resource->get(hdr, 3);
	std::cout << "Header is = " << hdr << std::endl;

	if (NxString(hdr) != "NXS") {
		NxThrow(NxString("'" + identifier + "' is not a NXS file!").c_str());
		resource->rewind();
		return false;
	}

	// Skip a byte.
	resource->getUChar();

	char msh[5] = {0,0,0,0,0};
	resource->get(msh, 4);
	NxString meshType(msh);
	
	std::cout << "Meshtype = " << meshType << std::endl;
	
	resource->rewind();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////


#undef NX_TEMP_COOKINGFILENAME
#undef NX_TEMP_COOKINGFILENAME_WRITE

}; //End of NxOgre namespace.
