#include "QuickGUISerialReader.h"
#include "QuickGUIScriptReader.h"
#include "QuickGUIManager.h"
#include "QuickGUIWidget.h"
#include "QuickGUIComponentWidget.h"
#include "QuickGUIContainerWidget.h"
#include "QuickGUISheet.h"

template<> QuickGUI::SerialReader* Ogre::Singleton<QuickGUI::SerialReader>::ms_Singleton = 0;

namespace QuickGUI
{
	SerialReader::SerialReader() :
		SerialBase()
	{
	}

	SerialReader* SerialReader::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	SerialReader& SerialReader::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	Ogre::String SerialReader::_helperFunction(const Ogre::String& propertyName, const Ogre::String& defaultValue)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);

		if(prop == NULL)
			return defaultValue;
		else
		{
			Ogre::String retVal = "";
			Ogre::StringVector sv = prop->getValues();
			for(int i = 0; i < static_cast<int>(sv.size()); ++i)
				retVal += sv[i] + " ";

			Ogre::StringUtil::trim(retVal);

			return retVal;
		}
	}

	bool SerialReader::begin(const Ogre::String& definitionType, const Ogre::String& definitionID)
	{
		ScriptDefinition* d = NULL;

		if(mCurrentDefinition != NULL)
			d = mCurrentDefinition->getDefinition(definitionType,definitionID);
		else
			d = ScriptReader::getSingleton().getDefinition(definitionType,definitionID);

		if(d == NULL)
			return false;

		mCurrentDefinition = d;

		return true;
	}

	void SerialReader::end()
	{
		mCurrentDefinition = mCurrentDefinition->mParentDefinition;
	}

	void SerialReader::IO(const Ogre::String& propertyName, unsigned short* member, unsigned short defaultValue)
	{
		(*member) = Ogre::StringConverter::parseUnsignedInt(_helperFunction(propertyName,Ogre::StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, BrushFilterMode* member, BrushFilterMode defaultValue)
	{
		(*member) = StringConverter::parseBrushFilterMode(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, ConsoleLayout* member, ConsoleLayout defaultValue)
	{
		(*member) = StringConverter::parseConsoleLayout(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, int* member, int defaultValue)
	{
		(*member) = Ogre::StringConverter::parseInt(_helperFunction(propertyName,Ogre::StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, unsigned int* member, unsigned int defaultValue)
	{
		(*member) = Ogre::StringConverter::parseUnsignedInt(_helperFunction(propertyName,Ogre::StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, ColourValue* member, ColourValue defaultValue)
	{
		(*member) = StringConverter::parseColourValue(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, VScrollBarButtonLayout* member, VScrollBarButtonLayout defaultValue)
	{
		(*member) = StringConverter::parseVScrollBarButtonLayout(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, HScrollBarButtonLayout* member, HScrollBarButtonLayout defaultValue)
	{
		(*member) = StringConverter::parseHScrollBarButtonLayout(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, ToolBarItemLayout* member, ToolBarItemLayout defaultValue)
	{
		(*member) = StringConverter::parseToolBarItemLayout(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, Ogre::String* member, Ogre::String defaultValue)
	{
		(*member) = _helperFunction(propertyName,defaultValue);
	}

	void SerialReader::IO(const Ogre::String& propertyName, bool* member, bool defaultValue)
	{
		(*member) = Ogre::StringConverter::parseBool(_helperFunction(propertyName,Ogre::StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, float* member, float defaultValue)
	{
		(*member) = Ogre::StringConverter::parseReal(_helperFunction(propertyName,Ogre::StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, Point* member, Point defaultValue)
	{
		(*member) = StringConverter::parsePoint(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, ProgressBarFillDirection* member, ProgressBarFillDirection defaultValue)
	{
		(*member) = StringConverter::parseProgressBarFillDirection(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, ProgressBarLayout* member, ProgressBarLayout defaultValue)
	{
		(*member) = StringConverter::parseProgressBarLayout(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, ProgressBarClippingEdge* member, ProgressBarClippingEdge defaultValue)
	{
		(*member) = StringConverter::parseProgressBarClippingEdge(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, Rect* member, Rect defaultValue)
	{
		(*member) = StringConverter::parseRect(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, Size* member, Size defaultValue)
	{
		(*member) = StringConverter::parseSize(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, HorizontalTextAlignment* member, HorizontalTextAlignment defaultValue)
	{
		(*member) = StringConverter::parseHorizontalTextAlignment(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, HorizontalAnchor* member, HorizontalAnchor defaultValue)
	{
		(*member) = StringConverter::parseHorizontalAnchor(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, VerticalAnchor* member, VerticalAnchor defaultValue)
	{
		(*member) = StringConverter::parseVerticalAnchor(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	void SerialReader::IO(const Ogre::String& propertyName, VerticalTextAlignment* member, VerticalTextAlignment defaultValue)
	{
		(*member) = StringConverter::parseVerticalTextAlignment(_helperFunction(propertyName,StringConverter::toString(defaultValue)));
	}

	bool SerialReader::isSerialWriter()
	{
		return false;
	}

	bool SerialReader::isSerialReader()
	{
		return true;
	}
}
