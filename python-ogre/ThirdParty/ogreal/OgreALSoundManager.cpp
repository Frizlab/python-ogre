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

#include "OgreALSoundManager.h"

template<> OgreAL::SoundManager* Ogre::Singleton<OgreAL::SoundManager>::ms_Singleton = 0;

namespace OgreAL {
	const Ogre::String SoundManager::FILE_TYPE = "FileType";
	const Ogre::String SoundManager::OGG = "OggFile";
	const Ogre::String SoundManager::WAV = "WavFile";
	const Ogre::String SoundManager::SOUND_FILE = "SoundFile";
	const Ogre::String SoundManager::LOOP_STATE = "LoopState";
	const Ogre::String SoundManager::OUTPUT_TYPE = "OutputType";
	const Ogre::String SoundManager::SOUND = "SoundOut";
	const Ogre::String SoundManager::STREAM = "StreamOut";
	const Ogre::String SoundManager::AUDIO_FORMAT = "AudioFormat";

	const ALenum SoundManager::xRamAuto = alGetEnumValue("AL_STORAGE_AUTO");
	const ALenum SoundManager::xRamHardware = alGetEnumValue("AL_STORAGE_HARDWARE");
	const ALenum SoundManager::xRamAccessible = alGetEnumValue("AL_STORAGE_ACCESSIBLE");

	SoundManager::SoundManager() : 
		mEAXSupport(false),
		mEAXVersion(0),
		mXRAMSupport(false),
		mDopplerFactor(1.0),
		mSpeedOfSound(343.3)
	{
		Ogre::LogManager::getSingleton().logMessage("*-*-* OgreAL Initialization");

		// Create and register Sound and Listener Factories
		mSoundFactory = new SoundFactory();
		mListenerFactory = new ListenerFactory();
		mResourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
		Ogre::Root::getSingleton().addMovableObjectFactory(mSoundFactory);
		Ogre::Root::getSingleton().addMovableObjectFactory(mListenerFactory);

		Ogre::LogManager::getSingleton().logMessage("*-*-* Creating OpenAL");

		// Enumerate the available devices and choose one
		alDeviceList(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
		/*
		** Currently we just choose the default device, but we should allow the user to 
		** pick which one they would like to use.
		*/
		ALCdevice *device = alcOpenDevice(NULL);

		Ogre::LogManager::getSingleton().logMessage("Choosing: " + Ogre::String(alcGetString(device, ALC_DEVICE_SPECIFIER)));

		if(device == NULL)
		{
			throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,
				"Failed to open audio device", "OgreAL::SoundManager::ctor");
		}

		// Create OpenAL Context
		ALCcontext *context = alcCreateContext(device, NULL);
		if(context == NULL)
		{
			throw Ogre::Exception(Ogre::Exception::ERR_INTERNAL_ERROR,
				"Failed to create OpenAL Context", "OgreAL::SoundManager::ctor");
		}

		alcMakeContextCurrent(context);
		checkError("SoundManager::ctor");

		// Check for Supported Formats
		ALenum eBufferFormat = 0;
		eBufferFormat = alcGetEnumValue(device, "AL_FORMAT_MONO16");
		if(eBufferFormat) mSupportedFormats[MONO_CHANNEL] = 
			new FormatData(eBufferFormat, "AL_FORMAT_MONO16", "Monophonic Sound");
		eBufferFormat = alcGetEnumValue(device, "AL_FORMAT_STEREO16");
		if(eBufferFormat) mSupportedFormats[STEREO_CHANNEL] = 
			new FormatData(eBufferFormat, "AL_FORMAT_STEREO16", "Stereo Sound");
		eBufferFormat = alcGetEnumValue(device, "AL_FORMAT_QUAD16");
		if(eBufferFormat) mSupportedFormats[QUAD_CHANNEL] = 
			new FormatData(eBufferFormat, "AL_FORMAT_QUAD16", "4 Channel Sound");
		eBufferFormat = alcGetEnumValue(device, "AL_FORMAT_51CHN16");
		if(eBufferFormat) mSupportedFormats[MULTI_CHANNEL_51] = 
			new FormatData(eBufferFormat, "AL_FORMAT_51CHN16", "5.1 Surround Sound");
		eBufferFormat = alcGetEnumValue(device, "AL_FORMAT_61CHN16");
		if(eBufferFormat) mSupportedFormats[MULTI_CHANNEL_61] = 
			new FormatData(eBufferFormat, "AL_FORMAT_61CHN16", "6.1 Surround Sound");
		eBufferFormat = alcGetEnumValue(device, "AL_FORMAT_71CHN16");
		if(eBufferFormat) mSupportedFormats[MULTI_CHANNEL_71] = 
			new FormatData(eBufferFormat, "AL_FORMAT_71CHN16", "7.1 Surround Sound");

		FormatMapIterator itr = getSupportedFormatIterator();
		Ogre::LogManager::getSingleton().logMessage("Supported Formats");
		Ogre::LogManager::getSingleton().logMessage("-----------------");
		while(itr.hasMoreElements())
		{
			Ogre::LogManager::getSingleton().logMessage(" * " + std::string(static_cast<char*>
				(itr.peekNextValue()->formatName)) + ", " + itr.peekNextValue()->formatDescription);
			itr.getNext();
		}

		// Check for EAX Support
		std::stringbuf versionString;
		for(int version = 5; version >= 2; version--)
		{
			versionString.str("EAX"+Ogre::StringConverter::toString(version)+".0");
			if(alIsExtensionPresent(versionString.str().data()) == AL_TRUE)
			{
				mEAXSupport = true;
				mEAXVersion = version;
				versionString.str("EAX " + Ogre::StringConverter::toString(version) + ".0 Detected");
				Ogre::LogManager::getSingleton().logMessage(versionString.str());
				break;
			}
		}

		// See how many sources the harware allows
		mMaxNumSources = _getMaxSources();

		// Check for X-RAM extension
		if(alIsExtensionPresent("EAX-RAM") == AL_TRUE)
		{
			mXRAMSupport = true;
			Ogre::LogManager::getSingleton().logMessage("X-RAM Detected");

			EAXSetBufferMode setXRamMode = (EAXSetBufferMode)alGetProcAddress("EAXSetBufferMode");
			EAXGetBufferMode getXRamMode = (EAXGetBufferMode)alGetProcAddress("EAXGetBufferMode");
			mXRamSize = alGetEnumValue("AL_EAX_RAM_SIZE");
			mXRamFree = alGetEnumValue("AL_EAX_RAM_FREE");
			
			Ogre::LogManager::getSingleton().logMessage("X-RAM: " + Ogre::StringConverter::toString(mXRamSize) +
				" (" + Ogre::StringConverter::toString(mXRamFree) + " free)");
		}

		createListener();

		Ogre::Root::getSingleton().addFrameListener(this);
	}

