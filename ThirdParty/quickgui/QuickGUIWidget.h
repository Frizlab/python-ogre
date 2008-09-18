#ifndef QUICKGUIWIDGET_H
#define QUICKGUIWIDGET_H

#include "QuickGUIAnchors.h"
#include "QuickGUIBaseDesc.h"
#include "QuickGUIBorderEnums.h"
#include "QuickGUIBrush.h"
#include "QuickGUIEventArgs.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIEventHandlerPointer.h"
#include "QuickGUIRect.h"
#include "QuickGUISerializable.h"
#include "QuickGUISkinTypeManager.h"
#include "QuickGUIStringConverter.h"
#include "QuickGUIEventTypes.h"

#include "OgrePrerequisites.h"

namespace QuickGUI
{
	// forward declarations
	class GUIManager;
	class Window;
	class Sheet;

	class _QuickGUIExport WidgetDesc :
		public BaseDesc
	{
	public:
		WidgetDesc();

		BrushFilterMode brushFilterMode;
		bool consumeKeyboardEvents;
		bool enabled;
		Rect dimensions;
		Ogre::String disabledSkinType;
		bool dragable;
		HorizontalAnchor horizontalAnchor;
		float hoverTime;
		Size maxSize;
		Size minSize;
		Ogre::String name;
		bool propagateEvents[WIDGET_EVENT_COUNT];
		bool resizable;
		bool scrollable;
		bool transparencyPicking;
		VerticalAnchor verticalAnchor;
		bool visible;
		Ogre::String skinTypeName;

		GUIManager* guiManager;
		Sheet* sheet;
		
		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "WidgetDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Widget"; }
		
		/**
		* Outlines how the desc class is written to script and read from script.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Widget :
		public Serializable
	{
	public:
		friend class GUIManager;
		friend class ComponentWidget;
		friend class ContainerWidget;
		friend class Sheet;
		friend class Menu;
		friend class MenuPanel;
		friend class Window;
	public:

		/**
		* Internal use only. (Called in factory method)
		*/
		void _createDescObject(const Ogre::String& className);
		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/** Adds an event handler to this widget
			@param
				EVENT Defined widget events, for example: WIDGET_EVENT_SIZE_CHANGED, WIDGET_EVENT_MOUSE_WHEEL, etc.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addWidgetEventHandler(QuickGUI::WIDGET_EVENT_MOUSE_BUTTON_DOWN,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addWidgetEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addWidgetEventHandler(WidgetEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			mWidgetEventHandlers[EVENT].push_back(new EventHandlerPointer<T>(function,obj));
		}
		void addWidgetEventHandler(WidgetEvent EVENT, EventHandlerSlot* function);

		/**
		* Offsets the widget.
		*/
		virtual void drag(int xOffset, int yOffset);

		/**
		* Recursively searches through children and returns the first widget found of the class given.
		*/
		virtual Widget* findFirstWidgetOfClass(const Ogre::String& className);
		/**
		* Recursively searches through children and returns first widget found with name given.
		* NULL is returned if the widget is not found.
		*/
		virtual Widget* findWidget(const Ogre::String& name);
		/**
		* Checks if point p is within this widget's dimensions.
		* NULL is returned if the point is outside dimensions.
		* If ignoreDisabled is true, disabled widgets are not considered in the search.
		*/
		virtual Widget* findWidgetAtPoint(const Point& p, bool ignoreDisabled = true);
		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireWidgetEvent(WidgetEvent EVENT, EventArgs& args);

