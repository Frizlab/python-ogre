#include "QuickGUIFactoryManager.h"
#include "QuickGUIWidget.h"
#include "QuickGUIDesc.h"

#include "OgreMemoryAllocatorConfig.h"

template<> QuickGUI::FactoryManager* Ogre::Singleton<QuickGUI::FactoryManager>::ms_Singleton = 0;

namespace QuickGUI
{
	FactoryManager::FactoryManager()
	{
		mWidgetFactory = new Factory<Widget>();
		mDescFactory = new Factory<Desc>();
	}

	FactoryManager::~FactoryManager()
	{
		delete mWidgetFactory;
		delete mDescFactory;
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

	Factory<Widget>* FactoryManager::getWidgetFactory()
	{
		return mWidgetFactory;
	}

	Factory<Desc>* FactoryManager::getDescFactory()
	{
		return mDescFactory;
	}
}
