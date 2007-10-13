#ifndef QUICKGUIIMAGE_H
#define QUICKGUIIMAGE_H

#include "QuickGUIWidget.h"

namespace QuickGUI
{
	/** Represents a simple Imagel.
		@remarks
		Pretty much a Label, but without text.
		@note
		Images also support Render To Texture.
	*/
	class _QuickGUIExport Image :
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
				group QuadContainer containing this widget.
			@param
				ParentWidget parent widget which created this widget.
        */
		Image(const Ogre::String& instanceName, const Size& pixelSize, Ogre::String textureName, GUIManager* gm);

	protected:
		virtual ~Image();
	};
}

#endif