		/**
		* Returns true if this widget is being dragged, false otherwise.
		*/
		bool getBeingDragged();
		/**
		* Returns the Border underneath the point p.
		* NOTE: Point is assumed to be relative to the Window.
		* NOTE: Exception is thrown if point is not over a border.
		*/
		BorderSide getBorderSide(Point p);
		/**
		* Returns the class name of the widget, ie "Button", "Window", etc.
		*/
		virtual Ogre::String getClass() = 0;
		/**
		* Returns the rectangle inside the borders of the widget, if borders exist.
		*/
		Rect getClientDimensions();
		/**
		* Returns true if this widget accepts notification of keyboard events,
		* false otherwise.
		*/
		bool getConsumeKeyboardEvents();
		/*
		* Gets the position and size of this widget.
		* NOTE: Position is relative to its parent.
		*/
		Rect getDimensions();
		/**
		* Returns the name of the SkinType applied when the widget is disabled.
		* NOTE: "" is returned if the widget's appearance does not change when disabled.
		*/
		Ogre::String getDisabledSkinType();
		/**
		* Returns true if this widget can be dragged, false otherwise.
		*/
		bool getDragable();
		/**
		* Returns true if this widget is able to receive injected input events,
		* false otherwise.
		* NOTE: Disabled widgets can still have events fired manually.
		*/
		bool getEnabled();
		/**
		* Returns true if this widget is grabbed, false otherwise.
		*/
		bool getGrabbed();
		/**
		* Returns a reference to the GUIManager this widget belongs to.
		*/
		GUIManager* getGUIManager();
		/**
		* Returns the height in pixels of this widget.
		*/
		float getHeight();
		/**
		* Returns the horizontal anchor set for this widget.
		*/
		HorizontalAnchor getHorizontalAnchor();
		/**
		* Get the number of seconds the cursor has to idle over a widget
		* before the ON_HOVER event is fired.
		*/
		float getHoverTime();
		/**
		* Returns the maximum size this widget can be.
		*/
		Size getMaxSize();
		/**
		* Returns the minimum size this widget can be.
		*/
		Size getMinSize();
		/**
		* Returns the name of the Widget.
		*/
		Ogre::String getName();
		/**
		* Returns the parent of this widget, or NULL if this widget has no parent.
		*/
		Widget* getParentWidget();
		/**
		* Get the position of this widget relative to its parent.
		* NOTE: This does not take scrolling into account.
		*/
		Point getPosition();
		/**
		* Returns true if the parent widget receives notification of an event when
		* this widget receives notification of an event.
		*/
		bool getPropagateEventFiring(WidgetEvent EVENT);
		/**
		* Returns true if the widget can be resized using the mouse, false otherwise.
		*/
		bool getResizable();
		/**
		* Gets the position of the widget as seen on the screen.
		*/
		Point getScreenPosition();
		/**
		* Gets the amount of pixels this widget is scrolled horizontally and vertically.
		*/
		Point getScroll();
		/**
		* Returns true if this widget can be scrolled by its parent, false otherwise.
		*/
		bool getScrollable();
		/**
		* Gets the Scrollable ContainerWidget this widget belongs to, or NULL if this widget
		* is not or does not belong to a Scrollable container.
		*/
		Widget* getScrollableContainerWidget();
		/**
		* Returns the sheet this widget belongs to.
		*/
		Sheet* getSheet();
		/**
		* Returns the size of the widget;
		*/
		Size getSize();
		/**
		* Gets the position of the Widget as seen on the texture its drawn on.
		*/
		Point getTexturePosition();
		/**
		* Returns true if this widget uses transparency picking, false otherwise.  For
		* Widgets that do not use transparency picking, they will receive mouse over events
		* simply by having the mouse over their texture dimensions.
		*/
		bool getTransparencyPicking();
		/**
		* Returns the vertical anchor set for this widget.
		*/
		VerticalAnchor getVerticalAnchor();
		/**
		* Returns true if the widget is configured as visible, false otherwise.
		* NOTE: Due to scrolling, a widget may be marked as visible, but not seen on the screen.
		*/
		bool getVisible();
		/**
		* Retrieves the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		Ogre::String getSkinTypeName();
		/**
		* Returns the width in pixels of this widget.
		*/
		float getWidth();

		/**
		* Returns true if there exists a Widget Event handler was registered by the object given,
		* false otherwise.
		*/
		bool hasEventHandler(WidgetEvent EVENT, void* obj);

		/**
		* Returns true if this widget is made up of more than 1 widget.
		*/
		virtual bool isComponentWidget();
		/**
		* Returns true if this widget is able to have child widgets.
		*/
		virtual bool isContainerWidget();
		/**
		* Returns true if this widget is a menu item, false otherwise.
		*/
		virtual bool isMenuItem();
		/**
		* Returns true if this widget is a ToolBarItem, false otherwise.
		*/
		virtual bool isToolBarItem();
		
		/**
		* Returns true if the point is over a border, false otherwise.
		* NOTE: Point is assumed to be relative to the Window.
		*/
		bool overBorderSide(Point p);

		/**
		* Flags the parent window as dirty causing its texture to be updated (redrawn).
		*/
		virtual void redraw();
		/**
		* Removes the Event Handler registered by the obj.
		*/
		void removeEventHandler(WidgetEvent EVENT, void* obj);
		/**
		* Resizes the widget so that its dimensions hit the point given.
		* NOTE: The point p is assumed to be relative to the Window the widget is a part of.
		*/
		void resize(BorderSide s, float xOffset, float yOffset);
		
