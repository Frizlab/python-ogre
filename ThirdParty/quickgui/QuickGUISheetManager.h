#ifndef QUICKGUISHEETMANAGER_H
#define QUICKGUISHEETMANAGER_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUISheet.h"

#include "OgreSingleton.h"

#include <map>

namespace QuickGUI
{
	/*
	* The Sheet manager manages creation, deletion, and access of Sheets
	*/
	class _QuickGUIExport SheetManager :
		public Ogre::Singleton<SheetManager>
	{
	public:
		friend class Root;
	public:
		static SheetManager& getSingleton(void); 
		static SheetManager* getSingletonPtr(void);

		/**
		* Create a Sheet from a desc object.
		*/
		Sheet* createSheet(SheetDesc* d);
		/**
		* Create a Sheet from a file.
		*/
		Sheet* createSheet(const Ogre::String& fileName);

		/**
		* Destroy the Sheet with the name given.
		*/
		void destroySheet(const Ogre::String& sheetName);
		/**
		* Destroy the Sheet given.
		*/
		void destroySheet(Sheet* s);

		/**
		* Returns true if a sheet with the name given exists, false otherwise.
		*/
		bool exists(const Ogre::String& sheetName);

		/**
		* Returns the Sheet with the name given.
		*/
		Sheet* getSheet(const Ogre::String& sheetName);

	protected:
		SheetManager();
		virtual ~SheetManager();

		std::map<Ogre::String,Sheet*> mSheets;

	private:
	};
}

#endif
