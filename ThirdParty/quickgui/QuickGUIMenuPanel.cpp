#include "QuickGUIMenuPanel.h"
#include "QuickGUIMenuItem.h"
#include "QuickGUIWidgetDescFactoryManager.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String MenuPanel::BACKGROUND = "background";

	void MenuPanel::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("MenuPanel");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("MenuPanel",d);
	}

	MenuPanelDesc::MenuPanelDesc() :
		WindowDesc()
	{
		supportScrollBars = false;
		titleBar = false;
		WindowDesc::titleBar = false;
		resizable = false;
	}

	MenuPanel::MenuPanel(const Ogre::String& name) :
		Window(name)
	{
		mSkinElementName = BACKGROUND;

		addWidgetEventHandler(WIDGET_EVENT_CLIENTSIZE_CHANGED,&MenuPanel::onClientSizeChanged,this);
	}

	MenuPanel::~MenuPanel()
	{
		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
			delete (*it);
	}

	void MenuPanel::_initialize(WidgetDesc* d)
	{
		Window::_initialize(d);

		setSkinType(d->skinTypeName);
	}

	Widget* MenuPanel::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new MenuPanel(widgetName);

		newWidget->_createDescObject("MenuPanelDesc");

		return newWidget;
	}

	void MenuPanel::addMenuItem(MenuItem* i)
	{
		mMenuItems.push_back(i);
		// Add the window as the parent, so that the texture position is calculated correctly. (Since its drawn via the MenuPanel)
		i->setParent(this);

		// Adjust height to fit all MenuItems
		SkinElement* background = mSkinType->getSkinElement(BACKGROUND);
		setHeight(getNextAvailableYPosition() + (background->getBorderThickness(BORDER_TOP) + background->getBorderThickness(BORDER_BOTTOM)));
	}

	void MenuPanel::_draw()
	{
		// check visibility
		if( !mWidgetDesc->visible )
			return;

		Brush* brush = Brush::getSingletonPtr();

		// check and store clip region
		Rect prevClipRegion = brush->getClipRegion();
		if ( prevClipRegion.getIntersection(Rect(mTexturePosition,mWidgetDesc->dimensions.size)) == Rect::ZERO )
			return;

		// draw self
		onDraw();

		// set clip region to client dimensions
		Rect clipRegion = mClientDimensions;
		clipRegion.translate(mTexturePosition);
		brush->setClipRegion(clipRegion);

		// draw components
		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->draw();

		// draw children
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			(*it)->draw();

		// draw menu items
		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
			(*it)->draw();

		// restore clip region
		brush->setClipRegion(prevClipRegion);
	}

	void MenuPanel::draw()
	{
		if(!mWidgetDesc->visible)
			return;

		Brush* brush = Brush::getSingletonPtr();

		if(mDirty)
		{
			// resizeRenderTarget
			resizeRenderTarget();

			// setRenderTarget
			brush->setRenderTarget(mTexture);

			// clearRenderTarget
			brush->clear();

			// update the texture!
			_draw();

			mDirty = false;
		}
		
		brush->setRenderTarget(NULL);
		brush->setTexture(mTexture);
		brush->drawRectangle(mWidgetDesc->dimensions,UVRect(0,0,1,1));
	}

	Widget* MenuPanel::findWidget(const Ogre::String& name)
	{
		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			if((*it)->getName() == name)
				return (*it);

		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
			if((*it)->getName() == name)
				return (*it);

		return ComponentWidget::findWidget(name);
	}

	Widget* MenuPanel::findWidgetAtPoint(const Point& p, bool ignoreDisabled)
	{
		Widget* w = Widget::findWidgetAtPoint(p,ignoreDisabled);

		if(w != NULL)
		{
			for(std::vector<Widget*>::reverse_iterator it = mChildren.rbegin(); it != mChildren.rend(); ++it)
			{
				Widget* w = (*it)->findWidgetAtPoint(p,ignoreDisabled);
				if(w != NULL)
					return w;
			}

			for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			{
				Widget* w = (*it).second->findWidgetAtPoint(p,ignoreDisabled);
				if(w != NULL)
					return w;
			}

			for(std::vector<MenuItem*>::reverse_iterator it = mMenuItems.rbegin(); it != mMenuItems.rend(); ++it)
			{
				Widget* w = (*it)->findWidgetAtPoint(p,ignoreDisabled);
				if(w != NULL)
					return w;
			}
		}
		
		return w;
	}

	Ogre::String MenuPanel::getClass()
	{
		return "MenuPanel";
	}

	void MenuPanel::onDraw()
	{
		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);

		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->brushFilterMode);

		brush->drawSkinElement(Rect(Point::ZERO,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));
	}

	float MenuPanel::getNextAvailableYPosition()
	{
		float maxY = 0;
		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
		{
			if(((*it)->getPosition().y + (*it)->getSize().height) > maxY)
				maxY = ((*it)->getPosition().y + (*it)->getSize().height);
		}

		return maxY;
	}

	void MenuPanel::onClientSizeChanged(const EventArgs& args)
	{
		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
		{
			(*it)->setWidth(mClientDimensions.size.width);
		}
	}

	void MenuPanel::serialize(SerialBase* b)
	{
		// Empty on purpose! MenuPanels don't serialize to disk.
	}

	void MenuPanel::updateTexturePosition()
	{
		// Remember that Windows are their own texture, thus their "screen" position is zero.

		mTexturePosition = Point::ZERO;

		for(std::map<Ogre::String,Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
			(*it).second->updateTexturePosition();

		for(std::vector<Widget*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
			(*it)->updateTexturePosition();

		for(std::vector<MenuItem*>::iterator it = mMenuItems.begin(); it != mMenuItems.end(); ++it)
			(*it)->updateTexturePosition();

		redraw();
	}
}