	SoundManager::~SoundManager()
	{
		Ogre::LogManager::getSingleton().logMessage("*-*-* OgreAL Shutdown");

		Ogre::Root::getSingleton().removeFrameListener(this);

		// Destroy the Listener and all Sounds
		destroyAllSounds();
		delete Listener::getSingletonPtr();

		// Unregister the Sound and Listener Factories
		Ogre::Root::getSingleton().removeMovableObjectFactory(mSoundFactory);
		Ogre::Root::getSingleton().removeMovableObjectFactory(mListenerFactory);

		delete mListenerFactory;
		delete mSoundFactory;

		// delete all FormatData pointers in the FormatMap;
		std::for_each(mSupportedFormats.begin(), mSupportedFormats.end(), DeleteSecond());

		Ogre::LogManager::getSingleton().logMessage("*-*-* Releasing OpenAL");

		// Release the OpenAL Context and the Audio device
		ALCcontext *context = alcGetCurrentContext();
		ALCdevice *device = alcGetContextsDevice(context);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	SoundManager* SoundManager::getSingletonPtr(void)
	{
		return ms_Singleton;
	}

	SoundManager& SoundManager::getSingleton(void)
	{  
		assert( ms_Singleton );  return (*ms_Singleton);  
	}

	Sound* SoundManager::createSound(const Ogre::String& name, 
		const Ogre::String& fileName, bool loop, AudioFormat format)
	{
		mFileTypePair.clear();
		mFileTypePair[OUTPUT_TYPE] = SOUND;
		mFileTypePair[AUDIO_FORMAT] = Ogre::StringConverter::toString(format);

		return _createSound(name, fileName, loop);
	}

	Sound* SoundManager::createSoundStream(const Ogre::String& name, 
		const Ogre::String& fileName, bool loop, AudioFormat format)
	{
		mFileTypePair.clear();
		mFileTypePair[OUTPUT_TYPE] = STREAM;
		mFileTypePair[AUDIO_FORMAT] = Ogre::StringConverter::toString(format);

		return _createSound(name, fileName, loop);
	}

	Sound* SoundManager::_createSound(const Ogre::String& name, const Ogre::String& fileName, bool loop)
	{
		Ogre::String path = "";
		Ogre::String group = mResourceGroupManager->findGroupContainingResource(fileName);
		Ogre::FileInfoListPtr resourceList = mResourceGroupManager->listResourceFileInfo(group);
		Ogre::FileInfoList::iterator itr = resourceList->begin();
		for(;itr != resourceList->end(); itr++)
		{
			if(itr->basename.compare(fileName) == 0)
			{
				path = itr->archive->getName();
				break;
			}
		}
		if(path.compare("") == 0)
		{
			throw Ogre::Exception(Ogre::Exception::ERR_FILE_NOT_FOUND,
				"Unable to find " + fileName + ". Is it in the resources.cfg?",
				"OgreAL::SoundManager::_createSound");
		}	
		if(fileName.find(".ogg") != std::string::npos || fileName.find(".OGG") != std::string::npos)
		{
			mFileTypePair[FILE_TYPE] = OGG;
		}
		else if(fileName.find(".wav") != std::string::npos || fileName.find(".WAV") != std::string::npos)
		{
			mFileTypePair[FILE_TYPE] = WAV;
		}
		else
		{
			throw Ogre::Exception(Ogre::Exception::ERR_INVALIDPARAMS,
				"Sound file '" + fileName + "' is of an unsupported file type, ",
				"OgreAL::SoundManager::_createSound");
		}

		mFileTypePair[SOUND_FILE] = path + "/" + fileName;
		mFileTypePair[LOOP_STATE] = Ogre::StringConverter::toString(loop);

		if (mSoundMap.find(name) != mSoundMap.end())
		{
			throw Ogre::Exception(Ogre::Exception::ERR_DUPLICATE_ITEM,
				"A Sound with name '" + name + "' already exists.",
				"OgreAL::SoundManager::_createSound");
		}

		Sound *newSound = static_cast<Sound*>(mSoundFactory->createInstance(name, NULL, &mFileTypePair));
		mSoundMap[name] = newSound;
		return newSound;
	}

	Sound* SoundManager::getSound(const Ogre::String& name)
	{
		SoundMap::iterator soundItr = mSoundMap.find(name);
		if(soundItr == mSoundMap.end())
		{
			throw Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND, 
					"Object named '" + name + "' does not exist.", 
					"SceneManager::getMovableObject");
		}

		return soundItr->second;
	}

