#include "QuickGUIComboBox.h"
#include "QuickGUIManager.h"
#include "QuickGUIMouseCursor.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	ComboBox::ComboBox(const Ogre::String& instanceName, const Size& pixelSize, Ogre::String texture, GUIManager* gm) :
		Image(instanceName,pixelSize,texture,gm),
		mRightToLeft(false),
		mHighlightedItem(0),
		mAutoSize(true),
		mVPixelPadHeight(10)
	{
		mWidgetType = TYPE_COMBOBOX;
		mSkinComponent = ".combobox";
		addEventHandler(EVENT_LOSE_FOCUS,&ComboBox::onLoseFocus,this);
		addEventHandler(EVENT_MOUSE_ENTER,&ComboBox::onMouseEnters,this);
		addEventHandler(EVENT_MOUSE_LEAVE,&ComboBox::onMouseLeaves,this);
		addEventHandler(EVENT_MOUSE_BUTTON_UP,&ComboBox::onMouseButtonUp,this);
		addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&ComboBox::onMouseButtonDown,this);

		mTextUtilities = new Text(mInstanceName+".TextUtilities",mQuadContainer,this);
		mTextUtilities->disable();

		Ogre::String name = mInstanceName+".DropDownList";
		mGUIManager->notifyNameUsed(mInstanceName+".DropDownList");

		mList = new List(name,Size(mSize.width,100),mTextureName + ".list" + mTextureExtension,mGUIManager);
		addChild(mList);
		mList->setPosition(0,mSize.height);
		mList->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		mList->setVerticalAnchor(ANCHOR_VERTICAL_BOTTOM);
		mList->setQuadLayer(Quad::LAYER_MENU);
		mList->setInheritQuadLayer(false);
		mList->setClippingWidget(NULL);
		mList->setInheritClippingWidget(false);
		mList->setShowWithParent(false);
		mList->setOffset(mOffset + 2);
		mList->allowScrolling(true);
		mList->hide();
		mList->setPropogateEventFiring(EVENT_MOUSE_BUTTON_UP,true);
		mList->setPropogateEventFiring(EVENT_MOUSE_BUTTON_DOWN,true);
		mList->setPropogateEventFiring(EVENT_MOUSE_ENTER,true);
		mList->setPropogateEventFiring(EVENT_MOUSE_LEAVE,true);

		mGUIManager->notifyNameUsed(mInstanceName+".SelectedItem");
		mMenuLabel = new MenuLabel(mInstanceName+".SelectedItem",Size(mSize.width - mSize.height,mSize.height),mTextureName + ".menulabel" + mTextureExtension,mGUIManager);
		addChild(mMenuLabel);
		mMenuLabel->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		mMenuLabel->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		mMenuLabel->setPosition(0,0);
		mMenuLabel->setPropogateEventFiring(EVENT_LOSE_FOCUS,true);
		mMenuLabel->setPropogateEventFiring(EVENT_MOUSE_ENTER,true);
		mMenuLabel->setPropogateEventFiring(EVENT_MOUSE_LEAVE,true);

		mGUIManager->notifyNameUsed(mInstanceName+".DropDownButton");
		mButton = new Button(mInstanceName+".DropDownButton",Size(mSize.height,mSize.height),mTextureName + ".button" + mTextureExtension,mGUIManager);
		addChild(mButton);
		mButton->setPosition(mSize.width - mSize.height,0);
		mButton->setAutoSize(false);
		mButton->setHorizontalAnchor(ANCHOR_HORIZONTAL_RIGHT);
		mButton->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		mButton->setPropogateEventFiring(EVENT_MOUSE_BUTTON_UP,true);
		mButton->setPropogateEventFiring(EVENT_MOUSE_ENTER,true);
		mButton->setPropogateEventFiring(EVENT_MOUSE_LEAVE,true);

		mHighlightTexture = mTextureName + ".highlight" + mTextureExtension;

		// create highlight container for the list
		mHighlightPanel = _createQuad();
		mHighlightPanel->setClippingWidget(mList);
		mHighlightPanel->setInheritClippingWidget(false);
		mHighlightPanel->setLayer(Quad::LAYER_MENU);
		mHighlightPanel->setInheritLayer(false);
		mHighlightPanel->setShowWithOwner(false);
		mHighlightPanel->setTexture(mHighlightTexture);
		// offset + 3, to be able to show over ListItems with Images and Buttons and Text
		mHighlightPanel->setOffset(mOffset+3);
		mHighlightPanel->_notifyQuadContainer(mQuadContainer);
	}

	ComboBox::~ComboBox()
	{
		delete mTextUtilities;

		Widget::removeAndDestroyAllChildWidgets();

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnSelectUserEventHandlers.begin(); it != mOnSelectUserEventHandlers.end(); ++it )
			delete (*it);
		mOnSelectUserEventHandlers.clear();
	}

	MenuLabel* ComboBox::addItem()
	{
		MenuLabel* newMenuLabel = mList->addMenuLabel();
		newMenuLabel->setPropogateEventFiring(EVENT_MOUSE_ENTER,true);
		newMenuLabel->setPropogateEventFiring(EVENT_MOUSE_LEAVE,true);
		return newMenuLabel;
	}

	void ComboBox::addOnSelectionEventHandler(MemberFunctionSlot* function)
	{
		mOnSelectUserEventHandlers.push_back(function);
	}

	void ComboBox::clearDropDownList()
	{
		mList->clear();
	}

	void ComboBox::clearSelection()
	{
		mHighlightPanel->setVisible(false);
		mHighlightedItem = NULL;
		mMenuLabel->setText("");
		mMenuLabel->setIconTexture("");
		mMenuLabel->setButtonTexture("");
	}

	int ComboBox::getItemIndex(MenuLabel* l)
	{
		return mList->getItemIndex(l);
	}

	int ComboBox::getNumberOfItems()
	{
		return mList->getNumberOfItems();
	}

	MenuLabel* ComboBox::getSelectedItem()
	{
		return mHighlightedItem;
	}

	int ComboBox::getSelectedItemIndex()
	{
		if(mHighlightedItem == NULL)
			return -1;

		return mList->getItemIndex(mHighlightedItem);
	}

	int ComboBox::getVerticalPixelPadHeight()
	{
		return mVPixelPadHeight;
	}

	void ComboBox::hideDropDownList(const EventArgs& args)
	{
		mList->hide();
	}

	void ComboBox::highlightListItem(MenuLabel* l)
	{
		if(l == NULL)
			return;

		mHighlightedItem = l;
		mHighlightPanel->setPosition(mHighlightedItem->getScreenPosition() + mHighlightedItem->getScrollOffset());
		mHighlightPanel->setSize(mHighlightedItem->getSize());
		mHighlightPanel->setVisible(true);
	}

	// Called when the user clicks outside the widget
	void ComboBox::onLoseFocus(const EventArgs& args)
	{
		const MouseEventArgs mea = dynamic_cast<const MouseEventArgs&>(args);

		// Selected Item (MenuLabel Widget) is set to call ComboBox::onLoseFocus.  Add this in to
		// handle situation where user clicks on Selected Item, and then ComboBox.
		if(isPointWithinBounds(mea.position))
			return;

		// Drop Down List children call this function when they lose focus, so this check is needed because you can enter this
		// function when you haven't actually clicked outside the combobox.
		if(mList->isPointWithinBounds(mea.position))
			return;

		mList->hide();
	}

	void ComboBox::onMouseEnters(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);
		if(mList->isPointWithinBounds(mea.position))
		{
			highlightListItem(dynamic_cast<MenuLabel*>(mea.widget));
		}
		
		setTexture(mTextureName + ".over" + mTextureExtension,false);
		mButton->applyButtonOverTexture();
	}

	void ComboBox::onMouseLeaves(const EventArgs& args)
	{
		mHighlightPanel->setVisible(false);
		mHighlightedItem = NULL;
		
		setTexture(mTextureName + mTextureExtension,false);
		mButton->applyDefaultTexture();
	}

	void ComboBox::onMouseButtonDown(const EventArgs& args)
	{
		setTexture(mTextureName + ".down" + mTextureExtension,false);
		mButton->applyButtonDownTexture();
	}

	void ComboBox::onMouseButtonUp(const EventArgs& args)
	{
		const MouseEventArgs& mea = dynamic_cast<const MouseEventArgs&>(args);
		if(mList->isPointWithinBounds(mea.position))
			onSelection(args);
		else
		{
			if(mList->isVisible())
			{
				mList->hide();
				mGUIManager->_menuClosed(mList);
			}
			else
			{
				mList->show();
				mGUIManager->_menuOpened(mList);
			}
		}
	}

	void ComboBox::onSelection(const EventArgs& args)
	{
		selectItem(dynamic_cast<MenuLabel*>(dynamic_cast<const WidgetEventArgs&>(args).widget));
		setTexture(mTextureName + mTextureExtension,false);
		mButton->applyDefaultTexture();
		mList->hide();

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnSelectUserEventHandlers.begin(); it != mOnSelectUserEventHandlers.end(); ++it )
			(*it)->execute(args);
	}

	void ComboBox::selectItem(unsigned int index)
	{
		Widget* w = mList->getItem(index);
		if( w == NULL )
			return;

		selectItem(dynamic_cast<MenuLabel*>(w));
	}

	void ComboBox::selectItem(MenuLabel* l)
	{
		mMenuLabel->setText(l->getText());
		mMenuLabel->setIconTexture(l->getIconTexture());
		mMenuLabel->setButtonTexture(l->getButtonTexture());
		mHighlightPanel->setVisible(false);
	}

	void ComboBox::setDropDownHeight(Ogre::Real pixelHeight)
	{
		mDropDownHeight = pixelHeight;
		mList->setHeight(mDropDownHeight);
	}

	void ComboBox::setDropDownWidth(Ogre::Real pixelWidth)
	{
		mDropDownWidth = pixelWidth;
		mList->setWidth(mDropDownWidth);
		mList->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT);
	}

	void ComboBox::setFont(const Ogre::String& fontScriptName, bool recursive)
	{
		Image::setFont(fontScriptName,recursive);
		mTextUtilities->setFont(fontScriptName);

		if(mAutoSize)
		{
			setHeight(mTextUtilities->getNewlineHeight() + mVPixelPadHeight);
			mAutoSize = true;
		}
	}

	void ComboBox::setHeight(Ogre::Real pixelHeight)
	{
		Image::setHeight(pixelHeight);
		mAutoSize = false;
	}

	void ComboBox::setHighlightTexture(const Ogre::String& texture)
	{
		mHighlightTexture = texture;
		mHighlightPanel->setTexture(mHighlightTexture);
	}

	void ComboBox::setRightToLeft(bool rightToLeft)
	{
		if(mRightToLeft == rightToLeft)
			return;

		mRightToLeft = rightToLeft;

		if(mRightToLeft)
		{
			mButton->setXPosition(0);
			mButton->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT);
			mMenuLabel->setPosition(mSize.height,0);
		}
		else
		{
			mButton->setXPosition(mSize.width - mButton->getWidth());
			mButton->setHorizontalAnchor(ANCHOR_HORIZONTAL_RIGHT);
			mMenuLabel->setPosition(0,0);
		}

		//mMenuLabel->setRightToLeft(mRightToLeft);
	}

	void ComboBox::setSize(const Ogre::Real& pixelWidth, const Ogre::Real& pixelHeight)
	{
		Image::setSize(pixelWidth,pixelHeight);
		mAutoSize = false;
	}

	void ComboBox::setSize(const Size& pixelSize)
	{
		ComboBox::setSize(pixelSize.width,pixelSize.height);
	}

	void ComboBox::setSkin(const Ogre::String& skinName, Ogre::String extension, bool recursive)
	{
		mSkinName = skinName;
		setTexture(mSkinName + mSkinComponent + extension);
	}

	void ComboBox::setTexture(const Ogre::String& textureName, bool updateBaseTexture)
	{
		Image::setTexture(textureName,updateBaseTexture);

		if(updateBaseTexture)
		{
			mList->setTexture(mTextureName + ".list" + mTextureExtension);
			mMenuLabel->setTexture(mTextureName + ".menulabel" + mTextureExtension);
			mButton->setTexture(mTextureName + ".button" + mTextureExtension);

			mHighlightTexture = mTextureName + ".highlight" + mTextureExtension;
			mHighlightPanel->setTexture(mHighlightTexture);
		}
	}

	void ComboBox::setVerticalPixelPadHeight(unsigned int height)
	{
		mVPixelPadHeight = height;

		if(mAutoSize)
		{
			setHeight(mTextUtilities->getNewlineHeight() + mVPixelPadHeight);
			mAutoSize = true;
		}
	}
}
