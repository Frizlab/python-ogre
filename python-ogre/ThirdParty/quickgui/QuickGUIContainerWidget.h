#ifndef QUICKGUICONTAINERWIDGET_H
#define QUICKGUICONTAINERWIDGET_H

#include "QuickGUIComponentWidget.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIVScrollBar.h"

namespace QuickGUI
{
	// forward declaration
	class GUIManager;
	class SerialReader;

	class _QuickGUIExport ContainerWidgetDesc :
			public ComponentWidgetDesc
	{
	public:
		ContainerWidgetDesc();

		/// Amount of Scroll occuring when the horizontal bar is pressed.
		float horzBarScrollPercent;
		/// Amount of Scroll occuring when the left/right buttons are pressed.
		float horzButtonScrollPercent;
		/// If true, scrollbars will be created.  Some ContainerWidgets,
		/// especially menu and toolbar widgets, don't need scrollbars.
		bool supportScrollBars;
		/// How thick the vertical and horizontal scrollbars are
		float scrollBarThickness;
		/// Amount of Scroll occuring when the vertical bar is pressed.
		float vertBarScrollPercent;
		/// Amount of Scroll occuring when the up/down buttons are pressed.
		float vertButtonScrollPercent;
		/// The horizontal scroll applied to all children
		float xScrollOffset;
		/// The vertical scroll applied to all children
		float yScrollOffset;

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport ContainerWidget :
		public ComponentWidget
	{
	public:
		// Skin Constants
		static const Ogre::String HSCROLLBAR;
		static const Ogre::String VSCROLLBAR;
	public:
		friend class GUIManager;
		friend class SerialReader;
		friend class Menu;
	public:
		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);
		/**
		* Notifies the Widget of its GUIManager. (for component/container widgets this function is recursive)
		*/
		virtual void _setGUIManager(GUIManager* gm);
		/**
		* Notifies the Widget of its Sheet. (for component/container widgets this function is recursvie)
		*/
		virtual void _setSheet(Sheet* sheet);
		/**
		* Function to update ScrollBars in terms of Slider length and visibility.
		*/
		virtual void _updateScrollBars();

		/**
		* Recursively searches through children and returns the first widget found of the class given.
		*/
		virtual Widget* findFirstWidgetOfClass(const Ogre::String& className);
		/**
		* Recursively searches through children and returns first widget found with name given.
		* NULL is returned if the widget is not found.
		*/
		virtual Widget* findWidget(const Ogre::String& name);
		/**
		* Checks if point p is within this widget's dimensions.
		* NULL is returned if the point is outside dimensions.
		* If ignoreDisabled is true, disabled widgets are not considered in the search.
		*/
		virtual Widget* findWidgetAtPoint(const Point& p, bool ignoreDisabled = true);

		/**
		* Returns true if this widget is able to have child widgets.
		*/
		virtual bool isContainerWidget();

		/**
		* Removes a child widget from this container widget.
		*/
		virtual void removeChild(Widget* w);

		/**
		* Builds the Widget from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
		/**
		* Sets the height of the widget.
		*/
		virtual void setHeight(float pixelHeight);
		/**
		* Sets the amount of Scroll occuring when the horizontal bar is pressed.
		*/
		void setHorzBarScrollPercent(float percent);
		/**
		* Sets the amount of Scroll occuring when the horizontal buttons are pressed.
		*/
		void setHorzButtonScrollPercent(float percent);
		/**
		* Sets the width and height of this widget.
		*/
		virtual void setSize(Size size);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
		/**
		* Sets the amount of Scroll occuring when the vertical bar is pressed.
		*/
		void setVertBarScrollPercent(float percent);
		/**
		* Sets the amount of Scroll occuring when the vertical buttons are pressed.
		*/
		void setVertButtonScrollPercent(float percent);
		/**
		* Sets the widget of the widget.
		*/
		virtual void setWidth(float pixelWidth);
		/**
		* Returns true if this Widget supports scrolling, false otherwise.
		*/
		bool supportsScrolling();

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();
		/**
		* Recalculate Screen and client dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();

	protected:
		ContainerWidget(const Ogre::String& name);
		virtual ~ContainerWidget();

		std::vector<Widget*> mChildren;

		ContainerWidgetDesc* mDesc;

		HScrollBar* mHScrollBar;
		VScrollBar* mVScrollBar;
		
		// Used to implement scrolling functionality
		Size mVirtualSize;
		// Iterates through children to determine VirtualSpace
		void _determineVirtualSize();
		void _setScrollX(float x);
		void _setScrollY(float y);

		void onHorizontalScroll(const EventArgs& args);
		void onVerticalScroll(const EventArgs& args);

		void onChildDimensionsChanged(const EventArgs& args);

		void onMouseWheel(const EventArgs& args);

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Internal function to set a widget's parent, updating its window reference and position.
		*/
		virtual void setParent(Widget* parent);

		/**
		* Prepares the widget for drawing.
		*/
		virtual void draw();
	private:
	};
}

#endif
