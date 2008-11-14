#ifndef QUICKGUITEXTBOX_H
#define QUICKGUITEXTBOX_H

#include "QuickGUIText.h"
#include "QuickGUITextCursor.h"
#include "QuickGUITextInputValidator.h"
#include "QuickGUITimerManager.h"
#include "QuickGUIWidget.h"

namespace QuickGUI
{
	class _QuickGUIExport TextBoxDesc :
			public WidgetDesc
	{
	public:
		TextBoxDesc();

		/// Amount of time until a cursor changes from visible to not visible, or vice versa.
		float cursorBlinkTime;
		Ogre::ColourValue defaultColor;
		Ogre::String defaultFontName;
		/// Horizontal alignment of text within this widget's client area.
		HorizontalTextAlignment horizontalTextAlignment;
		/// Amount of time a key must be held down before it starts repeating.
		float keyDownTime;
		/// Amount of time a key must be held down to repeat its input.
		float keyRepeatTime;
		bool maskText;
		Ogre::UTFString::code_point maskSymbol;
		unsigned int maxCharacters;
		/// Vertical alignment of text within this widget's client area.
		VerticalTextAlignment verticalTextAlignment;
		Ogre::String textCursorSkinTypeName;
		Point textPosition;

		/// Describes the Text used in this TextBox
		TextDesc textDesc;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TextBoxDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TextBox"; }

		// Factory method
		static WidgetDesc* factory() { return new TextBoxDesc(); }

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TextBox :
		public Widget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String TEXTOVERLAY;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		// Factory method
		static Widget* factory(const Ogre::String& widgetName);
	public:
		
		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Adds a character in front of the TextCursor, and increments the TextCursor
		* position.
		*/
		void addCharacter(Ogre::UTFString::code_point cp);

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the color used for all newly added characters.
		*/
		Ogre::ColourValue getDefaultColor();
		/**
		* Returns the font used for all newly added characters.
		*/
		Ogre::String getDefaultFont();
		/**
		* Returns the symbol used to mask text, if it is to be masked.
		*/
		Ogre::UTFString::code_point getMaskSymbol();
		/**
		* Returns true if the text is masked, false otherwise.
		*/
		bool getMaskText();
		/**
		* Returns the max number of characters this TextBox supports.
		*/
		int getMaxCharacters();
		/**
		* Gets the text in UTFString form.
		*/
		Ogre::UTFString getText();
		/**
		* Gets the "type" of the Text Cursor used.
		*/
		Ogre::String getTextCursorSkinType();

		void onWindowDrawn(const EventArgs& args);

		/**
		* Removes a character from the text at the index given, and
		* positions the text cursor before that character.
		* NOTE: If the cursor index is -1 it will stay -1.
		*/
		void removeCharacter(int index);

		/**
		* Sets all characters of the text to the specified color.
		*/
		void setColor(const Ogre::ColourValue& cv);
		/**
		* Sets the character at the index given to the specified color.
		*/
		void setColor(const Ogre::ColourValue& cv, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified color.
		*/
		void setColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be colored, otherwise only the first occurrence is colored.
		*/
		void setColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences);
		/**
		* Sets the index of the text cursor. A cursor index represents the position
		* to the left of the character with the same index. -1 represents the right most
		* position.
		*/
		void setCursorIndex(int index);
		/**
		* Sets the color used when adding characters to the TextBox.
		*/
		void setDefaultColor(const Ogre::ColourValue& cv);
		/**
		* Sets the font used when adding character to the TextBox.
		*/
		void setDefaultFont(const Ogre::String& fontName);
		/**
		* Enabled Widgets receive mouse and keyboard events via injections to the GUIManager.
		* Disabled Widgets can only receive these events if they are manually fired.
		*/
		virtual void setEnabled(bool enabled);
		/**
		* Sets all characters of the text to the specified font.
		*/
		void setFont(const Ogre::String& fontName);
		/**
		* Sets the character at the index given to the specified font.
		*/
		void setFont(const Ogre::String& fontName, unsigned int index);
		/**
		* Sets all characters within the defined range to the specified font.
		*/
		void setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex);
		/**
		* Searches text for c.  If allOccurrences is true, all characters of text matching c
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences);
		/**
		* Searches text for s.  If allOccurrences is true, all sub strings of text matching s
		* will be changed to the font specified, otherwise only the first occurrence is changed.
		*/
		void setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences);
		/**
		* Sets whether or not the Text will be masked. (password box)
		*/
		void setMaskText(bool mask, Ogre::UTFString::code_point maskSymbol);
		/**
		* Sets the maximum number of characters that this TextBox will support.
		*/
		void setMaxCharacters(unsigned int max);
		/**
		* Internal function to set a widget's parent, updating its window reference and position.
		*/
		virtual void setParent(Widget* parent);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv);
		/**
		* Sets the Horizontal alignment of Text as displayed within the TextBox area.
		*/
		void setHorizontalTextAlignment(HorizontalTextAlignment a);
		/**
		* Sets the Vertical alignment of Text as displayed within the TextBox area.
		*/
		void setVerticalTextAlignment(VerticalTextAlignment a);
		/**
		* Sets the "type" of the Text Cursor used.
		*/
		void setTextCursorSkinType(const Ogre::String type);
		/** Sets the input validator used to determine whether input is accepted.
            @param
                function member function assigned to process input.  Given in the form of myClass::myFunction.
				Function must return bool, and take a code point, index, and UTFString as its parameters.
            @param
                obj particular class instance that defines the handler for this event.  Here is an example:
				setTextInputValidator(myClass::myFunction,this);
			@note
				You may see Visual Studio give an error warning such as "error C2660: 'QuickGUI::TextBox::setTextInputValidator' : function does not take x arguments".
				This is an incorrect error message.  Make sure your function pointer points to a function which returns bool, and takes parameters "Ogre::UTFString::code_point cp, unsigned int index, const Ogre::UTFString& currentText".
        */
		template<typename T> void setTextInputValidator(bool (T::*TextInputValidator)(Ogre::UTFString::code_point cp, unsigned int index, const Ogre::UTFString& currentText), T* obj)
		{
			if(mTextInputValidatorSlot != NULL)
				delete mTextInputValidatorSlot;

			mTextInputValidatorSlot = new TextInputValidatorPointer<T>(TextInputValidator,obj);
		}

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();
		/**
		* Recalculate Screen and client dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();

	protected:
		TextBox(const Ogre::String& name);
		virtual ~TextBox();

		Text* mText;

		TextCursor* mTextCursor;
		int mCursorIndex;
		Point mCursorPosition;

		TextInputValidatorSlot* mTextInputValidatorSlot;

		/// Record is last key down was a function button or character input.
		bool mFunctionKeyDownLast;
		/// Store the last key that went down
		KeyEventArgs mLastKnownInput;
		/// Timer that repeats keys
		Timer* mKeyRepeatTimer;
		/**
		* Callback that repetitively inputs the last held down key.
		*/
		void keyRepeatTimerCallback();
		/// Timer that starts repeat timer
		Timer* mKeyDownTimer;
		/**
		* Callback that starts the key repeat timer.
		*/
		void keyDownTimerCallback();

		/// Storing reference to font for quick use.
		Ogre::FontPtr mCurrentFont;

		/// Timer that toggles cursor on and off.
		Timer* mBlinkTimer;
		/**
		* Toggles blinker on and off.
		*/
		void blinkTimerCallback();

		// Pointer pointing to mWidgetDesc object, but casted for quick use.
		TextBoxDesc* mDesc;

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();

		void onCharEntered(const EventArgs& args);
		void onKeyDown(const EventArgs& args);
		void onKeyUp(const EventArgs& args);
		void onKeyboardInputGain(const EventArgs& args);
		void onKeyboardInputLose(const EventArgs& args);
		void onMouseButtonDown(const EventArgs& args);
		void onTripleClick(const EventArgs& args);
		void onVisibleChanged(const EventArgs& args);

	private:
	};
}

#endif
