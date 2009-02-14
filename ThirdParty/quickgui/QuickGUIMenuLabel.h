#ifndef QUICKGUIMENULABEL_H
#define QUICKGUIMENULABEL_H

#include "QuickGUIMenuItem.h"
#include "QuickGUIText.h"

namespace QuickGUI
{
	class _QuickGUIExport MenuLabelDesc :
			public MenuItemDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
//	protected:
		MenuLabelDesc(const Ogre::String& id);
		virtual ~MenuLabelDesc() {}
	public:

		/// Vertical alignment of text within this widget's client area.
		VerticalTextAlignment menulabel_verticalTextAlignment;

		/// Describes the Text used in this Label
		TextDesc textDesc;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "MenuLabelDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "MenuLabel"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport MenuLabel :
		public MenuItem
	{
	public:
		// Skin Constants
		static const Ogre::String DEFAULT;
		static const Ogre::String DOWN;
		static const Ogre::String OVER;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class Factory<Widget>;
	public:

		/**
		* Internal function, do not use.  This is not a recursive function!
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the number of pixels placed between each line of text, if there
		* are multiple lines of text.
		*/
		float getVerticalLineSpacing();
		/**
		* Returns the vertical alignment of text within this widget's client area.
		*/
		VerticalTextAlignment getVerticalTextAlignment();

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
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv);
		/**
		* Sets the number of pixels placed between each line of text, if there
		* are multiple lines of text.
		*/
		void setVerticalLineSpacing(float distance);
		/**
		* Sets the Vertical alignment of Text as displayed within the Label area.
		*/
		void setVerticalTextAlignment(VerticalTextAlignment a);

	protected:
		MenuLabel(const Ogre::String& name);
		virtual ~MenuLabel();

		Text* mText;

		MenuLabelDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
		/**
		* Closes all Menus belonging to parent ToolBar.
		*/
		virtual void onMouseButtonUp(const EventArgs& args);
		/**
		* Applies menu label "over" image
		*/
		virtual void onMouseEnter(const EventArgs& args);
		/**
		* Applies menu label "default" image
		*/
		void onMouseLeave(const EventArgs& args);

	private:
	};
}

#endif
