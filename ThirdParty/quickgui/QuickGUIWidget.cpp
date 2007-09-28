#include "QuickGUIWidget.h"
#include "QuickGUIManager.h"
#include "QuickGUIPanel.h"
#include "QuickGUIWindow.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	Widget::Widget(const Ogre::String& instanceName, Type type, const Rect& pixelDimensions, Ogre::String textureName, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		mInstanceName(instanceName),
		mWidgetType(type),
		mParentWidget(ParentWidget),
		mGUIManager(gm),
		mPosition(Point(pixelDimensions.x,pixelDimensions.y)),
		mSize(Size(pixelDimensions.width,pixelDimensions.height)),
		mScrollOffset(Point::ZERO),
		mVisible(true),
		mGrabbed(false),
		mMovingEnabled(true),
		mDraggingEnabled(false),
		mEnabled(true),
		mWidgetImage(NULL),
		mQuadContainer(container),
		mOffset(0),
		mClippingRect(Rect(0,0,gm->getViewportWidth(),gm->getViewportHeight())),
		mTextureLocked(false),
		mGainFocusOnClick(true),
		mDragXOnly(false),
		mDragYOnly(false),
		mShowWithParent(true),
		mHideWithParent(true),
		mResizeOverTime(false),
		mRepositionOverTime(false),
		mInheritClippingRect(true)
	{
		mQuad = new Quad(instanceName + ".Quad",gm);
		mWidgetToDrag = this;
		_detectHierarchy();
		_initEventHandlers();

		// All widgets have a Quad base
		setTexture(textureName);

		if(!((mWidgetType == TYPE_SHEET) || (mWidgetType == TYPE_WINDOW) || (mWidgetType == TYPE_PANEL)))
			mQuad->_notifyQuadContainer(mQuadContainer);

		// Add event handlers before call to setDimensions, if you want the widget handlers to be called.
		// This is important for widgets to position themself correctly.
		addEventHandler(EVENT_POSITION_CHANGED,&Widget::onPositionChanged,this);

		setPosition(mPosition);
		setSize(mSize);

		if(mParentWidget != NULL)
		{
			// set the correct offset
			mOffset = mParentWidget->getOffset() + 1;
			// add to parents child widget list
			mParentWidget->_addToChildList(this);
			// inheritted properties
			mGainFocusOnClick = mParentWidget->getGainFocusOnClick();
			mClippingRect = mParentWidget->getClippingRect();
		}

		mQuad->setOffset(mOffset);
	}

	Widget::~Widget()
	{
		// Safest route is to destroy children first thing.
		removeAndDestroyAllChildWidgets();

		delete mQuad;

		if( mParentWidget != NULL )
		{
			mParentWidget->_removeFromChildList(this);
			mParentWidget = NULL;
		}

		delete mWidgetImage;

		// Cleanup Event Handlers.
		std::vector< std::vector<MemberFunctionSlot*> >::iterator it1;
		for( it1 = mUserEventHandlers.begin(); it1 != mUserEventHandlers.end(); ++it1 )
		{
			std::vector<MemberFunctionSlot*>::iterator it2;
			for( it2 = (*it1).begin(); it2 != (*it1).end(); ++it2 )
				delete (*it2);
			it1->clear();
		}
		mUserEventHandlers.clear();

		// Remove name from GUIManager name list. (So another widget can be created with this name, if desired)
		mGUIManager->removeWidgetName(mInstanceName);
	}

	void Widget::_addToChildList(Widget* w)
	{
		mChildWidgets.push_back(w);
		w->_notifyParent(this);

		WidgetEventArgs args(w);
		fireEvent(EVENT_CHILD_ADDED,args);
	}

	void Widget::_detectHierarchy()
	{
		mParentSheet = NULL;
		mParentWindow = NULL;
		mParentPanel = NULL;

		Widget* w = mParentWidget;
		while( w != NULL )
		{
			switch(w->getWidgetType())
			{
			case TYPE_SHEET:
				mParentSheet = dynamic_cast<Sheet*>(w);
				break;
			case TYPE_WINDOW:
				mParentWindow = dynamic_cast<Window*>(w);
				break;
			case TYPE_PANEL:
				// It is possible to have a widget inside a panel inside a panel..
				// we need to gaurd against this, we only want the immediate parent panel.
				if( mParentPanel == NULL ) 
					mParentPanel = dynamic_cast<Panel*>(w);
				break;
			default: break;
			}

			w = w->getParentWidget();
		}
	}

	void Widget::_initEventHandlers()
	{
		int index = 0;
		mUserEventHandlers.clear();
		// 22 common types of events currently
		while( index < 22 )
		{
			std::vector<MemberFunctionSlot*> eventTypeHandler;
			eventTypeHandler.clear();
			mUserEventHandlers.push_back(eventTypeHandler);
			++index;
		}
	}

	void Widget::_notifyParent(Widget* w)
	{
		if( mParentWidget == NULL )
			mParentWidget = w;
	}

	void Widget::_notifyRemovedFromChildList()
	{
		mParentWidget = NULL;
	}

	void Widget::_notifyQuadContainer(QuadContainer* g)
	{
		mQuadContainer = g;

		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->_notifyQuadContainer(g);
	}

	void Widget::_processFullTextureName(const Ogre::String& texture)
	{
		mFullTextureName = texture;

		if(mFullTextureName != "")
		{
			Ogre::String::size_type index = texture.find_last_of('.');
			if( index != Ogre::String::npos )
			{
				mTextureName = texture.substr(0,index);
				mTextureExtension = texture.substr(index,texture.length() - index);
			}
		}
		else
		{
			mTextureName = "";
			mTextureExtension = "";
		}
	}

	void Widget::_removeFromChildList(Widget* w)
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( w->getInstanceName() == (*it)->getInstanceName() )
			{
				(*it)->_notifyRemovedFromChildList();
				mChildWidgets.erase(it);
				
				fireEvent(EVENT_CHILD_REMOVED,WidgetEventArgs(w));
				return;
			}
		}
	}

	void Widget::_setScrollXOffset(Ogre::Real pixelXOffset)
	{
		mScrollOffset.x = pixelXOffset;
		mQuad->setXPosition(getScreenPosition().x + getScrollOffset().x);
	}

	void Widget::_setScrollYOffset(Ogre::Real pixelYOffset)
	{
		mScrollOffset.y = pixelYOffset;
		mQuad->setYPosition(getScreenPosition().y + getScrollOffset().y);
	}

	void Widget::addEventHandler(Event EVENT, MemberFunctionSlot* function)
	{
		mUserEventHandlers[EVENT].push_back(function);
	}

	void Widget::appearOverWidget(Widget* w)
	{
		if( (w->getQuadContainer() == NULL) || (mQuadContainer == NULL) )
			return;

		if( (w->getQuadContainer()->getID() != mQuadContainer->getID()) ||
			(w->getQuad()->getLayer() != mQuad->getLayer()) )
			return;

		setOffset(w->getHighestOffset() + 1);
	}

	void Widget::constrainDragging(bool DragXOnly, bool DragYOnly)
	{
		mDragXOnly = false;
		mDragYOnly = false;

		if(DragXOnly && DragYOnly)
			return;

		mDragXOnly = DragXOnly;
		mDragYOnly = DragYOnly;
	}

	void Widget::disable()
	{
		if((mWidgetType == Widget::TYPE_SHEET)) 
			return;

		setTexture(mDisabledTextureName,false);

		WidgetEventArgs args(this);
		fireEvent(EVENT_DISABLED,args);

		mEnabled = false;
	}

	void Widget::drag(const Ogre::Real& pixelX, const Ogre::Real& pixelY)
	{
		if(!mDraggingEnabled) 
			return;

		MouseEventArgs args(this);
		
		if(mWidgetToDrag != NULL) 
		{
			if(mDragXOnly)
			{
				args.moveDelta.x = pixelX;
				mWidgetToDrag->moveX(pixelX);
			}
			else if(mDragYOnly)
			{
				args.moveDelta.y = pixelY;
				mWidgetToDrag->moveY(pixelY);
			}
			else
			{
				args.moveDelta.x = pixelX;
				args.moveDelta.y = pixelY;
				mWidgetToDrag->move(pixelX,pixelY);
			}
		}

		// fire onDragged Event.		
		fireEvent(EVENT_DRAGGED,args);
	}

	bool Widget::draggingEnabled()
	{
		return mDraggingEnabled;
	}

	void Widget::enable()
	{
		if(mWidgetType == Widget::TYPE_SHEET) 
			return;

		mEnabled = true;

		setTexture(mFullTextureName,false);

		WidgetEventArgs args(this);
		fireEvent(EVENT_ENABLED,args);
	}

	bool Widget::enabled()
	{
		return mEnabled;
	}

	void Widget::enableDragging(bool enable)
	{
		mDraggingEnabled = enable;
	}

	void Widget::focus()
	{
		mGUIManager->setActiveWidget(this);
	}

	Ogre::String Widget::getDisabledTexture()
	{
		return mDisabledTextureName;
	}

	std::vector<Widget*>* Widget::getChildWidgetList()
	{
		return &mChildWidgets;
	}

	Widget* Widget::getChildWidget(const Ogre::String& name)
	{
		Widget* w = NULL;
		if( name == "" ) 
			return w;

		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getChildWidget(name) != NULL )
			{
				w = (*it);
				break;
			}
		}

		if( w != NULL ) 
			return w;
		else if( mInstanceName == name ) 
			return this;
		else 
			return NULL;
	}

	Rect Widget::getClippingRect()
	{
		return mClippingRect;
	}

	Ogre::String Widget::getDefaultSkin()
	{
		if(mParentSheet == NULL) return "qgui";
		else return mParentSheet->getDefaultSkin();
	}

	bool Widget::getGainFocusOnClick()
	{
		return mGainFocusOnClick;
	}

	bool Widget::getGrabbed()
	{
		return mGrabbed;
	}

	Ogre::Real Widget::getHeight()
	{
		return mSize.height;
	}

	bool Widget::getHideWithParent()
	{
		return mHideWithParent;
	}

	int Widget::getHighestOffset()
	{
		// iterate through child widgets..
		Widget* w = NULL;
		// Get the widget with the highest offset
		int widgetOffset = mOffset;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getWidgetType() == TYPE_SCROLL_PANE )
				continue;

			int temp = (*it)->getHighestOffset();
			if( temp > widgetOffset )
				widgetOffset = temp;
		}

		return widgetOffset;
	}

	bool Widget::getInheritClippingRect()
	{
		return mInheritClippingRect;
	}

	Ogre::String Widget::getInstanceName()
	{
		return mInstanceName;
	}

	bool Widget::getMovingEnabled()
	{
		return mMovingEnabled;
	}

	int Widget::getOffset()
	{
		return mOffset;
	}

	Panel* Widget::getParentPanel()
	{
		return mParentPanel;
	}

	Sheet* Widget::getParentSheet()
	{
		return mParentSheet;
	}

	Widget* Widget::getParentWidget()
	{
		return mParentWidget;
	}

	Window* Widget::getParentWindow()
	{
		return mParentWindow;
	}

	Quad* Widget::getQuad()
	{
		return mQuad;
	}

	QuadContainer* Widget::getQuadContainer()
	{
		return mQuadContainer;
	}

	Point Widget::getScreenPosition()
	{
		if(mParentWidget == NULL)
			return Point::ZERO;
		
		return mParentWidget->getScreenPosition() + mPosition;
	}

	bool Widget::getShowWithParent()
	{
		return mShowWithParent;
	}

	Rect Widget::getDimensions()
	{
		return Rect(mPosition,mSize);
	}

	GUIManager* Widget::getGUIManager()
	{
		return mGUIManager;
	}

	Point Widget::getPosition()
	{
		return mPosition;
	}

	Point Widget::getScrollOffset()
	{
		if( mParentWidget == NULL )
			return mScrollOffset;

		return mParentWidget->getScrollOffset() + mScrollOffset;
	}

	Size Widget::getSize()
	{
		return mSize;
	}

	Widget* Widget::getTargetWidget(const Point& pixelPosition)
	{
		if( !mVisible || !mEnabled || ((mQuadContainer == NULL) && (mWidgetType != TYPE_SHEET)) ) 
			return NULL;

		// iterate through child widgets..
		Widget* w = NULL;
		// Get the widget with the highest offset
		int widgetOffset = 0;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getWidgetType() == TYPE_SCROLL_PANE )
				continue;

			Widget* temp = (*it)->getTargetWidget(pixelPosition);
			if( (temp != NULL) && (temp->getOffset() > widgetOffset) )
			{
				w = temp;
				widgetOffset = temp->getOffset();
			}
		}

		if( w != NULL ) 
			return w;
		else if( isPointWithinBounds(pixelPosition) ) 
			return this;
		else 
			return NULL;
	}

	Ogre::String Widget::getTextureName(bool includeExtension)
	{
		if(includeExtension)
			return mFullTextureName;
		else
			return mTextureName;
	}

	Widget::Type Widget::getWidgetType()
	{
		return mWidgetType;
	}

	Ogre::Real Widget::getWidth()
	{
		return mSize.width;
	}

	Ogre::Real Widget::getXPosition()
	{
		return mPosition.x;
	}

	Ogre::Real Widget::getYPosition()
	{
		return mPosition.y;
	}

	void Widget::hide()
	{
		mGrabbed = false;

		mQuad->setVisible(false);

		// hide children
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if((*it)->getHideWithParent())
				(*it)->hide();
		}

		// Only fire event if we change visibility.  If we were already hidden, don't fire.
		if(mVisible)
		{
			WidgetEventArgs args(this);
			fireEvent(EVENT_HIDDEN,args);
		}

		mVisible = false;
	}

	bool Widget::isPointWithinBounds(const Point& pixelPosition)
	{
		if(!mVisible) 
			return false;

		Rect quadDimensions = mQuad->getDimensions();

		if(mWidgetType == TYPE_LISTITEM)
			quadDimensions = quadDimensions;

		if( pixelPosition.x < quadDimensions.x ||
			pixelPosition.x > (quadDimensions.x + quadDimensions.width) )
			return false;

		if( pixelPosition.y < quadDimensions.y ||
			pixelPosition.y > (quadDimensions.y + quadDimensions.height) )
			return false;

		if(overTransparentPixel(pixelPosition)) 
			return false;

		return true;
	}

	bool Widget::isVisible()
	{
		return mVisible;
	}

	bool Widget::isChild(Widget* w)
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( w->getInstanceName() == (*it)->getInstanceName() )
				return true;
		}

		return false;
	}

	void Widget::move(const Ogre::Real& pixelX, const Ogre::Real& pixelY)
	{
		if(!mMovingEnabled) 
			return;

		setPosition(mPosition.x + pixelX,mPosition.y + pixelY);
	}

	void Widget::move(const Point& pixelOffset)
	{
		move(pixelOffset.x,pixelOffset.y);
	}

	void Widget::moveX(Ogre::Real pixelX)
	{
		if(!mMovingEnabled) 
			return;

		setXPosition(mPosition.x + pixelX);
	}

	void Widget::moveY(Ogre::Real pixelY)
	{
		if(!mMovingEnabled) 
			return;

		setYPosition(mPosition.y + pixelY);
	}

	bool Widget::fireEvent(Event e, const EventArgs& args)
	{
		if(!mEnabled || mUserEventHandlers[e].empty()) 
			return false;

		std::vector<MemberFunctionSlot*>::iterator it;
		std::vector<MemberFunctionSlot*> userEventHandlers = mUserEventHandlers[e];
		for( it = userEventHandlers.begin(); it != userEventHandlers.end(); ++it )
			(*it)->execute(args);

		return true; 
	}

	void Widget::lockTexture()
	{
		mTextureLocked = true;
	}

	void Widget::onPositionChanged(const EventArgs& args)
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->setPosition((*it)->getPosition());
	}

	bool Widget::overTransparentPixel(const Point& mousePixelPosition)
	{
		if( mWidgetImage == NULL ) 
			return false;

		Point pt = mousePixelPosition - getScreenPosition();
		Ogre::Real relX = pt.x / mSize.width;
		Ogre::Real relY = pt.y / mSize.height;

		Ogre::ColourValue c = mWidgetImage->getColourAt(relX * mWidgetImage->getWidth(), relY * mWidgetImage->getHeight(),0);
		if( c.a <= 0.0 ) return true;

		return false;
	}

	void Widget::refresh()
	{
		setDimensions(getDimensions());

		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->refresh();
	}

	void Widget::removeAndDestroyAllChildWidgets()
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			(*it)->_notifyRemovedFromChildList();
			delete (*it);
		}
		mChildWidgets.clear();
	}

	void Widget::resizeOverTime(Ogre::Real seconds, Size finalPixelSize)
	{
		if(mParentWidget == NULL)
			return;

		mResizeOverTime = true;
		mResizeTime = seconds;
		mResizeTimer = 0;
		mInitialPixelSize = Size(mSize.width,mSize.height);

		mFinalPixelSize.width = finalPixelSize.width / mGUIManager->getViewportWidth();
		mFinalPixelSize.height = finalPixelSize.height / mGUIManager->getViewportHeight();
	}

	void Widget::setBaseTexture(const Ogre::String& textureName)
	{
		// separate extension from name, if exists.
		_processFullTextureName(textureName);

		mDisabledTextureName = mTextureName + ".disabled" + mTextureExtension;
	}

	void Widget::setClippingRect(const Rect& pixelDimensions)
	{
		mQuad->setClippingRect(pixelDimensions);

		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if((*it)->getInheritClippingRect())
				(*it)->setClippingRect(pixelDimensions);
		}
	}

	void Widget::timeElapsed(Ogre::Real time) 
	{
		if(!mEnabled) 
			return;

		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			(*it)->timeElapsed(time);
		}

		if(mResizeOverTime)
		{
			Ogre::Real totalWidthChange = mFinalPixelSize.width - mInitialPixelSize.width;
			setWidth(mInitialPixelSize.width + (totalWidthChange * (mResizeTimer / mResizeTime)));
			Ogre::Real totalHeightChange = mFinalPixelSize.height - mInitialPixelSize.height;
			setHeight(mInitialPixelSize.height + (totalHeightChange * (mResizeTimer / mResizeTime)));

			mResizeTimer += time;
			if(mResizeTimer >= mResizeTime)
				mResizeOverTime = false;
		}
	}

	void Widget::setDimensions(const Rect& pixelDimensions)
	{
		setPosition(pixelDimensions.x,pixelDimensions.y);
		setSize(pixelDimensions.width,pixelDimensions.height);
	}

	void Widget::setDisabledTexture(const Ogre::String& disabledTexture)
	{
		mDisabledTextureName = disabledTexture;
	}

	void Widget::setDraggingWidget(Widget* w)
	{
		mWidgetToDrag = w;
	}

	void Widget::setGainFocusOnClick(bool gainFocus)
	{
		mGainFocusOnClick = gainFocus;
	}

	void Widget::setGrabbed(bool grabbed)
	{
		mGrabbed = grabbed;
	}

	void Widget::setHeight(Ogre::Real pixelHeight)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		mSize.height = pixelHeight;

		WidgetEventArgs args(this);
		fireEvent(EVENT_SIZE_CHANGED,args);

		mQuad->setHeight(mSize.height);
	}

	void Widget::setHideWithParent(bool hide)
	{
		mHideWithParent = hide;
	}

	void Widget::setInheritClippingRect(bool inherit)
	{
		mInheritClippingRect = inherit;
	}

	void Widget::setMovingEnabled(bool enable)
	{
		mMovingEnabled = enable;
	}

	void Widget::setOffset(int offset)
	{
		mOffset = offset;
		mQuad->setOffset(mOffset);
	}

	void Widget::setPosition(const Ogre::Real& pixelX, const Ogre::Real& pixelY)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		mPosition.x = pixelX;
		mPosition.y = pixelY;

		WidgetEventArgs args(this);
		fireEvent(EVENT_POSITION_CHANGED,args);
		
		mQuad->setPosition(getScreenPosition() + getScrollOffset());
	}

	void Widget::setPosition(const Point& pixelPoint)
	{
		setPosition(pixelPoint.x,pixelPoint.y);
	}

	void Widget::setScreenPosition(const Ogre::Real& pixelX, const Ogre::Real& pixelY)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		Point parentScreenPosition = mParentWidget->getScreenPosition();

		setPosition((parentScreenPosition.x - pixelX),(parentScreenPosition.y - pixelY));
	}

	void Widget::setScreenXPosition(const Ogre::Real& pixelX)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		setXPosition(mParentWidget->getScreenPosition().x - pixelX);
	}

	void Widget::setScreenYPosition(const Ogre::Real& pixelY)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		setYPosition(mParentWidget->getScreenPosition().y - pixelY);
	}

	void Widget::setSize(const Ogre::Real& pixelWidth, const Ogre::Real& pixelHeight)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		mSize.width = pixelWidth;
		mSize.height = pixelHeight;

		WidgetEventArgs args(this);
		fireEvent(EVENT_SIZE_CHANGED,args);

		mQuad->setSize(mSize);
	}

	void Widget::setSize(const Size& pixelSize)
	{
		Widget::setSize(pixelSize.width,pixelSize.height);
	}

	void Widget::setShowWithParent(bool show)
	{
		mShowWithParent = show;
	}

	void Widget::setWidth(Ogre::Real pixelWidth)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		mSize.width = pixelWidth;

		WidgetEventArgs args(this);
		fireEvent(EVENT_SIZE_CHANGED,args);

		mQuad->setWidth(mSize.width);
	}

	void Widget::setXPosition(Ogre::Real pixelX)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		mPosition.x = pixelX;

		WidgetEventArgs args(this);
		fireEvent(EVENT_POSITION_CHANGED,args);
		
		mQuad->setXPosition(getScreenPosition().x + getScrollOffset().x);
	}

	void Widget::setYPosition(Ogre::Real pixelY)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		mPosition.y = pixelY;

		WidgetEventArgs args(this);
		fireEvent(EVENT_POSITION_CHANGED,args);
		
		mQuad->setYPosition(getScreenPosition().y + getScrollOffset().y);
	}

	void Widget::show()
	{
		mQuad->setVisible(true);

		// show children, except for Windows and lists of MenuList or ComboBox Widget.
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if((*it)->getShowWithParent())
				(*it)->show();
		}

		// Only fire event if we change visibility.  If we were already visible, don't fire.
		if(!mVisible)
		{
			WidgetEventArgs args(this);
			fireEvent(EVENT_SHOWN,args);
		}

		mVisible = true;
	}

	void Widget::setTexture(const Ogre::String& textureName, bool updateBaseTexture)
	{
		if(mTextureLocked)
			return;

		if(updateBaseTexture)
			setBaseTexture(textureName);

		mQuad->setTexture(textureName);

		if(Utility::textureExistsOnDisk(textureName))
		{
			Ogre::Image i;
			i.load(textureName,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			delete mWidgetImage;
			mWidgetImage = new Ogre::Image(i);
		}
	}

	void Widget::unlockTexture()
	{
		mTextureLocked = false;
	}
}
