#include "QuickGUISheetManager.h"

template<> QuickGUI::SheetManager* Ogre::Singleton<QuickGUI::SheetManager>::ms_Singleton = 0;

namespace QuickGUI
{
	SheetManager::SheetManager()
	{
	}

	SheetManager::~SheetManager()
	{
		for(std::map<Ogre::String,Sheet*>::iterator it = mSheets.begin(); it != mSheets.end(); ++it)
			OGRE_DELETE_T((*it).second,Sheet,Ogre::MEMCATEGORY_GENERAL);
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

	Sheet* SheetManager::createSheet(SheetDesc* d)
	{
		if(exists(d->widget_name))
			throw Exception(Exception::ERR_DUPLICATE_ITEM,"Sheet by the name of \"" + d->widget_name + "\" already exists!","SheetManager::createSheet");

		Sheet* newSheet = OGRE_NEW_T(Sheet,Ogre::MEMCATEGORY_GENERAL)(d);
		mSheets[d->widget_name] = newSheet;

		return newSheet;
	}

	Sheet* SheetManager::createSheet(const Ogre::String& fileName)
	{
		Sheet* newSheet = OGRE_NEW_T(Sheet,Ogre::MEMCATEGORY_GENERAL)(fileName);

		if(exists(newSheet->getName()))
			throw Exception(Exception::ERR_DUPLICATE_ITEM,"Sheet by the name of \"" + newSheet->getName() + "\" already exists!","SheetManager::createSheet");

		mSheets[newSheet->getName()] = newSheet;

		return newSheet;
	}

	void SheetManager::destroySheet(const Ogre::String& sheetName)
	{
		if(!exists(sheetName))
			throw Exception(Exception::ERR_ITEM_NOT_FOUND,"Sheet by the name of \"" + sheetName + "\" does not exist!","SheetManager::destroySheet");

		OGRE_DELETE_T(mSheets[sheetName],Sheet,Ogre::MEMCATEGORY_GENERAL);
		mSheets.erase(sheetName);
	}

	void SheetManager::destroySheet(Sheet* s)
	{
		destroySheet(s->getName());
	}

	bool SheetManager::exists(const Ogre::String& sheetName)
	{
		return (mSheets.find(sheetName) != mSheets.end());
	}

	Sheet* SheetManager::getSheet(const Ogre::String& sheetName)
	{
		if(!exists(sheetName))
			throw Exception(Exception::ERR_ITEM_NOT_FOUND,"Sheet by the name of \"" + sheetName + "\" does not exist!","SheetManager::destroySheet");

		return mSheets[sheetName];
	}
}
