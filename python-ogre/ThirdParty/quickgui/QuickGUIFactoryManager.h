#ifndef QUICKGUIFACTORYMANAGER_H
#define QUICKGUIFACTORYMANAGER_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIFactory.h"
#include "QuickGUIWidgetFactory.h"
#include "QuickGUIWidget.h"

#include "OgreSingleton.h"

#include <map>

namespace QuickGUI
{
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
		WidgetFactory<Widget>* getWidgetFactory();
		/**
		* Returns the WidgetDesc Factory.
		*/
		Factory<WidgetDesc>* getWidgetDescFactory();

	protected:
		FactoryManager();
		virtual ~FactoryManager();

		WidgetFactory<Widget>* mWidgetFactory;
		Factory<WidgetDesc>* mWidgetDescFactory;

	private:
	};
}

#endif
