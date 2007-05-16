#include "QuickGUIComboBox.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	ComboBox::ComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, const Ogre::String& highlightMaterial, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,dimensions,material,overlayContainer,ParentWidget),
		mListItemHorizontalAlignment(QuickGUI::QGUI_HA_MID),
		mListItemVerticalAlignment(QuickGUI::QGUI_VA_MID)
	{
		mWidgetType = Widget::QGUI_TYPE_COMBOBOX;

		// Create CloseButton - remember to position it relative to it's parent (TitleBar)
		// Height of the Title Bar
		Ogre::Real height = (mAbsoluteDimensions.w / mAbsoluteDimensions.z);
		// Button has same height as width - Make the button slightly smaller that the titlebar height
		Ogre::Real buttonHeight = 0.8;
		Ogre::Real buttonWidth = (height * buttonHeight);
		// Make a 5 pixel buffer
		Ogre::Real buffer = 5.0 / mPixelDimensions.z;
		Ogre::Vector4 bDimensions = Ogre::Vector4((1 - (buttonWidth + buffer)),0.1,buttonWidth,buttonHeight);
		mButton = new Button(mInstanceName+".ComboButton",bDimensions,mWidgetMaterial+".button",mChildrenContainer,this);
		mButton->_notifyZOrder(mWindowZOrder,1);

		// Make a 15 pixel buffer for the Label
		Ogre::Real bigBuffer = 15.0 / mPixelDimensions.z;
		mLabel = new Label(mInstanceName+".Label",Ogre::Vector4(bigBuffer,0,(1 - (buttonWidth + bigBuffer)),1),"",mOverlayContainer,this);
		mLabel->addEventHandler(Widget::QGUI_EVENT_DEACTIVATED,&ComboBox::deactivate,dynamic_cast<ComboBox*>(this));
		mLabel->_notifyZOrder(mWindowZOrder,0);

		mDropDownListContainer = createOverlayContainer(mInstanceName+".DropDownListContainer","");
		mChildrenContainer->addChildImpl(mDropDownListContainer);

		mList = new List(mInstanceName+".List",Ogre::Vector3(0,1.0,1.0),mWidgetMaterial+".list",highlightMaterial,mDropDownListContainer,this);
		mList->setCharacterHeight(mCharacterHeight);
		mList->setFont(mFont);
		mList->setTextColor(mTextTopColor,mTextBotColor);
		mList->hide();
		mList->_notifyZOrder(mWindowZOrder,2);
	}

	ComboBox::~ComboBox()
	{
		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnSelectUserEventHandlers.begin(); it != mOnSelectUserEventHandlers.end(); ++it )
			delete (*it);
		mOnSelectUserEventHandlers.clear();

		delete mLabel;
		delete mButton;
		delete mList;

		mChildrenContainer->removeChild(mDropDownListContainer->getName());
		om->destroyOverlayElement(mDropDownListContainer);
	}

	void ComboBox::_notifyDimensionsChanged()
	{
		Label::_notifyDimensionsChanged();

		mLabel->_notifyDimensionsChanged();
		mButton->_notifyDimensionsChanged();
		mList->_notifyDimensionsChanged();
	}

	void ComboBox::_notifyZOrder(Ogre::ushort windowZOrder, Ogre::ushort offsetZOrder)
	{
		Label::_notifyZOrder(windowZOrder,offsetZOrder);
	}

	void ComboBox::_notifyZOrder(Ogre::ushort windowZOrder)
	{
		mLabel->_notifyZOrder(windowZOrder);
		mButton->_notifyZOrder(windowZOrder);
		mList->_notifyZOrder(windowZOrder);
		Label::_notifyZOrder(windowZOrder);
	}

	void ComboBox::addOnSelectionEventHandler(MemberFunctionSlot* function)
	{
		mOnSelectUserEventHandlers.push_back(function);
	}

	void ComboBox::addListItem(const Ogre::UTFString& text)
	{
		ListItem* newListItem = mList->addListItem(text);
	}

	void ComboBox::alignListItemText(HorizontalAlignment ha, VerticalAlignment va)
	{
		mListItemHorizontalAlignment = ha;
		mListItemVerticalAlignment = va;

		mList->alignListItemText(mListItemHorizontalAlignment,mListItemVerticalAlignment);
	}

	int ComboBox::getNumberOfListItems()
	{
		return mList->getNumberOfListItems();
	}

	Widget* ComboBox::getTargetWidget(const Ogre::Vector2& p)
	{
		if(!mVisible) return NULL;

		if(mLabel->isPointWithinBounds(p)) return mLabel;
		if(mButton->isPointWithinBounds(p)) return mButton;
		if(isPointWithinBounds(p)) return this;
		if(mList->getTargetWidget(p)) return mList->getTargetWidget(p);

		return NULL;
	}

	// Called when the user clicks outside the widget
	void ComboBox::deactivate(EventArgs& e)
	{
		// Restore default material
		mPanelOverlayElement->setMaterialName(mWidgetMaterial);
		mButton->applyDefaultMaterial();
		// If the Mouse has clicked on the ComboBox's List or ListItems, the widget should not *deactivate*.
		// As for hiding the list, this will be taken care of in the onMouseButtonDown handler.  The list needs
		// to remain visible so that ListItem picking works correctly. (If list is hidden, you can't click the ListItem..)
		Ogre::Vector2 p = MouseCursor::getSingleton().getPixelPosition();
		if(mList->isPointWithinBounds(p) || mButton->isPointWithinBounds(p)) return;
		
		mList->hide();

		Label::deactivate(e);
	}

	bool ComboBox::deactivate(const EventArgs& e)
	{
		// Restore default material
		mPanelOverlayElement->setMaterialName(mWidgetMaterial);
		mButton->applyDefaultMaterial();
		// If the Mouse has clicked on the ComboBox's List or ListItems, the widget should not *deactivate*.
		// As for hiding the list, this will be taken care of in the onMouseButtonDown handler.  The list needs
		// to remain visible so that ListItem picking works correctly. (If list is hidden, you can't click the ListItem..)
		Ogre::Vector2 p = MouseCursor::getSingleton().getPixelPosition();
		if(mList->isPointWithinBounds(p) || mButton->isPointWithinBounds(p)) return true;
		
		mList->hide();

		return true;
	}

	bool ComboBox::onMouseEnters(MouseEventArgs& e)
	{
		if(!mList->isVisible()) mPanelOverlayElement->setMaterialName(mWidgetMaterial+".over");
		if(mList->isVisible()) mButton->applyButtonDownMaterial();

		return Label::onMouseEnters(e);
	}

	bool ComboBox::onMouseLeaves(MouseEventArgs& e)
	{
		if(!mList->isVisible()) mPanelOverlayElement->setMaterialName(mWidgetMaterial);

		return Label::onMouseLeaves(e);
	}

	bool ComboBox::onMouseButtonDown(MouseEventArgs& e)
	{
		if( e.button == MB_Left )
		{
			Widget* w = mList->getTargetWidget(e.position);
			if( w != NULL)
			{
				e.widget = w;
				return onSelection(e);
			}
			else
			{
				if(!mList->isVisible())
				{
					// apply button ".down" material
					mPanelOverlayElement->setMaterialName(mWidgetMaterial+".down");
					mButton->applyButtonDownMaterial();
					mList->show();
				}
				else
				{
					mPanelOverlayElement->setMaterialName(mWidgetMaterial+".over");
					mButton->applyDefaultMaterial();
					mList->hide();
				}
			}
		}

		return Label::onMouseButtonDown(e);
	}

	bool ComboBox::onMouseButtonUp(MouseEventArgs& e)
	{
		if(mList->isVisible()) mButton->applyButtonDownMaterial();

		return Label::onMouseButtonUp(e);
	}

	bool ComboBox::onSelection(WidgetEventArgs& e)
	{
		setText(e.widget->getText());
		mPanelOverlayElement->setMaterialName(mWidgetMaterial);
		mButton->applyDefaultMaterial();
		mList->hide();

		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnSelectUserEventHandlers.begin(); it != mOnSelectUserEventHandlers.end(); ++it )
			e.handled = (*it)->execute(e);

		return e.handled;
	}

	void ComboBox::removeListItem(unsigned int index)
	{
		mList->removeListItem(index);
	}

	void ComboBox::setCharacterHeight(const Ogre::Real& relativeHeight)
	{
		Label::setCharacterHeight(relativeHeight);
		mList->setCharacterHeight(relativeHeight);
	}

	void ComboBox::setText(const Ogre::UTFString& text)
	{
		// If text is bigger than combobox width, append "..." to a fitting portion of the text
		mText = text;	// store original text so we can retrieve it.

		mLabel->setFont(mFont);
		mLabel->setCharacterHeight(mCharacterHeight);
		mLabel->setTextColor(mTextTopColor,mTextBotColor);
		mLabel->setText(mText);
		mLabel->alignText(mHorizontalAlignment,mVerticalAlignment);
	}

	void ComboBox::setDropListWidth(const Ogre::Real& relativeWidth)
	{
		Ogre::Vector4 relativeDimensions = mList->getRelativeDimensions();
		relativeDimensions.z = relativeWidth;
		mList->setDimensions(relativeDimensions);
	}

	void ComboBox::timeElapsed(Ogre::Real time)
	{
		mList->timeElapsed(time);
		Label::timeElapsed(time);
	}
}
