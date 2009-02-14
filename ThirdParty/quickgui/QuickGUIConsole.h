#ifndef QUICKGUICONSOLE_H
#define QUICKGUICONSOLE_H

#include "QuickGUIConsoleEnums.h"
#include "QuickGUIConsoleInputHandler.h"
#include "QuickGUIComponentWidget.h"
#include "QuickGUITextArea.h"
#include "QuickGUITextBox.h"
#include "QuickGUITextInputValidator.h"

namespace QuickGUI
{
	class _QuickGUIExport ConsoleDesc :
			public ComponentWidgetDesc
	{
	public:
		template<typename BaseClassType> friend class Factory;
	protected:
		ConsoleDesc(const Ogre::String& id);
		virtual ~ConsoleDesc() {}
	public:

		HorizontalTextAlignment console_displayAreaHorizontalAlignment;
		float console_inputBoxHeight;
		Ogre::ColourValue console_inputBoxDefaultColor;
		Ogre::String console_inputBoxDefaultFontName;
		HorizontalTextAlignment console_inputBoxHorizontalAlignment;
		ConsoleLayout console_layout;
		Ogre::String console_inputBoxTextCursorDefaultSkinTypeName;

		TextDesc textDesc;

		/**
		* Returns the class of Desc object this is.
		*/
		virtual Ogre::String getClass() { return "ConsoleDesc"; }
		/**
		* Returns the class of Widget this desc object is meant for.
		*/
		virtual Ogre::String getWidgetClass() { return "Console"; }

		/**
		* Restore properties to default values
		*/
		virtual void resetToDefault();

		/**
		* Outlines how the desc class is written to XML and read from XML.
		*/
		virtual void serialize(SerialBase* b);
	};

	class _QuickGUIExport Console :
		public ComponentWidget
	{
	public:
		// Skin Constants
		static const Ogre::String BACKGROUND;
		static const Ogre::String TEXTBOX;
		static const Ogre::String TEXTAREA;
		// Define Skin Structure
		static void registerSkinDefinition();
	public:
		friend class Factory<Widget>;
	public:
		
		/**
		* Internal function, do not use.
		*/
		virtual void _initialize(WidgetDesc* d);

		/**
		* Add an event handler called when characters are input into the InputBox.
		*/
		template<typename T> void addCharEnteredEventHandler(void (T::*function)(const EventArgs&), T* obj)
		{
			mInputBox->addWidgetEventHandler(WIDGET_EVENT_CHARACTER_KEY,function,obj);
		}
		void addCharEnteredEventHandler(EventHandlerSlot* function);
		/**
		* Adds text to this object.
		*/
		void addInputBoxText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv);
		/**
		* Adds text to this object.
		*/
		void addInputBoxText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv);
		/**
		* Adds text to this object.
		*/
		void addInputBoxText(Ogre::UTFString s);
		/**
		* Adds Text using Text Segments.
		*/
		void addInputBoxText(std::vector<TextSegment> segments);

		/**
		* Clears Display Area.
		*/
		void clearDisplayArea();
		/**
		* Clears Input Box.
		*/
		void clearInputBox();

		/**
		* Sets the Sheet's keyboardListener to the InputBox.
		*/
		void focusInputBox();

		/**
		* Returns the class name of this Widget.
		*/
		virtual Ogre::String getClass();
		/**
		* Gets the alignment of text in the Display Area.
		*/
		HorizontalTextAlignment getDisplayAreaHorizontalAlignment();
		/**
		* Gets the default color used for text in the input box.
		*/
		Ogre::ColourValue getInputBoxDefaultColor();
		/**
		* Gets the default font used for text in the input box.
		*/
		Ogre::String getInputBoxDefaultFont();
		/**
		* Gets the height of the Input Box in pixels.
		*/
		float getInputBoxHeight();
		/**
		* Gets the alignment of text in the Input Box.
		*/
		HorizontalTextAlignment getInputBoxHorizontalAlignment();
		/**
		* Gets the InputBox's Text.
		*/
		Ogre::UTFString getInputBoxText();
		/**
		* Gets the InputBox's TextCursor's skin type.
		*/
		Ogre::String getInputBoxTextCursorSkinType();
		/**
		* Gets the InputBox's TextSegments.
		*/
		std::vector<TextSegment> getInputBoxTextSegments();		

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
		template<typename T> void setConsoleInputHandler(void (T::*ConsoleInputHandler)(Console* console, bool& clearInputBox, bool& addToDisplayArea), T* obj)
		{
			if(mConsoleInputHandlerSlot != NULL)
				OGRE_DELETE_T(mConsoleInputHandlerSlot,ConsoleInputHandlerSlot,Ogre::MEMCATEGORY_GENERAL);

			mConsoleInputHandlerSlot = OGRE_NEW_T(ConsoleInputHandlerPointer<T>,Ogre::MEMCATEGORY_GENERAL)(ConsoleInputHandler,obj);
		}
		/**
		* Sets the alignment of text in the Display Area.
		*/
		void setDisplayAreaHorizontalAlignment(HorizontalTextAlignment a);
		/**
		* Sets the default color used for text in the input box.
		*/
		void setInputBoxDefaultColor(const Ogre::ColourValue& cv);
		/**
		* Sets the default font used for text in the input box.
		*/
		void setInputBoxDefaultFont(const Ogre::String& fontName);
		/**
		* Sets the height of the Input Box in pixels.
		*/
		void setInputBoxHeight(float heightInPixels);
		/**
		* Gets the alignment of text in the Input Box.
		*/
		void setInputBoxHorizontalAlignment(HorizontalTextAlignment a);
		/**
		* Sets the text for this object.
		*/
		void setInputBoxText(Ogre::UTFString s);
		/**
		* Sets the text for this object.
		*/
		void setInputBoxText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv);
		/**
		* Sets the Text using Text Segments.
		*/
		void setInputBoxText(std::vector<TextSegment> segments);
		/**
		* Sets the SkinType of the TextCursor.
		*/
		void setInputBoxTextCursorSkinType(const Ogre::String& skinTypeName);
		/**
		* Sets whether the input box is on top of the display area or below it.
		*/
		void setLayout(ConsoleLayout l);
		/**
		* Sets the "type" of this widget.  For example you
		* can create several types of Button widgets: "close", "add", "fire.skill.1", etc.
		* NOTE: The type property determines what is drawn to the screen.
		*/
		virtual void setSkinType(const Ogre::String type);

	protected:
		Console(const Ogre::String& name);
		virtual ~Console();

		// Pointer pointing to mWidgetDesc object, but casted for quick use.
		ConsoleDesc* mDesc;

		ConsoleInputHandlerSlot* mConsoleInputHandlerSlot;

		TextBox* mInputBox;
		TextArea* mDisplayArea;

		void _updateComponentDimensions();

		void onTextSubmitted(const EventArgs& args);

		/**
		* Outlines how the widget is drawn to the current render target
		*/
		virtual void onDraw();
	};
}

#endif
