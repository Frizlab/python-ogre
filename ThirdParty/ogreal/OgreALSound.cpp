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

#include "OgreALSound.h"
#include "OgreALSoundManager.h"
#include "OgrePrerequisites.h"

namespace OgreAL {
	Sound::Sound() :
		mPitch(1.0),
		mGain(1.0),
		mMaxGain(1.0),
		mMinGain(0.0),
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

	Sound::Sound(const Ogre::String &name, const Ogre::String& fileName) :
		MovableObject(name),
		mPitch(1.0), mGain(1.0),
		mMaxGain(1.0),
		mMinGain(0.0),
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
		MovableObject(name),
		mBuffer(buffer),
		mLoop(loop?AL_TRUE:AL_FALSE),
		mPitch(1.0), mGain(1.0),
		mMaxGain(1.0), mMinGain(0.0),
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

		createAndBindSource();
	}

	Sound::~Sound()
	{
		alSourceStop(mSource);
		alSourcei(mSource, AL_BUFFER, 0);
		alDeleteSources(1, &mSource);
		checkError(__FUNCTION__);
	}

	bool Sound::play()
	{
		if(isPlaying()) return true;

		alSourcePlay(mSource);	    
		return true;
	}

	bool Sound::isPlaying() const
	{
		State state;    
		alGetSourcei(mSource, AL_SOURCE_STATE, &state);    
		return (state == AL_PLAYING);
	}

	bool Sound::pause()
	{
		if(!isPlaying()) return true;

		alSourcePause(mSource);
		return true;
	}

	bool Sound::isPaused() const
	{
		State state;    
		alGetSourcei(mSource, AL_SOURCE_STATE, &state);    
		return (state == AL_PAUSED);
	}

	bool Sound::stop()
	{
		if(isStopped()) return true;

		alSourceStop(mSource);
		return true;
	}

	bool Sound::isStopped() const
	{
		State state;    
		alGetSourcei(mSource, AL_SOURCE_STATE, &state);    
		return (state == AL_STOPPED);
	}

	bool Sound::isInitial() const
	{
		State state;    
		alGetSourcei(mSource, AL_SOURCE_STATE, &state);    
		return (state == AL_INITIAL);
	}

	void Sound::setPitch(Ogre::Real pitch)
	{
		mPitch = pitch;
		alSourcef(mSource, AL_PITCH, mPitch);
	}

	void Sound::setGain(Ogre::Real gain)
	{
		mGain = gain;
		alSourcef(mSource, AL_GAIN, mGain);
	}

	void Sound::setMaxGain(Ogre::Real maxGain)
	{
		mMaxGain = maxGain;
		alSourcef(mSource, AL_MAX_GAIN, mMaxGain);
	}

	void Sound::setMinGain(Ogre::Real minGain)
	{
		mMinGain = minGain;
		alSourcef(mSource, AL_MIN_GAIN, mMinGain);
	}

	void Sound::setMaxDistance(Ogre::Real maxDistance)
	{
		mMaxDistance = maxDistance;
		alSourcef(mSource, AL_MAX_DISTANCE, mMaxDistance);
	}

	void Sound::setRolloffFactor(Ogre::Real rolloffFactor)
	{
		mRolloffFactor = rolloffFactor;
		alSourcef(mSource, AL_ROLLOFF_FACTOR, mRolloffFactor);
	}

	void Sound::setReferenceDistance(Ogre::Real refDistance)
	{
		mReferenceDistance = refDistance;
		alSourcef(mSource, AL_REFERENCE_DISTANCE, mReferenceDistance);
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
		alSource3f(mSource, AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
	}

	void Sound::setVelocity(const Ogre::Vector3& vec)
	{
		mVelocity = vec;
		alSource3f(mSource, AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
	}

	void Sound::setRelativeToListener(bool relative)
	{
		// Do not set to relative if it's connected to a node
		if(mParentNode) return;

		mSourceRelative = relative;
		alSourcei(mSource, AL_SOURCE_RELATIVE, mSourceRelative);
	}

	void Sound::setOuterConeGain(Ogre::Real outerConeGain)
	{
		mOuterConeGain = outerConeGain;
		alSourcef(mSource, AL_CONE_OUTER_GAIN, mOuterConeGain);
	}

	void Sound::setInnerConeAngle(Ogre::Real innerConeAngle)
	{
		mInnerConeAngle = innerConeAngle;
		alSourcef(mSource, AL_CONE_INNER_ANGLE, mInnerConeAngle);
	}

	void Sound::setOuterConeAngle(Ogre::Real outerConeAngle)
	{
		mOuterConeAngle = outerConeAngle;
		alSourcef(mSource, AL_CONE_OUTER_ANGLE, mOuterConeAngle);
	}

	void Sound::setLoop(bool loop)
	{
		mLoop = loop?AL_TRUE:AL_FALSE;
		alSourcei(mSource, AL_LOOPING, mLoop);
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

	const Ogre::Vector3& Sound::getPosition() const
	{
		return mPosition;
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

	const Ogre::Vector3& Sound::getDirection() const
	{
		return mDirection;
	}

	const Ogre::Vector3& Sound::getDerivedPosition(void) const
	{
		return mDerivedPosition;
	}

	const Ogre::Vector3& Sound::getDerivedDirection(void) const
	{
		return mDerivedPosition;
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

	bool Sound::_updateSound()
	{
		_update();
		alSource3f(mSource, AL_POSITION, mDerivedPosition.x, mDerivedPosition.y, mDerivedPosition.z);
		alSource3f(mSource, AL_DIRECTION, mDerivedDirection.x, mDerivedDirection.y, mDerivedDirection.z);
		return true;
	}

	void Sound::createAndBindSource()
	{
		if(SoundManager::getSingleton().xRamSupport())
		{
			SoundManager::getSingleton().eaxSetBufferMode(1, &mBuffer, SoundManager::xRamHardware);
		}

		alGenSources(1, &mSource);
		initSource();
		checkError(__FUNCTION__);

		alSourcei(mSource, AL_BUFFER, mBuffer);
		checkError(__FUNCTION__);
	}

	void Sound::initSource()
	{
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
		alSourcei (mSource, AL_LOOPING,				mLoop);
		checkError(__FUNCTION__);
	}

	void Sound::checkError(const Ogre::String& source) const
	{
		int error = alGetError();
 
		if(error != AL_NO_ERROR)
		{
			throw Ogre::Exception(3, "OpenAL Error: " + errorToString(error), source);
		}
	}

	Ogre::String Sound::errorToString(int error) const
	{
		switch(error)
		{
			case AL_INVALID_VALUE:
				return Ogre::String("The value pointer given is not valid");
				break;
			case AL_INVALID_ENUM:
				return Ogre::String("The specified parameter is not valid");
				break;
			case AL_INVALID_NAME:
				return Ogre::String("The specified source name is not valid");
				break;
			case AL_INVALID_OPERATION:
				return Ogre::String("There is no current context");
				break;
			default:
				return Ogre::String("Unknown Error");
				break;
		}
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
			alSourcei(mSource, AL_SOURCE_RELATIVE, AL_FALSE);
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
		Ogre::String inType = params->find(SoundManager::FILE_TYPE)->second;
		Ogre::String outType = params->find(SoundManager::OUTPUT_TYPE)->second;
		bool loop = Ogre::StringConverter::parseBool(params->find(SoundManager::LOOP_STATE)->second);
		AudioFormat audioFormat = static_cast<AudioFormat>
			(Ogre::StringConverter::parseInt(params->find(SoundManager::AUDIO_FORMAT)->second));

		BufferMap::iterator bufferItr = mBufferMap.find(fileName);
		if(bufferItr != mBufferMap.end())
		{
			// We have this buffer loaded already!
			return new Sound((BufferRef)bufferItr->second, name, fileName, loop);
		}

		if(inType.compare(SoundManager::OGG) == 0)
		{
			if(outType.compare(SoundManager::SOUND) == 0)
			{
				Sound *sound = new OggSound(name, fileName, loop, audioFormat);
				// Save the reference to this buffer so we can point to it again later
				mBufferMap[fileName] = sound->getBufferRef();
				return sound;
			}
			else if(outType.compare(SoundManager::STREAM) == 0)
			{
				return new OggSoundStream(name, fileName, loop, audioFormat);
			}
			else
			{
				throw Ogre::Exception(6, "Unknown Ouput Type " + outType,
					"OgreAL::SoundFactory::createInstanceImpl");
			}
		}
		else if(inType.compare(SoundManager::WAV) == 0)
		{
			if(outType.compare(SoundManager::SOUND) == 0)
			{
				Sound *sound = new WavSound(name, fileName, loop, audioFormat);
				// Save the reference to this buffer so we can point to it again later
				mBufferMap[fileName] = sound->getBufferRef();
				return sound;
			}
			else if(outType.compare(SoundManager::STREAM) == 0)
			{
				throw Ogre::Exception(6, "Wav Streaming is not supported ",
					"OgreAL::SoundFactory::createInstanceImpl");
			}
			else
			{
				throw Ogre::Exception(6, "Unknown Ouput Type " + outType,
					"OgreAL::SoundFactory::createInstanceImpl");
			}
		}
		else
		{
			throw Ogre::Exception(6, "Unsupported File Type " + outType,
					"OgreAL::SoundFactory::createInstanceImpl");
		}
	}

	void SoundFactory::destroyInstance(Ogre::MovableObject* obj)
	{
		delete obj;
	}
} // Namespace
