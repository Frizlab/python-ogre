#include "QuickGUIScrollPane.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	ScrollPane::ScrollPane(const Ogre::String& instanceName, Type type, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Widget(instanceName,type,Rect(0,0,1,1),"",container,ParentWidget,gm),
		mScrollBarWidth(20),
		mHorizontalButtonLayout(HorizontalScrollBar::BUTTON_LAYOUT_OPPOSITE),
		mVerticalButtonLayout(VerticalScrollBar::BUTTON_LAYOUT_OPPOSITE),
		mHorizontalBarLayout(HORIZONTAL_BAR_LAYOUT_BOTTOM),
		mVerticalBarLayout(VERTICAL_BAR_LAYOUT_RIGHT)
	{
		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_SCROLL_PANE)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);
		}
		
		mGainFocusOnClick = false;

		// Create 4 ScrollBars, 2 vertical, 2 horizontal.  Set the parent to this widget's parent.
		Ogre::String parentName = mParentWidget->getInstanceName();
		
		Ogre::String skinSet;
		Ogre::String parentTexture = mParentWidget->getTextureName(true);
		if(parentTexture == "")
			skinSet = mParentSheet->getDefaultSkin();
		else
			skinSet = parentTexture.substr(0,parentTexture.find_first_of('.'));
		
		Rect parentPixelSize = mParentWidget->getDimensions();

		mTopBar = new HorizontalScrollBar(parentName+".TopScrollBar",TYPE_SCROLLBAR_HORIZONTAL,Rect(mScrollBarWidth,0,parentPixelSize.width - (mScrollBarWidth*2.0),mScrollBarWidth),skinSet+".scrollbar.horizontal.png",container,mParentWidget,gm);
		mTopBar->setShowWithParent(false);
		mTopBar->hide();
		mTopBar->addOnScrollEventHandler(&ScrollPane::onHorizontalScroll,this);

		mBottomBar = new HorizontalScrollBar(parentName+".BottomScrollBar",TYPE_SCROLLBAR_HORIZONTAL,Rect(mScrollBarWidth,parentPixelSize.height - mScrollBarWidth,parentPixelSize.width - (mScrollBarWidth*2.0),mScrollBarWidth),skinSet+".scrollbar.horizontal.png",container,mParentWidget,gm);
		mBottomBar->setShowWithParent(false);
		mBottomBar->hide();
		mBottomBar->addOnScrollEventHandler(&ScrollPane::onHorizontalScroll,this);

		mLeftBar = new VerticalScrollBar(parentName+".LeftScrollBar",TYPE_SCROLLBAR_VERTICAL,Rect(0,mScrollBarWidth,mScrollBarWidth,parentPixelSize.height - (mScrollBarWidth*2.0)),skinSet+".scrollbar.vertical.png",container,mParentWidget,gm);
		mLeftBar->setShowWithParent(false);
		mLeftBar->hide();
		mLeftBar->addOnScrollEventHandler(&ScrollPane::onVerticalScroll,this);

		mRightBar = new VerticalScrollBar(parentName+".RightScrollBar",TYPE_SCROLLBAR_VERTICAL,Rect(parentPixelSize.width - mScrollBarWidth,mScrollBarWidth,mScrollBarWidth,parentPixelSize.height - (mScrollBarWidth*2.0)),skinSet+".scrollbar.vertical.png",container,mParentWidget,gm);
		mRightBar->setShowWithParent(false);
		mRightBar->hide();
		mRightBar->addOnScrollEventHandler(&ScrollPane::onVerticalScroll,this);

		setHorizontalButtonLayout(mHorizontalButtonLayout);
		setVerticalButtonLayout(mVerticalButtonLayout);

		mParentWidget->addEventHandler(EVENT_CHILD_ADDED,&ScrollPane::onChildAddedToParent,this);
		mParentWidget->addEventHandler(EVENT_CHILD_REMOVED,&ScrollPane::onChildRemovedFromParent,this);
		mParentWidget->addEventHandler(EVENT_POSITION_CHANGED,&ScrollPane::onParentPositionChanged,this);
		mParentWidget->addEventHandler(EVENT_SIZE_CHANGED,&ScrollPane::onParentSizeChanged,this);
		addEventHandler(EVENT_SIZE_CHANGED,&ScrollPane::onSizeChanged,this);

		_adjustBarDimensions();
	}

	ScrollPane::~ScrollPane()
	{
	}

	void ScrollPane::_adjustBarDimensions()
	{
		bool leftBarUsed = ((mVerticalBarLayout == VERTICAL_BAR_LAYOUT_LEFT) || (mVerticalBarLayout == VERTICAL_BAR_LAYOUT_BOTH));
		bool rightBarUsed = ((mVerticalBarLayout == VERTICAL_BAR_LAYOUT_RIGHT) || (mVerticalBarLayout == VERTICAL_BAR_LAYOUT_BOTH));
		bool topBarUsed = ((mHorizontalBarLayout == HORIZONTAL_BAR_LAYOUT_TOP) || (mVerticalBarLayout == HORIZONTAL_BAR_LAYOUT_BOTH));
		bool bottomBarUsed = ((mHorizontalBarLayout == HORIZONTAL_BAR_LAYOUT_BOTTOM) || (mVerticalBarLayout == HORIZONTAL_BAR_LAYOUT_BOTH));

		// Determine Left and Right Bar Dimensions and Position
		if(topBarUsed)
		{
			if(bottomBarUsed)
			{
				mLeftBar->setYPosition(mScrollBarWidth);
				mLeftBar->setHeight(mParentWidget->getHeight() - (mScrollBarWidth*2));
				mRightBar->setYPosition(mScrollBarWidth);
				mRightBar->setHeight(mParentWidget->getHeight() - (mScrollBarWidth*2));
			}
			else
			{
				mLeftBar->setYPosition(mScrollBarWidth);
				mLeftBar->setHeight(mParentWidget->getHeight() - mScrollBarWidth);
				mRightBar->setYPosition(mScrollBarWidth);
				mRightBar->setHeight(mParentWidget->getHeight() - mScrollBarWidth);
			}
		}
		else
		{
			if(bottomBarUsed)
			{
				mLeftBar->setYPosition(0);
				mLeftBar->setHeight(mParentWidget->getHeight() - mScrollBarWidth);
				mRightBar->setYPosition(0);
				mRightBar->setHeight(mParentWidget->getHeight() - mScrollBarWidth);
			}
			else
			{
				mLeftBar->setYPosition(0);
				mLeftBar->setHeight(mParentWidget->getHeight());
				mRightBar->setYPosition(0);
				mRightBar->setHeight(mParentWidget->getHeight());
			}
		}

		// Determine Top and Bottom Bar Dimensions and Position
		if(leftBarUsed)
		{
			if(rightBarUsed)
			{
				mTopBar->setXPosition(mScrollBarWidth);
				mTopBar->setWidth(mParentWidget->getWidth() - (mScrollBarWidth*2));
				mBottomBar->setXPosition(mScrollBarWidth);
				mBottomBar->setWidth(mParentWidget->getWidth() - (mScrollBarWidth*2));
			}
			else
			{
				mTopBar->setXPosition(mScrollBarWidth);
				mTopBar->setWidth(mParentWidget->getWidth() - mScrollBarWidth);
				mBottomBar->setXPosition(mScrollBarWidth);
				mBottomBar->setWidth(mParentWidget->getWidth() - mScrollBarWidth);
			}
		}
		else
		{
			if(rightBarUsed)
			{
				mTopBar->setXPosition(0);
				mTopBar->setWidth(mParentWidget->getWidth() - mScrollBarWidth);
				mBottomBar->setXPosition(0);
				mBottomBar->setWidth(mParentWidget->getWidth() - mScrollBarWidth);
			}
			else
			{
				mTopBar->setXPosition(0);
				mTopBar->setWidth(mParentWidget->getWidth());
				mBottomBar->setXPosition(0);
				mBottomBar->setWidth(mParentWidget->getWidth());
			}
		}

		mBottomBar->setValue(-(mPosition.x) / mSize.width);
		mRightBar->setValue(-(mPosition.y) / mSize.height);
	}

	void ScrollPane::_determinePaneBounds()
	{
		if(!mEnabled)
			return;

		// Scroll Pane should not be smaller than parent width/height
		Size parentSize = mParentWidget->getSize();

		// find the minimum and maximum region encompassing the managed widgets.
		Ogre::Real width = parentSize.width;
		Ogre::Real height = parentSize.height;

		// Get min/max bounds for scroll pane.  By default, pane has same bounds as parent.
		// This may change depending on managed widgets that may lie above/below/left/right of current pane bounds.
		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			Rect wPixelDimensions = (*it)->getDimensions();
			
			if( (wPixelDimensions.x + wPixelDimensions.width) > width )
				width = (wPixelDimensions.x + wPixelDimensions.width);

			if( (wPixelDimensions.y + wPixelDimensions.height) > height )
				height = (wPixelDimensions.y + wPixelDimensions.height);
		}

		setSize(width,height);

		_syncBarWithParentDimensions();

		mBottomBar->setValue(-(mPosition.x) / mSize.width);
		mRightBar->setValue(-(mPosition.y) / mSize.height);
	}

	void ScrollPane::_showHScrollBars()
	{
		switch(mHorizontalBarLayout)
		{
		case HORIZONTAL_BAR_LAYOUT_TOP:
			if(!mTopBar->isVisible())
				mTopBar->show();
			break;
		case HORIZONTAL_BAR_LAYOUT_BOTTOM:
			if(!mBottomBar->isVisible())
				mBottomBar->show();
			break;
		case HORIZONTAL_BAR_LAYOUT_BOTH:
			if(!mTopBar->isVisible())
				mTopBar->show();
			if(!mBottomBar->isVisible())
				mBottomBar->show();
			break;
		case HORIZONTAL_BAR_LAYOUT_NONE:
			// show nothing
			break;
		}
	}

	void ScrollPane::_showVScrollBars()
	{
		switch(mVerticalBarLayout)
		{
		case VERTICAL_BAR_LAYOUT_LEFT:
			if(!mLeftBar->isVisible())
				mLeftBar->show();
			break;
		case VERTICAL_BAR_LAYOUT_RIGHT:
			if(!mRightBar->isVisible())
				mRightBar->show();
			break;
		case VERTICAL_BAR_LAYOUT_BOTH:
			if(!mLeftBar->isVisible())
				mLeftBar->show();
			if(!mRightBar->isVisible())
				mRightBar->show();
			break;
		case VERTICAL_BAR_LAYOUT_NONE:
			// show nothing`	
			break;
		}
	}

	void ScrollPane::_syncBarWithParentDimensions()
	{
		if(!mEnabled)
			return;

		if(mSize.width <= mParentWidget->getWidth())
		{
			if(mTopBar->isVisible())
				mTopBar->hide();
			if(mBottomBar->isVisible())
				mBottomBar->hide();
		}
		else
			_showHScrollBars();

		if(mSize.height <= mParentWidget->getHeight())
		{
			if(mLeftBar->isVisible())
				mLeftBar->hide();
			if(mRightBar->isVisible())
				mRightBar->hide();
		}
		else
			_showVScrollBars();

		// sync slider size
		Size parentSize = mParentWidget->getSize();
		// Take into consideration that scrollbar's take up some of the view.
		if(mRightBar->isVisible())
			parentSize.width -= mRightBar->getWidth();
		if(mBottomBar->isVisible())
			parentSize.height -= mBottomBar->getHeight();
		
		mTopBar->setSliderWidth(parentSize.width / mSize.width);
		mBottomBar->setSliderWidth(parentSize.width / mSize.width);
		mLeftBar->setSliderHeight(parentSize.height / mSize.height);
		mRightBar->setSliderHeight(parentSize.height / mSize.height);
	}

	void ScrollPane::disable()
	{
		WidgetEventArgs args(this);
		fireEvent(EVENT_DISABLED,args);

		mEnabled = false;
	}

	void ScrollPane::enable()
	{
		mEnabled = true;

		_determinePaneBounds();	

		WidgetEventArgs args(this);
		fireEvent(EVENT_ENABLED,args);
	}

	HorizontalScrollBar* ScrollPane::getBottomScrollBar()
	{
		return mBottomBar;
	}

	HorizontalScrollBar::ButtonLayout ScrollPane::getHorizontalButtonLayout()
	{
		return mHorizontalButtonLayout;
	}

	VerticalScrollBar* ScrollPane::getLeftScrollBar()
	{
		return mLeftBar;
	}

	VerticalScrollBar* ScrollPane::getRightScrollBar()
	{
		return mRightBar;
	}

	HorizontalScrollBar* ScrollPane::getTopScrollBar()
	{
		return mTopBar;
	}

	VerticalScrollBar::ButtonLayout ScrollPane::getVerticalButtonLayout()
	{
		return mVerticalButtonLayout;
	}

	void ScrollPane::onChildAddedToParent(const EventArgs& args)
	{
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;

		if((w->getWidgetType() == TYPE_TITLEBAR) || (w->getWidgetType() == TYPE_MENU))
			return;

		w->setClippingRect(mParentWidget->getDimensions());

		if(w->getWidgetType() == TYPE_TEXTBOX)
			w->addEventHandler(EVENT_GAIN_FOCUS,&ScrollPane::onChildTextBoxGainedFocus,this);

		mManagedWidgets.push_back(w);

		_determinePaneBounds();
	}

	void ScrollPane::onChildRemovedFromParent(const EventArgs& args)
	{
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;

		// remove widget pointer from managed list
		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			if( w->getInstanceName() == (*it)->getInstanceName() )
			{
				mManagedWidgets.erase(it);
				break;
			}
		}

		_determinePaneBounds();
	}

	void ScrollPane::onChildTextBoxGainedFocus(const EventArgs& args)
	{
	}

	void ScrollPane::onParentPositionChanged(const EventArgs& args)
	{
		if(!mEnabled)
			return;

		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
			(*it)->setClippingRect(mParentWidget->getDimensions());
	}

	void ScrollPane::onParentSizeChanged(const EventArgs& args)
	{
		if(!mEnabled)
			return;

		_syncBarWithParentDimensions();
	}

	void ScrollPane::onHorizontalScroll(const EventArgs& args)
	{
		if(!mEnabled)
			return;

		// sync both bars
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;
		if( w->getInstanceName() == mTopBar->getInstanceName() )
			mBottomBar->_setValue(mTopBar->getValue());
		else if( w->getInstanceName() == mBottomBar->getInstanceName() )
			mTopBar->_setValue(mBottomBar->getValue());

		// Move Scroll Pane
		Ogre::Real topValue = mTopBar->getValue();
		Ogre::Real botValue = mBottomBar->getValue();
		setXPosition(-(topValue) * mSize.width);

		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			// In the event of scroll panes inside scroll panes, moving a scroll pane will move the inner scroll panes.
			// In this case, we need to update the clipping rect.
			(*it)->setClippingRect(mParentWidget->getDimensions());
			(*it)->_setScrollXOffset(mPosition.x);
		}
	}

	void ScrollPane::onSizeChanged(const EventArgs& args)
	{
		if(!mEnabled)
			return;

		_syncBarWithParentDimensions();
	}

	void ScrollPane::onVerticalScroll(const EventArgs& args)
	{
		if(!mEnabled)
			return;

		// sync both bars
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;
		if( w->getInstanceName() == mLeftBar->getInstanceName() )
			mRightBar->_setValue(mLeftBar->getValue());
		else if( w->getInstanceName() == mRightBar->getInstanceName() )
			mLeftBar->_setValue(mRightBar->getValue());

		// Move Scroll Pane
		setYPosition(-(mLeftBar->getValue()) * mSize.height);

		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			// In the event of scroll panes inside scroll panes, moving a scroll pane will move the inner scroll panes.
			// In this case, we need to update the clipping rect.
			(*it)->setClippingRect(mParentWidget->getDimensions());
			(*it)->_setScrollYOffset(mPosition.y);
		}
	}

	void ScrollPane::setHorizontalBarLayout(HorizontalBarLayout layout)
	{
		mHorizontalBarLayout = layout;
		_syncBarWithParentDimensions();
		_adjustBarDimensions();
	}

	void ScrollPane::setHorizontalButtonLayout(HorizontalScrollBar::ButtonLayout layout)
	{
		mHorizontalButtonLayout = layout;
		mTopBar->setButtonLayout(mHorizontalButtonLayout);
		mBottomBar->setButtonLayout(mHorizontalButtonLayout);
	}

	void ScrollPane::scrollIntoView(Widget* w)
	{
		if((mParentWidget->getWidgetType() == TYPE_LIST) && dynamic_cast<List*>(mParentWidget)->getAutoSizeHeight())
			return;

		Rect wDimensions(w->getScreenPosition(),w->getSize());
		if(!wDimensions.inside(Rect(getScreenPosition(),mSize)))
			return;

		Point screenPos = getScreenPosition();
		Ogre::Real leftX = ((wDimensions.x - screenPos.x) / mSize.width);
		Ogre::Real rightX = (((wDimensions.x + wDimensions.width) - screenPos.x) / mSize.width);

		// see if we will be scrolling left, right, or not at all
		Ogre::Real hSliderValue = mTopBar->getValue();
		if( leftX < hSliderValue )
		{
			// Only need to set value of one, callbacks will sync the other scrollbar.
			mTopBar->setValue(leftX);
		}
		else if( rightX > (hSliderValue + mTopBar->getSliderWidth()) )
		{
			// Only need to set value of one, callbacks will sync the other scrollbar.
			mTopBar->setValue(rightX * (1 - mTopBar->getSliderWidth()));
		}

		Ogre::Real topY = ((wDimensions.y - screenPos.y) / mSize.height);
		Ogre::Real botY = (((wDimensions.y + wDimensions.height) - screenPos.y) / mSize.height);
		// see if we will be scrolling up, down, or not at all
		Ogre::Real vSliderValue = mLeftBar->getValue();
		if( topY < vSliderValue )
		{
			// Only need to set value of one, callbacks will sync the other scrollbar.
			mLeftBar->setValue(topY);
		}
		else if( botY > (vSliderValue + mLeftBar->getSliderHeight()) )
		{
			// Only need to set value of one, callbacks will sync the other scrollbar.
			mLeftBar->setValue(botY - mLeftBar->getSliderHeight());
		}
	}

	void ScrollPane::setVerticalBarLayout(VerticalBarLayout layout)
	{
		mVerticalBarLayout = layout;
		_syncBarWithParentDimensions();
		_adjustBarDimensions();
	}

	void ScrollPane::setVerticalButtonLayout(VerticalScrollBar::ButtonLayout layout)
	{
		mVerticalButtonLayout = layout;
		mLeftBar->setButtonLayout(mVerticalButtonLayout);
		mRightBar->setButtonLayout(mVerticalButtonLayout);
	}

	void ScrollPane::setPosition(const Ogre::Real& pixelX, const Ogre::Real& pixelY)
	{
		Widget::setPosition(pixelX,pixelY);
	}

	void ScrollPane::setPosition(const Point& pixelPosition)
	{
		Widget::setPosition(pixelPosition);
	}
}
