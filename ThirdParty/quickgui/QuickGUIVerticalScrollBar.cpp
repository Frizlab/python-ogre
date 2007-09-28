#include "QuickGUIVerticalScrollBar.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	VerticalScrollBar::VerticalScrollBar(const Ogre::String& name, Type type, const Rect& dimensions, GuiMetricsMode pMode, GuiMetricsMode sMode, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Image(name,type,dimensions,pMode,sMode,texture,container,ParentWidget,gm),
		mMinSliderPosition(0),
		mMaxSliderPosition(1),
		mSliderPixelWidth(mPixelDimensions.width),
		mScrollButtonPixelSize(Size(mPixelDimensions.width,mPixelDimensions.width)),
		mMouseDownOnTrack(false),
		mSmallChange(0.1),
		mLargeChange(0.4),
		mRelativeSliderPosition(Point::ZERO),
		mRepeatTimer(0),
		mScrollRepeatTime(0.5)
	{
		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_SCROLLBAR_VERTICAL)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);
		}

		addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&VerticalScrollBar::onMouseDownOnTrack,this);
		addEventHandler(EVENT_MOUSE_BUTTON_UP,&VerticalScrollBar::onMouseUpOnTrack,this);
		addEventHandler(EVENT_MOUSE_LEAVE,&VerticalScrollBar::onMouseLeaveTrack,this);
		addEventHandler(EVENT_POSITION_CHANGED,&VerticalScrollBar::onSizeChanged,this);

		mSliderTextureName = mTextureName + ".slider" + mTextureExtension;
		mScrollUpTextureName = mTextureName + ".up" + mTextureExtension;
		mScrollDownTextureName = mTextureName + ".down" + mTextureExtension;

		mSlider = new Button(mInstanceName+".Slider",TYPE_BUTTON,Rect(0,0,1,1),QGUI_GMM_RELATIVE,QGUI_GMM_RELATIVE,mSliderTextureName,mQuadContainer,this,mGUIManager);
		mSlider->enableDragging(true);
		mSlider->constrainDragging(false,true);
		mSlider->getQuad()->setLayer(mQuad->getLayer());
		mSlider->addEventHandler(EVENT_DRAGGED,&VerticalScrollBar::onSliderDragged,this);
		
		Ogre::Real scrollBarWidth = mPixelDimensions.width;
		mScrollUp1 = new Button(mInstanceName+".Up1",TYPE_BUTTON,Rect(0,0,scrollBarWidth,scrollBarWidth),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,mScrollUpTextureName,mQuadContainer,this,mGUIManager);
		mScrollUp1->getQuad()->setLayer(mQuad->getLayer());
		mScrollUp1->hide();
		mScrollUp1->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&VerticalScrollBar::onScrollUpDown,this);

		mScrollUp2 = new Button(mInstanceName+".Up2",TYPE_BUTTON,Rect(0,0,scrollBarWidth,scrollBarWidth),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,mScrollUpTextureName,mQuadContainer,this,mGUIManager);
		mScrollUp2->getQuad()->setLayer(mQuad->getLayer());
		mScrollUp2->hide();
		mScrollUp2->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&VerticalScrollBar::onScrollUpDown,this);

		mScrollDown1 = new Button(mInstanceName+".Down1",TYPE_BUTTON,Rect(0,0,scrollBarWidth,scrollBarWidth),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,mScrollDownTextureName,mQuadContainer,this,mGUIManager);
		mScrollDown1->getQuad()->setLayer(mQuad->getLayer());
		mScrollDown1->hide();
		mScrollDown1->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&VerticalScrollBar::onScrollDownDown,this);

		mScrollDown2 = new Button(mInstanceName+".Down2",TYPE_BUTTON,Rect(0,0,scrollBarWidth,scrollBarWidth),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,mScrollDownTextureName,mQuadContainer,this,mGUIManager);
		mScrollDown2->getQuad()->setLayer(mQuad->getLayer());
		mScrollDown2->hide();
		mScrollDown2->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&VerticalScrollBar::onScrollDownDown,this);
		
		_positionScrollButtons();
	}

	VerticalScrollBar::~VerticalScrollBar()
	{
	}

	void VerticalScrollBar::_constrainSlider()
	{
		Rect sliderDimensions = mSlider->getDimensions();
		Ogre::Real sliderStart = sliderDimensions.y;
		Ogre::Real sliderEnd = sliderDimensions.y + sliderDimensions.height;

		if(sliderStart < mMinSliderPosition)
			mSlider->setYPosition(mMinSliderPosition);
		else if(sliderEnd > mMaxSliderPosition)
			mSlider->setYPosition(mMaxSliderPosition - mSlider->getHeight());

		mRelativeSliderPosition = mSlider->getPosition();
	}

	void VerticalScrollBar::_positionScrollButtons()
	{
		Rect upDimensions = mScrollUp1->getDimensions();
		Rect downDimensions = mScrollDown1->getDimensions();
		mScrollUp1->setPosition(0.5 - (upDimensions.width/2.0),0);
		mScrollDown1->setPosition(0.5 - (upDimensions.width/2.0),upDimensions.height);
		mScrollUp2->setPosition(0.5 - (upDimensions.width/2.0),1 - downDimensions.height - upDimensions.height);
		mScrollDown2->setPosition(0.5 - (upDimensions.width/2.0),1 - downDimensions.height);
	}

	void VerticalScrollBar::_scroll(Ogre::Real change, ScrollEventArgs args)
	{
		mSlider->move(0,change);

		_constrainSlider();

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnScrollHandlers.begin(); it != mOnScrollHandlers.end(); ++it )
			(*it)->execute(args);
	}

	void VerticalScrollBar::_setValue(Ogre::Real value)
	{
		if( value < 0.0 )
			value = 0.0;
		if( value > (mMaxSliderPosition - mSlider->getHeight()) )
			value = (mMaxSliderPosition - mSlider->getHeight());

		mSlider->setYPosition(mMinSliderPosition + value);
		_constrainSlider();
	}

	void VerticalScrollBar::_showButtons()
	{
		switch(mButtonLayout)
		{
		case BUTTON_LAYOUT_ADJACENT_UP:
			mScrollUp1->show();
			mScrollDown1->show();
			break;
		case BUTTON_LAYOUT_ADJACENT_DOWN:
			mScrollUp2->show();
			mScrollDown2->show();
			break;
		case BUTTON_LAYOUT_MULTIPLE_BOTH:
			mScrollUp1->show();
			mScrollDown1->show();
			mScrollUp2->show();
			mScrollDown2->show();
			break;
		case BUTTON_LAYOUT_MULTIPLE_UP:
			mScrollUp1->show();
			mScrollDown1->show();
			mScrollUp2->show();
			break;
		case BUTTON_LAYOUT_MULTIPLE_DOWN:
			mScrollDown1->show();
			mScrollUp2->show();
			mScrollDown2->show();
			break;
		case BUTTON_LAYOUT_NONE:
			break;
		case BUTTON_LAYOUT_OPPOSITE:
			mScrollUp1->show();
			mScrollDown2->show();
			break;
		}
	}

	void VerticalScrollBar::addOnScrollEventHandler(MemberFunctionSlot* function)
	{
		mOnScrollHandlers.push_back(function);
	}

	VerticalScrollBar::ButtonLayout VerticalScrollBar::getButtonLayout()
	{
		return mButtonLayout;
	}

	Ogre::Real VerticalScrollBar::getLargeChange()
	{
		return mLargeChange;
	}

	Ogre::Real VerticalScrollBar::getRepeatTime()
	{
		return mScrollRepeatTime;
	}

	Size VerticalScrollBar::getScrollButtonSize(GuiMetricsMode mode)
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

	Ogre::Real VerticalScrollBar::getSliderHeight(GuiMetricsMode mode)
	{
		return mSlider->getHeight(mode);
	}

	Ogre::Real VerticalScrollBar::getSliderWidth(GuiMetricsMode mode)
	{
		return mSlider->getWidth(mode);
	}

	Ogre::Real VerticalScrollBar::getSmallChange()
	{
		return mSmallChange;
	}

	Ogre::Real VerticalScrollBar::getValue()
	{
		Ogre::Real retVal = ((mSlider->getYPosition() - mMinSliderPosition) / (mMaxSliderPosition - mMinSliderPosition));
		
		if(retVal < 0)
			retVal = 0;

		return retVal;
	}

	void VerticalScrollBar::onMouseDownOnTrack(const EventArgs& args)
	{
		if(dynamic_cast<const MouseEventArgs&>(args).button == MB_Left)
		{
			mMouseDownOnTrack = true;

			ScrollEventArgs scrollArgs(this);
			// if mouse clicked left of track, scroll left.
			if(dynamic_cast<const MouseEventArgs&>(args).position.y < mSlider->getYPosition(QGUI_GMM_PIXELS))
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

	void VerticalScrollBar::onMouseUpOnTrack(const EventArgs& args)
	{
		if(dynamic_cast<const MouseEventArgs&>(args).button == MB_Left)
		{
			mMouseDownOnTrack = false;
		}
	}

	void VerticalScrollBar::onMouseLeaveTrack(const EventArgs& args)
	{
		mMouseDownOnTrack = false;
	}

	void VerticalScrollBar::onSizeChanged(const EventArgs& args)
	{
		mSlider->setHeight(mSlider->getHeight());
		_positionScrollButtons();
		_constrainSlider();
	}

	void VerticalScrollBar::onScrollUpDown(const EventArgs& args)
	{
		if(dynamic_cast<const MouseEventArgs&>(args).button == MB_Left)
		{
			scrollUpSmall();

			mRepeatTimer = 0;
		}
	}

	void VerticalScrollBar::onScrollDownDown(const EventArgs& args)
	{
		if(dynamic_cast<const MouseEventArgs&>(args).button == MB_Left)
		{
			scrollDownSmall();

			mRepeatTimer = 0;
		}
	}

	void VerticalScrollBar::onSliderDragged(const EventArgs& args)
	{
		ScrollEventArgs scrollArgs(this);
		Point currentRelativeSliderPosition = dynamic_cast<const WidgetEventArgs&>(args).widget->getPosition();
		if(currentRelativeSliderPosition.y < mRelativeSliderPosition.y)
			scrollArgs.sliderIncreasedPosition = false;
		else
			scrollArgs.sliderIncreasedPosition = true;

		_constrainSlider();

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnScrollHandlers.begin(); it != mOnScrollHandlers.end(); ++it )
			(*it)->execute(scrollArgs);
	}

	void VerticalScrollBar::scrollUpLarge()
	{
		ScrollEventArgs scrollArgs(this);
		scrollArgs.sliderIncreasedPosition = false;
		_scroll(-mLargeChange,scrollArgs);
	}

	void VerticalScrollBar::scrollUpSmall()
	{
		ScrollEventArgs scrollArgs(this);
		scrollArgs.sliderIncreasedPosition = false;
		_scroll(-mSmallChange,scrollArgs);
	}

	void VerticalScrollBar::scrollDownLarge()
	{
		ScrollEventArgs scrollArgs(this);
		scrollArgs.sliderIncreasedPosition = true;
		_scroll(mLargeChange,scrollArgs);
	}

	void VerticalScrollBar::scrollDownSmall()
	{
		ScrollEventArgs scrollArgs(this);
		scrollArgs.sliderIncreasedPosition = true;
		_scroll(mSmallChange,scrollArgs);
	}

	void VerticalScrollBar::setBaseTexture(const Ogre::String& textureName)
	{
		Image::setBaseTexture(textureName);

		mSliderTextureName = mTextureName + ".slider" + mTextureExtension;
		mScrollUpTextureName = mTextureName + ".up" + mTextureExtension;
		mScrollDownTextureName = mTextureName + ".down" + mTextureExtension;
	}

	void VerticalScrollBar::setButtonLayout(ButtonLayout layout)
	{
		mButtonLayout = layout;

		switch(mButtonLayout)
		{
		case BUTTON_LAYOUT_ADJACENT_UP:
			{
				Rect temp = mScrollDown1->getDimensions();
				mMinSliderPosition = temp.y + temp.height;
				mMaxSliderPosition = 1.0;
			}
			break;
		case BUTTON_LAYOUT_ADJACENT_DOWN:
			{
				Rect temp = mScrollUp2->getDimensions();
				mMinSliderPosition = 0.0;
				mMaxSliderPosition = temp.y;
			}
			break;
		case BUTTON_LAYOUT_MULTIPLE_BOTH:
			{
				Rect temp1 = mScrollDown1->getDimensions();
				mMinSliderPosition = temp1.y + temp1.height;

				Rect temp2 = mScrollUp2->getDimensions();
				mMaxSliderPosition = temp2.y;
			}
			break;
		case BUTTON_LAYOUT_MULTIPLE_UP:
			{
				Rect temp1 = mScrollDown1->getDimensions();
				mMinSliderPosition = temp1.y + temp1.height;

				Rect temp2 = mScrollUp2->getDimensions();
				mMaxSliderPosition = temp2.y;
			}
			break;
		case BUTTON_LAYOUT_MULTIPLE_DOWN:
			{
				Rect temp1 = mScrollDown1->getDimensions();
				mMinSliderPosition = temp1.y + temp1.height;

				Rect temp2 = mScrollUp2->getDimensions();
				mMaxSliderPosition = temp2.y;
			}
			break;
		case BUTTON_LAYOUT_NONE:
			mMinSliderPosition = 0.0;
			mMaxSliderPosition = 1.0;
			break;
		case BUTTON_LAYOUT_OPPOSITE:
			{
				Rect temp1 = mScrollUp1->getDimensions();
				mMinSliderPosition = temp1.y + temp1.height;

				Rect temp2 = mScrollDown2->getDimensions();
				mMaxSliderPosition = temp2.y;
			}
			break;
		}

		// adjust slider size to fit within the new region: (mMinSliderPosition to mMaxSliderPosition)
		mSlider->setHeight(mSlider->getHeight() * (mMaxSliderPosition - mMinSliderPosition));
		_constrainSlider();

		if(mVisible)
		{
			mScrollUp1->hide();
			mScrollDown1->hide();
			mScrollUp2->hide();
			mScrollDown2->hide();
			_showButtons();
		}
	}

	void VerticalScrollBar::setLargeChange(Ogre::Real change)
	{
		mLargeChange = change;
	}

	void VerticalScrollBar::setScrollButtonSize(Size s, GuiMetricsMode mode)
	{
		mScrollUp1->setSize(s,mode);
		mScrollUp2->setSize(s,mode);
		mScrollDown1->setSize(s,mode);
		mScrollDown2->setSize(s,mode);

		_positionScrollButtons();

		mScrollButtonPixelSize = mScrollUp1->getSize(QGUI_GMM_PIXELS);

		setButtonLayout(mButtonLayout);
	}

	void VerticalScrollBar::setScrollRepeatTime(Ogre::Real timeInSeconds)
	{
		mScrollRepeatTime = timeInSeconds;
	}

	void VerticalScrollBar::setSliderWidth(Ogre::Real width, GuiMetricsMode mode)
	{
		mSlider->setWidth(width,mode);
		mSlider->setXPosition(0.5 - (mSlider->getWidth()/2.0));
		mSliderPixelWidth = mSlider->getWidth(QGUI_GMM_PIXELS);
	}

	void VerticalScrollBar::setSliderHeight(Ogre::Real relativeHeight)
	{
		mSlider->setHeight(relativeHeight * (mMaxSliderPosition - mMinSliderPosition));
		_constrainSlider();
	}

	void VerticalScrollBar::setSmallChange(Ogre::Real change)
	{
		mSmallChange = change;
	}

	void VerticalScrollBar::setValue(Ogre::Real value)
	{
		if( value < 0.0 )
			value = 0.0;
		if( value > (mMaxSliderPosition - mSlider->getHeight()) )
			value = (mMaxSliderPosition - mSlider->getHeight());

		mSlider->setYPosition(mMinSliderPosition + value);

		ScrollEventArgs scrollArgs(this);
		Ogre::Real currentValue = getValue();
		if( value < currentValue )
			scrollArgs.sliderIncreasedPosition = false;
		else
			scrollArgs.sliderIncreasedPosition = true;

		_scroll(0,scrollArgs);
	}

	void VerticalScrollBar::show()
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

	void VerticalScrollBar::timeElapsed(Ogre::Real time)
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

			if(mScrollUp1->isDown() || mScrollUp2->isDown())
			{
				scrollUpSmall();
				return;
			}
			
			if(mScrollDown1->isDown() || mScrollDown2->isDown())
			{
				scrollDownSmall();
				return;
			}

			mRepeatTimer = 0;
		}
	}
}
