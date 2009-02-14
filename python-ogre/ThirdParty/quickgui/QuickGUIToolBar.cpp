#include "QuickGUIToolBar.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String ToolBar::BACKGROUND = "background";

	void ToolBar::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ToolBar");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ToolBar",d);
	}

	ToolBarDesc::ToolBarDesc(const Ogre::String& id) :
		ContainerWidgetDesc(id)
	{
		resetToDefault();
	}

	void ToolBarDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		containerwidget_supportScrollBars = false;
		toolbar_itemLayout = TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE;
	}

	void ToolBarDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		b->IO("ItemLayout",&toolbar_itemLayout);
	}

	ToolBar::ToolBar(const Ogre::String& name) :
		ContainerWidget(name),
		mCurrentState(TOOLBAR_STATE_DEFAULT),
		mMenuOpened(false)
	{
		mSkinElementName = BACKGROUND;
	}

	ToolBar::~ToolBar()
	{
	}

	void ToolBar::_initialize(WidgetDesc* d)
	{
		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<ToolBarDesc*>(mWidgetDesc);

		ToolBarDesc* tbd = dynamic_cast<ToolBarDesc*>(d);

		mDesc->toolbar_itemLayout = tbd->toolbar_itemLayout;

		if(mWidgetDesc->widget_dimensions.size.width > mWidgetDesc->widget_dimensions.size.height)
			mOrientation = TOOLBAR_ORIENTATION_HORIZONTAL;
		else
			mOrientation = TOOLBAR_ORIENTATION_VERTICAL;

		setSkinType(d->widget_skinTypeName);
	}

	void ToolBar::addChild(Widget* w)
	{
		if(!w->isToolBarItem())
			throw Exception(Exception::ERR_INVALID_CHILD,"Cannot add a non-ToolBarItem to a ToolBar!","ToolBar::addChild");

		ContainerWidget::addChild(w);

		// Update link to parent ToolBar
		dynamic_cast<ToolBarItem*>(w)->notifyToolBarParent(this);

		if(w->getClass() == "Menu")
		{
			Menu* m = dynamic_cast<Menu*>(w);
			// For menu's that are directly connected to a ToolBar, we set their owner menu reference to themselves.
			m->notifyMenuParent(m);
			mMenus.push_back(m);
		}
	}

	void ToolBar::closeMenus()
	{
		for(std::vector<Menu*>::iterator it = mMenus.begin(); it != mMenus.end(); ++it)
			(*it)->closeMenu();

		mMenuOpened = false;
	}

	Menu* ToolBar::createMenu(MenuDesc* d)
	{
		d->toolBar = this;

		SkinElement* se = mSkinType->getSkinElement(mSkinElementName);

		if(mOrientation == TOOLBAR_ORIENTATION_HORIZONTAL)
		{
			d->widget_verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
			d->widget_dimensions.size = Size(d->textDesc.getTextWidth() + (se->getBorderThickness(BORDER_LEFT) + se->getBorderThickness(BORDER_RIGHT)),mDesc->widget_dimensions.size.height);
		}
		else
		{
			d->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
			d->widget_dimensions.size = Size(mDesc->widget_dimensions.size.width,d->textDesc.getTextHeight() + (se->getBorderThickness(BORDER_TOP) + se->getBorderThickness(BORDER_BOTTOM)));
		}

		Menu* newMenu = dynamic_cast<Menu*>(Widget::create("Menu",d));

		newMenu->setPosition(getNextAvailableItemPosition(newMenu));

		// Its important we add the child after getNextAvailableItemPosition,
		// as it queries children to find the next position.
		addChild(newMenu);

		return newMenu;
	}

	ToolBarItem* ToolBar::createToolBarItem(const Ogre::String& className, ToolBarItemDesc* d)
	{
		if(className == "Menu")
			return createMenu(dynamic_cast<MenuDesc*>(d));

		d->toolBar = this;

		if(mOrientation == TOOLBAR_ORIENTATION_HORIZONTAL)
			d->widget_verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
		else
			d->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;

		ToolBarItem* newItem = dynamic_cast<ToolBarItem*>(Widget::create(className,d));

		newItem->setPosition(getNextAvailableItemPosition(newItem));

		// Its important we add the child after getNextAvailableItemPosition,
		// as it queries children to find the next position.
		addChild(newItem);

		return newItem;
	}

	Ogre::String ToolBar::getClass()
	{
		return "ToolBar";
	}

	int ToolBar::getNumberOfItems()
	{
		return static_cast<int>(mChildren.size());
	}

	ToolBarOrientation ToolBar::getOrientation()
	{
		return mOrientation;
	}

	Point ToolBar::getNextAvailableItemPosition(ToolBarItem* newlyAddedItem)
	{
		if(mDesc->toolbar_itemLayout == TOOLBAR_ITEM_LAYOUT_NEGATIVE_TO_POSITIVE)
		{
			if(mOrientation == TOOLBAR_ORIENTATION_HORIZONTAL)
			{
				float xPos = 0;
				if(!mChildren.empty())
					xPos = mChildren.back()->getPosition().x + mChildren.back()->getSize().width;
				return Point(xPos,0);
			}
			else
			{
				float yPos = 0;
				if(!mChildren.empty())
					yPos = mChildren.back()->getPosition().y + mChildren.back()->getSize().height;
				return Point(0,yPos);
			}
		}
		else
		{
			if(mOrientation == TOOLBAR_ORIENTATION_HORIZONTAL)
			{
				float xPos = mDesc->widget_dimensions.size.width - newlyAddedItem->getSize().width;
				if(!mChildren.empty())
					xPos = mChildren.back()->getPosition().x - mChildren.back()->getSize().width;
				return Point(xPos,0);
			}
			else
			{
				float yPos = mDesc->widget_dimensions.size.height - newlyAddedItem->getSize().height;
				if(!mChildren.empty())
					yPos = mChildren.back()->getPosition().y - mChildren.back()->getSize().height;
				return Point(0,yPos);
			}
		}
	}

	bool ToolBar::isMenuOpened()
	{
		return mMenuOpened;
	}

	void ToolBar::onDraw()
	{
		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);
			
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));
	}

	void ToolBar::openMenu(Menu* m)
	{
		bool found = false;
		for(std::vector<Menu*>::iterator it = mMenus.begin(); it != mMenus.end(); ++it)
		{
			if((*it)->hasSubMenu(m))
			{
				(*it)->openMenu();	
				(*it)->openSubMenu(m);
				found = true;
				mMenuOpened = true;
			}
			else
				(*it)->closeMenu();
		}

		if(!found)
			throw Exception(Exception::ERR_INVALID_CHILD,"Given Menu is not a Child of this ToolBar!","ToolBar::openMenu");
	}
}
