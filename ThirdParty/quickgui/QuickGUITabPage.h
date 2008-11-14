#ifndef QUICKGUITABPAGE_H
#define QUICKGUITABPAGE_H

#include "QuickGUIComponentWidget.h"
#include "QuickGUITab.h"

#include "QuickGUIButton.h"
#include "QuickGUICheckBox.h"
#include "QuickGUIForwardDeclaredDesc.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIImage.h"
#include "QuickGUILabel.h"
#include "QuickGUIList.h"
#include "QuickGUIPanel.h"
#include "QuickGUIProgressBar.h"
#include "QuickGUITextArea.h"
#include "QuickGUITextBox.h"
#include "QuickGUIVScrollBar.h"

namespace QuickGUI
{
	// forward declaration
	class TabControl;

	class _QuickGUIExport TabPage :
		public Panel
	{
	public:
		// Skin Constants
		static const Ogre::String TAB;
		static const Ogre::String TAB_DISABLED;
		static const Ogre::String TAB_OVER;
		static const Ogre::String TAB_SELECTED;
		static const Ogre::String PAGE;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// TabControl sets the size of the TabPage, and position of the Tab
		friend class TabControl;
		// Factory method
		static Widget* factory(const Ogre::String& widgetName);
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Checks if point p is within this widget's dimensions.
		* NULL is returned if the point is outside dimensions.
		* If ignoreDisabled is true, disabled widgets are not considered in the search.
		*/
		virtual Widget* findWidgetAtPoint(const Point& p, bool ignoreDisabled = true);

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
		* Returns the horizontal alignment of text within the Tab area.
		*/
		HorizontalTextAlignment getHorizontalTextAlignment();
		/**
		* Returns the vertical alignment of text within the Tab area.
		*/
		VerticalTextAlignment getVerticalTextAlignment();

		/**
		* Returns true if the tab page has been selected, false otherwise.
		*/
		bool isSelected();

		/**
		* Returns true if the point is over a border, false otherwise.
		* NOTE: Point is assumed to be relative to the Window.
		*/
		virtual bool overBorderSide(Point p);

		/**
		* Removes a child widget from this container widget.
		*/
		virtual void removeChild(Widget* w);

		/**
		* Builds the Widget from a ScriptDefinition or Writes the widget to a ScriptDefinition.
		*/
		virtual void serialize(SerialBase* b);
		/**
		* Enabled Widgets receive mouse and keyboard events via injections to the GUIManager.
		* Disabled Widgets can only receive these events if they are manually fired.
		*/
		virtual void setEnabled(bool enabled);
		/**
		* Sets the Horizontal alignment of Text as displayed within the Label area.
		*/
		void setHorizontalTextAlignment(HorizontalTextAlignment a);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
		/**
		* Sets the height of this tab.
		*/
		void setTabHeight(float height);
		/**
		* Sets the Vertical alignment of Text as displayed within the Label area.
		*/
		void setVerticalTextAlignment(VerticalTextAlignment a);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();

	protected:
		TabPage(const Ogre::String& name);
		virtual ~TabPage();

		Tab* mTab;
		Panel* mPage;

		bool mSelected;

		// Pointer pointing to mWidgetDesc object, but casted for quick use.
		TabPageDesc* mDesc;

		/**
		* Adds a child widget to this container widget.
		*/
		virtual void addChild(Widget* w);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		void onMouseButtonUpOnTab(const EventArgs& args);
		void onMouseEnterTab(const EventArgs& args);
		void onMouseLeaveTab(const EventArgs& args);

		/**
		* Called internally by the parent TabControl.
		*/
		void deselect();
		/**
		* Called internally by the parent TabControl.
		*/
		void select();

		/**
		* Set whether or not this widget can be dragged by the mouse cursor.
		*/
		virtual void setDragable(bool dragable);
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
		* Internal function to set a widget's parent, updating its window reference and position.
		*/
		virtual void setParent(Widget* parent);
		/**
		* Sets the x and y position of this widget, relative to this widget's parent.
		*/
		virtual void setPosition(const Point& position);
		/**
		* Sets whether the widget can be resized using the mouse.
		*/
		virtual void setResizable(bool resizable);
		/**
		* Sets the width and height of this widget.
		*/
		virtual void setSize(Size size);
		/**
		* Sets whether the widget will receive mouse over events simply by having the mouse over
		* its texture dimensions, or only when the cursor is over non transparent parts.
		*/
		virtual void setTransparencyPicking(bool transparencyPicking);
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
