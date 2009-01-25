#ifndef QUICKGUIHTAB_H
#define QUICKGUIHTAB_H

#include "QuickGUILabel.h"

namespace QuickGUI
{
	class _QuickGUIExport TabDesc :
			public LabelDesc
	{
	public:
		template<typename BaseClassType>
		friend class Factory;
	protected:
		TabDesc(const Ogre::String& id);
		virtual ~TabDesc() {}
	public:

		/// width in pixels of the front edge of the tab.
		float tab_frontWidth;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TabDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Tab"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Tab :
		public Label
	{
	public:
		// Skin Constants
		static const Ogre::String FRONT;
		static const Ogre::String MAIN;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// Needed for TabPage to serial write TextDesc to disk
		friend class TabPage;
		template<typename BaseClassType>
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns the Border underneath the point p.
		* NOTE: Point is assumed to be relative to the Window.
		* NOTE: Exception is thrown if point is not over a border.
		*/
		virtual BorderSide getBorderSide(Point p);
		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();

		/**
		* Returns true if the point is over a border, false otherwise.
		* NOTE: Point is assumed to be relative to the Window.
		*/
		virtual bool overBorderSide(Point p);

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
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();

	protected:
		Tab(const Ogre::String& name);
		virtual ~Tab();

		TabDesc* mDesc;

		// adjusts allotted width and width of Tab based on Text.
		void _adjustTabWidth();

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

	private:
	};
}

#endif
