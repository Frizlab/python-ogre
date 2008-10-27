#ifndef QUICKGUILISTITEM_H
#define QUICKGUILISTITEM_H

#include "QuickGUIContainerWidget.h"

namespace QuickGUI
{
	class _QuickGUIExport ListItemDesc :
		public ContainerWidgetDesc
	{
	public:
		ListItemDesc();

		int index;
		bool selected;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ListItemDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "List"; }

		// Factory method
		static WidgetDesc* factory() { return new ListItemDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ListItem :
		public ContainerWidget
	{
	public:
		// Skin Constants
		static const Ogre::String DEFAULT;
		static const Ogre::String OVER;
		static const Ogre::String SELECTED;
	public:
		friend class List;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns the index of the ListItem in its owner List.
		*/
		int getIndex();
		/**
		* Returns true if the ListItem is selected, false otherwise.
		*/
		bool getSelected();

		/**
		* Sets the index of the ListItem
		*/
		void setIndex(unsigned int index);
		/**
		* Sets whether the ListItem is selected or not.
		*/
		void setSelected(bool selected);

	protected:
		ListItem(const Ogre::String& name);
		virtual ~ListItem();

		ListItemDesc* mDesc;

		void onMouseButtonDown(const EventArgs& args);
		void onMouseEnter(const EventArgs& args);
		void onMouseLeave(const EventArgs& args);

		/**
		* Sets the size and position (position relative to parent) of this Widget, respectively.
		*/
		void setDimensions(const Rect& r);
		/**
		* Sets the height of the widget.
		*/
		virtual void setHeight(float pixelHeight);
		/**
		* Sets Horizontal Anchor of this widget. A Left anchor will enforce the widget to maintain
		* its distance from the left side of its parent. A right anchor will enforce the widget to maintain
		* its distance from the right side of its parent. Left and Right Anchor will grow and shrink the
		* widget to maintain distance from both sides.
		*/
		virtual void setHorizontalAnchor(HorizontalAnchor a);
		/**
		* Sets the x and y position of this widget, relative to this widget's parent.
		*/
		void setPosition(const Point& position);
		/**
		* Sets the width and height of this widget.
		*/
		void setSize(const Size& size);
		/**
		* Sets Vertical Anchor of this widget. A Top anchor will enforce the widget to maintain
		* its distance from the top side of its parent. A bottom anchor will enforce the widget to maintain
		* its distance from the bottom side of its parent. Top and Bottom Anchor will grow and shrink the
		* widget to maintain distance from both sides.
		*/
		virtual void setVerticalAnchor(VerticalAnchor a);
		/**
		* Sets the widget of the widget.
		*/
		virtual void setWidth(float pixelWidth);

	private:
	};
}

#endif
