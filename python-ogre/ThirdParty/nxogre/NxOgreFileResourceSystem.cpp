/** \file    NxOgreFileResourceSystem.cpp
 *  \see     NxOgreFileResourceSystem.h
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
#include "NxOgreFileResourceSystem.h"
#include "NxOgreResourceManager.h"
#include "NxOgreFileResourceStream.h"
#include "NxCooking.h"

namespace NxOgre {

#define NXS_HEADER "NXS"
#define NXS_CONVEX_HEADER "CVXM"

////////////////////////////////////////////////////////////////////////////////////////////////

FileResourceSystem::FileResourceSystem() {}

////////////////////////////////////////////////////////////////////////////////////////////////

FileResourceSystem::~FileResourceSystem() {}

////////////////////////////////////////////////////////////////////////////////////////////////

void FileResourceSystem::init() {
	
}

////////////////////////////////////////////////////////////////////////////////////////////////

ResourceStream* FileResourceSystem::getStream(NxOgre::ResourceIdentifier i) {
	return new FileResourceStream(i);
}

////////////////////////////////////////////////////////////////////////////////////////////////

ResourceStream* FileResourceSystem::getStream(NxOgre::ResourceIdentifier i, void* d) {
	return new FileResourceStream(i,d);
}

////////////////////////////////////////////////////////////////////////////////////////////////

#undef NXS_HEADER
#undef NXS_CONVEX_HEADER

}; //End of NxOgre namespace.
