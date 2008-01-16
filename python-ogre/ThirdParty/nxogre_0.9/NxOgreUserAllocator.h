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

#ifndef __NXOGRE_USERALLOCATOR_H__
#define __NXOGRE_USERALLOCATOR_H__

#include "NxOgrePrerequisites.h"

namespace NxOgre {

	class UserAllocator : public NxUserAllocator {

	public:

							 UserAllocator();
		virtual				~UserAllocator();

				void		reset();

	#include <OgreNoMemoryMacros.h>

				void*		malloc(NxU32 size);
				void*		malloc(NxU32 size, NxMemoryType type);
				void*		mallocDEBUG(NxU32 size, const char* file, int line);
				void*		mallocDEBUG(NxU32 size, const char* file, int line, const char* className, NxMemoryType type);
				void*		realloc(void* memory, NxU32 size);
				void		free(void* memory);

	#include <OgreMemoryMacros.h>

				NxU32*		mMemBlockList;
				NxU32		mMemBlockListSize;
				NxU32		mMemBlockFirstFree;
				NxU32		mMemBlockUsed;

				NxI32		mNbAllocatedBytes;
				NxI32		mHighWaterMark;
				NxI32		mTotalNbAllocs;
				NxI32		mNbAllocs;
				NxI32		mNbReallocs;
	};

};

#endif
