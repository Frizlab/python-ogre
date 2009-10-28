#ifndef QUICKGUISERIALBASE_H
#define QUICKGUISERIALBASE_H

#include "QuickGUIAnchors.h"
#include "QuickGUIBrushEnums.h"
#include "QuickGUIConsoleEnums.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIProgressBarEnums.h"
#include "QuickGUIRect.h"
#include "QuickGUIEventTypes.h"
#include "QuickGUIScrollBarButtonLayouts.h"
#include "QuickGUITextAlignment.h"
#include "QuickGUIToolBarEnums.h"
#include "QuickGUIOgreEquivalents.h"

#include "OgrePrerequisites.h"

namespace QuickGUI
{
	// forward declaration
	class ScriptDefinition;

	/**
	* Abstract class setting the foundation for the 
	* SerialWriter and SerialReader class.
	*/
	class _QuickGUIExport SerialBase
	{
	public:
		SerialBase();

		virtual bool begin(const Ogre::String& definitionType, const Ogre::String& definitionID) = 0;

		virtual void end() = 0;

		ScriptDefinition* getCurrentDefinition();

		virtual bool isSerialWriter() = 0;
		virtual bool isSerialReader() = 0;
		// Serialization of properties
		virtual void IO(const Ogre::String& propertyName, unsigned short* member, unsigned short defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, BrushFilterMode* member, BrushFilterMode defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, ConsoleLayout* member, ConsoleLayout defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, int* member, int defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, unsigned int* member, unsigned int defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, ColourValue* member, ColourValue defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, ToolBarItemLayout* member, ToolBarItemLayout defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, Ogre::String* member, Ogre::String defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, bool* member, bool defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, float* member, float defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, Point* member, Point defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, ProgressBarFillDirection* member, ProgressBarFillDirection defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, ProgressBarLayout* member, ProgressBarLayout defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, ProgressBarClippingEdge* member, ProgressBarClippingEdge defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, Rect* member, Rect defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, VScrollBarButtonLayout* member, VScrollBarButtonLayout defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, HScrollBarButtonLayout* member, HScrollBarButtonLayout defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, Size* member, Size defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, HorizontalTextAlignment* member, HorizontalTextAlignment defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, HorizontalAnchor* member, HorizontalAnchor defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, VerticalAnchor* member, VerticalAnchor defaultValue) = 0;
		virtual void IO(const Ogre::String& propertyName, VerticalTextAlignment* member, VerticalTextAlignment defaultValue) = 0;

	protected:
		ScriptDefinition* mCurrentDefinition;

	private:
	};
}

#endif
