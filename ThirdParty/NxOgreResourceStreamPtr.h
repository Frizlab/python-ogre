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

#ifndef __NXOGRE_RESOURCESTREAMPTR_H__
#define __NXOGRE_RESOURCESTREAMPTR_H__

#include "NxOgrePrerequisites.h"

namespace NxOgre {

	/** @brief A generic abstract class to read or to write to a Resource, without exacting knowing
		       what exactly it is. It takes on a SmartPtr role in life, and the resource will be only
			   deleted from memory when it is no longer used.

		@note	All of ResourceStreamPtr code was used from the Ogre SmartPtr class, except for a changed
				constructor and removal of template.
				
		@see Ogre::SmartPtr
	*/
	class NxExport ResourceStreamPtr {

	protected:

		ResourceStream* pStream;
		unsigned int* pUseCount;
	
	public:

		OGRE_AUTO_SHARED_MUTEX // public to allow external locking
		/** Constructor, does not initialise the ResourceStreamPtr.
			@remarks
				<b>Dangerous!</b> You have to call bind() before using the ResourceStreamPtr.
		*/
		ResourceStreamPtr();

		ResourceStreamPtr(ResourceIdentifier);
		ResourceStreamPtr(ResourceIdentifier, void*);

		ResourceStreamPtr(const ResourceStreamPtr& r)
            : pStream(0), pUseCount(0)
		{
			// lock & copy other mutex pointer
            
            OGRE_SET_AUTO_SHARED_MUTEX_NULL
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
			    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
			    pStream = r.pStream;
			    pUseCount = r.pUseCount; 
			    // Handle zero pointer gracefully to manage STL containers
			    if(pUseCount)
			    {
				    ++(*pUseCount); 
			    }
            }
		}

		void createStream(ResourceIdentifier);
		void destroyStream();

		ResourceStreamPtr& operator=(const ResourceStreamPtr& r) {
			if (pStream == r.pStream)
				return *this;
			// Swap current data into a local copy
			// this ensures we deal with rhs and this being dependent
			ResourceStreamPtr tmp(r);
			swap(tmp);
			return *this;
		}
		
		virtual ~ResourceStreamPtr() {
            release();
		}

		 ResourceStream& operator*() const { assert(pStream); return *pStream; }
		 ResourceStream* operator->() const { assert(pStream); return pStream; }
		 ResourceStream* get() const { return pStream; }

		/** Binds rep to the ResourceStreamPtr.
			@remarks
				Assumes that the ResourceStreamPtr is uninitialised!
		*/
		void bind(ResourceStream* rep) {
			assert(!pStream && !pUseCount);
            OGRE_NEW_AUTO_SHARED_MUTEX
			OGRE_LOCK_AUTO_SHARED_MUTEX
			pUseCount = new unsigned int(1);
			pStream = rep;
		}

		 bool unique() const { OGRE_LOCK_AUTO_SHARED_MUTEX assert(pUseCount); return *pUseCount == 1; }
		 unsigned int useCount() const { OGRE_LOCK_AUTO_SHARED_MUTEX assert(pUseCount); return *pUseCount; }
		 unsigned int* useCountPointer() const { return pUseCount; }

		 ResourceStream* getPointer() const { return pStream; }

		 bool isNull(void) const { return pStream == 0; }

         void setNull(void) { 
			if (pStream)
			{
				// can't scope lock mutex before release incase deleted
				release();
				pStream = 0;
				pUseCount = 0;
			}
        }

    protected:

         void release(void)
        {
			bool destroyThis = false;

			/* If the mutex is not initialized to a non-zero value, then neither is pUseCount nor pStream.
			*/

            OGRE_MUTEX_CONDITIONAL(OGRE_AUTO_MUTEX_NAME)
			{
				// lock own mutex in limited scope (must unlock before destroy)
				OGRE_LOCK_AUTO_SHARED_MUTEX
				if (pUseCount)
				{
					if (--(*pUseCount) == 0) 
					{
						destroyThis = true;
	                }
				}
            }
			if (destroyThis)
				destroy();

            OGRE_SET_AUTO_SHARED_MUTEX_NULL
        }

        virtual void destroy(void);

		virtual void swap(ResourceStreamPtr &other) {
			std::swap(pStream, other.pStream);
			std::swap(pUseCount, other.pUseCount);
			#if OGRE_THREAD_SUPPORT
				std::swap(OGRE_AUTO_MUTEX_NAME, other.OGRE_AUTO_MUTEX_NAME);
			#endif
		}

	};

	 bool operator==(ResourceStreamPtr const& a, ResourceStreamPtr const& b){
		return a.get() == b.get();
	}

	 bool operator!=(ResourceStreamPtr const& a, ResourceStreamPtr const& b) {
		return a.get() != b.get();
	}

}; // End of namespace

#endif
