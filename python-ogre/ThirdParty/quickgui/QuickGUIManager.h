#ifndef QUICKGUIMANAGER_H
#define QUICKGUIMANAGER_H

#include "OgrePrerequisites.h"
#include "OgrePlatform.h"
#include "OgreRenderQueueListener.h"
#include "OgreRoot.h"
#include "OgreSceneManager.h"
#include "OgreViewport.h"

#include "QuickGUISkinSet.h"
#include "QuickGUIKeyCode.h"
#include "QuickGUIMouseButtonID.h"
#include "QuickGUIMouseCursor.h"
#include "QuickGUISheet.h"
#include "QuickGUIUtility.h"

#include <algorithm>
#include <list>
#include <map>
#include <time.h>
#include <utility>
#include <ctype.h>
#include <vector>

namespace QuickGUI
{
	/** Manages Windows, Mouse Cursor, and Input
		@remarks
		The most important class of QuickGUI, responsible for creating and
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
	class _QuickGUIExport GUIManager :
		public Ogre::RenderQueueListener
	{
	public:
		/** Constructor
        */
		GUIManager(Ogre::Viewport* vp);
		/** Standard Destructor. */
		~GUIManager();

		/**
		* Iterates through Window List and destroys it, which properly destroys all child widgets.
		*/
		void clearAll();

		Sheet* createSheet(const Ogre::String& name, const Ogre::String& texture);
		Sheet* createSheet(const Ogre::String& name);
		Sheet* createSheet();

		/** Destroys a Window and all child widgets that exist
		    @param
				name Name of the window to destroy.
			@note
				name can also be reference name given to the window.
			@note 
				no exception is thrown if window does not exist
		*/
		void destroySheet(const Ogre::String& name);
		/** Destroys a Window and all child widgets that exist
		    @param
				window Window to destroy.
		*/
		void destroySheet(Sheet* sheet);

		/**
		* Stores a reference to the widget in the free list, which will be destroyed
		* next frame.
		*/
		void destroyWidget(Widget* w);
		void destroyWidget(const Ogre::String& widgetName);

		/**
		* Returns the sheet currently being used, whether shown or hidden.
		*/
		Sheet* getActiveSheet();
		Ogre::String getDebugString();
		/**
		* Returns the default sheet, automatically created with the GUI manager.
		*/
		Sheet* getDefaultSheet();
		/**
		* Returns the SkinSet if exists, NULL otherwise.
		*/
		SkinSet* getSkinImageSet(const Ogre::String& name);

		MouseCursor* getMouseCursor();
		Widget* getMouseOverWidget();
		/**
		* Get viewport all widgets of this manager are rendering to.
		*/
		Ogre::Viewport* getViewport();
		/**
		* Get primary render window width in pixels
		*/
		Ogre::Real getViewportWidth();
		/**
		* Get primary render window height in pixels
		*/
		Ogre::Real getViewportHeight();

		/**
		* Iterates through sheet list and returns the Sheet with the
		* matching name.  Null if no match found.
		*/
		Sheet* getParentSheet(const Ogre::String& name);

		/**
		* Checks if skinName is a loaded skin SkinSet, and if textureName is an
		* embedded texture within the skin SkinSet.
		*/
		bool embeddedInSkinImageset(const Ogre::String& skinName, const Ogre::String& textureName);

		/**
		* Useful for Text Input Widgets, like the TextBox
		*/
		bool injectChar(Ogre::UTFString::unicode_char c);
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

		void loadSkin(const Ogre::String& skinName);

		virtual void renderQueueStarted(Ogre::uint8 id, const Ogre::String& invocation, bool& skipThisQueue);
		virtual void renderQueueEnded(Ogre::uint8 id, const Ogre::String& invocation, bool& repeatThisQueue);

		void removeFromRenderQueue();

		/**
		* Removes a name from the list of used Widget names. (if name in list)
		*/
		void removeWidgetName(const Ogre::String& name);

		/**
		* Sets the active sheet, displaying it on screen.
		* NOTE: You do not and should not hide previously used sheets before making this call.
		*  Hiding the previous sheet will hide all widgets and store it as hidden.  This call
		*  switches sheets used for rendering, thus, it does not affect widgets mVisible variable.
		*  The benefit is that you preserve Sheet state.
		*/
		void setActiveSheet(Sheet* s);
		/**
		* Activates the widget w, and deactivates the previously active widget. (if exists)
		*/
		void setActiveWidget(Widget* w);
		void setDebugString(const Ogre::String s);
		/*
		* Sets the Render Queue Group to render on.  By default, this is RENDER_QUEUE_OVERLAY.
		*/
		void setRenderQueueID(Ogre::uint8 id);
		/*
		* Hook into this SceneManager's render queue to render its quads to the screen.  By default,
		* the first Scene Manager instance is used.
		*/
		void setSceneManager(Ogre::SceneManager* sm);
		/*
		* Set the list of code points that will be accepted by the injectChar function.  English code points 9,32-166
		* are supported by default.
		*/
		void setSupportedCodePoints(const std::vector<Ogre::UTFString::code_point>& list);
		/*
		* Sets the viewport all widgets of this manager will render to.
		*/
		void setViewport(Ogre::Viewport* vp);
		/*
		* Returns true if a skin has been loaded with the name skinName, false otherwise.
		*/
		bool skinLoaded(const Ogre::String& skinName);

		/*
		* Returns true if the textureName represents:
		*  - an image file on disk. (ie *.png, *.jpg, etc)
		*  - an Ogre texture resource. (ie RenderToTexture, or Manually create Texture)
		*  - an image embedded within a skin SkinSet
		*/
		bool textureExists(const Ogre::String& textureName);

		/**
		* Checks if the desired widget name already exists.  If it already exists,
		* false is returned.  Otherwise, if addToList is true, the name is added to
		* list of used Widget names, and true is returned.
		*/
		bool validWidgetName(const Ogre::String& name, bool addToList = true);

	protected:
		// Viewport which renders all widgets belonging to this manager.
		Ogre::Viewport*			mViewport;

		// required to hook into the RenderQueue
		Ogre::SceneManager*		mSceneManager;
		// ID of the queue that we are hooked into
		Ogre::uint8				mQueueID;

		std::map<Ogre::String,SkinSet*> mSkinSets;

		MouseCursor*			mMouseCursor;

		Ogre::StringVector		mWidgetNames;

		// range of supported codepoints used for injectChar function.
		std::vector<Ogre::UTFString::code_point> mSupportedCodePoints;

		QuickGUI::Sheet*		mDefaultSheet;
		// Sheet currently being shown.
		QuickGUI::Sheet*		mActiveSheet;
		// Includes the Default Sheet.
		std::list<Sheet*>		mSheets;
		int						mAutoNameSheetCounter;

		Ogre::String			mDebugString;

		// list of widgets to delete on next frame.
		std::vector<Widget*>	mFreeList;

		// timer used to get time readings
		Ogre::Timer*			mTimer;
		unsigned long			mClickTimeout;	// max number of milliseconds a click can be performed in
		unsigned long			mMouseButtonTimings[8];
		// Keep track of mouse button down/up and on what widget.  This prevents left mouse button down on button A,
		// moving the mouse to button B, and releasing the left mouse button, causing button B to be pressed. (example)
		Widget*					mMouseButtonDown[8];

		Widget*					mWidgetContainingMouse;
		// Stores reference to last clicked Widget.
		Widget*					mActiveWidget;

		bool					mDraggingWidget;

		void _createDefaultTextures();
		Sheet* _createSheet(const Ogre::String& name, const Ogre::String& texture);
    };
}

#endif
