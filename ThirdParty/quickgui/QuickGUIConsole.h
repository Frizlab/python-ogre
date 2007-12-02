#ifndef QUICKGUICONSOLE_H
#define QUICKGUICONSOLE_H

#include "QuickGUIPrerequisites.h"
#include "QuickGUILabelArea.h"
#include "QuickGUITextBox.h"

#include <deque>

namespace QuickGUI
{
	class _QuickGUIExport Console :
		public LabelArea
	{
	public:
		friend class Panel;
	public:
		Console(const Ogre::String& name, GUIManager* gm);
		~Console();

		/**
		* Add user defined event that will be called when user presses Enter key with Textbox Activated.
		*/
		template<typename T> void addOnEnterPressedEventHandler(void (T::*function)(const EventArgs&), T* obj)
		{
			mInputBox->addOnEnterPressedEventHandler(new MemberFunctionPointer<T>(function,obj));
		}
		void addOnEnterPressedEventHandler(MemberFunctionSlot* function);

		void addText(const Ogre::UTFString& text);

		virtual void clearText();

		virtual void focus();

		int getMaxLines();

		void hideInputBox();

		/**
		* Sets the color of the text when the widget is disabled.
		*/
		virtual void setDisabledTextColor(const Ogre::ColourValue& c);
		virtual void setFont(const Ogre::String& fontScriptName, bool recursive = false);
		/**
		* Sets text horizontal alignment.
		*/
		virtual void setHorizontalAlignment(HorizontalAlignment ha);
		void setMaxLines(unsigned int maxLines);
		virtual void setText(const Ogre::UTFString& text);
		virtual void setTextColor(Ogre::ColourValue color);
		/**
		* Sets text vertical alignment.
		*/
		virtual void setVerticalAlignment(VerticalAlignment va);

		void showInputBox();

	protected:
		TextBox* mInputBox;

		// record input to support up/down toggling of previous input feature.
		std::deque<Ogre::String> mInputHistory;
		int inputIndex;
		void onKeyPressed(const EventArgs& args);

		// specifies the maximum number of lines of text the Console will store at any given time.
		int mMaxLines;

		void onInputBoxSizeChanged(const EventArgs& args);
		void onEnterPressed(const EventArgs& args);
	};
}

#endif
