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

#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"

template<> QuickGUI::SkinDefinitionManager* Ogre::Singleton<QuickGUI::SkinDefinitionManager>::ms_Singleton = 0;

namespace QuickGUI
{
	SkinDefinitionManager::SkinDefinitionManager()
	{
		OGRE_NEW SkinTypeManager();
	}

	SkinDefinitionManager::~SkinDefinitionManager()
	{
		for(std::map<Ogre::String,SkinDefinition*>::iterator it = mWidgetSkinDefs.begin(); it != mWidgetSkinDefs.end(); ++it)
			OGRE_DELETE_T((*it).second,SkinDefinition,Ogre::MEMCATEGORY_GENERAL);

		OGRE_DELETE SkinTypeManager::getSingletonPtr();
	}

	SkinDefinitionManager* SkinDefinitionManager::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	SkinDefinitionManager& SkinDefinitionManager::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	SkinDefinition* SkinDefinitionManager::getSkinDefinition(const Ogre::String& className)
	{
		if(!hasSkinDefinition(className))
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition does not exist for class \"" + className + "\"!","SkinDefinitionManager::getSkinDefinition");

		return mWidgetSkinDefs[className];
	}

	bool SkinDefinitionManager::hasSkinDefinition(const Ogre::String& className)
	{
		return (mWidgetSkinDefs.find(className) != mWidgetSkinDefs.end());
	}

	void SkinDefinitionManager::registerSkinDefinition(const Ogre::String& className, SkinDefinition* d)
	{
		if(hasSkinDefinition(className))
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition already exists for class \"" + className + "\"!","SkinDefinition::registerSkinDefinition");

		mWidgetSkinDefs[className] = d;
	}
}
