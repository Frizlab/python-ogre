/*
-----------------------------------------------------------------------------
This source file is part of QuickGUI
For the latest info, see http://www.ogre3d.org/addonforums/viewforum.php?f=13

Copyright (c) 2009 Stormsong Entertainment

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

(http://opensource.org/licenses/mit-license.php)
-----------------------------------------------------------------------------
*/

#include "QuickGUIListItem.h"
#include "QuickGUIList.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIRoot.h"
#include "QuickGUIManager.h"
#include "QuickGUISheet.h"
#include "QuickGUIDescManager.h"

namespace QuickGUI
{
	const Ogre::String ListItem::DEFAULT = "default";
	const Ogre::String ListItem::OVER = "over";
	const Ogre::String ListItem::SELECTED = "selected";

	ListItemDesc::ListItemDesc() :
		ContainerWidgetDesc()
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
		// Retrieve default values to supply to the serial reader/writer.
		// The reader uses the default value if the given property does not exist.
		// The writer does not write out the given property if it has the same value as the default value.
		ListItemDesc* defaultValues = DescManager::getSingleton().createDesc<ListItemDesc>(getClass(),"temp");
		defaultValues->resetToDefault();

		b->IO("Index",		&listitem_index,	defaultValues->listitem_index);
		b->IO("Selected",	&listitem_selected, defaultValues->listitem_selected);

		DescManager::getSingleton().destroyDesc(defaultValues);
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

		setSkinType(d->widget_skinTypeName);
		setIndex(lid->listitem_index);
		setSelected(lid->listitem_selected);
	}

	void ListItem::destroy()
	{
		if(isComponentOfAWidget())
			return;

		if(mParentWidget != NULL)
		{
			Ogre::String className = mParentWidget->getClass();
			if(className == "List")
				dynamic_cast<List*>(mParentWidget)->destroyItem(this);
			else if(className == "MenuPanel")
			{
				Widget* owner = dynamic_cast<MenuPanel*>(mParentWidget)->getOwner();
				Ogre::String ownerClassName = owner->getClass();
				if(ownerClassName == "ComboBox")
					dynamic_cast<ComboBox*>(owner)->destroyItem(this);
			}
		}
		else
		{
			if(mWidgetDesc->sheet != NULL)
				mWidgetDesc->sheet->mFreeList.push_back(this);
			else
				Root::getSingleton().mGUIManagers.begin()->second->mFreeList.push_back(this);
		}
	}

	bool ListItem::getEnabled()
	{
		return ContainerWidget::getEnabled();
	}

	int ListItem::getIndex()
	{
		return mDesc->listitem_index;
	}

	bool ListItem::getSelected()
	{
		return mDesc->listitem_selected;
	}

	bool ListItem::isPositionManaged()
	{
		return true;
	}

	bool ListItem::isSizeManaged()
	{
		return true;
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
			mSkinElement = mSkinType->getSkinElement(OVER);
			redraw();
		}
	}

	void ListItem::onMouseLeave(const EventArgs& args)
	{
		if(!mDesc->listitem_selected)
		{
			mSkinElement = mSkinType->getSkinElement(DEFAULT);
			redraw();
		}
	}

	void ListItem::setEnabled(bool enabled)
	{
		ContainerWidget::setEnabled(enabled);
	}

	void ListItem::setIndex(unsigned int index)
	{
		mDesc->listitem_index = index;
	}

	void ListItem::setSelected(bool selected)
	{
		mDesc->listitem_selected = selected;
		
		if(mSkinElement != NULL)
		{
			if(mDesc->listitem_selected)
				mSkinElement = mSkinType->getSkinElement(SELECTED);
			else
				mSkinElement = mSkinType->getSkinElement(DEFAULT);

			redraw();
		}
	}

	void ListItem::updateSkinElement()
	{
		mSkinElement = NULL;
		if(mSkinType != NULL)
			mSkinElement = mSkinType->getSkinElement(DEFAULT);
	}
}
