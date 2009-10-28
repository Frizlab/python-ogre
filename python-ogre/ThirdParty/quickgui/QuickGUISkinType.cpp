#include "QuickGUISkinType.h"
#include "QuickGUIScriptDefinition.h"

namespace QuickGUI
{
	SkinType::SkinType(const Ogre::String& className, const Ogre::String& typeName) :
		mClassName(className),
		mName(typeName)
	{
	}

	SkinType::~SkinType()
	{
		for(std::map<Ogre::String,SkinElement*>::iterator it = mSkinElements.begin(); it != mSkinElements.end(); ++it)
			OGRE_DELETE_T((*it).second,SkinElement,Ogre::MEMCATEGORY_GENERAL);

		for(std::map<Ogre::String,SkinReference*>::iterator it = mSkinReferences.begin(); it != mSkinReferences.end(); ++it)
			OGRE_DELETE_T((*it).second,SkinReference,Ogre::MEMCATEGORY_GENERAL);
	}

	void SkinType::addSkinReference(SkinReference* t)
	{
		if(t == NULL)
			return;

		if(hasSkinReference(t->skinAlias,t->className))
			throw Exception(Exception::ERR_SKINNING,"SkinReference for alias \"" + t->skinAlias + "\" already exists!","SkinType::addSkinReference");

		mSkinReferences[t->skinAlias] = t;
	}

	void SkinType::addSkinElement(SkinElement* e)
	{
		if(e == NULL)
			return;

		if(hasSkinElement(e->getName()))
			throw Exception(Exception::ERR_SKINNING,"SkinElement \"" + e->getName() + "\" already exists!","SkinType::addSkinElement");

		mSkinElements[e->getName()] = e;
	}

	Ogre::String SkinType::getClassName()
	{
		return mClassName;
	}

	SkinReference* SkinType::getSkinReference(const Ogre::String& skinRefID)
	{
		if(!hasSkinReference(skinRefID))
			throw Exception(Exception::ERR_SKINNING,"SkinReference with alias \"" + skinRefID + "\" does not exist!","SkinType::getSkinReference");

		return mSkinReferences[skinRefID];
	}

	Ogre::String SkinType::getName()
	{
		return mName;
	}

	SkinElement* SkinType::getSkinElement(const Ogre::String& elementName)
	{
		if(!hasSkinElement(elementName))
			throw Exception(Exception::ERR_SKINNING,"SkinElement \"" + elementName + "\" does not exist!","SkinType::getSkinElement");

		return mSkinElements[elementName];
	}

	bool SkinType::hasSkinReference(const Ogre::String& skinRefID, const Ogre::String& className)
	{
		std::map<Ogre::String,SkinReference*>::iterator it = mSkinReferences.find(skinRefID);
		if(it == mSkinReferences.end())
			return false;

		if((*it).second->className != className)
			return false;

		return true;
	}

	bool SkinType::hasSkinReference(const Ogre::String& skinRefID)
	{
		return (mSkinReferences.find(skinRefID) != mSkinReferences.end());
	}

	bool SkinType::hasSkinElement(const Ogre::String& elementName)
	{
		return (mSkinElements.find(elementName) != mSkinElements.end());
	}

	void SkinType::serialize(SerialBase* b)
	{
		if(b->isSerialReader())
		{
			mName = b->getCurrentDefinition()->getID();
			mClassName = b->getCurrentDefinition()->getType();
			
			std::list<ScriptDefinition*> defList = b->getCurrentDefinition()->getDefinitions("SkinReference");
			for(std::list<ScriptDefinition*>::iterator it = defList.begin(); it != defList.end(); ++it)
			{
				SkinReference* newSkinReference = OGRE_NEW_T(SkinReference,Ogre::MEMCATEGORY_GENERAL)((*it)->getID());
				newSkinReference->serialize(b);
				addSkinReference(newSkinReference);
			}

			defList = b->getCurrentDefinition()->getDefinitions("SkinElement");
			for(std::list<ScriptDefinition*>::iterator it = defList.begin(); it != defList.end(); ++it)
			{
				SkinElement* newSkinElement = OGRE_NEW_T(SkinElement,Ogre::MEMCATEGORY_GENERAL)((*it)->getID());
				newSkinElement->serialize(b);
				addSkinElement(newSkinElement);
			}			
		}
		else
		{
			b->begin(mClassName,mName);

			for(std::map<Ogre::String,SkinReference*>::iterator it = mSkinReferences.begin(); it != mSkinReferences.end(); ++it)
				(*it).second->serialize(b);

			for(std::map<Ogre::String,SkinElement*>::iterator it = mSkinElements.begin(); it != mSkinElements.end(); ++it)
				(*it).second->serialize(b);

			b->end();
		}
	}
}
