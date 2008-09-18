#include "QuickGUITextBox.h"
#include "QuickGUIWindow.h"
#include "QuickGUIManager.h"
#include "QuickGUIRoot.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String TextBox::BACKGROUND = "background";

	void TextBox::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("TextBox");
		d->defineSkinElement(BACKGROUND);
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

		for(int i = 0; i < PADDING_COUNT; ++i)
		{
			padding[i] = 5.0;
		}

		textAlignment = TEXT_ALIGNMENT_LEFT;
	}

	void TextBoxDesc::serialize(SerialBase* b)
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
		b->IO("TextPosition",&textPosition);

		textDesc.serialize(b);
	}

	TextBox::TextBox(const Ogre::String& name) :
		Widget(name),
		mText(NULL),
		mDesc(NULL),
		mTextCursor(NULL)
	{
		mSkinElementName = BACKGROUND;
	}

	TextBox::~TextBox()
	{
		TimerManager::getSingleton().destroyTimer(mBlinkTimer);
		TimerManager::getSingleton().destroyTimer(mKeyRepeatTimer);
		TimerManager::getSingleton().destroyTimer(mKeyDownTimer);

		delete mTextCursor;

		/*********** RYAN COMMENT
		if(mWindow != NULL)
		{
			if(mWindow->hasEventHandler(WINDOW_EVENT_DRAWN,this))
				mWindow->removeEventHandler(WINDOW_EVENT_DRAWN,this);
		}
		*/

		delete mText;
	}

	void TextBox::_initialize(WidgetDesc* d)
	{
		Widget::_initialize(d);

		mDesc = dynamic_cast<TextBoxDesc*>(mWidgetDesc);

		mTextCursor = new TextCursor();

		mDesc->consumeKeyboardEvents = true;
		mCursorIndex = -1;

		mFunctionKeyDownLast = false;

		addWidgetEventHandler(WIDGET_EVENT_CHARACTER_KEY,&TextBox::onCharEntered,this);
		addWidgetEventHandler(WIDGET_EVENT_KEY_DOWN,&TextBox::onKeyDown,this);
		addWidgetEventHandler(WIDGET_EVENT_KEY_UP,&TextBox::onKeyUp,this);
		addWidgetEventHandler(WIDGET_EVENT_KEYBOARD_INPUT_GAIN,&TextBox::onKeyboardInputGain,this);
		addWidgetEventHandler(WIDGET_EVENT_KEYBOARD_INPUT_LOSE,&TextBox::onKeyboardInputLose,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&TextBox::onMouseButtonDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK_TRIPLE,&TextBox::onTripleClick,this);

		TextBoxDesc* td = dynamic_cast<TextBoxDesc*>(d);

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = td->textDesc;

		setDefaultFont(td->defaultFontName);
		setDefaultColor(td->defaultColor);
		mDesc->maxCharacters = td->maxCharacters;

		// Set a really high width, we want everything on 1 line.
		mDesc->textDesc.allottedWidth = mDesc->maxCharacters * Text::getGlyphWidth(mDesc->defaultFontName,'0');
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
		mBlinkTimer->setCallback(&TextBox::blinkTimerCallback,this);

		timerDesc.timePeriod = mDesc->keyRepeatTime;
		mKeyRepeatTimer = TimerManager::getSingleton().createTimer(timerDesc);
		mKeyRepeatTimer->setCallback(&TextBox::keyRepeatTimerCallback,this);

		timerDesc.repeat = false;
		timerDesc.timePeriod = mDesc->keyDownTime;
		mKeyDownTimer = TimerManager::getSingleton().createTimer(timerDesc);
		mKeyDownTimer->setCallback(&TextBox::keyDownTimerCallback,this);

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
		mText->addCharacter(new Character(cp,mCurrentFont,mDesc->defaultColor));
		setCursorIndex(-1);
	}

	void TextBox::addCharacter(Ogre::UTFString::code_point cp, int index)
	{
		if((index < 0) || (index > mText->getLength()))
			throw Exception(Exception::ERR_TEXT,"Index out of bounds! index=" + Ogre::StringConverter::toString(index) + " length=" + Ogre::StringConverter::toString(mText->getLength()),"TextBox::addCharacter");

		mText->addCharacter(new Character(cp,mCurrentFont,mDesc->defaultColor), index);
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

	int TextBox::getMaxCharacters()
	{
		return mDesc->maxCharacters;
	}

	float TextBox::getPadding(Padding p)
	{
		if(p == PADDING_COUNT)
			throw Exception(Exception::ERR_INVALIDPARAMS,"PADDING_COUNT is not a valid parameter!","Label::getPadding");

		return mDesc->padding[p];
	}

	Ogre::UTFString TextBox::getText()
	{
		return mText->getText();
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

		if(mCursorIndex == -1)
			addCharacter(mLastKnownInput.codepoint);
		else
			addCharacter(mLastKnownInput.codepoint,mCursorIndex);
	}

	void TextBox::onDraw()
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

		Point textPosition = mTexturePosition;
		textPosition.translate(Point(0,mDesc->padding[PADDING_TOP]));
		textPosition.translate(mDesc->textPosition);
		mText->draw(textPosition);

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
			// If the text is empty, set cursor index to -1
			if(mText->empty())
				setCursorIndex(-1);
			else
			{
				// Convert position to coordinates relative to TextBox position
				Point relativePosition;
				relativePosition.x = mea.position.x - mTexturePosition.x;
				relativePosition.y = mea.position.y - mTexturePosition.y;

				// Convert relative TextBox position to coordinates relative to Text position
				Point relativeTextPosition;
				relativeTextPosition.x = relativePosition.x - mDesc->textPosition.x;
				relativeTextPosition.y = relativePosition.y - mDesc->textPosition.y;

				Character* lastCharacter = mText->getCharacter(mText->getLength()-1);
				// If we click to the right of the text, set cursor index to -1
				if((relativeTextPosition.x > (lastCharacter->dimensions.position.x + (lastCharacter->dimensions.size.width / 2.0))) && (relativeTextPosition.y >= lastCharacter->dimensions.position.y))
					setCursorIndex(-1);
				else
					setCursorIndex(mText->getCharacterIndexAtPosition(relativeTextPosition));
			}
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

	void TextBox::onWindowDrawn(const EventArgs& args)
	{
		mTextCursor->onDraw();
	}

	void TextBox::removeCharacter(int index)
	{
		if((index < 0) || (index > mText->getLength()))
			throw Exception(Exception::ERR_TEXT,"Index out of bounds! index=" + Ogre::StringConverter::toString(index) + " length=" + Ogre::StringConverter::toString(mText->getLength()),"TextBox::removeCharacter");

		mText->removeCharacter(index);

		if(index == mText->getLength())
			setCursorIndex(-1);
		else
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
		if((index < -1) || (index > (mText->getLength() - 1)))
			throw Exception(Exception::ERR_TEXT,"Index out of bounds! index=" + Ogre::StringConverter::toString(index) + " length=" + Ogre::StringConverter::toString(mText->getLength()),"TextBox::setCursorIndex");

		mCursorIndex = index;

		mBlinkTimer->reset();
		mTextCursor->setVisible(true);

		if(mText->getLength() == 0)
		{
			mDesc->textPosition.x = mDesc->padding[PADDING_LEFT];
			mCursorPosition.x = mDesc->textPosition.x;

			Point p = getScreenPosition();
			p.translate(mCursorPosition);
			mTextCursor->setPosition(p);
			redraw();

			return;
		}

		float relativeCursorPosition;
		if(mCursorIndex == -1)
		{
			Character* c = mText->getCharacter(mText->getLength() - 1);
			relativeCursorPosition = c->dimensions.position.x + c->dimensions.size.width;
		}
		else
			relativeCursorPosition = mText->getCharacter(mCursorIndex)->dimensions.position.x;

		float textBoxWidth = (mDesc->dimensions.size.width - mDesc->padding[PADDING_LEFT] - mDesc->padding[PADDING_RIGHT]);

		// If text fits within TextBox, align text
		if(mText->getTextWidth() < textBoxWidth)
		{
			switch(mDesc->textAlignment)
			{
			case TEXT_ALIGNMENT_CENTER:
				mDesc->textPosition.x = (textBoxWidth - mText->getTextWidth()) / 2.0;
				break;
			case TEXT_ALIGNMENT_LEFT:
				mDesc->textPosition.x = mDesc->padding[PADDING_LEFT];
				break;
			case TEXT_ALIGNMENT_RIGHT:
				mDesc->textPosition.x = (mDesc->dimensions.size.width - mDesc->padding[PADDING_RIGHT]) - mText->getTextWidth();
				break;
			}
		}
		// Else text is larger than TextBox bounds.  Ignore Text alignment property.
		else
		{
			// Case 1: if the distance between the cursor and beggining of text is less than 
			// half the text box size, left align the text.
			if(relativeCursorPosition < (textBoxWidth / 2.0))
			{
				mDesc->textPosition.x = mDesc->padding[PADDING_LEFT];
			}
			// Case 2: if the distance between the cursor and end of text is less than
			// half the text box size, right align the text.
			else if((mText->getTextWidth() - relativeCursorPosition) < (textBoxWidth / 2.0))
			{
				mDesc->textPosition.x = (mDesc->dimensions.size.width - mDesc->padding[PADDING_RIGHT]) - mText->getTextWidth();
			}
			// Case 3: Center the cursor with the TextBox and position the Text accordingly
			else
			{
				mDesc->textPosition.x = (textBoxWidth / 2.0) - relativeCursorPosition;
			}
		}

		mCursorPosition.x = mDesc->textPosition.x + relativeCursorPosition;

		Point p = getScreenPosition();
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

	void TextBox::setMaxCharacters(unsigned int max)
	{
		if(max == mDesc->maxCharacters)
			return;
		
		if(max < mDesc->maxCharacters)
		{
			throw Exception(Exception::ERR_TEXT,"Cannot set max Characters when text exceeds max! (Data loss)","TextBox::setMaxCharacters");
		}

		mDesc->maxCharacters = max;
	}

	void TextBox::setPadding(Padding p, float distance)
	{
		if(p == PADDING_COUNT)
			throw Exception(Exception::ERR_INVALIDPARAMS,"PADDING_COUNT is not a valid parameter!","Label::setPadding");

		mDesc->padding[p] = distance;

		redraw();
	}

	void TextBox::setParent(Widget* parent)
	{
		Widget::setParent(parent);

		mWindow->addWindowEventHandler(WINDOW_EVENT_DRAWN,&TextBox::onWindowDrawn,this);
	}

	void TextBox::setText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mText->setText(s,fp,cv);

		redraw();
	}

	void TextBox::updateTexturePosition()
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
