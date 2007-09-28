#ifndef QUICKGUIWIDGET_H
#define QUICKGUIWIDGET_H

#include "OgreException.h"
#include "OgrePrerequisites.h"

#include "QuickGUIEventArgs.h"
#include "QuickGUIMemberFunctionPointer.h"
#include "QuickGUIRect.h"
#include "QuickGUIQuad.h"
#include "QuickGUIQuadContainer.h"
#include "QuickGUIExportDLL.h"

#include <vector>

namespace QuickGUI
{
	// Forward declarations
	class Window;
	class Sheet;
	class Panel;
	class GUIManager;

	/**
	* Useful for widgets horizontally aligning child widgets, for example a
	* TitleBar aligning its label widget
	*/
	enum HorizontalAlignment
	{
		QGUI_HA_LEFT			=  0,
		QGUI_HA_MID					,
		QGUI_HA_RIGHT
	};
	/**
	* Useful for widgets vertically aligning child widgets, for example a
	* TitleBar aligning its label widget
	*/
	enum VerticalAlignment
	{
		QGUI_VA_TOP				=  0,
		QGUI_VA_MID					,
		QGUI_VA_BOTTOM
	};

	/** Main QuickGUI components for creating GUI.
		@remarks
		This class outlines the interface for
		widgets, providing default functionality.
	*/
	class _QuickGUIExport Widget
	{
	public:
		// GUIManager is the only manager that can destroy widgets.
		friend class GUIManager;
		friend class ScrollPane;

		/**
		* Outlining Types of widgets in the library.
		*/
		enum Type
		{
			TYPE_BUTTON				=  0,
			TYPE_COMBOBOX				,
			TYPE_IMAGE					,
			TYPE_LABEL					,
			TYPE_LIST					,
			TYPE_LISTITEM				,
			TYPE_MENU					,
			TYPE_MENULIST				,
			TYPE_NSTATEBUTTON			,
			TYPE_PANEL					,
			TYPE_PROGRESSBAR			,
			TYPE_SCROLL_PANE			,
			TYPE_SCROLLBAR_HORIZONTAL	,
			TYPE_SCROLLBAR_VERTICAL		,
			TYPE_SHEET					,
			TYPE_TEXTBOX				,
			TYPE_TITLEBAR				,
			TYPE_TRACKBAR_HORIZONTAL	,
			TYPE_TRACKBAR_VERTICAL		,
			TYPE_WINDOW
		};
		/**
		* All widgets must support these events
		*/
		enum Event
		{
			EVENT_CHARACTER_KEY		=  0,
			EVENT_CHILD_ADDED			,
			EVENT_CHILD_REMOVED			,
			EVENT_DISABLED				,
			EVENT_DRAGGED				,
			EVENT_DROPPED				,
			EVENT_ENABLED				,
			EVENT_GAIN_FOCUS			,
			EVENT_HIDDEN				,
			EVENT_KEY_DOWN				,
			EVENT_KEY_UP				,
			EVENT_LOSE_FOCUS			,
			EVENT_MOUSE_BUTTON_DOWN		,
			EVENT_MOUSE_BUTTON_UP		,
			EVENT_MOUSE_CLICK			,
			EVENT_MOUSE_ENTER			,
			EVENT_MOUSE_LEAVE			,
			EVENT_MOUSE_MOVE			,
			EVENT_MOUSE_WHEEL			,
			EVENT_POSITION_CHANGED		,
			EVENT_SHOWN					,
			EVENT_SIZE_CHANGED
		};
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The x Position, y Position, width, and height of the widget.
			@param
				positionMode The GuiMetricsMode for the values given for the position. (absolute/relative/pixel)
			@param
				sizeMode The GuiMetricsMode for the values given for the size. (absolute/relative/pixel)
			@param
				textureName The name of the texture used to visually represent the widget. (ie "qgui.window.png")
			@param
				group The QuadContainer containing the Quad used by this Widget.
			@param
				ParentWidget parent widget which created this widget.
        */
		//GuiMetricsMode
		Widget(const Ogre::String& instanceName, Type type, const Rect& pixelDimensions, Ogre::String textureName, QuadContainer* container, Widget* ParentWidget, GUIManager* gm);

