#include "QuickGUIMenu.h"
#include "QuickGUIMouseCursor.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Menu::Menu(const Ogre::String& name, Type type, const Rect& pixelDimensions, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Image(name,type,pixelDimensions,texture,container,ParentWidget,gm),
		mMenuListCounter(0),
		mShowMenus(0),
		mCurrentOpenList(NULL)
	{
		mShowWithParent = false;

		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_MENU)
		{
			mQuad->setLayer(Quad::LAYER_MENU);
		}

		mMenuLists.clear();
	}

	Menu::~Menu()
	{
		clearAllMenuLists();
	}

	MenuList* Menu::addMenuList(const Ogre::String& name, const Ogre::UTFString& text, Ogre::Real pixelX, Ogre::Real pixelWidth, const Ogre::String& texture)
	{
		MenuList* newMenuList = new MenuList(name,TYPE_MENULIST,Rect(pixelX,0,pixelWidth,mSize.height),texture,mQuadContainer,this,mGUIManager);
		newMenuList->addEventHandler(Widget::EVENT_MOUSE_ENTER,&Menu::showMenuList,this);
		newMenuList->addEventHandler(Widget::EVENT_MOUSE_BUTTON_DOWN,&Menu::toggleMenuList,this);

		newMenuList->getText()->setCaption(text);
		mMenuLists.push_back(newMenuList);

		if(!mVisible)
			newMenuList->hide();

		return newMenuList;
	}

	MenuList* Menu::addMenuList(const Ogre::UTFString& text, Ogre::Real pixelX, Ogre::Real pixelWidth, const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName+".MenuList" + Ogre::StringConverter::toString(mMenuListCounter);
		++mMenuListCounter;

		return addMenuList(name,text,pixelX,pixelWidth,texture);
	}

	MenuList* Menu::addMenuList(const Ogre::UTFString& text, Ogre::Real pixelX, Ogre::Real pixelWidth)
	{
		Ogre::String name = mInstanceName+".MenuList" + Ogre::StringConverter::toString(mMenuListCounter);
		++mMenuListCounter;

		Ogre::String material = getDefaultSkin() + ".menulist.png";

		return addMenuList(name,text,pixelX,pixelWidth,material);
	}

	void Menu::clearAllMenuLists()
	{
		Widget::removeAndDestroyAllChildWidgets();
		mMenuLists.clear();
	}

	MenuList* Menu::getMenuList(unsigned int index)
	{
		if( (static_cast<int>(mMenuLists.size()) - 1) < static_cast<int>(index) ) return NULL;
		return mMenuLists[index];
	}

	MenuList* Menu::getMenuList(const Ogre::String& name)
	{
		std::vector<MenuList*>::iterator it;
		for( it = mMenuLists.begin(); it != mMenuLists.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) return (*it);
		}

		return NULL;
	}

	void Menu::hideMenuLists()
	{
		std::vector<MenuList*>::iterator it;
		for( it = mMenuLists.begin(); it != mMenuLists.end(); ++it )
			(*it)->getList()->hide();
		
		mCurrentOpenList = NULL;
	}

	bool Menu::pointOverMenuListButton(Point pixelPosition)
	{
		std::vector<MenuList*>::iterator it;
		for(it = mMenuLists.begin(); it != mMenuLists.end(); ++it )
		{
			if( (*it)->isPointWithinBounds(pixelPosition) )
				return true;
		}

		return false;
	}

	void Menu::setShowMenuState(bool show)
	{
		mShowMenus = false;
	}

	void Menu::showMenuList(const EventArgs& e)
	{
		if(mShowMenus)
		{
			const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(e);

			if( (mea.widget == NULL) || (mea.widget->getWidgetType() != Widget::TYPE_MENULIST) )
				return;

			MenuList* l = dynamic_cast<MenuList*>(mea.widget);
			std::vector<MenuList*>::iterator it;
			for( it = mMenuLists.begin(); it != mMenuLists.end(); ++it )
			{
				if( (*it)->isPointWithinBounds(mea.position) )
				{
					l = (*it);
					break;
				}
			}

			if( mCurrentOpenList == NULL )
				l->getList()->show();
			else if( l->getInstanceName() != mCurrentOpenList->getInstanceName() )
			{
				mCurrentOpenList->getList()->hide();
				l->getList()->show();
			}

			mCurrentOpenList = l;
		}
	}

	void Menu::toggleMenuList(const EventArgs& e)
	{
		const MouseEventArgs mea = static_cast<const MouseEventArgs&>(e);

		if( (mea.widget == NULL) || (mea.widget->getWidgetType() != Widget::TYPE_MENULIST) )
				return;

		if( mea.button == MB_Left )
		{
			// this function only deals with MenuList Widgets, so we can cast
			MenuList* l = dynamic_cast<MenuList*>(mea.widget);
			std::vector<MenuList*>::iterator it;
			for( it = mMenuLists.begin(); it != mMenuLists.end(); ++it )
			{
				if( (*it)->isPointWithinBounds(mea.position) )
				{
					l = (*it);
					break;
				}
			}

			if(l->getList()->isVisible())
			{
				l->getList()->hide();
				mCurrentOpenList = NULL;
				mShowMenus = false;
			}
			else
			{
				if( (mCurrentOpenList != NULL) && (mCurrentOpenList->getInstanceName() != l->getInstanceName()) )
					mCurrentOpenList->getList()->hide();

				l->getList()->show();
				mCurrentOpenList = l;
				mShowMenus = true;
			}
		}
	}
}
