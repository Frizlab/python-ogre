#ifndef SIMPLEGUILIST_H
#define SIMPLEGUILIST_H

#include "SimpleGUILabel.h"
#include "SimpleGUIListItem.h"
#include "SimpleGUIPrerequisites.h"

#include <vector>

namespace SimpleGUI
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
				show toggle visibility.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		List(const Ogre::String& name, const Ogre::Vector3& dimensions, const Ogre::String& material, const Ogre::String& highlightMaterial, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		~List();

		/**
		* Internal method that converts relative dimensions to absolute and 
		* pixel dimensions, and calls _applyDimensions().  Also notifies
		* all child widgets that the dimensions have changed.
		*/
		void _notifyDimensionsChanged();

		ListItem* addListItem(const Ogre::String& text);
		void alignListItemText(HorizontalAlignment ha, VerticalAlignment va);

		void deactivate(EventArgs& e);

		ListItem* getListItem(unsigned int index);
		int getNumberOfListItems();
		Widget* getTargetWidget(const Ogre::Vector2& p);

		void hide();
		void hideHighlight();
		void highlightListItem(ListItem* i);

		void removeList(unsigned int index);
		void removeListItem(unsigned int index);

		void setCharacterHeight(const Ogre::Real& relativeHeight);
		void show();

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
