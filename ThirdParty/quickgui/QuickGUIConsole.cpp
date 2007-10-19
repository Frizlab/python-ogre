#include "QuickGUIConsole.h"

namespace QuickGUI
{
	Console::Console(const Ogre::String& instanceName, Size pixelSize, Ogre::String textureName, GUIManager* gm) :
		MultiLineLabel(instanceName,pixelSize,textureName,gm),
		mMaxLines(30)
	{
		mWidgetType = TYPE_CONSOLE;

		mInputBox = new TextBox(mInstanceName+".InputBox",Size(mSize.width,0),"qgui.textbox.png",mGUIManager);
		addChild(mInputBox);
		mInputBox->setPosition(0,mSize.height - mInputBox->getHeight());
		mInputBox->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		mInputBox->setVerticalAnchor(ANCHOR_VERTICAL_BOTTOM);
		mInputBox->addEventHandler(EVENT_SIZE_CHANGED,&Console::onInputBoxSizeChanged,this);
		mInputBox->addOnEnterPressedEventHandler(&Console::onEnterPressed,this);

		// assumption that the InputBox top border is 5 pixels or less.
		mTextList->setHeight(mSize.height - mInputBox->getHeight() - 5);
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

		// Caption Iterator
		int textIndex = 0;
		// Index denoting the end of a line that can fix within the widgets width.
		int lineEnd = 0;

		while( textIndex < static_cast<int>(mCaption.size()) )
		{
			lineEnd = _getLine(textIndex);

			TextBox* tb = mTextList->addTextBox();
			tb->setTexture("");
			tb->setReadOnly(true);
			tb->setText(mCaption.substr(textIndex,lineEnd - textIndex));

			textIndex = lineEnd + 1;
		}

		int numLinesOfText = mTextList->getNumberOfItems();
		while(numLinesOfText > mMaxLines)
			mTextList->removeItem(0);

		mTextList->getScrollPane()->scrollIntoView(mTextList->getItem(mTextList->getNumberOfItems() - 1));
	}

	void Console::clearText()
	{
		MultiLineLabel::clearText();
		mInputBox->clearText();
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
	}

	void Console::onInputBoxSizeChanged(const EventArgs& args)
	{
		mTextList->setHeight(mSize.height - mInputBox->getHeight());
		mInputBox->setYPosition(mSize.height - mInputBox->getHeight());
	}

	void Console::setDisabledTextColor(const Ogre::ColourValue& c)
	{
		MultiLineLabel::setDisabledTextColor(c);
		mInputBox->setDisabledTextColor(c);
	}

	void Console::setHorizontalAlignment(HorizontalAlignment ha)
	{
		MultiLineLabel::setHorizontalAlignment(ha);
		mInputBox->setHorizontalAlignment(ha);
	}

	void Console::setText(const Ogre::UTFString& text)
	{
		mInputBox->setText(text);
	}

	void Console::setTextColor(Ogre::ColourValue color)
	{
		MultiLineLabel::setTextColor(color);
		mInputBox->setTextColor(color);
	}

	void Console::setVerticalAlignment(VerticalAlignment va)
	{
		MultiLineLabel::setVerticalAlignment(va);
		mInputBox->setVerticalAlignment(va);
	}

	void Console::showInputBox()
	{
		mInputBox->show();
		mInputBox->setShowWithParent(true);
	}
}
