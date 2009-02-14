#ifndef QUICKGUITOOLBARITEM_H
#define QUICKGUITOOLBARITEM_H

#include "QuickGUIContainerWidget.h"

namespace QuickGUI
{
	// forward declaration
	class ToolBar;

	class _QuickGUIExport ToolBarItemDesc :
			public ContainerWidgetDesc
	{
	public:
		friend class ToolBar;
		template<typename BaseClassType> friend class Factory;
//	protected:
		ToolBarItemDesc(const Ogre::String& id);
		virtual ~ToolBarItemDesc() {}
	public:

		/// For Horizontally oriented ToolBars, this is the width of the Widget.
		/// For Vertically oriented ToolBars, this is the height of the widget.
		float toolbaritem_length;

		/// ToolBar that this Widget belongs to
		ToolBar* toolBar;

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ToolBarItem :
		public ContainerWidget
	{
	public:
		friend class ToolBar;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns true if this widget is a ToolBarItem, false otherwise.
		*/
		virtual bool isToolBarItem();

	protected:
		ToolBarItem(const Ogre::String& name);
		virtual ~ToolBarItem();

		/**
		* Updates the ToolBar Link.
		*/
		virtual void notifyToolBarParent(ToolBar* b);

		/// Derivative ToolBar Items will implement the following functions to define their default behavior.
		virtual void onMouseButtonDown(const EventArgs& args);
		virtual void onMouseButtonUp(const EventArgs& args);
		virtual void onMouseEnter(const EventArgs& args);
		virtual void onMouseLeave(const EventArgs& args);

		/**
		* Sets the size and position (position relative to parent) of this Widget, respectively.
		*/
		virtual void setDimensions(const Rect& r);
		/**
		* Sets the height of the widget.
		*/
		virtual void setHeight(float pixelHeight);
		/**
		* Sets the x and y position of this widget, relative to this widget's parent.
		*/
		virtual void setPosition(const Point& position);
		/**
		* Sets the width and height of this widget.
		*/
		virtual void setSize(const Size& size);
		/**
		* Sets the widget of the widget.
		*/
		virtual void setWidth(float pixelWidth);
	private:
	};
}

#endif
