#ifndef QUICKGUITEXTBOX_H
#define QUICKGUITEXTBOX_H

#include "QuickGUILabel.h"
#include "QuickGUIPrerequisites.h"
#include "QuickGUIText.h"
#include "QuickGUIWidget.h"

namespace QuickGUI
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
				overlayContainer associates the internal OverlayElement with a specified zOrder.
			@param
				ParentWidget parent widget which created this widget.
        */
		TextBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget);
		virtual ~TextBox();

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
		// Notify Widget of it's Window's zOrder, and it's own zOrder offset.
		void _notifyZOrder(Ogre::ushort windowZOrder, Ogre::ushort offsetZOrder);
		// Notify Widget that Window's zOrder has changed. Updates child widgets also.
		void _notifyZOrder(Ogre::ushort windowZOrder);
		/**
		* Adds a character to the textBox right before text cursor.
		*/
		void addCharacter(Ogre::UTFString::unicode_char c);
		/**
		* Add user defined event that will be called when user presses Enter key with Textbox Activated.
		*/
		template<typename T> void addOnEnterPressedEventHandler(bool (T::*function)(const EventArgs&), T* obj)
		{
			mOnEnterPressedUserEventHandlers.push_back(new MemberFunctionPointer<T>(function,obj));
		}
		void addOnEnterPressedEventHandler(MemberFunctionSlot* function);
		/**
		* Aligns the child Label widget horizontally and vertically.
		*/
		void alignText(HorizontalAlignment ha, VerticalAlignment va);
		/**
		* Method to erase the character right before the text cursor.
		*/
		void backSpace();
		/**
		* Method to erase the character right after the text cursor.
		*/
		void deleteCharacter();
		/**
		* Iterates through visible Children widgets to find and return the widget that is *hit* by the point.
		* Returns NULL is nothing is *hit*.
		*/
		Widget* getTargetWidget(const Ogre::Vector2& p);
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
		void maskUserInput(const Ogre::UTFString::unicode_char& symbol=' ');

		// Overridden Event Handling functions
		/**
		* Handler for the QGUI_EVENT_DEACTIVATED event, and deactivates all child widgets (if exist)
		*/
		void deactivate(EventArgs& e);
		/**
		* User defined event handler called when user presses Enter.
		* Note that this event is not passed to its parents, the event is specific to this widget.
		*/
		bool onEnterPressed(KeyEventArgs& e);
		/**
		* Handler for the QGUI_EVENT_KEY_DOWN event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onKeyDown(KeyEventArgs& e);
		/**
		* Handler for the QGUI_EVENT_KEY_UP event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onKeyUp(KeyEventArgs& e);
		/**
		* Handler for the QGUI_EVENT_CHARACTER_KEY event.  Appends character to the end of the Label's text.
		* If not handled, it will be passed to the parent widget (if exists)
		*/
		bool onCharacter(KeyEventArgs& e);
		/**
		* Default Handler for the QGUI_EVENT_MOUSE_BUTTON_DOWN event.  If not handled, it will be passed
		* to the parent widget (if exists)
		*/
		bool onMouseButtonDown(MouseEventArgs& e);
		/**
		* Sets the the number of times materials should be tiled in the x and y direction on the Button's
		* PanelOverlayElement.
		*/
		void setTiling(const Ogre::Real& horizontalRepetitions, const Ogre::Real& verticalRepetitions, Ogre::ushort layer=0);
		/**
		* If set to true, cannot input text to textbox
		*/
		void setReadOnly(bool readOnly);
		void setText(const Ogre::UTFString& text);
		/**
		* Shows the widget, including any child widgets.
		*/
		void show();

		/**
		* Default Handler for injecting Time.
		*/
		void timeElapsed(Ogre::Real time);

	protected:
		// Default material, displayed in its original state.
		Ogre::String						mMaterial;

		bool								mMaskUserInput;
		Ogre::UTFString::unicode_char		mMaskSymbol;

		Ogre::Real							mBackSpaceTimer;
		bool								mBackSpaceDown;
		Ogre::Real							mDeleteTimer;
		bool								mDeleteDown;
		Ogre::Real							mMoveCursorTimer;
		bool								mLeftArrowDown;
		bool								mRightArrowDown;
		Ogre::Real							mCursorVisibilityTimer;
		bool								mReadOnly;

		Ogre::PanelOverlayElement*			mOverlayElement;

		Text*								mTextWidget;
		bool								mInputMode;

		std::vector<MemberFunctionSlot*> mOnEnterPressedUserEventHandlers;

		// Container adds another zOrder, and enusures the Text will appear over
		// the background
		Ogre::OverlayContainer*				mChildrenContainer;
	};
}

#endif