		// This function should not be called by users.  Its purpose is to add this widget to its parent's child list.
		void _addToChildList(Widget* w);
		// This function should not be called by users.  Its purpose is to set the widget's parent.
		void _notifyParent(Widget* w);
		// This function should not be called by users.  Its purpose is to notify the widget that it has no parent.
		void _notifyRemovedFromChildList();
		// Notifies the widget of the QuadContainer it belongs to.
		void _notifyQuadContainer(QuadContainer* g);
		// This function should not be called by users.  Its purpose is to remove this widget from its parent's child list.
		virtual void _removeFromChildList(Widget* w);

		/** Adds an event handler to this widget
			@param
				EVENT Defined widget events, for example: EVENT_GAIN_FOCUS, EVENT_CHARACTER_KEY, EVENT_MOUSE_BUTTON_DOWN, etc
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addEventHandler(QuickGUI::EVENT_MOUSE_BUTTON_DOWN,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addEventHandler(Event EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			mUserEventHandlers[EVENT].push_back(new MemberFunctionPointer<T>(function,obj));
		}

		void addEventHandler(Event EVENT, MemberFunctionSlot* function);

		/**
		* Alters the widgets offset to be higher than widget w.  Widget w must be in the
		* same QuadContainer and Layer.
		*/
		virtual void appearOverWidget(Widget* w);
		void constrainDragging(bool DragXOnly, bool DragYOnly);
		/**
		* Disable Widget, making it unresponsive to events.
		* NOTE: Sheets cannot be disabled.
		*/
		virtual void disable();
		/**
		* Moves draggingWidget.  By default, dragging widget is this widget, but this can be changed.
		* Allows dragging the titlebar or it's text to drag the window, for example.
		*/
		void drag(const Ogre::Real& pixelX, const Ogre::Real& pixelY);
		/**
		* Returns true if the widget is able to be dragged, false otherwise.
		*/
		bool draggingEnabled();
		/**
		* Enable Widget, allowing it to accept and handle events.
		* NOTE: Sheets cannot be enabled/disabled
		*/
		virtual void enable();
		/**
		* Returns true is widget is enabled, false otherwise.
		*/
		bool enabled();
		/**
		* Enable or Disable dragging.
		*/
		void enableDragging(bool enable);
		/**
		* Sets focus to the widget by firing an activation event.
		*/
		virtual void focus();

		std::vector<Widget*>* getChildWidgetList();
		Widget* getChildWidget(const Ogre::String& name);
		Ogre::String getDefaultSkin();
		Rect getDimensions();
		GUIManager* getGUIManager();
		Point getPosition();
		Point getScrollOffset();
		Size getSize();

		Rect getClippingRect();
		/**
		* Returns the name of the texture used when this widget becomes disabled.
		*/
		Ogre::String getDisabledTexture();
		/**
		* Returns true if the widget will gain focus when clicked, false otherwise.
		*/
		bool getGainFocusOnClick();
		bool getGrabbed();
		Ogre::Real getHeight();
		/**
		* Returns true if this widget is hidden when its parent is hidden.
		*/
		bool getHideWithParent();
		/**
		* Iterates through all child widgets and retrieves the highest offset.
		*/
		int getHighestOffset();
		/**
		* Returns true if clipping rect is inheritted from its parent.
		*/
		bool getInheritClippingRect();
		Ogre::String getInstanceName();
		/**
		* Returns true if window is able to be repositions, false otherwise.
		*/
		bool getMovingEnabled();
		/**
		* Returns the number of parent iterations required to get to Sheet widget.
		*/
		int getOffset();
		/**
		* Get Panel this widget belongs to.
		* NOTE: This value may be NULL.
		*/
		Panel* getParentPanel();
		/**
		* Get Sheet this widget belongs to.
		* NOTE: This value may be NULL.
		*/
		Sheet* getParentSheet();
		/**
		* Get Widget this widget belongs to.
		* NOTE: This value may be NULL.
		*/
		Widget* getParentWidget();
		/**
		* Get Window this widget belongs to.
		* NOTE: This value may be NULL.
		*/
		Window* getParentWindow();
		/*
		* Get Render Object that visually represents the widget.
		*/
		Quad* getQuad();
		/*
		* Get Render Object Group this widget's Quad belongs in.
		*/
		QuadContainer* getQuadContainer();
		/*
		* Get the screen pixel coordinates this widget is drawn at.
		* NOTE: This is not the same as getPosition, which returns a value relative to parent.
		* NOTE: This may not be the actual screen coordinates, since QuickGUI supports scrolling.
		*/
		Point getScreenPosition();
		/**
		* Get whether or not this widget is shown when its parent is shown.
		*/
		bool getShowWithParent();
		/**
		* Iterates through visible Children widgets to find and return the widget that is *hit* by the point.
		* Returns NULL is nothing is *hit*.
		*/
		virtual Widget* getTargetWidget(const Point& pixelPosition);
		
