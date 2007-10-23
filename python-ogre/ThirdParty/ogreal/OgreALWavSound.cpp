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
#include "OgreALException.h"

namespace OgreAL {
	WavSound::WavSound(const Ogre::String& name, const Ogre::String& soundFile, bool loop, AudioFormat format) :
		Sound(name, soundFile),
		mWavFile(0)
	{
		const unsigned int BUFFER_SIZE = 32768;     // 32 KB buffers
		size_t bytes;
		std::vector <char> data;

		// Local resources
		char *array = NULL;

		// Main process
		try
		{
			// Open for binary reading
			mWavFile = fopen(soundFile.c_str(), "rb");
			OGREAL_CHECK(mWavFile != 0, 13, "Could not load wav from " + soundFile);

			// buffers
			char magic[5];
			magic[4] = '\0';
			unsigned char buffer32[4];
			unsigned char buffer16[2];

			// check magic
			OGREAL_CHECK(fread(magic,4,1,mWavFile) == 1, 13, "Cannot read wav file "+ soundFile );
			OGREAL_CHECK(std::string(magic) == "RIFF", 13, "Wrong wav file format. This file is not a .wav file (no RIFF magic): "+ soundFile );

			// skip 4 bytes (file size)
			fseek(mWavFile,4,SEEK_CUR);

			// check file format
			OGREAL_CHECK(fread(magic,4,1,mWavFile) == 1, 13, "Cannot read wav file "+ soundFile );
			OGREAL_CHECK(std::string(magic) == "WAVE", 13, "Wrong wav file format. This file is not a .wav file (no WAVE format): "+ soundFile );

			// check 'fmt ' sub chunk (1)
			OGREAL_CHECK(fread(magic,4,1,mWavFile) == 1, 13, "Cannot read wav file "+ soundFile );
			OGREAL_CHECK(std::string(magic) == "fmt ", 13, "Wrong wav file format. This file is not a .wav file (no 'fmt ' subchunk): "+ soundFile );

			// read (1)'s size
			OGREAL_CHECK(fread(buffer32,4,1,mWavFile) == 1, 13, "Cannot read wav file "+ soundFile );
			unsigned long subChunk1Size = readByte32(buffer32);
			OGREAL_CHECK(subChunk1Size >= 16, 13, "Wrong wav file format. This file is not a .wav file ('fmt ' chunk too small, truncated file?): "+ soundFile );

			// check PCM audio format
			OGREAL_CHECK(fread(buffer16,2,1,mWavFile) == 1, 13, "Cannot read wav file "+ soundFile );
			unsigned short audioFormat = readByte16(buffer16);
			OGREAL_CHECK(audioFormat == 1, 13, "Wrong wav file format. This file is not a .wav file (audio format is not PCM): "+ soundFile );

			// read number of channels
			OGREAL_CHECK(fread(buffer16,2,1,mWavFile) == 1, 13, "Cannot read wav file "+ soundFile );
			unsigned short channels = readByte16(buffer16);

			// read frequency (sample rate)
			OGREAL_CHECK(fread(buffer32,4,1,mWavFile) == 1, 13, "Cannot read wav file "+ soundFile );
			unsigned long frequency = readByte32(buffer32);

			// skip 6 bytes (Byte rate (4), Block align (2))
			fseek(mWavFile,6,SEEK_CUR);

			// read bits per sample
			OGREAL_CHECK(fread(buffer16,2,1,mWavFile) == 1, 13, "Cannot read wav file "+ soundFile );
			unsigned short bps = readByte16(buffer16);

			if (channels == 1)
				mFormat = (bps == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
			else
				mFormat = (bps == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;

			// check 'data' sub chunk (2)
			OGREAL_CHECK(fread(magic,4,1,mWavFile) == 1, 13, "Cannot read wav file "+ soundFile );
			OGREAL_CHECK(std::string(magic) == "data" || std::string(magic) == "fact", 13, "Wrong wav file format. This file is not a .wav file (no data subchunk): "+ soundFile );

			// fact is an option section we don't need to worry about
			if(std::string(magic) == "fact")
			{
				fseek(mWavFile,8,SEEK_CUR);
			}

			OGREAL_CHECK(fread(buffer32,4,1,mWavFile) == 1, 13, "Cannot read wav file "+ soundFile );
			unsigned long subChunk2Size = readByte32(buffer32);

			// The frequency of the sampling rate
			mFreq = frequency;
			OGREAL_CHECK(sizeof(mFreq) == sizeof(frequency), 13, "freq and frequency different sizes");

			array = new char[BUFFER_SIZE];

			while (data.size() != subChunk2Size)
			{
				// Read up to a buffer's worth of decoded sound data
				bytes = fread(array, 1, BUFFER_SIZE, mWavFile);

				if (bytes <= 0)
					break;

				if (data.size() + bytes > subChunk2Size)
					bytes = subChunk2Size - data.size();

				// Append to end of buffer
				data.insert(data.end(), array, array + bytes);
			}

			delete []array;
			array = NULL;

			fclose(mWavFile);
			mWavFile = 0;

			alGenBuffers(1, &mBuffer);
			OGREAL_CHECK(alGetError() == AL_NO_ERROR, 13, "Could not generate buffer");
			OGREAL_CHECK(AL_NONE != mBuffer, 13, "Could not generate buffer");

			alBufferData(mBuffer, mFormat, &data[0], Size(data.size()), mFreq);
			OGREAL_CHECK(alGetError() == AL_NO_ERROR, 13, "Could not load buffer data");

			//return buffer;
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

			if (array)
			{
				delete []array;
			}

			if(mWavFile)
			{
				fclose(mWavFile);
				mWavFile = 0;
			}

			throw (e);
		}

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
