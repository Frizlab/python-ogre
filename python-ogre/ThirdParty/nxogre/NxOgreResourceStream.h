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

#ifndef __NXOGRE_RESOURCE_STREAM_H__
#define __NXOGRE_RESOURCE_STREAM_H__

#include "NxOgrePrerequisites.h"
#include "NxStream.h"

namespace NxOgre {

	/////////////////////////////////////////////////////////////////////

	/** An abstract pool of data to write or to read to, to be used with the ResourceStreamPtr and a specific
		ResourceStream

		@see ResourceStreamPtr
		@see FileResourceStream
		@see MemoryResourceStream
		@see OgreResourceStream
	*/
	class NxExport ResourceStream : public NxStream {

		friend class ResourceStreamPtr;

		public:

			/** Re-use the stream
			*/
			virtual		void			reuse(ResourceIdentifier) {}

			/** Close the stream
			*/
			virtual		void			close()				{}
			
			/** The size of the stream.
			*/
			virtual		NxU32			size()		const	{return 0;}

			/** Set the read pointer to X bytes from the beginning of the stream.
			*/
			virtual		void			seek(long)			{}
			
			/** Skip forward so many bytes from the current position of the read pointer
			*/
			virtual		void			skip(long)			{}
			
			/** How far is the read pointer from the beginning of the stream
			*/
			virtual		size_t			tell()		const	{return 0;}							
			
			/** Reset the readPointer to the beginning.
				@note <strong>Must</strong> be used with MemoryStreams after writing and reading is about to commence.
			*/
			virtual		void			rewind()			{}
		
			/** Write char to the stream*/
			virtual		void			writeUChar(unsigned char) = 0;

			/** Write short to the stream*/
			virtual		void			writeUShort(unsigned short) = 0;

			/** Write unsigned int to the stream*/
			virtual		void			writeUInt(unsigned int) = 0;

			/** Write float to the stream*/
			virtual		void			writeFloat(float) = 0;

			/** Write double to the stream*/
			virtual		void			writeDouble(double) = 0;
	
			/** Write a stl string to the stream*/
			virtual		void			writeString(std::string) = 0;
	
			/** Write any data to the stream*/
			virtual		void			write(const void*, size_t length) = 0;

			/** Get a char from the stream*/
			virtual		unsigned char	getUChar()	const = 0;
			
			/** Get a short from the stream*/
			virtual		unsigned short	getShort()	const = 0;
			
			/** Get a unsigned int from the stream*/
			virtual		unsigned int	getUInt()	const = 0;
			
			/** Get a float from the stream*/
			virtual		float			getFloat()	const = 0;
			
			/** Get a double from the stream*/
			virtual		double			getDouble()	const = 0;
			
			/** Get a stl string from the stream*/
			virtual		std::string		getString()	const = 0;

			/** Get any data from the stream*/
			virtual		void			get(void* buffer, size_t length) const = 0;

		protected:

			/** PhysX Interface. Do not override or use. @See ResourceStreamPtr::getUChar
				@internal
			*/
			NxU8	readByte()		const	{return getUChar();}

			/** PhysX Interface. Do not override or use. @See ResourceStreamPtr::getUChar
				@internal
			*/
			NxU16	readWord()		const	{return getShort();}

			/** PhysX Interface. Do not override or use. @See ResourceStreamPtr::getUChar
				@internal
			*/
			NxU32	readDword()		const	{return getUInt();}

			/** PhysX Interface. Do not override or use. @See ResourceStreamPtr::getUChar
				@internal
			*/
			NxReal	readFloat()		const	{return getFloat();}

			
			/** PhysX Interface. Do not override or use. @See ResourceStreamPtr::getUChar
				@internal
			*/
			double	readDouble()	const	{return getDouble();}
			
			
			/** PhysX Interface. Do not override or use. @See ResourceStreamPtr::getUChar
				@internal
			*/
			void	readBuffer(void* buffer, NxU32 size) const {return get(buffer, size);}

			
			/** PhysX Interface. Do not override or use. @See ResourceStreamPtr::getUChar
				@internal
			*/
			NxStream&	storeByte(NxU8 b)		{writeUChar(b);return *this;}

			
			/** PhysX Interface. Do not override or use. @See ResourceStreamPtr::getUChar
				@internal
			*/
			NxStream&	storeWord(NxU16 w)		{writeUShort(w);return *this;}

			
			/** PhysX Interface. Do not override or use.
				@See ResourceStreamPtr::storeFloat
				@internal
			*/
			NxStream&	storeDword(NxU32 d)		{writeUInt(d);return *this;}

			
			/** PhysX Interface. Do not override or use.
				@See ResourceStreamPtr::storeFloat
				@internal
			*/
			NxStream&	storeFloat(NxReal f)	{writeFloat(f);return *this;}

			
			/** PhysX Interface. Do not override or use.
				@See ResourceStreamPtr::storeDouble
				@internal
			*/
			NxStream&	storeDouble(NxF64 f)	{writeDouble(f);return *this;}

			
			/** PhysX Interface. Do not override or use.
				@See ResourceStreamPtr::store
				@internal
			*/
			NxStream&	storeBuffer(const void* buffer, NxU32 size)	{write(buffer, size);return *this;}


			ResourceStream(ResourceIdentifier)			{}
			ResourceStream(ResourceIdentifier, void*)	{}
			virtual ~ResourceStream()					{}

	};

	/////////////////////////////////////////////////////////////////////

};

#endif
