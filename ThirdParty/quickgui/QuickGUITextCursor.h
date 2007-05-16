#ifndef QUICKGUITEXTCURSOR_H
#define QUICKGUITEXTCURSOR_H

#include "QuickGUIPrerequisites.h"
#include "QuickGUIWidget.h"

namespace QuickGUI
{
	/** A Cursor marking a position in Text.
		@note
		Every Text Widget has a TextCursor.
	*/
	class TextCursor :
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
		TextCursor(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~TextCursor();

		/**
		* Internal method that sets the pixel location and size of the widget.
		*/
		void _applyDimensions();
		/**
		* Internal method that converts relative dimensions to absolute and 
		* pixel dimensions, and calls _applyDimensions().  Also notifies
		* all child widgets that the dimensions have changed.
		*/
		void _notifyDimensionsChanged();

		/**
		* Hides the widget, including any children widgets.
		*/
		void hide();

		/**
		* Sets the pixel x and y position of the widget.
		*/
		void setPixelPosition(const Ogre::Real& xPixelPos, const Ogre::Real& yPixelPos);

		/**
		* Sets the the number of times materials should be tiled in the x and y direction on the Button's
		* PanelOverlayElement.
		*/
		void setTiling(const Ogre::Real& horizontalRepetitions, const Ogre::Real& verticalRepetitions, Ogre::ushort layer=0);
		/**
		* Shows the widget, including any child widgets.
		*/
		void show();

		void toggleVisibility();

	protected:
		// Default material, displayed in its original state.
		Ogre::String						mMaterial;

		Ogre::PanelOverlayElement*			mPanelOverlayElement;
	};
}

#endif
