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

#ifndef _OGREAL_SOUND_H_
#define _OGREAL_SOUND_H_

#include "OgreALPrereqs.h"

namespace OgreAL {
	/**
	 * Sound.
	 * @remarks
	 * Sounds are created using the SoundManager::createSound method
	 * and then can be attached to Ogre::SceneNodes, placed at a point
	 * in space or placed at an offset relative to the listener.
	 *
	 * @see Ogre::MovableObject
	 */
	class OgreAL_Export Sound : public Ogre::MovableObject
	{
	protected:
		/*
		** Constructors are protected to enforce the use of the 
		** factory via SoundManager::createSound
		*/
		/** Default Constructor. */
		Sound();
		/** Normal Constructor. Should not be called directly! Use SoundManager::createSound */
		Sound(const Ogre::String& name, const Ogre::String& fileName, bool stream);
		/** Constructor to be called if BufferRef exists */
		Sound(BufferRef buffer, const Ogre::String& name, const Ogre::String& fileName, bool loop = false);

	public:
		/** Standard Destructor */
		virtual ~Sound();

		enum Priority
		{
			LOW,
			NORMAL,
			HIGH
		};

		/** Plays the sound. */
		bool play();
		/** Returns true if the sound is playing, otherwise false. */
		virtual bool isPlaying() const;
		/** Pauses the sound. @note Upon calling play again, the sound will resume where it left off */
		virtual bool pause();
		/** Returns true if the sound is paused, otherwise false. */
		virtual bool isPaused() const;
		/** Stops the sound. @note Upon calling play again, the sound will resume from the begining */
		bool stop();
		/** Returns true if the sound is stopped, otherwise false. */
		virtual bool isStopped() const;
		/** Returns true if the source does not have a state yet, otherwise false */
		virtual bool isInitial() const;
		/** Starts playing the song while fading in.*/
		bool fadeIn(Ogre::Real fadeTime);
		/** Fades out, but keeps playing at volume 0, so it can be faded in again.*/
		bool fadeOut(Ogre::Real fadeTime);

