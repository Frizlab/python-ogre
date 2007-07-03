#ifndef QUICKGUIMENULIST_H
#define QUICKGUIMENULIST_H

#include "QuickGUIList.h"
#include "QuickGUIButton.h"

namespace QuickGUI
{
	/** Represents a MenuStrip.
		@remarks
		A button widget, with list.  When pressed, the
		list is shown.
		@note
		List Height depends on the height of its combined ListItems.
		@note
		MenuLists are created from the Menu.
	*/
	class _QuickGUIExport MenuList :
		public Button
	{
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
				material Ogre material defining the widget image. ex. "qgui.menulist"
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		MenuList(const Ogre::String& name, const Ogre::Vector4& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~MenuList();

		/**
		* Adds (and creates) a ListItem and adds it to the List.
		* Note that a pointer to the created List Item is returned.
		* ListItem's support an Image Widget and NStateButton Widget.
		*/
		ListItem* addListItem(const Ogre::UTFString& text);
		ListItem* addListItem(const Ogre::String& name, const Ogre::UTFString& text);
		/**
		* Aligns ListItem Text vertically and horizontally.
		*/
		void alignListItemText(HorizontalAlignment ha, VerticalAlignment va);
		/**
		* Destroys all List Items.
		*/
		void clearList();
		/**
		* Default Event Handler called when widget is deactivated.
		* Overridden.  If user clicks on a child widget, do not deactivate.
		*/
		void deactivate(EventArgs& e);
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
		* Does not show highlighting of ListItems, when Mouse Hovers over them.
		*/
		void hideHighlight();
		/**
		* Manually set a ListItem to be highlighted.
		*/
		void highlightListItem(ListItem* i);
		/**
		* Hide list.
		*/
		void hideList();
		/**
		* Returns true if the list is visible, false otherwise.
		*/
		bool isListVisible();
		/**
		* Returns true if the mouse is over the button, false otherwise.
		*/
		bool isMouseOverButton();
		/**
		* Removes (and destroys) a ListItem from the List.  No exception is thrown
		* if the index is out of bounds.
		*/
		void removeListItem(unsigned int index);
		/**
		* Manually sets the height of the characters for each list item.
		* Note that the text can be set larger than the widget dimensions.
		*/
		void setListCharacterHeight(const Ogre::Real& relativeHeight);
		/**
		* Sets the material displayed when the mouse moves over a list item.
		*/
		void setListHighlightMaterial(const Ogre::String& material);
		/**
		* Sets the width of the List, relative to its parent.
		*/
		void setListWidth(const Ogre::Real& relativeWidth);
		/**
		* Show list.
		*/
		void showList();
		
	protected:
		List* mList;
	};
}

#endif
