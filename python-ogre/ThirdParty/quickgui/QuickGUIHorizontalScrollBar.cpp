#include "QuickGUIHorizontalScrollBar.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	HorizontalScrollBar::HorizontalScrollBar(const Ogre::String& name, Type type, const Rect& dimensions, GuiMetricsMode pMode, GuiMetricsMode sMode, const Ogre::String& texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Image(name,type,dimensions,pMode,sMode,texture,container,ParentWidget,gm),
		mMinSliderPosition(0),
		mMaxSliderPosition(1),
		mSliderPixelHeight(mPixelDimensions.height),
		mScrollButtonPixelSize(Size(mPixelDimensions.height,mPixelDimensions.height)),
		mMouseDownOnTrack(false),
		mSmallChange(0.1),
		mLargeChange(0.4),
		mRelativeSliderPosition(Point::ZERO),
		mRepeatTimer(0),
		mScrollRepeatTime(0.5)
	{
		if(mWidgetType == TYPE_SCROLLBAR_HORIZONTAL)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);
		}

		addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onMouseDownOnTrack,this);
		addEventHandler(EVENT_MOUSE_BUTTON_UP,&HorizontalScrollBar::onMouseUpOnTrack,this);
		addEventHandler(EVENT_MOUSE_LEAVE,&HorizontalScrollBar::onMouseLeaveTrack,this);
		addEventHandler(EVENT_POSITION_CHANGED,&HorizontalScrollBar::onSizeChanged,this);

		mSliderTextureName = mTextureName + ".slider" + mTextureExtension;
		mScrollLeftTextureName = mTextureName + ".left" + mTextureExtension;
		mScrollRightTextureName = mTextureName + ".right" + mTextureExtension;

		mSlider = new Button(mInstanceName+".Slider",TYPE_BUTTON,Rect(0,0,1,1),QGUI_GMM_RELATIVE,QGUI_GMM_RELATIVE,mSliderTextureName,mQuadContainer,this,mGUIManager);
		mSlider->enableDragging(true);
		mSlider->constrainDragging(true,false);
		mSlider->getQuad()->setLayer(mQuad->getLayer());
		mSlider->addEventHandler(EVENT_DRAGGED,&HorizontalScrollBar::onSliderDragged,this);
		
		Ogre::Real scrollBarHeight = mPixelDimensions.height;
		mScrollLeft1 = new Button(mInstanceName+".Left1",TYPE_BUTTON,Rect(0,0,scrollBarHeight,scrollBarHeight),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,mScrollLeftTextureName,mQuadContainer,this,mGUIManager);
		mScrollLeft1->getQuad()->setLayer(mQuad->getLayer());
		mScrollLeft1->hide();
		mScrollLeft1->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onScrollLeftDown,this);
		
		mScrollLeft2 = new Button(mInstanceName+".Left2",TYPE_BUTTON,Rect(0,0,scrollBarHeight,scrollBarHeight),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,mScrollLeftTextureName,mQuadContainer,this,mGUIManager);
		mScrollLeft2->getQuad()->setLayer(mQuad->getLayer());
		mScrollLeft2->hide();
		mScrollLeft2->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onScrollLeftDown,this);
		
		mScrollRight1 = new Button(mInstanceName+".Right1",TYPE_BUTTON,Rect(0,0,scrollBarHeight,scrollBarHeight),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,mScrollRightTextureName,mQuadContainer,this,mGUIManager);
		mScrollRight1->getQuad()->setLayer(mQuad->getLayer());
		mScrollRight1->hide();
		mScrollRight1->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onScrollRightDown,this);
		
		mScrollRight2 = new Button(mInstanceName+".Right2",TYPE_BUTTON,Rect(0,0,scrollBarHeight,scrollBarHeight),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,mScrollRightTextureName,mQuadContainer,this,mGUIManager);
		mScrollRight2->getQuad()->setLayer(mQuad->getLayer());
		mScrollRight2->hide();
		mScrollRight2->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&HorizontalScrollBar::onScrollRightDown,this);
		
		_positionScrollButtons();
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

		mRelativeSliderPosition = mSlider->getPosition();
	}

	void HorizontalScrollBar::_positionScrollButtons()
	{
		Rect leftDimensions = mScrollLeft1->getDimensions();
		Rect rightDimensions = mScrollRight1->getDimensions();
		mScrollLeft1->setPosition(0,0.5 - (leftDimensions.height/2.0));
		mScrollRight1->setPosition(leftDimensions.width,0.5 - (rightDimensions.height/2.0));
		mScrollLeft2->setPosition(1 - rightDimensions.width - leftDimensions.width,0.5 - (leftDimensions.height/2.0));
		mScrollRight2->setPosition(1 - rightDimensions.width,0.5 - (rightDimensions.height/2.0));
	}

	void HorizontalScrollBar::_scroll(Ogre::Real change, ScrollEventArgs args)
	{
		mSlider->move(change,0);

		_constrainSlider();

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnScrollHandlers.begin(); it != mOnScrollHandlers.end(); ++it )
			(*it)->execute(args);
	}

	void HorizontalScrollBar::_setValue(Ogre::Real value)
	{
		if( value < 0.0 )
			value = 0.0;
		if( value > (mMaxSliderPosition - mSlider->getWidth()) )
			value = (mMaxSliderPosition - mSlider->getWidth());

		mSlider->setXPosition(mMinSliderPosition + value);
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

	Size HorizontalScrollBar::getScrollButtonSize(GuiMetricsMode mode)
	{
		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			return Size(mScrollButtonPixelSize.width / mGUIManager->getViewportWidth(),mScrollButtonPixelSize.height / mGUIManager->getViewportHeight());
		case QGUI_GMM_PIXELS:
			return mScrollButtonPixelSize;
		case QGUI_GMM_RELATIVE:
		default:
			return Size(mScrollButtonPixelSize.width / mPixelDimensions.width,mScrollButtonPixelSize.height / mPixelDimensions.height);
		}
	}

	Ogre::Real HorizontalScrollBar::getSliderHeight(GuiMetricsMode mode)
	{
		return mSlider->getHeight(mode);
	}

	Ogre::Real HorizontalScrollBar::getSliderWidth(GuiMetricsMode mode)
	{
		return mSlider->getWidth(mode);
	}

	Ogre::Real HorizontalScrollBar::getSmallChange()
	{
		return mSmallChange;
	}

	Ogre::Real HorizontalScrollBar::getValue()
	{
		Ogre::Real retVal = ((mSlider->getXPosition() - mMinSliderPosition) / (mMaxSliderPosition - mMinSliderPosition));
		
		if(retVal < 0)
			retVal = 0;

		return retVal;
	}

	void HorizontalScrollBar::onMouseDownOnTrack(const EventArgs& args)
	{
		if(dynamic_cast<const MouseEventArgs&>(args).button == MB_Left)
		{
			mMouseDownOnTrack = true;

			ScrollEventArgs scrollArgs(this);
			// if mouse clicked left of track, scroll left.
			if(dynamic_cast<const MouseEventArgs&>(args).position.x < mSlider->getXPosition(QGUI_GMM_PIXELS))
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
		mSlider->setWidth(mSlider->getWidth());
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
		Point currentRelativeSliderPosition = dynamic_cast<const WidgetEventArgs&>(args).widget->getPosition();
		if(currentRelativeSliderPosition.x < mRelativeSliderPosition.x)
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
				mMaxSliderPosition = 1.0;
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
			mMaxSliderPosition = 1.0;
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

		// adjust slider size to fit within the new region: (mMinSliderPosition to mMaxSliderPosition)
		mSlider->setWidth(mSlider->getWidth() * (mMaxSliderPosition - mMinSliderPosition));
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

	void HorizontalScrollBar::setScrollButtonSize(Size s, GuiMetricsMode mode)
	{
		mScrollLeft1->setSize(s,mode);
		mScrollLeft2->setSize(s,mode);
		mScrollRight1->setSize(s,mode);
		mScrollRight2->setSize(s,mode);

		_positionScrollButtons();

		mScrollButtonPixelSize = mScrollLeft1->getSize(QGUI_GMM_PIXELS);

		setButtonLayout(mButtonLayout);
	}

	void HorizontalScrollBar::setScrollRepeatTime(Ogre::Real timeInSeconds)
	{
		mScrollRepeatTime = timeInSeconds;
	}

	void HorizontalScrollBar::setSliderHeight(Ogre::Real height, GuiMetricsMode mode)
	{
		mSlider->setHeight(height,mode);
		mSlider->setYPosition(0.5 - (mSlider->getHeight()/2.0));
		mSliderPixelHeight = mSlider->getHeight(QGUI_GMM_PIXELS);
	}

	void HorizontalScrollBar::setSliderWidth(Ogre::Real relativeWidth)
	{
		mSlider->setWidth(relativeWidth * (mMaxSliderPosition - mMinSliderPosition));
		_constrainSlider();
	}

	void HorizontalScrollBar::setSmallChange(Ogre::Real change)
	{
		mSmallChange = change;
	}

	void HorizontalScrollBar::setValue(Ogre::Real value)
	{
		if( value < 0.0 )
			value = 0.0;
		if( value > (mMaxSliderPosition - mSlider->getWidth()) )
			value = (mMaxSliderPosition - mSlider->getWidth());

		mSlider->setXPosition(mMinSliderPosition + value);

		ScrollEventArgs scrollArgs(this);
		Ogre::Real currentValue = getValue();
		if( value < currentValue )
			scrollArgs.sliderIncreasedPosition = false;
		else
			scrollArgs.sliderIncreasedPosition = true;

		_scroll(0,scrollArgs);
	}

	void HorizontalScrollBar::show()
	{
		mQuad->setVisible(true);

		// show children, except for Windows and lists of MenuList or ComboBox Widget.
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( ((*it)->getWidgetType() == TYPE_BUTTON) &&
				((*it)->getInstanceName() != mSlider->getInstanceName()) )
				continue;

			(*it)->show();
		}

		_showButtons();

		// Only fire event if we change visibility.  If we were already visible, don't fire.
		if(!mVisible)
		{
			WidgetEventArgs args(this);
			fireEvent(EVENT_SHOWN,args);
		}

		mVisible = true;
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
