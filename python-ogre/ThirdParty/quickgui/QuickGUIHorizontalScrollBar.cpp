#include "QuickGUIHorizontalScrollBar.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	HorizontalScrollBar::HorizontalScrollBar(const Ogre::String& instanceName, const Size& pixelSize, const Ogre::String& texture, GUIManager* gm) :
		Image(instanceName,pixelSize,texture,gm),
		mMinSliderPosition(0),
		mMaxSliderPosition(mSize.width),
		mMouseDownOnTrack(false),
		mSmallChange(0.1),
		mLargeChange(0.4),
		mRepeatTimer(0),
		mScrollRepeatTime(0.5),
		mButtonLayout(BUTTON_LAYOUT_OPPOSITE)
	{
		mWidgetType = TYPE_SCROLLBAR_HORIZONTAL;
		mGUIManager->registerTimeListener(this);
		addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onMouseDownOnTrack,this);
		addEventHandler(EVENT_MOUSE_BUTTON_UP,&HorizontalScrollBar::onMouseUpOnTrack,this);
		addEventHandler(EVENT_MOUSE_LEAVE,&HorizontalScrollBar::onMouseLeaveTrack,this);

		mSliderTextureName = mTextureName + ".slider" + mTextureExtension;
		mScrollLeftTextureName = mTextureName + ".left" + mTextureExtension;
		mScrollRightTextureName = mTextureName + ".right" + mTextureExtension;

		mSlider = new Button(mInstanceName+".Slider",Size(mSize.height,mSize.height),mSliderTextureName,mGUIManager);
		addChild(mSlider);
		mSlider->setPosition(mSize.height,0);
		mSlider->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		mSlider->enableDragging(true);
		mSlider->constrainDragging(true,false);
		mSlider->setAutoSize(false);
		mSlider->addEventHandler(EVENT_DRAGGED,&HorizontalScrollBar::onSliderDragged,this);
		
		Ogre::Real scrollBarHeight = mSize.height;
		mScrollLeft1 = new Button(mInstanceName+".Left1",Size(mSize.height,mSize.height),mScrollLeftTextureName,mGUIManager);
		addChild(mScrollLeft1);
		mScrollLeft1->setPosition(0,0);
		mScrollLeft1->setAutoSize(false);
		mScrollLeft1->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		mScrollLeft1->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onScrollLeftDown,this);
		
		mScrollRight1 = new Button(mInstanceName+".Right1",Size(mSize.height,mSize.height),mScrollRightTextureName,mGUIManager);
		addChild(mScrollRight1);
		mScrollRight1->setPosition(mSize.height,0);
		mScrollRight1->setAutoSize(false);
		mScrollRight1->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		mScrollRight1->setShowWithParent(false);
		mScrollRight1->hide();
		mScrollRight1->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onScrollRightDown,this);

		mScrollLeft2 = new Button(mInstanceName+".Left2",Size(mSize.height,mSize.height),mScrollLeftTextureName,mGUIManager);
		addChild(mScrollLeft2);
		mScrollLeft2->setPosition(mSize.width - (mSize.height*2.0),0);
		mScrollLeft2->setAutoSize(false);
		mScrollLeft2->setHorizontalAnchor(ANCHOR_HORIZONTAL_RIGHT);
		mScrollLeft2->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		mScrollLeft2->setShowWithParent(false);
		mScrollLeft2->hide();
		mScrollLeft2->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onScrollLeftDown,this);
		
		mScrollRight2 = new Button(mInstanceName+".Right2",Size(mSize.height,mSize.height),mScrollRightTextureName,mGUIManager);
		addChild(mScrollRight2);
		mScrollRight2->setPosition(mSize.width - mSize.height,0);
		mScrollRight2->setAutoSize(false);
		mScrollRight2->setHorizontalAnchor(ANCHOR_HORIZONTAL_RIGHT);
		mScrollRight2->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		mScrollRight2->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onScrollRightDown,this);

		mMinSliderPosition = mScrollRight1->getXPosition();
		mMaxSliderPosition = mScrollRight2->getXPosition();
	}

	HorizontalScrollBar::~HorizontalScrollBar()
	{
		mGUIManager->unregisterTimeListener(this);
	}

	void HorizontalScrollBar::_constrainSlider()
	{
		Rect sliderDimensions = mSlider->getDimensions();
		Ogre::Real sliderStart = sliderDimensions.x;
		Ogre::Real sliderEnd = sliderDimensions.x + sliderDimensions.width;

		if(sliderStart < mMinSliderPosition)
			mSlider->setXPosition(mMinSliderPosition);
		else if(sliderEnd > mMaxSliderPosition)
			mSlider->setXPosition(mMaxSliderPosition - mSlider->getWidth());
	}

	void HorizontalScrollBar::_determineMinMax()
	{
		switch(mButtonLayout)
		{
		case BUTTON_LAYOUT_ADJACENT_LEFT:
			mMinSliderPosition = mScrollRight1->getXPosition() + mScrollRight1->getWidth();
			mMaxSliderPosition = mSize.width;
			break;
		case BUTTON_LAYOUT_ADJACENT_RIGHT:
			mMinSliderPosition = 0;
			mMaxSliderPosition = mScrollLeft2->getXPosition();
			break;
		case BUTTON_LAYOUT_MULTIPLE_BOTH:
			mMinSliderPosition = mScrollRight1->getXPosition() + mScrollRight1->getWidth();
			mMaxSliderPosition = mScrollLeft2->getXPosition();
			break;
		case BUTTON_LAYOUT_MULTIPLE_LEFT:
			mMinSliderPosition = mScrollRight1->getXPosition() + mScrollRight1->getWidth();
			mMaxSliderPosition = mScrollLeft2->getXPosition();
			break;
		case BUTTON_LAYOUT_MULTIPLE_RIGHT:
			mMinSliderPosition = mScrollRight1->getXPosition() + mScrollRight1->getWidth();
			mMaxSliderPosition = mScrollLeft2->getXPosition();
			break;
		case BUTTON_LAYOUT_NONE:
			mMinSliderPosition = 0;
			mMaxSliderPosition = mSize.width;
			break;
		case BUTTON_LAYOUT_OPPOSITE:
			mMinSliderPosition = mScrollRight1->getXPosition();
			mMaxSliderPosition = mScrollRight2->getXPosition();
			break;
		}
	}

	void HorizontalScrollBar::_scroll(Ogre::Real change, ScrollEventArgs args)
	{
		mSlider->moveX(change * (mMaxSliderPosition - mMinSliderPosition));

		_constrainSlider();

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnScrollHandlers.begin(); it != mOnScrollHandlers.end(); ++it )
			(*it)->execute(args);
	}

	void HorizontalScrollBar::_setValue(Ogre::Real value)
	{
		Ogre::Real pixelX = (value * (mMaxSliderPosition - mMinSliderPosition)) + mMinSliderPosition;

		mSlider->setXPosition(pixelX);
		_constrainSlider();
	}

	void HorizontalScrollBar::_showButtons()
	{
		switch(mButtonLayout)
		{
		case BUTTON_LAYOUT_ADJACENT_LEFT:
			mScrollLeft1->show();
			mScrollLeft1->setShowWithParent(true);
			mScrollRight1->show();
			mScrollRight1->setShowWithParent(true);
			mScrollLeft2->hide();
			mScrollLeft2->setShowWithParent(false);
			mScrollRight2->hide();
			mScrollRight2->setShowWithParent(false);
			break;
		case BUTTON_LAYOUT_ADJACENT_RIGHT:
			mScrollLeft1->hide();
			mScrollLeft1->setShowWithParent(false);
			mScrollRight1->hide();
			mScrollRight1->setShowWithParent(false);
			mScrollLeft2->show();
			mScrollLeft2->setShowWithParent(true);
			mScrollRight2->show();
			mScrollRight2->setShowWithParent(true);
			break;
		case BUTTON_LAYOUT_MULTIPLE_BOTH:
			mScrollLeft1->show();
			mScrollLeft1->setShowWithParent(true);
			mScrollRight1->show();
			mScrollRight1->setShowWithParent(true);
			mScrollLeft2->show();
			mScrollLeft2->setShowWithParent(true);
			mScrollRight2->show();
			mScrollRight2->setShowWithParent(true);
			break;
		case BUTTON_LAYOUT_MULTIPLE_LEFT:
			mScrollLeft1->show();
			mScrollLeft1->setShowWithParent(true);
			mScrollRight1->show();
			mScrollRight1->setShowWithParent(true);
			mScrollLeft2->show();
			mScrollLeft2->setShowWithParent(true);
			mScrollRight2->hide();
			mScrollRight2->setShowWithParent(false);
			break;
		case BUTTON_LAYOUT_MULTIPLE_RIGHT:
			mScrollLeft1->hide();
			mScrollLeft1->setShowWithParent(false);
			mScrollRight1->show();
			mScrollRight1->setShowWithParent(true);
			mScrollLeft2->show();
			mScrollLeft2->setShowWithParent(true);
			mScrollRight2->show();
			mScrollRight2->setShowWithParent(true);
			break;
		case BUTTON_LAYOUT_NONE:
			mScrollLeft1->hide();
			mScrollLeft1->setShowWithParent(false);
			mScrollRight1->hide();
			mScrollRight1->setShowWithParent(false);
			mScrollLeft2->hide();
			mScrollLeft2->setShowWithParent(false);
			mScrollRight2->hide();
			mScrollRight2->setShowWithParent(false);
			break;
		case BUTTON_LAYOUT_OPPOSITE:
			mScrollLeft1->show();
			mScrollLeft1->setShowWithParent(true);
			mScrollRight1->hide();
			mScrollRight1->setShowWithParent(false);
			mScrollLeft2->hide();
			mScrollLeft2->setShowWithParent(false);
			mScrollRight2->show();
			mScrollRight2->setShowWithParent(true);
			break;
		}

		if(!mVisible)
			hide();
	}

	void HorizontalScrollBar::addOnScrollEventHandler(MemberFunctionSlot* function)
	{
		mOnScrollHandlers.push_back(function);
	}

	HorizontalScrollBar::ButtonLayout HorizontalScrollBar::getButtonLayout()
	{
		return mButtonLayout;
	}

	Ogre::Real HorizontalScrollBar::getLargeChange()
	{
		return mLargeChange;
	}

	Ogre::Real HorizontalScrollBar::getRepeatTime()
	{
		return mScrollRepeatTime;
	}

	Ogre::Real HorizontalScrollBar::getSliderWidth()
	{
		return mSlider->getWidth();
	}

	Ogre::Real HorizontalScrollBar::getSmallChange()
	{
		return mSmallChange;
	}

	Ogre::Real HorizontalScrollBar::getValue()
	{
		Ogre::Real retVal = ((mSlider->getXPosition() - mMinSliderPosition) / (mMaxSliderPosition - mMinSliderPosition));
		return retVal;
	}

	void HorizontalScrollBar::onMouseDownOnTrack(const EventArgs& args)
	{
		if(dynamic_cast<const MouseEventArgs&>(args).button == MB_Left)
		{
			mMouseDownOnTrack = true;

			Point mousePosition = dynamic_cast<const MouseEventArgs&>(args).position - getScreenPosition();

			ScrollEventArgs scrollArgs(this);
			// if mouse clicked left of track, scroll left.
			if(mousePosition.x < mSlider->getXPosition())
			{
				scrollArgs.sliderIncreasedPosition = false;
				_scroll(-mLargeChange,scrollArgs);
			}
			else
			{
				scrollArgs.sliderIncreasedPosition = true;
				_scroll(mLargeChange,scrollArgs);
			}

			mRepeatTimer = 0;
		}
	}

	void HorizontalScrollBar::onMouseUpOnTrack(const EventArgs& args)
	{
		if(dynamic_cast<const MouseEventArgs&>(args).button == MB_Left)
		{
			mMouseDownOnTrack = false;
		}
	}

	void HorizontalScrollBar::onMouseLeaveTrack(const EventArgs& args)
	{
		mMouseDownOnTrack = false;
	}

	void HorizontalScrollBar::onSizeChanged(const EventArgs& args)
	{
		Image::onSizeChanged(args);
		_determineMinMax();
		_constrainSlider();
	}

	void HorizontalScrollBar::onScrollLeftDown(const EventArgs& args)
	{
		if(dynamic_cast<const MouseEventArgs&>(args).button == MB_Left)
		{
			scrollLeftSmall();

			mRepeatTimer = 0;
		}
	}

	void HorizontalScrollBar::onScrollRightDown(const EventArgs& args)
	{
		if(dynamic_cast<const MouseEventArgs&>(args).button == MB_Left)
		{
			scrollRightSmall();

			mRepeatTimer = 0;
		}
	}

	void HorizontalScrollBar::onSliderDragged(const EventArgs& args)
	{
		ScrollEventArgs scrollArgs(this);
		if(dynamic_cast<const MouseEventArgs&>(args).moveDelta.x < 0)
			scrollArgs.sliderIncreasedPosition = false;
		else
			scrollArgs.sliderIncreasedPosition = true;

		_scroll(0,scrollArgs);
	}

	void HorizontalScrollBar::scrollLeftLarge()
	{
		ScrollEventArgs scrollArgs(this);
		scrollArgs.sliderIncreasedPosition = false;
		_scroll(-mLargeChange,scrollArgs);
	}

	void HorizontalScrollBar::scrollLeftSmall()
	{
		ScrollEventArgs scrollArgs(this);
		scrollArgs.sliderIncreasedPosition = false;
		_scroll(-mSmallChange,scrollArgs);
	}

	void HorizontalScrollBar::scrollRightLarge()
	{
		ScrollEventArgs scrollArgs(this);
		scrollArgs.sliderIncreasedPosition = true;
		_scroll(mLargeChange,scrollArgs);
	}

	void HorizontalScrollBar::scrollRightSmall()
	{
		ScrollEventArgs scrollArgs(this);
		scrollArgs.sliderIncreasedPosition = true;
		_scroll(mSmallChange,scrollArgs);
	}

	void HorizontalScrollBar::setBaseTexture(const Ogre::String& textureName)
	{
		Image::setBaseTexture(textureName);

		mSliderTextureName = mTextureName + ".slider" + mTextureExtension;
		mScrollLeftTextureName = mTextureName + ".left" + mTextureExtension;
		mScrollRightTextureName = mTextureName + ".right" + mTextureExtension;
	}

	void HorizontalScrollBar::setButtonLayout(ButtonLayout layout)
	{
		mButtonLayout = layout;

		_showButtons();
		_determineMinMax();
	}

	void HorizontalScrollBar::setLargeChange(Ogre::Real change)
	{
		mLargeChange = change;
	}

	void HorizontalScrollBar::setScrollRepeatTime(Ogre::Real timeInSeconds)
	{
		mScrollRepeatTime = timeInSeconds;
	}

	void HorizontalScrollBar::setSliderWidth(Ogre::Real pixelWidth)
	{
		mSlider->setWidth(pixelWidth * (mMaxSliderPosition - mMinSliderPosition));
		_constrainSlider();
	}

	void HorizontalScrollBar::setSmallChange(Ogre::Real change)
	{
		mSmallChange = change;
	}

	void HorizontalScrollBar::setValue(Ogre::Real value)
	{
		ScrollEventArgs scrollArgs(this);
		if(value < getValue())
			scrollArgs.sliderIncreasedPosition = false;
		else
			scrollArgs.sliderIncreasedPosition = true;

		Ogre::Real pixelX = (value * (mMaxSliderPosition - mMinSliderPosition)) + mMinSliderPosition;

		if( pixelX < 0.0 )
			pixelX = 0.0;
		if( pixelX > (mMaxSliderPosition - mSlider->getWidth()) )
			pixelX = (mMaxSliderPosition - mSlider->getWidth());

		mSlider->setXPosition(pixelX);

		_scroll(0,scrollArgs);
	}

	void HorizontalScrollBar::timeElapsed(Ogre::Real time)
	{
		Image::timeElapsed(time);
		mRepeatTimer += time;

		if(mRepeatTimer > mScrollRepeatTime)
		{
			if(mMouseDownOnTrack)
			{
				MouseEventArgs args(this);
				args.button = MB_Left;
				args.position = mGUIManager->getMouseCursor()->getPosition();
				onMouseDownOnTrack(args);
				return;
			}

			if(mScrollLeft1->isDown() || mScrollLeft2->isDown())
			{
				scrollLeftSmall();
				return;
			}
			
			if(mScrollRight1->isDown() || mScrollRight2->isDown())
			{
				scrollRightSmall();
				return;
			}

			mRepeatTimer = 0;
		}
	}
}
