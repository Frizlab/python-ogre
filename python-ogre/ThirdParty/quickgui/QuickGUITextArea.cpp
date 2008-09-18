#include "QuickGUITextArea.h"
#include "QuickGUIWindow.h"
#include "QuickGUIManager.h"
#include "QuickGUIRoot.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String TextArea::BACKGROUND = "background";

	void TextArea::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("TextArea");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TextArea",d);
	}

	TextAreaDesc::TextAreaDesc() :
		WidgetDesc()
	{
		cursorBlinkTime = 0.5;
		defaultColor = Ogre::ColourValue::White;
		defaultFontName = Root::getSingleton().getDefaultFontName();
		keyDownTime = 0.6;
		keyRepeatTime = 0.04;
		maxCharacters = 255;

		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			padding[i] = 5.0;
		}

		textAlignment = TEXT_ALIGNMENT_LEFT;
	}

	void TextAreaDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			b->IO(StringConverter::toString(static_cast<Padding>(i)),&padding[i]);
		}

		b->IO("CursorBlinkTime",&cursorBlinkTime);
		b->IO("DefaultColor",&defaultColor);
		b->IO("DefaultFontName",&defaultFontName);
		b->IO("KeyDownTime",&keyDownTime);
		b->IO("KeyRepeatTime",&keyRepeatTime);
		b->IO("MaxCharacters",&maxCharacters);
		b->IO("TextAlignment",&textAlignment);

		textDesc.serialize(b);
	}

	TextArea::TextArea(const Ogre::String& name) :
		Widget(name),
		mText(NULL),
		mDesc(NULL),
		mTextCursor(NULL)
	{
		mSkinElementName = BACKGROUND;
	}

	TextArea::~TextArea()
	{
		TimerManager::getSingleton().destroyTimer(mBlinkTimer);
		TimerManager::getSingleton().destroyTimer(mKeyRepeatTimer);
		TimerManager::getSingleton().destroyTimer(mKeyDownTimer);

		delete mTextCursor;
		delete mText;
	}

	void TextArea::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<TextAreaDesc*>(mWidgetDesc);

		mTextCursor = new TextCursor();

		mDesc->consumeKeyboardEvents = true;
		mCursorIndex = -1;

		mFunctionKeyDownLast = false;

		addWidgetEventHandler(WIDGET_EVENT_CHARACTER_KEY,&TextArea::onCharEntered,this);
		addWidgetEventHandler(WIDGET_EVENT_KEY_DOWN,&TextArea::onKeyDown,this);
		addWidgetEventHandler(WIDGET_EVENT_KEY_UP,&TextArea::onKeyUp,this);
		addWidgetEventHandler(WIDGET_EVENT_KEYBOARD_INPUT_GAIN,&TextArea::onKeyboardInputGain,this);
		addWidgetEventHandler(WIDGET_EVENT_KEYBOARD_INPUT_LOSE,&TextArea::onKeyboardInputLose,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK_TRIPLE,&TextArea::onTripleClick,this);

		TextAreaDesc* td = dynamic_cast<TextAreaDesc*>(d);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = td->textDesc;

		setDefaultFont(td->defaultFontName);
		setDefaultColor(td->defaultColor);
		mDesc->maxCharacters = td->maxCharacters;

		// Set a really high width, we want everything on 1 line.
		mDesc->textDesc.allottedWidth = mDesc->dimensions.size.width - mDesc->padding[PADDING_LEFT] - mDesc->padding[PADDING_RIGHT];

		if(mText != NULL)
			delete mText;

		mText = new Text(mDesc->textDesc);

		setPadding(PADDING_BOTTOM,td->padding[PADDING_BOTTOM]);
		setPadding(PADDING_LEFT,td->padding[PADDING_LEFT]);
		setPadding(PADDING_RIGHT,td->padding[PADDING_RIGHT]);
		setPadding(PADDING_TOP,td->padding[PADDING_TOP]);

		mDesc->cursorBlinkTime = td->cursorBlinkTime;
		mDesc->keyDownTime = td->keyDownTime;
		mDesc->keyRepeatTime = td->keyRepeatTime;

		TimerDesc timerDesc;
		timerDesc.repeat = true;
		timerDesc.timePeriod = mDesc->cursorBlinkTime;
		mBlinkTimer = TimerManager::getSingleton().createTimer(timerDesc);
		mBlinkTimer->setCallback(&TextArea::blinkTimerCallback,this);

		timerDesc.timePeriod = mDesc->keyRepeatTime;
		mKeyRepeatTimer = TimerManager::getSingleton().createTimer(timerDesc);
		mKeyRepeatTimer->setCallback(&TextArea::keyRepeatTimerCallback,this);

		timerDesc.repeat = false;
		timerDesc.timePeriod = mDesc->keyDownTime;
		mKeyDownTimer = TimerManager::getSingleton().createTimer(timerDesc);
		mKeyDownTimer->setCallback(&TextArea::keyDownTimerCallback,this);

		setSkinType(d->skinTypeName);
	}

	Widget* TextArea::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new TextArea(widgetName);

		newWidget->_createDescObject("TextAreaDesc");

		return newWidget;
	}

	Ogre::String TextArea::getClass()
	{
		return "TextArea";
	}

	Ogre::UTFString TextArea::getText()
	{
		return mText->getText();
	}

	void TextArea::addCharacter(Ogre::UTFString::code_point cp)
	{
		mText->addCharacter(new Character(cp,mCurrentFont,mDesc->defaultColor));
		setCursorIndex(-1);
	}

	void TextArea::addCharacter(Ogre::UTFString::code_point cp, int index)
	{
		if((index < 0) || (index > mText->getLength()))
			throw Exception(Exception::ERR_TEXT,"Index out of bounds! index=" + Ogre::StringConverter::toString(index) + " length=" + Ogre::StringConverter::toString(mText->getLength()),"TextBox::addCharacter");

		mText->addCharacter(new Character(cp,mCurrentFont,mDesc->defaultColor), index);
		setCursorIndex(mCursorIndex+1);
	}

	void TextArea::blinkTimerCallback()
	{
		mTextCursor->setVisible(!mTextCursor->getVisible());
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

	void TextArea::onCharEntered(const EventArgs& args)
	{
		const KeyEventArgs kea = dynamic_cast<const KeyEventArgs&>(args);
		mLastKnownInput.codepoint = kea.codepoint;
		mLastKnownInput.keyMask = kea.keyMask;
		mLastKnownInput.keyModifiers = kea.keyModifiers;

		if(mCursorIndex == -1)
			addCharacter(mLastKnownInput.codepoint);
		else
			addCharacter(mLastKnownInput.codepoint,mCursorIndex);
	}

	void TextArea::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));

		Ogre::ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		Rect clipRegion;
		clipRegion.size = 
			Size(
				mDesc->dimensions.size.width - mDesc->padding[PADDING_RIGHT] - mDesc->padding[PADDING_LEFT],
				mDesc->dimensions.size.height - mDesc->padding[PADDING_BOTTOM] - mDesc->padding[PADDING_TOP]);
		clipRegion.position = mTexturePosition;
		clipRegion.translate(Point(mDesc->padding[PADDING_LEFT],mDesc->padding[PADDING_TOP]));

		brush->setClipRegion(prevClipRegion.getIntersection(clipRegion));

		mText->draw(clipRegion.position);

		brush->setClipRegion(prevClipRegion);

		Brush::getSingleton().setColor(prevColor);
	}

	void TextArea::onKeyDown(const EventArgs& args)
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
			if(mText->getLength() > 0)
			{
				if(mCursorIndex == -1)
				{
					if(kea.keyModifiers & CTRL)
						setCursorIndex(mText->getIndexOfPreviousWord(mText->getLength() - 1));
					else
						setCursorIndex(mText->getLength() - 1);
				}
				else if(mCursorIndex > 0)
				{
					if(kea.keyModifiers & CTRL)
						setCursorIndex(mText->getIndexOfPreviousWord(mCursorIndex));
					else
						setCursorIndex(mCursorIndex - 1);
				}
			}
			break;
		case KC_RIGHT:
			if((mText->getLength() > 0) && (mCursorIndex != -1))
			{
				if(mCursorIndex == (mText->getLength() - 1))
				{
					if(kea.keyModifiers & CTRL)
						setCursorIndex(mText->getIndexOfNextWord(mCursorIndex));
					else
						setCursorIndex(-1);
				}
				else
				{
					if(kea.keyModifiers & CTRL)
						setCursorIndex(mText->getIndexOfNextWord(mCursorIndex));
					else
						setCursorIndex(mCursorIndex + 1);
				}
			}
			break;
		case KC_DELETE:
			if(mText->getLength() > 0)
			{
				if(mCursorIndex != -1)
					removeCharacter(mCursorIndex);
			}
			break;
		case KC_BACK:
			if(mText->getLength() > 0)
			{
				if(mCursorIndex == -1)
					removeCharacter(mText->getLength() - 1);
				else if(mCursorIndex > 0)
					removeCharacter(mCursorIndex - 1);
			}
			break;
		case KC_END:
			if(!mText->empty())
				setCursorIndex(-1);
			break;
		case KC_HOME:
			if(!mText->empty())
				setCursorIndex(0);
			break;
		case KC_A:
			mFunctionKeyDownLast = true;
			break;
		default:
			mFunctionKeyDownLast = false;
			break;
		}
	}

	void TextArea::onKeyUp(const EventArgs& args)
	{
		mKeyDownTimer->stop();
		mKeyRepeatTimer->stop();
	}

	void TextArea::onKeyboardInputGain(const EventArgs& args)
	{
		mTextCursor->setVisible(true);
		mBlinkTimer->start();
	}

	void TextArea::onKeyboardInputLose(const EventArgs& args)
	{
		mBlinkTimer->stop();
		mTextCursor->setVisible(false);
	}

	void TextArea::onTripleClick(const EventArgs& args)
	{
		const MouseEventArgs mea = dynamic_cast<const MouseEventArgs&>(args);
		if(mea.button == MB_Left)
		{
			mText->highlight();
			redraw();
		}
	}

	void TextArea::onWindowDrawn(const EventArgs& args)
	{
		mTextCursor->onDraw();
	}

	void TextArea::removeCharacter(int index)
	{
		if((index < 0) || (index > mText->getLength()))
			throw Exception(Exception::ERR_TEXT,"Index out of bounds! index=" + Ogre::StringConverter::toString(index) + " length=" + Ogre::StringConverter::toString(mText->getLength()),"TextBox::removeCharacter");

		mText->removeCharacter(index);

		if(index == mText->getLength())
			setCursorIndex(-1);
		else
			setCursorIndex(index);
	}

	void TextArea::setCursorIndex(int index)
	{
		if((index < -1) || (index > (mText->getLength() - 1)))
			throw Exception(Exception::ERR_TEXT,"Index out of bounds! index=" + Ogre::StringConverter::toString(index) + " length=" + Ogre::StringConverter::toString(mText->getLength()),"TextBox::setCursorIndex");

		mCursorIndex = index;

		mBlinkTimer->reset();
		mTextCursor->setVisible(true);

		if(mText->getLength() == 0)
		{
			mCursorPosition.x = mDesc->padding[PADDING_LEFT];

			Point p = getScreenPosition();
			p.translate(mCursorPosition);
			mTextCursor->setPosition(p);
			redraw();

			return;
		}

		Point relativeCursorPosition;
		if(mCursorIndex == -1)
		{
			Character* c = mText->getCharacter(mText->getLength() - 1);
			relativeCursorPosition.x = c->dimensions.position.x + c->dimensions.size.width;
			relativeCursorPosition.y = mText->getCharacterYPosition(mText->getLength() - 1);
		}
		else
		{
			relativeCursorPosition.x = mText->getCharacter(mCursorIndex)->dimensions.position.x;
			relativeCursorPosition.y = mText->getCharacterYPosition(mCursorIndex);
		}

		mCursorPosition.x = mDesc->padding[PADDING_LEFT] + relativeCursorPosition.x;
		mCursorPosition.y = relativeCursorPosition.y;

		Point p = getScreenPosition();
		p.translate(mCursorPosition);
		mTextCursor->setPosition(p);
		redraw();
	}

	void TextArea::setDefaultColor(const Ogre::ColourValue& cv)
	{
		mDesc->defaultColor = cv;
	}

	void TextArea::setDefaultFont(const Ogre::String& fontName)
	{
		mDesc->defaultFontName = fontName;
		mCurrentFont = Text::getFont(fontName);
	}

	void TextArea::setMaxCharacters(unsigned int max)
	{
		if(max == mDesc->maxCharacters)
			return;
		
		if(max < mDesc->maxCharacters)
		{
			throw Exception(Exception::ERR_TEXT,"Cannot set max Characters when text exceeds max! (Data loss)","TextBox::setMaxCharacters");
		}

		mDesc->maxCharacters = max;
	}

	void TextArea::setPadding(Padding p, float distance)
	{
		if(p == PADDING_COUNT)
			throw Exception(Exception::ERR_INVALIDPARAMS,"PADDING_COUNT is not a valid parameter!","Label::setPadding");

		mDesc->padding[p] = distance;

		redraw();
	}

	void TextArea::setParent(Widget* parent)
	{
		Widget::setParent(parent);

		mWindow->addWindowEventHandler(WINDOW_EVENT_DRAWN,&TextArea::onWindowDrawn,this);
	}

	void TextArea::updateTexturePosition()
	{
		Widget::updateTexturePosition();

		if(mDesc != NULL)
		{
			Point p = getScreenPosition();
			p.translate(Point(mDesc->padding[PADDING_LEFT],0));
			p.translate(mCursorPosition);
			mTextCursor->setPosition(p);
		}
	}
}
