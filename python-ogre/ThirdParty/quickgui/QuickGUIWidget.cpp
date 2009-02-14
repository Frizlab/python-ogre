#include "QuickGUIWidget.h"
#include "QuickGUIWindow.h"
#include "QuickGUIRoot.h"
#include "QuickGUISheet.h"
#include "QuickGUISerialWriter.h"

namespace QuickGUI
{
	int Widget::mWidgetCounter = 0;

	WidgetDesc::WidgetDesc(const Ogre::String& id) :
		Desc()
	{
		mID = id;

		resetToDefault();
	}

	void WidgetDesc::resetToDefault()
	{
		widget_brushFilterMode = BRUSHFILTER_LINEAR;
		widget_consumeKeyboardEvents = false;
		widget_contextMenuName = "";
		widget_enabled = true;
		widget_dimensions = Rect::ZERO;
		widget_disabledSkinType = "";
		widget_dragable = false;
		guiManager = NULL;
		widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
		widget_hoverTime = Root::getSingleton().getDefaultHoverTime();
		widget_inheritOpacity = true;
		widget_maxSize = Size::ZERO;
		widget_minSize = Size(5,5);
		widget_name = "";
		widget_positionRelativeToParentClientDimensions = true;
		widget_relativeOpacity = 1.0;
		widget_resizeFromBottom = false;
		widget_resizeFromLeft = false;
		widget_resizeFromRight = false;
		widget_resizeFromTop = false;
		widget_scrollable = true;
		widget_serialize = true;
		sheet = NULL;
		widget_transparencyPicking = true;
		widget_verticalAnchor = ANCHOR_VERTICAL_TOP;
		widget_visible = true;
		widget_skinTypeName = "default";
	}

	void WidgetDesc::serialize(SerialBase* b)
	{
		b->IO("BrushFilterMode",&widget_brushFilterMode);
		b->IO("ConsumeKeyboardEvents",&widget_consumeKeyboardEvents);
		b->IO("ContextMenuName",&widget_contextMenuName);
		b->IO("Enabled",&widget_enabled);
		b->IO("Dimensions",&widget_dimensions);
		b->IO("DisabledSkinType",&widget_disabledSkinType);
		b->IO("Dragable",&widget_dragable);
		b->IO("HorizontalAnchor",&widget_horizontalAnchor);
		b->IO("HoverTime",&widget_hoverTime);
		b->IO("InheritOpacity",&widget_inheritOpacity);
		b->IO("MaxSize",&widget_maxSize);
		b->IO("MinSize",&widget_minSize);
		b->IO("PositionRelativeToParentClientDimensions",&widget_positionRelativeToParentClientDimensions);
		b->IO("RelativeOpacity",&widget_relativeOpacity);
		b->IO("ResizeFromBottom",&widget_resizeFromBottom);
		b->IO("ResizeFromLeft",&widget_resizeFromLeft);
		b->IO("ResizeFromRight",&widget_resizeFromRight);
		b->IO("ResizeFromTop",&widget_resizeFromTop);
		b->IO("Scrollable",&widget_scrollable);
		b->IO("TransparencyPicking",&widget_transparencyPicking);
		b->IO("VerticalAnchor",&widget_verticalAnchor);
		b->IO("Visible",&widget_visible);
		b->IO("SkinType",&widget_skinTypeName);
	}

	Widget::Widget(const Ogre::String& widget_name) :
		Serializable(),
		mBeingDragged(false),
		mComponentOfAWidget(false),
		mGrabbed(false),
		mInitialized(false),
		mParentWidget(0),
		mSkinElementName(""),
		mSkinType(0),
		mWidgetDesc(NULL),
		mWindow(0)
	{
		mName = widget_name;
	}

