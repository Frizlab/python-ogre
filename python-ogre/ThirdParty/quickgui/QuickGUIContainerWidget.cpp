#include "QuickGUIContainerWidget.h"
#include "QuickGUIBrush.h"
#include "QuickGUISerialWriter.h"
#include "QuickGUIWindow.h"
#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUIWidgetDescFactoryManager.h"
#include "QuickGUIScriptDefinition.h"

namespace QuickGUI
{
	const Ogre::String ContainerWidget::HSCROLLBAR = "hscrollbar";
	const Ogre::String ContainerWidget::VSCROLLBAR = "vscrollbar";

	ContainerWidgetDesc::ContainerWidgetDesc() :
		ComponentWidgetDesc()
	{
		horzBarScrollPercent = 0.2;
		horzButtonScrollPercent = 0.1;
		supportScrollBars = true;
		scrollBarThickness = 15;
		vertBarScrollPercent = 0.2;
		vertButtonScrollPercent = 0.1;
		xScrollOffset = 0;
		yScrollOffset = 0;
	}

	void ContainerWidgetDesc::serialize(SerialBase* b)
	{
		ComponentWidgetDesc::serialize(b);

		b->IO("HorzBarScrollPercent",&horzBarScrollPercent);
		b->IO("HorzButtonScrollPercent",&horzButtonScrollPercent);
		b->IO("SupportScrolling",&supportScrollBars);
		b->IO("ScrollBarThickness",&scrollBarThickness);
		b->IO("VertBarScrollPercent",&vertBarScrollPercent);
		b->IO("VertButtonScrollPercent",&vertButtonScrollPercent);
		b->IO("XScrollOffset",&xScrollOffset);
		b->IO("YScrollOffset",&yScrollOffset);
	}

	ContainerWidget::ContainerWidget(const Ogre::String& name) :
		ComponentWidget(name),
		mHScrollBar(0),
		mVScrollBar(0)
	{
	}

	ContainerWidget::~ContainerWidget()
	{
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			delete (*it);
	}

