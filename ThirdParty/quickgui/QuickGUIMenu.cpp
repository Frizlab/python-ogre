#include "QuickGUIMenu.h"
#include "QuickGUIMouseCursor.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Menu::Menu(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,dimensions,material,overlayContainer,ParentWidget),
		mMenuListCounter(0),
		mShowMenus(0)
	{
		mWidgetType = Widget::QGUI_TYPE_MENU;
		mMenuListAnchors.clear();
		mMenuLists.clear();
	}

	Menu::~Menu()
	{
		clearAllMenuLists();
	}

	void Menu::_notifyDimensionsChanged()
	{
		Label::_notifyDimensionsChanged();

		std::vector<Button*>::iterator it1;
		for( it1 = mMenuListAnchors.begin(); it1 != mMenuListAnchors.end(); ++it1 )
			(*it1)->_notifyDimensionsChanged();

		std::vector<List*>::iterator it2;
		for( it2 = mMenuLists.begin(); it2 != mMenuLists.end(); ++it2 )
			(*it2)->_notifyDimensionsChanged();
	}

	void Menu::_notifyZOrder(Ogre::ushort windowZOrder, Ogre::ushort offsetZOrder)
	{
		Label::_notifyZOrder(windowZOrder,offsetZOrder);
	}

	void Menu::_notifyZOrder(Ogre::ushort windowZOrder)
	{
		std::vector<Button*>::iterator it1;
		for( it1 = mMenuListAnchors.begin(); it1 != mMenuListAnchors.end(); ++it1 )
			(*it1)->_notifyZOrder(windowZOrder);

		std::vector<List*>::iterator it2;
		for( it2 = mMenuLists.begin(); it2 != mMenuLists.end(); ++it2 )
			(*it2)->_notifyZOrder(windowZOrder);

		Label::_notifyZOrder(windowZOrder);
	}

	List* Menu::addMenuList(const Ogre::UTFString& anchorText, const Ogre::Vector4& anchorRelativeDimensions, const Ogre::String& anchorMaterial, const Ogre::Real& relativeMenuListWidth, const Ogre::String& MenuListMaterial, Ogre::String highlightMaterial)
	{
		Ogre::String anchorName = mInstanceName + "_ListAnchor" + Ogre::StringConverter::toString(mMenuListCounter);
		Ogre::String listName = mInstanceName + "_List" + Ogre::StringConverter::toString(mMenuListCounter);
		++mMenuListCounter;

		Button* newAnchorButton = new Button(anchorName,anchorRelativeDimensions,anchorMaterial,mChildrenContainer,this);
		newAnchorButton->setCharacterHeight(mCharacterHeight);
		newAnchorButton->setFont(mFont);
		newAnchorButton->setTextColor(mTextTopColor,mTextBotColor);
		newAnchorButton->setText(anchorText);
		newAnchorButton->_notifyZOrder(mWindowZOrder,1);
		mMenuListAnchors.push_back(newAnchorButton);
		if(!mVisible) newAnchorButton->hide();

		List* newMenuList = new List(listName,Ogre::Vector3(anchorRelativeDimensions.x,1.0,relativeMenuListWidth),MenuListMaterial,highlightMaterial,mOverlayContainer,this);
		newMenuList->setCharacterHeight(mCharacterHeight);
		newMenuList->setFont(mFont);
		newMenuList->setTextColor(mTextTopColor,mTextBotColor);
		newMenuList->_notifyZOrder(mWindowZOrder,1);
		mMenuLists.push_back(newMenuList);
		newMenuList->hide();

		newAnchorButton->addEventHandler(Widget::QGUI_EVENT_MOUSE_BUTTON_DOWN,&Menu::evtHndlr_toggleMenuList,dynamic_cast<Menu*>(this));
		newAnchorButton->addEventHandler(Widget::QGUI_EVENT_DEACTIVATED,&Menu::evtHndlr_hideMenuList,dynamic_cast<Menu*>(this));

		return newMenuList;
	}

	List* Menu::addMenuList(const Ogre::UTFString& anchorText, const Ogre::Vector4& anchorRelativeDimensions, const Ogre::Real& relativeMenuListWidth)
	{
		Ogre::String anchorMaterial = GUIManager::getSingleton().getDefaultSkin() + ".menubutton";
		Ogre::String menuListMaterial = GUIManager::getSingleton().getDefaultSkin() + ".menulist";
		Ogre::String highlightMaterial = GUIManager::getSingleton().getDefaultSkin() + ".listitem.highlight";

		return addMenuList(anchorText,anchorRelativeDimensions,anchorMaterial,relativeMenuListWidth,menuListMaterial,highlightMaterial);
	}

	void Menu::clearAllMenuLists()
	{
		std::vector<Button*>::iterator it1;
		for( it1 = mMenuListAnchors.begin(); it1 != mMenuListAnchors.end(); ++it1 )
			delete (*it1);
		mMenuListAnchors.clear();

		std::vector<List*>::iterator it2;
		for( it2 = mMenuLists.begin(); it2 != mMenuLists.end(); ++it2 )
			delete (*it2);
		mMenuLists.clear();
	}

	// Called when the user clicks outside the widget
	void Menu::deactivate(EventArgs& e)
	{
		// If the Mouse has clicked on any of the menu's List or ListItems, the widget should not *deactivate*.
		// As for hiding the list, this will be taken care of in the onMouseButtonDown handler.  The list needs
		// to remain visible so that ListItem picking works correctly. (If list is hidden, you can't click the ListItem..)
		if(getTargetWidget(MouseCursor::getSingleton().getPixelPosition()) != NULL) return;
		
		std::vector<List*>::iterator it;
		for( it = mMenuLists.begin(); it != mMenuLists.end(); ++it )
		{
			(*it)->hide();
		}
		mShowMenus = false;

		Label::deactivate(e);
	}

	bool Menu::evtHndlr_hideMenuList(const EventArgs& e)
	{
		// If the Mouse has clicked on any of the menu's List or ListItems, the widget should not *deactivate*.
		// As for hiding the list, this will be taken care of in the onMouseButtonDown handler.  The list needs
		// to remain visible so that ListItem picking works correctly. (If list is hidden, you can't click the ListItem..)
		if(getTargetWidget(MouseCursor::getSingleton().getPixelPosition()) != NULL) return true;
		
		hideMenus();

		return true;
	}

	bool Menu::evtHndlr_toggleMenuList(const EventArgs& e)
	{
		const EventArgs* temp = &e;
		const MouseEventArgs* mea = static_cast<const MouseEventArgs*>(temp);
		if( mea->button == QuickGUI::MB_Left )
		{
			// get index of button clicked
			int index = 0;
			std::vector<Button*>::iterator it;
			for( it = mMenuListAnchors.begin(); it != mMenuListAnchors.end(); ++it )
			{
				if( (*it)->getInstanceName() == mea->widget->getInstanceName() )
					break;
				
				++index;
			}
			// get target list corresponding to the clicked anchor (button) and toggle visibility
			if(mMenuLists[index]->isVisible()) 
			{
				mMenuLists[index]->hide();
				mShowMenus = false;
			}
			else 
			{
				// Hide all menus and show just the desired one
				hideMenus();
				mMenuLists[index]->show();
				mShowMenus = true;
			}
		}
		
		return true;
	}

	Widget* Menu::getTargetWidget(const Ogre::Vector2& p)
	{
		if(!mVisible) return NULL;

		std::vector<Button*>::iterator it1;
		for( it1 = mMenuListAnchors.begin(); it1 != mMenuListAnchors.end(); ++it1 )
			if( (*it1)->isPointWithinBounds(p) ) return (*it1);

		std::vector<List*>::iterator it2;
		for( it2 = mMenuLists.begin(); it2 != mMenuLists.end(); ++it2 )
			if( (*it2)->getTargetWidget(p) != NULL ) return (*it2)->getTargetWidget(p);

		if(isPointWithinBounds(p)) return this;

		return NULL;
	}

	void Menu::hide()
	{
		std::vector<Button*>::iterator it1;
		for( it1 = mMenuListAnchors.begin(); it1 != mMenuListAnchors.end(); ++it1 )
			(*it1)->hide();

		hideMenus();

		Label::hide();
	}

	void Menu::hideMenus()
	{
		std::vector<List*>::iterator it2;
		for( it2 = mMenuLists.begin(); it2 != mMenuLists.end(); ++it2 )
			(*it2)->hide();

		mShowMenus = false;
	}

	bool Menu::onMouseButtonUp(MouseEventArgs& e)
	{
		if( e.button == QuickGUI::MB_Left )
		{
			Widget* w = getTargetWidget(MouseCursor::getSingleton().getPixelPosition());

			// If mouse went down over a list item's Image or NStateButton, set Pointer to ListItem
			if( w != NULL )
			{
				if( (w->getParentWidget() != NULL) && (w->getParentWidget()->getWidgetType() == Widget::QGUI_TYPE_LISTITEM) )
					w = w->getParentWidget();
			}

			// Traverse parents to find root List - make sure we have a list item, or a child widget of a list item (Image of NStateButton)
			if( (w != NULL) && (w->getWidgetType() == Widget::QGUI_TYPE_LISTITEM) )
			{
				while( (w->getParentWidget() != NULL) && (w->getParentWidget()->getWidgetType() == Widget::QGUI_TYPE_LIST) )
					w = w->getParentWidget();

				w->hide();
				mShowMenus = false;
			}
		}

		return Label::onMouseButtonUp(e);
	}

	bool Menu::onMouseMoved(MouseEventArgs& e)
	{
		if(mShowMenus)
		{
			// get index of button clicked
			int index = 0;
			bool found = false;
			std::vector<Button*>::iterator it;
			for( it = mMenuListAnchors.begin(); it != mMenuListAnchors.end(); ++it )
			{
				if( (*it)->isPointWithinBounds(MouseCursor::getSingleton().getPixelPosition()) )
				{
					found = true;
					break;
				}
				
				++index;
			}

			if(found)
			{
				if(!mMenuLists[index]->isVisible())
				{
					// Hide all menus and show just the desired one
					hideMenus();
					mMenuLists[index]->show();
					mShowMenus = true;
				}
			}
		}

		return Label::onMouseMoved(e);
	}

	void Menu::show()
	{
		std::vector<Button*>::iterator it1;
		for( it1 = mMenuListAnchors.begin(); it1 != mMenuListAnchors.end(); ++it1 )
			(*it1)->show();

		Label::show();
	}

	void Menu::timeElapsed(Ogre::Real time)
	{
		std::vector<Button*>::iterator it1;
		for( it1 = mMenuListAnchors.begin(); it1 != mMenuListAnchors.end(); ++it1 )
			(*it1)->timeElapsed(time);

		std::vector<List*>::iterator it2;
		for( it2 = mMenuLists.begin(); it2 != mMenuLists.end(); ++it2 )
			(*it2)->timeElapsed(time);

		Label::timeElapsed(time);
	}
}