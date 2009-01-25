#ifndef QUICKGUIWIDGETFACTORY_H
#define QUICKGUIWIDGETFACTORY_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"

#include "OgrePrerequisites.h"
#include "OgreStringConverter.h"

namespace QuickGUI
{
	template<typename BaseClassType>
	class _QuickGUIExport WidgetFactory
	{
	public:
		// Only FactoryManager can create/destroy Factories
		friend class FactoryManager;
	protected:
		typedef BaseClassType* (*createObjectFunc)(const Ogre::String&);

	public:
		template<typename ClassType>
		bool registerClass(const Ogre::String& className)
		{
			if (m_object_creator.find(className) != m_object_creator.end())
			return false;

			m_object_creator[className] = &createObject<BaseClassType,ClassType>;

			return true;
		}

		bool unregisterClass(const Ogre::String& className)
		{
			return (m_object_creator.erase(className) == 1);
		}

		BaseClassType* createInstance(const Ogre::String& className, const Ogre::String& instanceName)
		{
			if(instanceExists(instanceName))
				throw Exception(Exception::ERR_FACTORY,"Instance with name \"" + instanceName + "\" already exists!","WidgetFactory::createInstance");

			typename std::map<Ogre::String, createObjectFunc>::iterator iter = m_object_creator.find(className);

			if (iter == m_object_creator.end())
				throw Exception(Exception::ERR_FACTORY,"\"" + className + "\" is not a registered class!","WidgetFactory::createInstance");

			BaseClassType* newInstance = ((*iter).second)(instanceName);
			mInstances[instanceName] = newInstance;

			return newInstance;
		}

		BaseClassType* createInstance(const Ogre::String& className)
		{
			return createInstance(className,Ogre::StringConverter::toString(mNumAutoNamedInstances++));
		}

		bool destroyInstance(const Ogre::String& instanceName)
		{
			if(!instanceExists(instanceName))
				throw Exception(Exception::ERR_FACTORY,"Instance with name \"" + instanceName + "\" does not exist!","WidgetFactory::getInstance");

			OGRE_DELETE_T(mInstances[instanceName],BaseClassType,Ogre::MEMCATEGORY_GENERAL);
			mInstances.erase(instanceName);

			return true;
		}

		bool destroyInstance(BaseClassType* instance)
		{
    		typename std::map<Ogre::String, BaseClassType*>::iterator it;
			for(it = mInstances.begin(); it != mInstances.end(); ++it)
			{
				if((*it).second == instance)
				{
					OGRE_DELETE_T((*it).second,BaseClassType,Ogre::MEMCATEGORY_GENERAL);
					mInstances.erase(it);

					return true;
				}
			}

			return false;
		}

		BaseClassType* getInstance(const Ogre::String& instanceName)
		{
			if(!instanceExists(instanceName))
				throw Exception(Exception::ERR_FACTORY,"Instance with name \"" + instanceName + "\" does not exist!","WidgetFactory::getInstance");

			return mInstances[instanceName];
		}

		bool instanceExists(const Ogre::String& instanceName)
		{
			return (mInstances.find(instanceName) != mInstances.end());
		}

	protected:
		WidgetFactory() { mNumAutoNamedInstances = 0; }
		virtual ~WidgetFactory()
		{
    		typename std::map<Ogre::String, BaseClassType*>::iterator it;
			for(it = mInstances.begin(); it != mInstances.end(); ++it)
				OGRE_DELETE_T((*it).second,BaseClassType,Ogre::MEMCATEGORY_GENERAL);
		}

		std::map<Ogre::String, createObjectFunc> m_object_creator;
		std::map<Ogre::String, BaseClassType*> mInstances;

		unsigned int mNumAutoNamedInstances;

		template<typename BaseClassType1, typename ClassType>
		static BaseClassType1* createObject(const Ogre::String& param1)
		{
			BaseClassType1* newInstance = OGRE_NEW_T(ClassType,Ogre::MEMCATEGORY_GENERAL)(param1);

			newInstance->_createDescObject(newInstance->getClass() + "Desc");

			return newInstance;
		}
	};
}

#endif
