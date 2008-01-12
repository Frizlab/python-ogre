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

#ifdef NX_DEBUG
	
	#define NxCreateAllocator(s) new NxOgre::UserAllocator(s);
		
	class NxExport UserAllocator : public NxUserAllocator {
		
		public:
			
			UserAllocator(const NxString& name) : mName(name), m(0),md(0), r(0), f(0) {}
			
			~UserAllocator() {
				
				std::stringstream ss;
				
				ss	<<	"Allocator '" << mName << 
						"' =>  m:" << m <<
						", md: " << md <<
						", r: " << r <<
						", f: " << f << 
					std::endl;
					
				NxDebug(ss.str());
					
			}

	#include <OgreNoMemoryMacros.h>
			void *malloc(NxU32 size)
	#include <OgreMemoryMacros.h>
			{
				m++;
				return malloc(size);
			}
	

	#include <OgreNoMemoryMacros.h>
			void *mallocDEBUG(NxU32 size,const char *fileName, int line)
	#include <OgreMemoryMacros.h>			
			{
				md++;
				return _malloc_dbg(size,_NORMAL_BLOCK, fileName, line);
			} 
	


	#include <OgreNoMemoryMacros.h>
			void * realloc(void * memory, NxU32 size)
	#include <OgreMemoryMacros.h>
			{
				r++;
				return realloc(memory,size);
			}

	#include <OgreNoMemoryMacros.h>
			void free(void * memory)
	#include <OgreMemoryMacros.h>
			{
				f++;
				free(memory);
			}
	

			NxString mName;
			NxI32 m,md,r,f;
			

	};
#else

	#define NxCreateAllocator(s) new NxOgre::UserAllocator();

	class NxExport UserAllocator : public NxUserAllocator {
	
		public:
			
			UserAllocator() {}
			~UserAllocator() {}
			
			void *malloc(NxU32 size) {
					return ::malloc(size);
			}
			
			void *mallocDEBUG(NxU32 size,const char *fileName, int line) {
				return ::_malloc_dbg(size,_NORMAL_BLOCK, fileName, line);
			} 
			
			void * realloc(void * memory, NxU32 size) {
				return ::realloc(memory,size);
			}
			
			void free(void * memory) {
				::free(memory);
			}

	};
#endif

};

#endif
