#ifndef QUICKGUILISTITEM_H
#define QUICKGUILISTITEM_H

#include "QuickGUIImage.h"
#include "QuickGUILabel.h"
#include "QuickGUINStateButton.h"

namespace QuickGUI
{
	/** Represents an element of a List.
		@remarks
		List Items can contain text, a button, and an image.
		@note
		Lists Items are meant to be created by Lists.
	*/
	class _QuickGUIExport ListItem :
		public Label
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The x pos, y pos, width and height of the widget.
			@param
				positionMode The GuiMetricsMode for the values given for the position. (absolute/relative/pixel)
			@param
				sizeMode The GuiMetricsMode for the values given for the size. (absolute/relative/pixel)
			@param
				group QuadContainer containing this widget.
			@param
				ParentWidget parent widget which created this widget.
        */
		ListItem(const Ogre::String& name, const Rect& pixelDimensions, GUIManager* gm);

		/**
		* Add an NStateButton to the ListItem.  Useful examples: properties - (...), checkbox - [x]
		* Note that clicking this button doesn't cause the list to be hidden.
		*/
		NStateButton* addNStateButton(const Rect& pixelDimensions);
		/**
		* Add an Image to the ListItem.
		*/
		Image* addImage(const Rect& pixelDimensions, const Ogre::String& texture);

		/**
		* Returns a pointer to the NStateButton, if it exists.
		*/
		NStateButton* getNStateButton();
		/**
		* Returns a pointer to the Image, if it exists.
		*/
		Image* getImage();

		/*
		* Used to propogate mouse events through Image/Button to ListItem.
		*/
		void onLoseFocus(const EventArgs& args);

		/*
		* Used to propogate mouse events through Image/Button to ListItem.
		*/
		void onMouseButtonDown(const EventArgs& args);

		/*
		* Used to propogate mouse events through Image/Button to ListItem.
		*/
		void onMouseButtonUp(const EventArgs& args);

		/**
		* Event Handler for the EVENT_MOUSE_ENTER event.
		*/
		void onMouseEnters(const EventArgs& args);
		
		/**
		* Event Handler for the EVENT_MOUSE_LEAVE event.
		*/
		void onMouseLeaves(const EventArgs& args);

		/**
		* Remove (and destroy) the NStateButton.
		* No exception is thrown if NStateButton does not exist.
		*/
		void removeNStateButton();
		/**
		* Remove (and destroy) the Image.
		* No exception is thrown if Image does not exist.
		*/
		void removeImage();

		/*
		* Note: Must be called prior to creating Image/Button.
		*/
		void setPropogateButtonMouseEvents(bool propogate);
		void setPropogateImageMouseEvents(bool propogate);

	protected:
		virtual ~ListItem();
		virtual void setParent(Widget* parent);

		Widget* mClippingWidget;

		Image* mImage;
		NStateButton* mButton;

		// Used to determine if clicks on the item/button pass up to parent.
		bool mPropogateImageMouseEvents;
		bool mPropogateButtonMouseEvents;
	};
}

#endif
