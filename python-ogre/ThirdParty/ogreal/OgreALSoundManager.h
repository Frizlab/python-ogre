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

#ifndef _OGRE_AL_SOUND_MANAGER_H_
#define _OGRE_AL_SOUND_MANAGER_H_

#include <algorithm>
#include <bitset>
#include <map>
#include <vector>

#include "OgreException.h"
#include "OgreIteratorWrappers.h"
#include "OgreString.h"

#include "OgreALListener.h"
#include "OgreALOggSound.h"
#include "OgreALOggSoundStream.h"
#include "OgreALPrereqs.h"
#include "OgreALSound.h"
#include "OgreALWavSound.h"

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
	 *     sounds, sound streams and listeners used in the application.  Sounds 
	 *	   SoundStreams and Listeners should only ever be created using the 
	 *     creation methods provided in this class.
	 *
	 * @see Ogre::Singleton
	 * @see Ogre::FrameListener
	 */
	class OgreAL_Export SoundManager : public Ogre::Singleton<SoundManager>, Ogre::FrameListener
	{
	public:
		/**
		 * Constructor.
		 * @param sceneManager A pointer to the Ogre::SceneManager being used
		 */
		SoundManager();
		/** Standard Destructor. */
		virtual ~SoundManager();
		/** Returns the Listener singleton object */
		static SoundManager& getSingleton();
		/** Returns a pointer to the Listener singleton object */
		static SoundManager* getSingletonPtr();
		/**
		 * Creates a sound.  This is the only way sounds should be instantiated
		 * @param name The name used to refer to the sound
		 * @param fileName The name of the sound file to load
		 * @param loop Should the sound be looped once it has finished playing
		 * @param format This allows you to use a non-default AudioFormat, this is needed in order for sounds that are encoded as multi-channel to play properly
		 * @return Returns a pointer to the newly created sound
		 */
		virtual Sound* createSound(const Ogre::String& name, const Ogre::String& fileName, bool loop = false, AudioFormat format = DEFAULT);
		/**
		 * Creates a sound stream.  This is the only way sound streams should be instantiated
		 * @param name The name used to refer to the sound stream
		 * @param fileName The name of the sound file to load
		 * @param loop Should the sound stream be looped once it has finished playing
		 * @param format This allows you to use a non-default AudioFormat, this is needed in order for sounds that are encoded as multi-channel to play properly
		 * @return Returns a pointer to the newly created sound object
		 */
		virtual Sound* createSoundStream(const Ogre::String& name, const Ogre::String& fileName, bool loop = false, AudioFormat format = DEFAULT);
		/** Returns the requested sound object. @param name The name of the sound to return */
        virtual Sound* getSound(const Ogre::String& name);
		/** Returns true is the specified sound is loaded into the SoundManager. @param name The name of the sound to check for */
		virtual bool hasSound(const Ogre::String& name) const;
		/** Destroys the specified sound. @param name The name of the sound to destroy */
        virtual void destroySound(const Ogre::String& name);
		/** Destroys the specified sound. @param sound A pointer to the sound to destroy */
        virtual void destroySound(Sound* sound);
		/** Destroys all sounds loaded into the SoundManager */
        virtual void destroyAllSounds();
		/** Pauses all sounds that are currently playing. */
		virtual void pauseAllSounds();
		/** Resumes all sounds that were paused with the previous call to pauseAllSounds(). */
		virtual void resumeAllSounds(); 
		/** This should never be called and is included only for completeness */
		virtual Listener* createListener();
		/** This is how you should get a pointer to the listener object. */
		virtual Listener* getListener();
		/** This callback in envoked each frame and serves to update the sounds and the listener */
		bool frameStarted(const Ogre::FrameEvent& evt);
		/**
		 * Sets the Doppler factor.
		 * The Doppler factor is a simple scaling factor that of the source and listener
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
		 * The default is 343.3 m/s, but the units can be assigned to anything
		 * that you want as long as the velocities of the Sounds and
		 * Listener are expressed in the same units.
		 */
		void setSpeedOfSound(Ogre::Real speedOfSound);
		/** Returns the speed of sound */
		Ogre::Real getSpeedOfSound() const {return mSpeedOfSound;}
		/** Returns an iterator for the list of supported buffer formats */
		FormatMapIterator getSupportedFormatIterator();
		/**
		 * Returns the FormatData containing information for the specified buffer format
		 * or NULL if the specified format is not found.
		 */
		const FormatData* retrieveFormatData(AudioFormat format);
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

		static const Ogre::String FILE_TYPE;
		static const Ogre::String OGG;
		static const Ogre::String WAV;
		static const Ogre::String SOUND_FILE;
		static const Ogre::String LOOP_STATE;
		static const Ogre::String OUTPUT_TYPE;
		static const Ogre::String SOUND;
		static const Ogre::String STREAM;
		static const Ogre::String AUDIO_FORMAT;

		static const BufferFormat xRamAuto;
		static const BufferFormat xRamHardware;
		static const BufferFormat xRamAccessible;

	protected:
		/// Translate the OpenAL error code to a string
		const Ogre::String errorToString(int code) const;
		/// Checks for OpenAL errors
		void checkError(const Ogre::String& reference) const;
		/// Enumerates the valid OpenAL devices
		void alDeviceList(const ALCchar *devices) const;
		
		SoundFactory *mSoundFactory;
		ListenerFactory *mListenerFactory;
		Ogre::NameValuePairList mFileTypePair;
		Ogre::ResourceGroupManager *mResourceGroupManager;

		bool mEAXSupport;
		int mEAXVersion;
		bool mXRAMSupport;
		Ogre::Real mDopplerFactor;
		Ogre::Real mSpeedOfSound;

	private:
		Sound* _createSound(const Ogre::String& name, const Ogre::String& fileName, bool loop);
		int _getMaxSources();
		struct UpdateSound;

		int mMaxNumSources;
		FormatMap mSupportedFormats;

		SoundMap mSoundMap;
		SoundList mPauseResumeAll;

		Size mXRamSize;
		Size mXRamFree;

		EAXSetBufferMode mSetXRamMode;
		EAXGetBufferMode mGetXRamMode;
	};
} // Namespace
#endif