		/**
		* Builds the Widget from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
		/**
		* Set true if this widget accepts notification of keyboard events,
		* false otherwise.
		*/
		void setConsumeKeyboardEvents(bool consume);
		/**
		* Sets the size and position (position relative to parent) of this Widget, respectively.
		*/
		void setDimensions(const Rect& r);
		/**
		* Sets the name of the SkinType applied when the widget is disabled.
		* NOTE: Use "" to prevent the widget's apperance from changing when disabled.
		*/
		void setDisabledSkinType(const Ogre::String& SkinTypeName);
		/**
		* Set whether or not this widget can be dragged by the mouse cursor.
		*/
		void setDragable(bool dragable);
		/**
		* Enabled Widgets receive mouse and keyboard events via injections to the GUIManager.
		* Disabled Widgets can only receive these events if they are manually fired.
		*/
		void setEnabled(bool enabled);
		/**
		* Notify the widget it is grabbed or not grabbed.
		*/
		void setGrabbed(bool grabbed);
		/**
		* Sets the height of the widget.
		*/
		virtual void setHeight(float pixelHeight);
		/**
		* Sets Horizontal Anchor of this widget. A Left anchor will enforce the widget to maintain
		* its distance from the left side of its parent. A right anchor will enforce the widget to maintain
		* its distance from the right side of its parent. Left and Right Anchor will grow and shrink the
		* widget to maintain distance from both sides.
		*/
		virtual void setHorizontalAnchor(HorizontalAnchor a);
		/**
		* Set the number of seconds the cursor has to idle over a widget
		* before the ON_HOVER event is fired. (Commonly used for tool tips)
		*/
		void setHoverTime(float seconds);
		/**
		* Sets the maximum size this widget can be.
		*/
		void setMaxSize(const Size& s);
		/**
		* Sets the minimum size this widget can be.
		*/
		void setMinSize(const Size& s);
		void setName(const Ogre::String& name);
		/**
		* Sets the x and y position of this widget, relative to this widget's parent.
		*/
		void setPosition(const Point& position);
		/**
		* When set to true for a given event, the parent widget gets notification of the widget events
		* received by this widget.
		*/
		void setPropagateEventFiring(WidgetEvent EVENT, bool propogate);
		/**
		* Sets whether the widget can be resized using the mouse.
		*/
		virtual void setResizable(bool resizable);
		/**
		* Scroll the widget horizontally and vertically.
		*/
		void setScroll(unsigned int scrollX, unsigned int scrollY);
		/**
		* Scroll the widget horizontally.
		*/
		void setScrollX(unsigned int scrollX);
		/**
		* Scroll the widget vertically.
		*/
		void setScrollY(unsigned int scrollY);
		/**
		* Sets whether the widget can be scrolled by its parent widget.
		*/
		void setScrollable(bool scrollable);
		/**
		* Sets the width and height of this widget.
		*/
		virtual void setSize(Size size);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
		/**
		* Sets whether the widget will receive mouse over events simply by having the mouse over
		* its texture dimensions, or only when the cursor is over non transparent parts.
		*/
		void setTransparencyPicking(bool transparencyPicking);
		/**
		* Sets Vertical Anchor of this widget. A Top anchor will enforce the widget to maintain
		* its distance from the top side of its parent. A bottom anchor will enforce the widget to maintain
		* its distance from the bottom side of its parent. Top and Bottom Anchor will grow and shrink the
		* widget to maintain distance from both sides.
		*/
		virtual void setVerticalAnchor(VerticalAnchor a);
		/**
		* Sets whether or not the widget gets drawn.
		*/
		void setVisible(bool visible);
		/**
		* Sets the widget of the widget.
		*/
		virtual void setWidth(float pixelWidth);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();
		/**
		* Recalculate Screen and client dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();

	protected:
		Widget(const Ogre::String& name);
		virtual ~Widget();

		// Used mainly for serialization
		Ogre::String mName;

		WidgetDesc* mWidgetDesc;

		bool mGrabbed;
		bool mBeingDragged;

		Widget* mParentWidget;
		Window* mWindow;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mWidgetEventHandlers[WIDGET_EVENT_COUNT];

		// Area for child widgets, inside borders if they exist.
		Rect mClientDimensions;
		// Absolute position of the widget relative to it's Window's texture
		// For Windows (Sheet), this is always 0,0
		Point mTexturePosition;

		// The skin currently used to draw the widget
		SkinType* mSkinType;
		// The name of the SkinElement to use for border detection and client area calculations
		Ogre::String mSkinElementName;

		// Prevents initializing the widget twice.
		bool mInitialized;

		// True if this widget is used as a component, false otherwise.
		// Components do not have to be drawn inside client dimensions.
		bool mComponent;

		Point mScrollOffset;

		/**
		* Internal function to set a widget's parent, updating its window reference and position.
		*/
		virtual void setParent(Widget* parent);

		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw() = 0;

	private:

	};
}

#endif
