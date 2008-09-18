#include "QuickGUIWidgetFactoryManager.h"

template<> QuickGUI::WidgetFactoryManager* Ogre::Singleton<QuickGUI::WidgetFactoryManager>::ms_Singleton = 0; 

namespace QuickGUI
{
	WidgetFactoryManager* WidgetFactoryManager::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	WidgetFactoryManager& WidgetFactoryManager::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	Widget* WidgetFactoryManager::createWidget(const Ogre::String& className, const Ogre::String& name)
	{
		std::map<Ogre::String,WidgetFactory>::iterator it = mFactories.find(className);
		if(it == mFactories.end())
			throw Exception(Exception::ERR_FACTORY_NOT_FOUND,"No factory for widget class \"" + className + "\" has been registered!", "WidgetFactoryManager::createWidget");

		// Execute the function pointer using the passed in WidgetDesc parameter.
		return (*it).second(name);
	}

	bool WidgetFactoryManager::hasWidgetFactory(const Ogre::String& className)
	{
		return (mFactories.find(className) != mFactories.end());
	}

	void WidgetFactoryManager::registerWidgetFactory(const Ogre::String& className, WidgetFactory factory )
	{
		if(mFactories.find(className) != mFactories.end())
			throw Exception(Exception::ERR_DUPLICATE_FACTORY,"Factory for widget class \"" + className + "\" has already been registered!", "WidgetFactoryManager::registerWidgetFactory");

		mFactories[className] = factory;
	}
}
