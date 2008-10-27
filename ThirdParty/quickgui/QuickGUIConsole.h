#ifndef QUICKGUICONSOLE_H
#define QUICKGUICONSOLE_H

#include "QuickGUIComponentWidget.h"
#include "QuickGUITextArea.h"
#include "QuickGUITextBox.h"

namespace QuickGUI
{
	class _QuickGUIExport ConsoleDesc :
			public ComponentWidgetDesc
	{
	public:
		ConsoleDesc();

		TextBoxDesc textBoxDesc;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ConsoleDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Console"; }

		// Factory method
		static WidgetDesc* factory() { return new ConsoleDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Console :
		public ComponentWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String TEXTBOX;
		static const Ogre::String TEXTAREA;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// Factory method
		static Widget* factory(const Ogre::String& widgetName);
	public:
		
		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

	protected:
		Console(const Ogre::String& name);
		virtual ~Console();

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
	};
}

#endif
