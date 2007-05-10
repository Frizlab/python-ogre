#include "SimpleGUIText.h"
#include "OgreFontManager.h"
#include "OgreFont.h"

namespace SimpleGUI
{
	Text::Text(const Ogre::String& name, const Ogre::Vector3& dimensions, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Widget(name,Ogre::Vector4(dimensions.x,dimensions.y,0,dimensions.z),overlayContainer,ParentWidget)
	{
		mWidgetType = Widget::SGUI_TEXT;
		mTruncateMode = RIGHT;
		mFeedbackString = "...";
		mCursorPositions.push_back(mPixelDimensions.x);

		mTextAreaOverlayElement = createTextAreaOverlayElement(mInstanceName+"_Caption",mPixelDimensions,"");
		mOverlayContainer->addChild(mTextAreaOverlayElement);
		mTextAreaOverlayElement->show();
		mTextAreaOverlayElement->setFontName(mFont);
		mTextAreaOverlayElement->setCharHeight(mPixelDimensions.w);

		if(show) this->show();
		else hide();
	}

	Text::~Text()
	{
		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		mOverlayContainer->removeChild(mTextAreaOverlayElement->getName());
		om->destroyOverlayElement(mTextAreaOverlayElement);
	}

	Ogre::String Text::_adjustWidth()
	{
		Ogre::String displayText = mText;
		// Adding a 5 pixel buffer, which helps textBoxes which have borders
		int parentWidth = mParentWidget->getPixelDimensions().z - 5;

		if( getTextWidth(displayText) > parentWidth )
		{
			// calling getTextWidth indexes the text, so cursor can be placed within text.  Only reason
			// we don't break out early with truncate mode set to NONE.
			if(mTruncateMode != NONE)
			{
				// get width of feedback string
				Ogre::Real feedbackWidth = getTextWidth(mFeedbackString);
				Ogre::Real allotedWidth = parentWidth - feedbackWidth;
				while( (getTextWidth(displayText) > allotedWidth) && (displayText != "") )
				{
					if(mTruncateMode == RIGHT) displayText = displayText.substr(0,displayText.length() - 1);
					else if(mTruncateMode == LEFT) displayText = displayText.substr(1,displayText.length());
				}
				// concatenate
				if(mTruncateMode == RIGHT) displayText += mFeedbackString;
				else if(mTruncateMode == LEFT) displayText = mFeedbackString + displayText;
			}
		}

		return displayText;
	}

	void Text::_applyDimensions()
	{
		mTextAreaOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
		mTextAreaOverlayElement->setDimensions(mPixelDimensions.z,mPixelDimensions.w);
		mTextAreaOverlayElement->setCharHeight(mPixelDimensions.w);
	}

	void Text::_notifyDimensionsChanged()
	{
		_updateDimensions(mRelativeDimensions);
		_applyDimensions();
	}

	void Text::_notifyTextChanged()
	{
		// adjust width to match the width of text
		mRelativeDimensions.z = (getTextWidth() / mParentWidget->getPixelDimensions().z);
		_notifyDimensionsChanged();
	}

	void Text::activate(EventArgs& e)
	{
		Widget::activate(e);
	}

	Ogre::Real Text::getTextWidth()
	{
		return getTextWidth(mText);
	}

	Ogre::Real Text::getTextWidth(const Ogre::String& text)
	{
		mCursorPositions.clear();
		// Store first cursor position
		int xOrigin = mPixelDimensions.x;
		mCursorPositions.push_back(xOrigin);

		if(text == "") return 0.0;

		Ogre::FontManager* fm = Ogre::FontManager::getSingletonPtr();
		Ogre::Font* f = dynamic_cast<Ogre::Font*>(fm->getByName(mFont).get());

		Ogre::Real width = 0.0;
		for( int index = 0; index < static_cast<int>(text.length()); ++index )
		{
			if( text[index] == ' ' ) width += (f->getGlyphAspectRatio('0') * mPixelDimensions.w);
			else width += (f->getGlyphAspectRatio(text[index]) * mPixelDimensions.w);
			mCursorPositions.push_back(width + xOrigin);
		}

		// now we know the text width in pixels, and have index positions at the start/end of each character.

		return width;
	}

	void Text::hide()
	{
		mTextAreaOverlayElement->hide();
		mVisible = false;
	}

	void Text::setCharacterHeight(const Ogre::Real& relativeHeight)
	{
		// Enforce the Text Widget's dimensions to match the Actual Text Dimensions,
		// as Text is not bounded to it's overlay element size
		setHeight(relativeHeight);
	}

	void Text::setTruncationFeedback(const Ogre::String& visualFeedback)
	{
		mFeedbackString = visualFeedback;
		setText(mText);
	}

	void Text::setFont(const Ogre::String& font)
	{
		mFont = font;
		_notifyTextChanged();
	}

	void Text::setHeight(const Ogre::Real& relativeHeight)
	{
		mRelativeDimensions.w = relativeHeight;
		_notifyTextChanged();
	}

	void Text::setText(const Ogre::String& text)
	{
		mText = text;

		mTextAreaOverlayElement->setFontName(mFont);
		mTextAreaOverlayElement->setCharHeight(mPixelDimensions.w);
		mTextAreaOverlayElement->setColourTop(mTextTopColor);
		mTextAreaOverlayElement->setColourBottom(mTextBotColor);
		mTextAreaOverlayElement->setCaption(_adjustWidth());
		
		_notifyTextChanged();
	}

	void Text::setTruncateMode(TruncateMode MODE)
	{
		mTruncateMode = MODE;
		setText(mText);
	}

	void Text::show()
	{
		mTextAreaOverlayElement->show();
		mVisible = true;
	}
}