		/**
		 * Sets the pitch multiplier.
		 * @param pitch The new pitch multiplier
		 * @note pitch must always be positive non-zero, all other values will be ignored
		 */
		void setPitch(Ogre::Real pitch);
		/** Returns the pitch multiplier. */
		Ogre::Real getPitch() const {return mPitch;}
		/** 
		 * Sets the gain. 
		 * @param gain The gain where 1.0 is full volume and 0.0 is off
		 * @note Negative values will be ignored
		 */
		void setGain(Ogre::Real gain);
		/** Returns the gain. */
		Ogre::Real getGain() const {return mGain;}
		/** 
		 * Sets the max gain. 
		 * @param maxGain The maximum amount of gain allowed for this source
		 * @note Valid range is [0.0 - 1.0] all other values will be ignored
		 */
		void setMaxGain(Ogre::Real maxGain);
		/** Returns the max gain. */
		Ogre::Real getMaxGain() const {return mMaxGain;}
		/** 
		 * Sets the min gain. 
		 * @param minGain The minimum amount of gain allowed for this source.
		 * @note Valid range is [0.0 - 1.0] all other values will be ignored
		 */
		void setMinGain(Ogre::Real minGain);
		/** Returns the gain. */
		Ogre::Real getMinGain() const {return mMinGain;}
		/**
		 * Set the min max and current gain.
		 * @param maxGain The maximum amount of gain allowed for this source
		 * @param minGain The minimum amount of gain allowed for this source
		 * @param gain The current gain for this source
		 * @note Gain should be positive
		 */
		void setGainValues(Ogre::Real maxGain, Ogre::Real minGain, Ogre::Real gain);
		/**
		 * Sets the Max Distance.
		 * @param maxDistance The max distance used in the Inverse Clamped Distance Model
		 * @note This is the distance where there will be no further attenuation of the source
		 * @note Negative values will be ignored
		 */
		void setMaxDistance(Ogre::Real maxDistance);
		/** Returns the max distance */
		Ogre::Real getMaxDistance() const {return mMaxDistance;}
		/**
		 * Sets the rolloff factor.
		 * @param rolloffFactor The rolloff rate for the source 
		 * @note Used for distance attenuation
		 * @note Negative values will be ignored
		 */
		void setRolloffFactor(Ogre::Real rolloffFactor);
		/** Returns the rolloff factor */
		Ogre::Real getRolloffFactor() const {return mRolloffFactor;}
		/**
		 * Sets the reference distance.
		 * @param refDistance The reference distance used in attenuation calculations.
		 * @note This is the distance under which the volume for the
		 *     source would normally drop by half
		 * @note Negative values will be ignored
		 */
		void setReferenceDistance(Ogre::Real refDistance);
		/** Returns the reference distance. */
		Ogre::Real getReferenceDistance() const {return mReferenceDistance;}
		/**
		 * Sets the variables used in the distance attenuation calculation.
		 * @param maxDistance The max distance used in the Inverse Clamped Distance Model
		 * @param rolloffFactor The rolloff rate for the source
		 * @param refDistance The reference distance used in attenuation calculations
		 */
		void setDistanceValues(Ogre::Real maxDistance, Ogre::Real rolloffFactor, Ogre::Real refDistance);
		/**
		 * Sets the velocity of the sound.
		 * @param x The x part of the velocity vector
		 * @param y The y part of the velocity vector
		 * @param z The z part of the velocity vector
		 */
		void setVelocity(Ogre::Real x, Ogre::Real y, Ogre::Real z);
		/** Sets the velocity of the sound. @param vec The velocity vector */
		void setVelocity(const Ogre::Vector3& vec);
		/** Returns the velocity of the sound. */
		const Ogre::Vector3& getVelocity() const {return mVelocity;}
		/** 
		 * Sets the source relation to the listner.
		 * @param relative Whether the sound position is
		 *    relative to the listener or not.
		 * @note If this is set to true then the position of the
		 * sound will be relative to the listener.  However,
		 * setting this to true will have no effect if the sound
		 * is connected to a scene node.
		 */
		void setRelativeToListener(bool relative);
		/** Returns true if the sound is relative to the listener, otherwise false. */
		bool isRelativeToListener() const {return mSourceRelative==AL_TRUE?true:false;}
		/**
		 * Sets the position of the sound.
		 * @param x The x part of the position
		 * @param y The y part of the position
		 * @param z The z part of the position
		 * @note In the case that this sound is attached to a SceneNode this
		 *     position become the offset from the parent scene node
		 */
		void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);
		/**
		 * Sets the position of the sound.
		 * @param vec The new postion for the sound.
		 * @note In the case that this sound is attached to a SceneNode this
		 *     position become the offset from the parent scene node
		 */
		void setPosition(const Ogre::Vector3& vec);
		/** Returns the position of the sound. */
		const Ogre::Vector3& getPosition() const {return mPosition;}
		/**
		 * Sets the direction of the sound.
		 * @param x The x part of the direction vector
		 * @param y The y part of the direction vector
		 * @param z The z part of the direction vector
		 * @note In the case that this sound is attached to a SceneNode this
		 *     directions becomes relative to the parent's direction
		 */
		void setDirection(Ogre::Real x, Ogre::Real y, Ogre::Real z);
		/**
		 * Sets the direction of the sound.
		 * @param vec The direction vector.
		 * @note In the case that this sound is attached to a SceneNode this
		 *     directions becomes relative to the parent's direction
		 */
		void setDirection(const Ogre::Vector3& vec);
		/** Returns the direction of the sound. */
		const Ogre::Vector3& getDirection() const {return mDirection;}
		/**
		 * Sets the gain outside the sound cone of a directional sound.
		 * @param outerConeGain The gain outside the directional cone
		 * @note Each directional source has three zones:<ol><li>The inner zone as defined by the 
		 *     <i>setInnerConeAngle</i> where the gain is constant and is set by <i>setGain</i></li>
		 *     <li>The outer zone which is set by <i>setOuterConeAngle</i> and the gain is a linear 
		 *     transition between the gain and the outerConeGain</li><li>Outside of the sound cone.  
		 *     The gain in this zone is set by <i>setOuterConeGain</i></li></ol>
		 * @note Valid range is [0.0 - 1.0] all other values will be ignored
		 */
		void setOuterConeGain(Ogre::Real outerConeGain);
		/** Returns the outerConeGain */
		Ogre::Real getOuterConeGain() const {return mOuterConeGain;}
		/**
		 * Sets the inner angle of the sound cone for a directional sound.
		 * @param innerConeAngle The angle that defines the inner cone of a directional sound.  Valid values are [0 - 360]
		 * @note Each directional source has three zones:<ol><li>The inner zone as defined by the 
		 *     <i>setInnerConeAngle</i> where the gain is constant and is set by <i>setGain</i></li>
		 *     <li>The outer zone which is set by <i>setOuterConeAngle</i> and the gain is a linear 
		 *     transition between the gain and the outerConeGain</li><li>Outside of the sound cone.  
		 *     The gain in this zone is set by <i>setOuterConeGain</i></li></ol>
		 */
		void setInnerConeAngle(Ogre::Real innerConeAngle);
		/** Return the innerConeAngle */
		Ogre::Real getInnerConeAngle() const {return mInnerConeAngle;}
		/**
		 * Sets the outer angle of the sound cone for a directional sound.
		 * @param outerConeAngle The angle that defines the outer cone of a directional sound.  Valid values are [0 - 360]
		 * @note Each directional source has three zones:<ol><li>The inner zone as defined by the 
		 *     <i>setInnerConeAngle</i> where the gain is constant and is set by <i>setGain</i></li>
		 *     <li>The outer zone which is set by <i>setOuterConeAngle</i> and the gain is a linear 
		 *     transition between the gain and the outerConeGain</li><li>Outside of the sound cone.  
		 *     The gain in this zone is set by <i>setOuterConeGain</i></li></ol>
		 */
		void setOuterConeAngle(Ogre::Real outerConeAngle);
		/** Returns the outerConeAngle */
		Ogre::Real getOuterConeAngle() {return mOuterConeAngle;}
		/** Sets the looping state */
		virtual void setLoop(bool loop);
		/** Returns looping state */
		bool isLooping() const {return mLoop==AL_TRUE?true:false;}
		/** Returns streaming state */
		bool isStreaming() const {return mStream==AL_TRUE?true:false;}
		/** Sets the priority of the sound */
		void setPriority(Priority priority) {mPriority = priority;}
		/** Returns the current Priority for the sound */
		Priority getPriority() const {return mPriority;}
		/** Returns the duration of the audio in seconds */
		Ogre::Real getSecondDuration() {return mLengthInSeconds;}
		/** Sets the offset within the audio stream in seconds. @note Negative values will be ignored */
		virtual void setSecondOffset(Ogre::Real seconds);
		/** Returns the current offset within the audio stream in seconds */
		virtual Ogre::Real getSecondOffset();
		/** Returns the position of the sound including any transform from nodes it is attached to. */
		const Ogre::Vector3& getDerivedPosition() const {return mDerivedPosition;}
		/** Returns the direction of the sound including any transform from nodes it is attached to. */
		const Ogre::Vector3& getDerivedDirection() const {return mDerivedDirection;}
		/** Returns the name of the file used to create this Sound. */
		const Ogre::String& getFileName() const {return mFileName;}

