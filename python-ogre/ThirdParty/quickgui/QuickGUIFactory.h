#ifndef QUICKGUIFACTORY_H
#define QUICKGUIFACTORY_H

#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"

#include "OgrePrerequisites.h"
#include "OgreStringConverter.h"

#include <ctype.h>

namespace QuickGUI
{
	template<typename BaseClassType>
	class _QuickGUIExport Factory
	{
	public:
		// Only FactoryManager can create/destroy Factories
		friend class FactoryManager;
	protected:
		typedef BaseClassType* (Factory::*createObjectFunc)(const Ogre::String&);

	public:
		template<typename ClassType>
		bool registerClass(const Ogre::String& className)
		{
			if (m_object_creator.find(className) != m_object_creator.end())
			return false;
            #if !defined ( __PYTHONOGRE_BUILD_CODE )
			    m_object_creator[className] = &Factory::createObject<ClassType>;
            #endif
			return true;
		}

		bool unregisterClass(const Ogre::String& className)
		{
			return (m_object_creator.erase(className) == 1);
		}

		template<typename ClassType>
		ClassType* createInstance(const Ogre::String& className, const Ogre::String& name)
		{
			typename std::map<Ogre::String, createObjectFunc>::iterator iter = m_object_creator.find(className);

			if (iter == m_object_creator.end())
				throw Exception(Exception::ERR_FACTORY,"\"" + className + "\" is not a registered class!","Factory::createInstance");

			Ogre::String instanceName = resolveName(name);

			if((name != "") && (instanceName != name))
			{
				Ogre::Log* defaultLog = Ogre::LogManager::getSingletonPtr()->getDefaultLog();
				if(defaultLog != NULL)
					defaultLog->logMessage("[QGUI] Factory::createInstance: Provided name \"" + name + "\" was taken.  Instance was created with name \"" + instanceName + "\".");
			}

			BaseClassType* newInstance = (this->*(*iter).second)(instanceName);
			mInstances[instanceName] = newInstance;

			if(name == "")
				++mNumAutoNamedInstances;

			return dynamic_cast<ClassType*>(newInstance);
		}

		template<typename ClassType>
		ClassType* createInstance(const Ogre::String& className)
		{
			return createInstance<ClassType>(className,"");
		}

		bool destroyInstance(const Ogre::String& instanceName)
		{
			if(mInDestructor)
				return false;

			if(!instanceExists(instanceName))
				throw Exception(Exception::ERR_FACTORY,"Instance with name \"" + instanceName + "\" does not exist!","Factory::getInstance");

			OGRE_DELETE_T(mInstances[instanceName],BaseClassType,Ogre::MEMCATEGORY_GENERAL);
			mInstances.erase(instanceName);

			return true;
		}

		bool destroyInstance(BaseClassType* instance)
		{
			if(mInDestructor)
				return false;

			for(typename std::map<Ogre::String, BaseClassType*>::iterator it = mInstances.begin(); it != mInstances.end(); ++it)
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

		template<typename ClassType>
		ClassType* getInstance(const Ogre::String& instanceName)
		{
			if(!instanceExists(instanceName))
				throw Exception(Exception::ERR_FACTORY,"Instance with name \"" + instanceName + "\" does not exist!","Factory::getInstance");

			return dynamic_cast<ClassType*>(mInstances[instanceName]);
		}

		bool instanceExists(const Ogre::String& instanceName)
		{
			return (mInstances.find(instanceName) != mInstances.end());
		}

		Ogre::String resolveName(const Ogre::String& name)
		{
			if(name != "")
			{
				if(instanceExists(name))
				{
					int strLen = static_cast<int>(name.size());

					int counter = strLen - 1;
					while(isdigit(name[counter]))
						--counter;

					if(counter == (strLen - 1))
						return (name + Ogre::StringConverter::toString(1));
					else
					{
						int n = atoi(name.substr(counter + 1).c_str());
						
						return (name.substr(0,counter) + Ogre::StringConverter::toString(n));
					}
				}
				else
					return name;
			}

			return Ogre::StringConverter::toString(mNumAutoNamedInstances);
		}

		Ogre::String updateName(BaseClassType* instance, const Ogre::String& name)
		{
			typename std::map<Ogre::String, BaseClassType*>::iterator it;
			for(it = mInstances.begin(); it != mInstances.end(); ++it)
			{
				if((*it).second == instance)
					break;
			}

			if(it == mInstances.end())
				throw Exception(Exception::ERR_FACTORY,"Object is not an object created by this factory!","Factory::updateName");

			mInstances.erase(it);

			Ogre::String newInstanceName = resolveName(name);

			if((name != "") && (newInstanceName != name))
			{
				Ogre::Log* defaultLog = Ogre::LogManager::getSingletonPtr()->getDefaultLog();
				if(defaultLog != NULL)
					defaultLog->logMessage("[QGUI] Factory::createInstance: Provided name \"" + name + "\" was taken.  Instance was created with name \"" + newInstanceName + "\".");
			}

			mInstances[newInstanceName] = instance;

			if(name == "")
				++mNumAutoNamedInstances;

			return newInstanceName;
		}

	protected:
		Factory() : mNumAutoNamedInstances(0), mInDestructor(false) {}
		virtual ~Factory()
		{
			mInDestructor = true;

			for(typename std::map<Ogre::String, BaseClassType*>::iterator it = mInstances.begin(); it != mInstances.end(); ++it)
				OGRE_DELETE_T((*it).second,BaseClassType,Ogre::MEMCATEGORY_GENERAL);
		}

		std::map<Ogre::String, createObjectFunc> m_object_creator;
		std::map<Ogre::String, BaseClassType*> mInstances;

		unsigned int mNumAutoNamedInstances;

		// This flag is used to protect Objects from Deleting Factory Objects while the Factory is destroying all Objects
		// in its destructor.
		bool mInDestructor;

		template<typename ClassType>
		BaseClassType* createObject(const Ogre::String& param1)
		{
			return OGRE_NEW_T(ClassType,Ogre::MEMCATEGORY_GENERAL)(param1);
		}
	};
}

#endif
