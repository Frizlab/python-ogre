#include "QuickGUIListItem.h"
#include "QuickGUIList.h"
#include "QuickGUIManager.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	ListItem::ListItem(const Ogre::String& name, const Rect& pixelDimensions, GUIManager* gm) :
		Label(name,pixelDimensions,"",gm),
		mImage(0),
		mButton(0),
		mPropogateImageMouseEvents(true),
		mPropogateButtonMouseEvents(true)
	{
		mWidgetType = TYPE_LISTITEM;
		mQuad->setClippingWidget(mClippingWidget);
		mText->_clipToWidgetDimensions(mClippingWidget);

		addEventHandler(EVENT_MOUSE_ENTER,&ListItem::onMouseEnters,this);
		addEventHandler(EVENT_MOUSE_LEAVE,&ListItem::onMouseLeaves,this);
	}
	
	ListItem::~ListItem()
	{
		Widget::removeAndDestroyAllChildWidgets();

		mImage = NULL;
		mButton = NULL;
	}

	NStateButton* ListItem::addNStateButton(const Rect& pixelDimensions)
	{
		if(mButton != NULL) 
			return NULL;

		mButton = new NStateButton(mInstanceName+".NStateButton",pixelDimensions,mGUIManager);
		addChild(mButton);
		mButton->getQuad()->setLayer(mQuad->getLayer());
		mButton->getQuad()->setClippingWidget(mClippingWidget);
		
		if(mPropogateButtonMouseEvents)
		{
			mButton->addEventHandler(Widget::EVENT_MOUSE_ENTER,&ListItem::onMouseEnters,this);
			mButton->addEventHandler(Widget::EVENT_MOUSE_BUTTON_DOWN,&ListItem::onMouseButtonDown,this);
			mButton->addEventHandler(Widget::EVENT_MOUSE_BUTTON_UP,&ListItem::onMouseButtonUp,this);
			mButton->addEventHandler(Widget::EVENT_LOSE_FOCUS,&ListItem::onLoseFocus,this);
		}

		if(!mVisible) 
			mButton->hide();

		return mButton;
	}

	Image* ListItem::addImage(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if(mImage != NULL) 
			return NULL;

		mImage = new Image(mInstanceName+".Image",pixelDimensions,texture,mGUIManager);
		addChild(mImage);
		mImage->getQuad()->setLayer(mQuad->getLayer());
		mImage->getQuad()->setClippingWidget(mClippingWidget);

		if(mPropogateImageMouseEvents)
		{
			mImage->addEventHandler(Widget::EVENT_MOUSE_ENTER,&ListItem::onMouseEnters,this);
			mImage->addEventHandler(Widget::EVENT_MOUSE_BUTTON_DOWN,&ListItem::onMouseButtonDown,this);
			mImage->addEventHandler(Widget::EVENT_MOUSE_BUTTON_UP,&ListItem::onMouseButtonUp,this);
			mImage->addEventHandler(Widget::EVENT_LOSE_FOCUS,&ListItem::onLoseFocus,this);
		}
		
		if(!mVisible) 
			mImage->hide();
		
		return mImage;
	}

	NStateButton* ListItem::getNStateButton()
	{
		return mButton;
	}

	Image* ListItem::getImage()
	{
		return mImage;
	}

	void ListItem::onLoseFocus(const EventArgs& args)
	{
		fireEvent(EVENT_LOSE_FOCUS,args);
	}

	void ListItem::onMouseButtonDown(const EventArgs& args)
	{
		fireEvent(EVENT_MOUSE_BUTTON_DOWN,args);
	}

	void ListItem::onMouseButtonUp(const EventArgs& args)
	{
		fireEvent(EVENT_MOUSE_BUTTON_UP,args);
	}

	void ListItem::onMouseEnters(const EventArgs& args)
	{
		dynamic_cast<List*>(mParentWidget)->highlightListItem(this);
	}

	void ListItem::onMouseLeaves(const EventArgs& args)
	{
		dynamic_cast<List*>(mParentWidget)->hideHighlight();
	}

	void ListItem::removeNStateButton()
	{
		if(mButton == NULL) 
			return;

		mGUIManager->destroyWidget(mButton);
		mButton = NULL;
	}

	void ListItem::removeImage()
	{
		if(mImage == NULL) 
			return;

		mGUIManager->destroyWidget(mImage);
		mImage = NULL;
	}

	void ListItem::setParent(Widget* parent)
	{
		mClippingWidget = mParentWidget;
		mQuad->setClippingWidget(mClippingWidget);
		Label::setParent(parent);
	}

	void ListItem::setPropogateButtonMouseEvents(bool propogate)
	{
		mPropogateButtonMouseEvents = propogate;
	}

	void ListItem::setPropogateImageMouseEvents(bool propogate)
	{
		mPropogateImageMouseEvents = propogate;
	}
}