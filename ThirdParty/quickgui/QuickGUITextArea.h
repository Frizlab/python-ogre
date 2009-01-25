#ifndef QUICKGUITEXTAREA_H
#define QUICKGUITEXTAREA_H

#include "QuickGUIText.h"
#include "QuickGUITextCursor.h"
#include "QuickGUITimerManager.h"
#include "QuickGUIContainerWidget.h"

namespace QuickGUI
{
	class _QuickGUIExport TextAreaDesc :
		public ContainerWidgetDesc
	{
	public:
		template<typename BaseClassType>
		friend class Factory;
	protected:
		TextAreaDesc(const Ogre::String& id);
		virtual ~TextAreaDesc() {}
	public:

		/// Amount of time until a cursor changes from visible to not visible, or vice versa.
		float textarea_cursorBlinkTime;
		Ogre::ColourValue textarea_defaultColor;
		Ogre::String textarea_defaultFontName;
		/// Amount of time a key must be held down before it starts repeating.
		float textarea_keyDownTime;
		/// Amount of time a key must be held down to repeat its input.
		float textarea_keyRepeatTime;
		unsigned int textarea_maxCharacters;
		/// Sets the text to read only
		bool textarea_readOnly;
		Ogre::String textarea_textCursorDefaultSkinTypeName;

		/// Describes the Text used in this TextBox
		TextDesc textDesc;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "TextAreaDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "TextArea"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport TextArea :
		public ContainerWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String TEXTOVERLAY;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		template<typename BaseClassType>
		friend class WidgetFactory;
	public:

		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);
		// Iterates through children to determine VirtualSpace
		virtual void _determineVirtualSize();
		/**
		* Function to update ScrollBars in terms of Slider length and visibility.
		*/
		virtual void _updateScrollBars();

		/**
		* Adds a character in front of the TextCursor, and increments the TextCursor
		* position.
		*/
		void addCharacter(Ogre::UTFString::code_point cp);
		/**
		* Adds text to this object.
		*/
		void addText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv);
		/**
		* Adds text to this object.
		*/
		void addText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv);
		/**
		* Adds text to this object.
		*/
		void addText(Ogre::UTFString s);
		/**
		* Adds Text using Text Segments.
		*/
		void addText(std::vector<TextSegment> segments);
		/**
		* Adds text to this object.
		*/
		void addTextLine(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv);
		/**
		* Adds text to this object.
		*/
		void addTextLine(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv);
		/**
		* Adds text to this object.
		*/
		void addTextLine(Ogre::UTFString s);
		/**
		* Adds Text using Text Segments.
		*/
		void addTextLine(std::vector<TextSegment> segments);

		/**
		* Clears the Text of this widget.
		*/
		void clearText();

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Returns the position of the index relative to the widget.
		*/
		Point getCursorIndexPosition(int index);
		/**
		* Gets the horizontal alignment of text.
		*/
		HorizontalTextAlignment getHorizontalAlignment();
		/**
		* Returns true if the text cannot be manipulated via input, false otherwise.
		*/
		bool getReadOnly();
		/**
		* Gets the TextCursor's skin type.
		*/
		Ogre::String getTextCursorSkinType();
		/**
		* Gets the text in UTFString form.
		*/
		Ogre::UTFString getText();

		/**
		* Returns true if the index is above the currently shown portion of text, false otherwise.
		*/
		bool isCursorIndexAboveViewableText(int index);
		/**
		* Returns true if the index is below the currently shown portion of text, false otherwise.
		*/
		bool isCursorIndexBelowViewableText(int index);
		/**
		* Returns true if the index is visible in the currently shown portion of text, false otherwise.
		*/
		bool isCursorIndexVisible(int index);

		/**
		* Moves Text Cursor to the character below the cursor.
		*/
		void moveCursorDown();
		/**
		* Moves Text Cursor to the character to the left of the cursor.
		*/
		void moveCursorLeft();
		/**
		* Moves Text Cursor to the character to the right of the cursor.
		*/
		void moveCursorRight();
		/**
		* Moves Text Cursor to the character above the cursor.
		*/
		void moveCursorUp();

		void onWindowDrawn(const EventArgs& args);

		/**
		* Removes a character from the text at the index given, and
		* positions the text cursor before that character.
		* NOTE: If the cursor index is -1 it will stay -1.
		*/
		void removeCharacter(int index);

		/**
		* Sets the cursor to the bottom of the text, scrolling the text into view.
		*/
		void scrollToBottom();
		/**
		* Sets the cursor to the top of the text, scrolling the text into view.
		*/
		void scrollToTop();
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
		* Sets the horizontal alignment of text.
		*/
		void setHorizontalAlignment(HorizontalTextAlignment a);
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
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv);
		/**
		* Sets the text for this object.
		*/
		void setText(Ogre::UTFString s);
		/**
		* Sets the Text using Text Segments.
		*/
		void setText(std::vector<TextSegment> segments);
		/**
		* Sets the TextCursor's skin type.
		*/
		void setTextCursorSkinType(const Ogre::String& skinTypeName);
		/**
		* If set, the TextCursor will not display, and text cannot be added or removed using mouse/keyboard input.
		*/
		void setReadOnly(bool readOnly);

		/**
		* Recalculate Client dimensions, relative to Widget's actual dimensions.
		*/
		virtual void updateClientDimensions();
		/**
		* Recalculate Screen and client dimensions and force a redrawing of the widget.
		*/
		virtual void updateTexturePosition();

	protected:
		TextArea(const Ogre::String& name);
		virtual ~TextArea();

		Text* mText;

		TextCursor* mTextCursor;
		int mCursorIndex;
		Point mCursorPosition;

		Point _convertScreenToTextCoordinates(const Point& p);

		/// Position of Text, used for scrolling support
		Point mTextPosition;

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
		TextAreaDesc* mDesc;

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

		virtual void _setScrollY(float y);

	private:
	};
}

#endif
