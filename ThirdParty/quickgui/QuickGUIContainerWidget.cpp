#include "QuickGUIContainerWidget.h"
#include "QuickGUIBrush.h"
#include "QuickGUISerialWriter.h"
#include "QuickGUIWindow.h"
#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUIScriptDefinition.h"
#include "QuickGUIFactoryManager.h"

namespace QuickGUI
{
	const Ogre::String ContainerWidget::HSCROLLBAR = "hscrollbar";
	const Ogre::String ContainerWidget::VSCROLLBAR = "vscrollbar";

	ContainerWidgetDesc::ContainerWidgetDesc(const Ogre::String& id) :
		ComponentWidgetDesc(id)
	{
		resetToDefault();
	}

	void ContainerWidgetDesc::resetToDefault()
	{
		ComponentWidgetDesc::resetToDefault();

		containerwidget_clipChildrenToDimensions = true;
		containerwidget_horzBarScrollPercent = 0.2;
		containerwidget_horzButtonScrollPercent = 0.1;
		containerwidget_supportScrollBars = true;
		containerwidget_scrollBarThickness = 15;
		containerwidget_vertBarScrollPercent = 0.2;
		containerwidget_vertButtonScrollPercent = 0.1;
		containerwidget_xScrollOffset = 0;
		containerwidget_yScrollOffset = 0;
	}

	void ContainerWidgetDesc::serialize(SerialBase* b)
	{
		ComponentWidgetDesc::serialize(b);

		b->IO("ClipChildrenToDimensions",&containerwidget_clipChildrenToDimensions);
		b->IO("HorzBarScrollPercent",&containerwidget_horzBarScrollPercent);
		b->IO("HorzButtonScrollPercent",&containerwidget_horzButtonScrollPercent);
		b->IO("SupportScrolling",&containerwidget_supportScrollBars);
		b->IO("ScrollBarThickness",&containerwidget_scrollBarThickness);
		b->IO("VertBarScrollPercent",&containerwidget_vertBarScrollPercent);
		b->IO("VertButtonScrollPercent",&containerwidget_vertButtonScrollPercent);
		b->IO("XScrollOffset",&containerwidget_xScrollOffset);
		b->IO("YScrollOffset",&containerwidget_yScrollOffset);
	}

	ContainerWidget::ContainerWidget(const Ogre::String& name) :
		ComponentWidget(name),
		mDesc(NULL),
		mHScrollBar(NULL),
		mVScrollBar(NULL)
	{
	}

	ContainerWidget::~ContainerWidget()
	{
		Factory<Widget>* widgetFactory = FactoryManager::getSingleton().getWidgetFactory();
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			widgetFactory->destroyInstance((*it));
	}

