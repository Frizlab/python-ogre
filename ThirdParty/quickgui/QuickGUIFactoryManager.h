#ifndef QUICKGUIFACTORYMANAGER_H
#define QUICKGUIFACTORYMANAGER_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIFactory.h"
#include "QuickGUIWidget.h"

#include "OgreSingleton.h"

#include <map>

namespace QuickGUI
{
	// forward declarations
	class Widget;
	class Desc;

	/*
	* The Factory manager manages factories of QuickGUI, including Widget and Desc factories.
	*/
	class _QuickGUIExport FactoryManager :
		public Ogre::Singleton<FactoryManager>
	{
	public:
		friend class Root;
	public:
		static FactoryManager& getSingleton(void); 
		static FactoryManager* getSingletonPtr(void);

		/**
		* Returns the Widget Factory.
		*/
		Factory<Widget>* getWidgetFactory();
		/**
		* Returns the WidgetDesc Factory.
		*/
		Factory<Desc>* getDescFactory();

	protected:
		FactoryManager();
		virtual ~FactoryManager();

		Factory<Widget>* mWidgetFactory;
		Factory<Desc>* mDescFactory;

	private:
	};
}

#endif
