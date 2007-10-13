#ifndef QUICKGUILIST_H
#define QUICKGUILIST_H

#include "QuickGUIImage.h"
#include "QuickGUIMenuLabel.h"
#include "QuickGUIScrollPane.h"

#include <vector>

namespace QuickGUI
{
	/** Represents a List.
		@remarks
		Lists are designed to hold a dynamic number of ListItem Widgets.
		The most command use of Lists are in the Menu and ComboBox Widget.
		@note
		List Height depends on the height of its combined ListItems.
		@note
		Lists are generally created from the Menu, ComboBox, and even List Widget.
	*/
	class _QuickGUIExport List :
		public Image
	{
	public:
		friend class ComboBox;
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The x Position, y Position, and width of the widget.
			@param
				positionMode The GuiMetricsMode for the values given for the position. (absolute/relative/pixel)
			@param
				sizeMode The GuiMetricsMode for the values given for the size. (absolute/relative/pixel)
			@param
				material Ogre material defining the widget image.
			@param
				group QuadContainer containing this widget.
			@param
				ParentWidget parent widget which created this widget.
        */
		List(const Ogre::String& instanceName, const Size& pixelSize, Ogre::String texture, GUIManager* gm);

		MenuLabel* addMenuLabel();
		void allowScrolling(bool allow);

		void clear();

		Widget* getItem(unsigned int index);
		int getItemIndex(Widget* w);
		int getNumberOfItems();
		ScrollPane* getScrollPane();

		void removeItem(Widget* w);
		void removeItem(unsigned int index);

		bool scrollingAllowed();
		
		/**
		* Sets the pixel height of each ListItem.
		*/
		void setItemPixelHeight(const Ogre::Real& heightInPixels);
		/**
		* Shows the widget, including any child widgets.
		*/
		void show();

	protected:
		virtual ~List();

		ScrollPane* mScrollPane;
		bool mScrollingAllowed;

		VerticalScrollBar* mRightScrollBar;
		HorizontalScrollBar* mBottomScrollBar;

		int mAutoNameWidgetCounter;
		Ogre::Real mItemHeight;

		std::vector<Widget*> mItems;

		void onChildAdded(const EventArgs& args);
		void onChildRemoved(const EventArgs& args);
		void onSizeChanged(const EventArgs& args);
	};
}

#endif
