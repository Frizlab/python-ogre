/*---------------------------------------------------------------------------*\
** This source file is part of OgreAL                                        **
** an OpenAL plugin for the Ogre Rendering Engine.                           **
**                                                                           **
** Copyright 2006 Casey Borders                                              **
**                                                                           **
** OgreAL is free software; you can redistribute it and/or modify it under   **
** the terms of the GNU Lesser General Public License as published by the    **
** Free Software Foundation; either version 2, or (at your option) any later **
** version.                                                                  **
**                                                                           **
** The developer really likes screenshots and while he recognises that the   **
** fact that this is an AUDIO plugin means that the fruits of his labor will **
** never been seen in these images he would like to kindly ask that you send **
** screenshots of your application using his library to                      **
** screenshots@mooproductions.org                                            **
**                                                                           **
** Please bear in mind that the sending of these screenshots means that you  **
** are agreeing to allow the developer to display them in the media of his   **
** choice.  They will, however, be fully credited to the person sending the  **
** email or, if you wish them to be credited differently, please state that  **
** in the body of the email.                                                 **
**                                                                           **
** OgreAL is distributed in the hope that it will be useful, but WITHOUT     **
** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     **
** FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for **
** more details.                                                             **
**                                                                           **
** You should have received a copy of the GNU General Public License along   **
** with OgreAL; see the file LICENSE.  If not, write to the                  **
** Free Software Foundation, Inc.,                                           **
** 59 Temple Place - Suite 330,                                              **
** Boston, MA 02111-1307, USA.                                               **
\*---------------------------------------------------------------------------*/

#ifndef _OGRE_AL_PREREQS_H_
#define _OGRE_AL_PREREQS_H_

#include "Ogre.h"

namespace OgreAL{
	#if OGRE_COMPILER == OGRE_COMPILER_MSVC
	#	include "al.h"
	#	include "alc.h"
	#	include "xram.h"
	#   ifndef OgreAL_Export
    	#	ifdef OGRE_AL_EXPORT 
    	#		define OgreAL_Export __declspec(dllexport)
    	#	else
    	#		define OgreAL_Export __declspec(dllimport)
	    #	endif
	#   endif    
	#elif OGRE_COMPILER == OGRE_COMPILER_GNUC
	#	include "AL/al.h"
	#	include "AL/alc.h"
	#	if defined(OGRE_AL_EXPORT) && OGRE_COMP_VER >= 400
	#		define OgreAL_Export __attribute__ ((visibility("default")))
	#	else
	#		define OgreAL_Export
	#	endif
		// fake EAX/X-RAM
		typedef ALenum (*EAXGetBufferMode)(ALuint buffer, ALint *pReserved);
		typedef ALboolean (*EAXSetBufferMode)(ALsizei n, ALuint *buffers, ALint value);
	#else // Other Compilers
	#	include "al.h"
	#	include "alc.h"
	#	include "xram.h"
	#	define OgreAL_Export
	#endif

	enum AudioFormat
	{
		DEFAULT,
		MONO_CHANNEL,
		STEREO_CHANNEL,
		QUAD_CHANNEL,
		MULTI_CHANNEL_51,
		MULTI_CHANNEL_61,
		MULTI_CHANNEL_71
	};

	enum Exception
	{
		SOUND_MANAGER	 = 0x100,
		SOUND_FACTORY	 = 0x200,
		SOUND			 = 0x300,
		LISTENER_FACTORY = 0x400,
		LISTENER		 = 0x500
	};

	typedef ALenum BufferFormat;
	typedef ALenum State;
	typedef ALuint SourceRef;
	typedef ALuint BufferRef;
	typedef ALuint EAXMode;
	typedef ALsizei Size;
	typedef ALchar* ALString;

	struct DeleteObject
	{
		template<typename T>
		void operator()(const T* ptr)const
		{
			delete ptr;
		}
	};

	struct DeleteFirst
	{
		template<typename T1, typename T2>
		void operator()(std::pair<T1*, T2> pair)const
		{
			delete pair.first;
		}
	};

	struct DeleteSecond
	{
		template<typename T1, typename T2>
		void operator()(std::pair<T1, T2*> pair)const
		{
			delete pair.second;
		}
	};

	static unsigned short readByte16(const unsigned char buffer[2])
	{
		#if(OGRE_ENDIAN == OGRE_ENDIAN_BIG)
			return (buffer[0] << 8) + buffer[1];
		#else
			return (buffer[1] << 8) + buffer[0];
		#endif
	}
	static unsigned long readByte32(const unsigned char buffer[4])
	{
		#if(OGRE_ENDIAN == OGRE_ENDIAN_BIG)
			return (buffer[0] << 24) + (buffer[1] << 16) + (buffer[2] << 8) + buffer[3];
		#else
			return (buffer[3] << 24) + (buffer[2] << 16) + (buffer[1] << 8) + buffer[0];
		#endif
	}
}
#endif