	Widget::~Widget()
	{
		FactoryManager::getSingleton().getDescFactory()->destroyInstance(mWidgetDesc->widget_name);

		// Clean up all user defined event handlers.
		for(int index = 0; index < WIDGET_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mWidgetEventHandlers[index].begin(); it != mWidgetEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void Widget::_createDescObject()
	{
		if(mWidgetDesc != NULL)
			return;

		mWidgetDesc = FactoryManager::getSingleton().getDescFactory()->createInstance<WidgetDesc>(getClass() + "Desc", mName);
		// Assign widget_name, stored from widget creation
		mWidgetDesc->widget_name = mName;

		if(mWidgetDesc->getWidgetClass() != getClass())
			throw Exception(Exception::ERR_INVALID_DESC,"Desc of class \"" + mWidgetDesc->getClass() + "\" is not meant for Widget of class \"" + getClass() + "\"!","Widget::_initialize");
	}

	void Widget::_initialize(WidgetDesc* d)
	{
		_createDescObject();

		if(mInitialized)
			throw Exception(Exception::ERR_INTERNAL_ERROR,"Widget is already initialized!","Widget::_initialize");

		mInitialized = true;

		setConsumeKeyboardEvents(d->widget_consumeKeyboardEvents);
		setContextMenuName(d->widget_contextMenuName);
		setEnabled(d->widget_enabled);
		setDimensions(d->widget_dimensions);
		setDragable(d->widget_dragable);
		setHorizontalAnchor(d->widget_horizontalAnchor);
		setHoverTime(d->widget_hoverTime);
		setInheritOpacity(d->widget_inheritOpacity);
		setMaxSize(d->widget_maxSize);
		setMinSize(d->widget_minSize);
		setName(d->widget_name);
		setPositionRelativeToParentClientDimensions(d->widget_positionRelativeToParentClientDimensions);
		setRelativeOpacity(d->widget_relativeOpacity);
		setResizeFromBottom(d->widget_resizeFromBottom);
		setResizeFromLeft(d->widget_resizeFromLeft);
		setResizeFromRight(d->widget_resizeFromRight);
		setResizeFromTop(d->widget_resizeFromTop);
		setScrollable(d->widget_scrollable);
		setSerialize(d->widget_serialize);
		setTransparencyPicking(d->widget_transparencyPicking);
		setVerticalAnchor(d->widget_verticalAnchor);
		setVisible(d->widget_visible);
	}

	void Widget::_setGUIManager(GUIManager* gm)
	{
		mWidgetDesc->guiManager = gm;
	}

	void Widget::_setSheet(Sheet* sheet)
	{
		if((sheet != NULL) && (sheet->findWidget(mWidgetDesc->widget_name) != NULL) && (sheet->findWidget(mWidgetDesc->widget_name) != this))
			throw Exception(Exception::ERR_DUPLICATE_ITEM,"A widget with widget_name \"" + mWidgetDesc->widget_name + "\" already exists in Sheet \"" + sheet->getName() + "\"!","Widget::_setSheet");

		mWidgetDesc->sheet = sheet;
	}

	void Widget::addWidgetEventHandler(WidgetEvent EVENT, EventHandlerSlot* function)
	{
		mWidgetEventHandlers[EVENT].push_back(function);
	}

	Widget* Widget::create(const Ogre::String& className, WidgetDesc* d)
	{
		if(d->widget_name == "")
		{
			d->widget_name = className + Ogre::StringConverter::toString(mWidgetCounter);
			++mWidgetCounter;
		}

		Ogre::String cName = d->getWidgetClass();

		if(className != d->getWidgetClass())
			throw Exception(Exception::ERR_INVALID_DESC,"Desc object for Widget of class \"" + d->getWidgetClass() + "\" does not match Widget of class \"" + className + "\"!","GUIManager::createWidget");

		Widget* newWidget = FactoryManager::getSingleton().getWidgetFactory()->createInstance<Widget>(className,d->widget_name);
		newWidget->_initialize(d);

		// Reset the widget_name, in case it was changed.  This Desc could be used multiple times, so this is important!
		d->widget_name = "";

		return newWidget;
	}

	void Widget::drag(int xOffset, int yOffset)
	{
		mBeingDragged = true;
		mWidgetDesc->widget_dimensions.position.translate(Point(xOffset,yOffset));

		updateTexturePosition();

		WidgetEventArgs wargs(this);
		fireWidgetEvent(WIDGET_EVENT_DRAGGED,wargs);
	}

	void Widget::draw()
	{
		// check visibility
		if( !mWidgetDesc->widget_visible )
			return;

		Brush* brush = Brush::getSingletonPtr();

		// check clip region
		Rect clipRegion = brush->getClipRegion();
		if ( clipRegion.getIntersection(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size)) == Rect::ZERO )
			return;

		// Set opacity before drawing operations
		brush->setOpacity(getAbsoluteOpacity());

		// onDraw
		onDraw();
	}

	Widget* Widget::findFirstWidgetOfClass(const Ogre::String& className)
	{
		if(getClass() == className)
			return this;
		return NULL;
	}

	Widget* Widget::findWidget(const Ogre::String& widget_name)
	{
		if(mWidgetDesc->widget_name == widget_name)
			return this;
		return NULL;
	}

	Widget* Widget::findWidgetAtPoint(const Point& p, bool ignoreDisabled)
	{
		// If we are not widget_visible, return NULL
		if(!mWidgetDesc->widget_visible)
			return NULL;

		// If we ignore disabled and this widget is !widget_enabled, return NULL
		if(ignoreDisabled && !mWidgetDesc->widget_enabled)
			return NULL;

		// See if point is within widget bounds
		if(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size).isPointWithinBounds(p))
		{
			// Take transparency picking into account
			if(mWidgetDesc->widget_transparencyPicking && (mSkinType != NULL))
			{
				// If the background is transparent, return NULL
				if(mSkinType->getSkinElement(mSkinElementName)->getTextureName() == "")
					return NULL;

				// Get relative position
				Point relPos = p - mTexturePosition;
				// Get percentage of position relative to widget widget_dimensions
				relPos.x = relPos.x / mWidgetDesc->widget_dimensions.size.width;
				relPos.y = relPos.y / mWidgetDesc->widget_dimensions.size.height;
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

	float Widget::getAbsoluteOpacity()
	{
		if(!mWidgetDesc->widget_inheritOpacity || (mParentWidget == NULL))
			return mWidgetDesc->widget_relativeOpacity;

		return mParentWidget->getAbsoluteOpacity() * mWidgetDesc->widget_relativeOpacity;
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
			if(p.y > (mWidgetDesc->widget_dimensions.size.height - se->getBorderThickness(BORDER_BOTTOM)))
				return BORDER_BOTTOM_LEFT;
			return BORDER_LEFT;
		}

		if(p.x > (mWidgetDesc->widget_dimensions.size.width - se->getBorderThickness(BORDER_RIGHT)))
		{
			if(p.y < se->getBorderThickness(BORDER_TOP))
				return BORDER_TOP_RIGHT;
			if(p.y > (mWidgetDesc->widget_dimensions.size.height - se->getBorderThickness(BORDER_BOTTOM)))
				return BORDER_BOTTOM_RIGHT;
			return BORDER_RIGHT;
		}

		if(p.y < se->getBorderThickness(BORDER_TOP))
			return BORDER_TOP;

		return BORDER_BOTTOM;
	}

	BrushFilterMode Widget::getBrushFilterMode()
	{
		return mWidgetDesc->widget_brushFilterMode;
	}

	Rect Widget::getClientDimensions()
	{
		return mClientDimensions;
	}

	bool Widget::getConsumeKeyboardEvents()
	{
		return mWidgetDesc->widget_consumeKeyboardEvents;
	}

	Ogre::String Widget::getContextMenuName()
	{
		return mWidgetDesc->widget_contextMenuName;
	}

	Rect Widget::getDimensions()
	{
		return mWidgetDesc->widget_dimensions;
	}

	Ogre::String Widget::getDisabledSkinType()
	{
		return mWidgetDesc->widget_disabledSkinType;
	}

	bool Widget::getDragable()
	{
		return mWidgetDesc->widget_dragable;
	}

	bool Widget::getEnabled()
	{
		return mWidgetDesc->widget_enabled;
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
		return mWidgetDesc->widget_dimensions.size.height;
	}

	HorizontalAnchor Widget::getHorizontalAnchor()
	{
		return mWidgetDesc->widget_horizontalAnchor;
	}

	float Widget::getHoverTime()
	{
		return mWidgetDesc->widget_hoverTime;
	}

	bool Widget::getInheritOpacity()
	{
		return mWidgetDesc->widget_inheritOpacity;
	}

	Size Widget::getMaxSize()
	{
		return mWidgetDesc->widget_maxSize;
	}

	Size Widget::getMinSize()
	{
		return mWidgetDesc->widget_minSize;
	}

	Ogre::String Widget::getName()
	{
		if(isComponentOfAWidget())
			return mParentWidget->getName() + mWidgetDesc->widget_name;

		return mWidgetDesc->widget_name;
	}

	Widget* Widget::getParentWidget()
	{
		return mParentWidget;
	}

	Point Widget::getPosition()
	{
		return mWidgetDesc->widget_dimensions.position;
	}

	bool Widget::getPositionRelativeToParentClientDimensions()
	{
		return mWidgetDesc->widget_positionRelativeToParentClientDimensions;
	}

	float Widget::getRelativeOpacity()
	{
		return mWidgetDesc->widget_relativeOpacity;
	}

	bool Widget::getResizeFromBottom()
	{
		return mWidgetDesc->widget_resizeFromBottom;
	}

	bool Widget::getResizeFromLeft()
	{
		return mWidgetDesc->widget_resizeFromLeft;
	}

	bool Widget::getResizeFromRight()
	{
		return mWidgetDesc->widget_resizeFromRight;
	}

	bool Widget::getResizeFromTop()
	{
		return mWidgetDesc->widget_resizeFromTop;
	}

	Point Widget::getScreenPosition()
	{
		if(mParentWidget == NULL)
			return mWidgetDesc->widget_dimensions.position + mScrollOffset;

		// Component widget's positions are not typically relative to their parent's client dimensions.
		// Other widgets can share this functionality also
		if(!mWidgetDesc->widget_positionRelativeToParentClientDimensions)
			return mParentWidget->getScreenPosition() + (mWidgetDesc->widget_dimensions.position - mScrollOffset);
		else
			return mParentWidget->getScreenPosition() + mParentWidget->getClientDimensions().position + (mWidgetDesc->widget_dimensions.position - mScrollOffset);
	}

	Point Widget::getScroll()
	{
		return mScrollOffset;
	}

	bool Widget::getScrollable()
	{
		return mWidgetDesc->widget_scrollable;
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

	bool Widget::getSerialize()
	{
		return mWidgetDesc->widget_serialize;
	}

	Sheet* Widget::getSheet()
	{
		return mWidgetDesc->sheet;
	}

	Size Widget::getSize()
	{
		return mWidgetDesc->widget_dimensions.size;
	}

	Point Widget::getTexturePosition()
	{
		return mTexturePosition;
	}

	bool Widget::getTransparencyPicking()
	{
		return mWidgetDesc->widget_transparencyPicking;
	}

	VerticalAnchor Widget::getVerticalAnchor()
	{
		return mWidgetDesc->widget_verticalAnchor;
	}

	bool Widget::getVisible()
	{
		return mWidgetDesc->widget_visible;
	}

	Ogre::String Widget::getSkinTypeName()
	{
		return mWidgetDesc->widget_skinTypeName;
	}

	float Widget::getWidth()
	{
		return mWidgetDesc->widget_dimensions.size.width;
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

	bool Widget::isComponentOfAWidget()
	{
		return mComponentOfAWidget;
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

		if((p.x < 0) || (p.x > mWidgetDesc->widget_dimensions.size.width))
			return false;

		if((p.y < 0) || (p.y > mWidgetDesc->widget_dimensions.size.height))
			return false;

		if(mSkinType == NULL)
			return false;

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
		if((p.x < se->getBorderThickness(BORDER_LEFT)) || (p.x > (mWidgetDesc->widget_dimensions.size.width - se->getBorderThickness(BORDER_RIGHT))))
			return true;

		if((p.y < se->getBorderThickness(BORDER_TOP)) || (p.y > (mWidgetDesc->widget_dimensions.size.height - se->getBorderThickness(BORDER_BOTTOM))))
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
				OGRE_DELETE_T(ehs,EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
				return;
			}
		}

		throw Exception(Exception::ERR_ITEM_NOT_FOUND,"No \"" + StringConverter::toString(EVENT) + "\" event handlers found that were registered by the given object!","Widget::removeEventHandler");
	}

	void Widget::resize(BorderSide s, float xOffset, float yOffset)
	{
		bool changePosition = false;
		Point newPosition = mWidgetDesc->widget_dimensions.position;
		
		// Record size before resize
		Size previousSize = mWidgetDesc->widget_dimensions.size;

		switch(s)
		{
		case BORDER_BOTTOM:
			setHeight(mWidgetDesc->widget_dimensions.size.height + yOffset);
			break;
		case BORDER_TOP_LEFT:
			{
				Size newSize(mWidgetDesc->widget_dimensions.size);
				newSize.width -= xOffset;
				newSize.height -= yOffset;
				setSize(newSize);

				changePosition = true;
				newPosition.x += previousSize.width - mWidgetDesc->widget_dimensions.size.width;
				newPosition.y += previousSize.height - mWidgetDesc->widget_dimensions.size.height;
			}
			break;
		case BORDER_BOTTOM_LEFT: 
			{
				Size newSize(mWidgetDesc->widget_dimensions.size);
				newSize.width -= xOffset;
				newSize.height += yOffset;
				setSize(newSize);

				changePosition = true;
				newPosition.x += previousSize.width - mWidgetDesc->widget_dimensions.size.width;
			}
			break;
		case BORDER_TOP_RIGHT:
			{				
				Size newSize(mWidgetDesc->widget_dimensions.size);
				newSize.width += xOffset;
				newSize.height -= yOffset;
				setSize(newSize);

				changePosition = true;
				newPosition.y += previousSize.height - mWidgetDesc->widget_dimensions.size.height;
			}
			break;
		case BORDER_BOTTOM_RIGHT:
			{
				Size newSize(mWidgetDesc->widget_dimensions.size);
				newSize.width += xOffset;
				newSize.height += yOffset;
				setSize(newSize);
			}
			break;
		case BORDER_LEFT: 
			{
				setWidth(mWidgetDesc->widget_dimensions.size.width - xOffset);

				changePosition = true;
				newPosition.x += previousSize.width - mWidgetDesc->widget_dimensions.size.width;
			}
			break;
		case BORDER_RIGHT:
			setWidth(mWidgetDesc->widget_dimensions.size.width + xOffset);
			break;
		case BORDER_TOP:
			{
				setHeight(mWidgetDesc->widget_dimensions.size.height - yOffset);

				changePosition = true;
				newPosition.y += previousSize.height - mWidgetDesc->widget_dimensions.size.height;
			}
			break;
		}

		// Top and/or Left resizes require repositioning of the widget.
		if(changePosition)
			setPosition(newPosition);
	}

	void Widget::serialize(SerialBase* b)
	{
		// Create Desc object if its not already created.
		_createDescObject();

		if(!mWidgetDesc->widget_serialize)
			return;

		b->begin(getClass(),getName());

		mWidgetDesc->serialize(b);

		if(b->isSerialReader())
		{
			// Apply Desc properties
			_initialize(mWidgetDesc);
		}

		b->end();
	}

	void Widget::setBrushFilterMode(BrushFilterMode m)
	{
		mWidgetDesc->widget_brushFilterMode = m;

		redraw();
	}

	void Widget::setConsumeKeyboardEvents(bool consume)
	{
		mWidgetDesc->widget_consumeKeyboardEvents = consume;
	}

	void Widget::setContextMenuName(const Ogre::String& contextMenuName)
	{
		mWidgetDesc->widget_contextMenuName = contextMenuName;
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

		mWidgetDesc->widget_disabledSkinType = SkinTypeName;

		if(!mWidgetDesc->widget_enabled)
			redraw();
	}

	void Widget::setDragable(bool dragable)
	{
		mWidgetDesc->widget_dragable = dragable;
	}

	void Widget::setEnabled(bool enabled)
	{
		if(mWidgetDesc->widget_enabled == enabled)
			return;

		mWidgetDesc->widget_enabled = enabled;

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_ENABLED_CHANGED,args);

		if(!mWidgetDesc->widget_enabled)
			redraw();
	}

	void Widget::setGrabbed(bool grabbed)
	{
		mGrabbed = grabbed;
	}

	void Widget::setHeight(float pixelHeight)
	{
		pixelHeight = Ogre::Math::Ceil(pixelHeight);

		// Enforce max height
		if((mWidgetDesc->widget_maxSize.height > 0) && (pixelHeight > mWidgetDesc->widget_maxSize.height))
			pixelHeight = mWidgetDesc->widget_maxSize.height;
		// Enforce min height
		else if(pixelHeight < mWidgetDesc->widget_minSize.height)
			pixelHeight = mWidgetDesc->widget_minSize.height;

		mWidgetDesc->widget_dimensions.size.height = pixelHeight;

		updateClientDimensions();

		redraw();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SIZE_CHANGED,args);
	}

	void Widget::setHorizontalAnchor(HorizontalAnchor a)
	{
		mWidgetDesc->widget_horizontalAnchor = a;
	}

	void Widget::setHoverTime(float seconds)
	{
		mWidgetDesc->widget_hoverTime = seconds;
	}

	void Widget::setInheritOpacity(bool inherit)
	{
		mWidgetDesc->widget_inheritOpacity = inherit;

		redraw();
	}

	void Widget::setMaxSize(const Size& s)
	{
		mWidgetDesc->widget_maxSize = s;

		setSize(mWidgetDesc->widget_dimensions.size);
	}

	void Widget::setMinSize(const Size& s)
	{
		mWidgetDesc->widget_minSize = s;

		setSize(mWidgetDesc->widget_dimensions.size);
	}

	void Widget::setName(const Ogre::String& name)
	{
		mWidgetDesc->widget_name = FactoryManager::getSingleton().getDescFactory()->updateName(mWidgetDesc,name);
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
		mWidgetDesc->widget_dimensions.position = position;
		mWidgetDesc->widget_dimensions.position.round();

		// Update screen rectangle

		updateTexturePosition();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_POSITION_CHANGED,args);
	}

