#ifndef SIMPLEGUITEXTBOX_H
#define SIMPLEGUITEXTBOX_H

#include "SimpleGUILabel.h"
#include "SimpleGUIPrerequisites.h"
#include "SimpleGUIText.h"
#include "SimpleGUIWidget.h"

namespace SimpleGUI
{
	/** Represents a TextBox.
		@remarks
		TextBoxes allow the user to input data on the screen,
		which can be used for other purposes.  The TextBox class 
		requires at least 2 materials to define it's image:
		Background Image, Border.  For example, if you pass
		the constructor "sample.textbox" as its arguement for the 
		material, the class will expect "sample.textbox.border"
		to exist.
		@note
		TextBoxes must be created by the Window widget.
	*/
	class TextBox :
		public Widget
	{
	public:
		/** Constructor
            @param
                name The name to be given to the widget (must be unique).
            @param
                dimensions The relative x Position, y Position, width, and height of the widget.
			@param
				material Ogre material defining the widget image.
			@param
				show toggle visibility.
			@param
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		TextBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		~TextBox();

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
		* When user has changed the font, character height, or text,
		* the label must be updated and aligned according to its parent.
		*/
		void _notifyTextChanged();
		/**
		* Adds a character to the textBox.
		*/
		void addCharacter(char c);
		template<typename T> void addOnEnterPressedEventHandler(bool (T::*function)(const EventArgs&), T* obj)
		{
			mOnEnterPressedUserEventHandlers.push_back(new MemberFunctionPointer<T>(function,obj));
		}
		/**
		* Aligns the child Label widget horizontally and vertically.
		*/
		void alignText(HorizontalAlignment ha, VerticalAlignment va);
		/**
		* Method to erase the last character from the text in the TextBox.
		*/
		void backSpace();
		/**
		* Gets a handle to the Label widget used for this Widget.
		*/
		Text* getTextWidget();
		bool getReadOnly();
		/**
		* Hides the widget, including any children widgets.
		*/
		void hide();
		/**
		* Hides the actual user input and writes the designated character
		* in its place.  Great for visible password protection.
		*/
		void maskUserInput(const char& symbol=' ');

		// Overridden Event Handling functions
		/**
		* Handler for the SGUI_ACTIVATED event, and activates all child widgets (if exist)
		*/
		void activate(EventArgs& e);
		/**
		* Handler for the SGUI_DEACTIVATED event, and deactivates all child widgets (if exist)
		*/
		void deactivate(EventArgs& e);
		/**
		* User defined event handler called when user presses Enter.
		*/
		bool onEnterPressed(KeyEventArgs& e);
		/**
		* Handler for the SGUI_KEY_DOWN event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onKeyDown(KeyEventArgs& e);
		/**
		* Handler for the SGUI_KEY_UP event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onKeyUp(KeyEventArgs& e);
		/**
		* Handler for the SGUI_CHARACTER_KEY event.  Appends character to the end of the Label's text.
		* If not handled, it will be passed to the parent widget (if exists)
		*/
		bool onCharacter(KeyEventArgs& e);
		/**
		* Sets the the number of times materials should be tiled in the x and y direction on the Button's
		* PanelOverlayElement.
		*/
		void setTiling(const Ogre::Real& horizontalRepetitions, const Ogre::Real& verticalRepetitions, Ogre::ushort layer=0);
		/**
		* If set to true, cannot input text to textbox
		*/
		void setReadOnly(bool readOnly);
		void setText(const Ogre::String& text);
		/**
		* Shows the widget, including any child widgets.
		*/
		void show();

		void timeElapsed(Ogre::Real time);

	protected:
		// Default material, displayed in its original state.
		Ogre::String						mMaterial;

		bool								mMaskUserInput;
		char								mMaskSymbol;

		Ogre::Real							mTimeAccumulator;
		bool								mBackSpaceDown;
		bool								mReadOnly;

		Ogre::BorderPanelOverlayElement*	mBorderOverlayElement;

		Text*								mTextWidget;

		std::vector<MemberFunctionSlot*> mOnEnterPressedUserEventHandlers;

		// Container adds another zOrder, and enusures the Text will appear over
		// the background
		Ogre::OverlayContainer*				mChildrenContainer;
	};
}

#endif
