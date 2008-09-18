#ifndef QUICKGUILABEL_H
#define QUICKGUILABEL_H

#include "QuickGUIPadding.h"
#include "QuickGUIWidget.h"
#include "QuickGUIText.h"

namespace QuickGUI
{
	class _QuickGUIExport LabelDesc :
			public WidgetDesc
	{
	public:
		LabelDesc();

		float padding[PADDING_COUNT];

		/// Describes the Text used in this Label
		TextDesc textDesc;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "LabelDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Label"; }

		// Factory method
		static WidgetDesc* factory() { return new LabelDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Label :
		public Widget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// Factory method
		static Widget* factory(const Ogre::String& widgetName);
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

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
		* Returns the number of pixels placed between each line of text, if there
		* are multiple lines of text.
		*/
		float getVerticalLineSpacing();

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
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s);
		/**
		* Sets the number of pixels placed between each line of text, if there
		* are multiple lines of text.
		*/
		void setVerticalLineSpacing(float distance);

	protected:
		Label(const Ogre::String& name);
		~Label();

		// Current settings applied to newly added text.
		Ogre::String mCurrentFontName;
		Ogre::ColourValue mCurrentColourValue;

		Text* mText;

		// Pointer pointing to mWidgetDesc object, but casted for quick use.
		LabelDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
