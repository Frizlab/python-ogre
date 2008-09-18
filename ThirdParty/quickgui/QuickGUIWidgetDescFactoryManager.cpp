#include "QuickGUIWidgetDescFactoryManager.h"

template<> QuickGUI::WidgetDescFactoryManager* Ogre::Singleton<QuickGUI::WidgetDescFactoryManager>::ms_Singleton = 0; 

namespace QuickGUI
{
	WidgetDescFactoryManager* WidgetDescFactoryManager::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	WidgetDescFactoryManager& WidgetDescFactoryManager::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	WidgetDesc* WidgetDescFactoryManager::createWidgetDesc(const Ogre::String& className)
	{
		std::map<Ogre::String,WidgetDescFactory>::iterator it = mFactories.find(className);
		if(it == mFactories.end())
			throw Exception(Exception::ERR_FACTORY_NOT_FOUND,"No factory for widget desc \"" + className + "\" has been registered!", "WidgetDescFactoryManager::createWidgetDesc");

		// Execute the function pointer using the passed in WidgetDesc parameter.
		return (*it).second();
	}

	void WidgetDescFactoryManager::registerWidgetDescFactory(const Ogre::String& className, WidgetDescFactory factory)
	{
		if(mFactories.find(className) != mFactories.end())
			throw Exception(Exception::ERR_DUPLICATE_FACTORY,"Factory for widget desc \"" + className + "\" has already been registered!", "WidgetDescFactoryManager::registerWidgetFactory");

		mFactories[className] = factory;
	}
}
