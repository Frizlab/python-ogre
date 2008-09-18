#ifndef QUICKGUIMOUSECURSOR_H
#define QUICKGUIMOUSECURSOR_H

#include "QuickGUIBrush.h"
#include "QuickGUIEventArgs.h"
#include "QuickGUIEventTypes.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIEventHandlerPointer.h"
#include "QuickGUIRect.h"
#include "QuickGUISkinType.h"
#include "QuickGUISkinTypeManager.h"

#include "OgreImage.h"
#include "OgrePrerequisites.h"

namespace QuickGUI
{
	// forward declaration
	class GUIManager;

	class _QuickGUIExport MouseCursorDesc
	{
	public:
		MouseCursorDesc();

		/// whether or not to hide the mouse when it touches viewport edges
		bool clipOnEdges;
		bool enabled;
		Ogre::String skin;
		// This will be set by the GUIManager creating the MouseCursor.
		GUIManager* guiManager;
		bool visible;
	};

	class _QuickGUIExport MouseCursor
	{
	public:
		friend class GUIManager;
	public:
		// Skin Constants
		static const Ogre::String TEXTURE;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:

		/** Adds an event handler
			@param
				EVENT Defined events, for example: MOUSE_CUSSOR_EVENT_ENABLED_CHANGED, MOUSE_CURSOR_EVENT_BORDER_LEAVE, etc.
            @param
                function member function assigned to handle the event.  Given in the form of myClass::myFunction.
				Function must return bool, and take QuickGUI::EventArgs as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				addWidgetEventHandler(QuickGUI::MOUSE_CUSSOR_EVENT_ENABLED_CHANGED,myClass::myFunction,this);
			@note
				Multiple user defined event handlers can be defined for an event.  All added event handlers will be called
				whenever the event is fired.
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::Widget::addWidgetEventHandler' : function does not take 3 arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns void, and takes parameter "const EventArgs&".
        */
		template<typename T> void addCursorEventHandler(MouseCursorEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			mEventHandlers[EVENT].push_back(new EventHandlerPointer<T>(function,obj));
		}
		void addCursorEventHandler(MouseCursorEvent EVENT, EventHandlerSlot* function);

		void draw();

		/**
		* Returns true if the cursor will be hidden when touching the viewport edges, false otherwise.
		*/
		bool getClipOnEdges();
		/**
		* Returns true if the cursor is able to inject mouse events into the GUIManager,
		* false otherwise.
		*/
		bool getEnabled();
		/**
		* Returns the pixel location of the cursor as it appears on the screen.
		*/
		Point getPosition();
		/**
		* Returns true if the cursor is drawn on the screen, false otherwise.
		*/
		bool getVisible();

		/**
		* Sets whether the cursor will be hidden or shown when touching the viewport edges.
		*/
		void setClipOnEdges(bool clip);
		/**
		* Sets whether the cursor is able to inject mouse events into the GUIManager.
		*/
		void setEnabled(bool enable);
		/**
		* Updates the cursor so that it is drawn at the position given.
		*/
		void setPosition(float xPosition, float yPosition);
		/**
		* Updates the cursor so that it is drawn at the position given.
		*/
		void setPosition(const Point& p);
		/**
		* Sets the "type" of mouse cursor.  For example you
		* can change between several types: "default", "hresize", "diag1resize", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		void setSkinType(const Ogre::String type);
		/**
		* Sets whether the cursor will be drawn on the screen.
		*/
		void setVisible(bool visible);

	protected:
		MouseCursor(const MouseCursorDesc& d);
		~MouseCursor();

		MouseCursorDesc mMouseCursorDesc;

		Rect mDimensions;

		// Current Skin type of the cursor (for example changing the cursor directly, ie resizing)
		Ogre::String mSkinType;
		// Last Skin type as set by a user (for example restoring the cursor)
		Ogre::String mDefaultSkinType;

		SkinTypeManager* mSkinTypeManager;

		/**
		* Internal function used to set the size of the cursor, as it is drawn on the screen.
		*/
		void setSize(float pixelWidth, float pixelHeight);

		// Left/Top/Right/Bottom
		bool mEnteredBorders[4];
		// Event handlers! One List per event
		std::vector<EventHandlerSlot*> mEventHandlers[MOUSE_CURSOR_EVENT_COUNT];

		/**
		* Sets the "type" of mouse cursor.  For example you
		* can change between several types: "default", "hresize", "diag1resize", etc.
		* NOTE: The type property determines what is drawn to the screen.
		* NOTE: This does not update the default skin type of the cursor, and is for internal use only.
		*/
		void _setSkinType(const Ogre::String type);

	private:
	};
}

#endif