	void Widget::setPositionRelativeToParentClientDimensions(bool relativeToClientOrigin)
	{
		mWidgetDesc->widget_positionRelativeToParentClientDimensions = relativeToClientOrigin;

		updateTexturePosition();
	}

	void Widget::setRelativeOpacity(float opacity)
	{
		if(opacity > 1)
			opacity = 1;
		else if(opacity < 0)
			opacity = 0;

		mWidgetDesc->widget_relativeOpacity = opacity;

		redraw();
	}

	void Widget::setResizeFromAllSides(bool resizable)
	{
		setResizeFromBottom(resizable);
		setResizeFromLeft(resizable);
		setResizeFromRight(resizable);
		setResizeFromTop(resizable);
	}

	void Widget::setResizeFromBottom(bool resizable)
	{
		mWidgetDesc->widget_resizeFromBottom = resizable;
	}

	void Widget::setResizeFromLeft(bool resizable)
	{
		mWidgetDesc->widget_resizeFromLeft = resizable;
	}

	void Widget::setResizeFromRight(bool resizable)
	{
		mWidgetDesc->widget_resizeFromRight = resizable;
	}

	void Widget::setResizeFromTop(bool resizable)
	{
		mWidgetDesc->widget_resizeFromTop = resizable;
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
		mWidgetDesc->widget_scrollable = scrollable;
	}

