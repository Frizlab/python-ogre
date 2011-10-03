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

#include "QuickGUIEventHandlerManager.h"

template<> QuickGUI::EventHandlerManager* Ogre::Singleton<QuickGUI::EventHandlerManager>::ms_Singleton = 0;

namespace QuickGUI
{
	EventHandlerManager::EventHandlerManager()
	{
	}

	EventHandlerManager::~EventHandlerManager()
	{
	}

	EventHandlerManager* EventHandlerManager::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	EventHandlerManager& EventHandlerManager::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	void EventHandlerManager::executEventHandler(const Ogre::String& handlerName, EventArgs& args)
	{
		std::map<Ogre::String,EventHandlerSlot*>::iterator it = mUserDefinedEventHandlers.find(handlerName);

		if(it != mUserDefinedEventHandlers.end())
			mUserDefinedEventHandlers[handlerName]->execute(args);
	}

	bool EventHandlerManager::hasEventHandler(const Ogre::String& handlerName)
	{
		return (mUserDefinedEventHandlers.find(handlerName) != mUserDefinedEventHandlers.end());
	}

	void EventHandlerManager::registerEventHandler(const Ogre::String& handlerName, EventHandlerSlot* function)
	{
		std::map<Ogre::String,EventHandlerSlot*>::iterator it = mUserDefinedEventHandlers.find(handlerName);

		if(it != mUserDefinedEventHandlers.end())
			OGRE_DELETE_T(it->second,EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);

		mUserDefinedEventHandlers[handlerName] = function;
	}

	void EventHandlerManager::unregisterEventHandler(const Ogre::String& handlerName)
	{
		std::map<Ogre::String,EventHandlerSlot*>::iterator it = mUserDefinedEventHandlers.find(handlerName);

		if(it != mUserDefinedEventHandlers.end())
		{
			EventHandlerSlot* s = it->second;
			mUserDefinedEventHandlers.erase(it);
			OGRE_DELETE_T(s,EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}
}
