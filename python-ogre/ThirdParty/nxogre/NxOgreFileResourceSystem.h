//
//	NxFile a wrapper for the PhysX (formerly Novodex) physics library and the File 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxFile.org http://www.nxFile.org
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

#ifndef __NXOGRE_FILE_RESOURCE_SYSTEM_H__
#define __NXOGRE_FILE_RESOURCE_SYSTEM_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreResourceSystem.h"
#include "NxOgreResourceStream.h"
#include "NxStream.h"

namespace NxOgre {

	/////////////////////////////////////////////////////////////////////

	class NxExport FileResourceSystem : public ResourceSystem {
		
		public:

			FileResourceSystem();
			~FileResourceSystem();

			void init();

			virtual ResourceStream*		getStream(ResourceIdentifier);
			virtual ResourceStream*		getStream(ResourceIdentifier, void*);
			virtual ResourceAccessList	getAllNXS()						{ResourceAccessList list; return list;}
			virtual ResourceAccessList	getAllMESH()					{ResourceAccessList list; return list;}
			virtual ResourceAccessList	getAllNXM()						{ResourceAccessList list; return list;}
			virtual ResourceAccessList	getAllEndingWith(NxString)		{ResourceAccessList list; return list;}
			virtual void				loadThese(ResourceAccessList)	{}

	};

	/////////////////////////////////////////////////////////////////////

	class NxExport FileResourceStream : public ResourceStream {

		public:

			FileResourceStream(ResourceIdentifier);
			FileResourceStream(ResourceIdentifier, void*);

			virtual		~FileResourceStream();

			/** Re-use the stream
			*/
			virtual		void			reuse(ResourceIdentifier);

			/** Close the stream
			*/
			virtual		void			close();
			
			/** The size of the stream.
			*/
			virtual		NxU32			size()		const;

			/** Set the read pointer to X bytes from the beginning of the stream.
			*/
			virtual		void			seek(long);
			
			/** Skip forward so many bytes from the current position of the read pointer
			*/
			virtual		void			skip(long);
			
			/** How far is the read pointer from the beginning of the stream
			*/
			virtual		size_t			tell()		const;						
			
			/** Reset the readPointer to the beginning.
			*/
			virtual		void			rewind();

			/** Write char to the stream*/
			void	writeUChar(unsigned char);

			/** Write short to the stream*/
			void	writeUShort(unsigned short);

			/** Write unsigned int to the stream*/
			void	writeUInt(unsigned int);

			/** Write float to the stream*/
			void	writeFloat(float);

			/** Write double to the stream*/
			void	writeDouble(double);

			/** Write a stl string to the stream*/
			void	writeString(std::string);

			/** Write any data to the stream*/
			void	write(const void*, size_t length);

			/** Get a char from the stream*/
			unsigned char	getUChar()	const;

			/** Get a short from the stream*/
			unsigned short	getShort()	const;

			/** Get a unsigned int from the stream*/
			unsigned int	getUInt()	const;

			/** Get a float from the stream*/
			float			getFloat()	const;

			/** Get a double from the stream*/
			double			getDouble()	const;

			/** Get a stl string from the stream*/
			std::string		getString()	const;

			/** Get any data from the stream*/
			void			get(void* buffer, size_t length) const;

			FILE*			fp;
			NxU32			mSize;
	};

	/////////////////////////////////////////////////////////////////////

};

#endif
