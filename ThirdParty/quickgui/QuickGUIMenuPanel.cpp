#include "QuickGUIMenuPanel.h"
#include "QuickGUIListItem.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	const Ogre::String MenuPanel::BACKGROUND = "background";

	void MenuPanel::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("MenuPanel");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("MenuPanel",d);
	}

	MenuPanelDesc::MenuPanelDesc(const Ogre::String& id) :
		WindowDesc(id)
	{
		resetToDefault();
	}

	void MenuPanelDesc::resetToDefault()
	{
		WindowDesc::resetToDefault();

		menupanel_owner = NULL;
		menupanel_maxHeight = 0;

		containerwidget_supportScrollBars = true;

		window_titleBar = false;

		WindowDesc::window_titleBar = false;

		widget_resizable = false;
	}

	MenuPanel::MenuPanel(const Ogre::String& name) :
		Window(name),
		mDesc(NULL)
	{
		mSkinElementName = BACKGROUND;

		addWidgetEventHandler(WIDGET_EVENT_CLIENTSIZE_CHANGED,&MenuPanel::onClientSizeChanged,this);
		addWindowEventHandler(WINDOW_EVENT_FOCUS_LOST,&MenuPanel::onLoseFocus,this);
		addWidgetEventHandler(WIDGET_EVENT_VISIBLE_CHANGED,&MenuPanel::onVisibleChanged,this);
	}

	MenuPanel::~MenuPanel()
	{
	}

	void MenuPanel::_adjustHeight()
	{
		// Update MenuPanel height - for now, just expand to fit all items
		float totalItemHeight = 0;
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			float y = (*it)->getPosition().y + (*it)->getHeight();
			if(y > totalItemHeight)
				totalItemHeight = y;
		}

		// Adjust Height
		SkinElement* background = mSkinType->getSkinElement(BACKGROUND);
		float desiredHeight = totalItemHeight + (background->getBorderThickness(BORDER_TOP) + background->getBorderThickness(BORDER_BOTTOM));

		if(mDesc->menupanel_maxHeight > 0)
		{
			if(desiredHeight < mDesc->menupanel_maxHeight)
				setHeight(desiredHeight);
			else
				setHeight(mDesc->menupanel_maxHeight);
		}
		else
			setHeight(desiredHeight);
	}

	void MenuPanel::_initialize(WidgetDesc* d)
	{
		mDesc = dynamic_cast<MenuPanelDesc*>(mWidgetDesc);

		Window::_initialize(d);

		MenuPanelDesc* lpd = dynamic_cast<MenuPanelDesc*>(d);

		mDesc->menupanel_owner = lpd->menupanel_owner;
		mDesc->menupanel_maxHeight = lpd->menupanel_maxHeight;

		setSkinType(d->widget_skinTypeName);
	}

	void MenuPanel::addWidget(Widget* w)
	{
		addChild(w);

		_adjustHeight();
	}

	void MenuPanel::clearWidgets()
	{
		//mListItems.clear();

		mDesc->widget_dimensions.size.height = 1;

		redraw();
	}

	Ogre::String MenuPanel::getClass()
	{
		return "MenuPanel";
	}

	Widget* MenuPanel::getOwner()
	{
		return mDesc->menupanel_owner;
	}

	void MenuPanel::onDraw()
	{
		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		brush->drawSkinElement(Rect(Point::ZERO,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));
	}

	void MenuPanel::onClientSizeChanged(const EventArgs& args)
	{
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			(*it)->setWidth(mClientDimensions.size.width);
		}
	}

	void MenuPanel::onLoseFocus(const EventArgs& args)
	{
		if(mDesc->menupanel_owner != NULL)
		{
			Ogre::String ownerClass = mDesc->menupanel_owner->getClass();
			if(ownerClass == "Menu")
			{
				ToolBar* tb = dynamic_cast<Menu*>(mDesc->menupanel_owner)->getToolBar();
				if(tb->findWidget(mDesc->guiManager->getLastClickedWidget()->getName()) == NULL)
					tb->closeMenus();
			}
			else if(ownerClass == "ComboBox")
			{
				ComboBox* cb = dynamic_cast<ComboBox*>(mDesc->menupanel_owner);

				// If we click a widget other than the combobox, hide the list.
				// If we did click the combobox, the combobox will hide the list automatically.
				if((mDesc->guiManager->getLastClickedWidget() != NULL) && (cb->findWidget(mDesc->guiManager->getLastClickedWidget()->getName()) == NULL))
					cb->hideDropDownList();
			}
		}
	}

	void MenuPanel::onVisibleChanged(const EventArgs& args)
	{
		if(mDesc->widget_visible)
		{
			mDesc->sheet->focusWindow(this);
		}
	}

	void MenuPanel::removeWidget(Widget* w)
	{			
		removeChild(w);

		_adjustHeight();
	}

	void MenuPanel::serialize(SerialBase* b)
	{
		// Empty on purpose! MenuPanels don't serialize to disk.
	}

	void MenuPanel::setMaxHeight(float height)
	{
		mDesc->menupanel_maxHeight = height;

		_adjustHeight();
	}
}
