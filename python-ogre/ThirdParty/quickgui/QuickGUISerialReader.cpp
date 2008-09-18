#include "QuickGUISerialReader.h"
#include "QuickGUIScriptReader.h"
#include "QuickGUIManager.h"
#include "QuickGUIWidget.h"
#include "QuickGUIComponentWidget.h"
#include "QuickGUIContainerWidget.h"
#include "QuickGUIWidgetDescFactoryManager.h"
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

	void SerialReader::begin(const Ogre::String& definitionType, const Ogre::String& definitionID)
	{
		if(mCurrentDefinition != NULL)
			mCurrentDefinition = mCurrentDefinition->getDefinition(definitionType,definitionID);
		else
			mCurrentDefinition = ScriptReader::getSingleton().getDefinition(definitionType,definitionID);

		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"ScriptDefinition \"" + definitionType + " " + definitionID + "\" could not be found!","SerialReader::begin");
	}

	void SerialReader::end()
	{
		mCurrentDefinition = mCurrentDefinition->mParentDefinition;
	}

	void SerialReader::IO(const Ogre::String& propertyName, BrushFilterMode* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		if(sv.empty())
			(*member) = BRUSHFILTER_LINEAR;
		else
			(*member) = StringConverter::parseBrushFilterMode(sv[0]);
	}

	void SerialReader::IO(const Ogre::String& propertyName, int* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		if(sv.empty())
			(*member) = 0;
		else
			(*member) = Ogre::StringConverter::parseInt(sv[0]);
	}

	void SerialReader::IO(const Ogre::String& propertyName, unsigned int* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		if(sv.empty())
			(*member) = 0;
		else
			(*member) = Ogre::StringConverter::parseUnsignedInt(sv[0]);
	}

	void SerialReader::IO(const Ogre::String& propertyName, Ogre::ColourValue* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		if(sv.empty())
		{
			member->r = 1.0;
			member->g = 1.0;
			member->b = 1.0;
			member->a = 1.0;
		}
		else
		{
			member->r = Ogre::StringConverter::parseReal(sv[0]);
			member->g = Ogre::StringConverter::parseReal(sv[1]);
			member->b = Ogre::StringConverter::parseReal(sv[2]);
			member->a = Ogre::StringConverter::parseReal(sv[3]);
		}
	}

	void SerialReader::IO(const Ogre::String& propertyName, VScrollBarButtonLayout* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		if(sv.empty())
			(*member) =  VSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE;
		else
			(*member) = StringConverter::parseVScrollBarButtonLayout(sv[0]);
	}

	void SerialReader::IO(const Ogre::String& propertyName, HScrollBarButtonLayout* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		if(sv.empty())
			(*member) =  HSCROLL_BAR_BUTTON_LAYOUT_OPPOSITE;
		else
			(*member) = StringConverter::parseHScrollBarButtonLayout(sv[0]);
	}

	void SerialReader::IO(const Ogre::String& propertyName, ToolBarItemLayout* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		*member = StringConverter::parseToolBarItemLayout(sv[0]);
	}

	void SerialReader::IO(const Ogre::String& propertyName, Ogre::String* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		if(sv.empty())
			(*member) = "";
		else
			(*member) = sv[0];
	}

	void SerialReader::IO(const Ogre::String& propertyName, bool* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		if(sv.empty())
			(*member) = true;
		else
			(*member) = Ogre::StringConverter::parseBool(sv[0]);
	}

	void SerialReader::IO(const Ogre::String& propertyName, float* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		if(sv.empty())
			(*member) = 0.0;
		else
			(*member) = Ogre::StringConverter::parseReal(sv[0]);
	}

	void SerialReader::IO(const Ogre::String& propertyName, Point* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();
		
		if(sv.empty())
			(*member) = Point::ZERO;
		else
		{
			*member = Point(
				Ogre::StringConverter::parseReal(sv[0]),
				Ogre::StringConverter::parseReal(sv[1]));
		}
	}

	void SerialReader::IO(const Ogre::String& propertyName, ProgressBarFillDirection* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();
		
		if(sv.empty())
			(*member) = PROGRESSBAR_FILLS_NEGATIVE_TO_POSITIVE;
		else
			*member = StringConverter::parseProgressBarFillDirection(sv[0]);
	}

	void SerialReader::IO(const Ogre::String& propertyName, ProgressBarLayout* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();
		
		if(sv.empty())
			(*member) = PROGRESSBAR_LAYOUT_HORIZONTAL;
		else
			*member = StringConverter::parseProgressBarLayout(sv[0]);
	}

	void SerialReader::IO(const Ogre::String& propertyName, ProgressBarClippingEdge* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();
		
		if(sv.empty())
			(*member) = PROGRESSBAR_CLIP_LEFT_BOTTOM;
		else
			*member = StringConverter::parseProgressBarClippingEdge(sv[0]);
	}

	void SerialReader::IO(const Ogre::String& propertyName, Rect* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();
		
		if(sv.empty())
			(*member) = Rect(0,0,0,0);
		else
		{
			(*member) = Rect(
				Ogre::StringConverter::parseReal(sv[0]),
				Ogre::StringConverter::parseReal(sv[1]),
				Ogre::StringConverter::parseReal(sv[2]),
				Ogre::StringConverter::parseReal(sv[3]));
		}
	}

	void SerialReader::IO(const Ogre::String& propertyName, Size* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		if(sv.empty())
			(*member) = Size::ZERO;
		else
		{
			(*member) = Size(
				Ogre::StringConverter::parseReal(sv[0]),
				Ogre::StringConverter::parseReal(sv[1]));
		}
	}

	void SerialReader::IO(const Ogre::String& propertyName, TextAlignment* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		*member = StringConverter::parseTextAlignment(sv[0]);
	}

	void SerialReader::IO(const Ogre::String& propertyName, HorizontalAnchor* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		*member = StringConverter::parseHorizontalAnchor(sv[0]);
	}

	void SerialReader::IO(const Ogre::String& propertyName, VerticalAnchor* member)
	{
		if(mCurrentDefinition == NULL)
			throw Exception(Exception::ERR_SERIALIZATION,"SerialReader has not been correctly setup to retrieve properties. (Missing call to SerialReader::begin?)","SerialReader::IO");

		DefinitionProperty* prop = mCurrentDefinition->getProperty(propertyName);
		Ogre::StringVector sv = prop->getValues();

		*member = StringConverter::parseVerticalAnchor(sv[0]);
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
