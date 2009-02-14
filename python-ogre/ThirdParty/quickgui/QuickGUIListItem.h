#ifndef QUICKGUILISTITEM_H
#define QUICKGUILISTITEM_H

#include "QuickGUIContainerWidget.h"

namespace QuickGUI
{
	class _QuickGUIExport ListItemDesc :
		public ContainerWidgetDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		ListItemDesc(const Ogre::String& id);
		virtual ~ListItemDesc() {}
	public:


		int listitem_index;
		bool listitem_selected;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ListItemDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "List"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

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
		// List has to set positions to manage ListItems
		friend class List;
		// ComboBox has to set positions to manage ListItems
		friend class ComboBox;
		// ListPanel's destructor cleans up any remaining ListItems, requiring access to destructor
		friend class ListPanel;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Returns the listitem_index of the ListItem in its owner List.
		*/
		int getIndex();
		/**
		* Returns true if the ListItem is listitem_selected, false otherwise.
		*/
		bool getSelected();

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
		* Sets the listitem_index of the ListItem
		*/
		void setIndex(unsigned int index);
		/**
		* Sets the x and y position of this widget, relative to this widget's parent.
		*/
		void setPosition(const Point& position);
		/**
		* Sets whether the ListItem is listitem_selected or not.
		*/
		void setSelected(bool selected);
		/**
		* Sets the width and height of this widget.
		*/
		void setSize(const Size& size);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
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
