#include "QuickGUIListItem.h"
#include "QuickGUIList.h"

namespace QuickGUI
{
	const Ogre::String ListItem::DEFAULT = "default";
	const Ogre::String ListItem::OVER = "over";
	const Ogre::String ListItem::SELECTED = "selected";

	ListItemDesc::ListItemDesc() :
		ContainerWidgetDesc()
	{
		selected = false;
		supportScrollBars = false;
		transparencyPicking = false;
	}

	void ListItemDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		b->IO("Selected",&selected);
	}

	ListItem::ListItem(const Ogre::String& name) :
		ContainerWidget(name)
	{
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&ListItem::onMouseButtonDown,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&ListItem::onMouseEnter,this);
		addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&ListItem::onMouseLeave,this);
	}

	ListItem::~ListItem()
	{
	}

	void ListItem::_initialize(WidgetDesc* d)
	{
		ContainerWidget::_initialize(d);

		ListItemDesc* lid = dynamic_cast<ListItemDesc*>(d);
		mDesc = dynamic_cast<ListItemDesc*>(mWidgetDesc);

		setSelected(lid->selected);
	}

	bool ListItem::getSelected()
	{
		return mDesc->selected;
	}

	void ListItem::onMouseButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			dynamic_cast<List*>(mParentWidget)->selectItem(mea);
		}
	}

	void ListItem::onMouseEnter(const EventArgs& args)
	{
		if(!mDesc->selected)
		{
			mSkinElementName = OVER;
			redraw();
		}
	}

	void ListItem::onMouseLeave(const EventArgs& args)
	{
		if(!mDesc->selected)
		{
			mSkinElementName = DEFAULT;
			redraw();
		}
	}

	void ListItem::setDimensions(const Rect& r)
	{
		ContainerWidget::setDimensions(r);
	}

	void ListItem::setHeight(float pixelHeight)
	{
		ContainerWidget::setHeight(pixelHeight);
	}

	void ListItem::setHorizontalAnchor(HorizontalAnchor a)
	{
		ContainerWidget::setHorizontalAnchor(a);
	}
	
	void ListItem::setPosition(const Point& position)
	{
		ContainerWidget::setPosition(position);
	}

	void ListItem::setSelected(bool selected)
	{
		mDesc->selected = selected;
		
		if(mDesc->selected)
			mSkinElementName = SELECTED;
		else
			mSkinElementName = DEFAULT;

		redraw();
	}
	
	void ListItem::setSize(const Size& size)
	{
		ContainerWidget::setSize(size);
	}

	void ListItem::setVerticalAnchor(VerticalAnchor a)
	{
		ContainerWidget::setVerticalAnchor(a);
	}

	void ListItem::setWidth(float pixelWidth)
	{
		ContainerWidget::setWidth(pixelWidth);
	}
}
