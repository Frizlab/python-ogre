#ifndef QUICKGUITITLEBAR_H
#define QUICKGUITITLEBAR_H

#include "QuickGUIButton.h"
#include "QuickGUIComponentWidget.h"

namespace QuickGUI
{
	class _QuickGUIExport TitleBarDesc :
		public ComponentWidgetDesc
	{
	public:
		TitleBarDesc();

		bool closeButton;
		float closeButtonPadding;
		Ogre::String closeButtonSkinType;

		float padding[PADDING_COUNT];

		/// Describes the Text used in this Label
		TextDesc textDesc;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TitleBarDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TitleBar"; }

		// Factory method
		static WidgetDesc* factory() { return new TitleBarDesc(); }
	};

	class TitleBar :
		public ComponentWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String CLOSE_BUTTON;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class GUIManager;
		friend class Window;

		// Factory method
		static Widget* factory(const Ogre::String& widgetName);
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Drags the window by a specified offset.
		*/
		virtual void drag(int xOffset, int yOffset);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Gets the distance between a Label border and the text.
		*/
		float getPadding(Padding p);
		/**
		* Gets the text in UTFString form.
		*/
		Ogre::UTFString getText();

		/**
		* Sets all characters of the text to the specified color.
		*/
		void setColor(const Ogre::ColourValue& cv);
		/**
		* Sets the character at the index given to the specified color.
		*/
		void setColor(const Ogre::ColourValue& cv, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified color.
		*/
		void setColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences);
		/**
		* Sets all characters of the text to the specified font.
		*/
		void setFont(const Ogre::String& fontName);
		/**
		* Sets the character at the index given to the specified font.
		*/
		void setFont(const Ogre::String& fontName, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified font.
		*/
		void setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences);
		/**
		* Sets the distance between a Label border and the text.
		*/
		void setPadding(Padding p, float distance);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv);

	protected:
		TitleBar(const Ogre::String& name);
		virtual ~TitleBar();

		Text* mText;

		Button* mCloseButton;

		TitleBarDesc* mDesc;

		/**
		* Hides the window.  Only relevant if this TitleBar has a close button.
		*/
		void onCloseButtonUp(const EventArgs& args);
		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		/**
		* Sets whether the widget can be resized using the mouse.
		*/
		virtual void setResizable(bool resizable);

	private:
	};
}

#endif
