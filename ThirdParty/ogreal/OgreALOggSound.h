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

#ifndef _OGRE_AL_OGG_SOUND_H_
#define _OGRE_AL_OGG_SOUND_H_

#include <string>
#include <vector>

#include "ogg/ogg.h"
#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"
#include "OgreALPrereqs.h"
#include "OgreALSound.h"

namespace OgreAL {
	/**
	 * OggSound.
	 * @note
	 * This object is only to be instantiated using the SoundManager::createSound
	 * method.
	 * @remark This is a sound that plays OggVorbis files
	 *
	 * @see OgreAL::Sound
	 */
	class OgreAL_Export OggSound : public Sound
	{
	protected:
		/*
		** Constructor is protected to enforce the use of the 
		** factory via SoundManager::createSound
		*/

		/**
		 * Constructor.
		 * @param name The name used to reference this sound
		 * @param soundFile The name of the file to load
		 * @param loop Should the sound loop once it has played
		 */
		OggSound(const Ogre::String& name, const Ogre::String& soundFile, bool loop, AudioFormat format);

	public:
		/** Standard Destructor. */
		virtual ~OggSound();

	protected:
		/// Translate the OpenAL error code to a string
		virtual Ogre::String errorToString(int error) const;

		friend class SoundFactory;

	private: 
        FILE*           mOggFile;       // file handle
        OggVorbis_File  mOggStream;     // stream handle
        vorbis_info*    mVorbisInfo;    // some formatting data
        vorbis_comment* mVorbisComment; // user comments 
	};
} // Namespace
#endif
