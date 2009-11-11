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

#ifndef QUICKGUISHEET_H
#define QUICKGUISHEET_H

#include "QuickGUIException.h"
#include "QuickGUIWindow.h"
#include "QuickGUISerialReader.h"
#include "QuickGUISerialWriter.h"

#include <list>

namespace QuickGUIEditor
{
	// forward declarations
	class MainForm;
}

namespace QuickGUI
{
	// forward declaration
	class GUIManager;
	class ModalWindow;
	class ContextMenu;

	class ModalWindowDesc;
	class ContextMenuDesc;

	class _QuickGUIExport SheetDesc :
			public WindowDesc
	{
	public:
		friend class DescFactory;
	protected:
		SheetDesc();
		virtual ~SheetDesc() {}
	public:

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "SheetDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Sheet"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);

	public:

		bool sheet_mouseCursorEnabled;
		unsigned int sheet_mouseCursorQueryFilter;
		std::string sheet_mouseCursorSkinTypeName;
		bool sheet_mouseCursorVisible;

	protected:
		HorizontalAnchor widget_horizontalAnchor;
		VerticalAnchor widget_verticalAnchor;
		bool widget_dragable;
		Size widget_maxSize;
		Size widget_minSize;
		bool widget_visible;

		// From WindowDesc, but not for use in Sheets
		bool window_titleBar;
		bool window_titleBarCloseButton;
		Ogre::String window_titleBarSkinType;
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
		// Creates/Destroys Sheets
		friend class SheetManager;
		// GUIManager calls protected 'draw' function
		friend class GUIManager;
		friend class SerialReader;
		// Menu class creates a MenuPanel which is a Window, and needs to be able to add/remove them.
		friend class Menu;
		// ComboBox class creates a ListPanel which is a Window, and needs to be able to add/remove them.
		friend class ComboBox;
		// Widgets that are components of more complex Widgets should not have their name registered with the Sheet
		friend class ComponentWidget;
		// Needs access to free list
		friend class ContainerWidget;
		friend class ContextMenu;
		friend class List;
		friend class ListItem;
		friend class MenuItem;
		friend class TabControl;
		friend class ToolBarItem;
		// Widget::setName needs to update Sheet's name map, when Widget's are renamed.
		friend class Widget;
		// Window::setName '	'	'
		friend class Window;
		// Editor needs to be able to create Sheets
		friend class QuickGUIEditor::MainForm;
		// Root sets mKeyboardListener to NULL directly
		friend class Root;
	public:

		/**
		* Notifies the Widget of its GUIManager. (for component/container widgets this function is recursive)
		*/
		virtual void _setGUIManager(GUIManager* gm);
		/**
		* Notifies the Widget of its Sheet. (for component/container widgets this function is recursvie)
		*/
		virtual void _setSheet(Sheet* sheet);

		/** Adds an event handler to this widget
			@param
				EVENT Defined widget events, for example: WINDOW_EVENT_FOCUS_LOST, WINDOW_EVENT_FOCUS_GAINED, etc.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addWidgetEventHandler(QuickGUI::WINDOW_EVENT_FOCUS_LOST,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addWidgetEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addSheetEventHandler(SheetEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			addSheetEventHandler(EVENT, OGRE_NEW_T(EventHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(function,obj));
		}
		void addSheetEventHandler(SheetEvent EVENT, EventHandlerSlot* function);

		/**
		* Internal function to add a window. (also used by SerialReader class)
		*/
		void addWindow(Window* w);

		/**
		* Internal function to add a modal window. (also used by SerialReader class)
		*/
		void addModalWindow(ModalWindow* w);

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Moves the Window to the end of the list, meaning it will be drawn last, on top of other Windows.
		*/
		void bringToFront(Window* w);

