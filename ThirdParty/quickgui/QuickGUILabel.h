#ifndef QUICKGUILABEL_H
#define QUICKGUILABEL_H

#include "QuickGUIImage.h"
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
	class _QuickGUIExport Label :
		public Image
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
				textureName The name of the texture used to visually represent the widget. (ie "qgui.window.png")
			@param
				group QuadContainer containing this widget.
			@param
				ParentWidget parent widget which created this widget.
        */
		Label(const Ogre::String& name, Type type, const Rect& pixelDimensions, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm);

		/**
		* Aligns the child Label widget horizontally and vertically
		*/
		void alignText();
		/**
		* Disable Widget, making it unresponsive to events.
		*/
		void disable();
		/**
		* Enable Widget, allowing it to accept and handle events.
		*/
		void enable();
		/**
		* Convenience method to return the Text object's caption.
		*/
		virtual Ogre::UTFString getCaption();
		/**
		* Returns a reference to the Text object used by this widget.
		*/
		Text* getText();
		/*
		* Returns the dimensions of the area used for text aligning and displaying.
		*/
		Rect getTextBounds();
		/*
		* Hides the widget, including text.
		*/
		void hide();
		void onPositionChanged(const EventArgs& args);
		void onSizeChanged(const EventArgs& args);
		/**
		* Convenience method.  For advance text use, use getText function to
		* get a reference to the Text object.
		*/
		virtual void setCaption(const Ogre::UTFString& caption);
		virtual void setClippingRect(const Rect& pixelDimensions);
		/**
		* Sets the color of the text when the widget is disabled.
		*/
		void setDisabledTextColor(const Ogre::ColourValue& c);
		/**
		* Sets text vertical alignment.
		*/
		void setVerticalAlignment(VerticalAlignment va);
		/**
		* Sets text horizontal alignment.
		*/
		void setHorizontalAlignment(HorizontalAlignment ha);
		/**
		* Manipulates the offset used to determine this widgets zOrder in rendering.
		*/
		void setOffset(int offset);
		/**
		* Manually set position of widget.
		*/
		void setPosition(const Ogre::Real& pixelX, const Ogre::Real& pixelY);
		void setPosition(Point pixelPosition);
		/**
		* Manually set size of widget.
		*/
		void setSize(const Ogre::Real& pixelWidth, const Ogre::Real& height);
		void setSize(const Size& pixelSize);
		/**
		* Sets the dimensions of the area used for text aligning and displaying.
		*/
		void setTextBounds(const Point& relativePixelOffset, const Size& relativePixelSize);
		/*
		* Shows the widget, including text.
		*/
		void show();

	protected:
		virtual ~Label();

		Text* mText;
		VerticalAlignment mVerticalAlignment;
		HorizontalAlignment	mHorizontalAlignment;

		Point mTextBoundsPixelOffset;
		Size mTextBoundsPixelSize;

		// The widgets material changes often, so we need to keep track
		// of the default original material.
		Ogre::String mDefaultTexture;

		Ogre::ColourValue mTextColor;
		Ogre::ColourValue mDisabledTextColor;
	};
}

#endif
