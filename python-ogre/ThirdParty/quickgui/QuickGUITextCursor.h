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
	class _QuickGUIExport TextCursor :
		public Widget
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The x Position, y Position, width and height of the widget.
			@param
				positionMode The GuiMetricsMode for the values given for the position. (absolute/relative/pixel)
			@param
				sizeMode The GuiMetricsMode for the values given for the size. (absolute/relative/pixel)
			@param
				material Ogre material defining the widget image.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		TextCursor(const Ogre::String& name, const Ogre::Vector4& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~TextCursor();

		/**
		* Internal method that sets the pixel location and size of the widget.
		*/
		void _applyDimensions();

		/**
		* Sets the pixel x and y position of the widget.
		*/
		void setPixelPosition(const Ogre::Real& xPixelPos, const Ogre::Real& yPixelPos);

		void toggleVisibility();

	protected:
		// Default material, displayed in its original state.
		Ogre::String						mMaterial;
	};
}

#endif
