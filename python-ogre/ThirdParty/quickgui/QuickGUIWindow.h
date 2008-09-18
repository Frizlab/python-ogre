#ifndef QUICKGUIWINDOW_H
#define QUICKGUIWINDOW_H

#include "QuickGUIContainerWidget.h"
#include "QuickGUITitleBar.h"
#include "QuickGUIToolBar.h"
#include "QuickGUIPanel.h"

#include "OgreHardwarePixelBuffer.h"
#include "OgreRenderTexture.h"
#include "OgreTextureManager.h"

namespace QuickGUI
{
	// forward declaration
	class GUIManager;
	class Sheet;

	class _QuickGUIExport WindowDesc :
			public PanelDesc
	{
	public:
		WindowDesc();

		bool titleBar;
		Ogre::String titleBarSkinType;
		bool titleBarCloseButton;
		/// number of pixels between from the top, bottom, and right of TitleBar right end
		float titleBarCloseButtonPadding;
		Ogre::String titleBarCloseButtonSkinType;

		float titleBarPadding[PADDING_COUNT];

		/// Describes the Text used in this Window's TitleBar
		TextDesc titleBarTextDesc;

		virtual Ogre::String getClass() { return "WindowDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Window"; }

		// Factory method
		static WidgetDesc* factory() { return new WindowDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Window :
		public Panel
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String TITLEBAR;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class GUIManager;
		friend class Sheet;
		friend class ComponentWidget;
		friend class ContainerWidget;
		friend class Menu;
		friend class Widget;

		// Factory method
		static Widget* factory(const Ogre::String& widgetName);
	public:
		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

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
		template<typename T> void addWindowEventHandler(WindowEvent EVENT, void (T::*function)(const EventArgs&), T* obj)
		{
			mWindowEventHandlers[EVENT].push_back(new EventHandlerPointer<T>(function,obj));
		}
		void addWindowEventHandler(WindowEvent EVENT, EventHandlerSlot* function);

		/**
		* Creates a child ToolBar.
		*/
		ToolBar* createToolBar(ToolBarDesc& d);

		/**
		* Event Handler that executes the appropriate User defined Event Handlers for a given event.
		* Returns true if the event was handled, false otherwise.
		*/
		bool fireWindowEvent(WindowEvent e, EventArgs& args);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Gets the TitleBar's text in UTFString form.
		*/
		Ogre::UTFString getTitleBarText();

		/**
		* Returns true if there exists a Widget Event handler was registered by the object given,
		* false otherwise.
		*/
		bool hasWindowEventHandler(WindowEvent EVENT, void* obj);

		/**
		* Flags the window as dirty causing its texture to be updated (redrawn).
		*/
		virtual void redraw();
		/**
		* Removes the Event Handler registered by the obj.
		*/
		void removeWindowEventHandler(WindowEvent EVENT, void* obj);

		/**
		* Writes the windows generated texture to file.
		*/
		void saveTextureToFile(const Ogre::String& filename);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
		/**
		* Sets all characters of the text to the specified color.
		*/
		void setTitleBarTextColor(const Ogre::ColourValue& cv);
		/**
		* Sets the character at the index given to the specified color.
		*/
		void setTitleBarTextColor(const Ogre::ColourValue& cv, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified color.
		*/
		void setTitleBarTextColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setTitleBarTextColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setTitleBarTextColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences);
		/**
		* Sets all characters of the text to the specified font.
		*/
		void setTitleBarTextFont(const Ogre::String& fontName);
		/**
		* Sets the character at the index given to the specified font.
		*/
		void setTitleBarTextFont(const Ogre::String& fontName, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified font.
		*/
		void setTitleBarTextFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setTitleBarTextFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setTitleBarTextFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();
		/**
		* Recalculate Screen and client dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();

	protected:
		Window(const Ogre::String& name);
		virtual ~Window();

		WindowDesc* mDesc;

		TitleBar* mTitleBar;

		// Toggled true if the window's texture needs to be updated (redrawn).
		bool mDirty;

		Ogre::TexturePtr mTexture;

		// Event handlers! One List per event per widget
		std::vector<EventHandlerSlot*> mWindowEventHandlers[WINDOW_EVENT_COUNT];

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();

		/**
		* Creates the Render Target (texture) used for this window and its child widgets.
		*/
		void createRenderTarget();

		/**
		* Resizes the window's texture to the correct size
		*/
		void resizeRenderTarget();

	private:
	};
}

#endif
