#include "SimpleGUITextBox.h"
#include "SimpleGUIManager.h"

namespace SimpleGUI
{
	TextBox::TextBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Widget(name,dimensions,overlayContainer,ParentWidget),
		mMaterial(material),
		mBorderOverlayElement(0),
		mMaskUserInput(0),
		mBackSpaceDown(0),
		mTimeAccumulator(0.0),
		mReadOnly(0)
	{
		mWidgetType = Widget::SGUI_TEXTBOX;

		// Border Overlay gives us ability to assign material to TextBox border and Panel separately.
		mBorderOverlayElement = createBorderPanelOverlayElement(mInstanceName+"_Border",mPixelDimensions,mMaterial);
		mBorderOverlayElement->setBorderMaterialName(mMaterial+".border");
		Ogre::Real borderSize = 3.0;
		mBorderOverlayElement->setBorderSize(borderSize,borderSize);
		mOverlayContainer->addChild(mBorderOverlayElement);
		mBorderOverlayElement->show();

		mChildrenContainer = createOverlayContainer(mInstanceName+"_ChildrenContainer","");
		mOverlayContainer->addChildImpl(mChildrenContainer);

		mCharacterHeight = 0.75;
		Ogre::Vector3 textDimensions = Ogre::Vector3(0,0,mCharacterHeight);
		// Label has no material, since it directly overlaps the textbox overlay element
		mTextWidget = new Text(mInstanceName+"_Text",textDimensions,show,mChildrenContainer,this);
		mTextWidget->setTruncateMode(Text::LEFT);
		mTextWidget->setTruncationFeedback("");
		//mTextWidget->addTextCursor('-',mFont,Ogre::ColourValue::White,Ogre::ColourValue::White);
		mHorizontalAlignment = SGUI_HA_LEFT;
		mVerticalAlignment = SGUI_VA_MID;

		if( show ) this->show();
		else hide();
	}

	TextBox::~TextBox()
	{
		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		delete mTextWidget;

		// destroy Children container
		mOverlayContainer->removeChild(mChildrenContainer->getName());
		om->destroyOverlayElement(mChildrenContainer);

		// destroy border overlay element
		mOverlayContainer->removeChild(mBorderOverlayElement->getName());
		om->destroyOverlayElement(mBorderOverlayElement);
	}

	void TextBox::_applyDimensions()
	{
		mBorderOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
		mBorderOverlayElement->setDimensions(mPixelDimensions.z,mPixelDimensions.w);
	}

	void TextBox::_notifyDimensionsChanged()
	{
		_updateDimensions(mRelativeDimensions);
		_applyDimensions();
		
		mTextWidget->_notifyDimensionsChanged();
	}

	void TextBox::_notifyTextChanged()
	{
		mTextWidget->setFont(mFont);
		mTextWidget->setCharacterHeight(mCharacterHeight);
		mTextWidget->setTextColor(mTextTopColor,mTextBotColor);
		mTextWidget->setText(mTextWidget->getText());

		alignText(mHorizontalAlignment,mVerticalAlignment);
	}

	void TextBox::addCharacter(char c)
	{
		if(mReadOnly) return;

		mText += c;

		Ogre::String s = mTextWidget->getText();
		if(mMaskUserInput) s += mMaskSymbol;
		else s += c;
		mTextWidget->setText(s);
		_notifyTextChanged();
	}

