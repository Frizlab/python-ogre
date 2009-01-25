#include "QuickGUITabControl.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUITabPage.h"

namespace QuickGUI
{
	const Ogre::String TabControl::BACKGROUND = "background";

	void TabControl::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("TabControl");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TabControl",d);
	}

	TabControl::TabControl(const Ogre::String& name) :
		ContainerWidget(name),
		mSelectedTab(NULL)
	{
		mSkinElementName = BACKGROUND;
	}

	TabControl::~TabControl()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < TABCONTROL_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mTabControlEventHandlers[index].begin(); it != mTabControlEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void TabControl::_initialize(WidgetDesc* d)
	{
		TabControlDesc* tcd = dynamic_cast<TabControlDesc*>(d);
		tcd->containerwidget_supportScrollBars = false;

		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<TabControlDesc*>(mWidgetDesc);

		setTabHeight(tcd->tabcontrol_tabHeight);
		setTabOverlap(tcd->tabcontrol_tabOverlap);
		setTabReordering(tcd->tabcontrol_tabReordering);

		setSkinType(d->widget_skinTypeName);
	}

	void TabControl::addChild(Widget* w)
	{
		ContainerWidget::addChild(w);

		TabPage* p = dynamic_cast<TabPage*>(w);
		p->setSize(mClientDimensions.size);
		p->setTabHeight(mDesc->tabcontrol_tabHeight);
		
		updateTabPages();

		selectTabPage(p);
	}

	void TabControl::addTabControlEventHandler(TabControlEvent EVENT, EventHandlerSlot* function)
	{
		mTabControlEventHandlers[EVENT].push_back(function);
	}

	TabPage* TabControl::createTabPage(TabPageDesc* d)
	{
		d->widget_dimensions.position = Point::ZERO;
		d->widget_dimensions.size = mClientDimensions.size;
		d->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
		d->widget_verticalAnchor = ANCHOR_VERTICAL_TOP_BOTTOM;
		TabPage* newTabPage = dynamic_cast<TabPage*>(Widget::create("TabPage",d));

		addChild(newTabPage);

		return newTabPage;
	}

	void TabControl::draw()
	{
		// check visibility
		if( !mWidgetDesc->widget_visible )
			return;

		Brush* brush = Brush::getSingletonPtr();

		// check and store clip region
		Rect prevClipRegion = brush->getClipRegion();
		if ( prevClipRegion.getIntersection(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size)) == Rect::ZERO )
			return;

		// set clip region to dimensions
		brush->setClipRegion(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size).getIntersection(prevClipRegion));

		// draw self
		onDraw();

		// draw components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			dynamic_cast<TabPage*>((*it).second)->draw();

		// set clip region to client dimensions
		Rect clipRegion = mClientDimensions;
		clipRegion.translate(mTexturePosition);
		brush->setClipRegion(clipRegion);

		// draw children - draw all of the unselected TabPages first, followed by the selected TabPage
		for(std::vector<Widget*>::reverse_iterator it = mChildren.rbegin(); it != mChildren.rend(); ++it)
		{
			TabPage* tp = dynamic_cast<TabPage*>(*it);

			if(tp != mSelectedTab)
				dynamic_cast<TabPage*>(*it)->draw();
		}

		if(mSelectedTab != NULL)
			mSelectedTab->draw();

		// restore clip region
		brush->setClipRegion(prevClipRegion);
	}

	Widget* TabControl::findWidgetAtPoint(const Point& p, bool ignoreDisabled)
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
			// Check selected tab first.
			if(mSelectedTab != NULL)
			{
				w = mSelectedTab->findWidgetAtPoint(p,ignoreDisabled);
				if(w != NULL)
					return w;
			}

			for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			{
				Widget* w = (*it)->findWidgetAtPoint(p,ignoreDisabled);
				if(w != NULL)
					return w;
			}
		}
				
		return this;
	}

	bool TabControl::fireTabControlEvent(TabControlEvent e, EventArgs& args)
	{
		if(mTabControlEventHandlers[e].empty())
			return false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mTabControlEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		return true;
	}

	Ogre::String TabControl::getClass()
	{
		return "TabControl";
	}

	TabPage* TabControl::getSelectedTabPage()
	{
		return mSelectedTab;
	}

	float TabControl::getTabHeight()
	{
		return mDesc->tabcontrol_tabHeight;
	}

	int TabControl::getTabIndex(TabPage* p)
	{
		int counter = 0;
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			if((*it) == p)
				return counter;

			++counter;
		}

		return -1;
	}

	float TabControl::getTabOverlap()
	{
		return mDesc->tabcontrol_tabOverlap;
	}

	TabPage* TabControl::getTabPage(unsigned int index)
	{
		int counter = 0;

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			if(counter == index)
				return dynamic_cast<TabPage*>(*it);
		}

		return NULL;
	}

	bool TabControl::getTabReordering()
	{
		return mDesc->tabcontrol_tabReordering;
	}

	void TabControl::onDraw()
	{
		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mWidgetDesc->widget_brushFilterMode);
			
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));
	}

	void TabControl::reorderTabPage(TabPage* p, unsigned int index)
	{
		bool removed = false;
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			if((*it) == p)
			{
				mChildren.erase(it);
				removed = true;
				break;
			}
		}

		if(!removed)
			return;

		int counter = 0;
		bool added = false;
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			if(counter == index)
			{
				mChildren.insert(it,p);
				added = true;
				break;
			}

			++counter;
		}

		if(!added)
			mChildren.push_back(p);

		updateTabPages();
	}

	void TabControl::selectTabPage(TabPage* p)
	{
		if(p == NULL)
			throw Exception(Exception::ERR_INVALID_CHILD,"TabPage is NULL, cannot select it!","TabControl::selecteTabPage");

		if(p->getParentWidget() != this)
			throw Exception(Exception::ERR_INVALID_CHILD,"TabPage is not a child of this TabControl!","TabControl::selectTabPage");

		mSelectedTab = p;

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			dynamic_cast<TabPage*>(*it)->deselect();
		}

		mSelectedTab->select();

		WidgetEventArgs args(this);
		fireTabControlEvent(TABCONTROL_EVENT_SELECTION_CHANGED,args);
	}

	void TabControl::setTabHeight(float height)
	{
		mDesc->tabcontrol_tabHeight = height;

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			TabPage* tp = dynamic_cast<TabPage*>(*it);

			tp->setTabHeight(mDesc->tabcontrol_tabHeight);
		}
	}

	void TabControl::setTabOverlap(float overlap)
	{
		mDesc->tabcontrol_tabOverlap = overlap;

		updateTabPages();
	}

	void TabControl::setTabReordering(bool reordering)
	{
		mDesc->tabcontrol_tabReordering = reordering;
	}

	void TabControl::updateTabPages()
	{
		Ogre::String TabControlName = getName();

		int counter = 0;
		float x = 0;
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			TabPage* tp = dynamic_cast<TabPage*>(*it);

			tp->setName(TabControlName + Ogre::StringConverter::toString(counter));
			tp->mTab->setPosition(Point(x,0));
			x += tp->mTab->getSize().width - mDesc->tabcontrol_tabOverlap;
			++counter;
		}
	}
}
