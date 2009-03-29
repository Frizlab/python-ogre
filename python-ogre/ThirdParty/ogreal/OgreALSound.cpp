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
#include "OgreALSound.h"
#include "OgreALOggSound.h"
#include "OgreALWavSound.h"
#include "OgreALSoundManager.h"

namespace OgreAL {
	Sound::Sound() :
		mSource(0),
		mFormat(0),
		mFreq(0),
		mSize(0),
		mBPS(0),
		mChannels(0),
		mLengthInSeconds(0),
		mManualRestart(false),
		mManualStop(false),
		mPreviousOffset(0),
		mBuffers(0),
		mBufferSize(0),
		mBuffersLoaded(false),
		mBuffersQueued(false),
		mPriority(NORMAL),
		mFinishedCallback(0),
		mLoopedCallback(0),
		mStartTime(0),
		mStream(false),
		mPitch(1.0),
		mGain(1.0),
		mMaxGain(1.0),
		mMinGain(0.0),
		mFadeMode(FADE_NONE),
		mFadeTime(0.0),
		mMaxDistance(3400.0),
		mRolloffFactor(1.0),
		mReferenceDistance(150.0),
		mOuterConeGain(0.0),
		mInnerConeAngle(360.0),
		mOuterConeAngle(360.0),
		mPosition(Ogre::Vector3::ZERO),
		mVelocity(Ogre::Vector3::ZERO),
		mDirection(Ogre::Vector3::NEGATIVE_UNIT_Z),
		mFileName(""),
		mSourceRelative(AL_FALSE),
		mDerivedPosition(Ogre::Vector3::ZERO),
		mDerivedDirection(Ogre::Vector3::NEGATIVE_UNIT_Z)
	{
		mParentNode = NULL;
	}

	Sound::Sound(const Ogre::String &name, const Ogre::String& fileName, bool stream) :
		mSource(0),
        mFormat(0),
		mFreq(0),
		mSize(0),
		mBPS(0),
		mChannels(0),
		mLengthInSeconds(0),
		mManualRestart(false),
		mManualStop(false),
		mPreviousOffset(0),
		mBuffers(0),
		mBufferSize(0),
		mBuffersLoaded(false),
		mBuffersQueued(false),
		mNumBuffers(stream?2:1),
		mPriority(NORMAL),
		mFinishedCallback(0),
		mLoopedCallback(0),
		mStartTime(0),
		mStream(stream),
		MovableObject(name),
		mPitch(1.0), mGain(1.0),
		mMaxGain(1.0),
		mMinGain(0.0),
		mFadeMode(FADE_NONE),
		mFadeTime(0.0),
		mMaxDistance(3400.0),
		mRolloffFactor(1.0),
		mReferenceDistance(150.0),
		mOuterConeGain(0.0),
		mInnerConeAngle(360.0),
		mOuterConeAngle(360.0),
		mPosition(Ogre::Vector3::ZERO),
		mVelocity(Ogre::Vector3::ZERO),
		mDirection(Ogre::Vector3::NEGATIVE_UNIT_Z),
		mFileName(fileName),
		mSourceRelative(AL_FALSE),
		mDerivedPosition(Ogre::Vector3::ZERO),
		mDerivedDirection(Ogre::Vector3::NEGATIVE_UNIT_Z)
	{
		mParentNode = NULL;
	}


	Sound::Sound(BufferRef buffer, const Ogre::String& name, const Ogre::String& fileName, bool loop) :
		mSource(0),
		mFormat(0),
		mFreq(0),
		mSize(0),
		mBPS(0),
		mChannels(0),
		mLengthInSeconds(0),
		mManualRestart(false),
		mManualStop(false),
		mPreviousOffset(0),
		mBufferSize(0),
		mBuffersLoaded(true),
		mBuffersQueued(false),
		mNumBuffers(1),
		mPriority(NORMAL),
		mFinishedCallback(0),
		mLoopedCallback(0),
		mStartTime(0),
		mStream(false),
		MovableObject(name),
		mLoop(loop?AL_TRUE:AL_FALSE),
		mPitch(1.0), mGain(1.0),
		mMaxGain(1.0), mMinGain(0.0),
		mFadeMode(FADE_NONE),
		mFadeTime(0.0),
		mMaxDistance(3400.0),
		mRolloffFactor(1.0),
		mReferenceDistance(150.0),
		mOuterConeGain(0.0),
		mInnerConeAngle(360.0),
		mOuterConeAngle(360.0),
		mPosition(Ogre::Vector3::ZERO),
		mVelocity(Ogre::Vector3::ZERO),
		mDirection(Ogre::Vector3::NEGATIVE_UNIT_Z),
		mFileName(fileName),
		mSourceRelative(AL_FALSE),
		mDerivedPosition(Ogre::Vector3::ZERO),
		mDerivedDirection(Ogre::Vector3::NEGATIVE_UNIT_Z)
	{
		mParentNode = NULL;

		mBuffers = new BufferRef[1];
		mBuffers[0] = buffer;

		alGetBufferi(mBuffers[0], AL_FREQUENCY, &mFreq);
		alGetBufferi(mBuffers[0], AL_BITS, &mBPS);
		alGetBufferi(mBuffers[0], AL_CHANNELS, &mChannels);
		alGetBufferi(mBuffers[0], AL_SIZE, &mSize);

		calculateFormatInfo();

		// mBufferSize is equal to 1/4 of a second of audio
		mLengthInSeconds = (float)mSize / (float)(mBufferSize * 4);
	}

