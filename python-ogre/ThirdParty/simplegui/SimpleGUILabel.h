#ifndef SIMPLEGUILABEL_H
#define SIMPLEGUILABEL_H

#include "SimpleGUIPrerequisites.h"
#include "SimpleGUIText.h"
#include "SimpleGUIWidget.h"

namespace SimpleGUI
{
	/** Represents a traditional Label.
		@remarks
		Labels are SimpleGUI's method to showing text.
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
				show toggle visibility.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		Label(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		~Label();

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
		* Default Handler for the SGUI_ACTIVATED event, and activates all child widgets (if exist)
		*/
		virtual void activate(EventArgs& e);
		/**
		* Default Handler for the SGUI_DEACTIVATED event, and deactivates all child widgets (if exist)
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
