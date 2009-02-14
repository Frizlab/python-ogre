#include "QuickGUITextArea.h"
#include "QuickGUIWindow.h"
#include "QuickGUIManager.h"
#include "QuickGUIRoot.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String TextArea::BACKGROUND = "background";
	const Ogre::String TextArea::TEXTOVERLAY = "textoverlay";

	void TextArea::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TextArea");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(TEXTOVERLAY);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TextArea",d);
	}

	TextAreaDesc::TextAreaDesc(const Ogre::String& id) :
		ContainerWidgetDesc(id)
	{
		resetToDefault();
	}

	void TextAreaDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		textarea_cursorBlinkTime = 0.5;
		textarea_defaultColor = Ogre::ColourValue::White;
		textarea_defaultFontName = Root::getSingleton().getDefaultFontName();
		textarea_keyDownTime = 0.6;
		textarea_keyRepeatTime = 0.04;
		textarea_maxCharacters = 255;
		textarea_readOnly = false;
		textarea_textCursorDefaultSkinTypeName = "default";

		textDesc.resetToDefault();
	}

	void TextAreaDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		b->IO("CursorBlinkTime",&textarea_cursorBlinkTime);
		b->IO("DefaultColor",&textarea_defaultColor);
		b->IO("DefaultFontName",&textarea_defaultFontName);
		b->IO("KeyDownTime",&textarea_keyDownTime);
		b->IO("KeyRepeatTime",&textarea_keyRepeatTime);
		b->IO("MaxCharacters",&textarea_maxCharacters);
		b->IO("TextCursorDefaultSkinTypeName",&textarea_textCursorDefaultSkinTypeName);

		textDesc.serialize(b);
	}

	TextArea::TextArea(const Ogre::String& name) :
		ContainerWidget(name),
		mText(NULL),
		mDesc(NULL),
		mTextCursor(NULL),
		mCursorIndex(0),
		mBlinkTimer(NULL)
	{
		mSkinElementName = BACKGROUND;

		addWidgetEventHandler(WIDGET_EVENT_CHARACTER_KEY,&TextArea::onCharEntered,this);
		addWidgetEventHandler(WIDGET_EVENT_KEY_DOWN,&TextArea::onKeyDown,this);
		addWidgetEventHandler(WIDGET_EVENT_KEY_UP,&TextArea::onKeyUp,this);
		addWidgetEventHandler(WIDGET_EVENT_KEYBOARD_INPUT_GAIN,&TextArea::onKeyboardInputGain,this);
		addWidgetEventHandler(WIDGET_EVENT_KEYBOARD_INPUT_LOSE,&TextArea::onKeyboardInputLose,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&TextArea::onMouseButtonDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK_TRIPLE,&TextArea::onTripleClick,this);
		addWidgetEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,&TextArea::onVisibleChanged,this);

		mTextCursor = OGRE_NEW_T(TextCursor,Ogre::MEMCATEGORY_GENERAL)(this);
	}

	TextArea::~TextArea()
	{
		TimerManager::getSingleton().destroyTimer(mBlinkTimer);
		TimerManager::getSingleton().destroyTimer(mKeyRepeatTimer);
		TimerManager::getSingleton().destroyTimer(mKeyDownTimer);

		OGRE_DELETE_T(mTextCursor,TextCursor,Ogre::MEMCATEGORY_GENERAL);
		OGRE_DELETE_T(mText,Text,Ogre::MEMCATEGORY_GENERAL);
	}

	Point TextArea::_convertScreenToTextCoordinates(const Point& p)
	{
		// Convert position to coordinates relative to TextBox position
		Point relativePosition;
		relativePosition.x = p.x - mTexturePosition.x;
		relativePosition.y = p.y - mTexturePosition.y;

		// Convert relative TextBox position to coordinates relative to client area
		relativePosition -= mClientDimensions.position;

		// Convert relative client position to coordinates relative to Text position
		Point relativeTextPosition;
		relativeTextPosition.x = relativePosition.x - mTextPosition.x;
		relativeTextPosition.y = relativePosition.y - mTextPosition.y;

		return relativeTextPosition;
	}

	void TextArea::_initialize(WidgetDesc* d)
	{
		TextAreaDesc* td = dynamic_cast<TextAreaDesc*>(d);

		// Create Timers before calling ContainerWidget::_initialize.  Some functions like setEnabled affect timers.
		TimerDesc timerDesc;
		timerDesc.repeat = true;
		timerDesc.timePeriod = td->textarea_cursorBlinkTime;
		mBlinkTimer = TimerManager::getSingleton().createTimer(timerDesc);
		mBlinkTimer->setCallback(&TextArea::blinkTimerCallback,this);

		timerDesc.timePeriod = td->textarea_keyRepeatTime;
		mKeyRepeatTimer = TimerManager::getSingleton().createTimer(timerDesc);
		mKeyRepeatTimer->setCallback(&TextArea::keyRepeatTimerCallback,this);

		timerDesc.repeat = false;
		timerDesc.timePeriod = td->textarea_keyDownTime;
		mKeyDownTimer = TimerManager::getSingleton().createTimer(timerDesc);
		mKeyDownTimer->setCallback(&TextArea::keyDownTimerCallback,this);

		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<TextAreaDesc*>(mWidgetDesc);

		mTextCursor->setHeight(Text::getFontHeight(td->textarea_defaultFontName));
		mTextCursor->setSkinType(td->textarea_textCursorDefaultSkinTypeName);

		mDesc->widget_consumeKeyboardEvents = true;
		mCursorIndex = -1;

		mFunctionKeyDownLast = false;

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = td->textDesc;
		mDesc->textDesc.allottedWidth = td->widget_dimensions.size.width;

		setDefaultFont(td->textarea_defaultFontName);
		setDefaultColor(td->textarea_defaultColor);
		mDesc->textarea_maxCharacters = td->textarea_maxCharacters;

		setSkinType(d->widget_skinTypeName);

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
		mDesc->textDesc.allottedWidth = td->widget_dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
		mText = OGRE_NEW_T(Text,Ogre::MEMCATEGORY_GENERAL)(mDesc->textDesc);

		mDesc->textarea_cursorBlinkTime = td->textarea_cursorBlinkTime;
		mDesc->textarea_keyDownTime = td->textarea_keyDownTime;
		mDesc->textarea_keyRepeatTime = td->textarea_keyRepeatTime;
		mDesc->textarea_readOnly = td->textarea_readOnly;
	}

	void TextArea::_determineVirtualSize()
	{
		mVirtualSize = mClientDimensions.size;

		if(mText != NULL)
		{
			float textHeight = mText->getTextHeight();

			if(textHeight > mVirtualSize.height)
				mVirtualSize.height = textHeight;
		}
	}

	void TextArea::_setScrollY(float y)
	{
		if(!mDesc->containerwidget_supportScrollBars)
			return;

		mDesc->containerwidget_yScrollOffset = (y * mVirtualSize.height);
		mTextPosition.y = -(y * mVirtualSize.height);
		mTextPosition.roundUp();

		// Make sure TextCursor scrolls with the text
		if(mText != NULL)
		{
			// Position Cursor
			mCursorPosition = mTextPosition + mText->getPositionAtCharacterIndex(mCursorIndex);

			Point p = getScreenPosition();
			p.translate(mClientDimensions.position);
			p.translate(mCursorPosition);
			mTextCursor->setPosition(p);
		}

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			(*it)->setScrollY(mDesc->containerwidget_yScrollOffset);
	}

	void TextArea::_updateScrollBars()
	{
		ContainerWidget::_updateScrollBars();

		if(mText != NULL)
			mText->setAllottedWidth(mClientDimensions.size.width);
	}

	void TextArea::addCharacter(Ogre::UTFString::code_point cp)
	{
		mText->addCharacter(OGRE_NEW_T(Character,Ogre::MEMCATEGORY_GENERAL)(cp,mCurrentFont,mDesc->textarea_defaultColor),mCursorIndex);

		_determineVirtualSize();
		_updateScrollBars();
		
		setCursorIndex(mCursorIndex+1);
	}

	void TextArea::addText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mText->addText(s,fp,cv);

		_determineVirtualSize();
		_updateScrollBars();

		scrollToBottom();

		redraw();
	}

	void TextArea::addText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv)
	{
		addText(s,Text::getFont(fontName),cv);
	}

	void TextArea::addText(Ogre::UTFString s)
	{
		addText(s,mDesc->textarea_defaultFontName,mDesc->textarea_defaultColor);
	}

	void TextArea::addText(std::vector<TextSegment> segments)
	{
		mText->addText(segments);

		_determineVirtualSize();
		_updateScrollBars();

		scrollToBottom();

		redraw();
	}

	void TextArea::addTextLine(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mText->addTextLine(s,fp,cv);

		_determineVirtualSize();
		_updateScrollBars();

		scrollToBottom();

		redraw();
	}

	void TextArea::addTextLine(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv)
	{
		addTextLine(s,Text::getFont(fontName),cv);
	}

	void TextArea::addTextLine(Ogre::UTFString s)
	{
		addTextLine(s,mDesc->textarea_defaultFontName,mDesc->textarea_defaultColor);
	}

	void TextArea::addTextLine(std::vector<TextSegment> segments)
	{
		mText->addTextLine(segments);

		_determineVirtualSize();
		_updateScrollBars();

		scrollToBottom();

		redraw();
	}

	void TextArea::blinkTimerCallback()
	{
		mTextCursor->setVisible(!mTextCursor->getVisible());
	}

	void TextArea::clearText()
	{
		mText->clearText();

		_determineVirtualSize();
		_updateScrollBars();
	}

	Ogre::String TextArea::getClass()
	{
		return "TextArea";
	}

	Point TextArea::getCursorIndexPosition(int index)
	{
		Point charPos = mText->getCharacterPosition(index);
		charPos += mTextPosition;

		return charPos;
	}

	HorizontalTextAlignment TextArea::getHorizontalAlignment()
	{
		return mText->getHorizontalTextAlignment();
	}

	bool TextArea::getReadOnly()
	{
		return mDesc->textarea_readOnly;
	}

	Rect TextArea::getScreenRect()
	{
		return mTextBoxClipRegion;
	}

	Ogre::UTFString TextArea::getText()
	{
		return mText->getText();
	}

	Ogre::String TextArea::getTextCursorSkinType()
	{
		return mDesc->textarea_textCursorDefaultSkinTypeName;
	}

	bool TextArea::isCursorIndexAboveViewableText(int index)
	{
		if(static_cast<int>(index) > mText->getLength())
			index = mText->getLength();
		else if(index < 0)
			index = 0;

		// Get characters position within text
		float charYPos = mText->getCharacterYPosition(index);
		charYPos += mTextPosition.y;

		if(charYPos < 0)
			return true;
		else
			return false;
	}

	bool TextArea::isCursorIndexBelowViewableText(int index)
	{
		if(static_cast<int>(index) > mText->getLength())
			index = mText->getLength();
		else if(index < 0)
			index = 0;

		// Get characters position within text
		float charYPos = mText->getCharacterYPosition(index);
		charYPos += mTextPosition.y;
		// Make sure to include bottom of character when querying if cursor is within view
		Character* c = mText->getCharacter(index);
		charYPos += c->dimensions.size.height;
		// Add a 2 pixel buffer
		charYPos += 2;

		if(charYPos >= mClientDimensions.size.height)
			return true;
		else
			return false;
	}

	bool TextArea::isCursorIndexVisible(int index)
	{
		if(isCursorIndexAboveViewableText(index))
			return false;
		else if(isCursorIndexBelowViewableText(index))
			return false;
		else
			return true;
	}

	void TextArea::keyDownTimerCallback()
	{
		mKeyRepeatTimer->start();
	}

	void TextArea::keyRepeatTimerCallback()
	{
		if(mFunctionKeyDownLast)
			onKeyDown(mLastKnownInput);
		else
			onCharEntered(mLastKnownInput);
	}

	void TextArea::moveCursorDown()
	{
		Point globalPos = mTextCursor->getPosition();
		globalPos.x += (mTextCursor->getSize().width / 2.0);
		// Position is at the top left of cursor, we want the position that is below the cursor.
		// Increment y by cursor height, and then increment again by half cursor height. (1.5)
		globalPos.y += (mTextCursor->getSize().height * 1.5);

		setCursorIndex(mText->getCursorIndexAtPosition(_convertScreenToTextCoordinates(globalPos)));
	}

	void TextArea::moveCursorLeft()
	{
		setCursorIndex(mCursorIndex - 1);
	}

	void TextArea::moveCursorRight()
	{
		setCursorIndex(mCursorIndex + 1);
	}

	void TextArea::moveCursorUp()
	{
		Point globalPos = mTextCursor->getPosition();
		globalPos.x += (mTextCursor->getSize().width / 2.0);
		globalPos.y -= (mTextCursor->getSize().height / 2.0);

		setCursorIndex(mText->getCursorIndexAtPosition(_convertScreenToTextCoordinates(globalPos)));
	}

	void TextArea::onCharEntered(const EventArgs& args)
	{
		if(mDesc->textarea_readOnly)
			return;

		const KeyEventArgs kea = dynamic_cast<const KeyEventArgs&>(args);
		mLastKnownInput.codepoint = kea.codepoint;
		mLastKnownInput.keyMask = kea.keyMask;
		mLastKnownInput.keyModifiers = kea.keyModifiers;

		addCharacter(mLastKnownInput.codepoint);
	}

	void TextArea::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		// Draw Background

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		Rect textArea = Rect(mTexturePosition,mClientDimensions.size);
		textArea.translate(mClientDimensions.position);

		mTextBoxClipRegion = prevClipRegion.getIntersection(textArea);
		brush->setClipRegion(mTextBoxClipRegion);

		textArea.translate(mTextPosition);

		// Draw text
		mText->draw(textArea.position);

		// Draw Text Overlay SkinElement

		brush->drawSkinElement(mTextBoxClipRegion,st->getSkinElement(TEXTOVERLAY));

		// Restore clipping

		brush->setClipRegion(prevClipRegion);

		brush->setColor(prevColor);
	}

	void TextArea::onKeyDown(const EventArgs& args)
	{
		if(mDesc->textarea_readOnly)
			return;

		const KeyEventArgs kea = dynamic_cast<const KeyEventArgs&>(args);
		mLastKnownInput.keyMask = kea.keyMask;
		mLastKnownInput.keyModifiers = kea.keyModifiers;
		mLastKnownInput.scancode = kea.scancode;

		mFunctionKeyDownLast = true;
		mKeyDownTimer->start();

		switch(kea.scancode)
		{
		case KC_BACK:
			removeCharacter(mCursorIndex - 1);
			break;
		case KC_DELETE:
			removeCharacter(mCursorIndex);
			break;
		case KC_DOWN:
			moveCursorDown();
			break;
		case KC_END:
			if(kea.keyModifiers & CTRL)
				setCursorIndex(mText->getLength());
			else
				setCursorIndex(mText->getIndexOfTextLineEnd(mCursorIndex));
			break;
		case KC_LEFT:
			if(kea.keyModifiers & CTRL)
				setCursorIndex(mText->getIndexOfPreviousWord(mCursorIndex));
			else
				setCursorIndex(mCursorIndex - 1);
			break;
		case KC_HOME:
			if(kea.keyModifiers & CTRL)
				setCursorIndex(0);
			else
				setCursorIndex(mText->getIndexOfTextLineBegin(mCursorIndex));
			break;
		case KC_RETURN:
			addCharacter(Text::UNICODE_CR);
			break;
		case KC_RIGHT:
			if(kea.keyModifiers & CTRL)
				setCursorIndex(mText->getIndexOfNextWord(mCursorIndex));
			else
				setCursorIndex(mCursorIndex + 1);
			break;
		case KC_UP:
			moveCursorUp();
			break;
		case KC_LSHIFT:
		case KC_RSHIFT:
		case KC_LCONTROL:
		case KC_RCONTROL:
			break;
		default:
			mFunctionKeyDownLast = false;
			break;
		}
	}

	void TextArea::onKeyUp(const EventArgs& args)
	{
		if(mDesc->textarea_readOnly)
			return;

		mKeyDownTimer->stop();
		mKeyRepeatTimer->stop();
	}

	void TextArea::onKeyboardInputGain(const EventArgs& args)
	{
		if(mDesc->textarea_readOnly)
			return;

		mTextCursor->setVisible(true);
		mBlinkTimer->start();
	}

	void TextArea::onKeyboardInputLose(const EventArgs& args)
	{
		if(mDesc->textarea_readOnly)
			return;

		mBlinkTimer->stop();
		mTextCursor->setVisible(false);
	}

	void TextArea::onMouseButtonDown(const EventArgs& args)
	{
		if(mDesc->textarea_readOnly)
			return;

		const MouseEventArgs mea = dynamic_cast<const MouseEventArgs&>(args);
		if(mea.button == MB_Left)
		{
			mText->clearHighlights();
			setCursorIndex(mText->getCursorIndexAtPosition(_convertScreenToTextCoordinates(mea.position)));
		}
	}

	void TextArea::onTripleClick(const EventArgs& args)
	{
		if(mDesc->textarea_readOnly)
			return;

		const MouseEventArgs mea = dynamic_cast<const MouseEventArgs&>(args);
		if(mea.button == MB_Left)
		{
			mText->highlight();
			redraw();
		}
	}

	void TextArea::onVisibleChanged(const EventArgs& args)
	{
		if(!mWidgetDesc->widget_visible)
		{
			mBlinkTimer->stop();
			mTextCursor->setVisible(false);
		}
	}

	void TextArea::onWindowDrawn(const EventArgs& args)
	{
		mTextCursor->onDraw();
	}

	void TextArea::removeCharacter(int index)
	{
		if(index < 0)
			return;

		if(index >= mText->getLength())
			return;

		mText->removeCharacter(index);

		_determineVirtualSize();
		_updateScrollBars();

		// Update index
		setCursorIndex(index);
	}

	void TextArea::scrollToBottom()
	{
		setCursorIndex(mText->getLength());
	}

	void TextArea::scrollToTop()
	{
		setCursorIndex(0);
	}

	void TextArea::setCursorIndex(int index)
	{
		if(static_cast<int>(index) > mText->getLength())
			index = mText->getLength();
		else if(index < 0)
			index = 0;

		// Reset cursor blinking every time we move the cursor
		if(!mDesc->textarea_readOnly && (mDesc->sheet->getKeyboardListener() == this))
		{
			mBlinkTimer->reset();
			mTextCursor->setVisible(true);
		}

		// Update Cursor Height to match the font of the character to the left of cursor
		if(index == 0)
			mTextCursor->setHeight(Text::getFontHeight(mDesc->textarea_defaultFontName));
		else
			mTextCursor->setHeight(Text::getFontHeight(mText->getCharacter(static_cast<unsigned int>(index - 1))->fontPtr));

		// Update CursorIndex
		mCursorIndex = index;

		// See if Scrolling of Text needs to occur
		if(isCursorIndexAboveViewableText(mCursorIndex))
		{
			if(mVScrollBar != NULL)
				mVScrollBar->setPercentage(mText->getCharacterYPosition(mCursorIndex) / mVirtualSize.height);
		}
		else if(isCursorIndexBelowViewableText(mCursorIndex))
		{
			if(mVScrollBar != NULL)
			{
				TextLine* tl = mText->getTextLineFromIndex(mCursorIndex);
				// I throw in a buffer of 2 pixels, so floating point rounding won't be so bad
				mVScrollBar->setPercentage((mText->getCharacterYPosition(mCursorIndex) + tl->getHeight() + 2 - mClientDimensions.size.height) / mVirtualSize.height);
			}
		}

		// Position Cursor
		mCursorPosition = mTextPosition + mText->getPositionAtCharacterIndex(mCursorIndex);

		Point p = getScreenPosition();
		p.translate(mClientDimensions.position);
		p.translate(mCursorPosition);
		mTextCursor->setPosition(p);
		redraw();
	}

	void TextArea::setDefaultColor(const Ogre::ColourValue& cv)
	{
		mDesc->textarea_defaultColor = cv;
	}

	void TextArea::setDefaultFont(const Ogre::String& fontName)
	{
		mDesc->textarea_defaultFontName = fontName;
		mCurrentFont = Text::getFont(fontName);
	}

	void TextArea::setHorizontalAlignment(HorizontalTextAlignment a)
	{
		mText->setHorizontalTextAlignment(a);

		redraw();
	}

	void TextArea::setMaxCharacters(unsigned int max)
	{
		if(max == mDesc->textarea_maxCharacters)
			return;
		
		if(max < mDesc->textarea_maxCharacters)
		{
			throw Exception(Exception::ERR_TEXT,"Cannot set max Characters when text exceeds max! (Data loss)","TextBox::setMaxCharacters");
		}

		mDesc->textarea_maxCharacters = max;
	}

	void TextArea::setParent(Widget* parent)
	{
		ContainerWidget::setParent(parent);

		if(mWindow != NULL)
		{
			if(parent !=  NULL)
				mWindow->addWindowEventHandler(WINDOW_EVENT_DRAWN,&TextArea::onWindowDrawn,this);
			else
				mWindow->removeWindowEventHandler(WINDOW_EVENT_DRAWN,this);
		}
	}

	void TextArea::setReadOnly(bool readOnly)
	{
		mDesc->textarea_readOnly = readOnly;

		if(mDesc->textarea_readOnly)
		{
			mTextCursor->setVisible(false);
			mBlinkTimer->stop();
		}
	}

	void TextArea::setTextCursorSkinType(const Ogre::String& skinTypeName)
	{
		mDesc->textarea_textCursorDefaultSkinTypeName = skinTypeName;

		mTextCursor->setSkinType(skinTypeName);
	}

	void TextArea::setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mText->setText(s,fp,cv);

		_determineVirtualSize();
		_updateScrollBars();

		scrollToBottom();

		redraw();
	}

	void TextArea::setText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv)
	{
		setText(s,Text::getFont(fontName),cv);
	}

	void TextArea::setText(Ogre::UTFString s)
	{
		setText(s,mDesc->textarea_defaultFontName,mDesc->textarea_defaultColor);
	}

	void TextArea::setText(std::vector<TextSegment> segments)
	{
		mText->setText(segments);

		_determineVirtualSize();
		_updateScrollBars();

		scrollToBottom();

		redraw();
	}

	void TextArea::updateClientDimensions()
	{
		ContainerWidget::updateClientDimensions();

		if(mText != NULL)
			mText->setAllottedWidth(mClientDimensions.size.width);

		redraw();
	}

	void TextArea::updateTexturePosition()
	{
		Widget::updateTexturePosition();

		if(mDesc != NULL)
		{
			Point p = getScreenPosition();
			p.translate(Point(0,0));
			p.translate(mCursorPosition);
			mTextCursor->setPosition(p);
		}
	}
}
