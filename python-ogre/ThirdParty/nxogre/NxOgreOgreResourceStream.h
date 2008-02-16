/** \file    NxOgreOgreResourceStream.h
 *  \brief   Header for the OgreResourceStream class.
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

#ifndef __NXOGRE_OGRE_RESOURCE_STREAM_H__
#define __NXOGRE_OGRE_RESOURCE_STREAM_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreResourceStream.h"
#include "OgreDataStream.h"				// For DataStreamPtr

namespace NxOgre {
	
	/////////////////////////////////////////////////////////////////////

	class NxPublicClass OgreResourceStream : public ResourceStream {

		public:

			
			OgreResourceStream(ResourceIdentifier);
			OgreResourceStream(ResourceIdentifier, void*);
			virtual	~OgreResourceStream();

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
			void	writeUChar(unsigned char){}

			/** Write short to the stream*/
			void	writeUShort(NxU16){}

			/** Write unsigned int to the stream*/
			void	writeUInt(unsigned int){}

			/** Write float to the stream*/
			void	writeFloat(float){}

			/** Write double to the stream*/
			void	writeDouble(double){}

			/** Write a stl string to the stream*/
			void	writeString(std::string){}

			/** Write any data to the stream*/
			void	write(const void*, size_t length){}

			/** Get a char from the stream*/
			unsigned char	getUChar()	const;

			/** Get a short from the stream*/
			NxU16	getShort()			const;

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


			Ogre::DataStreamPtr data;
	};

	/////////////////////////////////////////////////////////////////////

};

#endif
