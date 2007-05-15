#ifndef SIMPLEGUILISTITEM_H
#define SIMPLEGUILISTITEM_H

#include "SimpleGUIImage.h"
#include "SimpleGUILabel.h"
#include "SimpleGUINStateButton.h"
#include "SimpleGUIPrerequisites.h"

namespace SimpleGUI
{
	/** Represents an element of a List.
		@remarks
		List Items can contain text, a button, and an image.
		@note
		Lists Items are meant to be created by Lists.
	*/
	class ListItem :
		public Label
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x pos, y pos, width and height of the widget.
			@param
				show toggle visibility.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		ListItem(const Ogre::String& name, const Ogre::Vector4& dimensions, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		~ListItem();

		/**
		* Internal method that converts relative dimensions to absolute and 
		* pixel dimensions, and calls _applyDimensions().  Also notifies
		* all child widgets that the dimensions have changed.
		*/
		void _notifyDimensionsChanged();

		NStateButton* addNStateButton(const Ogre::Vector4& dimensions);
		Image* addImage(const Ogre::Vector4& dimensions, const Ogre::String& material);

		void deactivate(EventArgs& e);
		bool evtHndlr_hideMenus(const EventArgs& e);

		NStateButton* getNStateButton();
		Image* getImage();
		Widget* getTargetWidget(const Ogre::Vector2& p);

		void hide();

		bool onMouseButtonUp(MouseEventArgs& e);
		bool onMouseEnters(MouseEventArgs& e);
		bool onMouseLeaves(MouseEventArgs& e);

		void removeNStateButton();
		void removeImage();

		void show();

	protected:
		Image* mImage;
		NStateButton* mButton;
	};
}

#endif