	void ContainerWidget::_determineVirtualSize()
	{
		mVirtualSize = mClientDimensions.size;

		Point tempPoint;
		Size tempSize;
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			if((*it)->getVisible())
			{
				tempPoint = (*it)->getPosition();
				tempSize = (*it)->getSize();

				if((tempPoint.x + tempSize.width) > mVirtualSize.width)
					mVirtualSize.width = (tempPoint.x + tempSize.width);
				if((tempPoint.y + tempSize.height) > mVirtualSize.height)
					mVirtualSize.height = (tempPoint.y + tempSize.height);
			}
		}
	}

	void ContainerWidget::_initialize(WidgetDesc* d)
	{
		ComponentWidget::_initialize(d);

		mDesc = dynamic_cast<ContainerWidgetDesc*>(mWidgetDesc);
		ContainerWidgetDesc* cwd = dynamic_cast<ContainerWidgetDesc*>(d);

		mDesc->containerwidget_clipChildrenToDimensions = cwd->containerwidget_clipChildrenToDimensions;
		mDesc->containerwidget_supportScrollBars = cwd->containerwidget_supportScrollBars;
		mDesc->containerwidget_scrollBarThickness = cwd->containerwidget_scrollBarThickness;
		mDesc->containerwidget_horzBarScrollPercent = cwd->containerwidget_horzBarScrollPercent;
		mDesc->containerwidget_horzButtonScrollPercent = cwd->containerwidget_horzButtonScrollPercent;
		mDesc->containerwidget_vertBarScrollPercent = cwd->containerwidget_vertBarScrollPercent;
		mDesc->containerwidget_vertButtonScrollPercent = cwd->containerwidget_vertButtonScrollPercent;

		if(mDesc->containerwidget_supportScrollBars)
		{
			HScrollBarDesc* hd = FactoryManager::getSingleton().getDescFactory()->getInstance<HScrollBarDesc>("DefaultHScrollBarDesc");
			hd->resetToDefault();
			hd->widget_dimensions.size.height = mDesc->containerwidget_scrollBarThickness;
			// Do not anchor, as it interferes with dynamically resizing and positioning the scrollbars
			hd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
			hd->widget_verticalAnchor = ANCHOR_VERTICAL_BOTTOM;
			hd->hscrollbar_barScrollPercent = mDesc->containerwidget_horzBarScrollPercent;
			hd->hscrollbar_buttonScrollPercent = mDesc->containerwidget_horzButtonScrollPercent;
			hd->widget_visible = false;

			mHScrollBar = dynamic_cast<HScrollBar*>(Widget::create("HScrollBar",hd));
			mHScrollBar->enableLiveSlider(&(mClientDimensions.size.width),&(mVirtualSize.width));
			mHScrollBar->addScrollBarEventHandler(QuickGUI::SCROLLBAR_EVENT_ON_SCROLLED,&ContainerWidget::onHorizontalScroll,this);
			addComponent(HSCROLLBAR,mHScrollBar);

			VScrollBarDesc* vd = FactoryManager::getSingleton().getDescFactory()->getInstance<VScrollBarDesc>("DefaultVScrollBarDesc");
			vd->resetToDefault();
			vd->widget_dimensions.size.width = mDesc->containerwidget_scrollBarThickness;
			// Do not anchor, as it interferes with dynamically resizing and positioning the scrollbars
			vd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
			vd->widget_verticalAnchor = ANCHOR_VERTICAL_TOP;
			vd->vscrollbar_barScrollPercent = mDesc->containerwidget_vertBarScrollPercent;
			vd->vscrollbar_buttonScrollPercent = mDesc->containerwidget_vertButtonScrollPercent;
			vd->widget_visible = false;

			mVScrollBar = dynamic_cast<VScrollBar*>(Widget::create("VScrollBar",vd));
			mVScrollBar->enableLiveSlider(&(mClientDimensions.size.height),&(mVirtualSize.height));
			mVScrollBar->addScrollBarEventHandler(QuickGUI::SCROLLBAR_EVENT_ON_SCROLLED,&ContainerWidget::onVerticalScroll,this);
			addComponent(VSCROLLBAR,mVScrollBar);

			addWidgetEventHandler(WIDGET_EVENT_MOUSE_WHEEL,&ContainerWidget::onMouseWheel,this);
		}
	}

	void ContainerWidget::_setGUIManager(GUIManager* gm)
	{
		ComponentWidget::_setGUIManager(gm);

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			(*it)->_setGUIManager(gm);
	}

	void ContainerWidget::_setSheet(Sheet* sheet)
	{
		ComponentWidget::_setSheet(sheet);

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			(*it)->_setSheet(sheet);
	}

	void ContainerWidget::_setScrollX(float x)
	{
		if(!mDesc->containerwidget_supportScrollBars)
			return;

		mDesc->containerwidget_xScrollOffset = (x * mVirtualSize.width);

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->setScrollX(mDesc->containerwidget_xScrollOffset);
		}
	}

	void ContainerWidget::_setScrollY(float y)
	{
		if(!mDesc->containerwidget_supportScrollBars)
			return;

		mDesc->containerwidget_yScrollOffset = (y * mVirtualSize.height);

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->setScrollY(mDesc->containerwidget_yScrollOffset);
		}
	}

	void ContainerWidget::_updateScrollBars()
	{
		bool updateVirtualSize = false;

		if(mHScrollBar)
		{
			// Sync slider size to virtual/actual width ratio
			mHScrollBar->setPercentage(mHScrollBar->getPercentage());

			if((mHScrollBar->getSliderWidthPercentage() > 0.99) && mHScrollBar->getVisible())
			{
				mHScrollBar->setVisible(false);
				mClientDimensions.size.height += mHScrollBar->getHeight();
				updateVirtualSize = true;
			}
			else if((mHScrollBar->getSliderWidthPercentage() < 0.99) && !mHScrollBar->getVisible())
			{
				mHScrollBar->setVisible(true);
				mClientDimensions.size.height -= mHScrollBar->getHeight();
				updateVirtualSize = true;
			}
		}

		if(mVScrollBar)
		{
			// Sync slider size to virtual/actual width ratio
			mVScrollBar->setPercentage(mVScrollBar->getPercentage());

			if((mVScrollBar->getSliderHeightPercentage() > 0.99) && mVScrollBar->getVisible())
			{
				mVScrollBar->setVisible(false);
				mClientDimensions.size.width += mVScrollBar->getWidth();
				updateVirtualSize = true;
			}
			else if((mVScrollBar->getSliderHeightPercentage() < 0.99) && !mVScrollBar->getVisible())
			{
				mVScrollBar->setVisible(true);
				mClientDimensions.size.width -= mVScrollBar->getWidth();
				updateVirtualSize = true;
			}
		}

		// Make sure ScrollBars are the correct size
		if(mHScrollBar)
			mHScrollBar->setWidth(mClientDimensions.size.width);
		if(mVScrollBar)
			mVScrollBar->setHeight(mClientDimensions.size.height);

		if(updateVirtualSize)
		{
			WidgetEventArgs args(this);
			fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);
			_determineVirtualSize();
		}
	}

	void ContainerWidget::addChild(Widget* w)
	{
		mChildren.push_back(w);
		w->setParent(this);
		w->_setGUIManager(mWidgetDesc->guiManager);
		w->_setSheet(mWidgetDesc->sheet);

		if(mDesc->containerwidget_supportScrollBars)
		{
			// Register Event Handler for when child is resized, moved, or visibility changes
			w->addWidgetEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,&ContainerWidget::onChildVisibilityChanged,this);
			w->addWidgetEventHandler(WIDGET_EVENT_SIZE_CHANGED,&ContainerWidget::onChildDimensionsChanged,this);
			w->addWidgetEventHandler(WIDGET_EVENT_POSITION_CHANGED,&ContainerWidget::onChildDimensionsChanged,this);

			Point tempPoint = w->getPosition();
			Size tempSize = w->getSize();

			if((tempPoint.x + tempSize.width) > mVirtualSize.width)
				mVirtualSize.width = (tempPoint.x + tempSize.width);
			if((tempPoint.y + tempSize.height) > mVirtualSize.height)
				mVirtualSize.height = (tempPoint.y + tempSize.height);

			// Check if ScrollBars need to be marked visible or not
			_updateScrollBars();
		}
	}

	void ContainerWidget::draw()
	{
		// check visibility
		if( !mWidgetDesc->widget_visible )
			return;

		Brush* brush = Brush::getSingletonPtr();

		// store clip region
		Rect prevClipRegion = brush->getClipRegion();

		// set clip region to dimensions
		brush->setClipRegion(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size).getIntersection(prevClipRegion));

		// Set opacity before drawing operations
		brush->setOpacity(getAbsoluteOpacity());

		// check and clip region - do not draw if we are outside clipping region
		if ( prevClipRegion.getIntersection(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size)) != Rect::ZERO )
		{
			// draw self
			onDraw();
		}

		if(!mDesc->containerwidget_clipChildrenToDimensions)
			brush->setClipRegion(prevClipRegion);

		// draw components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->draw();

		if(mDesc->containerwidget_clipChildrenToDimensions)
		{
			// set clip region to client dimensions
			Rect clipRegion = mClientDimensions;
			clipRegion.translate(mTexturePosition);
			brush->setClipRegion(clipRegion);
		}
		else
			brush->setClipRegion(prevClipRegion);

		// draw children
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			(*it)->draw();

		// restore clip region
		brush->setClipRegion(prevClipRegion);
	}

	Widget* ContainerWidget::findFirstWidgetOfClass(const Ogre::String& className)
	{
		Widget* w = NULL;
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			w = (*it)->findFirstWidgetOfClass(className);
			if(w != NULL)
				return w;
		}

		return Widget::findFirstWidgetOfClass(className);
	}

	Widget* ContainerWidget::findWidget(const Ogre::String& name)
	{
		Widget* w;
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			w = (*it)->findWidget(name);
			if(w != NULL)
				return w;
		}

		return ComponentWidget::findWidget(name);
	}

	Widget* ContainerWidget::findWidgetAtPoint(const Point& p, bool ignoreDisabled)
	{
		// If we are not widget_visible, return NULL
		if(!mWidgetDesc->widget_visible)
			return NULL;

		// If we ignore disabled and this widget is !widget_enabled, return NULL
		if(ignoreDisabled && !mWidgetDesc->widget_enabled)
			return NULL;

		// Check components before verifying point is within bounds. (Components can lie outside widget dimensions)
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			Widget* w = (*it).second->findWidgetAtPoint(p,ignoreDisabled);
			if(w != NULL)
				return w;
		}

		// See if point is within widget bounds
		if(!Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size).isPointWithinBounds(p))
			return NULL;

		// Get the client bounds as displayed on the texture
		Rect clientBounds(mTexturePosition,mClientDimensions.size);
		clientBounds.translate(mClientDimensions.position);

		if(clientBounds.isPointWithinBounds(p))
		{
			for(std::vector<Widget*>::reverse_iterator it = mChildren.rbegin(); it != mChildren.rend(); ++it)
			{
				Widget* w = (*it)->findWidgetAtPoint(p,ignoreDisabled);
				if(w != NULL)
					return w;
			}
		}
				
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

	std::vector<Widget*> ContainerWidget::getChildren()
	{
		std::vector<Widget*> list(mChildren);

		return list;
	}

	bool ContainerWidget::getClipChildrenToDimensions()
	{
		return mDesc->containerwidget_clipChildrenToDimensions;
	}

	bool ContainerWidget::isContainerWidget()
	{
		return true;
	}

	void ContainerWidget::onChildVisibilityChanged(const EventArgs& args)
	{
		_determineVirtualSize();
		_updateScrollBars();
	}

	void ContainerWidget::onChildDimensionsChanged(const EventArgs& args)
	{
		_determineVirtualSize();
		_updateScrollBars();
	}

	void ContainerWidget::onHorizontalScroll(const EventArgs& args)
	{
		_setScrollX(mHScrollBar->getPercentage());
	}

	void ContainerWidget::onMouseWheel(const EventArgs& args)
	{
		if((mVScrollBar == NULL) || !mVScrollBar->getVisible())
			return;

		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.wheelChange > 0)
			mVScrollBar->scrollUp();
		else
			mVScrollBar->scrollDown();
	}

	void ContainerWidget::onVerticalScroll(const EventArgs& args)
	{
		_setScrollY(mVScrollBar->getPercentage());
	}

	void ContainerWidget::removeChild(Widget* w)
	{
		std::vector<Widget*>::iterator it = std::find(mChildren.begin(),mChildren.end(),w);
		if(it == mChildren.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"Widget \"" + w->getName() + "\" is not a child of widget \"" + getName() + "\"","ContainerWidget::removeChild");

		mChildren.erase(it);

		w->_setSheet(NULL);
		w->_setGUIManager(NULL);
		w->setParent(NULL);

		if(mDesc->containerwidget_supportScrollBars)
		{
			// Register Event Handler for when child is resized, moved, or visibility changed
			w->removeEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,this);
			w->removeEventHandler(WIDGET_EVENT_SIZE_CHANGED,this);
			w->removeEventHandler(WIDGET_EVENT_POSITION_CHANGED,this);

			_determineVirtualSize();
			_updateScrollBars();
		}
	}

	void ContainerWidget::scrollChildIntoView(Widget* child)
	{
		std::vector<Widget*>::iterator it = std::find(mChildren.begin(),mChildren.end(),child);
		if(it == mChildren.end())
			throw Exception(Exception::ERR_INVALID_CHILD,"Widget \"" + child->getName() + "\" is not a child of widget \"" + getName() + "\"","ContainerWidget::scrollChildIntoView");

		Point scrollAmount = child->getScroll();
		Point relativePosition = child->getPosition();

		scrollPointIntoView(relativePosition + scrollAmount);
	}

	void ContainerWidget::scrollPointIntoView(const Point& p)
	{
		Point pointFromVirtualArea = p - Point(mDesc->containerwidget_xScrollOffset,mDesc->containerwidget_yScrollOffset);

		// Check x for scrolling

		// Check if we need to scroll left
		if(pointFromVirtualArea.x < 0)
		{
			if(mHScrollBar != NULL)
				mHScrollBar->setPercentage(p.x / mVirtualSize.width);
		}
		// Check if we need to scroll right
		else if(pointFromVirtualArea.x > mClientDimensions.size.width)
		{
			if(mHScrollBar != NULL)
				mHScrollBar->setPercentage((p.x - mClientDimensions.size.width) / mVirtualSize.width);
		}

		// Check y for scrolling
		
		// Check if we need to scroll up
		if(pointFromVirtualArea.y < 0)
		{
			if(mVScrollBar != NULL)
				mVScrollBar->setPercentage(p.y / mVirtualSize.height);
		}
		// Check if we need to scroll down
		else if(pointFromVirtualArea.y > mClientDimensions.size.height)
		{
			if(mVScrollBar != NULL)
				mVScrollBar->setPercentage((p.y - mClientDimensions.size.height) / mVirtualSize.height);
		}
	}

	void ContainerWidget::serialize(SerialBase* b)
	{
		// Create internal Desc object if it doesn't exist
		_createDescObject();

		if(!mWidgetDesc->widget_serialize)
			return;

		b->begin(getClass(),getName());

		mWidgetDesc->serialize(b);

		b->begin("Child","widgets");
		// If we are reading, we need to create and populate widgets
		if(b->isSerialReader())
		{
			// Apply Desc properties
			_initialize(mWidgetDesc);

			std::list<ScriptDefinition*> defList = b->getCurrentDefinition()->getDefinitions();
			for(std::list<ScriptDefinition*>::iterator it = defList.begin(); it != defList.end(); ++it)
			{
				// Create Empty Widget, supplying class name and widget name from script
				Widget* newWidget = FactoryManager::getSingleton().getWidgetFactory()->createInstance<Widget>((*it)->getType(),(*it)->getID());

				// Populate Desc object from Script Text, and initialize widget
				newWidget->serialize(b);

				// Add as child Widget
				addChild(newWidget);
			}
		}
		// If we are writing, we need to serialize children
		else
		{
			for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			{
				(*it)->serialize(b);
			}
		}
		b->end();

		b->end();
	}

	void ContainerWidget::setClipChildrenToDimensions(bool clip)
	{
		mDesc->containerwidget_clipChildrenToDimensions = clip;

		redraw();
	}

	void ContainerWidget::setHeight(float pixelHeight)
	{
		pixelHeight = Ogre::Math::Ceil(pixelHeight);

		float previousHeight = mWidgetDesc->widget_dimensions.size.height;

		// Modified code from Widget::setHeight, to incorporate ScrollBars
		{
			// Enforce max height
			if((mWidgetDesc->widget_maxSize.height > 0) && (pixelHeight > mWidgetDesc->widget_maxSize.height))
				pixelHeight = mWidgetDesc->widget_maxSize.height;
			// Enforce min height
			else if((mWidgetDesc->widget_minSize.height > 0) && (pixelHeight < mWidgetDesc->widget_minSize.height))
				pixelHeight = mWidgetDesc->widget_minSize.height;

			mWidgetDesc->widget_dimensions.size.height = pixelHeight;

			redraw();

			updateClientDimensions();
		}

		float difference = mWidgetDesc->widget_dimensions.size.height - previousHeight;

		// Handle anchoring for Components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			switch((*it).second->getVerticalAnchor())
			{
			case ANCHOR_VERTICAL_CENTER:
				{
					// Center vertically
					Point p = (*it).second->getPosition();
					p.y = (mWidgetDesc->widget_dimensions.size.height / 2.0) - ((*it).second->getHeight() / 2.0);
					(*it).second->setPosition(p);
				}
				break;
			case ANCHOR_VERTICAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it).second->setHeight((*it).second->getHeight() + difference);
					// Center vertically
					Point p = (*it).second->getPosition();
					p.y = (mWidgetDesc->widget_dimensions.size.height / 2.0) - ((*it).second->getHeight() / 2.0);
					(*it).second->setPosition(p);
				}
				break;
			case ANCHOR_VERTICAL_TOP_BOTTOM:
				{
					// grow or shrink the widget according to new dimensions
					(*it).second->setHeight((*it).second->getHeight() + difference);
				}
				break;
			case ANCHOR_VERTICAL_BOTTOM:
				{
					// re-align component widget according to bottom edge of this widgets new dimensions
					Point p = (*it).second->getPosition();
					p.y += difference;
					(*it).second->setPosition(p);
				}
				break;
			}
		}

		// Handle anchoring for Children
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			switch((*it)->getVerticalAnchor())
			{
			case ANCHOR_VERTICAL_CENTER:
				{
					// Center vertically
					Point p = (*it)->getPosition();
					p.y = (mWidgetDesc->widget_dimensions.size.height / 2.0) - ((*it)->getHeight() / 2.0);
					(*it)->setPosition(p);
				}
				break;
			case ANCHOR_VERTICAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it)->setHeight((*it)->getHeight() + difference);
					// Center vertically
					Point p = (*it)->getPosition();
					p.y = (mWidgetDesc->widget_dimensions.size.height / 2.0) - ((*it)->getHeight() / 2.0);
					(*it)->setPosition(p);
				}
				break;
			case ANCHOR_VERTICAL_TOP_BOTTOM:
				{
					// grow or shrink the widget according to new dimensions
					(*it)->setHeight((*it)->getHeight() + difference);
				}
				break;
			case ANCHOR_VERTICAL_BOTTOM:
				{
					// re-align component widget according to bottom edge of this widgets new dimensions
					Point p = (*it)->getPosition();
					p.y += difference;
					(*it)->setPosition(p);
				}
				break;
			}
		}

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SIZE_CHANGED,args);

		_determineVirtualSize();
		_updateScrollBars();
	}

	void ContainerWidget::setHorzBarScrollPercent(float percent)
	{
		percent = Ogre::Math::Floor(Ogre::Math::Ceil(percent * 1000.0)) / 1000.0;

		mDesc->containerwidget_horzBarScrollPercent = percent;

		if(mHScrollBar)
			mHScrollBar->setBarScrollPercent(percent);
	}

	void ContainerWidget::setHorzButtonScrollPercent(float percent)
	{
		percent = Ogre::Math::Floor(Ogre::Math::Ceil(percent * 1000.0)) / 1000.0;

		mDesc->containerwidget_horzButtonScrollPercent = percent;

		if(mHScrollBar)
			mHScrollBar->setButtonScrollPercent(percent);
	}

	void ContainerWidget::setSize(Size size)
	{
		size.roundUp();

		Size previousSize = mWidgetDesc->widget_dimensions.size;

		// Modified code from Widget::setSize, to incorporate ScrollBars
		{
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

			redraw();

			updateClientDimensions();
		}

		// Get difference.  A larger size indicates a positive difference
		Size difference = mWidgetDesc->widget_dimensions.size - previousSize;

		// Handle anchoring for Components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			switch((*it).second->getHorizontalAnchor())
			{
			case ANCHOR_HORIZONTAL_CENTER:
				{
					// Center vertically
					Point p = (*it).second->getPosition();
					p.x = (mWidgetDesc->widget_dimensions.size.width / 2.0) - ((*it).second->getWidth() / 2.0);
					(*it).second->setPosition(p);
				}
				break;
			case ANCHOR_HORIZONTAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it).second->setWidth((*it).second->getWidth() + difference.width);
					// Center vertically
					Point p = (*it).second->getPosition();
					p.x = (mWidgetDesc->widget_dimensions.size.width / 2.0) - ((*it).second->getWidth() / 2.0);
					(*it).second->setPosition(p);
				}
				break;
			case ANCHOR_HORIZONTAL_LEFT_RIGHT:
				{
					// grow or shrink the widget according to new dimensions
					(*it).second->setWidth((*it).second->getWidth() + difference.width);
				}
				break;
			case ANCHOR_HORIZONTAL_RIGHT:
				{
					// re-align component widget according to right edge of this widgets new dimensions
					Point p = (*it).second->getPosition();
					p.x += difference.width;
					(*it).second->setPosition(p);
				}
				break;
			}

			switch((*it).second->getVerticalAnchor())
			{
			case ANCHOR_VERTICAL_CENTER:
				{
					// Center vertically
					Point p = (*it).second->getPosition();
					p.y = (mWidgetDesc->widget_dimensions.size.height / 2.0) - ((*it).second->getHeight() / 2.0);
					(*it).second->setPosition(p);
				}
				break;
			case ANCHOR_VERTICAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it).second->setHeight((*it).second->getHeight() + difference.height);
					// Center vertically
					Point p = (*it).second->getPosition();
					p.y = (mWidgetDesc->widget_dimensions.size.height / 2.0) - ((*it).second->getHeight() / 2.0);
					(*it).second->setPosition(p);
				}
				break;
			case ANCHOR_VERTICAL_TOP_BOTTOM:
				{
					// grow or shrink the widget according to new dimensions
					(*it).second->setHeight((*it).second->getHeight() + difference.height);
				}
				break;
			case ANCHOR_VERTICAL_BOTTOM:
				{
					// re-align component widget according to bottom edge of this widgets new dimensions
					Point p = (*it).second->getPosition();
					p.y += difference.height;
					(*it).second->setPosition(p);
				}
				break;
			}
		}

		// Handle Anchoring for children
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			switch((*it)->getHorizontalAnchor())
			{
			case ANCHOR_HORIZONTAL_CENTER:
				{
					// Center vertically
					Point p = (*it)->getPosition();
					p.x = (mWidgetDesc->widget_dimensions.size.width / 2.0) - ((*it)->getWidth() / 2.0);
					(*it)->setPosition(p);
				}
				break;
			case ANCHOR_HORIZONTAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it)->setWidth((*it)->getWidth() + difference.width);
					// Center vertically
					Point p = (*it)->getPosition();
					p.x = (mWidgetDesc->widget_dimensions.size.width / 2.0) - ((*it)->getWidth() / 2.0);
					(*it)->setPosition(p);
				}
				break;
			case ANCHOR_HORIZONTAL_LEFT_RIGHT:
				{
					// grow or shrink the widget according to new dimensions
					(*it)->setWidth((*it)->getWidth() + difference.width);
				}
				break;
			case ANCHOR_HORIZONTAL_RIGHT:
				{
					// re-align component widget according to right edge of this widgets new dimensions
					Point p = (*it)->getPosition();
					p.x += difference.width;
					(*it)->setPosition(p);
				}
				break;
			}

			switch((*it)->getVerticalAnchor())
			{
			case ANCHOR_VERTICAL_CENTER:
				{
					// Center vertically
					Point p = (*it)->getPosition();
					p.y = (mWidgetDesc->widget_dimensions.size.height / 2.0) - ((*it)->getHeight() / 2.0);
					(*it)->setPosition(p);
				}
				break;
			case ANCHOR_VERTICAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it)->setHeight((*it)->getHeight() + difference.height);
					// Center vertically
					Point p = (*it)->getPosition();
					p.y = (mWidgetDesc->widget_dimensions.size.height / 2.0) - ((*it)->getHeight() / 2.0);
					(*it)->setPosition(p);
				}
				break;
			case ANCHOR_VERTICAL_TOP_BOTTOM:
				{
					// grow or shrink the widget according to new dimensions
					(*it)->setHeight((*it)->getHeight() + difference.height);
				}
				break;
			case ANCHOR_VERTICAL_BOTTOM:
				{
					// re-align component widget according to bottom edge of this widgets new dimensions
					Point p = (*it)->getPosition();
					p.y += difference.height;
					(*it)->setPosition(p);
				}
				break;
			}
		}

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SIZE_CHANGED,args);

		_determineVirtualSize();
		_updateScrollBars();
	}

	void ContainerWidget::setSkinType(const Ogre::String type)
	{
		if(!SkinTypeManager::getSingleton().hasSkinType(getClass(),type))
			throw Exception(Exception::ERR_SKINNING,"SkinType \"" + type + "\" does not exist!","Widget::setSkinType");

		mSkinType = SkinTypeManager::getSingleton().getSkinType(getClass(),type);

		mWidgetDesc->widget_skinTypeName = type;

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);

		if(mHScrollBar)
			mHScrollBar->setPosition(Point(se->getBorderThickness(BORDER_LEFT),mWidgetDesc->widget_dimensions.size.height - se->getBorderThickness(BORDER_BOTTOM) - mDesc->containerwidget_scrollBarThickness));

		if(mVScrollBar)
			mVScrollBar->setPosition(Point(mWidgetDesc->widget_dimensions.size.width - se->getBorderThickness(BORDER_RIGHT) - mVScrollBar->getWidth(),se->getBorderThickness(BORDER_TOP)));

		// Initially set dimensions of ScrollBars and update client dimensions
		updateClientDimensions();

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if(mSkinType->hasComponentType((*it).first))
				(*it).second->setSkinType(mSkinType->getComponentType((*it).first)->typeName);
		}

		WidgetEventArgs wea(this);
		fireWidgetEvent(WIDGET_EVENT_SKIN_CHANGED,wea);

		// Determine Virtual Size.  This can cause ScrollBars to be shown and hidden.
		_determineVirtualSize();
		// Check ScrollBar visibility, update client dimensions if needbe, and re-determine virtual size
		_updateScrollBars();
	}

	bool ContainerWidget::supportsScrolling()
	{
		return mDesc->containerwidget_supportScrollBars;
	}

	void ContainerWidget::setVertBarScrollPercent(float percent)
	{
		percent = Ogre::Math::Floor(Ogre::Math::Ceil(percent * 1000.0)) / 1000.0;

		mDesc->containerwidget_vertBarScrollPercent = percent;

		if(mVScrollBar)
			mVScrollBar->setBarScrollPercent(percent);
	}

	void ContainerWidget::setVertButtonScrollPercent(float percent)
	{
		percent = Ogre::Math::Floor(Ogre::Math::Ceil(percent * 1000.0)) / 1000.0;

		mDesc->containerwidget_vertButtonScrollPercent = percent;

		if(mVScrollBar)
			mVScrollBar->setButtonScrollPercent(percent);
	}

	void ContainerWidget::setWidth(float pixelWidth)
	{
		pixelWidth = Ogre::Math::Ceil(pixelWidth);

		float previousWidth = mWidgetDesc->widget_dimensions.size.width;

		// Modified code from Widget::setWidth, to incorporate ScrollBars
		{
			// Enforce max width
			if((mWidgetDesc->widget_maxSize.width > 0) && (pixelWidth > mWidgetDesc->widget_maxSize.width))
				pixelWidth = mWidgetDesc->widget_maxSize.width;
			// Enforce min width
			else if((mWidgetDesc->widget_minSize.width > 0) && (pixelWidth < mWidgetDesc->widget_minSize.width))
				pixelWidth = mWidgetDesc->widget_minSize.width;

			mWidgetDesc->widget_dimensions.size.width = pixelWidth;

			redraw();

			updateClientDimensions();
		}

		float difference = mWidgetDesc->widget_dimensions.size.width - previousWidth;

		// Handle anchoring for Components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			switch((*it).second->getHorizontalAnchor())
			{
			case ANCHOR_HORIZONTAL_CENTER:
				{
					// Center vertically
					Point p = (*it).second->getPosition();
					p.x = (mWidgetDesc->widget_dimensions.size.width / 2.0) - ((*it).second->getWidth() / 2.0);
					(*it).second->setPosition(p);
				}
				break;
			case ANCHOR_HORIZONTAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it).second->setWidth((*it).second->getWidth() + difference);
					// Center vertically
					Point p = (*it).second->getPosition();
					p.x = (mWidgetDesc->widget_dimensions.size.width / 2.0) - ((*it).second->getWidth() / 2.0);
					(*it).second->setPosition(p);
				}
				break;
			case ANCHOR_HORIZONTAL_LEFT_RIGHT:
				{
					// grow or shrink the widget according to new dimensions
					(*it).second->setWidth((*it).second->getWidth() + difference);
				}
				break;
			case ANCHOR_HORIZONTAL_RIGHT:
				{
					// re-align component widget according to right edge of this widgets new dimensions
					Point p = (*it).second->getPosition();
					p.x += difference;
					(*it).second->setPosition(p);
				}
				break;
			}
		}

		// Handle anchoring for Children
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			switch((*it)->getHorizontalAnchor())
			{
			case ANCHOR_HORIZONTAL_CENTER:
				{
					// Center vertically
					Point p = (*it)->getPosition();
					p.x = (mWidgetDesc->widget_dimensions.size.width / 2.0) - ((*it)->getWidth() / 2.0);
					(*it)->setPosition(p);
				}
				break;
			case ANCHOR_HORIZONTAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it)->setWidth((*it)->getWidth() + difference);
					// Center vertically
					Point p = (*it)->getPosition();
					p.x = (mWidgetDesc->widget_dimensions.size.width / 2.0) - ((*it)->getWidth() / 2.0);
					(*it)->setPosition(p);
				}
				break;
			case ANCHOR_HORIZONTAL_LEFT_RIGHT:
				{
					// grow or shrink the widget according to new dimensions
					(*it)->setWidth((*it)->getWidth() + difference);
				}
				break;
			case ANCHOR_HORIZONTAL_RIGHT:
				{
					// re-align component widget according to right edge of this widgets new dimensions
					Point p = (*it)->getPosition();
					p.x += difference;
					(*it)->setPosition(p);
				}
				break;
			}
		}

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_SIZE_CHANGED,args);

		_determineVirtualSize();
		_updateScrollBars();
	}

	void ContainerWidget::setParent(Widget* parent)
	{
		ComponentWidget::setParent(parent);

		// Update children's window reference via setParent
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->setParent(this);
		}
	}

	void ContainerWidget::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		if(mSkinType != NULL)
		{
			// Default
			SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
			mClientDimensions.position.x = se->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = se->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->widget_dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->widget_dimensions.size.height - (se->getBorderThickness(BORDER_TOP) + se->getBorderThickness(BORDER_BOTTOM));

			// First we want to adjust the client dimensions, depending on whether the scrollbars are visible.

			// If HorizontalScrollBar is visible, update its dimensions and update client dimensions
			if(mHScrollBar && mHScrollBar->getVisible())
				mClientDimensions.size.height -= mHScrollBar->getHeight();

			// If VerticalScrollBar is visible, update its dimensions and update client dimensions
			if(mVScrollBar && mVScrollBar->getVisible())
				mClientDimensions.size.width -= mVScrollBar->getWidth();

			// Now that client dimensions are fully updated, we can adjust ScrollBars position and size

			if(mHScrollBar)
				mHScrollBar->setWidth(mClientDimensions.size.width);

			if(mVScrollBar)
				mVScrollBar->setHeight(mClientDimensions.size.height);
		}

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);

		redraw();
	}

	void ContainerWidget::updateTexturePosition()
	{
		// Derive texture position and client dimensions
		Widget::updateTexturePosition();

		// Update component screen dimensions, must be done after client and screen rect have been calculated
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			(*it).second->updateTexturePosition();
		}

		// Update children screen dimensions, must be done after client and screen rect have been calculated
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->updateTexturePosition();
		}
	}
}
