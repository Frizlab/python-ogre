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

#include "QuickGUISheetManager.h"
#include "QuickGUISheet.h"
#include "QuickGUIDescManager.h"
#include "QuickGUIException.h"

template<> QuickGUI::SheetManager* Ogre::Singleton<QuickGUI::SheetManager>::ms_Singleton = 0;

namespace QuickGUI
{
	SheetManager::SheetManager()
	{
	}

	SheetManager::~SheetManager()
	{
		cleanup();

		for(std::list<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
			OGRE_DELETE_T((*it),Sheet,Ogre::MEMCATEGORY_GENERAL);
	}

	SheetManager* SheetManager::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	SheetManager& SheetManager::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	void SheetManager::cleanup()
	{
		std::list<Sheet*>::iterator it;
		while(!mFreeList.empty())
		{
			// Remove from mSheets list if found
			it = std::find(mSheets.begin(),mSheets.end(),mFreeList.front());
			if(it != mSheets.end())
				mSheets.erase(it);
			// Delete Sheet
			OGRE_DELETE_T(mFreeList.front(),Sheet,Ogre::MEMCATEGORY_GENERAL);
			// Update FreeList
			mFreeList.pop_front();
		}
	}

	Sheet* SheetManager::createSheet(SheetDesc* d)
	{
		Sheet* newSheet = OGRE_NEW_T(Sheet,Ogre::MEMCATEGORY_GENERAL)(d);
		mSheets.push_back(newSheet);

		return newSheet;
	}

	Sheet* SheetManager::createSheet(const Size& s)
	{
		SheetDesc* sd = DescManager::getSingleton().getDefaultSheetDesc();
		sd->resetToDefault();
		sd->widget_dimensions.size = s;

		Sheet* newSheet = OGRE_NEW_T(Sheet,Ogre::MEMCATEGORY_GENERAL)(sd);
		mSheets.push_back(newSheet);

		return newSheet;
	}

	Sheet* SheetManager::createSheet(const Ogre::String& s, bool loadFromFile)
	{
		Sheet* newSheet = OGRE_NEW_T(Sheet,Ogre::MEMCATEGORY_GENERAL)(s,loadFromFile);

		mSheets.push_back(newSheet);

		return newSheet;
	}

	void SheetManager::destroySheet(Sheet* s)
	{
		if(s == NULL)
			return;

		if(std::find(mSheets.begin(),mSheets.end(),s) != mSheets.end())
		{
			std::list<Sheet*>::iterator it = std::find(mFreeList.begin(),mFreeList.end(),s);
			if(it == mFreeList.end())
				mFreeList.push_back(s);
		}
		else
			throw Exception(Exception::ERR_INVALIDPARAMS,"The sheet provided was not created by the SheetManager!","SheetManager::destroySheet");
	}

	Sheet* SheetManager::getSheet(const std::string& sheetName)
	{
		for(std::list<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
		{
			if((*it)->getName() == sheetName)
				return (*it);
		}

		return NULL;
	}

	std::list<Sheet*> SheetManager::getSheets()
	{
		std::list<Sheet*> sheets;

		for(std::list<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
		{
			// Only add sheets that are not currently queued for deletion.
			if(std::find(mFreeList.begin(),mFreeList.end(),(*it)) == mFreeList.end())
				sheets.push_back((*it));
		}

		return sheets;
	}

	void SheetManager::resizeAllSheets(Size s)
	{
		for(std::list<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
			(*it)->setSize(s);
	}

	void SheetManager::resizeAllSheets(int widthInPixels, int heightInPixels)
	{
		resizeAllSheets(Size(widthInPixels,heightInPixels));
	}

	bool SheetManager::sheetExists(const std::string& sheetName)
	{
		for(std::list<Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
		{
			if((*it)->getName() == sheetName)
				return true;
		}

		return false;
	}
}
