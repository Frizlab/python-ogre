#include "QuickGUIWidget.h"
#include "QuickGUIWidgetDescFactoryManager.h"
#include "QuickGUIWindow.h"
#include "QuickGUIRoot.h"
#include "QuickGUISheet.h"
#include "QuickGUISerialWriter.h"

namespace QuickGUI
{
	int Widget::mWidgetCounter = 0;

	WidgetDesc::WidgetDesc()
	{
		brushFilterMode = BRUSHFILTER_LINEAR;
		consumeKeyboardEvents = false;
		enabled = true;
		dimensions = Rect::ZERO;
		disabledSkinType = "";
		dragable = false;
		guiManager = NULL;
		horizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
		hoverTime = Root::getSingleton().getDefaultHoverTime();
		maxSize = Size::ZERO;
		minSize = Size(0,0);
		name = "";
		resizable = false;
		scrollable = true;
		sheet = NULL;
		transparencyPicking = true;
		verticalAnchor = ANCHOR_VERTICAL_TOP;
		visible = true;
		skinTypeName = "default";
	}

	void WidgetDesc::serialize(SerialBase* b)
	{
		b->IO("BrushFilterMode",&brushFilterMode);
		b->IO("ConsumeKeyboardEvents",&consumeKeyboardEvents);
		b->IO("Enabled",&enabled);
		b->IO("Dimensions",&dimensions);
		b->IO("DisabledSkinType",&disabledSkinType);
		b->IO("Dragable",&dragable);
		b->IO("HorizontalAnchor",&horizontalAnchor);
		b->IO("HoverTime",&hoverTime);
		b->IO("MaxSize",&maxSize);
		b->IO("MinSize",&minSize);
		b->IO("Resizable",&resizable);
		b->IO("Scrollable",&scrollable);
		b->IO("TransparencyPicking",&transparencyPicking);
		b->IO("VerticalAnchor",&verticalAnchor);
		b->IO("Visible",&visible);
		b->IO("SkinType",&skinTypeName);
	}

	Widget::Widget(const Ogre::String& name) :
		Serializable(),
		mBeingDragged(false),
		mComponent(false),
		mGrabbed(false),
		mInitialized(false),
		mParentWidget(0),
		mSkinElementName(""),
		mSkinType(0),
		mWidgetDesc(NULL),
		mWindow(0)
	{
		mName = name;
	}

