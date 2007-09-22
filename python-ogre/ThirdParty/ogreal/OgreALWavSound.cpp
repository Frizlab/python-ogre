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

#include "OgreALWavSound.h"
#include "OgreALSoundManager.h"

namespace OgreAL {
	WavSound::WavSound(const Ogre::String& name, const Ogre::String& soundFile, bool loop, AudioFormat format) :
		Sound(name, soundFile)
	{
		alGenBuffers(1, &mBuffer);
		checkError(__FUNCTION__);

		ALbyte* filename = reinterpret_cast<ALbyte*>(const_cast<char*>(soundFile.c_str()));
		alutLoadWAVFile(filename, &mFormat, &mData, &mSize, &mFreq, &mLoop);

		// Use a different buffer format?
		const FormatData *formatData = SoundManager::getSingleton().retrieveFormatData(format);
		if(formatData)
			mFormat = formatData->formatEnum;

		alBufferData(mBuffer, mFormat, mData, mSize, mFreq);
		alutUnloadWAV(mFormat, mData, mSize, mFreq);

		mLoop = loop?AL_TRUE:AL_FALSE;

		createAndBindSource();
	}

	WavSound::~WavSound()
	{}

	Ogre::String WavSound::errorToString(int error) const
	{
		return Sound::errorToString(error);
	}
} // Namespace
