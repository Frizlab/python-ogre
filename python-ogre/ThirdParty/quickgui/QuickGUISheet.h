#ifndef QUICKGUISHEET_H
#define QUICKGUISHEET_H

#include "QuickGUIException.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUIModalWindow.h"
#include "QuickGUISerialReader.h"
#include "QuickGUISerialWriter.h"
#include "QuickGUIWindow.h"

#include <list>

namespace QuickGUI
{
	// forward declaration
	class GUIManager;

	class _QuickGUIExport SheetDesc :
			public WindowDesc
	{
	public:
		SheetDesc();

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "SheetDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Sheet"; }

		// Factory method
		static WidgetDesc* factory() { return new SheetDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);

	protected:
		HorizontalAnchor horizontalAnchor;
		VerticalAnchor verticalAnchor;
		bool dragable;
		Size maxSize;
		Size minSize;
		bool visible;

		// From WindowDesc, but not for use in Sheets
		bool titleBar;
		bool titleBarCloseButton;
		Ogre::String titleBarType;
	};

	class _QuickGUIExport Sheet :
		public Window
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class GUIManager;
		friend class SerialReader;

		// Factory method
		static Widget* factory(const Ogre::String& widgetName);
	public:
		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		void bringToFront(Window* w);

		ModalWindow* createModalWindow(ModalWindowDesc& d);
		MenuPanel* createMenuPanel(MenuPanelDesc& d);
		Window* createWindow(WindowDesc& d);

		void destroyMenuPanel(const Ogre::String& name);
		void destroyMenuPanel(MenuPanel* p);
		void destroyModalWindow(const Ogre::String& name);
		void destroyModalWindow(ModalWindow* w);
		void destroyWindow(const Ogre::String& name);
		void destroyWindow(Window* w);

		/**
		* Recursively searches through children and returns first widget found with name given.
		* NULL is returned if the widget is not found.
		*/
		virtual Widget* findWidget(const Ogre::String& name);
		/**
		* Returns the window at point p.  If ignoreDisabled is set to true,
		* disabled windows will be ignored during the search.
		*/
		Window* findWindowAtPoint(const Point& p, bool ignoreDisabled = true);
		/**
		* Brings the window to the front and fires WIDGET_EVENT_FOCUS_LOST and
		* WIDGET_EVENT_FOCUS_GAINED for previous and currently focused windows.
		* Returns true if any events are fired, false otherwise.
		*/
		bool focusWindow(Window* w);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the widget that should receive keyboard events;
		*/
		Widget* getKeyboardListener();
		MenuPanel* getMenuPanel(const Ogre::String& name);
		ModalWindow* getModalWindow(const Ogre::String& name);
		Window* getWindow(const Ogre::String& name);
		Window* getWindowInFocus();

		/**
		* Returns true if this sheet has a child menu panel with the name give, false
		* otherwise.
		*/
		bool hasMenuPanel(const Ogre::String& name);
		/**
		* Returns true if this sheet has a child modal window with the name given, false
		* otherwise.
		*/
		bool hasModalWindow(const Ogre::String& name);
		/**
		* Returns true if this sheet has a child window with the name given, false
		* otherwise.
		*/
		bool hasWindow(const Ogre::String& name);

		/**
		* Serializes this widget, including its properties, to disk.
		*/
		virtual void saveToDisk(const Ogre::String fileName);
		/**
		* Outlines how the desc class is written to script and read from script.
		*/
		virtual void serialize(SerialBase* b);
		/**
		* Notifies the Sheet of the Widget that should receive keyboard events.
		*/
		void setKeyboardListener(Widget* w);

	protected:
		Sheet(const Ogre::String& name);
		~Sheet();

		std::list<ModalWindow*> mModalWindows;
		std::list<Window*> mWindows;

		Window* mWindowInFocus;
		Widget* mKeyboardListener;

		/**
		* Internal function to add a window. (also used by SerialReader class)
		*/
		void addWindow(Window* w);

		/**
		* Internal function to add a modal window. (also used by SerialReader class)
		*/
		void addModalWindow(ModalWindow* w);

		/**
		* Offsets the widget.
		*/
		void drag(unsigned int xOffset, unsigned int yOffset);

		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();

	private:
		
	};
}

#endif
