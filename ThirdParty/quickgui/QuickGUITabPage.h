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
		friend class Factory<Widget>;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		virtual void addWidgetEventHandler(WidgetEvent EVENT, EventHandlerSlot* function);

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
		HorizontalTextAlignment getTabHorizontalTextAlignment();
		/**
		* Returns the vertical alignment of text within the Tab area.
		*/
		VerticalTextAlignment getTabVerticalTextAlignment();
		/**
		* Gets the buffer width added to the end of Tab.
		*/
		float getTabWidthBuffer();

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
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);
		/**
		* Sets all characters of the text to the specified font.
		*/
		void setTabFont(const Ogre::String& fontName);
		/**
		* Sets the character at the index given to the specified font.
		*/
		void setTabFont(const Ogre::String& fontName, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified font.
		*/
		void setTabFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setTabFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setTabFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences);
		/**
		* Sets the height of this tab.
		*/
		void setTabHeight(float height);
		/**
		* Sets the Horizontal alignment of Text as displayed within the Label area.
		*/
		void setTabHorizontalTextAlignment(HorizontalTextAlignment a);
		/**
		* Sets the text for this object.
		*/
		void setTabText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setTabText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setTabText(Ogre::UTFString s);
		/**
		* Sets the Vertical alignment of Text as displayed within the Label area.
		*/
		void setTabVerticalTextAlignment(VerticalTextAlignment a);
		/**
		* Sets the buffer used to set the width of the main area of the Tab.
		*/
		void setTabWidthBuffer(float pixelBuffer);

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

		void onMouseButtonDownOnTab(const EventArgs& args);
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
		* Convenience methods allowing all sides to enable/disable resizing using the mouse cursor.
		*/
		void setResizeFromAllSides(bool resizable);
		/**
		* If set true, the bottom border of this widget can be used with the mouse cursor to resize the widget.
		*/
		void setResizeFromBottom(bool resizable);
		/**
		* If set true, the left border of this widget can be used with the mouse cursor to resize the widget.
		*/
		void setResizeFromLeft(bool resizable);
		/**
		* If set true, the right border of this widget can be used with the mouse cursor to resize the widget.
		*/
		void setResizeFromRight(bool resizable);
		/**
		* If set true, the top border of this widget can be used with the mouse cursor to resize the widget.
		*/
		void setResizeFromTop(bool resizable);
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