	void Widget::setSerialize(bool serialize)
	{
		mWidgetDesc->widget_serialize = serialize;
	}

	void Widget::setSize(Size size)
	{
		size.roundUp();

		// Enforce max width
		if((mWidgetDesc->widget_maxSize.width > 0) && (size.width > mWidgetDesc->widget_maxSize.width))
			size.width = mWidgetDesc->widget_maxSize.width;
		// Enforce min width
		else if((mWidgetDesc->widget_minSize.width > 0) && (size.width < mWidgetDesc->widget_minSize.width))
			size.width = mWidgetDesc->widget_minSize.width;

		// Enforce min height
		if((mWidgetDesc->widget_maxSize.height > 0) && (size.height > mWidgetDesc->widget_maxSize.height))
			size.height = mWidgetDesc->widget_maxSize.height;
		// Enforce min width
		else if((mWidgetDesc->widget_minSize.height > 0) && (size.height < mWidgetDesc->widget_minSize.height))
			size.height = mWidgetDesc->widget_minSize.height;

		// Update size
		mWidgetDesc->widget_dimensions.size = size;

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

		mWidgetDesc->widget_skinTypeName = type;

		updateClientDimensions();

		redraw();

		WidgetEventArgs wea(this);
		fireWidgetEvent(WIDGET_EVENT_SKIN_CHANGED,wea);
	}

