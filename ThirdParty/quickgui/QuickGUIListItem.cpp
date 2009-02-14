#include "QuickGUIListItem.h"
#include "QuickGUIList.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUIComboBox.h"

namespace QuickGUI
{
	const Ogre::String ListItem::DEFAULT = "default";
	const Ogre::String ListItem::OVER = "over";
	const Ogre::String ListItem::SELECTED = "selected";

	ListItemDesc::ListItemDesc(const Ogre::String& id) :
		ContainerWidgetDesc(id)
	{
		resetToDefault();
	}

	void ListItemDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		listitem_index = -1;
		listitem_selected = false;
		containerwidget_supportScrollBars = false;
		widget_transparencyPicking = false;
	}

	void ListItemDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		b->IO("Index",&listitem_index);
		b->IO("Selected",&listitem_selected);
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

		setIndex(lid->listitem_index);
		setSelected(lid->listitem_selected);
	}

	int ListItem::getIndex()
	{
		return mDesc->listitem_index;
	}

	bool ListItem::getSelected()
	{
		return mDesc->listitem_selected;
	}

	void ListItem::onMouseButtonDown(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			// List Items can belong to Lists or ComboBox
			if(mParentWidget->getClass() == "List")
				dynamic_cast<List*>(mParentWidget)->selectItem(mea);
			else if(mParentWidget->getClass() == "MenuPanel")
			{
				MenuPanel* lp = dynamic_cast<MenuPanel*>(mParentWidget);
				if(lp->getOwner() != NULL)
				{
					if(lp->getOwner()->getClass() == "ComboBox")
						dynamic_cast<ComboBox*>(lp->getOwner())->selectItem(mea);
				}
			}
		}
	}

	void ListItem::onMouseEnter(const EventArgs& args)
	{
		if(!mDesc->listitem_selected)
		{
			mSkinElementName = OVER;
			redraw();
		}
	}

	void ListItem::onMouseLeave(const EventArgs& args)
	{
		if(!mDesc->listitem_selected)
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

	void ListItem::setIndex(unsigned int index)
	{
		mDesc->listitem_index = index;
	}
	
	void ListItem::setPosition(const Point& position)
	{
		ContainerWidget::setPosition(position);
	}

	void ListItem::setSelected(bool selected)
	{
		mDesc->listitem_selected = selected;
		
		if(mDesc->listitem_selected)
			mSkinElementName = SELECTED;
		else
			mSkinElementName = DEFAULT;

		redraw();
	}
	
	void ListItem::setSize(const Size& size)
	{
		ContainerWidget::setSize(size);
	}

	void ListItem::setSkinType(const Ogre::String type)
	{
		ContainerWidget::setSkinType(type);
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
