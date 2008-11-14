#ifndef QUICKGUITABDESC_H
#define QUICKGUITABDESC_H

/**
* Descs in this class are not forward declared, however their corresponding Widgets
* are.
*/

#include "QuickGUIContainerWidget.h"

namespace QuickGUI
{
	class _QuickGUIExport PanelDesc :
			public ContainerWidgetDesc
	{
	public:
		PanelDesc();

		virtual Ogre::String getClass() { return "PanelDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Panel"; }

		// Factory method
		static WidgetDesc* factory() { return new PanelDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		//virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TabPageDesc :
			public PanelDesc
	{
	public:
		TabPageDesc();

		/// Vertical alignment of text within this widget's client area.
		VerticalTextAlignment verticalTextAlignment;

		/// Describes the Text used in this Label
		TextDesc textDesc;

		/// width in pixels of the front edge of the tab.
		float tabFrontWidth;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TabPageDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TabPage"; }

		// Factory method
		static WidgetDesc* factory() { return new TabPageDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TabControlDesc :
			public ContainerWidgetDesc
	{
	public:
		TabControlDesc();

		int selectedTab;
		/// Amount of overlap between Tabs
		float tabOverlap;
		/// Pixel height of Tabs
		float tabHeight;
		/// Determines whether tabs can be reordered using click and drag
		bool tabReordering;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TabControlDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TabControl"; }

		// Factory method
		static WidgetDesc* factory() { return new TabControlDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};
}

#endif
