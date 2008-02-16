/** \file    NxOgreAllocator.h
 *  \brief   Header for the Allocator and WatchedPointer classes and the Allocator macros.
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
 *           MERCHANTABILITY or FITNESS FOR l PARTICULAR PURPOSE.  See the GNU
 *           Lesser General Public License for more details.
 *           
 *           You should have received l copy of the GNU Lesser General Public
 *           License along with this library; if not, write to the Free Software
 *           Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __NXOGRE_ALLOCATOR_H__
#define __NXOGRE_ALLOCATOR_H__

#include "NxOgreConfig.h"
#include "BetajaenCC.h"

namespace NxOgre {
    class Allocator;
    
#if (NX_DEBUG == 1 && NX_USE_ALLOCATOR_MACROS == 1)

	/** \brief NxNew macro. Replaces new operator when you want to watch a pointer
		\example
			<code>
				MyClass* inst = NxNew(MyClass) MyClass(5, 4, 3, 2, 1);
			</code>
	*/
	#define NxNew(T) ::NxOgre::WatchMyPointer<T>(__FILE__, __FUNCTION__, __LINE__, 0) = new
	#define NxNewDescribed(T, Description) ::NxOgre::WatchMyPointer<T>(__FILE__, __FUNCTION__, __LINE__, Description) = new

	/** \brief Pass on a pre-made pointer into the watch list
		\example
			<code>
				NxWatch(MyClass, inst);
			</code>
	*/
	#define NxWatch(T, PTR) {::NxOgre::Allocator::getPtr()->watch_ptr<T>(PTR, __FILE__, __FUNCTION__, __LINE__, 0);}
	#define NxWatchDescribed(T, PTR, Description) {::NxOgre::Allocator::getPtr()->watch_ptr<T>(PTR, __FILE__, __FUNCTION__, __LINE__, Description);}

	/** \brief Deletes a watched pointer and set's it to zero.
		\note  Non watched pointers can be passed through, but a warning is placed
			   into the console window.
		\example
			<code>
				NxDelete(inst);
			</code>
	*/
	#define NxDelete(ptr) {::NxOgre::Allocator::getPtr()->watch_delete(static_cast<void*>(ptr));};delete ptr;ptr=0;
#else
	#define NxNew(T)
	#define NxNewDescribed(T, D)
	#define NxWatch(T, PTR)
	#define NxWatch(T, PTR, D)
	#define NxDelete delete ptr;ptr = 0;
#endif

	/** \brief Tells the allocator to watch a pointer. Used with the NxNew macro.
		\example
			<code>
				MyClass* inst = NxNew(MyClass) MyClass(5, 4, 3, 2, 1);
			</code>
	*/
	template <class T>
	class WatchMyPointer {
		
		public:
		
			NxTemplateFunction WatchMyPointer(const char* _file, const char* _function, unsigned int _line, const char* _description = 0)
			: ptr(0), file(_file), function(_function), line(_line), description(_description)
			{}

			template <class T1>
			explicit NxTemplateFunction WatchMyPointer(T1* _t) : ptr(_t) {}

			template <class T1>
			NxTemplateFunction WatchMyPointer& operator=(T1* _t) {
				ptr = _t;
				::NxOgre::Allocator::getPtr()->watch_new(static_cast<void*>(ptr), typeid(T1).name(), file, function, line, sizeof(T1), description);
				return *this;
			}

			template <class T1>
			NxTemplateFunction operator T1*() const {
				return ptr;
			}

			T*           ptr;
			const char*  file;
			const char*  function;
			const char*  description;
			unsigned int line;

	};

	class NxPublicClass Allocator {

		public:

			Allocator();
			~Allocator();

		protected:

			class NxPublicClass WatchedPointer {
				
				friend class Allocator;

				public:

					WatchedPointer();
					~WatchedPointer();

					void* getPtr();

				protected:

					void* mPtr;

					struct ReallySimpleString {
						void set(const char* c);
						void destroy();
						unsigned char* str;
						unsigned len;
					};

					ReallySimpleString   mPtrType;
					ReallySimpleString   mFile;
					ReallySimpleString   mMethod;
					ReallySimpleString   mDescription;

					unsigned int mLine;
					unsigned int mSize;

			};

		public:

			/** \brief Get Allocator singleton pointer
			*/
			static Allocator* getPtr();

			/** \brief Add an existing created pointer to the watch list
			*/
			template <class T>
			void watch_ptr(T* ptr, const char* fileName, const char* function, unsigned int line, const char* description) {
				watch_new((void*) ptr, typeid(T).name(), fileName, function, line, sizeof(T), description);
			}

			/** \brief Add an newely created pointer, used with the NxNew macro
			*/
			void watch_new(void* ptr, const char* type_id, const char* fileName, const char* function, unsigned int line, unsigned int sz, const char* description);
			
			/** \brief Remove a watched pointer from the list, used with the NxDelete macro
			*/
			void watch_delete(void* ptr);
			
			/** \brief Dump the full list of the watched pointers to a file.
			*/
			void dump();

		protected:

			unsigned int NbNew, NbDelete;

			static Allocator* mAllocatorPtr;
			Betajaen::SharedList<WatchedPointer> mAllocations;

	};

};

#endif
