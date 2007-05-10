#include "SimpleGUIComboBox.h"
#include "SimpleGUIMouseCursor.h"

namespace SimpleGUI
{
	ComboBox::ComboBox(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, const Ogre::String& highlightMaterial, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,dimensions,material,show,overlayContainer,ParentWidget),
		mListItemHorizontalAlignment(SimpleGUI::SGUI_HA_MID),
		mListItemVerticalAlignment(SimpleGUI::SGUI_VA_MID)
	{
		mWidgetType = Widget::SGUI_COMBOBOX;

		// Create CloseButton - remember to position it relative to it's parent (TitleBar)
		// Height of the Title Bar
		Ogre::Real height = (mAbsoluteDimensions.w / mAbsoluteDimensions.z);
		// Button has same height as width - Make the button slightly smaller that the titlebar height
		Ogre::Real buttonHeight = 0.8;
		Ogre::Real buttonWidth = (height * buttonHeight);
		// Make a 5 pixel buffer
		Ogre::Real buffer = 5.0 / mPixelDimensions.z;
		Ogre::Vector4 bDimensions = Ogre::Vector4((1 - (buttonWidth + buffer)),0.1,buttonWidth,buttonHeight);
		mButton = new Button(mInstanceName+"_ComboButton",bDimensions,mMaterial+".button",show,mChildrenContainer,this);

		// Make a 15 pixel buffer for the Label
		Ogre::Real bigBuffer = 15.0 / mPixelDimensions.z;
		mLabel = new Label(mInstanceName+"_Label",Ogre::Vector4(bigBuffer,0,(1 - (buttonWidth + bigBuffer)),1),"",show,mChildrenContainer,this);

		mList = new List(mInstanceName+"_list",Ogre::Vector3(0,1.0,1.0),mMaterial+".list",highlightMaterial,false,mOverlayContainer,this);
		mList->setCharacterHeight(mCharacterHeight);
		mList->setFont(mFont);
		mList->setTextColor(mTextTopColor,mTextBotColor);
	}

	ComboBox::~ComboBox()
	{
		std::vector<MemberFunctionSlot*>::iterator it;
		for( it = mOnSelectUserEventHandlers.begin(); it != mOnSelectUserEventHandlers.end(); ++it )
			delete (*it);
		mOnSelectUserEventHandlers.clear();

		delete mLabel;
		delete mButton;
		delete mList;
	}

	void ComboBox::_notifyDimensionsChanged()
	{
		Label::_notifyDimensionsChanged();

		mLabel->_notifyDimensionsChanged();
		mButton->_notifyDimensionsChanged();
		mList->_notifyDimensionsChanged();
	}

	void ComboBox::addListItem(const Ogre::String& text)
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
		mPanelOverlayElement->setMaterialName(mMaterial);
		mButton->applyDefaultMaterial();
		// If the Mouse has clicked on the ComboBox's List or ListItems, the widget should not *deactivate*.
		// As for hiding the list, this will be taken care of in the onMouseButtonDown handler.  The list needs
		// to remain visible so that ListItem picking works correctly. (If list is hidden, you can't click the ListItem..)
		Ogre::Vector2 p = MouseCursor::getSingleton().getPixelPosition();
		if(mList->isPointWithinBounds(p) || mButton->isPointWithinBounds(p)) return;
		
		mList->hide();

		Label::deactivate(e);
	}

	bool ComboBox::onMouseEnters(MouseEventArgs& e)
	{
		if(!mList->isVisible()) mPanelOverlayElement->setMaterialName(mMaterial+".over");
		if(mList->isVisible()) mButton->applyButtonDownMaterial();

		return Label::onMouseEnters(e);
	}

	bool ComboBox::onMouseLeaves(MouseEventArgs& e)
	{
		if(!mList->isVisible()) mPanelOverlayElement->setMaterialName(mMaterial);

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
					mPanelOverlayElement->setMaterialName(mMaterial+".down");
					mButton->applyButtonDownMaterial();
					mList->show();
				}
				else
				{
					mPanelOverlayElement->setMaterialName(mMaterial+".over");
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
		mPanelOverlayElement->setMaterialName(mMaterial);
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

	void ComboBox::setText(const Ogre::String& text)
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
