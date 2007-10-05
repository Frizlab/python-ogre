#include "QuickGUIScrollPane.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	ScrollPane::ScrollPane(const Ogre::String& instanceName, const Rect& pixelDimensions, GUIManager* gm) :
		Widget(instanceName,pixelDimensions,"",gm),
		mScrollBarWidth(20),
		mHorizontalButtonLayout(HorizontalScrollBar::BUTTON_LAYOUT_OPPOSITE),
		mVerticalButtonLayout(VerticalScrollBar::BUTTON_LAYOUT_OPPOSITE),
		mTopBar(0),
		mLeftBar(0)
	{
		mWidgetType = TYPE_SCROLL_PANE;
		mGainFocusOnClick = false;
/*
		mTopBar = new HorizontalScrollBar(parentName+".TopScrollBar",TYPE_SCROLLBAR_HORIZONTAL,Rect(mScrollBarWidth,0,parentPixelSize.width - (mScrollBarWidth*2.0),mScrollBarWidth),skinSet+".scrollbar.horizontal.png",container,mParentWidget,gm);
		mTopBar->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		mTopBar->setShowWithParent(false);
		mTopBar->hide();
		mTopBar->addOnScrollEventHandler(&ScrollPane::onHorizontalScroll,this);
*/
		mBottomBar = new HorizontalScrollBar(mInstanceName+".BottomScrollBar",Rect(0,mSize.height - 20,mSize.width - (20),20),"qgui.scrollbar.horizontal.png",gm);
		mBottomBar->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		mBottomBar->setVerticalAnchor(ANCHOR_VERTICAL_BOTTOM);
		mBottomBar->addOnScrollEventHandler(&ScrollPane::onHorizontalScroll,this);
/*
		mLeftBar = new VerticalScrollBar(parentName+".LeftScrollBar",TYPE_SCROLLBAR_VERTICAL,Rect(0,mScrollBarWidth,mScrollBarWidth,parentPixelSize.height - (mScrollBarWidth*2.0)),skinSet+".scrollbar.vertical.png",container,mParentWidget,gm);
		mLeftBar->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		mLeftBar->setShowWithParent(false);
		mLeftBar->hide();
		mLeftBar->addOnScrollEventHandler(&ScrollPane::onVerticalScroll,this);
*/
		mRightBar = new VerticalScrollBar(mInstanceName+".RightScrollBar",Rect(mSize.width - 20,0,20,mSize.height - 20),"qgui.scrollbar.vertical.png",gm);
		mRightBar->setHorizontalAnchor(ANCHOR_HORIZONTAL_RIGHT);
		mRightBar->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		mRightBar->addOnScrollEventHandler(&ScrollPane::onVerticalScroll,this);
	}

	ScrollPane::~ScrollPane()
	{
	}

	void ScrollPane::_determinePaneBounds()
	{
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

	void ScrollPane::_syncBarWithParentDimensions()
	{
		Size parentSize = mParentWidget->getSize();

		if(mSize.width <= parentSize.width)
		{
			if(mTopBar)
				mTopBar->hide();
			mBottomBar->hide();
		}
		else
		{
			if(mTopBar)
				mTopBar->show();
			mBottomBar->show();
		}

		if(mSize.height <= parentSize.height)
		{
			if(mLeftBar)
				mLeftBar->hide();
			mRightBar->hide();
		}
		else
		{
			if(mLeftBar)
				mLeftBar->show();
			mRightBar->show();
		}

		// sync slider size
		// Take into consideration that scrollbar's take up some of the view.
		if(mRightBar->isVisible())
			parentSize.width -= mRightBar->getWidth();
		if(mBottomBar->isVisible())
			parentSize.height -= mBottomBar->getHeight();
		
		if(mTopBar)
			mTopBar->setSliderWidth(parentSize.width / mSize.width);
		mBottomBar->setSliderWidth(parentSize.width / mSize.width);
		if(mLeftBar)
			mLeftBar->setSliderHeight(parentSize.height / mSize.height);
		mRightBar->setSliderHeight(parentSize.height / mSize.height);
	}

	HorizontalScrollBar* ScrollPane::getBottomScrollBar()
	{
		return mBottomBar;
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

	void ScrollPane::onChildAddedToParent(const EventArgs& args)
	{
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;
		Type t = w->getWidgetType();
		if( (t == TYPE_TITLEBAR) || 
			(t == TYPE_MENU) || 
			(t == TYPE_SCROLLBAR_HORIZONTAL) ||
			(t == TYPE_SCROLLBAR_VERTICAL) ||
			(t == TYPE_BORDER) )
			return;

		w->addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&ScrollPane::onChildClicked,this);
		w->addEventHandler(EVENT_POSITION_CHANGED,&ScrollPane::onChildPositionChanged,this);
		w->addEventHandler(EVENT_SIZE_CHANGED,&ScrollPane::onChildSizeChanged,this);

		mManagedWidgets.push_back(w);

		Point widgetExtents = w->getPosition() + w->getSize();
		bool resized = false;
		if(widgetExtents.x > mSize.width)
		{
			setWidth(widgetExtents.x);
			resized = true;
		}
		if(widgetExtents.y > mSize.height)
		{
			setHeight(widgetExtents.y);
			resized = true;
		}

		if(resized)
		{
			_syncBarWithParentDimensions();

			mBottomBar->setValue(-(mPosition.x) / mSize.width);
			mRightBar->setValue(-(mPosition.y) / mSize.height);
		}
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

	void ScrollPane::onChildClicked(const EventArgs& args)
	{
		scrollIntoView(dynamic_cast<const WidgetEventArgs&>(args).widget);
	}

	void ScrollPane::onChildPositionChanged(const EventArgs& args)
	{
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;
		Point widgetExtents = w->getPosition() + w->getSize();
		bool resized = false;
		if(widgetExtents.x > mSize.width)
		{
			setWidth(widgetExtents.x);
			resized = true;
		}
		if(widgetExtents.y > mSize.height)
		{
			setHeight(widgetExtents.y);
			resized = true;
		}

		if(resized)
		{
			_syncBarWithParentDimensions();

			mBottomBar->setValue(-(mPosition.x) / mSize.width);
			mRightBar->setValue(-(mPosition.y) / mSize.height);
		}
	}

	void ScrollPane::onChildSizeChanged(const EventArgs& args)
	{
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;
		Point widgetExtents = w->getPosition() + w->getSize();
		bool resized = false;
		if(widgetExtents.x > mSize.width)
		{
			setWidth(widgetExtents.x);
			resized = true;
		}
		if(widgetExtents.y > mSize.height)
		{
			setHeight(widgetExtents.y);
			resized = true;
		}

		if(resized)
		{
			_syncBarWithParentDimensions();

			mBottomBar->setValue(-(mPosition.x) / mSize.width);
			mRightBar->setValue(-(mPosition.y) / mSize.height);
		}
	}

	void ScrollPane::onParentSizeChanged(const EventArgs& args)
	{
		_syncBarWithParentDimensions();
	}

	void ScrollPane::onHorizontalScroll(const EventArgs& args)
	{
		// sync both bars
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;
		if( mTopBar && (w->getInstanceName() == mTopBar->getInstanceName()) )
			mBottomBar->_setValue(mTopBar->getValue());
		else if( w->getInstanceName() == mBottomBar->getInstanceName() )
			if(mTopBar)
				mTopBar->_setValue(mBottomBar->getValue());

		// Move Scroll Pane
		setXPosition(-(mBottomBar->getValue()) * mSize.width);

		// Get parent's on-screen dimensions.
		Rect parentDimensions(mParentWidget->getScreenPosition() + mParentWidget->getScrollOffset(),mParentWidget->getSize());

		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			(*it)->_setScrollXOffset(mPosition.x);
		}
	}

	void ScrollPane::onVerticalScroll(const EventArgs& args)
	{
		// sync both bars
		Widget* w = dynamic_cast<const WidgetEventArgs&>(args).widget;
		if( mLeftBar && (w->getInstanceName() == mLeftBar->getInstanceName()) )
			mRightBar->_setValue(mLeftBar->getValue());
		else if( w->getInstanceName() == mRightBar->getInstanceName() )
			if(mLeftBar)
				mLeftBar->_setValue(mRightBar->getValue());

		// Move Scroll Pane
		setYPosition(-(mRightBar->getValue()) * mSize.height);

		// Get parent's on-screen dimensions.
		Rect parentDimensions(mParentWidget->getScreenPosition() + mParentWidget->getScrollOffset(),mParentWidget->getSize());

		std::vector<Widget*>::iterator it;
		for( it = mManagedWidgets.begin(); it != mManagedWidgets.end(); ++it )
		{
			(*it)->_setScrollYOffset(mPosition.y);
		}
	}

	void ScrollPane::setHorizontalButtonLayout(HorizontalScrollBar::ButtonLayout layout)
	{
		mHorizontalButtonLayout = layout;
		if(mTopBar)
			mTopBar->setButtonLayout(mHorizontalButtonLayout);
		mBottomBar->setButtonLayout(mHorizontalButtonLayout);
	}

	void ScrollPane::scrollIntoView(Widget* w)
	{
		Rect wDimensions(w->getScreenPosition(),w->getSize());
		if(!wDimensions.inside(Rect(getScreenPosition(),mSize)))
			return;

		Point parentPosition = mParentWidget->getPosition();
		Size parentSize = mParentWidget->getSize();
		Point parentScreenPos = mParentWidget->getScreenPosition() + mParentWidget->getScrollOffset();

		Point widgetPosition = w->getPosition();
		Size widgetSize = w->getSize();
		Point widgetScreenPos = w->getScreenPosition() + w->getScrollOffset();

		// see if we will be scrolling left, right, or not at all
		if( widgetScreenPos.x < parentScreenPos.x )
		{
			mBottomBar->setValue(widgetPosition.x / mSize.width);
		}
		else if( (widgetScreenPos.x + wDimensions.width) > (parentScreenPos.x + parentSize.width) )
		{
			mBottomBar->setValue((widgetPosition.x + wDimensions.width) / mSize.width);
		}

		// see if we will be scrolling up, down, or not at all
		if( widgetScreenPos.y < parentScreenPos.y )
		{
			mRightBar->setValue((parentPosition.y - widgetPosition.y) / mSize.height);
		}
		else if( (widgetScreenPos.y + wDimensions.height) > (parentScreenPos.y + parentSize.height) )
		{
			mRightBar->setValue((widgetPosition.y + wDimensions.height) / mSize.height);
		}
	}

	void ScrollPane::setParent(Widget* parent)
	{
		mParentWidget = parent;

		if(mParentWidget != NULL)
		{
			_detectHierarchy();
			// set the correct offset
			setOffset(mParentWidget->getOffset() + 1);
			setSize(mSize);
			setPosition(mPosition);
			// calculated properties
			Size parentSize = mParentWidget->getSize();
			mPixelsFromParentRight = parentSize.width - (mPosition.x + mSize.width);
			mPixelsFromParentBottom = parentSize.height - (mPosition.y + mSize.height);
			setClippingWidget(mParentWidget,true);
			// inheritted properties
			if(!mParentWidget->isVisible())
				hide();
			setQuadLayer(mParentWidget->getQuadLayer());
			mGainFocusOnClick = mParentWidget->getGainFocusOnClick();

			mParentWidget->addChild(mBottomBar);
			mParentWidget->addChild(mRightBar);

			mParentWidget->addEventHandler(EVENT_CHILD_ADDED,&ScrollPane::onChildAddedToParent,this);
			mParentWidget->addEventHandler(EVENT_CHILD_REMOVED,&ScrollPane::onChildRemovedFromParent,this);
			mParentWidget->addEventHandler(EVENT_SIZE_CHANGED,&ScrollPane::onParentSizeChanged,this);
		}
	}

	void ScrollPane::setVerticalButtonLayout(VerticalScrollBar::ButtonLayout layout)
	{
		mVerticalButtonLayout = layout;
		if(mLeftBar)
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
