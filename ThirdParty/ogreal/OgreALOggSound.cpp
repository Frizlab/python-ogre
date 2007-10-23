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

#include "OgreALOggSound.h"
#include "OgreALSoundManager.h"
#include "OgreALException.h"

namespace OgreAL {
	OggSound::OggSound(const Ogre::String& name, const Ogre::String& soundFile, bool loop, AudioFormat format) :
		Sound(name, soundFile),
		mOggFile(0),
		mVorbisInfo(0),
		mVorbisComment(0)
	{
		try
		{
			mOggFile = fopen(soundFile.c_str(), "rb");
			OGREAL_CHECK(mOggFile != 0, 1, "Could not open Ogg file.");

			OGREAL_CHECK(ov_open(mOggFile, &mOggStream, NULL, 0) >= 0, 1, "Could not open Ogg stream.");
			mVorbisInfo = ov_info(&mOggStream, -1);

			unsigned long channels = mVorbisInfo->channels;
			mFreq = mVorbisInfo->rate;
			mLoop = loop;

			switch(channels)
			{
			case 1:
				mFormat = AL_FORMAT_MONO16;
				// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
				mBufferSize = mFreq >> 1;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				mBufferSize -= (mBufferSize % 2);
				break;
			case 2:
				mFormat = AL_FORMAT_STEREO16;
				// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
				mBufferSize = mFreq;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				mBufferSize -= (mBufferSize % 4);
				break;
			case 4:
				mFormat = alGetEnumValue("AL_FORMAT_QUAD16");
				// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
				mBufferSize = mFreq * 2;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				mBufferSize -= (mBufferSize % 8);
				break;
			case 6:
				mFormat = alGetEnumValue("AL_FORMAT_51CHN16");
				// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
				mBufferSize = mFreq * 3;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				mBufferSize -= (mBufferSize % 12);
				break;
			case 7:
				mFormat = alGetEnumValue("AL_FORMAT_61CHN16");
				// Set BufferSize to 250ms (Frequency * 16 (16bit 7-channel) divided by 4 (quarter of a second))
				mBufferSize = mFreq * 4;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				mBufferSize -= (mBufferSize % 16);
				break;
			case 8:
				mFormat = alGetEnumValue("AL_FORMAT_71CHN16");
				// Set BufferSize to 250ms (Frequency * 20 (16bit 8-channel) divided by 4 (quarter of a second))
				mBufferSize = mFreq * 5;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				mBufferSize -= (mBufferSize % 20);
				break;
			default:
				// Couldn't determine buffer format so log the error and default to mono
				Ogre::LogManager::getSingleton().logMessage("!!WARNING!! Could not determine buffer format!  Defaulting to MONO");

				mFormat = AL_FORMAT_MONO16;
				// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
				mBufferSize = mFreq >> 1;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				mBufferSize -= (mBufferSize % 2);
				break;
			}

			alGenBuffers(1, &mBuffer);
			OGREAL_CHECK(alGetError() == AL_NO_ERROR, 13, "Could not generate buffer");
			int currSection;
			long size = 0;
			char data[4096*8];
			std::vector<char> buffer;
			do
			{
				size = ov_read(&mOggStream, data, sizeof(data), 0, 2, 1, &currSection);
				buffer.insert(buffer.end(), data, data + size);
			}while(size > 0);

			ov_clear(&mOggStream);
			alBufferData(mBuffer, mFormat, &buffer[0], static_cast<ALsizei>(buffer.size()), mFreq);
			OGREAL_CHECK(alGetError() == AL_NO_ERROR, 13, "Could not generate buffer");
		}
		catch(Exception e)
		{
			if (mBuffer)
			{
				if (alIsBuffer(mBuffer) == AL_TRUE)
				{
					alDeleteBuffers(1, &mBuffer);
				}
			}

			if(mOggFile)
			{
				fclose(mOggFile);
				mOggFile = 0;
			}

			ov_clear(&mOggStream);

			throw (e);
		}

		createAndBindSource();
	}

	OggSound::~OggSound()
	{}

	Ogre::String OggSound::errorToString(int error) const
	{
		switch(error)
		{
			case OV_EREAD:
				return Ogre::String("Read from media.");
			case OV_ENOTVORBIS:
				return Ogre::String("Not Vorbis data.");
			case OV_EVERSION:
				return Ogre::String("Vorbis version mismatch.");
			case OV_EBADHEADER:
				return Ogre::String("Invalid Vorbis header.");
			case OV_EFAULT:
				return Ogre::String("Internal logic fault (bug or heap/stack corruption.");
			default:
				return Sound::errorToString(error);
		}
	}
} // Namespace
