#include "QuickGUIContextMenu.h"
#include "QuickGUISheet.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIFactoryManager.h"

namespace QuickGUI
{
	void ContextMenu::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ContextMenu");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(TITLEBAR);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ContextMenu",d);
	}

	ContextMenuDesc::ContextMenuDesc(const Ogre::String& id) :
		WindowDesc(id)
	{
		resetToDefault();
	}

	void ContextMenuDesc::resetToDefault()
	{
		WindowDesc::resetToDefault();

		contextmenu_itemHeight = 25;
		contextmenu_maxMenuHeight = 0;
		contextmenu_subMenuOverlap = 2;

		widget_dimensions.size = Size(100,25);
		widget_resizeFromBottom = false;
		widget_resizeFromLeft = false;
		widget_resizeFromRight = false;
		widget_resizeFromTop = false;
		widget_visible = false;
	}

	void ContextMenuDesc::serialize(SerialBase* b)
	{
		WindowDesc::serialize(b);

		b->IO("ItemHeight",&contextmenu_itemHeight);
		b->IO("MaxMenuHeight",&contextmenu_maxMenuHeight);
		b->IO("SubMenuOverlap",&contextmenu_subMenuOverlap);
	}

	ContextMenu::ContextMenu(const Ogre::String& name) :
		Window(name),
		mAutoNameCounter(0)
	{
		mSkinElementName = BACKGROUND;

		addWidgetEventHandler(WIDGET_EVENT_CLIENTSIZE_CHANGED,&ContextMenu::onClientSizeChanged,this);
		addWindowEventHandler(WINDOW_EVENT_FOCUS_LOST,&ContextMenu::onLoseFocus,this);
		addWidgetEventHandler(WIDGET_EVENT_POSITION_CHANGED,&ContextMenu::onPositionChanged,this);
		addWidgetEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,&ContextMenu::onVisibleChanged,this);
	}

	ContextMenu::~ContextMenu()
	{
	}

	float ContextMenu::_getNextAvailableYPosition()
	{
		float maxY = 0;
		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
		{
			if(((*it)->getPosition().y + (*it)->getSize().height) > maxY)
				maxY = ((*it)->getPosition().y + (*it)->getSize().height);
		}

		return maxY;
	}

	void ContextMenu::_initialize(WidgetDesc* d)
	{
		d->widget_visible = false;
		d->widget_resizeFromBottom = false;
		d->widget_resizeFromLeft = false;
		d->widget_resizeFromRight = false;
		d->widget_resizeFromTop = false;

		Window::_initialize(d);

		setSkinType(d->widget_skinTypeName);

		mDesc = dynamic_cast<ContextMenuDesc*>(mWidgetDesc);

		ContextMenuDesc* cmd = dynamic_cast<ContextMenuDesc*>(d);

		if(cmd->window_titleBar)
			Window::setHeight(
				mTitleBar->getHeight() + 
				mSkinType->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_TOP) +
				mSkinType->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_BOTTOM));
		else
			Window::setHeight(
				mSkinType->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_TOP) +
				mSkinType->getSkinElement(mSkinElementName)->getBorderThickness(BORDER_BOTTOM));

		// Copy over all properties from desc param
		mDesc->contextmenu_itemHeight = cmd->contextmenu_itemHeight;
		mDesc->contextmenu_maxMenuHeight = cmd->contextmenu_maxMenuHeight;
		mDesc->contextmenu_subMenuOverlap = cmd->contextmenu_subMenuOverlap;
	}

	void ContextMenu::_setGUIManager(GUIManager* gm)
	{
		Window::_setGUIManager(gm);

		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
			(*it)->_setGUIManager(gm);
	}

	void ContextMenu::_setSheet(Sheet* sheet)
	{
		Window::_setSheet(sheet);

		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
			(*it)->_setSheet(sheet);
	}

	void ContextMenu::_updateItemPositions()
	{
		int y = 0;
		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
		{
			(*it)->setPosition(Point(0,y));
			y += mDesc->contextmenu_itemHeight;
		}
	}

	Ogre::String ContextMenu::getClass()
	{
		return "ContextMenu";
	}

	void ContextMenu::addChild(Widget* w)
	{
		if(!w->isMenuItem())
			throw Exception(Exception::ERR_INVALID_CHILD,"Cannot add non-MenuItem to a menu!","ContextMenu::addChild");

		ContainerWidget::addChild(w);

		MenuItem* menuItem = dynamic_cast<MenuItem*>(w);
		// Update link to parent ContextMenu
		menuItem->notifyContextMenuParent(this);

		mMenuItems.push_back(menuItem);

		if(w->getClass() == "Menu")
			mSubMenus.push_back(dynamic_cast<Menu*>(w));

		Window::setHeight(mWidgetDesc->widget_dimensions.size.height + mDesc->contextmenu_itemHeight);
	}

	void ContextMenu::closeMenu()
	{
		// Close all SubMenus
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
			(*it)->closeMenu();

		setVisible(false);
	}

	void ContextMenu::closeSubMenus()
	{
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
		{
			(*it)->closeMenu();
		}
	}

	MenuItem* ContextMenu::createMenuItem(const Ogre::String& className, MenuItemDesc* d)
	{
		if(className == "Menu")
			return createSubMenu(dynamic_cast<MenuDesc*>(d));

		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".AutoName.MenuItem." + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		d->widget_verticalAnchor = ANCHOR_VERTICAL_TOP;
		d->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;

		// Determine position of MenuItem
		d->widget_dimensions.position.y = _getNextAvailableYPosition();
		d->widget_dimensions.size.width = mClientDimensions.size.width;
		d->widget_dimensions.size.height = mDesc->contextmenu_itemHeight;
		
		MenuItem* newMenuItem = dynamic_cast<MenuItem*>(Widget::create(className,d));
		addChild(newMenuItem);

		return newMenuItem;
	}

	MenuLabel* ContextMenu::createMenuLabel(MenuLabelDesc* d)
	{
		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".AutoName.MenuItem." + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		d->widget_verticalAnchor = ANCHOR_VERTICAL_TOP;
		d->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;

		// Determine position of MenuItem
		d->widget_dimensions.position.y = _getNextAvailableYPosition();
		d->widget_dimensions.size.width = mClientDimensions.size.width;
		d->widget_dimensions.size.height = mDesc->contextmenu_itemHeight;

		MenuLabel* newMenuLabel = dynamic_cast<MenuLabel*>(Widget::create("MenuLabel",d));
		addChild(newMenuLabel);

		return newMenuLabel;
	}

	Menu* ContextMenu::createSubMenu(MenuDesc* d)
	{
		if(d->widget_name == "")
		{
			d->widget_name = getName() + ".AutoName.MenuItem." + Ogre::StringConverter::toString(mAutoNameCounter);
			++mAutoNameCounter;
		}

		d->widget_verticalAnchor = ANCHOR_VERTICAL_TOP;
		d->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;

		// Determine position of Menu
		d->widget_dimensions.position.y = _getNextAvailableYPosition();
		d->widget_dimensions.size.width = mClientDimensions.size.width;
		d->widget_dimensions.size.height = mDesc->contextmenu_itemHeight;

		Menu* newMenu = dynamic_cast<Menu*>(Widget::create("Menu",d));

		addChild(newMenu);

		return newMenu;
	}

	float ContextMenu::getItemHeight()
	{
		return mDesc->contextmenu_itemHeight;
	}

	int ContextMenu::getNumberOfItems()
	{
		return static_cast<int>(mMenuItems.size());
	}

	bool ContextMenu::hasSubMenu(Menu* m)
	{
		// recurse submenus to see if menu is a submenu
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
		{
			if( (*it)->hasSubMenu(m) )
				return true;
		}

		return false;
	}

	void ContextMenu::hide()
	{
		closeMenu();
	}

	void ContextMenu::onClientSizeChanged(const EventArgs& args)
	{
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->setWidth(mClientDimensions.size.width);
		}
	}

	void ContextMenu::onLoseFocus(const EventArgs& args)
	{
		if(findWidget(mDesc->sheet->getWindowInFocus()->getName()) == NULL)
			hide();
	}

	void ContextMenu::onPositionChanged(const EventArgs& args)
	{
		closeSubMenus();
	}

	void ContextMenu::onVisibleChanged(const EventArgs& args)
	{
		if(mDesc->widget_visible)
			mDesc->sheet->focusWindow(this);
		else
			// Make sure SubMenus are closed. (ie pressing 'X' button while menus are visible)
			closeSubMenus();
	}

	void ContextMenu::openMenu(const Point& position)
	{
		// Close all SubMenus
		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
			(*it)->closeMenu();

		mWidgetDesc->widget_dimensions.position = position;

		if(position.y < 0)
			mWidgetDesc->widget_dimensions.position.y = 0;
		else if((position.y + mWidgetDesc->widget_dimensions.size.height) > mWidgetDesc->sheet->getHeight())
			mWidgetDesc->widget_dimensions.position.y = mWidgetDesc->sheet->getHeight() - mWidgetDesc->widget_dimensions.size.height;

		if(position.x < 0)
			mWidgetDesc->widget_dimensions.position.x = 0;
		else if((position.x + mWidgetDesc->widget_dimensions.size.width) > mWidgetDesc->sheet->getWidth())
			mWidgetDesc->widget_dimensions.position.x = mWidgetDesc->sheet->getWidth() - mWidgetDesc->widget_dimensions.size.width;

		setVisible(true);

		// If this menu is already visible, setVisible won't toggle redrawing
		redraw();
	}

	void ContextMenu::openSubMenu(Menu* m)
	{
		openMenu(mWidgetDesc->widget_dimensions.position);

		for(std::vector<Menu*>::iterator it = mSubMenus.begin(); it != mSubMenus.end(); ++it)
		{
			if((*it)->hasSubMenu(m))
				(*it)->openSubMenu(m);
			else
				(*it)->closeMenu();
		}
	}

	void ContextMenu::setDimensions(const Rect& r)
	{
		Window::setPosition(r.position);
		Window::setWidth(r.size.width);
	}

	void ContextMenu::setHeight(float pixelHeight)
	{
		Window::setHeight(pixelHeight);
	}

	void ContextMenu::setItemHeight(float height)
	{
		mDesc->contextmenu_itemHeight = height;

		_updateItemPositions();
	}

	void ContextMenu::setSize(const Size& size)
	{
		Window::setWidth(size.width);
	}

	void ContextMenu::show(const Point& position)
	{
		openMenu(position);
	}
}