		Ogre::String getTextureName(bool includeExtension = true);
		/**
		* Returns the type of the widget, as enumerated above. ex. TYPE_BUTTON.
		*/
		Type getWidgetType();
		Ogre::Real getWidth();
		Ogre::Real getXPosition();
		Ogre::Real getYPosition();
		/**
		* Sets mVisible to false.  Widgets should override this to implement how they handle
		* hiding.
		*/
		virtual void hide();
		/**
		* Returns true if pixel point p is inside the pixel dimensions of this widget.
		*/
		virtual bool isPointWithinBounds(const Point& pixelPosition);
		bool isVisible();
		/**
		* Returns true if Widget w is a child of this widget, false otherwise.
		*/
		bool isChild(Widget* w);

		/**
		* Offset the widget position.  Useful for dragging/moving widgets.
		*/
		void move(const Ogre::Real& pixelX, const Ogre::Real& pixelY);
		void move(const Point& pixelOffset);
		void moveX(Ogre::Real pixelX);
		void moveY(Ogre::Real pixelY);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireEvent(Event e, const EventArgs& args);
		/*
		* Function disabling ability to change widget's texture.
		*/
		void lockTexture();
		virtual void onPositionChanged(const EventArgs& args);
		/**
		* Determins if the mouse if over a transparent part of the image defining the widget.
		* Used to determin if the mouse is *over* a widget. (non transparent parts)
		*/
		bool overTransparentPixel(const Point& mousePixelPosition);
		/**
		* Makes sure relative position and size are correct, according to parent.  Actual/Pixel position and size are maintained.
		*/
		void refresh();
		/**
		* Properly cleans up all child widgets.
		*/
		void removeAndDestroyAllChildWidgets();
		/**
		* Scales the widget over time.
		*/
		void resizeOverTime(Ogre::Real seconds, Size finalPixelSize);
		/**
		* Stores/Updates the texture used for the widget, and allows the widget to derive other needed textures. (by overriding this function)
		*/
		virtual void setBaseTexture(const Ogre::String& textureName);
		virtual void setClippingRect(const Rect& pixelDimensions);
		/**
		* Manually set the Dimensions of the widget.
		*/
		void setDimensions(const Rect& pixelDimensions);
		/**
		* Stores the texture to be used when the widget becomes disabled.
		*/
		void setDisabledTexture(const Ogre::String& disabledTexture);
		/**
		* This function specifies the widget that will be moved when the widget is *dragged*.
		* By default, the Dragging Widget is the widget itself, but this allows for the possibility
		* of moving a window by *dragging* the titlebar, or even the titlbar's text widget.
		*/
		void setDraggingWidget(Widget* w);
		/**
		* Allows clicking on a widget to not change the active widget.
		*/
		void setGainFocusOnClick(bool gainFocus);
		/**
		* Manually set mGrabbed to true.
		*/
		void setGrabbed(bool grabbed);
		void setHeight(Ogre::Real pixelHeight);
		/**
		* If set to true, this widget will be hidden when its parent's widget is hidden.
		* NOTE: All widgets have this set to true by default.
		*/
		void setHideWithParent(bool hide);
		/**
		* If set to true, widget will share the same clipping rect as its parent.
		*/
		void setInheritClippingRect(bool inherit);
		/**
		* If set to false, widget cannot be moved.
		*/
		void setMovingEnabled(bool enable);
		/**
		* Manipulates the offset used to determine this widgets zOrder in rendering.
		*/
		virtual void setOffset(int offset);
		/**
		* Manually set position of widget.
		* NOTE: the values given are relative to the parent's top left corner, and not the screen!  For screen positioning,
		*  user the setScreenPosition function.
		*/
		void setPosition(const Ogre::Real& pixelX, const Ogre::Real& pixelY);
		void setPosition(const Point& pixelPoint);
		/**
		* Manually set position of widget.
		* NOTE: the values given are relative to the render windows's top left corner, and not the parent widget!
		*/
		void setScreenPosition(const Ogre::Real& pixelX, const Ogre::Real& pixelY);
		void setScreenXPosition(const Ogre::Real& pixelX);
		void setScreenYPosition(const Ogre::Real& pixelY);
		/**
		* Manually set size of widget.
		*/
		void setSize(const Ogre::Real& pixelWidth, const Ogre::Real& pixelHeight);
		void setSize(const Size& pixelSize);
		/**
		* If set to true, this widget will be shown when its parent's widget is shown.
		* NOTE: most widgets have this set to true by default. (Menu's are false by default)
		*/
		void setShowWithParent(bool show);
		void setWidth(Ogre::Real pixelWidth);
		void setXPosition(Ogre::Real pixelX);
		void setYPosition(Ogre::Real pixelY);
		/**
		* Sets mVisible to true.  Widgets should override this to implement how they handle
		* showing.
		*/
		virtual void show();
		/**
		* Applies the texture to the Quad if exists in some form, and updates the Image used for
		* transparency picking.
		*/
		virtual void setTexture(const Ogre::String& textureName, bool updateBaseTexture = true);

