#include "QuickGUIWidget.h"
#include "QuickGUIManager.h"
#include "QuickGUIPanel.h"
#include "QuickGUIWindow.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	Widget::Widget(const Ogre::String& instanceName, Type type, const Rect& dimensions, GuiMetricsMode pMode, GuiMetricsMode sMode, Ogre::String textureName, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		mInstanceName(instanceName),
		mWidgetType(type),
		mParentWidget(ParentWidget),
		mGUIManager(gm),
		mRelativeDimensions(dimensions),
		mAbsoluteDimensions(Rect(0,0,1,1)),
		mPixelDimensions(Rect(0,0,gm->getViewportWidth(),gm->getViewportHeight())),
		mVisible(true),
		mGrabbed(false),
		mMovingEnabled(true),
		mDraggingEnabled(false),
		mEnabled(true),
		mWidgetImage(NULL),
		mQuadContainer(container),
		mOffset(0),
		mClippingRect(Rect(0,0,1,1)),
		mTextureLocked(false),
		mGainFocusOnClick(true),
		mDragXOnly(false),
		mDragYOnly(false),
		mShowWithParent(true),
		mHideWithParent(true)
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

		setDimensions(dimensions,pMode,sMode);

		if(mParentWidget != NULL)
		{
			// set the correct offset
			mOffset = mParentWidget->getOffset() + 1;
			// add to parents child widget list
			mParentWidget->_addToChildList(this);
			// inheritted property
			mGainFocusOnClick = mParentWidget->getGainFocusOnClick();
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
		// 12 is the number of types of events: EVENT_GAIN_FOCUS, EVENT_CHARACTER_KEY, etc.
		while( index < 21 )
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

		// get highest offset of any of the widgets children
		int highestOffset = w->getOffset();
		std::vector<Widget*>::iterator it;
		std::vector<Widget*>* childList = w->getChildWidgetList();
		for( it = childList->begin(); it != childList->end(); ++it )
		{
			if( (*it)->getOffset() > highestOffset )
				highestOffset = (*it)->getOffset();
		}

		// Set offset higher by 3, to allow for an additional quad layer, and text layer.
		mOffset = highestOffset + 3;
		mQuad->setOffset(mOffset);
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

		WidgetEventArgs args(this);
		fireEvent(EVENT_DISABLED,args);

		mEnabled = false;

		setTexture(mDisabledTextureName,false);
	}

	void Widget::drag(const Ogre::Real& xVal, const Ogre::Real& yVal,GuiMetricsMode mode)
	{
		if(!mDraggingEnabled) 
			return;

		if(mWidgetToDrag != NULL) 
		{
			Point p = getPosition(mode);
			if(mDragXOnly)
				mWidgetToDrag->setScreenPosition(p.x + xVal,p.y,mode);
			else if(mDragYOnly)
				mWidgetToDrag->setScreenPosition(p.x,p.y + yVal,mode);
			else
				mWidgetToDrag->setScreenPosition(p.x + xVal,p.y + yVal,mode);
		}

		// fire onDragged Event.
		WidgetEventArgs args(this);
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

	Ogre::Real Widget::getHeight(GuiMetricsMode mode)
	{
		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			return mAbsoluteDimensions.height;
		case QGUI_GMM_PIXELS:
			return mPixelDimensions.height;
		case QGUI_GMM_RELATIVE:
		default:
			return mRelativeDimensions.height;
		}
	}

	bool Widget::getHideWithParent()
	{
		return mHideWithParent;
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

	bool Widget::getShowWithParent()
	{
		return mShowWithParent;
	}

	Rect Widget::getDimensions(GuiMetricsMode position, GuiMetricsMode size)
	{
		Rect retVal = Rect::ZERO;

		switch(position)
		{
		case QGUI_GMM_ABSOLUTE:
			retVal.x = mAbsoluteDimensions.x;
			retVal.y = mAbsoluteDimensions.y;
			break;
		case QGUI_GMM_RELATIVE:
			retVal.x = mRelativeDimensions.x;
			retVal.y = mRelativeDimensions.y;
			break;
		case QGUI_GMM_PIXELS:
			retVal.x = mPixelDimensions.x;
			retVal.y = mPixelDimensions.y;
			break;
		}

		switch(size)
		{
		case QGUI_GMM_ABSOLUTE:
			retVal.width = mAbsoluteDimensions.width;
			retVal.height = mAbsoluteDimensions.height;
			break;
		case QGUI_GMM_RELATIVE:
			retVal.width = mRelativeDimensions.width;
			retVal.height = mRelativeDimensions.height;
			break;
		case QGUI_GMM_PIXELS:
			retVal.width = mPixelDimensions.width;
			retVal.height = mPixelDimensions.height;
			break;
		}

		return retVal;
	}

	GUIManager* Widget::getGUIManager()
	{
		return mGUIManager;
	}

	Point Widget::getPosition(GuiMetricsMode mode)
	{
		Point retVal = Point::ZERO;

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			retVal.x = mAbsoluteDimensions.x;
			retVal.y = mAbsoluteDimensions.y;
			break;
		case QGUI_GMM_RELATIVE:
			retVal.x = mRelativeDimensions.x;
			retVal.y = mRelativeDimensions.y;
			break;
		case QGUI_GMM_PIXELS:
			retVal.x = mPixelDimensions.x;
			retVal.y = mPixelDimensions.y;
			break;
		}

		return retVal;
	}

	Size Widget::getSize(GuiMetricsMode mode)
	{
		Size retVal = Size::ZERO;

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			retVal.width = mAbsoluteDimensions.width;
			retVal.height = mAbsoluteDimensions.height;
			break;
		case QGUI_GMM_RELATIVE:
			retVal.width = mRelativeDimensions.width;
			retVal.height = mRelativeDimensions.height;
			break;
		case QGUI_GMM_PIXELS:
			retVal.width = mPixelDimensions.width;
			retVal.height = mPixelDimensions.height;
			break;
		}

		return retVal;
	}

	Widget* Widget::getTargetWidget(const Point& p)
	{
		if( !mVisible || !mEnabled || (mWidgetType == TYPE_SCROLL_PANE) ) 
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

			Widget* temp = (*it)->getTargetWidget(p);
			if( (temp != NULL) && (temp->getOffset() > widgetOffset) )
			{
				w = temp;
				widgetOffset = temp->getOffset();
			}
		}

		if( w != NULL ) return w;
		else if( isPointWithinBounds(p) ) return this;
		else return NULL;
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

	Ogre::Real Widget::getWidth(GuiMetricsMode mode)
	{
		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			return mAbsoluteDimensions.width;
		case QGUI_GMM_PIXELS:
			return mPixelDimensions.width;
		case QGUI_GMM_RELATIVE:
		default:
			return mRelativeDimensions.width;
		}
	}

	Ogre::Real Widget::getXPosition(GuiMetricsMode mode)
	{
		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			return mAbsoluteDimensions.x;
		case QGUI_GMM_PIXELS:
			return mPixelDimensions.x;
		case QGUI_GMM_RELATIVE:
		default:
			return mRelativeDimensions.x;
		}
	}

	Ogre::Real Widget::getYPosition(GuiMetricsMode mode)
	{
		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			return mAbsoluteDimensions.y;
		case QGUI_GMM_PIXELS:
			return mPixelDimensions.y;
		case QGUI_GMM_RELATIVE:
		default:
			return mRelativeDimensions.y;
		}
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

	bool Widget::isPointWithinBounds(const Point& p)
	{
		if(!mVisible) 
			return false;

		if( p.x < mPixelDimensions.x ||
			p.x > (mPixelDimensions.x + mPixelDimensions.width) )
			return false;

		if( p.y < mPixelDimensions.y ||
			p.y > (mPixelDimensions.y + mPixelDimensions.height) )
			return false;

		if(overTransparentPixel(p)) return false;

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

	void Widget::move(const Ogre::Real& xVal, const Ogre::Real& yVal, GuiMetricsMode mode)
	{
		if(!mMovingEnabled) 
			return;

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			mAbsoluteDimensions.x += xVal;
			mAbsoluteDimensions.y += yVal;
			setPosition(mAbsoluteDimensions.x,mAbsoluteDimensions.y,mode);
			break;
		case QGUI_GMM_PIXELS:
			mPixelDimensions.x += xVal;
			mPixelDimensions.y += yVal;
			setPosition(mPixelDimensions.x,mPixelDimensions.y,mode);
			break;
		case QGUI_GMM_RELATIVE:
			mRelativeDimensions.x += xVal;
			mRelativeDimensions.y += yVal;
			setPosition(mRelativeDimensions.x,mRelativeDimensions.y);
			break;
		}
	}

	void Widget::move(const Point& p, GuiMetricsMode mode)
	{
		move(p.x,p.y,mode);
	}

	void Widget::moveX(Ogre::Real xVal, GuiMetricsMode mode)
	{
		if(!mMovingEnabled) 
			return;

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			mAbsoluteDimensions.x += xVal;
			setXPosition(mAbsoluteDimensions.x,mode);
			break;
		case QGUI_GMM_PIXELS:
			mPixelDimensions.x += xVal;
			setXPosition(mPixelDimensions.x,mode);
			break;
		case QGUI_GMM_RELATIVE:
			mRelativeDimensions.x += xVal;
			setXPosition(mRelativeDimensions.x);
			break;
		}
	}

	void Widget::moveY(Ogre::Real yVal, GuiMetricsMode mode)
	{
		if(!mMovingEnabled) 
			return;

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			mAbsoluteDimensions.y += yVal;
			setYPosition(mAbsoluteDimensions.y,mode);
			break;
		case QGUI_GMM_PIXELS:
			mPixelDimensions.y += yVal;
			setYPosition(mPixelDimensions.y,mode);
			break;
		case QGUI_GMM_RELATIVE:
			mRelativeDimensions.y += yVal;
			setYPosition(mRelativeDimensions.y);
			break;
		}
	}

	bool Widget::fireEvent(Event e, const EventArgs& args)
	{
		if(!mEnabled || mUserEventHandlers[e].empty() ) 
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

	bool Widget::overTransparentPixel(const Point& mousePosition)
	{
		if( mWidgetImage == NULL ) 
			return false;

		Point pt(mousePosition.x - mPixelDimensions.x,mousePosition.y - mPixelDimensions.y);
		Ogre::Real relX = pt.x / mPixelDimensions.width;
		Ogre::Real relY = pt.y / mPixelDimensions.height;

		Ogre::ColourValue c = mWidgetImage->getColourAt(relX * mWidgetImage->getWidth(), relY * mWidgetImage->getHeight(),0);
		if( c.a <= 0.0 ) return true;

		return false;
	}

	void Widget::refresh()
	{
		setDimensions(getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE),QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE);

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

	void Widget::setBaseTexture(const Ogre::String& textureName)
	{
		// separate extension from name, if exists.
		_processFullTextureName(textureName);

		mDisabledTextureName = mTextureName + ".disabled" + mTextureExtension;
	}

	void Widget::setClippingRect(const Rect& r)
	{
		mQuad->setClippingRect(r);

		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			(*it)->setClippingRect(r);
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
	}

	void Widget::setDimensions(const Rect& dimensions, GuiMetricsMode positionMode, GuiMetricsMode sizeMode)
	{
		setPosition(dimensions.x,dimensions.y,positionMode);
		setSize(dimensions.width,dimensions.height,sizeMode);
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

	void Widget::setHeight(Ogre::Real height, GuiMetricsMode mode)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		Ogre::Real absParentHeight = mParentWidget->getHeight(QGUI_GMM_ABSOLUTE);
		Ogre::Real pixParentHeight = mParentWidget->getHeight(QGUI_GMM_PIXELS);

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			mRelativeDimensions.height = (height / absParentHeight);
			break;
		case QGUI_GMM_RELATIVE:
			mRelativeDimensions.height = height;
			break;
		case QGUI_GMM_PIXELS:
		default:
			mRelativeDimensions.height = (height / pixParentHeight);
			break;
		}

		mAbsoluteDimensions.height = mRelativeDimensions.height * absParentHeight;

		mPixelDimensions.height = mAbsoluteDimensions.height * mGUIManager->getViewportHeight();

		WidgetEventArgs args(this);
		fireEvent(EVENT_SIZE_CHANGED,args);

		mQuad->setHeight(mAbsoluteDimensions.height);
	}

	void Widget::setHideWithParent(bool hide)
	{
		mHideWithParent = hide;
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

	void Widget::setPosition(const Ogre::Real& xVal, const Ogre::Real& yVal, GuiMetricsMode mode)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		Point absParentPosition = mParentWidget->getPosition(QGUI_GMM_ABSOLUTE);
		Size absParentSize = mParentWidget->getSize(QGUI_GMM_ABSOLUTE);
		Point pixParentPosition = mParentWidget->getPosition(QGUI_GMM_PIXELS);
		Size pixParentSize = mParentWidget->getSize(QGUI_GMM_PIXELS);

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			mRelativeDimensions.x = (xVal / absParentSize.width);
			mRelativeDimensions.y = (yVal / absParentSize.height);
			break;
		case QGUI_GMM_RELATIVE:
			mRelativeDimensions.x = xVal;
			mRelativeDimensions.y = yVal;
			break;
		case QGUI_GMM_PIXELS:
		default:
			mRelativeDimensions.x = (xVal / pixParentSize.width);
			mRelativeDimensions.y = (yVal / pixParentSize.height);
			break;
		}

		mAbsoluteDimensions.x = absParentPosition.x + (mRelativeDimensions.x * absParentSize.width);
		mAbsoluteDimensions.y = absParentPosition.y + (mRelativeDimensions.y * absParentSize.height);

		mPixelDimensions.x = mAbsoluteDimensions.x * mGUIManager->getViewportWidth();
		mPixelDimensions.y = mAbsoluteDimensions.y * mGUIManager->getViewportHeight();

		WidgetEventArgs args(this);
		fireEvent(EVENT_POSITION_CHANGED,args);
		
		mQuad->setPosition(Point(mAbsoluteDimensions.x,mAbsoluteDimensions.y));
	}

	void Widget::setPosition(const Point& p, GuiMetricsMode mode)
	{
		Widget::setPosition(p.x,p.y,mode);
	}

	void Widget::setScreenPosition(const Ogre::Real& xVal, const Ogre::Real& yVal, GuiMetricsMode mode)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		Point absParentPosition = mParentWidget->getPosition(QGUI_GMM_ABSOLUTE);
		Point pixParentPosition = mParentWidget->getPosition(QGUI_GMM_PIXELS);

		switch(mode)
		{
		case QGUI_GMM_RELATIVE:
		case QGUI_GMM_ABSOLUTE:
			setPosition((xVal - absParentPosition.x),(yVal - absParentPosition.y),QGUI_GMM_ABSOLUTE);
			break;
		case QGUI_GMM_PIXELS:
			setPosition((xVal - pixParentPosition.x),(yVal - pixParentPosition.y),mode);
			break;
		}
	}

	void Widget::setSize(const Ogre::Real& width, const Ogre::Real& height, GuiMetricsMode mode)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		Size absParentSize = mParentWidget->getSize(QGUI_GMM_ABSOLUTE);
		Size pixParentSize = mParentWidget->getSize(QGUI_GMM_PIXELS);

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			mRelativeDimensions.width = (width / absParentSize.width);
			mRelativeDimensions.height = (height / absParentSize.height);
			break;
		case QGUI_GMM_RELATIVE:
			mRelativeDimensions.width = width;
			mRelativeDimensions.height = height;
			break;
		case QGUI_GMM_PIXELS:
		default:
			mRelativeDimensions.width = (width / pixParentSize.width);
			mRelativeDimensions.height = (height / pixParentSize.height);
			break;
		}

		mAbsoluteDimensions.width = mRelativeDimensions.width * absParentSize.width;
		mAbsoluteDimensions.height = mRelativeDimensions.height * absParentSize.height;

		mPixelDimensions.width = mAbsoluteDimensions.width * mGUIManager->getViewportWidth();
		mPixelDimensions.height = mAbsoluteDimensions.height * mGUIManager->getViewportHeight();

		WidgetEventArgs args(this);
		fireEvent(EVENT_SIZE_CHANGED,args);

		mQuad->setSize(Size(mAbsoluteDimensions.width,mAbsoluteDimensions.height));
	}

	void Widget::setSize(const Size& s, GuiMetricsMode mode)
	{
		Widget::setSize(s.width,s.height,mode);
	}

	void Widget::setShowWithParent(bool show)
	{
		mShowWithParent = show;
	}

	void Widget::setWidth(Ogre::Real width, GuiMetricsMode mode)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		Ogre::Real absParentWidth = mParentWidget->getWidth(QGUI_GMM_ABSOLUTE);
		Ogre::Real pixParentWidth = mParentWidget->getWidth(QGUI_GMM_PIXELS);

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			mRelativeDimensions.width = (width / absParentWidth);
			break;
		case QGUI_GMM_RELATIVE:
			mRelativeDimensions.width = width;
			break;
		case QGUI_GMM_PIXELS:
		default:
			mRelativeDimensions.width = (width / pixParentWidth);
			break;
		}

		mAbsoluteDimensions.width = mRelativeDimensions.width * absParentWidth;

		mPixelDimensions.width = mAbsoluteDimensions.width * mGUIManager->getViewportWidth();

		WidgetEventArgs args(this);
		fireEvent(EVENT_SIZE_CHANGED,args);

		mQuad->setWidth(mAbsoluteDimensions.width);
	}

	void Widget::setXPosition(Ogre::Real x, GuiMetricsMode mode)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		Ogre::Real absParentX = mParentWidget->getXPosition(QGUI_GMM_ABSOLUTE);
		Ogre::Real absParentWidth = mParentWidget->getWidth(QGUI_GMM_ABSOLUTE);
		Ogre::Real pixParentX = mParentWidget->getYPosition(QGUI_GMM_PIXELS);
		Ogre::Real pixParentWidth = mParentWidget->getWidth(QGUI_GMM_PIXELS);

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			mRelativeDimensions.x = (x / absParentWidth);
			break;
		case QGUI_GMM_RELATIVE:
			mRelativeDimensions.x = x;
			break;
		case QGUI_GMM_PIXELS:
		default:
			mRelativeDimensions.x = (x / pixParentWidth);
			break;
		}

		mAbsoluteDimensions.x = absParentX + (mRelativeDimensions.x * absParentWidth);

		mPixelDimensions.x = mAbsoluteDimensions.x * mGUIManager->getViewportWidth();

		WidgetEventArgs args(this);
		fireEvent(EVENT_POSITION_CHANGED,args);
		
		mQuad->setXPosition(mAbsoluteDimensions.x);
	}

	void Widget::setYPosition(Ogre::Real y, GuiMetricsMode mode)
	{
		if((mWidgetType == Widget::TYPE_SHEET) || (mParentWidget == NULL))
			return;

		Ogre::Real absParentY = mParentWidget->getYPosition(QGUI_GMM_ABSOLUTE);
		Ogre::Real absParentHeight = mParentWidget->getHeight(QGUI_GMM_ABSOLUTE);
		Ogre::Real pixParentY = mParentWidget->getYPosition(QGUI_GMM_PIXELS);
		Ogre::Real pixParentHeight = mParentWidget->getHeight(QGUI_GMM_PIXELS);

		switch(mode)
		{
		case QGUI_GMM_ABSOLUTE:
			mRelativeDimensions.y = (y / absParentHeight);
			break;
		case QGUI_GMM_RELATIVE:
			mRelativeDimensions.y = y;
			break;
		case QGUI_GMM_PIXELS:
		default:
			mRelativeDimensions.y = (y / pixParentHeight);
			break;
		}

		mAbsoluteDimensions.y = absParentY + (mRelativeDimensions.y * absParentHeight);

		mPixelDimensions.y = mAbsoluteDimensions.y * mGUIManager->getViewportHeight();

		WidgetEventArgs args(this);
		fireEvent(EVENT_POSITION_CHANGED,args);
		
		mQuad->setYPosition(mAbsoluteDimensions.y);
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
