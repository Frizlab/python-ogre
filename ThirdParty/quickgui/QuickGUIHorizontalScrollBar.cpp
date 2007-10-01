#include "QuickGUIHorizontalScrollBar.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	HorizontalScrollBar::HorizontalScrollBar(const Ogre::String& name, Type type, const Rect& pixelDimensions, const Ogre::String& texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Image(name,type,pixelDimensions,texture,container,ParentWidget,gm),
		mMinSliderPosition(0),
		mMaxSliderPosition(mSize.width),
		mMouseDownOnTrack(false),
		mSmallChange(0.1),
		mLargeChange(0.4),
		mRepeatTimer(0),
		mScrollRepeatTime(0.5)
	{
		if(mWidgetType == TYPE_SCROLLBAR_HORIZONTAL)
		{
			mQuad->setLayer(mParentWidget->getQuad()->getLayer());
		}

		addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onMouseDownOnTrack,this);
		addEventHandler(EVENT_MOUSE_BUTTON_UP,&HorizontalScrollBar::onMouseUpOnTrack,this);
		addEventHandler(EVENT_MOUSE_LEAVE,&HorizontalScrollBar::onMouseLeaveTrack,this);
		addEventHandler(EVENT_POSITION_CHANGED,&HorizontalScrollBar::onSizeChanged,this);

		mSliderTextureName = mTextureName + ".slider" + mTextureExtension;
		mScrollLeftTextureName = mTextureName + ".left" + mTextureExtension;
		mScrollRightTextureName = mTextureName + ".right" + mTextureExtension;

		mSlider = new Button(mInstanceName+".Slider",TYPE_BUTTON,Rect(0,0,mSize.height,mSize.height),mSliderTextureName,mQuadContainer,this,mGUIManager);
		mSlider->enableDragging(true);
		mSlider->constrainDragging(true,false);
		mSlider->addEventHandler(EVENT_DRAGGED,&HorizontalScrollBar::onSliderDragged,this);
		
		Ogre::Real scrollBarHeight = mSize.height;
		mScrollLeft1 = new Button(mInstanceName+".Left1",TYPE_BUTTON,Rect(0,0,mSize.height,mSize.height),mScrollLeftTextureName,mQuadContainer,this,mGUIManager);
		mScrollLeft1->setShowWithParent(false);
		mScrollLeft1->hide();
		mScrollLeft1->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onScrollLeftDown,this);
		
		mScrollLeft2 = new Button(mInstanceName+".Left2",TYPE_BUTTON,Rect(0,0,mSize.height,mSize.height),mScrollLeftTextureName,mQuadContainer,this,mGUIManager);
		mScrollLeft2->setShowWithParent(false);
		mScrollLeft2->hide();
		mScrollLeft2->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onScrollLeftDown,this);
		
		mScrollRight1 = new Button(mInstanceName+".Right1",TYPE_BUTTON,Rect(0,0,mSize.height,mSize.height),mScrollRightTextureName,mQuadContainer,this,mGUIManager);
		mScrollRight1->setShowWithParent(false);
		mScrollRight1->hide();
		mScrollRight1->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onScrollRightDown,this);
		
		mScrollRight2 = new Button(mInstanceName+".Right2",TYPE_BUTTON,Rect(0,0,mSize.height,mSize.height),mScrollRightTextureName,mQuadContainer,this,mGUIManager);
		mScrollRight2->setShowWithParent(false);
		mScrollRight2->hide();
		mScrollRight2->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onScrollRightDown,this);
		
		_positionScrollButtons();
		setButtonLayout(BUTTON_LAYOUT_OPPOSITE);
		_constrainSlider();
	}

	HorizontalScrollBar::~HorizontalScrollBar()
	{
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

	void HorizontalScrollBar::_positionScrollButtons()
	{
		Rect leftDimensions = mScrollLeft1->getDimensions();
		Rect rightDimensions = mScrollRight1->getDimensions();
		mScrollLeft1->setPosition(0,(mSize.height/2.0) - (leftDimensions.height/2.0));
		mScrollRight1->setPosition(leftDimensions.width,(mSize.height/2.0) - (rightDimensions.height/2.0));
		mScrollLeft2->setPosition(mSize.width - rightDimensions.width - leftDimensions.width,(mSize.height/2.0) - (leftDimensions.height/2.0));
		mScrollRight2->setPosition(mSize.width - rightDimensions.width,(mSize.height/2.0) - (rightDimensions.height/2.0));
	}

	void HorizontalScrollBar::_scroll(Ogre::Real change, ScrollEventArgs args)
	{
		mSlider->moveX(change * mSize.width);

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
			mScrollRight1->show();
			break;
		case BUTTON_LAYOUT_ADJACENT_RIGHT:
			mScrollLeft2->show();
			mScrollRight2->show();
			break;
		case BUTTON_LAYOUT_MULTIPLE_BOTH:
			mScrollLeft1->show();
			mScrollRight1->show();
			mScrollLeft2->show();
			mScrollRight2->show();
			break;
		case BUTTON_LAYOUT_MULTIPLE_LEFT:
			mScrollLeft1->show();
			mScrollRight1->show();
			mScrollLeft2->show();
			break;
		case BUTTON_LAYOUT_MULTIPLE_RIGHT:
			mScrollRight1->show();
			mScrollLeft2->show();
			mScrollRight2->show();
			break;
		case BUTTON_LAYOUT_NONE:
			break;
		case BUTTON_LAYOUT_OPPOSITE:
			mScrollLeft1->show();
			mScrollRight2->show();
			break;
		}
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

	Size HorizontalScrollBar::getScrollButtonSize()
	{
		return mScrollLeft1->getSize();
	}

	Ogre::Real HorizontalScrollBar::getSliderHeight()
	{
		return mSlider->getHeight();
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
		_positionScrollButtons();
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

		switch(mButtonLayout)
		{
		case BUTTON_LAYOUT_ADJACENT_LEFT:
			{
				Rect temp = mScrollRight1->getDimensions();
				mMinSliderPosition = temp.x + temp.width;
				mMaxSliderPosition = mSize.width;
			}
			break;
		case BUTTON_LAYOUT_ADJACENT_RIGHT:
			{
				Rect temp = mScrollLeft2->getDimensions();
				mMinSliderPosition = 0.0;
				mMaxSliderPosition = temp.x;
			}
			break;
		case BUTTON_LAYOUT_MULTIPLE_BOTH:
			{
				Rect temp1 = mScrollRight1->getDimensions();
				mMinSliderPosition = temp1.x + temp1.width;

				Rect temp2 = mScrollLeft2->getDimensions();
				mMaxSliderPosition = temp2.x;
			}
			break;
		case BUTTON_LAYOUT_MULTIPLE_LEFT:
			{
				Rect temp1 = mScrollRight1->getDimensions();
				mMinSliderPosition = temp1.x + temp1.width;

				Rect temp2 = mScrollLeft2->getDimensions();
				mMaxSliderPosition = temp2.x;
			}
			break;
		case BUTTON_LAYOUT_MULTIPLE_RIGHT:
			{
				Rect temp1 = mScrollRight1->getDimensions();
				mMinSliderPosition = temp1.x + temp1.width;

				Rect temp2 = mScrollLeft2->getDimensions();
				mMaxSliderPosition = temp2.x;
			}
			break;
		case BUTTON_LAYOUT_NONE:
			mMinSliderPosition = 0.0;
			mMaxSliderPosition = mSize.width;
			break;
		case BUTTON_LAYOUT_OPPOSITE:
			{
				Rect temp1 = mScrollLeft1->getDimensions();
				mMinSliderPosition = temp1.x + temp1.width;

				Rect temp2 = mScrollRight2->getDimensions();
				mMaxSliderPosition = temp2.x;
			}
			break;
		}

		_constrainSlider();

		if(mVisible)
		{
			mScrollLeft1->hide();
			mScrollRight1->hide();
			mScrollLeft2->hide();
			mScrollRight2->hide();
			_showButtons();
		}
	}

	void HorizontalScrollBar::setLargeChange(Ogre::Real change)
	{
		mLargeChange = change;
	}

	void HorizontalScrollBar::setScrollButtonSize(Size pixelSize)
	{
		mScrollLeft1->setSize(pixelSize);
		mScrollLeft2->setSize(pixelSize);
		mScrollRight1->setSize(pixelSize);
		mScrollRight2->setSize(pixelSize);

		_positionScrollButtons();
		setButtonLayout(mButtonLayout);
	}

	void HorizontalScrollBar::setScrollRepeatTime(Ogre::Real timeInSeconds)
	{
		mScrollRepeatTime = timeInSeconds;
	}

	void HorizontalScrollBar::setSliderHeight(Ogre::Real pixelHeight)
	{
		mSlider->setHeight(pixelHeight);
		mSlider->setYPosition((mSize.height/2.0) - (mSlider->getHeight()/2.0));
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

	void HorizontalScrollBar::show()
	{
		Image::show();
		_showButtons();
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
