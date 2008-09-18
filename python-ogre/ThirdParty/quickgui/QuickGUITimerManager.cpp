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
			delete (*it);
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
		Timer* newTimer = new Timer(d);
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
				delete t;
				return;
			}
		}
	}
}