		/**
		* Function required for certain widgets/functions to function properly, ie TextBox and fade.
		*/
		virtual void timeElapsed(Ogre::Real time);

		/*
		* Allows texture of widget to change. (behavior by default)
		*/
		void unlockTexture();

	protected:
		virtual ~Widget();

		GUIManager*					mGUIManager;
		Ogre::String				mInstanceName;
		Type						mWidgetType;

		// PROPERTIES
		bool						mDragXOnly;
		bool						mDragYOnly;
		bool						mVisible;
		bool						mEnabled;
		bool						mGainFocusOnClick;
		bool						mGrabbed;
		bool						mTextureLocked;
		bool						mMovingEnabled;
		bool						mDraggingEnabled;
		Ogre::Real					mCharacterHeight;
		Ogre::String				mFullTextureName;
		Ogre::String				mTextureName;
		Ogre::String				mTextureExtension;
		Ogre::String				mDisabledTextureName;
		// number of parents iterated to get to sheet.
		int							mOffset;
		bool						mHideWithParent;
		bool						mShowWithParent;
		bool						mInheritClippingRect;
		// used for transparency picking
		Ogre::Image*				mWidgetImage;
		Widget*						mParentWidget;
		Sheet*						mParentSheet;
		Window*						mParentWindow;
		Panel*						mParentPanel;

		Widget*						mWidgetToDrag;

		// EFFECTS
		bool						mResizeOverTime;
		Ogre::Real					mResizeTimer;
		Ogre::Real					mResizeTime;
		Size						mInitialPixelSize;
		Size						mFinalPixelSize;
		
		bool						mRepositionOverTime;
		Point						mFinalPixelPosition;

		Rect						mClippingRect;

		Quad*						mQuad;

		// Keeping track of the QuadContainer this Quad belongs to.
		QuadContainer*				mQuadContainer;

		// List of any child widgets this widget may have.
		std::vector<Widget*>		mChildWidgets;

		// Pixel position relative to parent.  (0,0) is the Parent Widgets top Left corner.
		Point						mPosition;
		// Used for scrolling widgets.
		Point						mScrollOffset;
		// Size in pixels.
		Size						mSize;

		// Event handlers! Only one per event per widget
		std::vector< std::vector<MemberFunctionSlot*> > mUserEventHandlers;

		void _initEventHandlers();

		/*
		* Iterates through parents and stores references to parent Panel, Window, and Sheet, if exist.
		*/
		void _detectHierarchy();

		/*
		* Breaks the texture name into its name and extension, if it has an extension.
		*/
		void _processFullTextureName(const Ogre::String& texture);

		void _setScrollXOffset(Ogre::Real pixelXOffset);
		void _setScrollYOffset(Ogre::Real pixelYOffset);
	};
}

#endif