	bool SoundManager::hasSound(const Ogre::String& name) const
	{
		SoundMap::const_iterator soundItr = mSoundMap.find(name);
		if(soundItr != mSoundMap.end())
			return true;
		else
			return false;
	}

	void SoundManager::destroySound(Sound *sound)
	{
		destroySound(sound->getName());
	}

	void SoundManager::destroySound(const Ogre::String& name)
	{
		SoundMap::iterator soundItr = mSoundMap.find(name);
		if(soundItr != mSoundMap.end())
		{			
			mSoundFactory->destroyInstance(soundItr->second);
			mSoundMap.erase(soundItr);
		}
	}

	void SoundManager::destroyAllSounds()
	{
		// delete all Sound pointers in the SoundMap
		std::for_each(mSoundMap.begin(), mSoundMap.end(), DeleteSecond());
		mSoundMap.clear();
	}
	
	void SoundManager::pauseAllSounds()
	{
		SoundMap::iterator soundItr;
		for(soundItr = mSoundMap.begin(); soundItr != mSoundMap.end(); soundItr++)
		{
			if(soundItr->second->isPlaying())
			{
				soundItr->second->pause();
				mPauseResumeAll.push_back(soundItr->second);
			}
		}
	}

	void SoundManager::resumeAllSounds()
	{
		SoundList::iterator soundItr;
		for(soundItr = mPauseResumeAll.begin(); soundItr != mPauseResumeAll.end(); soundItr++)
		{
			(*soundItr)->play();
		}
		mPauseResumeAll.clear();
	} 

	Listener* SoundManager::createListener()
	{
		Listener *listener = Listener::getSingletonPtr();
		if(!listener)
		{
			listener = static_cast<Listener*>
				(mListenerFactory->createInstance("ListenerSingleton", NULL));
		}

		return listener;
	}

