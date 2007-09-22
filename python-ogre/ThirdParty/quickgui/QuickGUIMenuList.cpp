#include "QuickGUIMenuList.h"
#include "QuickGUIMouseCursor.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	MenuList::MenuList(const Ogre::String& name, Type type, const Rect& dimensions, GuiMetricsMode pMode, GuiMetricsMode sMode, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Button(name,type,dimensions,pMode,sMode,texture,container,ParentWidget,gm)
	{
		mShowWithParent = false;

		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_MENULIST)
		{
			mQuad->setLayer(Quad::LAYER_MENU);
			mText->setLayer(Quad::LAYER_MENU);
		}

		mButtonTexture = mTextureName + ".button" + mTextureExtension;
		mListTexture = mTextureName + ".list" + mTextureExtension;

		addEventHandler(EVENT_LOSE_FOCUS,&MenuList::onLoseFocus,this);

		// create list
		mList = new List(mInstanceName+".List",TYPE_LIST,Rect(0,1,1,0),QGUI_GMM_RELATIVE,QGUI_GMM_RELATIVE,mTextureName + ".list" + mTextureExtension,mQuadContainer,this,mGUIManager);
		mList->setShowWithParent(false);
		mList->hide();
		mList->addEventHandler(EVENT_CHILD_ADDED,&MenuList::addDefaultListItemHandler,this);

		setTexture(mButtonTexture);
		mDefaultTexture = mTextureName;
	}

	MenuList::~MenuList()
	{
		Widget::removeAndDestroyAllChildWidgets();
	}

	void MenuList::addDefaultListItemHandler(const EventArgs& args)
	{
		dynamic_cast<const WidgetEventArgs&>(args).widget->addEventHandler(Widget::EVENT_MOUSE_BUTTON_UP,&MenuList::hideMenuList,this);
		dynamic_cast<const WidgetEventArgs&>(args).widget->addEventHandler(Widget::EVENT_LOSE_FOCUS,&MenuList::hideMenuList,this);
	}

	ListItem* MenuList::addListItem(const Ogre::UTFString& text)
	{
		return mList->addListItem(text);
	}

	ListItem* MenuList::addListItem(const Ogre::String& name, const Ogre::UTFString& text)
	{
		return mList->addListItem(name,text);
	}

	void MenuList::hideMenuList(const EventArgs& e)
	{
		Menu* m = dynamic_cast<Menu*>(mParentWidget);

		m->setShowMenuState(false);
		m->hideMenuLists();
	}

	void MenuList::onLoseFocus(const EventArgs& args)
	{
		const MouseEventArgs mea = dynamic_cast<const MouseEventArgs&>(args);
		// if user clicked on a child of this MenuList, do not completely hide MenuLists.
		if(getTargetWidget(mea.position) != NULL)
			return;

		Menu* m = dynamic_cast<Menu*>(mParentWidget);

		// if the user has clicked another MenuList from the same Menu, do not complete this
		// function.
		if( m->pointOverMenuListButton(mea.position) )
			return;

		m->setShowMenuState(false);
		m->hideMenuLists();
	}

	List* MenuList::getList()
	{
		return mList;
	}
}
