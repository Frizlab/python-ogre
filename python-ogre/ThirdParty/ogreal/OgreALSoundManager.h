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

#ifndef _OGREAL_SOUND_MANAGER_H_
#define _OGREAL_SOUND_MANAGER_H_

#include <algorithm>
#include <map>
#include <vector>

#include "OgreException.h"
#include "OgreIteratorWrappers.h"
#include "OgreString.h"

#include "OgreALListener.h"
#include "OgreALPrereqs.h"
#include "OgreALSound.h"

#if OGREAL_THREADED
#	include <boost/thread/thread.hpp>
#	include <boost/thread/recursive_mutex.hpp>
#	include <boost/thread/xtime.hpp>
#endif

namespace OgreAL {
	/**
	 * FormatData.
	 * @remark This is a storage class to hold data about different
	 * OpenAL buffer formats.
	 */
	class OgreAL_Export FormatData
	{
	public:
		FormatData(BufferFormat alEnum, ALString alChar, const Ogre::String& description)
		{
			formatEnum = alEnum;
			formatName = alChar;
			formatDescription = description;
		}
		/// OpenAL buffer type
		BufferFormat formatEnum;
		/// OpenAL buffer type name
		ALString formatName;
		/// Buffer type description
		Ogre::String formatDescription;
	};

	typedef std::map<Ogre::String, Sound*> SoundMap;
	typedef std::vector<Sound*> SoundList;
	typedef std::map<AudioFormat, FormatData*> FormatMap;
	typedef Ogre::MapIterator<FormatMap> FormatMapIterator;

	/**
	 * SoundManager.
	 * @remark This class is responsible for creating and managing all of the
	 *     sounds and listeners used in the application.  Sounds  and Listeners
	 *	   should only ever be created using the creation methods provided in this class.
	 *
	 * @see Ogre::Singleton
	 * @see Ogre::FrameListener
	 */
	class OgreAL_Export SoundManager : public Ogre::Singleton<SoundManager>, Ogre::FrameListener
	{
	public:
		/**
		 * Constructor.
		 * @param deviceName An optional parameter that allows the user to suggest which device to use.
		 *		The list of valid devices can be obtained by calling SoundManager::getDeviceList()
		 */
		SoundManager(const Ogre::String& deviceName = "");
		/** Standard Destructor. */
		virtual ~SoundManager();
		/** Returns the SoundManager singleton object */
		static SoundManager& getSingleton();
		/** Returns a pointer to the SoundManager singleton object */
		static SoundManager* getSingletonPtr();
		/**
		 * Creates a sound.  This is the only way sounds should be instantiated
		 * @param name The name used to refer to the sound
		 * @param fileName The name of the sound file to load
		 * @param loop Should the sound be looped once it has finished playing
		 * @param stream Should the sound be streamed or should the whole file be loaded into memory at once
		 * @return Returns a pointer to the newly created sound
		 */
		virtual Sound* createSound(const Ogre::String& name, const Ogre::String& fileName, bool loop = false, bool stream = false);
		/** Returns the requested sound object. @param name The name of the sound to return */
		virtual Sound* getSound(const Ogre::String& name) const;
		/** Returns true if the specified sound is loaded in the SoundManager. @param name The name of the sound to check for */
		virtual bool hasSound(const Ogre::String& name) const;
		/** Destroys the specified sound. @param name The name of the sound to destroy */
		virtual void destroySound(const Ogre::String& name);
		/** Destroys the specified sound. @param sound A pointer to the sound to destroy */
		virtual void destroySound(Sound* sound);
		/** Destroys all sounds loaded in the SoundManager */
		virtual void destroyAllSounds();
		/** Pauses all sounds that are currently playing. */
		virtual void pauseAllSounds();
		/** Resumes all sounds that were paused with the previous call to pauseAllSounds(). */
		virtual void resumeAllSounds();
		/** This is how you should get a pointer to the listener object. */
		virtual Listener* getListener() const;
		/**
		 * Sets the Doppler factor.
		 * The Doppler factor is a simple scaling factor of the source and listener
		 * velocities to exaggerate or deemphasize the Doppler (pitch) shift resulting
		 * from the Doppler Effect Calculations.
		 * @note Negative values will be ignored.
		 */
		void setDopplerFactor(Ogre::Real dopplerFactor);
		/** Returns the Doppler factor */
		Ogre::Real getDopplerFactor() const {return mDopplerFactor;}
		/** 
		 * Sets the speed of sound used in the Doppler calculations.
		 * This sets the propagation speed used in the Doppler calculations.
		 * The default is 343.3 m/s (Speed of sound through air), but the
		 * units can be assigned to anything that you want as long as the
		 * velocities of the Sounds and Listener are expressed in the same units.
		 * @note Negative values will be ignored.
		 */
		void setSpeedOfSound(Ogre::Real speedOfSound);
		/** Returns the speed of sound */
		Ogre::Real getSpeedOfSound() const {return mSpeedOfSound;}
		/** 
		 * Returns a list of all posible sound devices on the system.
		 * @note Versions of OpenAL prior to 1.0 do not support this feature
		 */
		static Ogre::StringVector getDeviceList();
		/** Returns an iterator for the list of supported buffer formats */
		FormatMapIterator getSupportedFormatIterator();
		/**
		 * Returns the FormatData containing information for the specified buffer format
		 * or NULL if the specified format is not found.
		 */
		const FormatData* retrieveFormatData(AudioFormat format) const;
		/** Returns the maximum number of sources allowed by the hardware */
		int maxSources() const {return mMaxNumSources;}
		/** 
		 * Returns the version of EAX that is supported
		 * @note There is nothing implemented that uses EAX yet, I am just planning ahead.
		 */
		int eaxSupport() const {return mEAXVersion;}
		/** Returns true if X-Ram is supported, otherwise false */
		bool xRamSupport() const {return mXRAMSupport;}
		/** Sets the mode to be used by sound buffers if X-Ram is supported */
		ALboolean eaxSetBufferMode(Size numBuffers, BufferRef *buffers, EAXMode bufferMode);
		/** Returns the eaxBufferMode if X-Ram is supported */
		ALenum eaxGetBufferMode(BufferRef buffer, ALint *reserved = 0);
		/** Removes a BufferRef from the BufferMap */
		void _removeBufferRef(const Ogre::String& bufferName);
		/** Adds a BufferRef to the BufferMap to be used later */
		void _addBufferRef(const Ogre::String& bufferName, BufferRef buffer);
		/** Used by the fading: returns the time since last frame. */
		Ogre::Real _getLastDeltaTime() const {return mLastDeltaTime;}
		/** Requests a dynamically allocated Source. */
		SourceRef _requestSource(Sound *sound);
		/** Releases a dynamically allocated Source. */
		SourceRef _releaseSource(Sound *sound);

