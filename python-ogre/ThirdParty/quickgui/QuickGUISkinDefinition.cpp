#include "QuickGUISkinDefinition.h"
#include "QuickGUIFactoryManager.h"
#include "QuickGUIWidgetFactory.h"

namespace QuickGUI
{
	SkinDefinition::SkinDefinition(const std::string& className) :
		mClassName(className),
		mDefinitionCompleted(false)
	{
	}

	void SkinDefinition::defineSkinReference(const std::string& refName, const std::string& widgetClass)
	{
		if(mDefinitionCompleted)
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition has already been finalized, cannot add more references!","SkinDefinition::defineSkinReference");

		if(mDefinedReferences.find(refName) != mDefinedReferences.end())
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition already contains reference for Component \"" + refName + "\"!","SkinDefinition::defineSkinReference");

		if(!FactoryManager::getSingletonPtr()->getWidgetFactory()->classRegistered(widgetClass))
			throw Exception(Exception::ERR_SKINNING,"Widget Class \"" + widgetClass + "\" is not recognized! Did you forget to register it with the Widget Factory?","SkinDefinition::defineSkinReference");

		mDefinedReferences[refName] = widgetClass;
	}

	void SkinDefinition::defineSkinElement(const std::string& elementName)
	{
		if(mDefinitionCompleted)
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition has already been finalized, cannot add more references!","SkinDefinition::defineSkinElement");

		if(mDefinedElements.find(elementName) != mDefinedElements.end())
			throw Exception(Exception::ERR_SKINNING,"SkinDefinition already contains reference for SkinElement \"" + elementName + "\"!","SkinDefinition::defineSkinElement");

		mDefinedElements.insert(elementName);
	}

	void SkinDefinition::definitionComplete()
	{
		mDefinitionCompleted = true;
	}

	bool SkinDefinition::definitionCompleted()
	{
		return mDefinitionCompleted;
	}

	bool SkinDefinition::validateSkinType(SkinType* t)
	{
		for(std::set<std::string>::iterator it = mDefinedElements.begin(); it != mDefinedElements.end(); ++it)
		{
			if(!t->hasSkinElement((*it)))
				return false;
		}

		for(std::map<std::string,std::string>::iterator it = mDefinedReferences.begin(); it != mDefinedReferences.end(); ++it)
		{
			if(!t->hasSkinReference((*it).first,(*it).second))
				return false;
		}

		return true;
	}
}
