#ifndef QUICKGUIMODALWINDOW_H
#define QUICKGUIMODALWINDOW_H

#include "QuickGUIWindow.h"

namespace QuickGUI
{
	// forward declaration
	class GUIManager;

	class _QuickGUIExport ModalWindowDesc :
			public WindowDesc
	{
	public:
		ModalWindowDesc();

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ModalWindowDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "ModalWindow"; }

		// Factory method
		static WidgetDesc* factory() { return new ModalWindowDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		//virtual void serialize(SerialBase* b);
	};


	class _QuickGUIExport ModalWindow :
		public Window
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class GUIManager;
		friend class Sheet;

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
		ModalWindow(const Ogre::String& name);
		virtual ~ModalWindow();

	private:
	};
}

#endif
