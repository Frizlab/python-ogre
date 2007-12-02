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

#include "OgreALException.h"
#include "OgreALSoundManager.h"

template<> OgreAL::SoundManager* Ogre::Singleton<OgreAL::SoundManager>::ms_Singleton = 0;

namespace OgreAL {
	const Ogre::String SoundManager::SOUND_FILE = "SoundFile";
	const Ogre::String SoundManager::LOOP_STATE = "LoopState";
	const Ogre::String SoundManager::STREAM = "Stream";

	const ALenum SoundManager::xRamAuto = alGetEnumValue("AL_STORAGE_AUTO");
	const ALenum SoundManager::xRamHardware = alGetEnumValue("AL_STORAGE_HARDWARE");
	const ALenum SoundManager::xRamAccessible = alGetEnumValue("AL_STORAGE_ACCESSIBLE");

	SoundManager::SoundManager(const Ogre::String& deviceName) : 
		mEAXSupport(false),
		mEAXVersion(0),
		mXRAMSupport(false),
		mContext(0),
		mDevice(0),
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

		initializeDevice(deviceName);

		checkFeatureSupport();

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
		mSupportedFormats.clear();

		Ogre::LogManager::getSingleton().logMessage("*-*-* Releasing OpenAL");

		// Release the OpenAL Context and the Audio device
		alcMakeContextCurrent(NULL);
		CheckError(alcGetError(mDevice), "Failed to make context current");

		alcDestroyContext(mContext);
		CheckError(alcGetError(mDevice), "Failed to destroy context");