	void ContainerWidget::_determineVirtualSize()
	{
		mVirtualSize = mClientDimensions.size;

		Point tempPoint;
		Size tempSize;
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			tempPoint = (*it)->getPosition();
			tempSize = (*it)->getSize();

			if((tempPoint.x + tempSize.width) > mVirtualSize.width)
				mVirtualSize.width = (tempPoint.x + tempSize.width);
			if((tempPoint.y + tempSize.height) > mVirtualSize.height)
				mVirtualSize.height = (tempPoint.y + tempSize.height);
		}
	}

	void ContainerWidget::_initialize(WidgetDesc* d)
	{
		ComponentWidget::_initialize(d);

		mDesc = dynamic_cast<ContainerWidgetDesc*>(mWidgetDesc);
		ContainerWidgetDesc* cwd = dynamic_cast<ContainerWidgetDesc*>(d);

		mDesc->supportScrollBars = cwd->supportScrollBars;
		mDesc->scrollBarThickness = cwd->scrollBarThickness;
		mDesc->horzBarScrollPercent = cwd->horzBarScrollPercent;
		mDesc->horzButtonScrollPercent = cwd->horzButtonScrollPercent;
		mDesc->vertBarScrollPercent = cwd->vertBarScrollPercent;
		mDesc->vertButtonScrollPercent = cwd->vertButtonScrollPercent;

		if(mDesc->supportScrollBars)
		{
			HScrollBarDesc hd;
			hd.dimensions.size.height = mDesc->scrollBarThickness;
			// Do not anchor, as it interferes with dynamically resizing and positioning the scrollbars
			hd.horizontalAnchor = ANCHOR_HORIZONTAL_LEFT;
			hd.verticalAnchor = ANCHOR_VERTICAL_BOTTOM;
			hd.barScrollPercent = mDesc->horzBarScrollPercent;
			hd.buttonScrollPercent = mDesc->horzButtonScrollPercent;
			hd.visible = false;

			mHScrollBar = dynamic_cast<HScrollBar*>(mWidgetDesc->guiManager->createWidget("HScrollBar",hd));
			mHScrollBar->enableLiveSlider(&(mClientDimensions.size.width),&(mVirtualSize.width));
			mHScrollBar->addScrollBarEventHandler(QuickGUI::SCROLLBAR_EVENT_ON_SCROLLED,&ContainerWidget::onHorizontalScroll,this);
			addComponent(HSCROLLBAR,mHScrollBar);

			VScrollBarDesc vd;
			vd.dimensions.size.width = mDesc->scrollBarThickness;
			// Do not anchor, as it interferes with dynamically resizing and positioning the scrollbars
			vd.horizontalAnchor = ANCHOR_HORIZONTAL_RIGHT;
			vd.verticalAnchor = ANCHOR_VERTICAL_TOP;
			vd.barScrollPercent = mDesc->vertBarScrollPercent;
			vd.buttonScrollPercent = mDesc->vertButtonScrollPercent;
			vd.visible = false;

			mVScrollBar = dynamic_cast<VScrollBar*>(mWidgetDesc->guiManager->createWidget("VScrollBar",vd));
			mVScrollBar->enableLiveSlider(&(mClientDimensions.size.height),&(mVirtualSize.height));
			mVScrollBar->addScrollBarEventHandler(QuickGUI::SCROLLBAR_EVENT_ON_SCROLLED,&ContainerWidget::onVerticalScroll,this);
			addComponent(VSCROLLBAR,mVScrollBar);

			addWidgetEventHandler(WIDGET_EVENT_MOUSE_WHEEL,&ContainerWidget::onMouseWheel,this);
		}
	}

	void ContainerWidget::_setScrollX(float x)
	{
		if(!mDesc->supportScrollBars)
			return;

		mDesc->xScrollOffset = (x * mVirtualSize.width);

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->setScrollX(mDesc->xScrollOffset);
		}
	}

	void ContainerWidget::_setScrollY(float y)
	{
		if(!mDesc->supportScrollBars)
			return;

		mDesc->yScrollOffset = (y * mVirtualSize.height);

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->setScrollY(mDesc->yScrollOffset);
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

		if(mDesc->supportScrollBars)
		{
			// Register Event Handler for when child is resized or moved
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
		if( !mWidgetDesc->visible )
			return;

		Brush* brush = Brush::getSingletonPtr();

		// check and store clip region
		Rect prevClipRegion = brush->getClipRegion();
		if ( prevClipRegion.getIntersection(Rect(mTexturePosition,mWidgetDesc->dimensions.size)) == Rect::ZERO )
			return;

		// set clip region to dimensions
		brush->setClipRegion(Rect(mTexturePosition,mWidgetDesc->dimensions.size).getIntersection(prevClipRegion));

		// draw self
		onDraw();

		// draw components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->draw();

		// set clip region to client dimensions
		Rect clipRegion = mClientDimensions;
		clipRegion.translate(mTexturePosition);
		brush->setClipRegion(clipRegion);

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
		Widget* w = Widget::findWidgetAtPoint(p,ignoreDisabled);

		if(w == NULL)
			return NULL;

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			Widget* w = (*it).second->findWidgetAtPoint(p,ignoreDisabled);
			if(w != NULL)
				return w;
		}

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
				
		return this;
	}

	bool ContainerWidget::isContainerWidget()
	{
		return true;
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

		if(mDesc->supportScrollBars)
		{
			// Register Event Handler for when child is resized or moved
			w->removeEventHandler(WIDGET_EVENT_SIZE_CHANGED,this);
			w->removeEventHandler(WIDGET_EVENT_POSITION_CHANGED,this);

			_determineVirtualSize();
			_updateScrollBars();
		}
	}

	void ContainerWidget::serialize(SerialBase* b)
	{
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
				Widget* newWidget = WidgetFactoryManager::getSingleton().createWidget((*it)->getType(),(*it)->getID());

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

	void ContainerWidget::setHeight(float pixelHeight)
	{
		float previousHeight = mWidgetDesc->dimensions.size.height;

		// Modified code from Widget::setHeight, to incorporate ScrollBars
		{
			// Enforce max height
			if((mWidgetDesc->maxSize.height > 0) && (pixelHeight > mWidgetDesc->maxSize.height))
				pixelHeight = mWidgetDesc->maxSize.height;
			// Enforce min height
			else if((mWidgetDesc->minSize.height > 0) && (pixelHeight < mWidgetDesc->minSize.height))
				pixelHeight = mWidgetDesc->minSize.height;

			mWidgetDesc->dimensions.size.height = pixelHeight;

			redraw();

			updateClientDimensions();
		}

		float difference = mWidgetDesc->dimensions.size.height - previousHeight;

		// Handle anchoring for Components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			switch((*it).second->getVerticalAnchor())
			{
			case ANCHOR_VERTICAL_CENTER:
				{
					// Center vertically
					Point p = (*it).second->getPosition();
					p.y = (mWidgetDesc->dimensions.size.height / 2.0) - ((*it).second->getHeight() / 2.0);
					(*it).second->setPosition(p);
				}
				break;
			case ANCHOR_VERTICAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it).second->setHeight((*it).second->getHeight() + difference);
					// Center vertically
					Point p = (*it).second->getPosition();
					p.y = (mWidgetDesc->dimensions.size.height / 2.0) - ((*it).second->getHeight() / 2.0);
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
					p.y = (mWidgetDesc->dimensions.size.height / 2.0) - ((*it)->getHeight() / 2.0);
					(*it)->setPosition(p);
				}
				break;
			case ANCHOR_VERTICAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it)->setHeight((*it)->getHeight() + difference);
					// Center vertically
					Point p = (*it)->getPosition();
					p.y = (mWidgetDesc->dimensions.size.height / 2.0) - ((*it)->getHeight() / 2.0);
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
		mDesc->horzBarScrollPercent = percent;

		if(mHScrollBar)
			mHScrollBar->setBarScrollPercent(percent);
	}

	void ContainerWidget::setHorzButtonScrollPercent(float percent)
	{
		mDesc->horzButtonScrollPercent = percent;

		if(mHScrollBar)
			mHScrollBar->setButtonScrollPercent(percent);
	}

	void ContainerWidget::setSize(Size size)
	{
		Size previousSize = mWidgetDesc->dimensions.size;

		// Modified code from Widget::setSize, to incorporate ScrollBars
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

			redraw();

			updateClientDimensions();
		}

		// Get difference.  A larger size indicates a positive difference
		Size difference = mWidgetDesc->dimensions.size - previousSize;

		// Handle anchoring for Components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			switch((*it).second->getHorizontalAnchor())
			{
			case ANCHOR_HORIZONTAL_CENTER:
				{
					// Center vertically
					Point p = (*it).second->getPosition();
					p.x = (mWidgetDesc->dimensions.size.width / 2.0) - ((*it).second->getWidth() / 2.0);
					(*it).second->setPosition(p);
				}
				break;
			case ANCHOR_HORIZONTAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it).second->setWidth((*it).second->getWidth() + difference.width);
					// Center vertically
					Point p = (*it).second->getPosition();
					p.x = (mWidgetDesc->dimensions.size.width / 2.0) - ((*it).second->getWidth() / 2.0);
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
					p.y = (mWidgetDesc->dimensions.size.height / 2.0) - ((*it).second->getHeight() / 2.0);
					(*it).second->setPosition(p);
				}
				break;
			case ANCHOR_VERTICAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it).second->setHeight((*it).second->getHeight() + difference.height);
					// Center vertically
					Point p = (*it).second->getPosition();
					p.y = (mWidgetDesc->dimensions.size.height / 2.0) - ((*it).second->getHeight() / 2.0);
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
					p.x = (mWidgetDesc->dimensions.size.width / 2.0) - ((*it)->getWidth() / 2.0);
					(*it)->setPosition(p);
				}
				break;
			case ANCHOR_HORIZONTAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it)->setWidth((*it)->getWidth() + difference.width);
					// Center vertically
					Point p = (*it)->getPosition();
					p.x = (mWidgetDesc->dimensions.size.width / 2.0) - ((*it)->getWidth() / 2.0);
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
					p.y = (mWidgetDesc->dimensions.size.height / 2.0) - ((*it)->getHeight() / 2.0);
					(*it)->setPosition(p);
				}
				break;
			case ANCHOR_VERTICAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it)->setHeight((*it)->getHeight() + difference.height);
					// Center vertically
					Point p = (*it)->getPosition();
					p.y = (mWidgetDesc->dimensions.size.height / 2.0) - ((*it)->getHeight() / 2.0);
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

		mWidgetDesc->skinTypeName = type;

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);

		if(mHScrollBar)
			mHScrollBar->setPosition(Point(se->getBorderThickness(BORDER_LEFT),mWidgetDesc->dimensions.size.height - se->getBorderThickness(BORDER_BOTTOM) - mDesc->scrollBarThickness));

		if(mVScrollBar)
			mVScrollBar->setPosition(Point(mWidgetDesc->dimensions.size.width - se->getBorderThickness(BORDER_RIGHT) - mVScrollBar->getWidth(),se->getBorderThickness(BORDER_TOP)));

		// Initially set dimensions of ScrollBars and update client dimensions
		updateClientDimensions();

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->setSkinType(mSkinType->getComponentType((*it).first)->typeName);

		WidgetEventArgs wea(this);
		fireWidgetEvent(WIDGET_EVENT_SKIN_CHANGED,wea);

		// Determine Virtual Size.  This can cause ScrollBars to be shown and hidden.
		_determineVirtualSize();
		// Check ScrollBar visibility, update client dimensions if needbe, and re-determine virtual size
		_updateScrollBars();
	}

	bool ContainerWidget::supportsScrolling()
	{
		return mDesc->supportScrollBars;
	}

	void ContainerWidget::setVertBarScrollPercent(float percent)
	{
		mDesc->vertBarScrollPercent = percent;

		if(mVScrollBar)
			mVScrollBar->setBarScrollPercent(percent);
	}

	void ContainerWidget::setVertButtonScrollPercent(float percent)
	{
		mDesc->vertButtonScrollPercent = percent;

		if(mVScrollBar)
			mVScrollBar->setButtonScrollPercent(percent);
	}

	void ContainerWidget::setWidth(float pixelWidth)
	{
		float previousWidth = mWidgetDesc->dimensions.size.width;

		// Modified code from Widget::setWidth, to incorporate ScrollBars
		{
			// Enforce max width
			if((mWidgetDesc->maxSize.width > 0) && (pixelWidth > mWidgetDesc->maxSize.width))
				pixelWidth = mWidgetDesc->maxSize.width;
			// Enforce min width
			else if((mWidgetDesc->minSize.width > 0) && (pixelWidth < mWidgetDesc->minSize.width))
				pixelWidth = mWidgetDesc->minSize.width;

			mWidgetDesc->dimensions.size.width = pixelWidth;

			redraw();

			updateClientDimensions();
		}

		float difference = mWidgetDesc->dimensions.size.width - previousWidth;

		// Handle anchoring for Components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			switch((*it).second->getHorizontalAnchor())
			{
			case ANCHOR_HORIZONTAL_CENTER:
				{
					// Center vertically
					Point p = (*it).second->getPosition();
					p.x = (mWidgetDesc->dimensions.size.width / 2.0) - ((*it).second->getWidth() / 2.0);
					(*it).second->setPosition(p);
				}
				break;
			case ANCHOR_HORIZONTAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it).second->setWidth((*it).second->getWidth() + difference);
					// Center vertically
					Point p = (*it).second->getPosition();
					p.x = (mWidgetDesc->dimensions.size.width / 2.0) - ((*it).second->getWidth() / 2.0);
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
					p.x = (mWidgetDesc->dimensions.size.width / 2.0) - ((*it)->getWidth() / 2.0);
					(*it)->setPosition(p);
				}
				break;
			case ANCHOR_HORIZONTAL_CENTER_DYNAMIC:
				{
					// grow or shrink the widget according to new dimensions
					(*it)->setWidth((*it)->getWidth() + difference);
					// Center vertically
					Point p = (*it)->getPosition();
					p.x = (mWidgetDesc->dimensions.size.width / 2.0) - ((*it)->getWidth() / 2.0);
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
		mClientDimensions.size = mWidgetDesc->dimensions.size;

		if(mSkinType != NULL)
		{
			// Default
			SkinElement* se = mSkinType->getSkinElement(mSkinElementName);
			mClientDimensions.position.x = se->getBorderThickness(BORDER_LEFT);
			mClientDimensions.position.y = se->getBorderThickness(BORDER_TOP);
			mClientDimensions.size.width = mWidgetDesc->dimensions.size.width - (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT));
			mClientDimensions.size.height = mWidgetDesc->dimensions.size.height - (se->getBorderThickness(BORDER_TOP) + se->getBorderThickness(BORDER_BOTTOM));

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