	void Widget::setTransparencyPicking(bool transparencyPicking)
	{
		mWidgetDesc->widget_transparencyPicking = transparencyPicking;
	}

	void Widget::setVerticalAnchor(VerticalAnchor a)
	{
		mWidgetDesc->widget_verticalAnchor = a;
	}

	void Widget::setVisible(bool visible)
	{
		if(mWidgetDesc->widget_visible == visible)
			return;

		mWidgetDesc->widget_visible = visible;

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_VISIBLE_CHANGED,args);

		redraw();
	}

	void Widget::setWidth(float pixelWidth)
	{
		pixelWidth = Ogre::Math::Ceil(pixelWidth);

		// Enforce max width
		if((mWidgetDesc->widget_maxSize.width > 0) && (pixelWidth > mWidgetDesc->widget_maxSize.width))
			pixelWidth = mWidgetDesc->widget_maxSize.width;
		// Enforce min width
		else if(pixelWidth < mWidgetDesc->widget_minSize.width)
			pixelWidth = mWidgetDesc->widget_minSize.width;

		mWidgetDesc->widget_dimensions.size.width = pixelWidth;

		updateClientDimensions();

		redraw();

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SIZE_CHANGED,args);
	}

	void Widget::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		if(mSkinType != NULL)
		{
			SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
			mClientDimensions.position.x = se->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = se->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (se->getBorderThickness(BORDER_TOP) + se->getBorderThickness(BORDER_BOTTOM));
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
		//mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		// If there is a parent widget, translate by the parent's TexturePosition

		if (mParentWidget)
		{
			mTexturePosition.translate(mParentWidget->getTexturePosition());
			
			// Components in general aren't restricted to Parent's Client widget_dimensions.
			// Other widgets can share this functionality as well.
			if(mWidgetDesc->widget_positionRelativeToParentClientDimensions)
				mTexturePosition.translate(mParentWidget->getClientDimensions().position);
		}			

		// Translate by the Widget's position 
		mTexturePosition.translate(mWidgetDesc->widget_dimensions.position);

		if (mWidgetDesc->widget_scrollable)
			mTexturePosition.translate((mScrollOffset) * -1.0);

		// Force redraw

		redraw();
	}
}
