#include "QuickGUISerialWriter.h"
#include "QuickGUIScriptWriter.h"

template<> QuickGUI::SerialWriter* Ogre::Singleton<QuickGUI::SerialWriter>::ms_Singleton = 0;

namespace QuickGUI
{
	SerialWriter::SerialWriter() :
		SerialBase()
	{
	}

	SerialWriter* SerialWriter::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	SerialWriter& SerialWriter::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	void SerialWriter::_helperFunction(const Ogre::String& propertyName, const Ogre::String& value, const Ogre::String& defaultValue)
	{
		// If the value given is the same as the default value, no need to write it out
		if(value == defaultValue)
			return;

		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialWriter not setup to write!  Did you miss a call to SerialWriter::begin()?","SerialWriter::IO");

		DefinitionProperty* newProp = OGRE_NEW_T(DefinitionProperty,Ogre::MEMCATEGORY_GENERAL)(propertyName);
		newProp->mValues.push_back(value);
		
		if(mCurrentDefinition->mProperties.find(propertyName) != mCurrentDefinition->mProperties.end())
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition already contains the property \"" + propertyName + "\"!","SerialWriter::IO");
		else
			mCurrentDefinition->mProperties[propertyName] = newProp;
	}

	bool SerialWriter::begin(const Ogre::String& definitionType, const Ogre::String& definitionID)
	{
		ScriptDefinition* newDef = OGRE_NEW_T(ScriptDefinition,Ogre::MEMCATEGORY_GENERAL)(definitionType,definitionID);
		newDef->mParentDefinition = mCurrentDefinition;

		if(mCurrentDefinition != NULL)
		{
			if(mCurrentDefinition->mSubDefinitions[definitionType].find(definitionID) != mCurrentDefinition->mSubDefinitions[definitionType].end())
				throw Exception(Exception::ERR_SERIALIZATION,"Definition \"" + mCurrentDefinition->getType() + " " + mCurrentDefinition->getID() + "\" already contains a definition \"" + definitionType + " " + definitionID + "!","SerialWriter::begin");
			mCurrentDefinition->mSubDefinitions[definitionType][definitionID] = newDef;
			mCurrentDefinition->mSubDefinitionsInOrder.push_back(newDef);
		}

		mCurrentDefinition = newDef;

		return true;
	}

	void SerialWriter::end()
	{
		// If this is the root ScriptDefinition, add it to ScriptWriter
		if(mCurrentDefinition->mParentDefinition == NULL)
			ScriptWriter::getSingletonPtr()->addDefinition(mCurrentDefinition);
		
		mCurrentDefinition = mCurrentDefinition->mParentDefinition;
	}

	void SerialWriter::IO(const Ogre::String& propertyName, unsigned short* member, unsigned short defaultValue)
	{
		_helperFunction(propertyName,Ogre::StringConverter::toString(*member),Ogre::StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, BrushFilterMode* member, BrushFilterMode defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, ConsoleLayout* member, ConsoleLayout defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, int* member, int defaultValue)
	{
		_helperFunction(propertyName,Ogre::StringConverter::toString(*member),Ogre::StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, unsigned int* member, unsigned int defaultValue)
	{
		_helperFunction(propertyName,Ogre::StringConverter::toString(*member),Ogre::StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, ColourValue* member, ColourValue defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, VScrollBarButtonLayout* member, VScrollBarButtonLayout defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, HScrollBarButtonLayout* member, HScrollBarButtonLayout defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, ToolBarItemLayout* member, ToolBarItemLayout defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, Ogre::String* member, Ogre::String defaultValue)
	{
		_helperFunction(propertyName,*member,defaultValue);
	}

	void SerialWriter::IO(const Ogre::String& propertyName, bool* member, bool defaultValue)
	{
		_helperFunction(propertyName,Ogre::StringConverter::toString(*member),Ogre::StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, float* member, float defaultValue)
	{
		_helperFunction(propertyName,Ogre::StringConverter::toString(*member),Ogre::StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, Point* member, Point defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, ProgressBarFillDirection* member, ProgressBarFillDirection defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, ProgressBarLayout* member, ProgressBarLayout defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, ProgressBarClippingEdge* member, ProgressBarClippingEdge defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, Rect* member, Rect defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, Size* member, Size defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, HorizontalTextAlignment* member, HorizontalTextAlignment defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, HorizontalAnchor* member, HorizontalAnchor defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, VerticalAnchor* member, VerticalAnchor defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	void SerialWriter::IO(const Ogre::String& propertyName, VerticalTextAlignment* member, VerticalTextAlignment defaultValue)
	{
		_helperFunction(propertyName,StringConverter::toString(*member),StringConverter::toString(defaultValue));
	}

	bool SerialWriter::isSerialWriter()
	{
		return true;
	}

	bool SerialWriter::isSerialReader()
	{
		return false;
	}
}