		/**
		* Iterates through free list and destroys all widgets queued for deletion.
		*/
		void cleanupWidgets();
		/**
		* Create Context Menu.
		*/
		ContextMenu* createContextMenu(ContextMenuDesc* d);
		ContextMenu* createContextMenu();
		ContextMenu* createContextMenu(const Point& position);
		ContextMenu* createContextMenu(const Size& size);
		ContextMenu* createContextMenu(const Ogre::UTFString& titleBarText, const Size& size);
		ContextMenu* createContextMenu(std::vector<TextSegment> segments, const Size& size);
		ContextMenu* createContextMenuNoTitleBar(const Size& size);
		/**
		* Create ModalWindow.
		* NOTE: ModalWindows are drawn over all other windows and consume all input.
		*/
		ModalWindow* createModalWindow(ModalWindowDesc* d);
		ModalWindow* createModalWindow();
		ModalWindow* createModalWindow(const Rect& dimensions);
		ModalWindow* createModalWindow(const Ogre::UTFString& titleBarText, const Rect& dimensions);
		ModalWindow* createModalWindow(std::vector<TextSegment> segments, const Rect& dimensions);
		ModalWindow* createModalWindowNoTitleBar(const Rect& dimensions);
		/**
		* Create Window.
		*/
		Window* createWindow(WindowDesc* d);
		Window* createWindow();
		Window* createWindow(const Point& position);
		Window* createWindow(const Rect& dimensions);
		Window* createWindow(const Ogre::UTFString& titleBarText, const Rect& dimensions);
		Window* createWindow(std::vector<TextSegment> segments, const Rect& dimensions);
		Window* createWindowNoTitleBar(const Rect& dimensions);

		/**
		* Queues a widget for destruction.
		* NOTE: Widget must not be attached to another widget.
		*/
		void destroyContextMenu(const Ogre::String& name);
		void destroyContextMenu(ContextMenu* m);
		void destroyModalWindow(const Ogre::String& name);
		void destroyModalWindow(ModalWindow* w);
		void destroyWidget(Widget* w);
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
		Window* findWindowAtPoint(const Point& p, unsigned int queryFilter = -1, bool ignoreDisabled = true);
		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireSheetEvent(SheetEvent e, EventArgs& args);
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
		ContextMenu* getContextMenu(const Ogre::String& name);
		/**
		* Returns the widget that should receive keyboard events;
		*/
		Widget* getKeyboardListener();
		ModalWindow* getModalWindow(const Ogre::String& name);
		/**
		* Returns true if the Mouse Cursor is enabled, false otherwise.
		*/
		bool getMouseCursorEnabled();
		/**
		* Returns the query filter of the Mouse Cursor. The query filter is used in combination with widget queryFlags to
		* determine widget detection.  If the filter and flags are ANDed together and result in a non-zero value,
		* the widget is detected.
		*/
		unsigned int getMouseQueryFilter();
		/**
		* Retrieves the "type" of the MouseCursor.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		std::string getMouseCursorSkinTypeName();
		/**
		* Returns true if the Mouse Cursor is visible, false otherwise.
		*/
		bool getMouseCursorVisible();
		Window* getWindow(const Ogre::String& name);
		Window* getWindowInFocus();

		/**
		* Returns true if this sheet has a child contextmenu with the name given, false
		* otherwise.
		*/
		bool hasContextMenu(const Ogre::String& name);
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
		* Returns true if the given name is used by a widget belonging to this group, false otherwise.
		*/
		bool isNameUsed(const Ogre::String& name);
		/**
		* Returns true if this widget's position is managed by another widget.
		*/
		virtual bool isPositionManaged();
		/**
		* Returns true if this widget's size is managed by another widget.
		*/
		virtual bool isSizeManaged();

		/**
		* Flags the parent window as dirty causing its texture to be updated (redrawn).
		*/
		virtual void redraw();
		/**
		* Removes all Event Handlers registered by the given object.
		*/
		virtual void removeEventHandlers(void* obj);
		/**
		* Remove a Modal Window from this sheet.
		*/
		void removeModalWindow(ModalWindow* w);
		/**
		* Remove a window from this sheet.
		*/
		void removeWindow(Window* w);
		/**
		* Returns the name if it is not currently used by any widget in this Sheet, otherwise
		* returns a suggestion of a name not currently used by any Widget.
		* ie. Button -> Button1 -> Button2 -> etc.
		*/
		Ogre::String resolveName(const Ogre::String& name);

