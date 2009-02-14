#include "QuickGUITabPage.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUITabControl.h"
#include "QuickGUIManager.h"
#include "QuickGUIScriptDefinition.h"
#include "QuickGUIFactoryManager.h"

namespace QuickGUI
{
	const Ogre::String TabPage::TAB = "tab";
	const Ogre::String TabPage::TAB_DISABLED = "tab_disabled";
	const Ogre::String TabPage::TAB_OVER = "tab_over";
	const Ogre::String TabPage::TAB_SELECTED = "tab_selected";
	const Ogre::String TabPage::PAGE = "page";

	void TabPage::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TabPage");
		d->defineComponent(TAB);
		d->defineComponent(TAB_OVER);
		d->defineComponent(TAB_SELECTED);
		d->defineComponent(PAGE);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TabPage",d);
	}

	TabPage::TabPage(const Ogre::String& name) :
		Panel(name),
		mTab(NULL),
		mPage(NULL),
		mSelected(false)
	{
	}

	TabPage::~TabPage()
	{
	}

	void TabPage::_initialize(WidgetDesc* d)
	{
		TabPageDesc* tpd = dynamic_cast<TabPageDesc*>(d);
		tpd->containerwidget_supportScrollBars = false;

		Panel::_initialize(d);

		mDesc = dynamic_cast<TabPageDesc*>(mWidgetDesc);

		mDesc->tabpage_tabWidthBuffer = tpd->tabpage_tabWidthBuffer;

		// Create Tab
		TabDesc* td = FactoryManager::getSingleton().getDescFactory()->getInstance<TabDesc>("DefaultTabDesc");
		td->resetToDefault();
		td->widget_name = getName() + ".Tab";
		td->tab_frontWidth = tpd->tabpage_tabFrontWidth;
		td->label_verticalTextAlignment = tpd->tabpage_verticalTextAlignment;
		td->textDesc = tpd->textDesc;
		td->tab_widthBuffer = tpd->tabpage_tabWidthBuffer;
		mTab = dynamic_cast<Tab*>(Widget::create("Tab",td));
		mTab->addWidgetEventHandler(WIDGET_EVENT_MOUSE_BUTTON_DOWN,&TabPage::onMouseButtonDownOnTab,this);
		mTab->addWidgetEventHandler(WIDGET_EVENT_MOUSE_ENTER,&TabPage::onMouseEnterTab,this);
		mTab->addWidgetEventHandler(WIDGET_EVENT_MOUSE_LEAVE,&TabPage::onMouseLeaveTab,this);
		addComponent(TAB,mTab);

		// Create Page
		PanelDesc* pd = FactoryManager::getSingleton().getDescFactory()->getInstance<PanelDesc>("DefaultPanelDesc");
		pd->resetToDefault();
		pd->widget_name = getName() + ".Page";
		pd->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
		pd->widget_verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
		pd->widget_dimensions.position = Point(0,mTab->getHeight());
		pd->widget_dimensions.size = Size(mDesc->widget_dimensions.size.width,mDesc->widget_dimensions.size.height - mTab->getHeight());
		mPage = dynamic_cast<Panel*>(Widget::create("Panel",pd));
		addComponent(PAGE,mPage);

		setSkinType(d->widget_skinTypeName);
	}

	void TabPage::addChild(Widget* w)
	{
		mPage->addChild(w);
	}

	void TabPage::addWidgetEventHandler(WidgetEvent EVENT, EventHandlerSlot* function)
	{
		mPage->addWidgetEventHandler(EVENT,function);
	}

	void TabPage::deselect()
	{
		if(mSelected)
		{
			mSelected = false;
			mTab->setSkinType(mSkinType->getComponentType(TAB)->typeName);
			mPage->setVisible(false);
			redraw();
		}
	}

	Widget* TabPage::findWidgetAtPoint(const Point& p, bool ignoreDisabled)
	{
		// If we are not visible, return NULL
		if(!mWidgetDesc->widget_visible)
			return NULL;

		// If we ignore disabled and this widget is enabled, return NULL
		if(ignoreDisabled && !mWidgetDesc->widget_enabled)
			return NULL;

		Widget* w = NULL;
		
		// Test to see if the position is over the tab
		w = mTab->findWidgetAtPoint(p,ignoreDisabled);
		if(w != NULL)
			return w;

		// Test to see if the position is over the Page (Panel) or its children
		w = mPage->findWidgetAtPoint(p,ignoreDisabled);
		if(w != NULL)
			return w;
		
		return NULL;
	}

	BorderSide TabPage::getBorderSide(Point p)
	{
		return BORDER_NONE;
	}

	Ogre::String TabPage::getClass()
	{
		return "TabPage";
	}

	HorizontalTextAlignment TabPage::getTabHorizontalTextAlignment()
	{
		return mTab->getHorizontalTextAlignment();
	}

	VerticalTextAlignment TabPage::getTabVerticalTextAlignment()
	{
		return mDesc->tabpage_verticalTextAlignment;
	}

	float TabPage::getTabWidthBuffer()
	{
		return mDesc->tabpage_tabWidthBuffer;
	}

	bool TabPage::isSelected()
	{
		return mSelected;
	}

	void TabPage::onDraw()
	{
		// Nothing drawn for TabPage, its just a container for Tab and Panel (Page)
	}

	void TabPage::onMouseButtonDownOnTab(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		if(mea.button == MB_Left)
		{
			if(mParentWidget != NULL)
			{
				dynamic_cast<TabControl*>(mParentWidget)->selectTabPage(this);
			}
		}
	}

	void TabPage::onMouseEnterTab(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);

		TabControl* tc = dynamic_cast<TabControl*>(mParentWidget);

		if(tc->getTabReordering() && (mea.buttonMask & (1 << MB_Left)))
		{
			Widget* w = mDesc->guiManager->getLastClickedWidget();
			if((w != NULL) && (w->getClass() == "Tab") && (w != mTab))
			{
				tc->reorderTabPage(dynamic_cast<TabPage*>(w->getParentWidget()),tc->getTabIndex(this));
				return;
			}
		}

		if(!mSelected)
		{
			mTab->setSkinType(mSkinType->getComponentType(TAB_OVER)->typeName);
			redraw();
		}
	}

	void TabPage::onMouseLeaveTab(const EventArgs& args)
	{
		if(!mSelected)
		{
			mTab->setSkinType(mSkinType->getComponentType(TAB)->typeName);
			redraw();
		}
	}

	bool TabPage::overBorderSide(Point p)
	{
		return false;
	}

	void TabPage::removeChild(Widget* w)
	{
		mPage->removeChild(w);
	}

	void TabPage::select()
	{
		if(!mSelected)
		{
			mSelected = true;
			mPage->setVisible(true);
			mTab->setSkinType(mSkinType->getComponentType(TAB_SELECTED)->typeName);
			redraw();
		}
	}

	void TabPage::serialize(SerialBase* b)
	{
		if(!mWidgetDesc->widget_serialize)
			return;

		b->begin(getClass(),getName());

		mWidgetDesc->serialize(b);

		// If we are reading, we need to create and populate widgets
		if(b->isSerialReader())
		{
			// Serial Read into our TextDesc object
			dynamic_cast<TabPageDesc*>(mWidgetDesc)->textDesc.serialize(b);

			// Apply Desc properties
			_initialize(mWidgetDesc);

			b->begin("Child","widgets");
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
			b->end();
		}
		// If we are writing, we need to serialize children
		else
		{
			// Serialize Tab's text.  When serial reading, we read into TabPage's text object
			mTab->mDesc->textDesc.serialize(b);

			// Serialize Page's children.  They will be properly added on serial read since TabPage::addChild really calls
			// the pages addChild function.
			b->begin("Child","widgets");
			for(std::vector<Widget*>::iterator it = mPage->mChildren.begin(); it != mPage->mChildren.end(); ++it)
			{
				(*it)->serialize(b);
			}
			b->end();
		}

		b->end();
	}

	void TabPage::setEnabled(bool enabled)
	{
		Widget::setEnabled(enabled);

		// disable to tab so users cannot select tab via clicking.
		if(mTab != NULL)
		{
			mTab->setEnabled(enabled);

			if(enabled)
				mTab->setSkinType(mSkinType->getComponentType(TAB)->typeName);
			else
				mTab->setSkinType(mSkinType->getComponentType(TAB_DISABLED)->typeName);
		}
	}

	void TabPage::setDragable(bool dragable)
	{
		Panel::setDragable(dragable);
	}

	void TabPage::setHeight(float pixelHeight)
	{
		Panel::setHeight(pixelHeight);
	}

	void TabPage::setHorizontalAnchor(HorizontalAnchor a)
	{
		Panel::setHorizontalAnchor(a);
	}

	void TabPage::setParent(Widget* parent)
	{
		Panel::setParent(parent);

		if(parent != NULL)
			dynamic_cast<TabControl*>(parent)->selectTabPage(this);
	}

	void TabPage::setPosition(const Point& position)
	{
		Panel::setPosition(position);
	}

	void TabPage::setResizeFromAllSides(bool resizable)
	{
		Widget::setResizeFromAllSides(resizable);
	}

	void TabPage::setResizeFromBottom(bool resizable)
	{
		Widget::setResizeFromBottom(resizable);
	}

	void TabPage::setResizeFromLeft(bool resizable)
	{
		Widget::setResizeFromLeft(resizable);
	}

	void TabPage::setResizeFromRight(bool resizable)
	{
		Widget::setResizeFromRight(resizable);
	}

	void TabPage::setResizeFromTop(bool resizable)
	{
		Widget::setResizeFromTop(resizable);
	}

	void TabPage::setSize(Size size)
	{
		Panel::setSize(size);
	}

	void TabPage::setSkinType(const Ogre::String type)
	{
		Widget::setSkinType(type);
		
		if(mPage != NULL)
		{
			mPage->setSkinType(mSkinType->getComponentType(PAGE)->typeName);
		}
	}

	void TabPage::setTabFont(const Ogre::String& fontName)
	{
		mTab->setFont(fontName);
	}

	void TabPage::setTabFont(const Ogre::String& fontName, unsigned int index)
	{
		mTab->setFont(fontName,index);
	}

	void TabPage::setTabFont(const Ogre::String& fontName, unsigned int startIndex, unsigned int endIndex)
	{
		mTab->setFont(fontName,startIndex,endIndex);
	}

	void TabPage::setTabFont(const Ogre::String& fontName, Ogre::UTFString::code_point c, bool allOccurrences)
	{
		mTab->setFont(fontName,c,allOccurrences);
	}

	void TabPage::setTabFont(const Ogre::String& fontName, Ogre::UTFString s, bool allOccurrences)
	{
		mTab->setFont(fontName,s,allOccurrences);
	}

	void TabPage::setTabHeight(float height)
	{
		mTab->setHeight(height);
		mPage->setPosition(Point(0,mTab->getHeight() - mPage->mSkinType->getSkinElement(mPage->mSkinElementName)->getBorderThickness(BORDER_TOP)));
		mPage->setHeight(mDesc->widget_dimensions.size.height - mTab->getHeight() + mPage->mSkinType->getSkinElement(mPage->mSkinElementName)->getBorderThickness(BORDER_TOP));
	}

	void TabPage::setTabHorizontalTextAlignment(HorizontalTextAlignment a)
	{
		mTab->setHorizontalTextAlignment(a);

		redraw();
	}

	void TabPage::setTabText(Ogre::UTFString s, Ogre::FontPtr fp, const Ogre::ColourValue& cv)
	{
		mTab->setText(s,fp,cv);
	}

	void TabPage::setTabText(Ogre::UTFString s, const Ogre::String& fontName, const Ogre::ColourValue& cv)
	{
		mTab->setText(s,fontName,cv);
	}

	void TabPage::setTabText(Ogre::UTFString s)
	{
		mTab->setText(s);
	}

	void TabPage::setTabVerticalTextAlignment(VerticalTextAlignment a)
	{
		mTab->setVerticalTextAlignment(a);

		redraw();
	}

	void TabPage::setTabWidthBuffer(float pixelBuffer)
	{
		mDesc->tabpage_tabWidthBuffer = pixelBuffer;

		mTab->setWidthBuffer(pixelBuffer);
	}

	void TabPage::setTransparencyPicking(bool transparencyPicking)
	{
		Panel::setTransparencyPicking(transparencyPicking);
	}

	void TabPage::setVerticalAnchor(VerticalAnchor a)
	{
		Panel::setVerticalAnchor(a);
	}

	void TabPage::setWidth(float pixelWidth)
	{
		Panel::setWidth(pixelWidth);
	}

	void TabPage::updateClientDimensions()
	{
		mClientDimensions.position = Point::ZERO;
		mClientDimensions.size = mWidgetDesc->widget_dimensions.size;

		WidgetEventArgs args(this);
		fireWidgetEvent(WIDGET_EVENT_CLIENTSIZE_CHANGED,args);
	}
}
