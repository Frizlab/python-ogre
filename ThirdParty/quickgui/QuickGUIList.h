#ifndef QUICKGUILIST_H
#define QUICKGUILIST_H

#include "QuickGUILabel.h"
#include "QuickGUIListItem.h"
#include "QuickGUIPrerequisites.h"

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
	class List :
		public Label
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, and width of the widget.
			@param
				material Ogre material defining the widget image.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		List(const Ogre::String& name, const Ogre::Vector3& dimensions, const Ogre::String& material, const Ogre::String& highlightMaterial, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~List();

		/**
		* Internal method that converts relative dimensions to absolute and 
		* pixel dimensions, and calls _applyDimensions().  Also notifies
		* all child widgets that the dimensions have changed.
		*/
		void _notifyDimensionsChanged();
		// Notify Widget of it's Window's zOrder, and it's own zOrder offset.
		void _notifyZOrder(Ogre::ushort windowZOrder, Ogre::ushort offsetZOrder);
		// Notify Widget that Window's zOrder has changed. Updates child widgets also.
		void _notifyZOrder(Ogre::ushort windowZOrder);

		/**
		* Adds (and creates) a ListItem and adds it to the List.
		* Note that a pointer to the created List Item is returned.
		* ListItem's support an Image Widget and NStateButton Widget.
		*/
		ListItem* addListItem(const Ogre::UTFString& text);
		/**
		* Aligns ListItem Text vertically and horizontally.
		*/
		void alignListItemText(HorizontalAlignment ha, VerticalAlignment va);

		/**
		* Default Event Handler called when widget is deactivated.
		*/
		void deactivate(EventArgs& e);

		/**
		* Gets a ListItem from the List.  No exception is thrown
		* if the index is out of bounds.
		*/
		ListItem* getListItem(unsigned int index);
		/**
		* Returns the number of ListItems the ComboBox Drop Down List contains.
		*/
		int getNumberOfListItems();
		/**
		* Iterates through visible Children widgets to find and return the widget that is *hit* by the point.
		* Returns NULL is nothing is *hit*.
		*/
		Widget* getTargetWidget(const Ogre::Vector2& p);

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
		* Remove (and destroy) a child List, including cleanup of all it's ListItems.
		* No exception is thrown if the index is out of bounds.
		*/
		void removeList(unsigned int index);
		/**
		* Removes (and destroys) a ListItem from the List.  No exception is thrown
		* if the index is out of bounds.
		*/
		void removeListItem(unsigned int index);

		/**
		* Manually sets the height of the characters for each list item and Combo Box Label component.
		* Note that the text can be set larger than the widget dimensions.
		*/
		void setCharacterHeight(const Ogre::Real& relativeHeight);
		/**
		* Shows the widget, including any child widgets.
		*/
		void show();

		/**
		* Default Handler for injecting Time.
		*/
		void timeElapsed(Ogre::Real time);

	protected:
		int mAutoNameListItemCount;
		int mAutoNameListCount;
		std::vector<ListItem*> mItems;

		HorizontalAlignment mListItemHorizontalAlignment;
		VerticalAlignment mListItemVerticalAlignment;

		Ogre::PanelOverlayElement* mHighlightPanel;
		Ogre::String mHighlightMaterial;

		std::vector<List*> mChildLists;

		// This value is an absolute dimension
		Ogre::Real mDefaultListItemHeight;
	};
}

#endif