	Listener* SoundManager::getListener()
	{
		return Listener::getSingletonPtr();
	}

	struct SoundManager::UpdateSound
	{
		void operator()(std::pair<std::string, Sound*> pair)const
		{
			pair.second->_updateSound();
		}
	};

	bool SoundManager::frameStarted(const Ogre::FrameEvent& evt)
	{
		// Update the Sound and Listeners if necessary	
		std::for_each(mSoundMap.begin(), mSoundMap.end(), UpdateSound());
		Listener::getSingleton().updateListener();
		return true;
	}

	void SoundManager::checkError(const Ogre::String& reference) const
	{
		/*
		** TODO: Handle this better, I should reall do a look up against the OpenAL
		** error code in order to return some sort of helpful, textual feedback
		*/
		int error = alGetError();
	 
		if(error != AL_NO_ERROR)
			throw Ogre::Exception(1, "OpenAL error was raised.", reference);
	}

	void SoundManager::setDopplerFactor(Ogre::Real dopplerFactor)
	{
		mDopplerFactor = dopplerFactor;
		alDopplerFactor(mDopplerFactor);
	}

	void SoundManager::setSpeedOfSound(Ogre::Real speedOfSound)
	{
		mSpeedOfSound = speedOfSound;
		alSpeedOfSound(mSpeedOfSound);
	}

	FormatMapIterator SoundManager::getSupportedFormatIterator()
	{
		return FormatMapIterator(mSupportedFormats.begin(), mSupportedFormats.end());
	}

	const FormatData* SoundManager::retrieveFormatData(AudioFormat format)
	{
		FormatMap::const_iterator itr = mSupportedFormats.find(format);
		if(itr != mSupportedFormats.end())
			return itr->second;
		else
			return 0;
	}

	ALboolean SoundManager::eaxSetBufferMode(Size numBuffers, BufferRef *buffers, EAXMode bufferMode)
	{
		Ogre::LogManager::getSingleton().logMessage(" === Setting X-RAM Buffer Mode");
		if(bufferMode == xRamHardware)
		{
			ALint size;
			alGetBufferi(*buffers, AL_SIZE, &size);
			// If the buffer won't fit in xram, return false
			if(mXRamSize < (mXRamFree + size)) return AL_FALSE;

			Ogre::LogManager::getSingleton().logMessage(" === Allocating " + Ogre::StringConverter::toString(size) +
				" bytes of X-RAM");
		}
		// Try setting the buffer mode, if it fails return false
		if(mSetXRamMode(numBuffers, buffers, bufferMode) == AL_FALSE) return AL_FALSE;

		mXRamFree = alGetEnumValue("AL_EAX_RAM_FREE");
		return AL_TRUE;
	}
	
	ALenum SoundManager::eaxGetBufferMode(BufferRef buffer, ALint *reserved)
	{
		return mGetXRamMode(buffer, reserved);
	}

	int SoundManager::_getMaxSources()
	{
		SourceRef source;
		std::vector<SourceRef> sources;

		int numSources = 0;
		while (alGetError() == AL_NO_ERROR && numSources < 32)
		{
			source = 0;
			alGenSources(1, &source);
			if (source != 0)
			{
				sources.push_back(source);
				numSources++;
			}
			else
			{
				// Clear out the error
				alGetError();
				break;
			}
		}

		for(std::vector<SourceRef>::iterator it = sources.begin(); it != sources.end(); it++)
		{
			source = static_cast<SourceRef>(*it);
			alDeleteSources(1, &source);
		}

		return numSources;
	}

	void SoundManager::alDeviceList(const ALCchar *devices) const
	{
		std::string deviceString = "OpenAL Devices: ";

		// go through device list (each device terminated with a single NULL, list terminated with double NULL)
		while (*devices != NULL)
		{
			ALCdevice *device = alcOpenDevice(devices);
			if (device)
			{
				ALCcontext *context = alcCreateContext(device, NULL);
				if (context)
				{
					alcMakeContextCurrent(context);
					// if new actual device name isn't already in the list, then add it...
					deviceString.append(alcGetString(device, ALC_DEVICE_SPECIFIER));
					deviceString.append(", ");
					alcDestroyContext(context);
				}
				alcCloseDevice(device);
			}
			devices += strlen(devices) + 1;
		}
		Ogre::LogManager::getSingleton().logMessage(deviceString);
	}
} // Namespace
