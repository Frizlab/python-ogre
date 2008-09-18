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
			delete (*it).second;

		for(std::map<Ogre::String,ComponentType*>::iterator it = mComponentTypes.begin(); it != mComponentTypes.end(); ++it)
			delete (*it).second;
	}

	void SkinType::addComponentType(const Ogre::String& componentAlias, ComponentType* t)
	{
		if(hasComponentType(componentAlias))
			throw Exception(Exception::ERR_SKINNING,"ComponentType for alias \"" + componentAlias + "\" already exists!","SkinType::addComponentType");

		mComponentTypes[componentAlias] = t;
	}

	void SkinType::addSkinElement(const Ogre::String& elementName, SkinElement* e)
	{
		if(hasSkinElement(elementName))
			throw Exception(Exception::ERR_SKINNING,"SkinElement \"" + elementName + "\" already exists!","SkinType::addSkinElement");

		mSkinElements[elementName] = e;
	}

	Ogre::String SkinType::getClassName()
	{
		return mClassName;
	}

	ComponentType* SkinType::getComponentType(const Ogre::String& componentAlias)
	{
		if(!hasComponentType(componentAlias))
			throw Exception(Exception::ERR_SKINNING,"ComponentType with alias \"" + componentAlias + "\" does not exist!","SkinType::getComponentType");

		return mComponentTypes[componentAlias];
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

	bool SkinType::hasComponentType(const Ogre::String& componentAlias)
	{
		return (mComponentTypes.find(componentAlias) != mComponentTypes.end());
	}

	bool SkinType::hasSkinElement(const Ogre::String& elementName)
	{
		return (mSkinElements.find(elementName) != mSkinElements.end());
	}

	void SkinType::serialize(SerialBase* b)
	{
		b->begin("SkinType",mName);

		if(b->isSerialReader())
		{
			std::list<ScriptDefinition*> defList = b->getCurrentDefinition()->getDefinitions("ComponentType");
			for(std::list<ScriptDefinition*>::iterator it = defList.begin(); it != defList.end(); ++it)
			{
				ComponentType* newComponentType = new ComponentType((*it)->getID());
				newComponentType->serialize(b);
				addComponentType((*it)->getID(),newComponentType);
			}

			defList = b->getCurrentDefinition()->getDefinitions("SkinElement");
			for(std::list<ScriptDefinition*>::iterator it = defList.begin(); it != defList.end(); ++it)
			{
				SkinElement* newSkinElement = new SkinElement((*it)->getID());
				newSkinElement->serialize(b);
				addSkinElement((*it)->getID(),newSkinElement);
			}			
		}
		else
		{
			for(std::map<Ogre::String,ComponentType*>::iterator it = mComponentTypes.begin(); it != mComponentTypes.end(); ++it)
				(*it).second->serialize(b);

			for(std::map<Ogre::String,SkinElement*>::iterator it = mSkinElements.begin(); it != mSkinElements.end(); ++it)
				(*it).second->serialize(b);
		}

		b->end();
	}
}