	void TextBox::alignText(HorizontalAlignment ha, VerticalAlignment va)
	{
		mHorizontalAlignment = ha;
		mVerticalAlignment = va;

		Ogre::Vector2 relativeLabelPos = mTextWidget->getRelativePosition();
		Ogre::Vector2 labelPos = mTextWidget->getAbsolutePosition();
		Ogre::Vector2 labelSize = mTextWidget->getAbsoluteSize();

		if( mHorizontalAlignment == SGUI_HA_LEFT ) 
		{
			Ogre::Real widthBuffer = 5.0 / mPixelDimensions.z;
			mTextWidget->setPosition(widthBuffer,relativeLabelPos.y);
		}
		if( mHorizontalAlignment == SGUI_HA_MID )
		{
			mTextWidget->setPosition(((mAbsoluteDimensions.z / 2) - (labelSize.x / 2)) / mAbsoluteDimensions.z,relativeLabelPos.y);
		}
		if( mHorizontalAlignment == SGUI_HA_RIGHT )
		{
			Ogre::Real widthBuffer = 5.0 / mPixelDimensions.z;
			mTextWidget->setPosition(((mAbsoluteDimensions.z) - (labelSize.x) - widthBuffer) / mAbsoluteDimensions.z,relativeLabelPos.y);
		}

		// Make sure to update the position, in case alignment has moved it
		relativeLabelPos = mTextWidget->getRelativePosition();

		if( mVerticalAlignment == SGUI_VA_TOP ) 
		{
			Ogre::Real heightBuffer = 3.0 / mPixelDimensions.w;
			mTextWidget->setPosition(relativeLabelPos.x,heightBuffer);
		}
		if( mVerticalAlignment == SGUI_VA_MID )
		{
			mTextWidget->setPosition(relativeLabelPos.x,((mAbsoluteDimensions.w / 2) - (labelSize.y / 2)) / mAbsoluteDimensions.w);
		}
		if( mVerticalAlignment == SGUI_VA_BOTTOM )
		{
			Ogre::Real heightBuffer = 3.0 / mPixelDimensions.w;
			mTextWidget->setPosition(relativeLabelPos.x,((mAbsoluteDimensions.w) - (labelSize.y) - heightBuffer) / mAbsoluteDimensions.w);
		}
	}

	void TextBox::backSpace()
	{
		// update stored text
		if(mText != "" ) mText = mText.substr(0,mText.length()-1);

		// remove last character of label's text.  This could be a masked string (password)
		Ogre::String s = mTextWidget->getText();
		if( s != "" ) s = s.substr(0,s.length()-1);
		mTextWidget->setText(s);
		_notifyTextChanged();
	}

	Text* TextBox::getTextWidget()
	{
		return mTextWidget;
	}

	bool TextBox::getReadOnly()
	{
		return mReadOnly;
	}

	void TextBox::hide()
	{
		mBorderOverlayElement->hide();
		mTextWidget->hide();
		
		Widget::hide();
	}

	void TextBox::maskUserInput(const char& symbol)
	{
		mMaskUserInput = true;
		mMaskSymbol = symbol;
	}

	void TextBox::activate(EventArgs& e)
	{
		mTextWidget->activate(e);

		Widget::activate(e);
	}

	void TextBox::deactivate(EventArgs& e)
	{
		mTextWidget->deactivate(e);
		mBackSpaceDown = false;

		Widget::deactivate(e);
	}

	bool TextBox::onEnterPressed(KeyEventArgs& e)
	{
		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnEnterPressedUserEventHandlers.begin(); it != mOnEnterPressedUserEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);

		return e.handled;
	}

	bool TextBox::onCharacter(KeyEventArgs& e) 
	{ 
		if(!mReadOnly) addCharacter(e.codepoint);

		return Widget::onCharacter(e);
	}

	bool TextBox::onKeyDown(KeyEventArgs& e) 
	{ 
		if( (e.scancode == SimpleGUI::KC_BACK) && !mReadOnly ) 
		{
			mBackSpaceDown = true;
			mTimeAccumulator = 0.0;
			backSpace();
		}

		return Widget::onKeyDown(e);
	}

	bool TextBox::onKeyUp(KeyEventArgs& e)
	{
		if( (e.scancode == SimpleGUI::KC_BACK) && !mReadOnly )
			mBackSpaceDown = false;

		return Widget::onKeyUp(e);
	}

	void TextBox::setReadOnly(bool readOnly)
	{
		mReadOnly = readOnly;
	}

	void TextBox::setText(const Ogre::String& text)
	{
		Ogre::String t = text;
		if(mMaskUserInput)
		{
			int index = 0;
			while( 0 < t.length() )
			{
				t[index] = mMaskSymbol;
			}
		}
		mTextWidget->setText(t);
		Widget::setText(text);
	}

	void TextBox::setTiling(const Ogre::Real& horizontalRepetitions, const Ogre::Real& verticalRepetitions, Ogre::ushort layer)
	{
		mBorderOverlayElement->setTiling(horizontalRepetitions,verticalRepetitions,layer);
	}

	void TextBox::show()
	{
		mBorderOverlayElement->show();
		mTextWidget->show();
		
		Widget::show();
	}


	void TextBox::timeElapsed(Ogre::Real time)
	{
		mTimeAccumulator += time;

		// Hard coding the time to allow repetitive operations to be every .5 seconds
		if( mTimeAccumulator > 0.125 )
		{
			if(mBackSpaceDown && !mReadOnly) backSpace();
			mTimeAccumulator = 0.0;
		}
	}
}
