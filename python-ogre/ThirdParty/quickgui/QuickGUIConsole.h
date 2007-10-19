#ifndef QUICKGUICONSOLE_H
#define QUICKGUICONSOLE_H

#include "QuickGUIMultiLineLabel.h"
#include "QuickGUITextBox.h"

namespace QuickGUI
{
	class Console :
		public MultiLineLabel
	{
	public:
		Console(const Ogre::String& instanceName, Size pixelSize, Ogre::String textureName, GUIManager* gm);
		~Console();

		void addText(const Ogre::UTFString& text);

		virtual void clearText();

		void hideInputBox();

		/**
		* Add user defined event that will be called when user presses Enter key with Textbox Activated.
		*/
		template<typename T> void addOnEnterPressedEventHandler(void (T::*function)(const EventArgs&), T* obj)
		{
			mInputBox->addOnEnterPressedEventHandler(new MemberFunctionPointer<T>(function,obj));
		}
		void addOnEnterPressedEventHandler(MemberFunctionSlot* function);

		/**
		* Sets the color of the text when the widget is disabled.
		*/
		virtual void setDisabledTextColor(const Ogre::ColourValue& c);
		/**
		* Sets text horizontal alignment.
		*/
		virtual void setHorizontalAlignment(HorizontalAlignment ha);
		virtual void setText(const Ogre::UTFString& text);
		virtual void setTextColor(Ogre::ColourValue color);
		/**
		* Sets text vertical alignment.
		*/
		virtual void setVerticalAlignment(VerticalAlignment va);

		void showInputBox();

	protected:
		TextBox* mInputBox;

		// specifies the maximum number of lines of text the Console will store at any given time.
		int mMaxLines;

		void onInputBoxSizeChanged(const EventArgs& args);
		void onEnterPressed(const EventArgs& args);
	};
}

#endif