	Sound::~Sound()
	{
		stop();

		try
		{
		  // If we have buffers to deallocate, do so.
		  if(mBuffers != NULL) {
		    alDeleteBuffers(mNumBuffers, mBuffers);
		    CheckError(alGetError(), "Failed to delete Buffers, must still be in use.");
		    SoundManager::getSingleton()._removeBufferRef(mFileName);
		  }
		}
		catch(...)
		{
			// Don't die because of this.
		}

		if(mSource != AL_NONE)
		{
			alSourcei(mSource, AL_BUFFER, 0);
			CheckError(alGetError(), "Failed to release buffer");
		}

		delete[] mBuffers;

		SoundManager::getSingleton()._releaseSource(this);
	}

	bool Sound::play()
	{
		if(isPlaying()) return true;

		if(!isPaused())
		{
			mManualRestart = false;
			mManualStop = false;		
			mPreviousOffset = 0;
		}

		if(mStartTime == 0)
		{
			time(&mStartTime);
		}
		else if(!isPaused())
		{
			time_t currentTime;
			time(&currentTime);

			setSecondOffset(currentTime - mStartTime);
		}

		if(mSource != AL_NONE || (mSource = SoundManager::getSingleton()._requestSource(this)) != AL_NONE)
		{
			if(!mBuffersLoaded)
			{
				mBuffersLoaded = loadBuffers();
			}

			if(!mBuffersQueued)
			{
				// Unqueue any buffers that may be left over
				unqueueBuffers();
				queueBuffers();
			}

			initSource();

			alSourcePlay(mSource);
			CheckError(alGetError(), "Failed to play sound");
		}

		return false;
	}

	bool Sound::isPlaying() const
	{
		if(mSource == AL_NONE)
		{
			return false;
		}

		State state;
		alGetSourcei(mSource, AL_SOURCE_STATE, &state);
		CheckError(alGetError(), "Failed to get State");

		return (state == AL_PLAYING);
	}

	bool Sound::pause()
	{
		if(!isPlaying()) return true;

		if(mSource != AL_NONE)
		{
			alSourcePause(mSource);
			CheckError(alGetError(), "Failed to pause sound");
		}

		return false;
	}

	bool Sound::isPaused() const
	{
		if(mSource == AL_NONE)
		{
			return false;
		}

		State state;    
		alGetSourcei(mSource, AL_SOURCE_STATE, &state);
		CheckError(alGetError(), "Failed to get State");

		return (state == AL_PAUSED);
	}

	bool Sound::stop()
	{
		if(isStopped())
		{
			if(mSource != AL_NONE)
			{
				mSource = SoundManager::getSingleton()._releaseSource(this);
				return true;
			}
		}
		else if(mSource != AL_NONE)
		{
			mManualStop = true;

			// Stop the source
			alSourceStop(mSource);
			CheckError(alGetError(), "Failed to stop sound");

			unqueueBuffers();

			mBuffersLoaded = unloadBuffers();
		}

		mSource = SoundManager::getSingleton()._releaseSource(this);

		mStartTime = 0;

		return true;
	}

	bool Sound::isStopped() const
	{
		if(mSource == AL_NONE)
		{
			return true;
		}

		State state;    
		alGetSourcei(mSource, AL_SOURCE_STATE, &state);
		CheckError(alGetError(), "Failed to get State");

		return (state == AL_STOPPED);
	}