		/** Overridden from MovableObject */
		const Ogre::String& getMovableType() const;
		/** Overridden from MovableObject */
		const Ogre::AxisAlignedBox& getBoundingBox() const;
		/** Overridden from MovableObject */
		Ogre::Real getBoundingRadius() const {return 0; /* Not Visible */} 
		/** Overridden from MovableObject */
		void _updateRenderQueue(Ogre::RenderQueue* queue);
		/** Notifies the sound when it is attached to a node */
		void _notifyAttached(Ogre::Node *parent, bool isTagPoint = false);
	#if(OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR == 5)
		/** Overridden from MovableObject */
		virtual void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false){}
	#endif

	protected:
		/// Updates the sound if need be
		virtual bool updateSound();
		/// Sets the source for the sound
		void setSource(SourceRef source) {mSource = source;}
		/// Returns the SourceRef
		SourceRef getSource() {return mSource;}
		/// Convienance method to reset the sound state
		void initSource();
		/// Convienance method for creating buffers
		void generateBuffers();
		/// Calculates format info for the sounds. @note requires mFreq, mChannels and mBPS;
		void calculateFormatInfo();
		/// Queues buffers to be played
		void queueBuffers();
		/// Empties any queues that may still be active in the sound
		void unqueueBuffers();
		/// Loads the buffers to be played.  Returns whether the buffer is loaded.
		virtual bool loadBuffers() {return true;}
		/// Unloads the buffers.  Returns true if the buffers are still loaded.
		virtual bool unloadBuffers() {return true;}

		/// Returns the BufferRef for this sounds
		BufferRef getBufferRef() const {return mBuffers[0];}

		/// Postion taking into account the parent node
		mutable Ogre::Vector3 mDerivedPosition;
		/// Direction taking into account the parent node
		mutable Ogre::Vector3 mDerivedDirection;
		/// Stored versions of parent orientation
		mutable Ogre::Quaternion mLastParentOrientation;
		/// Stored versions of parent position
		mutable Ogre::Vector3 mLastParentPosition;

		Ogre::Real mPitch;
		Ogre::Real mGain;
		Ogre::Real mMaxGain;
		Ogre::Real mMinGain;
		Ogre::Real mMaxDistance;
		Ogre::Real mRolloffFactor;
		Ogre::Real mReferenceDistance;
		Ogre::Real mOuterConeGain;
		Ogre::Real mInnerConeAngle;
		Ogre::Real mOuterConeAngle;
		Ogre::Vector3 mPosition;
		Ogre::Vector3 mVelocity;
		Ogre::Vector3 mDirection;
		Ogre::String mFileName;
		ALboolean mSourceRelative;
		ALboolean mLoop;
		mutable bool mLocalTransformDirty;

		Ogre::DataStreamPtr mSoundStream;

		SourceRef mSource;
		BufferRef *mBuffers;
		int mNumBuffers;
		int mBufferSize;
		bool mStream;
		bool mBuffersLoaded;
		bool mBuffersQueued;
		Size mFreq;
		Size mSize;
		Size mChannels;
		Size mBPS;
		BufferFormat mFormat;

		Priority mPriority;

		// This allows us to start a sound back where is should be after being sacrificed
		time_t mStartTime;

		Ogre::Real mLengthInSeconds;
		
		friend class SoundManager;
		friend class SoundFactory;

	private:
		void _update() const;
		void _updateFading();

		enum FadeMode
		{
			FADE_NONE,
			FADE_IN,
			FADE_OUT
		};

		FadeMode    mFadeMode;
		Ogre::Real  mFadeTime;
		Ogre::Real  mRunning;
	};

	/** Factory object for creating sounds */
	class OgreAL_Export SoundFactory : public Ogre::MovableObjectFactory
	{
	public:
		SoundFactory();
		~SoundFactory();

		static Ogre::String FACTORY_TYPE_NAME;

		const Ogre::String& getType() const;
		void destroyInstance(Ogre::MovableObject* obj);

		void _removeBufferRef(const Ogre::String& bufferName);
		void _addBufferRef(const Ogre::String& bufferName, BufferRef buffer);

	protected:
		typedef std::map<std::string, BufferRef> BufferMap;
		BufferMap mBufferMap;

		Ogre::MovableObject* createInstanceImpl(const Ogre::String& name, const Ogre::NameValuePairList* params = 0);
	};
} // Namespace
#endif
