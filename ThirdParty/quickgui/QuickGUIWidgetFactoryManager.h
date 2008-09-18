#ifndef QUICKGUIWIDGETFACTORY_H
#define QUICKGUIWIDGETFACTORY_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIWidget.h"

#include "OgreSingleton.h"

#include <map>

namespace QuickGUI
{
	/*
	* This class stores access to functions used to create widgets.
	* User defined widgets will need to be registered with this class.
	*/
	class _QuickGUIExport WidgetFactoryManager :
		public Ogre::Singleton<WidgetFactoryManager>
	{
	public:

		typedef Widget* (*WidgetFactory)(const Ogre::String& name);

		static WidgetFactoryManager& getSingleton(void); 
		static WidgetFactoryManager* getSingletonPtr(void);

		Widget* createWidget(const Ogre::String& className, const Ogre::String& name);

		bool hasWidgetFactory(const Ogre::String& className);

		void registerWidgetFactory(const Ogre::String& className, WidgetFactory factory );

	protected:
	private:
		std::map<Ogre::String,WidgetFactory> mFactories;
	};
}

#endif
