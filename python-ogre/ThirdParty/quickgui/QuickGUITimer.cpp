/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

#include "QuickGUITimer.h"

#include "OgreMemoryAllocatorConfig.h"

namespace QuickGUI
{
	TimerDesc::TimerDesc()
	{
		repeat= true;
		timePeriod = -1;
	}

	Timer::Timer(TimerDesc& d) :
		mUpdate(false),
		mTimeAccumulator(0),
		mCallback(NULL)
	{
		mDesc.repeat = d.repeat;
		mDesc.timePeriod = d.timePeriod;
	}

	Timer::~Timer()
	{
		OGRE_DELETE_T(mCallback,TimerCallbackSlot,Ogre::MEMCATEGORY_GENERAL);
	}

	void Timer::clearCallback()
	{
		OGRE_DELETE_T(mCallback,TimerCallbackSlot,Ogre::MEMCATEGORY_GENERAL);
		mCallback = NULL;
	}

	bool Timer::getRepeat()
	{
		return mDesc.repeat;
	}

	float Timer::getTimePeriod()
	{
		return mDesc.timePeriod;
	}

	bool Timer::isUpdating()
	{
		return mUpdate;
	}

	void Timer::reset()
	{
		mTimeAccumulator = 0;
	}

	void Timer::resume()
	{
		mUpdate = true;
	}

	void Timer::setCallback(TimerCallbackSlot* p)
	{
		mCallback = p;
	}

	void Timer::setRepeat(bool repeat)
	{
		mDesc.repeat = repeat;
	}

	void Timer::setTimePeriod(float timeInSeconds)
	{
		mDesc.timePeriod = timeInSeconds;
	}

	void Timer::start()
	{
		mTimeAccumulator = 0;
		mUpdate = true;
	}

	void Timer::stop()
	{
		mUpdate = false;
	}

	void Timer::update(float timeInSeconds)
	{
		if(mUpdate)
		{
			mTimeAccumulator += timeInSeconds;

			if(mTimeAccumulator >= mDesc.timePeriod)
			{
				mTimeAccumulator = 0;
				if(!mDesc.repeat)
					mUpdate = false;
				if(mCallback != NULL)
					mCallback->execute();
			}
		}
	}
}
