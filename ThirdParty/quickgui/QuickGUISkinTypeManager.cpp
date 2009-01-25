#include "QuickGUISkinTypeManager.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIScriptReader.h"
#include "QuickGUIScriptWriter.h"
#include "QuickGUISerialReader.h"
#include "QuickGUISerialWriter.h"

#include "OgreLogManager.h"

#include <list>

template<> QuickGUI::SkinTypeManager* Ogre::Singleton<QuickGUI::SkinTypeManager>::ms_Singleton = 0;

namespace QuickGUI
{
	SkinTypeManager::SkinTypeManager()
	{
		if(SkinDefinitionManager::getSingletonPtr() == NULL)
			throw Exception(Exception::ERR_SKINNING,"SkinDefinitionManager must be created prior to SkinTypeManager!","SkinTypeManager::SkinTypeManager");
	}

	SkinTypeManager::~SkinTypeManager()
	{
		for(std::map<Ogre::String, std::map<Ogre::String,SkinType*> >::iterator it1 = mSkinTypes.begin(); it1 != mSkinTypes.end(); ++it1)
		{
			for(std::map<Ogre::String,SkinType*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
				OGRE_DELETE_T((*it2).second,SkinType,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	SkinTypeManager* SkinTypeManager::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	SkinTypeManager& SkinTypeManager::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	void SkinTypeManager::addSkinType(const Ogre::String& className, const Ogre::String& typeName, SkinType* t)
	{
		if(hasSkinType(className,typeName))
			throw Exception(Exception::ERR_SKINNING,"SkinType already exists for class \"" + className + "\" and type \"" + typeName + "\"!","SkinTypeManager::addSkinType");

		if(!SkinDefinitionManager::getSingleton().hasSkinDefinition(className))
			throw Exception(Exception::ERR_SKINNING,"A SkinClass definition for class \"" + className + "\" does not exist.  It must be added before any SkinTypes can be added!","SkinTypeManager::addSkinType");

		if(!SkinDefinitionManager::getSingleton().getSkinDefinition(className)->validateSkinType(t))
			throw Exception(Exception::ERR_SKINNING,"SkinType of class \"" + className + "\" and type \"" + typeName + "\" does not follow the SkinDefinition for class \"" + className + "\".","SkinTypeManager::addSkinType");

		mSkinTypes[className][typeName] = t;
	}

	SkinType* SkinTypeManager::getSkinType(const Ogre::String& className, const Ogre::String& typeName)
	{
		if(!hasSkinType(className,typeName))
			throw Exception(Exception::ERR_SKINNING,"SkinType does not exist for class \"" + className + "\" and type \"" + typeName + "\"!","SkinTypeManager::getSkinType");

		return mSkinTypes[className][typeName];
	}

	bool SkinTypeManager::hasSkinType(const Ogre::String& className, const Ogre::String& typeName)
	{
		if(mSkinTypes.find(className) == mSkinTypes.end())
			return false;

		if(mSkinTypes[className].find(typeName) == mSkinTypes[className].end())
			return false;

		return true;
	}

	void SkinTypeManager::loadTypes()
	{
		Ogre::Log* defaultLog = Ogre::LogManager::getSingletonPtr()->getDefaultLog();
		if(defaultLog != NULL)
			defaultLog->logMessage("[QGUI] Loading SkinClass and SkinType definitions...");

		SerialReader* sr = SerialReader::getSingletonPtr();
		std::list<ScriptDefinition*> defList = ScriptReader::getSingleton().getDefinitions("SkinClass");

		for(std::list<ScriptDefinition*>::iterator it = defList.begin(); it != defList.end(); ++it)
		{
			Ogre::String className = (*it)->getID();

			if(defaultLog != NULL)
				defaultLog->logMessage("[QGUI] SkinClass definition for class \"" + className + "\" Found.  Looking for SkinTypes...");

			sr->begin("SkinClass",className);
			
			std::list<ScriptDefinition*> types = (*it)->getDefinitions();
			for(std::list<ScriptDefinition*>::iterator typeItr = types.begin(); typeItr != types.end(); ++typeItr)
			{
				SkinType* newType = OGRE_NEW_T(SkinType,Ogre::MEMCATEGORY_GENERAL)(className,(*typeItr)->getID());
				newType->serialize(SerialReader::getSingletonPtr());

				if(defaultLog != NULL)
					defaultLog->logMessage("[QGUI] Adding SkinType definition \"" + newType->getName() + "\"...");

				addSkinType(className,(*typeItr)->getID(),newType);
			}

			sr->end();
		}
	}

	void SkinTypeManager::saveTypesToFile(const Ogre::String& fileName)
	{
		ScriptWriter::getSingletonPtr()->begin(fileName);

		SerialWriter* sw = SerialWriter::getSingletonPtr();
		for(std::map<Ogre::String, std::map<Ogre::String,SkinType*> >::iterator it1 = mSkinTypes.begin(); it1 != mSkinTypes.end(); ++it1)
		{
			sw->begin("SkinClass",(*it1).first);
			for(std::map<Ogre::String,SkinType*>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
			{
				(*it2).second->serialize(sw);
			}
			sw->end();
		}

		ScriptWriter::getSingletonPtr()->end();
	}
}
