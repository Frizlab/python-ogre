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

#ifndef _OGRE_AL_SOUND_STREAM_H_
#define _OGRE_AL_SOUND_STREAM_H_

#include "OgreALPrereqs.h"
#include "OgreALSound.h"

namespace OgreAL {
	/**
	 * SoundStream.
	 * @remarks
	 * SoundStreams are created using the SoundManager::createSoundStream method
	 * and then can be attached to Ogre::SceneNodes, placed at a point
	 * in space or placed at an offset relative to the listener.  They will then
	 * be updated by the SoundManager to stream in the sound.
	 *
	 * @see Sound
	 */
	class OgreAL_Export SoundStream : public Sound
	{
	protected:
		/*
		** Constructors are protected to enforce the use of the 
		** factory via SoundManager::createSound
		*/

		/** Default Constructor. */
		SoundStream();
		/** Normal Constructor. @note Should not be called directly! Use SoundManager::createSoundStream */
		SoundStream(const Ogre::String& name, const Ogre::String& fileName);

	public:
		/** Standard Destructor */
		virtual ~SoundStream();

	protected:
		/// Removes any buffers from the queue
		virtual void emptyQueue();

		friend class SoundFactory;

		ALuint mBuffers[2];
	};
} // Namespace
#endif
