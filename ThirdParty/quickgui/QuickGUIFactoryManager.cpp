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

#include "QuickGUIFactoryManager.h"
#include "QuickGUIDescFactory.h"
#include "QuickGUIWidgetFactory.h"

#include "OgreMemoryAllocatorConfig.h"

template<> QuickGUI::FactoryManager* Ogre::Singleton<QuickGUI::FactoryManager>::ms_Singleton = 0;

namespace QuickGUI
{
	FactoryManager::FactoryManager()
	{
		mWidgetFactory = new WidgetFactory();
		mDescFactory = new DescFactory();
	}

	FactoryManager::~FactoryManager()
	{
		delete mWidgetFactory;
		delete mDescFactory;
	}

	FactoryManager* FactoryManager::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	FactoryManager& FactoryManager::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	WidgetFactory* FactoryManager::getWidgetFactory()
	{
		return mWidgetFactory;
	}

	DescFactory* FactoryManager::getDescFactory()
	{
		return mDescFactory;
	}
}
