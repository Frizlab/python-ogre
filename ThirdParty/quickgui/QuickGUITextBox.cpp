#include "QuickGUITextBox.h"
#include "QuickGUIWindow.h"
#include "QuickGUIManager.h"
#include "QuickGUIRoot.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String TextBox::BACKGROUND = "background";
	const Ogre::String TextBox::TEXTOVERLAY = "textoverlay";

	void TextBox::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("TextBox");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(TEXTOVERLAY);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TextBox",d);
	}

	TextBoxDesc::TextBoxDesc() :
		WidgetDesc()
	{
		cursorBlinkTime = 0.5;
		defaultColor = Ogre::ColourValue::White;
		defaultFontName = Root::getSingleton().getDefaultFontName();
		keyDownTime = 0.6;
		keyRepeatTime = 0.04;
		maxCharacters = 255;
		// 42 happens to be the code point for * on regular english true type fonts
		maskSymbol = 42;
		maskText = false;

		textAlignment = TEXT_ALIGNMENT_LEFT;
		textCursorSkinTypeName = "default";
	}

	void TextBoxDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		b->IO("CursorBlinkTime",&cursorBlinkTime);
		b->IO("DefaultColor",&defaultColor);
		b->IO("DefaultFontName",&defaultFontName);
		b->IO("KeyDownTime",&keyDownTime);
		b->IO("KeyRepeatTime",&keyRepeatTime);
		b->IO("MaskSymbol",static_cast<unsigned short*>(&maskSymbol));
		b->IO("MaskText",&maskText);
		b->IO("MaxCharacters",&maxCharacters);
		b->IO("TextAlignment",&textAlignment);
		b->IO("TextCursorSkinTypeName",&textCursorSkinTypeName);
		b->IO("TextPosition",&textPosition);

		textDesc.serialize(b);
	}

	TextBox::TextBox(const Ogre::String& name) :
		Widget(name),
		mText(NULL),
		mDesc(NULL),
		mTextCursor(NULL),
		mTextInputValidatorSlot(NULL),
		mCursorIndex(0)
	{
		mSkinElementName = BACKGROUND;
	}

	TextBox::~TextBox()
	{
		TimerManager::getSingleton().destroyTimer(mBlinkTimer);
		TimerManager::getSingleton().destroyTimer(mKeyRepeatTimer);
		TimerManager::getSingleton().destroyTimer(mKeyDownTimer);

		delete mTextCursor;

		delete mText;

		delete mTextInputValidatorSlot;
	}

	void TextBox::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<TextBoxDesc*>(mWidgetDesc);

		TextBoxDesc* td = dynamic_cast<TextBoxDesc*>(d);

		mTextCursor = new TextCursor();
		mTextCursor->setSkinType(td->textCursorSkinTypeName);

		mDesc->consumeKeyboardEvents = true;

		mFunctionKeyDownLast = false;

		addWidgetEventHandler(WIDGET_EVENT_CHARACTER_KEY,&TextBox::onCharEntered,this);
		addWidgetEventHandler(WIDGET_EVENT_KEY_DOWN,&TextBox::onKeyDown,this);
		addWidgetEventHandler(WIDGET_EVENT_KEY_UP,&TextBox::onKeyUp,this);
		addWidgetEventHandler(WIDGET_EVENT_KEYBOARD_INPUT_GAIN,&TextBox::onKeyboardInputGain,this);
		addWidgetEventHandler(WIDGET_EVENT_KEYBOARD_INPUT_LOSE,&TextBox::onKeyboardInputLose,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&TextBox::onMouseButtonDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK_TRIPLE,&TextBox::onTripleClick,this);
		addWidgetEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,&TextBox::onVisibleChanged,this);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = td->textDesc;

		setDefaultFont(td->defaultFontName);
		setDefaultColor(td->defaultColor);
		mDesc->maxCharacters = td->maxCharacters;

		// Set a really high width, we want everything on 1 line.
		mDesc->textDesc.allottedWidth = mDesc->maxCharacters * Text::getGlyphWidth(mDesc->defaultFontName,'0');
			
		mText = new Text(mDesc->textDesc);

		mDesc->cursorBlinkTime = td->cursorBlinkTime;
		mDesc->keyDownTime = td->keyDownTime;
		mDesc->keyRepeatTime = td->keyRepeatTime;

		TimerDesc timerDesc;
		timerDesc.repeat = true;
		timerDesc.timePeriod = mDesc->cursorBlinkTime;
		mBlinkTimer = TimerManager::getSingleton().createTimer(timerDesc);
		mBlinkTimer->setCallback(&TextBox::blinkTimerCallback,this);

		timerDesc.timePeriod = mDesc->keyRepeatTime;
		mKeyRepeatTimer = TimerManager::getSingleton().createTimer(timerDesc);
		mKeyRepeatTimer->setCallback(&TextBox::keyRepeatTimerCallback,this);

		timerDesc.repeat = false;
		timerDesc.timePeriod = mDesc->keyDownTime;
		mKeyDownTimer = TimerManager::getSingleton().createTimer(timerDesc);
		mKeyDownTimer->setCallback(&TextBox::keyDownTimerCallback,this);

		setMaskText(td->maskText,td->maskSymbol);
		setMaxCharacters(td->maxCharacters);
		setSkinType(d->skinTypeName);
	}

	Widget* TextBox::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new TextBox(widgetName);

		newWidget->_createDescObject("TextBoxDesc");

		return newWidget;
	}

	Ogre::String TextBox::getClass()
	{
		return "TextBox";
	}

	void TextBox::addCharacter(Ogre::UTFString::code_point cp)
	{
		if(mText->getLength() >= static_cast<int>(mDesc->maxCharacters))
			return;

		if(mTextInputValidatorSlot != NULL)
			if(!mTextInputValidatorSlot->isInputValid(cp,mText->getLength() - 1,mText->getText()))
				return;

		mText->addCharacter(new Character(cp,mCurrentFont,mDesc->defaultColor),mCursorIndex);
		setCursorIndex(mCursorIndex+1);
	}

	void TextBox::blinkTimerCallback()
	{
		mTextCursor->setVisible(!mTextCursor->getVisible());
	}

	Ogre::ColourValue TextBox::getDefaultColor()
	{
		return mDesc->defaultColor;
	}

	Ogre::String TextBox::getDefaultFont()
	{
		return mDesc->defaultFontName;
	}

	Ogre::UTFString::code_point TextBox::getMaskSymbol()
	{
		return mDesc->maskSymbol;
	}

	bool TextBox::getMaskText()
	{
		return mDesc->maskText;
	}

	int TextBox::getMaxCharacters()
	{
		return mDesc->maxCharacters;
	}

	Ogre::UTFString TextBox::getText()
	{
		return mText->getText();
	}

	Ogre::String TextBox::getTextCursorSkinType()
	{
		return mDesc->textCursorSkinTypeName;
	}

	void TextBox::keyDownTimerCallback()
	{
		mKeyRepeatTimer->start();
	}

	void TextBox::keyRepeatTimerCallback()
	{
		if(mFunctionKeyDownLast)
			onKeyDown(mLastKnownInput);
		else
			onCharEntered(mLastKnownInput);
	}

	void TextBox::onCharEntered(const EventArgs& args)
	{
		const KeyEventArgs kea = dynamic_cast<const KeyEventArgs&>(args);
		mLastKnownInput.codepoint = kea.codepoint;
		mLastKnownInput.keyMask = kea.keyMask;
		mLastKnownInput.keyModifiers = kea.keyModifiers;

		addCharacter(mLastKnownInput.codepoint);
	}

	void TextBox::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);

		// Draw Background

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		Rect clipRegion = Rect(mTexturePosition,mClientDimensions.size);
		clipRegion.translate(mClientDimensions.position);

		Rect newClipRegion = prevClipRegion.getIntersection(clipRegion);
		brush->setClipRegion(newClipRegion);

		// Draw Text

		Point textPosition = clipRegion.position;
		textPosition.translate(mDesc->textPosition);
		mText->draw(textPosition);

		// Draw Text Overlay SkinElement

		brush->drawSkinElement(newClipRegion,st->getSkinElement(TEXTOVERLAY));

		// Restore clipping

		brush->setClipRegion(prevClipRegion);

		Brush::getSingleton().setColor(prevColor);
	}

	void TextBox::onKeyDown(const EventArgs& args)
	{
		const KeyEventArgs kea = dynamic_cast<const KeyEventArgs&>(args);
		mLastKnownInput.keyMask = kea.keyMask;
		mLastKnownInput.keyModifiers = kea.keyModifiers;
		mLastKnownInput.scancode = kea.scancode;

		mFunctionKeyDownLast = true;
		mKeyDownTimer->start();

		switch(kea.scancode)
		{
		case KC_LEFT:
			if(kea.keyModifiers & CTRL)
				setCursorIndex(mText->getIndexOfPreviousWord(mCursorIndex));
			else
				setCursorIndex(mCursorIndex - 1);
			break;
		case KC_RIGHT:
			if(kea.keyModifiers & CTRL)
				setCursorIndex(mText->getIndexOfNextWord(mCursorIndex));
			else
				setCursorIndex(mCursorIndex + 1);
			break;
		case KC_DELETE:
			removeCharacter(mCursorIndex);
			break;
		case KC_BACK:
			removeCharacter(mCursorIndex - 1);
			break;
		case KC_END:
			setCursorIndex(mText->getLength());
			break;
		case KC_HOME:
			setCursorIndex(0);
			break;
		default:
			mFunctionKeyDownLast = false;
			break;
		}
	}

	void TextBox::onKeyUp(const EventArgs& args)
	{
		mKeyDownTimer->stop();
		mKeyRepeatTimer->stop();
	}

	void TextBox::onKeyboardInputGain(const EventArgs& args)
	{
		mTextCursor->setVisible(true);
		mBlinkTimer->start();
	}

	void TextBox::onKeyboardInputLose(const EventArgs& args)
	{
		mBlinkTimer->stop();
		mTextCursor->setVisible(false);
	}

	void TextBox::onMouseButtonDown(const EventArgs& args)
	{
		const MouseEventArgs mea = dynamic_cast<const MouseEventArgs&>(args);
		if(mea.button == MB_Left)
		{
			// Convert position to coordinates relative to TextBox position
			Point relativePosition;
			relativePosition.x = mea.position.x - mTexturePosition.x;
			relativePosition.y = mea.position.y - mTexturePosition.y;

			// Convert relative TextBox position to coordinates relative to Text position
			Point relativeTextPosition;
			relativeTextPosition.x = relativePosition.x - mDesc->textPosition.x;
			relativeTextPosition.y = relativePosition.y - mDesc->textPosition.y;

			setCursorIndex(mText->getCursorIndexAtPosition(relativeTextPosition));
		}
	}

	void TextBox::onTripleClick(const EventArgs& args)
	{
		const MouseEventArgs mea = dynamic_cast<const MouseEventArgs&>(args);
		if(mea.button == MB_Left)
		{
			mText->highlight();
			redraw();
		}
	}

	void TextBox::onVisibleChanged(const EventArgs& args)
	{
		if(!mWidgetDesc->visible)
		{
			mBlinkTimer->stop();
			mTextCursor->setVisible(false);
		}
	}

	void TextBox::onWindowDrawn(const EventArgs& args)
	{
		mTextCursor->onDraw();
	}

	void TextBox::removeCharacter(int index)
	{
		if(index < 0)
			return;

		if(index >= mText->getLength())
			return;

		mText->removeCharacter(index);

		// Update index
		setCursorIndex(index);
	}

	void TextBox::setColor(const Ogre::ColourValue& cv)
	{
		mText->setColor(cv);

		redraw();
	}

	void TextBox::setColor(const Ogre::ColourValue& cv, unsigned int index)
	{
		mText->setColor(cv,index);

		redraw();
	}

	void TextBox::setColor(const Ogre::ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setColor(cv,startIndex,endIndex);

		redraw();
	}

	void TextBox::setColor(const Ogre::ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setColor(cv,c,allOccurrences);

		redraw();
	}

	void TextBox::setColor(const Ogre::ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setColor(cv,s,allOccurrences);

		redraw();
	}

	void TextBox::setCursorIndex(int index)
	{
		if(static_cast<int>(index) > mText->getLength())
			index = mText->getLength();
		else if(index < 0)
			index = 0;

		// Reset cursor blinking every time we move the cursor
		mBlinkTimer->reset();
		mTextCursor->setVisible(true);

		// Update CursorIndex
		mCursorIndex = index;

		// If text fits within TextBox, align text
		if(mText->getTextWidth() < mClientDimensions.size.width)
		{
			switch(mDesc->textAlignment)
			{
			case TEXT_ALIGNMENT_CENTER:
				mDesc->textPosition.x = (mClientDimensions.size.width - mText->getTextWidth()) / 2.0;
				break;
			case TEXT_ALIGNMENT_LEFT:
				mDesc->textPosition.x = 0;
				break;
			case TEXT_ALIGNMENT_RIGHT:
				mDesc->textPosition.x = mClientDimensions.size.width - mText->getTextWidth();
				break;
			}
		}
		// Else text is larger than TextBox bounds.  Ignore Text alignment property.
		else
		{
			// calculate the position of the desired index in relation to the textbox dimensions

			// X Position of cursor index relative to start of text
			Point relativeCursorPosition = mText->getPositionAtCharacterIndex(mCursorIndex);
			// X Position of cursor index relative to TextBox's top left client corner
			float indexPosition = (relativeCursorPosition.x + mDesc->textPosition.x);

			if(indexPosition < 0)
			{
				mDesc->textPosition.x -= indexPosition;
			}
			else if(indexPosition > mClientDimensions.size.width)
			{
				mDesc->textPosition.x -= (indexPosition - mClientDimensions.size.width);
			}
		}

		mCursorPosition.x = mDesc->textPosition.x + mText->getPositionAtCharacterIndex(mCursorIndex).x;

		// Position Cursor
		Point p = getScreenPosition();
		p.translate(mClientDimensions.position);
		p.translate(mCursorPosition);
		mTextCursor->setPosition(p);
		redraw();
	}

	void TextBox::setDefaultColor(const Ogre::ColourValue& cv)
	{
		mDesc->defaultColor = cv;
	}

	void TextBox::setDefaultFont(const Ogre::String& fontName)
	{
		mDesc->defaultFontName = fontName;
		mCurrentFont = Text::getFont(fontName);
	}

	void TextBox::setEnabled(bool enabled)
	{
		Widget::setEnabled(enabled);

		if(!mWidgetDesc->enabled)
		{
			if(mBlinkTimer != NULL)
				mBlinkTimer->stop();
			if(mTextCursor != NULL)
				mTextCursor->setVisible(false);

			if((mWidgetDesc->sheet != NULL) && (mWidgetDesc->sheet->getKeyboardListener() == this))
				mWidgetDesc->sheet->setKeyboardListener(NULL);
		}
	}

	void TextBox::setFont(const Ogre::String& fontName)
	{
		mText->setFont(fontName);

		redraw();
	}

	void TextBox::setFont(const Ogre::String& fontName, unsigned int index)
	{
		mText->setFont(fontName,index);

		redraw();
	}

	void TextBox::setFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setFont(fontName,startIndex,endIndex);

		redraw();
	}

	void TextBox::setFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setFont(fontName,c,allOccurrences);

		redraw();
	}

	void TextBox::setFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setFont(fontName,s,allOccurrences);

		redraw();
	}

	void TextBox::setMaskText(bool mask, Ogre::UTFString::code_point maskSymbol)
	{
		mDesc->maskText = mask;
		mDesc->maskSymbol = maskSymbol;

		mText->setMaskText(mask,maskSymbol);
	}

	void TextBox::setMaxCharacters(unsigned int max)
	{		
		if(static_cast<int>(max) < mDesc->textDesc.getTextLength())
		{
			throw Exception(Exception::ERR_TEXT,"Cannot set max Characters when text exceeds max! (Data loss)","TextBox::setMaxCharacters");
		}

		mDesc->maxCharacters = max;
	}

	void TextBox::setParent(Widget* parent)
	{
		Widget::setParent(parent);

		if(parent !=  NULL)
			mWindow->addWindowEventHandler(WINDOW_EVENT_DRAWN,&TextBox::onWindowDrawn,this);
		else
			mWindow->removeWindowEventHandler(WINDOW_EVENT_DRAWN,this);
	}

	void TextBox::setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mText->setText(s,fp,cv);

		redraw();
	}

	void TextBox::setTextCursorSkinType(const Ogre::String type)
	{
		mDesc->textCursorSkinTypeName = type;

		mTextCursor->setSkinType(type);
	}

	void TextBox::updateClientDimensions()
	{
		Widget::updateClientDimensions();

		if(mTextCursor != NULL)
			mTextCursor->setHeight(mClientDimensions.size.height);
	}

	void TextBox::updateTexturePosition()
	{
		Widget::updateTexturePosition();

		if(mDesc != NULL)
		{
			Point p = getScreenPosition();
			p.translate(mClientDimensions.position);
			p.translate(mCursorPosition);
			mTextCursor->setPosition(p);
		}
	}
}
