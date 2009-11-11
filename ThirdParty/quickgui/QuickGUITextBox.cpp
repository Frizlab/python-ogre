/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

#include "QuickGUITextBox.h"
#include "QuickGUIWindow.h"
#include "QuickGUIManager.h"
#include "QuickGUIRoot.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUISkinDefinition.h"
#include "QuickGUISheet.h"
#include "QuickGUICharacter.h"
#include "QuickGUIWindow.h"
#include "QuickGUIDescManager.h"

#include "OgreFont.h"

namespace QuickGUI
{
	const Ogre::String TextBox::BACKGROUND = "background";
	const Ogre::String TextBox::TEXTOVERLAY = "textoverlay";

	void TextBox::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TextBox");
		d->defineSkinElement(BACKGROUND);
		d->defineSkinElement(TEXTOVERLAY);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TextBox",d);
	}

	TextBoxDesc::TextBoxDesc() :
		WidgetDesc()
	{
		resetToDefault();
	}

	void TextBoxDesc::resetToDefault()
	{
		WidgetDesc::resetToDefault();

		widget_dimensions.size = Size(100,25);

		textbox_cursorBlinkTime = 0.5;
		textbox_defaultColor = Root::getSingleton().getDefaultColor();
		textbox_defaultFontName = Root::getSingleton().getDefaultFontName();
		textbox_horizontalPadding = 2;
		textbox_maxCharacters = 255;
		// 42 happens to be the code point for * on regular english true type fonts
		textbox_maskSymbol = 42;
		textbox_maskText = false;
		textbox_textCursorDefaultSkinTypeName = "default";
		textDesc.resetToDefault();
	}

	void TextBoxDesc::serialize(SerialBase* b)
	{
		WidgetDesc::serialize(b);

		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		TextBoxDesc* defaultValues = DescManager::getSingleton().createDesc<TextBoxDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("CursorBlinkTime",				&textbox_cursorBlinkTime,								defaultValues->textbox_cursorBlinkTime);
		b->IO("DefaultColor",					&textbox_defaultColor,									defaultValues->textbox_defaultColor);
		b->IO("DefaultFontName",				&textbox_defaultFontName,								defaultValues->textbox_defaultFontName);
		b->IO("TBHorizontalPadding",			&textbox_horizontalPadding,								defaultValues->textbox_horizontalPadding);
		b->IO("MaskSymbol",						static_cast<unsigned short*>(&textbox_maskSymbol),		defaultValues->textbox_maskSymbol);
		b->IO("MaskText",						&textbox_maskText,										defaultValues->textbox_maskText);
		b->IO("MaxCharacters",					&textbox_maxCharacters,									defaultValues->textbox_maxCharacters);
		b->IO("TextCursorDefaultSkinTypeName",	&textbox_textCursorDefaultSkinTypeName,					defaultValues->textbox_textCursorDefaultSkinTypeName);
		b->IO("TextPosition",					&textbox_textPosition,									defaultValues->textbox_textPosition);

		DescManager::getSingleton().destroyDesc(defaultValues);

		textDesc.serialize(b);
	}

	TextBox::TextBox(const Ogre::String& name) :
		Widget(name),
		mText(NULL),
		mDesc(NULL),
		mTextCursor(NULL),
		mTextInputValidatorSlot(NULL),
		mCursorIndex(0),
		mBlinkTimer(NULL)
	{
		addWidgetEventHandler(WIDGET_EVENT_CHARACTER_KEY,&TextBox::onCharEntered,this);
		addWidgetEventHandler(WIDGET_EVENT_KEY_DOWN,&TextBox::onKeyDown,this);
		addWidgetEventHandler(WIDGET_EVENT_KEYBOARD_INPUT_GAIN,&TextBox::onKeyboardInputGain,this);
		addWidgetEventHandler(WIDGET_EVENT_KEYBOARD_INPUT_LOSE,&TextBox::onKeyboardInputLose,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&TextBox::onMouseButtonDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_CLICK_TRIPLE,&TextBox::onTripleClick,this);
		addWidgetEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,&TextBox::onVisibleChanged,this);

		mTextCursor = OGRE_NEW_T(TextCursor,Ogre::MEMCATEGORY_GENERAL)(this);
	}

	TextBox::~TextBox()
	{
		if(mWindow != NULL)
			mWindow->removeWindowEventHandler(WINDOW_EVENT_DRAWN,this);

		TimerManager::getSingleton().destroyTimer(mBlinkTimer);

		OGRE_DELETE_T(mTextCursor,TextCursor,Ogre::MEMCATEGORY_GENERAL);

		OGRE_DELETE_T(mText,Text,Ogre::MEMCATEGORY_GENERAL);

		OGRE_DELETE_T(mTextInputValidatorSlot,TextInputValidatorSlot,Ogre::MEMCATEGORY_GENERAL);
	}

	void TextBox::_initialize(WidgetDesc* d)
	{
		TextBoxDesc* td = dynamic_cast<TextBoxDesc*>(d);

		// Create Timers before calling Widget::_initialize.  Some functions like setEnabled affect timers.
		TimerDesc timerDesc;
		timerDesc.repeat = true;
		timerDesc.timePeriod = td->textbox_cursorBlinkTime;
		mBlinkTimer = TimerManager::getSingleton().createTimer(timerDesc);
		mBlinkTimer->setCallback(&TextBox::blinkTimerCallback,this);

		Widget::_initialize(d);

		mDesc = dynamic_cast<TextBoxDesc*>(mWidgetDesc);

		mTextCursor->setSkinType(td->textbox_textCursorDefaultSkinTypeName);

		mDesc->widget_consumeKeyboardEvents = true;

		// Make a copy of the Text Desc.  The Text object will
		// modify it directly, which is used for serialization.
		mDesc->textDesc = td->textDesc;

		setDefaultFont(td->textbox_defaultFontName);
		setDefaultColor(td->textbox_defaultColor);
		mDesc->textbox_maxCharacters = td->textbox_maxCharacters;

		// Disable text wrapping, we want everything on 1 line.
		mDesc->textDesc.allottedSize.width = 0;
		mDesc->textDesc.allottedSize.height = mDesc->widget_dimensions.size.height;
			
		mText = OGRE_NEW_T(Text,Ogre::MEMCATEGORY_GENERAL)(mDesc->textDesc);

		mDesc->textbox_cursorBlinkTime = td->textbox_cursorBlinkTime;
		mDesc->textbox_horizontalPadding = td->textbox_horizontalPadding;

		setMaskText(td->textbox_maskText,td->textbox_maskSymbol);
		setMaxCharacters(td->textbox_maxCharacters);
		setSkinType(d->widget_skinTypeName);
		setHorizontalTextAlignment(mDesc->textDesc.horizontalTextAlignment);
		setVerticalTextAlignment(mDesc->textDesc.verticalTextAlignment);
		// Make sure the text cursor is not visible, as setting the alignment will reposition the text cursor and make it visible.
		mTextCursor->setVisible(false);
	}

	Ogre::String TextBox::getClass()
	{
		return "TextBox";
	}

	void TextBox::addCharacter(Ogre::UTFString::code_point cp)
	{
		if(mText->getLength() >= static_cast<int>(mDesc->textbox_maxCharacters))
			return;

		if(mTextInputValidatorSlot != NULL)
			if(!mTextInputValidatorSlot->isInputValid(cp,mCursorIndex,mText->getText()))
				return;

		mText->addCharacter(OGRE_NEW_T(Character,Ogre::MEMCATEGORY_GENERAL)(cp,mCurrentFont,mDesc->textbox_defaultColor),mCursorIndex);
		setCursorIndex(mCursorIndex+1);
	}

	void TextBox::addText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv)
	{
		mText->addText(s,fp,cv);

		redraw();
	}

	void TextBox::addText(Ogre::UTFString s, const Ogre::String& fontName, const ColourValue& cv)
	{
		addText(s,Text::getFont(fontName),cv);
	}

	void TextBox::addText(Ogre::UTFString s)
	{
		addText(s,mDesc->textbox_defaultFontName,mDesc->textbox_defaultColor);
	}

	void TextBox::addText(std::vector<TextSegment> segments)
	{
		mText->addText(segments);

		redraw();
	}

	void TextBox::blinkTimerCallback()
	{
		mTextCursor->setVisible(!mTextCursor->getVisible());
	}

	void TextBox::clearTextInputValidator()
	{
		delete mTextInputValidatorSlot;
		mTextInputValidatorSlot = NULL;
	}

	void TextBox::clearText()
	{
		mText->clearText();
		
		setCursorIndex(0);

		mTextCursor->setVisible(false);

		redraw();
	}

	ColourValue TextBox::getDefaultColor()
	{
		return mDesc->textbox_defaultColor;
	}

	Ogre::String TextBox::getDefaultFont()
	{
		return mDesc->textbox_defaultFontName;
	}

	float TextBox::getHorizontalPadding()
	{
		return mDesc->textbox_horizontalPadding;
	}

	HorizontalTextAlignment TextBox::getHorizontalTextAlignment()
	{
		return mDesc->textDesc.horizontalTextAlignment;
	}

	Ogre::UTFString::code_point TextBox::getMaskSymbol()
	{
		return mDesc->textbox_maskSymbol;
	}

	bool TextBox::getMaskText()
	{
		return mDesc->textbox_maskText;
	}

	int TextBox::getMaxCharacters()
	{
		return mDesc->textbox_maxCharacters;
	}

	Rect TextBox::getScreenRect()
	{
		Rect r = mTextBoxClipRegion;

		if((mDesc->sheet != NULL) && (mDesc->sheet != mWindow))
			r.translate(mWindow->getPosition());

		return r;
	}

	Ogre::UTFString TextBox::getText()
	{
		return mText->getText();
	}

	BrushFilterMode TextBox::getTextBrushFilterMode()
	{
		return mText->getBrushFilterMode();
	}

	std::vector<TextSegment> TextBox::getTextSegments()
	{
		return mText->getTextSegments();
	}

	Ogre::String TextBox::getTextCursorSkinType()
	{
		return mDesc->textbox_textCursorDefaultSkinTypeName;
	}

	VerticalTextAlignment TextBox::getVerticalTextAlignment()
	{
		return mDesc->textDesc.verticalTextAlignment;
	}

	void TextBox::onCharEntered(const EventArgs& args)
	{
		const KeyEventArgs kea = dynamic_cast<const KeyEventArgs&>(args);

		addCharacter(kea.codepoint);
	}

	void TextBox::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		// Draw Background

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),mSkinElement);

		ColourValue prevColor = brush->getColour();
		Rect prevClipRegion = brush->getClipRegion();

		Rect unpaddedClientArea = mClientDimensions;
		unpaddedClientArea.position.x -= mDesc->textbox_horizontalPadding;
		unpaddedClientArea.size.width += (mDesc->textbox_horizontalPadding * 2);

		Rect clipRegion = Rect(mTexturePosition,unpaddedClientArea.size);
		clipRegion.translate(unpaddedClientArea.position);

		mTextBoxClipRegion = prevClipRegion.getIntersection(clipRegion);
		brush->setClipRegion(mTextBoxClipRegion);

		Point textbox_textPosition = mTexturePosition;
		textbox_textPosition += mClientDimensions.position;
		textbox_textPosition.translate(mDesc->textbox_textPosition);
		mText->draw(textbox_textPosition);

		// Draw Text Overlay SkinElement

		brush->drawSkinElement(mTextBoxClipRegion,mSkinType->getSkinElement(TEXTOVERLAY));

		// Restore clipping

		brush->setClipRegion(prevClipRegion);

		brush->setColor(prevColor);
	}

	void TextBox::onKeyDown(const EventArgs& args)
	{
		const KeyEventArgs kea = dynamic_cast<const KeyEventArgs&>(args);

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
			break;
		}
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
			relativePosition.x = mea.position.x - (mTexturePosition.x + mWindow->getPosition().x);
			relativePosition.y = mea.position.y - (mTexturePosition.y + mWindow->getPosition().y);

			// Convert relative TextBox position to coordinates relative to client area
			relativePosition -= mClientDimensions.position;

			// Convert relative TextBox position to coordinates relative to Text position
			Point relativeTextPosition;
			relativeTextPosition.x = relativePosition.x - mDesc->textbox_textPosition.x;
			relativeTextPosition.y = relativePosition.y - mDesc->textbox_textPosition.y;

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
		if(!mWidgetDesc->widget_visible)
		{
			mBlinkTimer->stop();
			mTextCursor->setVisible(false);
		}
		else
		{
			if((mWidgetDesc->sheet != NULL) && (mWidgetDesc->sheet->getKeyboardListener() == this))
			{
				mBlinkTimer->start();
				mTextCursor->setVisible(true);
			}
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
			switch(mDesc->textDesc.horizontalTextAlignment)
			{
			case TEXT_ALIGNMENT_HORIZONTAL_CENTER:
				mDesc->textbox_textPosition.x = (mClientDimensions.size.width - mText->getTextWidth()) / 2.0;
				break;
			case TEXT_ALIGNMENT_HORIZONTAL_LEFT:
				mDesc->textbox_textPosition.x = 0;
				break;
			case TEXT_ALIGNMENT_HORIZONTAL_RIGHT:
				mDesc->textbox_textPosition.x = mClientDimensions.size.width - mText->getTextWidth();
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
			float indexPosition = (relativeCursorPosition.x + mDesc->textbox_textPosition.x);

			if(indexPosition < 0)
			{
				mDesc->textbox_textPosition.x -= indexPosition;
			}
			else if(indexPosition > mClientDimensions.size.width)
			{
				mDesc->textbox_textPosition.x -= (indexPosition - mClientDimensions.size.width);
			}
		}

		mCursorPosition.x = mDesc->textbox_textPosition.x + mText->getPositionAtCharacterIndex(mCursorIndex).x;

		// Position Cursor
		Point p = getScreenPosition();
		p.translate(mClientDimensions.position);
		p.translate(mCursorPosition);
		mTextCursor->setPosition(p);
		redraw();
	}

	void TextBox::setDefaultColor(const ColourValue& cv)
	{
		mDesc->textbox_defaultColor = cv;
	}

	void TextBox::setDefaultFont(const Ogre::String& fontName)
	{
		mDesc->textbox_defaultFontName = fontName;
		mCurrentFont = Text::getFont(fontName);
	}

	void TextBox::setEnabled(bool enabled)
	{
		Widget::setEnabled(enabled);

		if(!mWidgetDesc->widget_enabled)
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

	void TextBox::setHorizontalPadding(float padding)
	{
		mDesc->textbox_horizontalPadding = padding;

		updateClientDimensions();

		redraw();
	}

	void TextBox::setHorizontalTextAlignment(HorizontalTextAlignment a)
	{
		mDesc->textDesc.horizontalTextAlignment = a;

		setCursorIndex(mCursorIndex);
	}

	void TextBox::setMaskText(bool mask, Ogre::UTFString::code_point textbox_maskSymbol)
	{
		mDesc->textbox_maskText = mask;
		mDesc->textbox_maskSymbol = textbox_maskSymbol;

		mText->setMaskText(mask,textbox_maskSymbol);
	}

	void TextBox::setMaxCharacters(unsigned int max)
	{		
		if(static_cast<int>(max) < mDesc->textDesc.getTextLength())
		{
			throw Exception(Exception::ERR_TEXT,"Cannot set max Characters when text exceeds max! (Data loss)","TextBox::setMaxCharacters");
		}

		mDesc->textbox_maxCharacters = max;
	}

	void TextBox::setParent(Widget* parent)
	{
		if(mParentWidget != NULL)
		{
			// Remove event handler from previous window
			if(mParentWidget->getWindow() != NULL)
				mParentWidget->getWindow()->removeWindowEventHandler(WINDOW_EVENT_DRAWN,this);
		}

		mParentWidget = parent;

		if(mParentWidget != NULL)
		{
			mWindow = mParentWidget->getWindow();
			mWidgetDesc->guiManager = mParentWidget->getGUIManager();
			mWidgetDesc->sheet = mParentWidget->getSheet();

			// Check if widget should be centered in Parent's client area.
			setHorizontalAnchor(mWidgetDesc->widget_horizontalAnchor);
			setVerticalAnchor(mWidgetDesc->widget_verticalAnchor);

			// Add event handler to new window
			if(mWindow != NULL)
				mWindow->addWindowEventHandler(WINDOW_EVENT_DRAWN,&TextBox::onWindowDrawn,this);
		}

		// Update screen rectangle

		updateTexturePosition();
	}

	void TextBox::setText(Ogre::UTFString s)
	{
		setText(s,Text::getFont(mDesc->textbox_defaultFontName),mDesc->textbox_defaultColor);
	}

	void TextBox::setText(Ogre::UTFString s, Ogre::Font* fp, const ColourValue& cv)
	{
		mText->setText(s,fp,cv);

		if(mTextCursor->getVisible())
			setCursorIndex(mCursorIndex);

		redraw();
	}

	void TextBox::setText(std::vector<TextSegment> segments)
	{
		mText->setText(segments);

		redraw();
	}

	void TextBox::setTextBrushFilterMode(BrushFilterMode m)
	{
		mText->setBrushFilterMode(m);
	}

	void TextBox::setTextColor(const ColourValue& cv)
	{
		mText->setColor(cv);

		redraw();
	}

	void TextBox::setTextColor(const ColourValue& cv, unsigned int index)
	{
		mText->setColor(cv,index);

		redraw();
	}

	void TextBox::setTextColor(const ColourValue& cv, unsigned int startIndex, unsigned int endIndex)
	{
		mText->setColor(cv,startIndex,endIndex);

		redraw();
	}

	void TextBox::setTextColor(const ColourValue& cv, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mText->setColor(cv,c,allOccurrences);

		redraw();
	}

	void TextBox::setTextColor(const ColourValue& cv, Ogre::UTFString s, bool allOccurrences)
	{
		mText->setColor(cv,s,allOccurrences);

		redraw();
	}

	void TextBox::setTextCursorSkinType(const Ogre::String type)
	{
		mDesc->textbox_textCursorDefaultSkinTypeName = type;

		mTextCursor->setSkinType(type);
	}

	void TextBox::setVerticalTextAlignment(VerticalTextAlignment a)
	{
		if(mText != NULL)
			mText->setVerticalTextAlignment(a);
	}

	void TextBox::updateClientDimensions()
	{
		Widget::updateClientDimensions();

		// Shrink client area horizontally (on both left and right side) according to padding amount
		if(mDesc != NULL)
		{
			mClientDimensions.position.x += mDesc->textbox_horizontalPadding;
			mClientDimensions.size.width -= (mDesc->textbox_horizontalPadding * 2);
		}

		if(mTextCursor != NULL)
			mTextCursor->setHeight(mClientDimensions.size.height);

		if(mText != NULL)
			mText->setAllottedHeight(mClientDimensions.size.height);
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
