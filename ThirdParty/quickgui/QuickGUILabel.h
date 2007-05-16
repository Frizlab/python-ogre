#ifndef QUICKGUILABEL_H
#define QUICKGUILABEL_H

#include "QuickGUIPrerequisites.h"
#include "QuickGUIText.h"
#include "QuickGUIWidget.h"

namespace QuickGUI
{
	/** Represents a traditional Label.
		@remarks
		Labels are QuickGUI's method to showing text.
		@note
		Labels must be created by the Window class.
	*/
	class Label :
		public Widget
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width and height of the widget.
			@param
				material Ogre material defining the widget image.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		Label(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~Label();

		/**
		* Internal method that sets the pixel location and size of the widget.
		*/
		void _applyDimensions();
		/**
		* Internal method that converts relative dimensions to absolute and 
		* pixel dimensions, and calls _applyDimensions().  Also notifies
		* all child widgets that the dimensions have changed.
		*/
		virtual void _notifyDimensionsChanged();
		/**
		* When user has changed the font, character height, or text,
		* the label must be updated and aligned according to its parent.
		*/
		void _notifyTextChanged();
		// Notify Widget of it's Window's zOrder, and it's own zOrder offset.
		void _notifyZOrder(Ogre::ushort windowZOrder, Ogre::ushort offsetZOrder);
		// Notify Widget that Window's zOrder has changed. Updates child widgets also.
		void _notifyZOrder(Ogre::ushort windowZOrder);
		/**
		* Aligns the child Label widget horizontally and vertically
		*/
		void alignText(HorizontalAlignment ha, VerticalAlignment va);
		/**
		* Gets a handle to the Text widget used for this Widget.
		*/
		Text* getTextWidget();
		/**
		* Hides the widget, including any children widgets.
		*/
		virtual void hide();

		// Overridden Event Handling functions
		// If user Defined Events have been created, they will be called.
		/**
		* Default Handler for the QGUI_EVENT_ACTIVATED event, and activates all child widgets (if exist)
		*/
		virtual void activate(EventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_DEACTIVATED event, and deactivates all child widgets (if exist)
		*/
		virtual void deactivate(EventArgs& e);

		/**
		* Sets the the number of times materials should be tiled in the x and y direction on the Button's
		* PanelOverlayElement.
		*/
		void setTiling(const Ogre::Real& horizontalRepetitions, const Ogre::Real& verticalRepetitions, Ogre::ushort layer=0);
		/**
		* Shows the widget, including any child widgets.
		*/
		virtual void show();

		/**
		* Default Handler for injecting Time.
		*/
		void timeElapsed(Ogre::Real time);

	protected:
		// Default Label material, displayed in its original state.
		Ogre::String						mMaterial;

		// Container adds another zOrder, and enusures the Text will appear over
		// the background
		Ogre::OverlayContainer*				mChildrenContainer;

		Text*								mTextWidget;

		Ogre::PanelOverlayElement*			mPanelOverlayElement;
	};
}

#endif