		/**
		* Serializes this widget, including its properties, to disk.
		*/
		virtual void saveToDisk(const Ogre::String fileName);
		/**
		* Moves the Window to the beginning of the list, meaning it will be drawn first, underneath any other windows.
		*/
		void sendToBack(Window* w);
		/**
		* Outlines how the desc class is written to script and read from script.
		*/
		virtual void serialize(SerialBase* b);
		/**
		* Notifies the Sheet of the Widget that should receive keyboard events.
		*/
		void setKeyboardListener(Widget* w);
		/**
		* Sets whether the cursor is able to inject mouse events into the GUIManager.
		*/
		void setMouseCursorEnabled(bool enabled);
		/**
		* Sets the query filter of the Mouse Cursor. The query filter is used in combination with widget queryFlags to
		* determine widget detection.  If the filter and flags are ANDed together and result in a non-zero value,
		* the widget is detected.
		*/
		void setMouseQueryFilter(unsigned int filter);
		/**
		* Sets the "type" of the MouseCursor.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		void setMouseCursorSkinTypeName(const std::string& skinTypeName);
		/**
		* Sets whether the cursor will be drawn on the screen.
		*/
		void setMouseCursorVisible(bool visible);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();

	public:
		// Here we have to call out any protected Widget set accesors we want to expose

		using Widget::drag;
		using Widget::resize;
		using Widget::setConsumeKeyboardEvents;
		using Widget::setDimensions;
		using Widget::setDragable;
		using Widget::setHeight;
		using Widget::setHorizontalAnchor;
		using Widget::setMaxSize;
		using Widget::setMinSize;
		using Widget::setPosition;
		using Widget::setPositionRelativeToParentClientDimensions;
		using Widget::setQueryFlags;
		using Widget::setResizeFromAllSides;
		using Widget::setResizeFromBottom;
		using Widget::setResizeFromLeft;
		using Widget::setResizeFromRight;
		using Widget::setResizeFromTop;
		using Widget::setSerialize;
		using Widget::setSize;
		using Widget::setTransparencyPicking;
		using Widget::setVerticalAnchor;
		using Widget::setVisible;
		using Widget::setWidth;

	protected:

		/**
		* Standard Constructor.
		*/
		Sheet(SheetDesc* d);

		/**
		* Advanced Constructor - 
		* If loadFromFile is true, s is treated as a File, and opened, parsed and used to build a Sheet.
		* If loadFromFile is false, s is treated as a String, and parsed and used to build a Sheet.
		*/
		Sheet(const Ogre::String& s, bool loadFromFile);

		/**
		* Destructor.
		*/
		virtual ~Sheet();

		std::set<Ogre::String> mUsedNames;
		void _notifyNameFreed(const Ogre::String& name);
		void _notifyNameUsed(const Ogre::String& name);

		std::list<ModalWindow*> mModalWindows;
		std::list<Window*> mWindows;

		Window* mWindowInFocus;
		Widget* mKeyboardListener;

		bool mDeleting;

		SheetDesc* mDesc;

		// List allowing widgets to be queued for deletion.
		std::list<Widget*> mFreeList;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mSheetEventHandlers[SHEET_EVENT_COUNT];

		/**
		* Offsets the widget.
		*/
		void drag(unsigned int xOffset, unsigned int yOffset);

		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();

		/**
		* Called by GUIManager, passing pointers to variables holding viewport size.
		* This is used in cases where the viewport is smaller than the sheet size, and
		* scrolling support is enabled.
		*/
		void notifyViewport(float* width, float* height);

	private:
		
	};
}

#endif
