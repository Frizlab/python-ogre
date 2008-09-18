#include "QuickGUIToolBarItem.h"
#include "QuickGUIToolBar.h"

namespace QuickGUI
{
	ToolBarItemDesc::ToolBarItemDesc() :
		ContainerWidgetDesc()
	{
		supportScrollBars = false;
	}

	ToolBarItem::ToolBarItem(const Ogre::String& name) :
		ContainerWidget(name)
	{
	}

	ToolBarItem::~ToolBarItem()
	{
	}

	void ToolBarItem::_initialize(WidgetDesc* d)
	{
		ContainerWidget::_initialize(d);

		dynamic_cast<ToolBarItemDesc*>(mWidgetDesc)->toolBar = dynamic_cast<ToolBarItemDesc*>(d)->toolBar;

		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&ToolBarItem::onMouseButtonDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_UP,&ToolBarItem::onMouseButtonUp,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&ToolBarItem::onMouseEnter,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&ToolBarItem::onMouseLeave,this);
	}

	bool ToolBarItem::isToolBarItem()
	{
		return true;
	}

	void ToolBarItem::notifyToolBarParent(ToolBar* b)
	{
		dynamic_cast<ToolBarItemDesc*>(mWidgetDesc)->toolBar = b;
	}

	void ToolBarItem::onMouseButtonDown(const EventArgs& args)
	{
	}

	void ToolBarItem::onMouseButtonUp(const EventArgs& args)
	{
	}

	void ToolBarItem::onMouseEnter(const EventArgs& args)
	{
	}

	void ToolBarItem::onMouseLeave(const EventArgs& args)
	{
	}

	void ToolBarItem::setDimensions(const Rect& r)
	{
		ContainerWidget::setDimensions(r);
	}

	void ToolBarItem::setPosition(const Point& position)
	{
		ContainerWidget::setPosition(position);
	}

	void ToolBarItem::setSize(const Size& size)
	{
		ContainerWidget::setSize(size);
	}
}
