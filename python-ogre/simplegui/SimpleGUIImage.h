#ifndef SIMPLEGUIIMAGE_H
#define SIMPLEGUIIMAGE_H

#include "SimpleGUIPrerequisites.h"
#include "SimpleGUIWidget.h"

namespace SimpleGUI
{
	/** Represents a simple Imagel.
		@remarks
		Pretty much a Label, but without text.
		@note
		Images also support Render To Texture.
	*/
	class Image :
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
		Image(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		~Image();

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
		* Hides the widget, including any children widgets.
		*/
		virtual void hide();
		/**
		* Creates a material using the texture and applies it to the widget image.
		*/
		void setMaterial(Ogre::RenderTexture* texture);
		/**
		* Applies material to widget image.  If supplied name is a texture name,
		* material is created using the texture and applied to widget image.
		*/
		void setMaterial(const Ogre::String& name, bool texture = false);
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
		* Sets the texture coordinates for the panel.
		*/
		void setUV(const Ogre::Real& u1, const Ogre::Real& v1, const Ogre::Real& u2, const Ogre::Real& v2);
	protected:
		// Default Label material, displayed in its original state.
		Ogre::String						mMaterial;

		Ogre::PanelOverlayElement*			mPanelOverlayElement;

		Ogre::MaterialPtr					mMaterialPtr;
	};
}

#endif
