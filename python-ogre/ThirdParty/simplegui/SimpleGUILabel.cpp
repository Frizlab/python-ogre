#include "SimpleGUILabel.h"
#include "SimpleGUIManager.h"

namespace SimpleGUI
{
	Label::Label(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Widget(name,dimensions,overlayContainer,ParentWidget),
		mMaterial(material)
	{
		mWidgetType = Widget::SGUI_LABEL;

		mPanelOverlayElement = createPanelOverlayElement(mInstanceName+"_Background",mPixelDimensions,mMaterial);
		mOverlayContainer->addChild(mPanelOverlayElement);
		mPanelOverlayElement->show();

		mChildrenContainer = createOverlayContainer(mInstanceName+"_ChildrenContainer","");
		mOverlayContainer->addChildImpl(mChildrenContainer);

		mCharacterHeight = 0.8;
		mTextWidget = new Text(mInstanceName+"_text",Ogre::Vector3(0,0,mCharacterHeight),show,mChildrenContainer,this);

		mHorizontalAlignment = SGUI_HA_MID;
		mVerticalAlignment = SGUI_VA_MID;

		if(show) this->show();
		else hide();
	}

	Label::~Label()
	{
		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		delete mTextWidget;

		// destroy Children container
		mOverlayContainer->removeChild(mChildrenContainer->getName());
		om->destroyOverlayElement(mChildrenContainer);

		mOverlayContainer->removeChild(mPanelOverlayElement->getName());
		om->destroyOverlayElement(mPanelOverlayElement);
	}

	void Label::_applyDimensions()
	{
		mPanelOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
		mPanelOverlayElement->setDimensions(mPixelDimensions.z,mPixelDimensions.w);
	}

	void Label::_notifyDimensionsChanged()
	{
		_updateDimensions(mRelativeDimensions);
		_applyDimensions();

		mTextWidget->_notifyDimensionsChanged();
	}

	void Label::_notifyTextChanged()
	{
		mTextWidget->setFont(mFont);
		mTextWidget->setCharacterHeight(mCharacterHeight);
		mTextWidget->setTextColor(mTextTopColor,mTextBotColor);
		mTextWidget->setText(mText);

		alignText(mHorizontalAlignment,mVerticalAlignment);
	}

	void Label::alignText(HorizontalAlignment ha, VerticalAlignment va)
	{
		mHorizontalAlignment = ha;
		mVerticalAlignment = va;

		Ogre::Vector2 relativeLabelPos = mTextWidget->getRelativePosition();
		Ogre::Vector2 labelPos = mTextWidget->getAbsolutePosition();
		Ogre::Vector2 labelSize = mTextWidget->getAbsoluteSize();

		if( mHorizontalAlignment == SGUI_HA_LEFT ) 
		{
			// We should add a 5 pixel buffer
			Ogre::Real buffer = 5.0 / mPixelDimensions.z;
			mTextWidget->setPosition(buffer,relativeLabelPos.y);
		}
		else if( mHorizontalAlignment == SGUI_HA_MID )
		{
			mTextWidget->setPosition(((mAbsoluteDimensions.z / 2) - (labelSize.x / 2)) / mAbsoluteDimensions.z,relativeLabelPos.y);
		}
		else if( mHorizontalAlignment == SGUI_HA_RIGHT )
		{
			// We should add a 5 pixel buffer
			Ogre::Real buffer = 5.0 / mPixelDimensions.z;
			mTextWidget->setPosition(((mAbsoluteDimensions.z) - (labelSize.x) - buffer) / mAbsoluteDimensions.z,relativeLabelPos.y);
		}

		// Make sure to update the position, in case alignment has moved it
		relativeLabelPos = mTextWidget->getRelativePosition();

		if( mVerticalAlignment == SGUI_VA_TOP ) mTextWidget->setPosition(relativeLabelPos.x,0.0);
		else if( mVerticalAlignment == SGUI_VA_MID )
		{
			mTextWidget->setPosition(relativeLabelPos.x,((mAbsoluteDimensions.w / 2) - (labelSize.y / 2)) / mAbsoluteDimensions.w);
		}
		else if( mVerticalAlignment == SGUI_VA_BOTTOM )
		{
			mTextWidget->setPosition(relativeLabelPos.x,((mAbsoluteDimensions.w) - (labelSize.y)) / mAbsoluteDimensions.w);
		}
	}

	void Label::activate(EventArgs& e) 
	{
		mTextWidget->activate(e);
		Widget::activate(e);
	}

	void Label::deactivate(EventArgs& e) 
	{
		mTextWidget->deactivate(e);
		Widget::deactivate(e);
	}

	Text* Label::getTextWidget()
	{
		return mTextWidget;
	}

	void Label::hide()
	{
		mTextWidget->hide();
		mPanelOverlayElement->hide();
		Widget::hide();
	}

	void Label::setTiling(const Ogre::Real& horizontalRepetitions, const Ogre::Real& verticalRepetitions, Ogre::ushort layer)
	{
		mPanelOverlayElement->setTiling(horizontalRepetitions,verticalRepetitions,layer);
	}

	void Label::show()
	{
		mTextWidget->show();
		mPanelOverlayElement->show();
		Widget::show();
	}

	void Label::timeElapsed(Ogre::Real time)
	{
		mTextWidget->timeElapsed(time);
	}
}
