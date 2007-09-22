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

#include "OgreALOggSoundStream.h"
#include "OgreALSoundManager.h"

namespace OgreAL {
	OggSoundStream::OggSoundStream(const Ogre::String& name, const Ogre::String& soundFile, bool loop, AudioFormat format) :
		SoundStream(name, soundFile),
		mOggFile(0),
		mVorbisInfo(0),
		mVorbisComment(0)
	{
		if(!(mOggFile = fopen(soundFile.c_str(), "rb")))
		{
			throw Ogre::Exception(1, "Could not open Ogg file.", "OgreAL::OggSound::ctor");
		}

		if(ov_open(mOggFile, &mOggStream, NULL, 0) < 0)
		{
			fclose(mOggFile);	 
			throw Ogre::Exception(1, "Could not open Ogg stream. ", "OgreAL::OggSound::ctor");
		}

		mVorbisInfo = ov_info(&mOggStream, -1);

		unsigned long channels = mVorbisInfo->channels;
		mFreq = mVorbisInfo->rate;
		mLoop = loop;
		
		if(channels == 1)
		{
			mFormat = AL_FORMAT_MONO16;
			// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
			mBufferSize = mFreq >> 1;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 2);
		}
		else if(channels == 2)
		{
			mFormat = AL_FORMAT_STEREO16;
			// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
			mBufferSize = mFreq;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 4);
		}
		else if(channels == 4)
		{
			mFormat = alGetEnumValue("AL_FORMAT_QUAD16");
			// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
			mBufferSize = mFreq * 2;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 8);
		}
		else if(channels == 6)
		{
			mFormat = alGetEnumValue("AL_FORMAT_51CHN16");
			// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
			mBufferSize = mFreq * 3;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 12);
		}
		else if(channels == 7)
		{
			mFormat = alGetEnumValue("AL_FORMAT_61CHN16");
			// Set BufferSize to 250ms (Frequency * 16 (16bit 7-channel) divided by 4 (quarter of a second))
			mBufferSize = mFreq * 4;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 16);
		}
		else if(channels == 8)
		{
			mFormat = alGetEnumValue("AL_FORMAT_71CHN16");
			// Set BufferSize to 250ms (Frequency * 20 (16bit 8-channel) divided by 4 (quarter of a second))
			mBufferSize = mFreq * 5;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 20);
		}
		else
		{
			// Couldn't determine buffer format so log the error and default to mono
			Ogre::LogManager::getSingleton().logMessage("!!WARNING!! Could not determine buffer format!  Defaulting to MONO");

			mFormat = AL_FORMAT_MONO16;
			// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
			mBufferSize = mFreq >> 1;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 2);
		}

		alGenBuffers(2, mBuffers);
		alGenSources(1, &mSource);
		checkError(__FUNCTION__);

		if(SoundManager::getSingleton().xRamSupport())
		{
			SoundManager::getSingleton().eaxSetBufferMode(2, mBuffers, SoundManager::xRamAccessible);
		}

		for(int i = 0; i < 2; i++)
		{
			if(!stream(mBuffers[i]))
			{
				throw Ogre::Exception(1, "Could not open Ogg file.", "OgreAL::OggSound::ctor");
			}
		}

		alSourceQueueBuffers(mSource, 2, mBuffers);
		checkError(__FUNCTION__);

		initSource();

		// There is an issue with looping Ogg streams in OpenAL
		// so we'll do it manually in the update method
		alSourcei(mSource, AL_LOOPING, AL_FALSE);
		checkError(__FUNCTION__);
	}

	OggSoundStream::~OggSoundStream()
	{
		emptyQueue();
		alDeleteBuffers(2, mBuffers);
		ov_clear(&mOggStream);

		mOggFile = 0;
		mVorbisInfo = 0;
		mVorbisComment = 0;
	}

	bool OggSoundStream::play()
	{
		if(isPlaying())
		{
			return true;
		}
		else if(isStopped())
		{
			for(int i = 0; i < 2; i++)
			{
				if(!stream(mBuffers[i])) return false;
			}
		    
			alSourceQueueBuffers(mSource, 2, mBuffers);
			alSourcePlay(mSource);
			checkError(__FUNCTION__);
			return true;
		}
		else
		{
			alSourcePlay(mSource);
			checkError(__FUNCTION__);
			return true;
		}
	}

	bool OggSoundStream::stop()
	{
		if(isStopped() || isInitial()) return true;

		int queued;
		alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued);
		checkError(__FUNCTION__);
		alSourceStop(mSource);
		checkError(__FUNCTION__);
		alSourceUnqueueBuffers(mSource, queued, mBuffers);
		checkError(__FUNCTION__);

		ov_time_seek(&mOggStream, 0);

		return true;
	}

	bool OggSoundStream::_updateSound()
	{
		// Call the parent method to update the position
		Sound::_updateSound();

		// Update the stream
		int processed;
		bool active = true;

		alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);
		checkError(__FUNCTION__);
	 
		while(processed--)
		{
			ALuint buffer;
	        
			alSourceUnqueueBuffers(mSource, 1, &buffer);
			checkError(__FUNCTION__);
	 
			active = stream(buffer);
	 
			alSourceQueueBuffers(mSource, 1, &buffer);
			checkError(__FUNCTION__);

			if(!active)
			{
				if(mLoop)
				{
					active = true;
					ov_time_seek(&mOggStream, 0);
				}
				else
				{
					stop();
				}
			}
		}
	 
		return active;
	}

	bool OggSoundStream::stream(ALuint buffer)
	{
		char *data = new char[mBufferSize];
		int  size = 0;
		int  section;
		int  result;
	 
		while(size < mBufferSize)
		{
			result = ov_read(&mOggStream, data+size, mBufferSize-size, 0, 2, 1, &section);
	    
			if(result > 0)
				size += result;
			else
				if(result < 0)
					throw Ogre::Exception(6, "Ogg Error: " + errorToString(result), "OgreAL::OggSoundStream::stream");
				else
					break;
		}
	    
		checkError(__FUNCTION__);

		if(size == 0)
			return false;
	 
		alBufferData(buffer, mFormat, data, size, mFreq);
		checkError(__FUNCTION__);
	 
		return true;
	}

	Ogre::String OggSoundStream::errorToString(int error) const
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
