#ifndef QUICKGUISCROLLPANE_H
#define QUICKGUISCROLLPANE_H

#include "QuickGUIVerticalScrollBar.h"
#include "QuickGUIHorizontalScrollBar.h"
#include "QuickGUIWidget.h"

#include <vector>

namespace QuickGUI
{
	class _QuickGUIExport ScrollPane :
		public Widget
	{
	public:
		friend class Widget;
		friend class Panel;
		friend class Sheet;
		friend class Window;
		friend class List;

		enum HorizontalBarLayout
		{
			HORIZONTAL_BAR_LAYOUT_TOP		=  0,
			HORIZONTAL_BAR_LAYOUT_BOTTOM		,
			HORIZONTAL_BAR_LAYOUT_BOTH			,
			HORIZONTAL_BAR_LAYOUT_NONE
		};

		enum VerticalBarLayout
		{
			VERTICAL_BAR_LAYOUT_LEFT		=  0,
			VERTICAL_BAR_LAYOUT_RIGHT			,
			VERTICAL_BAR_LAYOUT_BOTH			,
			VERTICAL_BAR_LAYOUT_NONE
		};
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The x Position, y Position, width, and height of the widget.
			@param
				positionMode The GuiMetricsMode for the values given for the position. (absolute/relative/pixel)
			@param
				sizeMode The GuiMetricsMode for the values given for the size. (absolute/relative/pixel)
			@param
				textureName The name of the texture used to visually represent the widget. (ie "qgui.window.png")
			@param
				group The QuadContainer containing the Quad used by this Widget.
			@param
				ParentWidget parent widget which created this widget.
        */
		ScrollPane(const Ogre::String& instanceName, Type type, QuadContainer* container, Widget* ParentWidget, GUIManager* gm);
		~ScrollPane();

		// When widgets become managed/unmanaged, the pane may grow or shrink.
		void _determinePaneBounds();
		/**
		* Disable Widget, making it unresponsive to events.
		* NOTE: Sheets cannot be disabled.
		*/
		void disable();
		/**
		* Enable Widget, allowing it to accept and handle events.
		* NOTE: Sheets cannot be enabled/disabled
		*/
		void enable();

		HorizontalScrollBar* getBottomScrollBar();
		HorizontalScrollBar::ButtonLayout getHorizontalButtonLayout();
		VerticalScrollBar* getLeftScrollBar();
		VerticalScrollBar* getRightScrollBar();
		HorizontalScrollBar* getTopScrollBar();
		VerticalScrollBar::ButtonLayout getVerticalButtonLayout();

		void manageWidget(Widget* w);

		/**
		* Scrolls the Pane so that the given widget is in view.
		*/
		void scrollIntoView(Widget* w);
		void setHorizontalBarLayout(HorizontalBarLayout layout);
		void setHorizontalButtonLayout(HorizontalScrollBar::ButtonLayout layout);
		void setVerticalBarLayout(VerticalBarLayout layout);
		void setVerticalButtonLayout(VerticalScrollBar::ButtonLayout layout);

	protected:
		Ogre::Real mScrollBarWidth;

		HorizontalScrollBar* mTopBar;
		HorizontalScrollBar* mBottomBar;
		HorizontalScrollBar::ButtonLayout mHorizontalButtonLayout;
		HorizontalBarLayout mHorizontalBarLayout;

		VerticalScrollBar* mLeftBar;
		VerticalScrollBar* mRightBar;
		VerticalScrollBar::ButtonLayout mVerticalButtonLayout;	
		VerticalBarLayout mVerticalBarLayout;

		void _adjustBarDimensions();

		void onChildAddedToParent(const EventArgs& args);
		void onChildRemovedFromParent(const EventArgs& args);
		void onChildClicked(const EventArgs& args);
		void onParentSizeChanged(const EventArgs& args);
		void onParentPositionChanged(const EventArgs& args);
		void onHorizontalScroll(const EventArgs& args);
		void onSizeChanged(const EventArgs& args);
		void onVerticalScroll(const EventArgs& args);

		void _showHScrollBars();
		void _showVScrollBars();
		void _syncBarWithParentDimensions();

		std::vector<Widget*> mManagedWidgets;

	// Inherited functions that need to have their access level changed from public.
	protected:
		/**
		* Manually set position of widget.
		* NOTE: the values given are relative to the parent's top left corner, and not the screen!  For screen positioning,
		*  user the setScreenPosition function.
		*/
		void setPosition(const Ogre::Real& pixelX, const Ogre::Real& pixelY);
		void setPosition(const Point& pixelPosition);
	};
}

#endif