		static const Ogre::String FILE_TYPE;
		static const Ogre::String OGG;
		static const Ogre::String WAV;
		static const Ogre::String SOUND_FILE;
		static const Ogre::String LOOP_STATE;
		static const Ogre::String STREAM;
		static const Ogre::String AUDIO_FORMAT;

		static const BufferFormat xRamAuto;
		static const BufferFormat xRamHardware;
		static const BufferFormat xRamAccessible;

	protected:
		/// Frame Update callback
		bool frameStarted(const Ogre::FrameEvent& evt);
		/// Translate the OpenAL error code to a string
		const Ogre::String errorToString(int code) const;
		
		SoundFactory *mSoundFactory;
		ListenerFactory *mListenerFactory;

		bool mEAXSupport;
		int mEAXVersion;
		bool mXRAMSupport;
		bool mEFXSupport;
		int mSendsPerSource;

		Ogre::Real mDopplerFactor;
		Ogre::Real mSpeedOfSound;

	private:
		int createSourcePool();
		void createListener();
		void initializeDevice(const Ogre::String& deviceName);
		void checkFeatureSupport();
		void updateSounds();
		void updateSourceAllocations();

		struct UpdateSound;
		struct SortLowToHigh;
		struct SortHighToLow;

		int mMaxNumSources;
		FormatMap mSupportedFormats;

		int mMajorVersion;
		int mMinorVersion;

		SoundMap mSoundMap;
		SoundList mPauseResumeAll;
		SoundList mSoundsToDestroy;

		SourcePool mSourcePool;
		SoundList mActiveSounds;
		SoundList mQueuedSounds;

		ALCcontext *mContext;
		ALCdevice *mDevice;

		Ogre::Real mLastDeltaTime;

		// Mutex so we can protect against corruption
		OGREAL_AUTO_MUTEX

		#if OGREAL_THREADED
			static boost::thread *mOgreALThread;
			static bool mShuttingDown;

			static void threadUpdate()
			{
				while(!mShuttingDown)
				{
					SoundManager::getSingleton().updateSounds();
					mOgreALThread->yield();
				}
			}
		#endif

		// XRAM Funtions Pointers
		Size mXRamSize;
		Size mXRamFree;

		EAXSetBufferMode mSetXRamMode;
		EAXGetBufferMode mGetXRamMode;
	};
} // Namespace
#endif