	bool Sound::isInitial() const
	{
		if(mSource == AL_NONE)
		{
			return true;
		}

		State state;    
		alGetSourcei(mSource, AL_SOURCE_STATE, &state);
		CheckError(alGetError(), "Failed to get State");

		return (state == AL_INITIAL);
	}

	bool Sound::fadeIn(Ogre::Real fadeTime)
	{
		// Don't interrupt a current fade
		if(!isPlaying() && mFadeMode == FADE_NONE)
		{
		  // Parented source restriction removed. I think what Casey was 
		  // intending when blocking off this functionality was that, 
		  // because mono sources attached to nodes are influenced by gain
		  // drops and raises as a result of distance to the listener, the
		  // fade-in and fade-out would not necessarily reach the maximum
		  // gain set on the sound. There are still, however, legitimate
		  // uses for this regardless.
			mFadeMode = FADE_IN;
			mFadeTime = fadeTime;
			mRunning = 0.0;
			// Start at min gain..
			setGain(mMinGain);
			// ..and play
			return play();
		}
		return false;
	}

	bool Sound::fadeOut(Ogre::Real fadeTime)
	{
		// Don't interrupt a current fade
		if(isPlaying() && mFadeMode == FADE_NONE)
		{
		  // Parented source restriction removed. See fadeIn().
			mFadeMode = FADE_OUT;
			mFadeTime = fadeTime;
			mRunning = 0.0;
			return true;
		}
		return false;
	}

