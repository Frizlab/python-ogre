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

#if OGREAL_THREADED
	boost::thread *OgreAL::SoundManager::mOgreALThread = 0;
	bool OgreAL::SoundManager::mShuttingDown = false;
#endif

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
		mEFXSupport(false),
		mSendsPerSource(10),
		mContext(0),
		mDevice(0),
		mDopplerFactor(1.0),
		mSpeedOfSound(343.3),
		mMaxNumSources(0),
		mMajorVersion(0),
		mMinorVersion(0)
	{
		Ogre::LogManager::getSingleton().logMessage("*-*-* OgreAL Initialization");

		// Create and register Sound and Listener Factories
		mSoundFactory = new SoundFactory();
		mListenerFactory = new ListenerFactory();
		
		Ogre::Root::getSingleton().addMovableObjectFactory(mSoundFactory);
		Ogre::Root::getSingleton().addMovableObjectFactory(mListenerFactory);

		Ogre::LogManager::getSingleton().logMessage("*-*-* Creating OpenAL");

		initializeDevice(deviceName);

		checkFeatureSupport();

		createListener();

		#if OGREAL_THREADED
			// Kick off the update thread
			Ogre::LogManager::getSingleton().logMessage("Creating OgreAL Thread");
			mOgreALThread = new boost::thread(boost::function0<void>(&SoundManager::threadUpdate));
		#else
			// Register for frame events
			Ogre::Root::getSingleton().addFrameListener(this);
		#endif
	}

	SoundManager::~SoundManager()
	{
		Ogre::LogManager::getSingleton().logMessage("*-*-* OgreAL Shutdown");

		#if OGREAL_THREADED
			// Clean up the threading
			Ogre::LogManager::getSingleton().logMessage("Shutting Down OgreAL Thread");
			mShuttingDown = true;
			mOgreALThread->join();
			delete mOgreALThread;
			mOgreALThread = 0;
		#else
			// Unregister for frame events
			Ogre::Root::getSingleton().removeFrameListener(this);
		#endif

		// delete all Sound pointers in the SoundMap
		destroyAllSounds();

		// Destroy the Listener and all Sounds
		delete Listener::getSingletonPtr();

		// Clean out mActiveSounds and mQueuedSounds
		mActiveSounds.clear();
		mQueuedSounds.clear();

		// Clean up the SourcePool
		while(!mSourcePool.empty())
		{
			alDeleteSources(1, &mSourcePool.front());
			CheckError(alGetError(), "Failed to destroy source");
			mSourcePool.pop();
		}

		// delete all FormatData pointers in the FormatMap;
		std::for_each(mSupportedFormats.begin(), mSupportedFormats.end(), DeleteSecond());
		mSupportedFormats.clear();

		// Unregister the Sound and Listener Factories
		Ogre::Root::getSingleton().removeMovableObjectFactory(mSoundFactory);
		Ogre::Root::getSingleton().removeMovableObjectFactory(mListenerFactory);
		delete mListenerFactory;
		delete mSoundFactory;

		Ogre::LogManager::getSingleton().logMessage("*-*-* Releasing OpenAL");

		// Release the OpenAL Context and the Audio device
		alcMakeContextCurrent(NULL);
		alcDestroyContext(mContext);
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
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		CheckCondition(mSoundMap.find(name) == mSoundMap.end(), 13, "A Sound with name '" + name + "' already exists.");

		Ogre::NameValuePairList fileTypePair;
		fileTypePair[SOUND_FILE] = fileName;
		fileTypePair[LOOP_STATE] = Ogre::StringConverter::toString(loop);
		fileTypePair[STREAM] = Ogre::StringConverter::toString(stream);
		
		Sound *newSound = static_cast<Sound*>(mSoundFactory->createInstance(name, NULL, &fileTypePair));
		mSoundMap[name] = newSound;
		return newSound;
	}

	Sound* SoundManager::getSound(const Ogre::String& name) const
	{
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

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
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		SoundMap::const_iterator soundItr = mSoundMap.find(name);
		if(soundItr != mSoundMap.end())
			return true;
		else
			return false;
	}

	void SoundManager::destroySound(Sound *sound)
	{
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		destroySound(sound->getName());
	}

	void SoundManager::destroySound(const Ogre::String& name)
	{
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		SoundMap::iterator soundItr = mSoundMap.find(name);
		if(soundItr != mSoundMap.end())
		{			
			mSoundFactory->destroyInstance(soundItr->second);
			mSoundMap.erase(soundItr);
		}
	}

	void SoundManager::destroyAllSounds()
	{
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		// delete all Sound pointers in the SoundMap
		std::for_each(mSoundMap.begin(), mSoundMap.end(), DeleteSecond());
		mSoundMap.clear();
	}
	
	void SoundManager::pauseAllSounds()
	{
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

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
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

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
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		return Listener::getSingletonPtr();
	}

	struct SoundManager::UpdateSound
	{
		void operator()(std::pair<std::string, Sound*> pair)const
		{
			pair.second->updateSound();
		}
	};

	struct SoundManager::SortLowToHigh
	{
		bool operator()(const Sound *sound1, const Sound *sound2)const
		{
			// First we see if either sound has stopped and not given up its source
			if(sound1->isStopped() && !sound2->isStopped())
				return true;
			else if(sound2->isStopped() && !sound1->isStopped())
				return false;
			else if(sound1->isStopped() && sound2->isStopped())
				return sound1 < sound2;

			// If they are both playing we'll test priority
			if(sound1->getPriority() < sound2->getPriority())
				return true;
			else if (sound1->getPriority() > sound2->getPriority())
				return false;

			// If they are the same priority we'll test against the
			// distance from the listener
			Ogre::Real distSound1, distSound2;
			Ogre::Vector3 listenerPos = Listener::getSingleton().getDerivedPosition();
			if(sound1->isRelativeToListener())
			{
				distSound1 = sound1->getPosition().length();
			}
			else
			{
				distSound1 = sound1->getDerivedPosition().distance(listenerPos);
			}

			if(sound2->isRelativeToListener())
			{
				distSound2 = sound1->getPosition().length();
			}
			else
			{
				distSound2 = sound2->getDerivedPosition().distance(listenerPos);
			}

			if(distSound1 > distSound2)
				return true;
			else if(distSound1 < distSound2)
				return false;

			// If they are at the same priority and distance from the listener then
			// they are both equally well suited to being sacrificed so we compare
			// their pointers since it really doesn't matter
			return sound1 < sound2;
		}
	};

	struct SoundManager::SortHighToLow
	{
		bool operator()(const Sound *sound1, const Sound *sound2)const
		{
			// First we'll test priority
			if(sound1->getPriority() > sound2->getPriority())
				return true;
			else if (sound1->getPriority() < sound2->getPriority())
				return false;

			// If they are the same priority we'll test against the
			// distance from the listener
			Ogre::Real distSound1, distSound2;
			Ogre::Vector3 listenerPos = Listener::getSingleton().getDerivedPosition();
			if(sound1->isRelativeToListener())
			{
				distSound1 = sound1->getPosition().length();
			}
			else
			{
				distSound1 = sound1->getDerivedPosition().distance(listenerPos);
			}

			if(sound2->isRelativeToListener())
			{
				distSound2 = sound1->getPosition().length();
			}
			else
			{
				distSound2 = sound2->getDerivedPosition().distance(listenerPos);
			}

			if(distSound1 < distSound2)
				return true;
			else if(distSound1 > distSound2)
				return false;

			// If they are at the same priority and distance from the listener then
			// they are both equally well suited to stealing a source so we compare
			// their pointers since it really doesn't matter
			return sound1 < sound2;
		}
	};

	bool SoundManager::frameStarted(const Ogre::FrameEvent& evt)
	{
		updateSounds();
		return true;
	}

	void SoundManager::setDopplerFactor(Ogre::Real dopplerFactor)
	{
		// Negative values are invalid
		if(dopplerFactor < 0) return;

		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		mDopplerFactor = dopplerFactor;
		alDopplerFactor(mDopplerFactor);
		CheckError(alGetError(), "Failed to set Doppler Factor");
	}

	void SoundManager::setSpeedOfSound(Ogre::Real speedOfSound)
	{
		// Negative values are invalid
		if(speedOfSound < 0) return;

		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

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
			try
			{
				ALCdevice *device = alcOpenDevice(deviceList);
				CheckError(alcGetError(device), "Unable to open device");

				if(device)
				{
					// Device seems to be valid
					ALCcontext *context = alcCreateContext(device, NULL);
					CheckError(alcGetError(device), "Unable to create context");
					if(context)
					{
						// Context seems to be valid
						alcMakeContextCurrent(context);
						CheckError(alcGetError(device), "Unable to make context current");
						deviceVector.push_back(alcGetString(device, ALC_DEVICE_SPECIFIER));
						alcMakeContextCurrent(NULL);
						CheckError(alcGetError(device), "Unable to clear current context");
						alcDestroyContext(context);
						CheckError(alcGetError(device), "Unable to destroy context");
					}
					alcCloseDevice(device);
				}
			}
			catch(...)
			{
				// Don't die here, we'll just skip this device.
			}

			deviceList += strlen(deviceList) + 1;
		}

		return deviceVector;
	}

	FormatMapIterator SoundManager::getSupportedFormatIterator()
	{
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		return FormatMapIterator(mSupportedFormats.begin(), mSupportedFormats.end());
	}

	const FormatData* SoundManager::retrieveFormatData(AudioFormat format) const
	{
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		FormatMap::const_iterator itr = mSupportedFormats.find(format);
		if(itr != mSupportedFormats.end())
			return itr->second;
		else
			return 0;
	}

	ALboolean SoundManager::eaxSetBufferMode(Size numBuffers, BufferRef *buffers, EAXMode bufferMode)
	{
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

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
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		return mGetXRamMode(buffer, reserved);
	}

	void SoundManager::_removeBufferRef(const Ogre::String& bufferName)
	{
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		mSoundFactory->_removeBufferRef(bufferName);
	}

	void SoundManager::_addBufferRef(const Ogre::String& bufferName, BufferRef buffer)
	{
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		mSoundFactory->_addBufferRef(bufferName, buffer);
	}

	SourceRef SoundManager::_requestSource(Sound *sound)
	{
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		if(sound->getSource() != AL_NONE)
		{
			// This sound already has a source, so we'll just return the same one
			return sound->getSource();
		}

		SoundList::iterator soundItr;
		for(soundItr = mQueuedSounds.begin(); soundItr != mQueuedSounds.end(); soundItr++)
		{
			if((*soundItr) == sound)
			{
				// This sound has already requested a source
				return AL_NONE;
				break;
			}
		}

		if(!mSourcePool.empty())
		{
			mActiveSounds.push_back(sound);
			SourceRef source = mSourcePool.front();
			mSourcePool.pop();
			return source;
		}
		else
		{
			Sound *activeSound = mActiveSounds.front();
			Ogre::Vector3 listenerPos = Listener::getSingleton().getDerivedPosition();
			Ogre::Real distSound = sound->getDerivedPosition().distance(listenerPos);
			Ogre::Real distActiveSound = activeSound->getDerivedPosition().distance(listenerPos);

			if(sound->getPriority() > activeSound->getPriority() ||
				sound->getPriority() == activeSound->getPriority() && distSound < distActiveSound)
			{
				activeSound->pause();
				SourceRef source = activeSound->getSource();
				mActiveSounds.erase(mActiveSounds.begin());
				mQueuedSounds.push_back(activeSound);

				mActiveSounds.push_back(sound);
				return source;
			}
			else
			{
				mQueuedSounds.push_back(sound);
				return AL_NONE;
			}
		}
	}

	SourceRef SoundManager::_releaseSource(Sound *sound)
	{
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		bool soundFound = false;
		SoundList::iterator soundItr;
		for(soundItr = mActiveSounds.begin(); soundItr != mActiveSounds.end(); soundItr++)
		{
			if((*soundItr) == sound)
			{
				mActiveSounds.erase(soundItr);
				soundFound = true;
				break;
			}
		}

		// If it's not in the active list, check the queued list
		if(!soundFound)
		{
			for(soundItr = mQueuedSounds.begin(); soundItr != mQueuedSounds.end(); soundItr++)
			{
				if((*soundItr) == sound)
				{
					mQueuedSounds.erase(soundItr);
					break;
				}
			}
		}

		SourceRef source = sound->getSource();
		if(source != AL_NONE)
		{
			if(!mQueuedSounds.empty())
			{
				Sound *queuedSound = mQueuedSounds.front();
				mQueuedSounds.erase(mQueuedSounds.begin());

				queuedSound->setSource(source);
				queuedSound->play();
				mActiveSounds.push_back(queuedSound);
			}
			else
			{
				mSourcePool.push(source);
			}
		}

		return AL_NONE;
	}

	int SoundManager::createSourcePool()
	{
		SourceRef source;

		int numSources = 0;
		while(alGetError() == AL_NO_ERROR && numSources < 100)
		{
			source = 0;
			alGenSources(1, &source);
			if(source != 0)
			{
				mSourcePool.push(source);
				numSources++;
			}
			else
			{
				// Clear out the error
				alGetError();
				break;
			}
		}

		return numSources;
	}

	void SoundManager::initializeDevice(const Ogre::String& deviceName)
	{
		alcGetIntegerv(NULL, ALC_MAJOR_VERSION, sizeof(mMajorVersion), &mMajorVersion);
		CheckError(alcGetError(NULL), "Failed to retrieve version info");
		alcGetIntegerv(NULL, ALC_MINOR_VERSION, sizeof(mMinorVersion), &mMinorVersion);
		CheckError(alcGetError(NULL), "Failed to retrieve version info");

		Ogre::LogManager::getSingleton().logMessage("OpenAL Version: " +
			Ogre::StringConverter::toString(mMajorVersion) + "." +
			Ogre::StringConverter::toString(mMinorVersion));

		/*
		** OpenAL versions prior to 1.0 DO NOT support device enumeration, so we
		** need to test the current version and decide if we should try to find 
		** an appropriate device or if we should just open the default device.
		*/
		bool deviceInList = false;
		if(mMajorVersion >= 1 && mMinorVersion >= 1)
		{
			Ogre::LogManager::getSingleton().logMessage("Available Devices");
			Ogre::LogManager::getSingleton().logMessage("-----------------");

			// List devices in log and see if the sugested device is in the list
			Ogre::StringVector deviceList = getDeviceList();
			std::stringstream ss;
			Ogre::StringVector::iterator deviceItr;
			for(deviceItr = deviceList.begin(); deviceItr != deviceList.end() && (*deviceItr).compare("") != 0; deviceItr++)
			{
				deviceInList = (*deviceItr).compare(deviceName) == 0;
				ss << " * " << (*deviceItr);
				Ogre::LogManager::getSingleton().logMessage(ss.str());
				ss.clear(); ss.str("");
			}
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

		// Check for EFX Support
		if(alcIsExtensionPresent(mDevice, "ALC_EXT_EFX") == AL_TRUE)
		{
			Ogre::LogManager::getSingleton().logMessage("EFX Extension Found");
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

		// Create our pool of sources
		mMaxNumSources = createSourcePool();
	}

	void SoundManager::updateSounds()
	{
		// Lock Mutex
		OGREAL_LOCK_AUTO_MUTEX

		// Update the Sound and Listeners if necessary	
		std::for_each(mSoundMap.begin(), mSoundMap.end(), UpdateSound());
		Listener::getSingleton().updateListener();

		// Sort the active and queued sounds
		std::sort(mActiveSounds.begin(), mActiveSounds.end(), SortLowToHigh());
		std::sort(mQueuedSounds.begin(), mQueuedSounds.end(), SortHighToLow());

		// Check to see if we should sacrifice any sounds
		updateSourceAllocations();
	}

	void SoundManager::updateSourceAllocations()
	{
		while(!mQueuedSounds.empty())
		{
			Sound *queuedSound = mQueuedSounds.front();
			Sound *activeSound = mActiveSounds.front();

			Ogre::Real distQueuedSound, distActiveSound;
			Ogre::Vector3 listenerPos = Listener::getSingleton().getDerivedPosition();
			if(queuedSound->isRelativeToListener())
			{
				distQueuedSound = queuedSound->getPosition().length();
			}
			else
			{
				distQueuedSound = queuedSound->getDerivedPosition().distance(listenerPos);
			}

			if(activeSound->isRelativeToListener())
			{
				distActiveSound = activeSound->getPosition().length();
			}
			else
			{
				distActiveSound = activeSound->getDerivedPosition().distance(listenerPos);
			}

			if(queuedSound->getPriority() > activeSound->getPriority() ||
				queuedSound->getPriority() == activeSound->getPriority() && distQueuedSound < distActiveSound)
			{
				// Remove the sounds from their respective lists
				mActiveSounds.erase(mActiveSounds.begin());
				mQueuedSounds.erase(mQueuedSounds.begin());

				// Steal the source from the currently active sound
				activeSound->pause();
				activeSound->unqueueBuffers();
				SourceRef source = activeSound->getSource();
				activeSound->setSource(AL_NONE);

				// Kickstart the currently queued sound
				queuedSound->setSource(source);
				queuedSound->play();

				// Add the sound back to the correct lists
				mActiveSounds.push_back(queuedSound);
				mQueuedSounds.push_back(activeSound);
			}
			else
			{
				// We have no more sounds that we can sacrifice
				break;
			}
		}
	}
} // Namespace
