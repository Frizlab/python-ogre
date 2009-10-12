#ifndef QUICKGUIWIDGETFACTORY_H
#define QUICKGUIWIDGETFACTORY_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIWidget.h"

#include "OgrePrerequisites.h"

#include <map>
#include <string>

namespace QuickGUIEditor
{
	// forward declarations
	class MainForm;
}

namespace QuickGUI
{
	class _QuickGUIExport WidgetFactory
	{
	public:
		// Only FactoryManager can create/destroy Factories
		friend class FactoryManager;
		// Make Widgets as Friends, as they will create/destroy Widgets
		friend class ComboBox;
		friend class ComponentWidget;
		friend class ContainerWidget;
		friend class ContextMenu;
		friend class List;
		friend class Menu;
		friend class Sheet;
		friend class TabControl;
		friend class TabPage;
		friend class Widget;
		friend class GUIManager;
		// Editor creates and manages its own Widgets
		friend class QuickGUIEditor::MainForm;
	protected:
		typedef Widget* (WidgetFactory::*createWidgetFunction)(const std::string&);

	public:

		bool classRegistered(const std::string& className)
		{
			return (mFunctorMap.find(className) != mFunctorMap.end());
		}

		std::string& getNamePrefix()
		{
			return mNamingPrefix;
		}

		template<typename ClassType>
		bool registerClass(const std::string& className)
		{
			if (mFunctorMap.find(className) != mFunctorMap.end())
				return false;

			mFunctorMap[className] = &WidgetFactory::createWidget<ClassType>;

			return true;
		}

		/**
		* Sets the string prepended to every instance created by this factory
		*/
		void setNamePrefix(const std::string& prefix)
		{
			mNamingPrefix = prefix;
		}

		bool unregisterClass(const std::string& className)
		{
			return (mFunctorMap.erase(className) == 1);
		}

	protected:
		template<typename ClassType>
		ClassType* createInstance(const std::string& className, const std::string& name)
		{
			typename std::map<std::string, createWidgetFunction>::iterator iter = mFunctorMap.find(className);

			if (iter == mFunctorMap.end())
				throw Exception(Exception::ERR_FACTORY,"\"" + className + "\" is not a registered class!","WidgetFactory::createInstance");

			Widget* newInstance = (this->*(*iter).second)(mNamingPrefix + name);

			return dynamic_cast<ClassType*>(newInstance);
		}

		void destroyInstance(Widget* instance)
		{
			OGRE_DELETE_T(instance,Widget,Ogre::MEMCATEGORY_GENERAL);
		}

	protected:
		WidgetFactory() : mNamingPrefix("") {}
		virtual ~WidgetFactory() {}

		std::map<std::string, createWidgetFunction> mFunctorMap;

		std::string mNamingPrefix;
		
		template<typename ClassType>
		Widget* createWidget(const std::string& param1)
		{
			return OGRE_NEW_T(ClassType,Ogre::MEMCATEGORY_GENERAL)(param1);
		}
	};
}

#endif
