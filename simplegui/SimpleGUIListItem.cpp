#include "SimpleGUIListItem.h"
#include "SimpleGUIList.h"

namespace SimpleGUI
{
	ListItem::ListItem(const Ogre::String& name, const Ogre::Vector4& dimensions, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,dimensions,"",show,overlayContainer,ParentWidget),
		mImage(0),
		mButton(0)
	{
		mWidgetType = Widget::SGUI_LISTITEM;
		setCharacterHeight(1.0);
	}
	
	ListItem::~ListItem()
	{
		delete mImage;
		delete mButton;

		mImage = NULL;
		mButton = NULL;
	}

	void ListItem::_notifyDimensionsChanged()
	{
		Label::_notifyDimensionsChanged();

		if(mImage) mImage->_notifyDimensionsChanged();
		if(mButton) mButton->_notifyDimensionsChanged();
	}

	NStateButton* ListItem::addNStateButton(const Ogre::Vector4& dimensions)
	{
		if(mButton != NULL) return NULL;

		mButton = new NStateButton(mInstanceName+"_NStateButton",dimensions,mVisible,mOverlayContainer,this);
		mButton->addEventHandler(Widget::SGUI_DEACTIVATED,&ListItem::evtHndlr_hideMenus,dynamic_cast<ListItem*>(this));
		mButton->addEventHandler(Widget::SGUI_MOUSE_BUTTON_UP,&ListItem::evtHndlr_hideMenus,dynamic_cast<ListItem*>(this));
		return mButton;
	}

	Image* ListItem::addImage(const Ogre::Vector4& dimensions, const Ogre::String& material)
	{
		if(mImage != NULL) return NULL;

		mImage = new Image(mInstanceName+"_Image",dimensions,material,mVisible,mOverlayContainer,this);
		mImage->addEventHandler(Widget::SGUI_DEACTIVATED,&ListItem::evtHndlr_hideMenus,dynamic_cast<ListItem*>(this));
		return mImage;
	}

	void ListItem::deactivate(EventArgs& e)
	{
		if(mParentWidget) mParentWidget->deactivate(e);

		Label::deactivate(e);
	}

	bool ListItem::evtHndlr_hideMenus(const EventArgs& e)
	{
		deactivate(const_cast<EventArgs&>(e));

		return true;
	}

	NStateButton* ListItem::getNStateButton()
	{
		return mButton;
	}

	Image* ListItem::getImage()
	{
		return mImage;
	}

	Widget* ListItem::getTargetWidget(const Ogre::Vector2& p)
	{
		if(!mVisible) return NULL;

		if(mButton && mButton->isPointWithinBounds(p)) return mButton;
		if(mImage && mImage->isPointWithinBounds(p)) return mImage;

		if(isPointWithinBounds(p)) return this;

		return NULL;
	}

	void ListItem::hide()
	{
		if(mButton) mButton->hide();
		if(mImage) mImage->hide();

		Label::hide();
	}

	bool ListItem::onMouseButtonUp(MouseEventArgs& e)
	{
		bool retVal = Label::onMouseButtonUp(e);
		// When the widget or any of its child widgets receive the mouse up event, 
		// hide the menu containing this list item.  This is performed in Menu::onMouseButtonUp
		e.handled = false;
		if(mParentWidget) mParentWidget->onMouseButtonUp(e);
		return retVal;
	}

	bool ListItem::onMouseEnters(MouseEventArgs& e)
	{
		e.widget = this;
		List* parentList = dynamic_cast<List*>(mParentWidget);

		parentList->highlightListItem(this);

		return Label::onMouseEnters(e);
	}

	bool ListItem::onMouseLeaves(MouseEventArgs& e)
	{
		e.widget = this;
		List* parentList = dynamic_cast<List*>(mParentWidget);

		parentList->hideHighlight();

		return Label::onMouseLeaves(e);
	}

	void ListItem::removeNStateButton()
	{
		if(mButton == NULL) return;

		delete mButton;
		mButton = NULL;
	}

	void ListItem::removeImage()
	{
		if(mImage == NULL) return;

		delete mImage;
		mImage = NULL;
	}

	void ListItem::show()
	{
		if(mButton) mButton->show();
		if(mImage) mImage->show();

		Label::show();
	}
}