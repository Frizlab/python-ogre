#include "QuickGUIFactoryManager.h"

#include "OgreMemoryAllocatorConfig.h"

template<> QuickGUI::FactoryManager* Ogre::Singleton<QuickGUI::FactoryManager>::ms_Singleton = 0;

namespace QuickGUI
{
	FactoryManager::FactoryManager()
	{
		mWidgetFactory = new WidgetFactory<Widget>();
		mWidgetDescFactory = new Factory<WidgetDesc>();
	}

	FactoryManager::~FactoryManager()
	{
		delete mWidgetFactory;
		delete mWidgetDescFactory;
	}

	FactoryManager* FactoryManager::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	FactoryManager& FactoryManager::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	WidgetFactory<Widget>* FactoryManager::getWidgetFactory()
	{
		return mWidgetFactory;
	}

	Factory<WidgetDesc>* FactoryManager::getWidgetDescFactory()
	{
		return mWidgetDescFactory;
	}
}
