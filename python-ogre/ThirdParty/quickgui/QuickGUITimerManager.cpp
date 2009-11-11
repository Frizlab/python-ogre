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

#include "QuickGUITimerManager.h"

template<> QuickGUI::TimerManager* Ogre::Singleton<QuickGUI::TimerManager>::ms_Singleton = 0;

namespace QuickGUI
{
	TimerManager::TimerManager()
	{
		Ogre::Root::getSingleton().addFrameListener(this);
	}

	TimerManager::~TimerManager()
	{
		Ogre::Root::getSingleton().removeFrameListener(this);

		for(std::vector<Timer*>::iterator it = mTimers.begin(); it != mTimers.end(); ++it)
			OGRE_DELETE_T((*it),Timer,Ogre::MEMCATEGORY_GENERAL);
	}

	TimerManager* TimerManager::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	TimerManager& TimerManager::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	bool TimerManager::frameStarted(const Ogre::FrameEvent& evt)
	{
		for(std::vector<Timer*>::iterator it = mTimers.begin(); it != mTimers.end(); ++it)
			(*it)->update(evt.timeSinceLastFrame);

		return true;
	}

	Timer* TimerManager::createTimer(TimerDesc& d)
	{
		Timer* newTimer = OGRE_NEW_T(Timer,Ogre::MEMCATEGORY_GENERAL)(d);
		mTimers.push_back(newTimer);
		return newTimer;
	}

	void TimerManager::destroyTimer(Timer* t)
	{
		for(std::vector<Timer*>::iterator it = mTimers.begin(); it != mTimers.end(); ++it)
		{
			if((*it) == t)
			{
				Timer* t = (*it);
				mTimers.erase(it);
				OGRE_DELETE_T(t,Timer,Ogre::MEMCATEGORY_GENERAL);
				return;
			}
		}
	}
}
