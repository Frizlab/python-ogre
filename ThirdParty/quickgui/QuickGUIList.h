#ifndef QUICKGUILIST_H
#define QUICKGUILIST_H

#include "QuickGUIImage.h"
#include "QuickGUIListItem.h"
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
		List(const Ogre::String& name, const Rect& pixelDimensions, Ogre::String texture, GUIManager* gm);

		void _setClippingWidget(Widget* w);
		/**
		* Adds (and creates) a ListItem and adds it to the List.
		* Note that a pointer to the created List Item is returned.
		* ListItem's support an Image Widget and NStateButton Widget.
		*/
		ListItem* addListItem(const Ogre::UTFString& text);
		ListItem* addListItem(const Ogre::String& name, const Ogre::UTFString& text);

		/**
		* Destroys all List Items.
		*/
		void clearList();
		/**
		* Returns true if List height is configured to adjust its height to match its child ListItems as they are added.
		* NOTE: ScrollBars cannot be used with AutoSizeHeight.
		*/
		bool getAutoSizeHeight();
		/**
		* Gets a ListItem from the List.  No exception is thrown
		* if the index is out of bounds.
		*/
		ListItem* getListItem(unsigned int index);
		ListItem* getListItem(const Ogre::String& name);
		/**
		* Returns the number of ListItems the ComboBox Drop Down List contains.
		*/
		int getNumberOfListItems();

		/**
		* Hides the widget, including any children widgets.
		*/
		void hide();
		/**
		* Does not show highlighting of ListItems, when Mouse Hovers over them.
		*/
		void hideHighlight();
		/**
		* Manually set a ListItem to be highlighted.
		*/
		void highlightListItem(ListItem* i);

		/**
		* Removes (and destroys) a ListItem from the List.  No exception is thrown
		* if the index is out of bounds.
		*/
		void removeListItem(unsigned int index);
		/**
		* Configures List to adjust its height to match its child ListItems as they are added.
		* NOTE: ScrollBars cannot be used with AutoSizeHeight.
		*/
		void setAutoSizeHeight();
		/**
		* Sets the material displayed when the mouse moves over a list item.
		*/
		void setHighlightTexture(const Ogre::String& texture);
		/**
		* Sets the pixel height of each ListItem.
		*/
		void setListItemPixelHeight(const Ogre::Real& heightInPixels);
		/**
		* Sets the number of visible items viewable in the List.
		* NOTE: autoSizeHeight is disabled.
		* NOTE: scrollbars will be available when the number of List Items exceed the 
		*  number of Visible List Items.
		*/
		void setNumberOfVisibleItems(unsigned int number);
		virtual void setQuadLayer(Quad::Layer l);
		/**
		* Shows the widget, including any child widgets.
		*/
		void show();

	// functions that were public, and should not be, for this widget.
	protected:
		void setSize(const Ogre::Real& width, const Ogre::Real& height);
		void setSize(const Size& s);

	protected:
		virtual ~List();

		ScrollPane* mScrollPane;

		Ogre::Real mItemPixelHeight;

		bool mAutoSizeHeight;

		int mAutoNameListItemCount;
		std::vector<ListItem*> mItems;

		Quad* mHighlightPanel;
		Ogre::String mHighlightTexture;

		int mNumberOfVisibleItems;

		void onWidthChanged(const EventArgs& args);
	};
}

#endif
