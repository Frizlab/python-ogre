#include "QuickGUILabel.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Label::Label(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Widget(name,dimensions,material,overlayContainer,ParentWidget)
	{
		mWidgetType = Widget::QGUI_TYPE_LABEL;

		mPanelOverlayElement = createPanelOverlayElement(mInstanceName+"_Background",mPixelDimensions,mWidgetMaterial);
		mOverlayContainer->addChild(mPanelOverlayElement);
		mPanelOverlayElement->show();

		mChildrenContainer = createOverlayContainer(mInstanceName+"_ChildrenContainer","");
		mOverlayContainer->addChildImpl(mChildrenContainer);

		mCharacterHeight = 0.8;
		mTextWidget = new Text(mInstanceName+"_text",Ogre::Vector3(0,0,mCharacterHeight),mChildrenContainer,this);
		mTextWidget->_notifyZOrder(mWindowZOrder,1);

		mHorizontalAlignment = QGUI_HA_MID;
		mVerticalAlignment = QGUI_VA_MID;
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

		Widget::_notifyDimensionsChanged();
	}

	void Label::_notifyTextChanged()
	{
		mTextWidget->setFont(mFont);
		mTextWidget->setCharacterHeight(mCharacterHeight);
		mTextWidget->setTextColor(mTextTopColor,mTextBotColor);
		mTextWidget->setText(mText);

		alignText(mHorizontalAlignment,mVerticalAlignment);
	}

	void Label::_notifyZOrder(Ogre::ushort windowZOrder, Ogre::ushort offsetZOrder)
	{
		Widget::_notifyZOrder(windowZOrder,offsetZOrder);
	}

	void Label::_notifyZOrder(Ogre::ushort windowZOrder)
	{
		mTextWidget->_notifyZOrder(windowZOrder);

		Widget::_notifyZOrder(windowZOrder);
	}

	void Label::alignText(HorizontalAlignment ha, VerticalAlignment va)
	{
		mHorizontalAlignment = ha;
		mVerticalAlignment = va;

		Ogre::Vector2 relativeLabelPos = mTextWidget->getRelativePosition();
		Ogre::Vector2 labelPos = mTextWidget->getAbsolutePosition();
		Ogre::Vector2 labelSize = mTextWidget->getAbsoluteSize();

		if( mHorizontalAlignment == QGUI_HA_LEFT ) 
		{
			// We should add a 5 pixel buffer
			Ogre::Real buffer = 5.0 / mPixelDimensions.z;
			mTextWidget->setPosition(buffer,relativeLabelPos.y);
		}
		else if( mHorizontalAlignment == QGUI_HA_MID )
		{
			mTextWidget->setPosition(((mAbsoluteDimensions.z / 2) - (labelSize.x / 2)) / mAbsoluteDimensions.z,relativeLabelPos.y);
		}
		else if( mHorizontalAlignment == QGUI_HA_RIGHT )
		{
			// We should add a 5 pixel buffer
			Ogre::Real buffer = 5.0 / mPixelDimensions.z;
			mTextWidget->setPosition(((mAbsoluteDimensions.z) - (labelSize.x) - buffer) / mAbsoluteDimensions.z,relativeLabelPos.y);
		}

		// Make sure to update the position, in case alignment has moved it
		relativeLabelPos = mTextWidget->getRelativePosition();

		if( mVerticalAlignment == QGUI_VA_TOP ) mTextWidget->setPosition(relativeLabelPos.x,0.0);
		else if( mVerticalAlignment == QGUI_VA_MID )
		{
			mTextWidget->setPosition(relativeLabelPos.x,((mAbsoluteDimensions.w / 2) - (labelSize.y / 2)) / mAbsoluteDimensions.w);
		}
		else if( mVerticalAlignment == QGUI_VA_BOTTOM )
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
