#ifndef QUICKGUIBUTTON_H
#define QUICKGUIBUTTON_H

#include "QuickGUILabel.h"

namespace QuickGUI
{
	class _QuickGUIExport ButtonDesc :
			public LabelDesc
	{
	public:
		ButtonDesc();

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ButtonDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Button"; }

		// Factory method
		static WidgetDesc* factory() { return new ButtonDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Button :
		public Label
	{
	public:
		// Skin Constants
		static const Ogre::String DEFAULT;
		static const Ogre::String OVER;
		static const Ogre::String DOWN;
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
		Button(const Ogre::String& name);
		virtual ~Button();

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_OVER)
		*/
		void onMouseEnter(const EventArgs& args);
		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_DEFAULT)
		*/
		void onMouseLeave(const EventArgs& args);
		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_DOWN)
		*/
		void onMouseLeftButtonDown(const EventArgs& args);
		/**
		* Handler defined to implement the button's default natural behavior. (BUTTON_STATE_OVER)
		*/
		void onMouseLeftButtonUp(const EventArgs& args);

	private:
	};
}

#endif
