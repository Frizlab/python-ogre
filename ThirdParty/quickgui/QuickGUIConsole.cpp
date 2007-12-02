#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUIConsole.h"
#include "QuickGUIManager.h" 

namespace QuickGUI
{
	Console::Console(const Ogre::String& name, GUIManager* gm) :
		LabelArea(name,gm),
		mMaxLines(30),
		mInputBox(0),
		inputIndex(-1)
	{
		mWidgetType = TYPE_CONSOLE;
		mSkinComponent = ".console";
		mSize = Size(200,100);

		mInputBox = dynamic_cast<TextBox*>(_createComponent(mInstanceName+".InputBox",TYPE_TEXTBOX));
		mInputBox->setWidth(mSize.width);
		mInputBox->setPosition(0,mSize.height - mInputBox->getHeight());
		mInputBox->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		mInputBox->setVerticalAnchor(ANCHOR_VERTICAL_BOTTOM);
		mInputBox->setUseBorders(true);
		mInputBox->addEventHandler(EVENT_SIZE_CHANGED,&Console::onInputBoxSizeChanged,this);
		mInputBox->addEventHandler(EVENT_KEY_DOWN,&Console::onKeyPressed,this);
		mInputBox->addOnEnterPressedEventHandler(&Console::onEnterPressed,this);

		// assumption that the InputBox top border is 5 pixels or less.
		mTextList->setHeight(mSize.height - mInputBox->getHeight() - 5);

		// Create borders.
		setUseBorders(true);
	}

	Console::~Console()
	{
	}

	void Console::addOnEnterPressedEventHandler(MemberFunctionSlot* function)
	{
		mInputBox->addOnEnterPressedEventHandler(function);
	}

	void Console::addText(const Ogre::UTFString& text)
	{
		mCaption = text;
		mInputHistory.push_back(mCaption);

		// Caption Iterator
		int textIndex = 0;
		// Index denoting the end of a line that can fix within the widgets width.
		int lineEnd = 0;

		while( textIndex < static_cast<int>(mCaption.size()) )
		{
			lineEnd = _getLine(textIndex);

			TextBox* tb = mTextList->addTextBox();
			tb->setReadOnly(true);
			tb->setText(mCaption.substr(textIndex,lineEnd - textIndex));

			textIndex = lineEnd + 1;
		}

		int numLinesOfText = mTextList->getNumberOfItems();
		while(numLinesOfText > mMaxLines)
		{
			mTextList->removeItem(0);
			--numLinesOfText;
			mInputHistory.pop_front();
		}

		mTextList->getScrollPane()->scrollIntoView(mTextList->getItem(mTextList->getNumberOfItems() - 1));
	}

	void Console::clearText()
	{
		LabelArea::clearText();
		mInputBox->clearText();
	}

	void Console::focus()
	{
		mGUIManager->setActiveWidget(mInputBox);
	}

	int Console::getMaxLines()
	{
		return mMaxLines;
	}

	void Console::hideInputBox()
	{
		mInputBox->hide();
		mInputBox->setShowWithParent(false);
	}

	void Console::onEnterPressed(const EventArgs& args)
	{
		addText(mInputBox->getText());
		mInputBox->clearText();
		// set input index to the most recent input.
		inputIndex = static_cast<int>(mInputHistory.size());
	}

	void Console::onInputBoxSizeChanged(const EventArgs& args)
	{
		mTextList->setHeight(mSize.height - mInputBox->getHeight());
		mInputBox->setYPosition(mSize.height - mInputBox->getHeight());
	}

	void Console::setDisabledTextColor(const Ogre::ColourValue& c)
	{
		LabelArea::setDisabledTextColor(c);
		mInputBox->setDisabledTextColor(c);
	}

	void Console::setHorizontalAlignment(HorizontalAlignment ha)
	{
		LabelArea::setHorizontalAlignment(ha);
		mInputBox->setHorizontalAlignment(ha);
	}

	void Console::setMaxLines(unsigned int maxLines)
	{
		mMaxLines = maxLines;
	}

	void Console::setFont(const Ogre::String& fontScriptName, bool recursive)
	{
		if(fontScriptName == "")
			return;

		mTextHelper->setFont(fontScriptName);
		Widget::setFont(fontScriptName,recursive);
	}

	void Console::setText(const Ogre::UTFString& text)
	{
		if(mInputBox != NULL)
			mInputBox->setText(text);
	}

	void Console::setTextColor(Ogre::ColourValue color)
	{
		LabelArea::setTextColor(color);
		mInputBox->setTextColor(color);
	}

	void Console::setVerticalAlignment(VerticalAlignment va)
	{
		LabelArea::setVerticalAlignment(va);
		mInputBox->setVerticalAlignment(va);
	}

	void Console::showInputBox()
	{
		mInputBox->show();
		mInputBox->setShowWithParent(true);
	}

	void Console::onKeyPressed(const EventArgs& args)
	{
		const KeyEventArgs kea = dynamic_cast<const KeyEventArgs&>(args);
		
		// Only interested in key up and down.
		if(kea.scancode == KC_UP)
		{
			if(mInputHistory.empty() || (inputIndex <= 0))
				return;

			--inputIndex;
		}
		else if(kea.scancode == KC_DOWN)
		{
			if(mInputHistory.empty() || (inputIndex >= static_cast<int>(mInputHistory.size()) - 1))
			{
				inputIndex = static_cast<int>(mInputHistory.size());
				mInputBox->clearText();
				return;
			}

			++inputIndex;
		}
		else
			return;

		mInputBox->setText(mInputHistory[inputIndex]);
	}
}
