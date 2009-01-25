#ifndef QUICKGUIMANAGER_H
#define QUICKGUIMANAGER_H

#include "QuickGUIBorderEnums.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIMouseCursor.h"
#include "QuickGUISheet.h"
#include "QuickGUITimerManager.h"

#include "OgreRenderQueueListener.h"
#include "OgreSceneManager.h"
#include "OgreViewport.h"

#include <vector>

namespace QuickGUI
{
	class _QuickGUIExport GUIManagerDesc
	{
	public:
		GUIManagerDesc();

		// If name is "", name will automatically be generated.
		Ogre::String name;
		// If NULL, first found viewport is used for rendering.
		Ogre::Viewport* viewport;
		// required to hook into the RenderQueue. If NULL, first found scene manager is used.
		Ogre::SceneManager*	sceneManager;
		// ID of the queue that we are hooked into
		Ogre::uint8 queueID;
		// Description used to create mouseCursor
		MouseCursorDesc mouseCursorDesc;
		// range of supported codepoints used for injectChar function.
		std::set<Ogre::UTFString::code_point> supportedCodePoints;
		// manager will detect single, double, and triple clicks by default
		bool determineClickEvents;
		// Maximum number of milliseconds a click can be performed in. (mouse down to mouse up)
		unsigned long clickTime;
		// Maximum number of milliseconds a double click can be performed in. (mouse click to mouse down)
		unsigned long doubleClickTime;
		// Maximum number of milliseconds a triple click can be performed in. (double click to mouse down)
		unsigned long tripleClickTime;
		// Manager will send mouse wheel events to the widget last clicked, or the widget under cursor as an alternative.
		bool scrollLastClicked;
	};

	class _QuickGUIExport GUIManager :
		public Ogre::RenderQueueListener
	{
	public:
		friend class Root;
	public:

		/**
		* Draws the GUI.
		* NOTE: This is done automatically in the renderQueueEnded.
		*/
		void draw();

		/**
		* Returns the current sheet this GUIManager is managing.
		*/
		Sheet* getActiveSheet();
		/**
		* Returns the Default Sheet created in GUIManager's constructor and initially used as default.
		*/
		Sheet* getDefaultSheet();
		/**
		* Returns the last widget that was clicked via mouse button down,
		* single, double, or triple click.
		*/
		Widget* getLastClickedWidget();
		/**
		* Returns the mouse cursor object associated with this gui manager.
		*/
		MouseCursor* getMouseCursor();
		Ogre::String getName();
		/**
		* Returns true if the last clicked widget receives mouse wheel events,
		* false otherwise.
		*/
		bool getScrollLastClicked();
		/**
		* Returns the widget that is directly underneath the mouse cursor.
		*/
		Widget* getWidgetUnderMouseCursor();
		/**
		* Gets the main viewport that is rendered to.
		*/
		Ogre::Viewport* getViewport();

		/**
		* Useful for Text Input Widgets, like the TextBox
		*/
		bool injectChar(Ogre::UTFString::unicode_char c);
		bool injectKeyDown(const KeyCode& kc);
		bool injectKeyUp(const KeyCode& kc);

		bool injectMouseButtonDown(const MouseButtonID& button);
		bool injectMouseButtonUp(const MouseButtonID& button);
		bool injectMouseClick(const MouseButtonID& button);
		bool injectMouseDoubleClick(const MouseButtonID& button);
		bool injectMouseTripleClick(const MouseButtonID& button);
		bool injectMousePosition(const int& xPixelPosition, const int& yPixelPosition);
		bool injectMouseWheelChange(float delta);

		bool isSupportedCodePoint(Ogre::UTFString::unicode_char c);

		// Inheritted functions from Ogre::RenderQueueListener
		virtual void renderQueueStarted(Ogre::uint8 id, const std::string& invocation, bool& skipThisQueue);
		virtual void renderQueueEnded(Ogre::uint8 id, const std::string& invocation, bool& repeatThisQueue);

		/**
		* Sets the current Sheet to be drawn and interacted with.
		*/
		void setActiveSheet(Sheet* s);
		/**
		* Sets the Render Queue Group to render on.  By default, this is RENDER_QUEUE_OVERLAY.
		*/
		void setRenderQueueID(Ogre::uint8 id);
		/**
		* Hook into this SceneManager's render queue to render its quads to the screen.  By default,
		* the first Scene Manager instance is used.
		*/
		void setSceneManager(Ogre::SceneManager* sm);
		/**
		* If true, mouse wheel events are sent to the last clicked widget.  Otherwise,
		* the widget under the cursor receives the events.
		*/
		void setScrollLastClicked(bool scroll);

		/**
		* Set the list of code points that will be accepted by the injectChar function.  English code points 9,32-166
		* are supported by default.
		*/
		void setSupportedCodePoints(const std::set<Ogre::UTFString::code_point>& list);

		/*
		* Defines the main viewport to render to.
		*/
		void setViewport(Ogre::Viewport* vp);
	protected:
		/** Constructor */
		GUIManager(GUIManagerDesc& d);
		/** Standard Destructor. */
		~GUIManager();

		GUIManagerDesc mGUIManagerDesc;

		int mSheetCounter;
		int mWidgetCounter;

		MouseCursor* mMouseCursor;
		Brush* mBrush;

		// Default Sheet created and used on GUIManager construction
		Sheet* mDefaultSheet;
		Sheet* mActiveSheet;

		Widget*	mWidgetUnderMouseCursor;
		// Used to send mouse wheel events to
		Widget* mLastClickedWidget;

		// timer used to get time readings.
		Ogre::Timer* mTimer;
		/// Timer used to fire Hover events
		Timer* mHoverTimer;
		/**
		* Fires ON_HOVER event.
		*/
		void hoverTimerCallback();

		/**
		* Clears mMouseButtonDown structure.
		*/
		void _clearMouseTrackingData();

		// Store the last time a click was performed.
		unsigned long mTimeOfButtonDown[NUM_MOUSE_BUTTONS];
		unsigned long mTimeOfClick[NUM_MOUSE_BUTTONS];
		unsigned long mTimeOfDoubleClick[NUM_MOUSE_BUTTONS];

		// Keep track of mouse button down/up and on what widget.  This prevents left mouse button down on button A,
		// moving the mouse to button B, and releasing the left mouse button, causing button B to be pressed. (example)
		Widget*	mMouseButtonDown[NUM_MOUSE_BUTTONS];

		//! Bit field that holds status of buttons currently pressed down
		unsigned int mButtonMask;

		//! Bit field that holds status of Alt, Ctrl, Shift
		unsigned int mKeyModifiers;

		// Used to fire dragged/dropped events.
		bool mDraggingWidget;

		// Used to resize widgets
		bool mOverResizableBorder;
		// Record if left mouse button went down on border.
		bool mDownOnBorder;
		BorderSide mResizableBorder;
		// Store the previous border the widget was over.
		BorderSide mPreviousBorder;

		bool injectMouseMove(const int& xPixelOffset, const int& yPixelOffset);
	private:
	};
}

#endif