		alcCloseDevice(mDevice);
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
		const Ogre::String& fileName, bool loop, bool stream)
	{
		CheckCondition(mSoundMap.find(name) == mSoundMap.end(), 13, "A Sound with name '" + name + "' already exists.");

		mFileTypePair.clear();
		mFileTypePair[SOUND_FILE] = fileName;
		mFileTypePair[LOOP_STATE] = Ogre::StringConverter::toString(loop);
		mFileTypePair[STREAM] = Ogre::StringConverter::toString(stream);
		
		Sound *newSound = static_cast<Sound*>(mSoundFactory->createInstance(name, NULL, &mFileTypePair));
		mSoundMap[name] = newSound;
		return newSound;
	}

	Sound* SoundManager::getSound(const Ogre::String& name) const
	{
		SoundMap::const_iterator soundItr = mSoundMap.find(name);
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

	void SoundManager::createListener()
	{
		Listener *listener = Listener::getSingletonPtr();
		if(!listener)
		{
			listener = static_cast<Listener*>
				(mListenerFactory->createInstance("ListenerSingleton", NULL));
		}
	}

	Listener* SoundManager::getListener() const
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

	void SoundManager::setDopplerFactor(Ogre::Real dopplerFactor)
	{
		mDopplerFactor = dopplerFactor;
		alDopplerFactor(mDopplerFactor);
		CheckError(alGetError(), "Failed to set Doppler Factor");
	}

	void SoundManager::setSpeedOfSound(Ogre::Real speedOfSound)
	{
		mSpeedOfSound = speedOfSound;
		alSpeedOfSound(mSpeedOfSound);
		CheckError(alGetError(), "Failed to set Speed of Sound");
	}

	Ogre::StringVector SoundManager::getDeviceList()
	{
		const ALCchar* deviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

		Ogre::StringVector deviceVector;
		/*
		** The list returned by the call to alcGetString has the names of the
		** devices seperated by NULL characters and the list is terminated by
		** two NULL characters, so we can cast the list into a string and it
		** will automatically stop at the first NULL that it sees, then we
		** can move the pointer ahead by the lenght of that string + 1 and we
		** will be at the begining of the next string.  Once we hit an empty 
		** string we know that we've found the double NULL that terminates the
		** list and we can stop there.
		*/
		while(*deviceList != NULL)
		{
			ALCdevice *device = alcOpenDevice(deviceList);
			if(device)
			{
				// Device seems to be valid
				ALCcontext *context = alcCreateContext(device, NULL);
				if(context)
				{
					// Context seems to be valid
					alcMakeContextCurrent(context);
					deviceVector.push_back(alcGetString(device, ALC_DEVICE_SPECIFIER));
					alcDestroyContext(context);
				}
				alcCloseDevice(device);
			}
			deviceList += strlen(deviceList) + 1;
		}

		return deviceVector;
	}

	FormatMapIterator SoundManager::getSupportedFormatIterator()
	{
		return FormatMapIterator(mSupportedFormats.begin(), mSupportedFormats.end());
	}

	const FormatData* SoundManager::retrieveFormatData(AudioFormat format) const
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

	void SoundManager::_removeBufferRef(const Ogre::String& bufferName)
	{
		mSoundFactory->_removeBufferRef(bufferName);
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
			CheckError(alGetError(), "Failed to delete Source");
		}

		return numSources;
	}

	void SoundManager::initializeDevice(const Ogre::String& deviceName)
	{
		bool deviceInList = false;

		Ogre::LogManager::getSingleton().logMessage("OpenAL Devices");
		Ogre::LogManager::getSingleton().logMessage("--------------");

		std::stringstream ss;

		// List devices in log and see if the sugested device is in the list
		Ogre::StringVector deviceList = getDeviceList();
		Ogre::StringVector::iterator deviceItr;
		for(deviceItr = deviceList.begin(); deviceItr != deviceList.end(); deviceItr++)
		{
			deviceInList = (*deviceItr).compare(deviceName) == 0;
			ss << "  * " << (*deviceItr);
			Ogre::LogManager::getSingleton().logMessage(ss.str());
			ss.clear(); ss.str("");
		}

		// If the suggested device is in the list we use it, otherwise select the default device
		mDevice = alcOpenDevice(deviceInList ? deviceName.c_str() : NULL);
		CheckError(alcGetError(mDevice), "Failed to open Device");
		CheckCondition(mDevice != NULL, 13, "Failed to open audio device");

		Ogre::LogManager::getSingleton().logMessage("Choosing: " + Ogre::String(alcGetString(mDevice, ALC_DEVICE_SPECIFIER)));

		// Create OpenAL Context
		mContext = alcCreateContext(mDevice, NULL);
		CheckError(alcGetError(mDevice), "Failed to create Context");
		CheckCondition(mContext != NULL, 13, "Failed to create OpenAL Context");

		alcMakeContextCurrent(mContext);
		CheckError(alcGetError(mDevice), "Failed to set current context");
	}

	void SoundManager::checkFeatureSupport()
	{
		// Check for Supported Formats
		ALenum eBufferFormat = 0;
		eBufferFormat = alcGetEnumValue(mDevice, "AL_FORMAT_MONO16");
		if(eBufferFormat) mSupportedFormats[MONO_CHANNEL] = 
			new FormatData(eBufferFormat, "AL_FORMAT_MONO16", "Monophonic Sound");
		eBufferFormat = alcGetEnumValue(mDevice, "AL_FORMAT_STEREO16");
		if(eBufferFormat) mSupportedFormats[STEREO_CHANNEL] = 
			new FormatData(eBufferFormat, "AL_FORMAT_STEREO16", "Stereo Sound");
		eBufferFormat = alcGetEnumValue(mDevice, "AL_FORMAT_QUAD16");
		if(eBufferFormat) mSupportedFormats[QUAD_CHANNEL] = 
			new FormatData(eBufferFormat, "AL_FORMAT_QUAD16", "4 Channel Sound");
		eBufferFormat = alcGetEnumValue(mDevice, "AL_FORMAT_51CHN16");
		if(eBufferFormat) mSupportedFormats[MULTI_CHANNEL_51] = 
			new FormatData(eBufferFormat, "AL_FORMAT_51CHN16", "5.1 Surround Sound");
		eBufferFormat = alcGetEnumValue(mDevice, "AL_FORMAT_61CHN16");
		if(eBufferFormat) mSupportedFormats[MULTI_CHANNEL_61] = 
			new FormatData(eBufferFormat, "AL_FORMAT_61CHN16", "6.1 Surround Sound");
		eBufferFormat = alcGetEnumValue(mDevice, "AL_FORMAT_71CHN16");
		if(eBufferFormat) mSupportedFormats[MULTI_CHANNEL_71] = 
			new FormatData(eBufferFormat, "AL_FORMAT_71CHN16", "7.1 Surround Sound");

		// Log supported formats
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

		// See how many sources the harware allows
		mMaxNumSources = _getMaxSources();
	}
} // Namespace
