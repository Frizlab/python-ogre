#ifndef QUICKGUIWIDGETDESCFACTORY_H
#define QUICKGUIWIDGETDESCFACTORY_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIWidget.h"

#include "OgreSingleton.h"

#include <map>

namespace QuickGUI
{
	/*
	* This class stores access to functions used to create widgets desc objects.
	* User defined widget desc objects will need to be registered with this class.
	*/
	class _QuickGUIExport WidgetDescFactoryManager :
		public Ogre::Singleton<WidgetDescFactoryManager>
	{
	public:

		typedef WidgetDesc* (*WidgetDescFactory)();

		static WidgetDescFactoryManager& getSingleton(void); 
		static WidgetDescFactoryManager* getSingletonPtr(void);

		/**
		* Creates a desc object given the class name of the desc to create.
		*/
		WidgetDesc* createWidgetDesc(const Ogre::String& className);

		void registerWidgetDescFactory(const Ogre::String& className, WidgetDescFactory factory );

	protected:
	private:
		std::map<Ogre::String,WidgetDescFactory> mFactories;
	};
}

#endif