	void Sound::setPitch(Ogre::Real pitch)
	{
		if(pitch <= 0) return;

		mPitch = pitch;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_PITCH, mPitch);
			CheckError(alGetError(), "Failed to set Pitch");
		}
	}

	void Sound::setGain(Ogre::Real gain)
	{
		if(gain < 0) return;

		mGain = gain;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_GAIN, mGain);
			CheckError(alGetError(), "Failed to set Gain");
		}
	}

	void Sound::setMaxGain(Ogre::Real maxGain)
	{
		if(maxGain < 0 || maxGain > 1) return;
		
		mMaxGain = maxGain;
		
		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_MAX_GAIN, mMaxGain);
			CheckError(alGetError(), "Failed to set Max Gain");
		}
	}

	void Sound::setMinGain(Ogre::Real minGain)
	{
		if(minGain < 0 || minGain > 1) return;

		mMinGain = minGain;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_MIN_GAIN, mMinGain);
			CheckError(alGetError(), "Failed to set Min Gain");
		}
	}

	void Sound::setMaxDistance(Ogre::Real maxDistance)
	{
		if(maxDistance < 0) return;

		mMaxDistance = maxDistance;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_MAX_DISTANCE, mMaxDistance);
			CheckError(alGetError(), "Failed to set Max Distance");
		}
	}

	void Sound::setRolloffFactor(Ogre::Real rolloffFactor)
	{
		if(rolloffFactor < 0) return;

		mRolloffFactor = rolloffFactor;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_ROLLOFF_FACTOR, mRolloffFactor);
			CheckError(alGetError(), "Failed to set Rolloff Factor");
		}
	}

	void Sound::setReferenceDistance(Ogre::Real refDistance)
	{
		if(refDistance < 0) return;

		mReferenceDistance = refDistance;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_REFERENCE_DISTANCE, mReferenceDistance);
			CheckError(alGetError(), "Failed to set Reference Distance");
		}
	}

	void Sound::setDistanceValues(Ogre::Real maxDistance, Ogre::Real rolloffFactor, Ogre::Real refDistance)
	{
		setMaxDistance(maxDistance);
		setRolloffFactor(rolloffFactor);
		setReferenceDistance(refDistance);
	}

	void Sound::setVelocity(Ogre::Real x, Ogre::Real y, Ogre::Real z)
	{
		mVelocity.x = x;
		mVelocity.y = y;
		mVelocity.z = z;

		if(mSource != AL_NONE)
		{
			alSource3f(mSource, AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
			CheckError(alGetError(), "Failed to set Velocity");
		}
	}

	void Sound::setVelocity(const Ogre::Vector3& vec)
	{
		setVelocity(vec.x, vec.y, vec.z);
	}

	void Sound::setRelativeToListener(bool relative)
	{
		// Do not set to relative if it's connected to a node
		if(mParentNode) return;

		mSourceRelative = relative;

		if(mSource != AL_NONE)
		{
			alSourcei(mSource, AL_SOURCE_RELATIVE, mSourceRelative);
			CheckError(alGetError(), "Failed to set Source Relative");
		}
	}

	void Sound::setOuterConeGain(Ogre::Real outerConeGain)
	{
		if(outerConeGain < 0 || outerConeGain > 1) return;

		mOuterConeGain = outerConeGain;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_CONE_OUTER_GAIN, mOuterConeGain);
			CheckError(alGetError(), "Failed to set Outer Cone Gain");
		}
	}

	void Sound::setInnerConeAngle(Ogre::Real innerConeAngle)
	{
		mInnerConeAngle = innerConeAngle;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_CONE_INNER_ANGLE, mInnerConeAngle);
			CheckError(alGetError(), "Failed to set Inner Cone Angle");
		}
	}

	void Sound::setOuterConeAngle(Ogre::Real outerConeAngle)
	{
		mOuterConeAngle = outerConeAngle;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_CONE_OUTER_ANGLE, mOuterConeAngle);
			CheckError(alGetError(), "Failed to set Outer Cone Angle");
		}
	}

	void Sound::setLoop(bool loop)
	{
		mLoop = loop?AL_TRUE:AL_FALSE;

		if(mSource != AL_NONE && !mStream)
		{
			alSourcei(mSource, AL_LOOPING, mLoop);
			CheckError(alGetError(), "Failed to set Looping");
		}
	}

	void Sound::setSecondOffset(Ogre::Real seconds)
	{
		if(seconds < 0) return;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_SEC_OFFSET, seconds);
			CheckError(alGetError(), "Failed to set offset");
		}
	}

	Ogre::Real Sound::getSecondOffset()
	{
		Ogre::Real offset = 0;

		if(mSource != AL_NONE)
		{
			alGetSourcef(mSource, AL_SEC_OFFSET, &offset);
			CheckError(alGetError(), "Failed to set Looping");
		}

		return offset;
	}

	void Sound::setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z)
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
		mLocalTransformDirty = true;
	}

	void Sound::setPosition(const Ogre::Vector3& vec)
	{
		mPosition = vec;
		mLocalTransformDirty = true;
	}

	void Sound::setDirection(Ogre::Real x, Ogre::Real y, Ogre::Real z)
	{
		mDirection.x = x;
		mDirection.y = y;
		mDirection.z = z;
		mLocalTransformDirty = true;
	}

	void Sound::setDirection(const Ogre::Vector3& vec)
	{
		mDirection = vec;
		mLocalTransformDirty = true;
	}

	void Sound::_update() const
	{
		if (mParentNode)
		{
		    if (!(mParentNode->_getDerivedOrientation() == mLastParentOrientation &&
			mParentNode->_getDerivedPosition() == mLastParentPosition)
			|| mLocalTransformDirty)
		    {
			// Ok, we're out of date with SceneNode we're attached to
			mLastParentOrientation = mParentNode->_getDerivedOrientation();
			mLastParentPosition = mParentNode->_getDerivedPosition();
			mDerivedDirection = mLastParentOrientation * mDirection;
			mDerivedPosition = (mLastParentOrientation * mPosition) + mLastParentPosition;
		    }
		}
		else
		{
		    mDerivedPosition = mPosition;
		    mDerivedDirection = mDirection;
		}

		mLocalTransformDirty = false;
	}

	void Sound::_updateFading()
	{
		if(mFadeMode != FADE_NONE)
		{   
			mRunning += SoundManager::getSingletonPtr()->_getLastDeltaTime();
			// Calculate volume between min and max Gain over fade time
			Ogre::Real delta = mMaxGain - mMinGain;
			Ogre::Real gain;

			if(mFadeMode == FADE_IN)
			{
				gain = mMinGain + (delta * mRunning / mFadeTime);
				// Clamp & stop if needed
				if (gain > mMaxGain)
				{
					gain = mMaxGain;
					mFadeMode = FADE_NONE;
				}
			}
			else if(mFadeMode == FADE_OUT)
			{
				gain = mMaxGain - (delta * mRunning / mFadeTime);
				// Clamp & stop if needed
				if(gain < mMinGain)
				{
					gain = mMinGain;
					mFadeMode = FADE_NONE;
				}
			}

			// Set the adjusted gain
			setGain(gain);
		}
	}

	bool Sound::updateSound()
	{
		_update();

		if(mSource != AL_NONE)
		{
			if(isStopped() && !mManualStop && mFinishedCallback)
			{
				mFinishedCallback->execute(this);
			}

			Ogre::Real currOffset = getSecondOffset();
			if(currOffset < mPreviousOffset && !mManualRestart && mLoopedCallback)
			{
				mLoopedCallback->execute(this);
			}
			mPreviousOffset = currOffset;

			alSource3f(mSource, AL_POSITION, mDerivedPosition.x, mDerivedPosition.y, mDerivedPosition.z);
			CheckError(alGetError(), "Failed to set Position");

			alSource3f(mSource, AL_DIRECTION, mDerivedDirection.x, mDerivedDirection.y, mDerivedDirection.z);
			CheckError(alGetError(), "Failed to set Direction");

			// Fading
			_updateFading();
		}

		return true;
	}

	void Sound::initSource()
	{
		if(mSource == AL_NONE)
		{
			return;
		}

		alSourcef (mSource, AL_PITCH,				mPitch);
		alSourcef (mSource, AL_GAIN,				mGain);
		alSourcef (mSource, AL_MAX_GAIN,			mMaxGain);
		alSourcef (mSource, AL_MIN_GAIN,			mMinGain);
		alSourcef (mSource, AL_MAX_DISTANCE,		mMaxDistance);
		alSourcef (mSource, AL_ROLLOFF_FACTOR,		mRolloffFactor);
		alSourcef (mSource, AL_REFERENCE_DISTANCE,	mReferenceDistance);
		alSourcef (mSource, AL_CONE_OUTER_GAIN,		mOuterConeGain);
		alSourcef (mSource, AL_CONE_INNER_ANGLE,	mInnerConeAngle);
		alSourcef (mSource, AL_CONE_OUTER_ANGLE,	mOuterConeAngle);
		alSource3f(mSource, AL_POSITION,			mDerivedPosition.x, mDerivedPosition.y, mDerivedPosition.z);
		alSource3f(mSource, AL_VELOCITY,			mVelocity.x, mVelocity.y, mVelocity.z);
		alSource3f(mSource, AL_DIRECTION,			mDerivedDirection.x, mDerivedDirection.y, mDerivedDirection.z);
		alSourcei (mSource, AL_SOURCE_RELATIVE,		mSourceRelative);
		// There is an issue with looping and streaming, so we will
		// disable looping and deal with it on our own.
		alSourcei (mSource, AL_LOOPING,				mStream ? AL_FALSE : mLoop);
		CheckError(alGetError(), "Failed to initialize source");
	}

	void Sound::generateBuffers()
	{
		mBuffers = new BufferRef[mNumBuffers];
		alGenBuffers(mNumBuffers, mBuffers);
		CheckError(alGetError(), "Could not generate buffer");

		if(SoundManager::getSingleton().xRamSupport())
		{
			SoundManager::getSingleton().eaxSetBufferMode(mNumBuffers, mBuffers, SoundManager::xRamHardware);
		}
	}

	void Sound::calculateFormatInfo()
	{
		switch(mChannels)
		{
		case 1:
			if(mBPS == 8)
			{
				mFormat = AL_FORMAT_MONO8;
				// Set BufferSize to 250ms (Frequency divided by 4 (quarter of a second))
				mBufferSize = mFreq / 4;
			}
			else
			{
				mFormat = AL_FORMAT_MONO16;
				// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
				mBufferSize = mFreq >> 1;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				mBufferSize -= (mBufferSize % 2);
			}
			break;
		case 2:
			if(mBPS == 8)
			{
				mFormat = AL_FORMAT_STEREO16;
				// Set BufferSize to 250ms (Frequency * 2 (8bit stereo) divided by 4 (quarter of a second))
				mBufferSize = mFreq >> 1;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				mBufferSize -= (mBufferSize % 2);
			}
			else
			{
				mFormat = AL_FORMAT_STEREO16;
				// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
				mBufferSize = mFreq;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				mBufferSize -= (mBufferSize % 4);
			}
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
	}

	void Sound::queueBuffers()
	{
		alSourceQueueBuffers(mSource, mNumBuffers, mBuffers);
		CheckError(alGetError(), "Failed to bind Buffer");
	}

	void Sound::unqueueBuffers()
	{
		/*
		** If the sound doens't have a state yet it causes an
		** error when you try to unqueue the buffers! :S  In 
		** order to get around this we stop the source even if
		** it wasn't playing.
		*/
		alSourceStop(mSource);
		CheckError(alGetError(), "Failed to stop sound");

		int queued;
		alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued);
		CheckError(alGetError(), "Failed to get Source");

		alSourceUnqueueBuffers(mSource, queued, mBuffers);
		CheckError(alGetError(), "Failed to unqueue Buffers");

		
		mBuffersQueued = false;
	}

	const Ogre::String& Sound::getMovableType(void) const
	{
		return SoundFactory::FACTORY_TYPE_NAME;
	}

	const Ogre::AxisAlignedBox& Sound::getBoundingBox(void) const
	{
		// Null, Sounds are not visible
		static Ogre::AxisAlignedBox box;
		return box;
	}

	void Sound::_updateRenderQueue(Ogre::RenderQueue* queue)
	{
		// Sounds are not visible so do nothing
	}

	void Sound::_notifyAttached(Ogre::Node *parent, bool isTagPoint)
	{
		// Set the source not relative to the listener if it's attached to a node
		if(mSourceRelative)
		{
			mSourceRelative = false;
			if(mSource != AL_NONE)
			{
				alSourcei(mSource, AL_SOURCE_RELATIVE, AL_FALSE);
				CheckCondition(alGetError() == AL_NO_ERROR, 13, "Inalid Value");
			}
		}
		mParentNode = parent;
		_update();
	}

	//-----------------OgreAL::SoundFactory-----------------//

	Ogre::String SoundFactory::FACTORY_TYPE_NAME = "OgreAL_Sound";

	SoundFactory::SoundFactory()
	{}

	SoundFactory::~SoundFactory()
	{
		BufferMap::iterator bufferItr = mBufferMap.begin();
		while(bufferItr != mBufferMap.end())
		{
			alDeleteBuffers(1, &bufferItr->second);
			bufferItr++;
		}

		mBufferMap.clear();
	}

	const Ogre::String& SoundFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}

	Ogre::MovableObject* SoundFactory::createInstanceImpl(const Ogre::String& name, 
				const Ogre::NameValuePairList* params)
	{
		Ogre::String fileName = params->find(SoundManager::SOUND_FILE)->second;
		bool loop = Ogre::StringConverter::parseBool(params->find(SoundManager::LOOP_STATE)->second);
		bool stream = Ogre::StringConverter::parseBool(params->find(SoundManager::STREAM)->second);
		
		// Check to see if we can just piggy back another buffer
		if(!stream)
		{
			BufferMap::iterator bufferItr = mBufferMap.find(fileName);
			if(bufferItr != mBufferMap.end())
			{
				// We have this buffer loaded already!
				return new Sound((BufferRef)bufferItr->second, name, fileName, loop);
			}
		}

		Ogre::ResourceGroupManager *groupManager = Ogre::ResourceGroupManager::getSingletonPtr();
		Ogre::String group = groupManager->findGroupContainingResource(fileName);
		Ogre::DataStreamPtr soundData = groupManager->openResource(fileName, group);

		Sound *sound;
		if(fileName.find(".ogg") != std::string::npos || fileName.find(".OGG") != std::string::npos)
		{
			sound = new OggSound(name, soundData, loop, stream);			
		}
		else if(fileName.find(".wav") != std::string::npos || fileName.find(".WAV") != std::string::npos)
		{
			sound = new WavSound(name, soundData, loop, stream);
		}
		else
		{
			throw Ogre::Exception(Ogre::Exception::ERR_INVALIDPARAMS,
				"Sound file '" + fileName + "' is of an unsupported file type, ",
				"OgreAL::SoundManager::_createSound");
		}

		if(!stream)
		{
			// Save the reference to this buffer so we can point to it again later
			mBufferMap[fileName] = sound->getBufferRef();
		}
		return sound;
	}

	void SoundFactory::destroyInstance(Ogre::MovableObject* obj)
	{
		delete obj;
	}

	void SoundFactory::_removeBufferRef(const Ogre::String& bufferName)
	{
		BufferMap::iterator bufferItr = mBufferMap.find(bufferName);
		if(bufferItr != mBufferMap.end())
		{
			mBufferMap.erase(bufferItr);
		}
	}

	void SoundFactory::_addBufferRef(const Ogre::String& bufferName, BufferRef buffer)
	{
		BufferMap::const_iterator bufferItr = mBufferMap.find(bufferName);
		CheckCondition(bufferItr != mBufferMap.end(), 13, "Buffer named " + bufferName + " already exists!");

		CheckCondition(alIsBuffer(buffer) == AL_TRUE, 13, "Not a valid BufferRef");
		CheckError(alGetError(), "Failed to check buffer");

		mBufferMap[bufferName] = buffer;
	}
} // Namespace
