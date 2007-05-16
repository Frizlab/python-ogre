#ifndef SIMPLEGUIMANAGER_H
#define SIMPLEGUIMANAGER_H

#include "SimpleGUIKeyCode.h"
#include "SimpleGUIMenu.h"
#include "SimpleGUIMouseButtonID.h"
#include "SimpleGUIMouseCursor.h"
#include "SimpleGUIPrerequisites.h"
#include "SimpleGUIWindow.h"

#include <list>
#include <time.h>
#include <utility>
#include <ctype.h>

namespace SimpleGUI
{
	/** Manages Windows, Mouse Cursor, and Input
		@remarks
		The most important class of SimpleGUI, responsible for creating and
		destroying Windows, updating the Mouse Cursor, and handling input.
		GUIManager has a simple clearAll method, which cleans up all created
		widgets.  This supports multiple game states that have differing GUI
		Layouts.
		@note
		GUIManager is a Singleton, and frequently accessed by widgets, for
		information on the rendering window, default font, text color, character
		height, or setting focus to itself. (Window Widget does this)
		@note
		GUIManager allows 5 zOrder per window, and manages Window zOrdering so
		that windows and widgets are rendered properly on top of each other.
	*/
	class GUIManager :
		public Ogre::Singleton<GUIManager>
	{
	public:
		/** Constructor
            @param
                RenderWindowWidth Width of the primary render window in Pixels.
            @param
                RenderWindowHeight Height of the primary render window in Pixels.
        */
        GUIManager(const unsigned int& RenderWindowWidth, const unsigned int& RenderWindowHeight);
		/** Standard Destructor. */
		virtual ~GUIManager();
		/** Returns the Listener singleton object */
		static GUIManager& getSingleton();
		/** Returns a pointer to the Listener singleton object */
		static GUIManager* getSingletonPtr();

		/**
		* Updates MouseCursor class and all Windows that the window dimensions have changed.
		*/
		void _notifyWindowDimensions(const unsigned int& RenderWindowWidth, const unsigned int& RenderWindowHeight);
		/**
		* Iterates through Window List and destroys it, which properly destroys all child widgets.
		* NOTE: mouse cursor object is also destroyed.
		*/
		void clearAll();
		/** Create a Window with no material, borders, or TitleBar
            @param
                name The name to be given to the Window (must be unique).
            @param
                dimensions The relative x Position, y Position, width, and height of the window.
			@param
				toggle visibility.
        */
		Window* createEmptyWindow(const Ogre::String& name, const Ogre::Vector4& dimensions, bool show=true);
		/** Create a Window with no material, borders, or TitleBar
            @param
                dimensions The relative x Position, y Position, width, and height of the window.
			@param
				toggle visibility.
			@note
				Name for window will be autogenerated. (Convenience method)
        */
		Window* createEmptyWindow(const Ogre::Vector4& dimensions, bool show=true);
		/** Create a Mouse Cursor representing the Mouse
            @param
                dimensions The relative x Position, y Position, width, and height of the cursor.
            @param
                material Ogre material to define the cursor image.
			@param
				toggle visibility.
        */
		MouseCursor* createMouseCursor(const Ogre::Vector2& dimensions, const Ogre::String& material, bool show=true);
		/** Create a Window with material, borders, and TitleBar
            @param
                name The name to be given to the Window (must be unique).
            @param
                dimensions The relative x Position, y Position, width, and height of the window.
			@param
                material Ogre material to define the Window image.
			@param
				toggle visibility.
        */
		Window* createWindow(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show = true);
		/** Create a Window with material, borders, and TitleBar
            @param
                dimensions The relative x Position, y Position, width, and height of the window.
			@param
                material Ogre material to define the Window image.
			@param
				toggle visibility.
			@note
				Name for window will be autogenerated. (Convenience method)
        */
		Window* createWindow(const Ogre::Vector4& dimensions, const Ogre::String& material, bool show = true);
		/**
		* Destroys the Mouse Cursor - done in desctructor
		*/
		void destroyMouseCursor();
		/** Destroys a Window and all child widgets that exist
		    @param
				name Name of the window to destroy.
			@note 
				no exception is thrown if window does not exist
		*/
		void destroyWindow(const Ogre::String& name);
		/** Destroys a Window and all child widgets that exist
		    @param
				window Window to destroy.
		*/
		void destroyWindow(Window* window);

		Ogre::String getDefaultFont();
		Ogre::ColourValue getDefaultTextColor();
		Ogre::Real getDefaultCharacterHeight();
		MouseCursor* getMouseCursor();
		Widget* getMouseOverWidget();
		Ogre::Real getRenderWindowAspectRatio();
		/**
		* Get primary render window width in pixels
		*/
		unsigned int getRenderWindowWidth();
		/**
		* Get primary render window height in pixels
		*/
		unsigned int getRenderWindowHeight();

		Window* getWindow(const Ogre::String& name);

		/**
		* Useful for Text Input Widgets, like the TextBox
		*/
		bool injectChar(char c);
		bool injectKeyDown(const KeyCode& kc);
		bool injectKeyUp(const KeyCode& kc);
		
		bool injectMouseButtonDown(const MouseButtonID& button);
		bool injectMouseButtonUp(const MouseButtonID& button);
		/**
		* Injection when the mouse leaves the primary render window
		*/
		bool injectMouseLeaves(void);
		bool injectMouseMove(const int& xPixelOffset, const int& yPixelOffset);
		bool injectMousePosition(const int& xPixelPosition, const int& yPixelPosition);
		bool injectMouseWheelChange(float delta);

		void injectTime(Ogre::Real time);
		/**
		* Sets the window to have the highest zOrder (shown on top of all other windows)
		*/
		void setActiveWindow(Window* w);
		void setDefaultFont(const Ogre::String& font);
		void setDefaultTextColor(const Ogre::ColourValue& color);
		void setDefaultCharacterHeight(const Ogre::Real& height);

	protected:
		int						mRenderWindowWidth;
		int						mRenderWindowHeight;

		MouseCursor*			mMouseCursor;


		int						mAutoNameWindowCounter;
		// List of windows according to z-order.  highest z-order in front of list
        std::list<Window*>		mWindows;
		time_t					mClickTimeout;	// max number of seconds a click can be performed in
		time_t					mMouseButtonTimings[8];
		// Keep track of mouse button down/up and on what widget.  This prevents left mouse button down on button A,
		// moving the mouse to button B, and releasing the left mouse button, causing button B to be pressed. (example)
		Widget*					mMouseButtonDown[8];

		Ogre::String			mDefaultFont;
		Ogre::ColourValue		mDefaultTextColor;
		Ogre::Real				mDefaultCharacterHeight;

		Widget*					mWidgetContainingMouse;
		// Stores reference to last clicked Widget.
		Widget*					mActiveWidget;

		void sortListByWindowZOrder();
    };
}

#endif