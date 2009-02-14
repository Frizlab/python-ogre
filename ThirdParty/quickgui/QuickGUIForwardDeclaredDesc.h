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
		template<typename BaseClassType> friend class Factory;
	protected:
		PanelDesc(const Ogre::String& id);
		virtual ~PanelDesc() {}
	public:

		virtual Ogre::String getClass() { return "PanelDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Panel"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		//virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TabPageDesc :
			public PanelDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		TabPageDesc(const Ogre::String& id);
		virtual ~TabPageDesc() {}
	public:

		/// Vertical alignment of text within this widget's client area.
		VerticalTextAlignment	tabpage_verticalTextAlignment;

		/// Describes the Text used in this Label
		TextDesc textDesc;

		/// width in pixels of the front edge of the tab.
		float					tabpage_tabFrontWidth;
		/// pixel width to add to Tab width.
		float					tabpage_tabWidthBuffer;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TabPageDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TabPage"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TabControlDesc :
			public ContainerWidgetDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		TabControlDesc(const Ogre::String& id);
		virtual ~TabControlDesc() {}
	public:

		int		tabcontrol_selectedTab;
		/// Amount of overlap between Tabs
		float	tabcontrol_tabOverlap;
		/// Pixel height of Tabs
		float	tabcontrol_tabHeight;
		/// Determines whether tabs can be reordered using click and drag
		bool	tabcontrol_tabReordering;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TabControlDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TabControl"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};
}

#endif
