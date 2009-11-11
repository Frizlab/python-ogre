/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

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