	Widget::~Widget()
	{
		delete mWidgetDesc;

		// Clean up all user defined event handlers.
		for(int index = 0; index < WIDGET_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mWidgetEventHandlers[index].begin(); it != mWidgetEventHandlers[index].end(); ++it)
				delete (*it);
		}
	}

	void Widget::_createDescObject(const Ogre::String& className)
	{
		if(mInitialized)
			throw Exception(Exception::ERR_INTERNAL_ERROR,"Widget is already initialized!","Widget::_createDescObject");

		mWidgetDesc = WidgetDescFactoryManager::getSingleton().createWidgetDesc(className);
		// Assign name, stored from widget creation
		mWidgetDesc->name = mName;

		if(mWidgetDesc->getWidgetClass() != getClass())
			throw Exception(Exception::ERR_INVALID_DESC,"Desc of class \"" + className + "\" is not meant for Widget of class \"" + getClass() + "\"!","Widget::_initialize");
	}

	void Widget::_initialize(WidgetDesc* d)
	{
		if(mInitialized)
			throw Exception(Exception::ERR_INTERNAL_ERROR,"Widget is already initialized!","Widget::_initialize");

		if(mWidgetDesc == NULL)
			throw Exception(Exception::ERR_INVALID_STATE,"Desc object has not been created!  Did you forget to call _createDescObject in the factory method?","Widget::_initialize");

		mInitialized = true;

		setConsumeKeyboardEvents(d->consumeKeyboardEvents);
		setEnabled(d->enabled);
		setDimensions(d->dimensions);
		setDragable(d->dragable);
		setHorizontalAnchor(d->horizontalAnchor);
		setHoverTime(d->hoverTime);
		setMaxSize(d->maxSize);
		setMinSize(d->minSize);
		setName(d->name);
		setResizable(d->resizable);
		setScrollable(d->scrollable);
		setTransparencyPicking(d->transparencyPicking);
		setVerticalAnchor(d->verticalAnchor);
		setVisible(d->visible);
	}

	void Widget::_setGUIManager(GUIManager* gm)
	{
		mWidgetDesc->guiManager = gm;
	}

	void Widget::_setSheet(Sheet* sheet)
	{
		if((sheet != NULL) && (sheet->findWidget(mWidgetDesc->name) != NULL) && (sheet->findWidget(mWidgetDesc->name) != this))
			throw Exception(Exception::ERR_DUPLICATE_ITEM,"A widget with name \"" + mWidgetDesc->name + "\" already exists in Sheet \"" + sheet->getName() + "\"!","Widget::_setSheet");

		mWidgetDesc->sheet = sheet;
	}

	void Widget::addWidgetEventHandler(WidgetEvent EVENT, EventHandlerSlot* function)
	{
		mWidgetEventHandlers[EVENT].push_back(function);
	}

	Widget* Widget::create(const Ogre::String& className, WidgetDesc& d)
	{
		Ogre::String temp = d.name;

		if(d.name == "")
		{
			d.name = className + Ogre::StringConverter::toString(mWidgetCounter);
			++mWidgetCounter;
		}

		Ogre::String cName = d.getWidgetClass();

		if(className != d.getWidgetClass())
			throw Exception(Exception::ERR_INVALID_DESC,"Desc object for Widget of class \"" + d.getWidgetClass() + "\" does not match Widget of class \"" + className + "\"!","GUIManager::createWidget");

		Widget* newWidget = WidgetFactoryManager::getSingleton().createWidget(className,d.name);
		newWidget->_initialize(&d);

		// Restore the name, in case it was changed.  This Desc could be used multiple times, so this is important!
		d.name = temp;

		return newWidget;
	}

	void Widget::drag(int xOffset, int yOffset)
	{
		mBeingDragged = true;
		mWidgetDesc->dimensions.position.translate(Point(xOffset,yOffset));

		updateTexturePosition();

		WidgetEventArgs wargs(this);
		fireWidgetEvent(WIDGET_EVENT_DRAGGED,wargs);
	}

	void Widget::draw()
	{
		// check visibility
		if( !mWidgetDesc->visible )
			return;

		Brush* brush = Brush::getSingletonPtr();

		// check clip region
		Rect clipRegion = brush->getClipRegion();
		if ( clipRegion.getIntersection(Rect(mTexturePosition,mWidgetDesc->dimensions.size)) == Rect::ZERO )
			return;

		// onDraw
		onDraw();
	}

	Widget* Widget::findFirstWidgetOfClass(const Ogre::String& className)
	{
		if(getClass() == className)
			return this;
		return NULL;
	}

	Widget* Widget::findWidget(const Ogre::String& name)
	{
		if(mWidgetDesc->name == name)
			return this;
		return NULL;
	}

	Widget* Widget::findWidgetAtPoint(const Point& p, bool ignoreDisabled)
	{
		// If we are not visible, return NULL
		if(!mWidgetDesc->visible)
			return NULL;

		// If we ignore disabled and this widget is enabled, return NULL
		if(ignoreDisabled && !mWidgetDesc->enabled)
			return NULL;

		// See if point is within widget bounds
		if(mWidgetDesc->visible && Rect(mTexturePosition,mWidgetDesc->dimensions.size).isPointWithinBounds(p))
		{
			// Take transparency picking into account
			if(mWidgetDesc->transparencyPicking)
			{
				// Get relative position
				Point relPos = p - mTexturePosition;
				// Get percentage of position relative to widget dimensions
				relPos.x = relPos.x / mWidgetDesc->dimensions.size.width;
				relPos.y = relPos.y / mWidgetDesc->dimensions.size.height;
				// Get pixel position of texture
				Point pixelPos;
				SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
				pixelPos.x = relPos.x * (se->getWidth() - 1);
				pixelPos.y = relPos.y * (se->getHeight() - 1);
				
				if(mSkinType->getSkinElement(mSkinElementName)->transparentPixel(pixelPos))
					return NULL;
			}
			return this;
		}
		
		return NULL;
	}

	bool Widget::fireWidgetEvent(WidgetEvent EVENT, EventArgs& args)
	{
		// If there are no User defined event handlers we are done.
		if(mWidgetEventHandlers[EVENT].empty())
			return false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mWidgetEventHandlers[EVENT]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		return true;
	}

	bool Widget::getBeingDragged()
	{
		return mBeingDragged;
	}

	BorderSide Widget::getBorderSide(Point p)
	{
		if(!overBorderSide(p))
			return BORDER_NONE;

		// Convert point to relative (to widget) coordinates
		p -= mTexturePosition;

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
		if(p.x < se->getBorderThickness(BORDER_LEFT))
		{
			if(p.y < se->getBorderThickness(BORDER_TOP))
				return BORDER_TOP_LEFT;
			if(p.y > (mWidgetDesc->dimensions.size.height - se->getBorderThickness(BORDER_BOTTOM)))
				return BORDER_BOTTOM_LEFT;
			return BORDER_LEFT;
		}

		if(p.x > (mWidgetDesc->dimensions.size.width - se->getBorderThickness(BORDER_RIGHT)))
		{
			if(p.y < se->getBorderThickness(BORDER_TOP))
				return BORDER_TOP_RIGHT;
			if(p.y > (mWidgetDesc->dimensions.size.height - se->getBorderThickness(BORDER_BOTTOM)))
				return BORDER_BOTTOM_RIGHT;
			return BORDER_RIGHT;
		}

		if(p.y < se->getBorderThickness(BORDER_TOP))
			return BORDER_TOP;

		return BORDER_BOTTOM;
	}

	Rect Widget::getClientDimensions()
	{
		return mClientDimensions;
	}

	bool Widget::getConsumeKeyboardEvents()
	{
		return mWidgetDesc->consumeKeyboardEvents;
	}

	Rect Widget::getDimensions()
	{
		return mWidgetDesc->dimensions;
	}

	Ogre::String Widget::getDisabledSkinType()
	{
		return mWidgetDesc->disabledSkinType;
	}

	bool Widget::getDragable()
	{
		return mWidgetDesc->dragable;
	}

	bool Widget::getEnabled()
	{
		return mWidgetDesc->enabled;
	}

	bool Widget::getGrabbed()
	{
		return mGrabbed;
	}

	GUIManager* Widget::getGUIManager()
	{
		return mWidgetDesc->guiManager;
	}

	float Widget::getHeight()
	{
		return mWidgetDesc->dimensions.size.height;
	}

	HorizontalAnchor Widget::getHorizontalAnchor()
	{
		return mWidgetDesc->horizontalAnchor;
	}

	float Widget::getHoverTime()
	{
		return mWidgetDesc->hoverTime;
	}

	Size Widget::getMaxSize()
	{
		return mWidgetDesc->maxSize;
	}

	Size Widget::getMinSize()
	{
		return mWidgetDesc->minSize;
	}

	Ogre::String Widget::getName()
	{
		return mWidgetDesc->name;
	}

	Widget* Widget::getParentWidget()
	{
		return mParentWidget;
	}

	Point Widget::getPosition()
	{
		return mWidgetDesc->dimensions.position;
	}

	bool Widget::getResizable()
	{
		return mWidgetDesc->resizable;
	}

	Point Widget::getScreenPosition()
	{
		if(mParentWidget == NULL)
			return mWidgetDesc->dimensions.position + mScrollOffset;

		return mParentWidget->getScreenPosition() + mParentWidget->getClientDimensions().position + (mWidgetDesc->dimensions.position + mScrollOffset);
	}

	Point Widget::getScroll()
	{
		return mScrollOffset;
	}

	bool Widget::getScrollable()
	{
		return mWidgetDesc->scrollable;
	}

	Widget* Widget::getScrollableContainerWidget()
	{
		if(isContainerWidget() && dynamic_cast<ContainerWidget*>(this)->supportsScrolling())
			return this;

		Widget* w = mParentWidget;
		while(w != NULL)
		{
			if(w->isContainerWidget() && dynamic_cast<ContainerWidget*>(w)->supportsScrolling())
				return w;

			w = w->getParentWidget();
		}

		// NULL
		return w;
	}

	Sheet* Widget::getSheet()
	{
		return mWidgetDesc->sheet;
	}

	Size Widget::getSize()
	{
		return mWidgetDesc->dimensions.size;
	}

	Point Widget::getTexturePosition()
	{
		return mTexturePosition;
	}

	bool Widget::getTransparencyPicking()
	{
		return mWidgetDesc->transparencyPicking;
	}

	VerticalAnchor Widget::getVerticalAnchor()
	{
		return mWidgetDesc->verticalAnchor;
	}

	bool Widget::getVisible()
	{
		return mWidgetDesc->visible;
	}

	Ogre::String Widget::getSkinTypeName()
	{
		return mWidgetDesc->skinTypeName;
	}

	float Widget::getWidth()
	{
		return mWidgetDesc->dimensions.size.width;
	}

	bool Widget::hasEventHandler(WidgetEvent EVENT, void* obj)
	{
		for(std::vector<EventHandlerSlot*>::iterator it = mWidgetEventHandlers[EVENT].begin(); it != mWidgetEventHandlers[EVENT].end(); ++it)
		{
			if((*it)->getClass() == obj)
				return true;
		}

		return false;
	}

	bool Widget::isComponentWidget()
	{
		return false;
	}

	bool Widget::isContainerWidget()
	{
		return false;
	}

	bool Widget::isMenuItem()
	{
		return false;
	}

	bool Widget::isToolBarItem()
	{
		return false;
	}

	bool Widget::overBorderSide(Point p)
	{
		// Convert point to relative (to widget) coordinates
		p -= mTexturePosition;

		if((p.x < 0) || (p.x > mWidgetDesc->dimensions.size.width))
			return false;

		if((p.y < 0) || (p.y > mWidgetDesc->dimensions.size.height))
			return false;

		if(mSkinType == NULL)
			return false;

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
		if((p.x < se->getBorderThickness(BORDER_LEFT)) || (p.x > (mWidgetDesc->dimensions.size.width - se->getBorderThickness(BORDER_RIGHT))))
			return true;

		if((p.y < se->getBorderThickness(BORDER_TOP)) || (p.y > (mWidgetDesc->dimensions.size.height - se->getBorderThickness(BORDER_BOTTOM))))
			return true;

		return false;
	}

	void Widget::redraw()
	{
		if(mWindow != NULL)
			mWindow->redraw();
	}

	void Widget::removeEventHandler(WidgetEvent EVENT, void* obj)
	{
		for(std::vector<EventHandlerSlot*>::iterator it = mWidgetEventHandlers[EVENT].begin(); it != mWidgetEventHandlers[EVENT].end(); ++it)
		{
			if((*it)->getClass() == obj)
			{
				EventHandlerSlot* ehs = (*it);
				mWidgetEventHandlers[EVENT].erase(it);
				delete ehs;
				return;
			}
		}

		throw Exception(Exception::ERR_ITEM_NOT_FOUND,"No \"" + StringConverter::toString(EVENT) + "\" event handlers found that were registered by the given object!","Widget::removeEventHandler");
	}

	void Widget::resize(BorderSide s, float xOffset, float yOffset)
	{
		bool changePosition = false;
		Point newPosition = mWidgetDesc->dimensions.position;
		
		// Record size before resize
		Size previousSize = mWidgetDesc->dimensions.size;

		switch(s)
		{
		case BORDER_BOTTOM:
			setHeight(mWidgetDesc->dimensions.size.height + yOffset);
			break;
		case BORDER_TOP_LEFT:
			{
				Size newSize(mWidgetDesc->dimensions.size);
				newSize.width -= xOffset;
				newSize.height -= yOffset;
				setSize(newSize);

				changePosition = true;
				newPosition.x += previousSize.width - mWidgetDesc->dimensions.size.width;
				newPosition.y += previousSize.height - mWidgetDesc->dimensions.size.height;
			}
			break;
		case BORDER_BOTTOM_LEFT: 
			{
				Size newSize(mWidgetDesc->dimensions.size);
				newSize.width -= xOffset;
				newSize.height += yOffset;
				setSize(newSize);

				changePosition = true;
				newPosition.x += previousSize.width - mWidgetDesc->dimensions.size.width;
			}
			break;
		case BORDER_TOP_RIGHT:
			{				
				Size newSize(mWidgetDesc->dimensions.size);
				newSize.width += xOffset;
				newSize.height -= yOffset;
				setSize(newSize);

				changePosition = true;
				newPosition.y += previousSize.height - mWidgetDesc->dimensions.size.height;
			}
			break;
		case BORDER_BOTTOM_RIGHT:
			{
				Size newSize(mWidgetDesc->dimensions.size);
				newSize.width += xOffset;
				newSize.height += yOffset;
				setSize(newSize);
			}
			break;
		case BORDER_LEFT: 
			{
				setWidth(mWidgetDesc->dimensions.size.width - xOffset);

				changePosition = true;
				newPosition.x += previousSize.width - mWidgetDesc->dimensions.size.width;
			}
			break;
		case BORDER_RIGHT:
			setWidth(mWidgetDesc->dimensions.size.width + xOffset);
			break;
		case BORDER_TOP:
			{
				setHeight(mWidgetDesc->dimensions.size.height - yOffset);

				changePosition = true;
				newPosition.y += previousSize.height - mWidgetDesc->dimensions.size.height;
			}
			break;
		}

		// Top and/or Left resizes require repositioning of the widget.
		if(changePosition)
			setPosition(newPosition);
	}

	void Widget::serialize(SerialBase* b)
	{
		b->begin(getClass(),getName());

		mWidgetDesc->serialize(b);

		if(b->isSerialReader())
		{
			// Apply Desc properties
			_initialize(mWidgetDesc);
		}

		b->end();
	}

	void Widget::setConsumeKeyboardEvents(bool consume)
	{
		mWidgetDesc->consumeKeyboardEvents = consume;
	}

	void Widget::setDimensions(const Rect& r)
	{
		setSize(r.size);
		setPosition(r.position);
	}

	void Widget::setDisabledSkinType(const Ogre::String& SkinTypeName)
	{
		if((SkinTypeName != "") && (!SkinTypeManager::getSingleton().hasSkinType(getClass(),SkinTypeName)))
			throw Exception(Exception::ERR_ITEM_NOT_FOUND,"Skin Type \"" + SkinTypeName + "\" does not exist for class \"" + getClass() + "\"!","Widget::setDisabledSkinType");

		mWidgetDesc->disabledSkinType = SkinTypeName;

		if(!mWidgetDesc->enabled)
			redraw();
	}

	void Widget::setDragable(bool dragable)
	{
		mWidgetDesc->dragable = dragable;
	}

	void Widget::setEnabled(bool enabled)
	{
		if(mWidgetDesc->enabled == enabled)
			return;

		mWidgetDesc->enabled = enabled;

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_ENABLED_CHANGED,args);

		if(!mWidgetDesc->enabled)
			redraw();
	}

	void Widget::setGrabbed(bool grabbed)
	{
		mGrabbed = grabbed;
	}

	void Widget::setHeight(float pixelHeight)
	{
		// Enforce max height
		if((mWidgetDesc->maxSize.height > 0) && (pixelHeight > mWidgetDesc->maxSize.height))
			pixelHeight = mWidgetDesc->maxSize.height;
		// Enforce min height
		else if((mWidgetDesc->minSize.height > 0) && (pixelHeight < mWidgetDesc->minSize.height))
			pixelHeight = mWidgetDesc->minSize.height;

		mWidgetDesc->dimensions.size.height = pixelHeight;

		updateClientDimensions();

		redraw();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SIZE_CHANGED,args);
	}

	void Widget::setHorizontalAnchor(HorizontalAnchor a)
	{
		mWidgetDesc->horizontalAnchor = a;
	}

	void Widget::setHoverTime(float seconds)
	{
		mWidgetDesc->hoverTime = seconds;
	}

	void Widget::setMaxSize(const Size& s)
	{
		mWidgetDesc->maxSize = s;

		setSize(mWidgetDesc->dimensions.size);
	}

	void Widget::setMinSize(const Size& s)
	{
		mWidgetDesc->minSize = s;

		setSize(mWidgetDesc->dimensions.size);
	}

	void Widget::setName(const Ogre::String& name)
	{
		mWidgetDesc->name = name;
	}

	void Widget::setParent(Widget* parent)
	{
		// Set new parent and window

		mParentWidget = parent;

		if ( mParentWidget )
		{
			mWindow = mParentWidget->mWindow;
			mWidgetDesc->guiManager = mParentWidget->getGUIManager();
			mWidgetDesc->sheet = mParentWidget->getSheet();
		}
		else
		{
			mWindow = 0;
			mWidgetDesc->guiManager = 0;
			mWidgetDesc->sheet = 0;
		}

		// Update screen rectangle

		updateTexturePosition();
	}

	void Widget::setPosition(const Point& position)
	{
		mWidgetDesc->dimensions.position = position;		

		// Update screen rectangle

		updateTexturePosition();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_POSITION_CHANGED,args);
	}

	void Widget::setResizable(bool resizable)
	{
		mWidgetDesc->resizable = resizable;
	}

	void Widget::setScroll(unsigned int scrollX, unsigned int scrollY)
	{
		mScrollOffset.x = scrollX;
		mScrollOffset.y = scrollY;

		// Update screen rectangle

		updateTexturePosition();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SCROLL_CHANGED,args);
	}

	void Widget::setScrollX(unsigned int scrollX)
	{
		mScrollOffset.x = scrollX;

		// Update screen rectangle

		updateTexturePosition();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SCROLL_CHANGED,args);
	}

	void Widget::setScrollY(unsigned int scrollY)
	{
		mScrollOffset.y = scrollY;

		// Update screen rectangle

		updateTexturePosition();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SCROLL_CHANGED,args);
	}

	void Widget::setScrollable(bool scrollable)
	{
		mWidgetDesc->scrollable = scrollable;
	}

	void Widget::setSize(Size size)
	{
		// Enforce max width
		if((mWidgetDesc->maxSize.width > 0) && (size.width > mWidgetDesc->maxSize.width))
			size.width = mWidgetDesc->maxSize.width;
		// Enforce min width
		else if((mWidgetDesc->minSize.width > 0) && (size.width < mWidgetDesc->minSize.width))
			size.width = mWidgetDesc->minSize.width;

		// Enforce min height
		if((mWidgetDesc->maxSize.height > 0) && (size.height > mWidgetDesc->maxSize.height))
			size.height = mWidgetDesc->maxSize.height;
		// Enforce min width
		else if((mWidgetDesc->minSize.height > 0) && (size.height < mWidgetDesc->minSize.height))
			size.height = mWidgetDesc->minSize.height;

		// Update size
		mWidgetDesc->dimensions.size = size;

		updateClientDimensions();

		redraw();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SIZE_CHANGED,args);
	}

	void Widget::setSkinType(const Ogre::String type)
	{
		if(!SkinTypeManager::getSingleton().hasSkinType(getClass(),type))
			throw Exception(Exception::ERR_SKINNING,"SkinType \"" + type + "\" does not exist!","Widget::setSkinType");

		mSkinType = SkinTypeManager::getSingleton().getSkinType(getClass(),type);

		mWidgetDesc->skinTypeName = type;

		updateClientDimensions();

		redraw();

		WidgetEventArgs wea(this);
		fireWidgetEvent(WIDGET_EVENT_SKIN_CHANGED,wea);
	}

	void Widget::setTransparencyPicking(bool transparencyPicking)
	{
		mWidgetDesc->transparencyPicking = transparencyPicking;
	}

	void Widget::setVerticalAnchor(VerticalAnchor a)
	{
		mWidgetDesc->verticalAnchor = a;
	}

	void Widget::setVisible(bool visible)
	{
		if(mWidgetDesc->visible == visible)
			return;

		mWidgetDesc->visible = visible;

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_VISIBLE_CHANGED,args);

		redraw();
	}

	void Widget::setWidth(float pixelWidth)
	{
		// Enforce max width
		if((mWidgetDesc->maxSize.width > 0) && (pixelWidth > mWidgetDesc->maxSize.width))
			pixelWidth = mWidgetDesc->maxSize.width;
		// Enforce min width
		else if((mWidgetDesc->minSize.width > 0) && (pixelWidth < mWidgetDesc->minSize.width))
			pixelWidth = mWidgetDesc->minSize.width;

		mWidgetDesc->dimensions.size.width = pixelWidth;

		updateClientDimensions();

		redraw();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SIZE_CHANGED,args);
	}

	void Widget::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->dimensions.size;

		if(mSkinType != NULL)
		{
			SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
			mClientDimensions.position.x = se->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = se->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->dimensions.size.height - (se->getBorderThickness(BORDER_TOP) + se->getBorderThickness(BORDER_BOTTOM));
		}

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);
	}

	void Widget::updateTexturePosition()
	{
		// Set the Texture position to ZERO by default.

		mTexturePosition = Point::ZERO;

		// Set Client position to zero, and size to widget size by default.

		//mClientDimensions.position = Point::ZERO;
		//mClientDimensions.size = mWidgetDesc->dimensions.size;

		// If there is a parent widget, translate by the parent's TexturePosition

		if (mParentWidget)
		{
			mTexturePosition.translate(mParentWidget->getTexturePosition());
			
			// Components aren't restricted to Parent's Client dimensions
			if(!mComponent)
				mTexturePosition.translate(mParentWidget->getClientDimensions().position);
		}			

		// Translate by the Widget's position 
		mTexturePosition.translate(mWidgetDesc->dimensions.position);

		if (mWidgetDesc->scrollable)
			mTexturePosition.translate((mScrollOffset) * -1.0);

		// Force redraw

		redraw();
	}
}
