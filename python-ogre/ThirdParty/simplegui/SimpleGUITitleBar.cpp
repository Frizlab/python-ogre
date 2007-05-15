#include "SimpleGUITitleBar.h"
#include "SimpleGUIManager.h"

#include "SimpleGUIWindow.h"
#include "SimpleGUIMouseCursor.h"

namespace SimpleGUI
{
	TitleBar::TitleBar(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,dimensions,material,show,overlayContainer,ParentWidget)
	{
		mWidgetType = Widget::SGUI_TITLEBAR;

		setCharacterHeight(0.8);
		alignText(SGUI_HA_LEFT,SGUI_VA_MID);

		// Create CloseButton - remember to position it relative to it's parent (TitleBar)
		// Height of the Title Bar
		Ogre::Real titleBarHeight = (mAbsoluteDimensions.w / mAbsoluteDimensions.z);
		// Button has same height as width - Make the button slightly smaller that the titlebar height
		Ogre::Real buttonHeight = 0.8;
		Ogre::Real buttonWidth = (titleBarHeight * buttonHeight);
		Ogre::Vector4 cbDimensions = Ogre::Vector4((1 - (buttonWidth * 1.1)),0.1,buttonWidth,buttonHeight);
		mCloseButton = new Button(mInstanceName+"_CloseButton",cbDimensions,mMaterial+".button",show,mChildrenContainer,this);
		mCloseButton->addEventHandler(Widget::SGUI_MOUSE_CLICK,&Window::hide,dynamic_cast<Window*>(mParentWidget));
		mCloseButton->addEventHandler(Widget::SGUI_MOUSE_BUTTON_UP,&Window::hide,dynamic_cast<Window*>(mParentWidget));
		
		if(show) this->show();
		else hide();
	}

	TitleBar::~TitleBar()
	{
		// destroy close button
		delete mCloseButton;
		mCloseButton = NULL;
	}

	void TitleBar::_notifyDimensionsChanged()
	{
		_updateDimensions(mRelativeDimensions);
		_applyDimensions();

		mTextWidget->_notifyDimensionsChanged();
		mCloseButton->_notifyDimensionsChanged();
	}

	Button* TitleBar::getCloseButton()
	{
		return mCloseButton;
	}

	Widget* TitleBar::getTargetWidget(const Ogre::Vector2& p)
	{		
		if(mCloseButton && (mCloseButton->isPointWithinBounds(p)) ) return mCloseButton;

		if(isPointWithinBounds(p)) return this;

		return NULL;
	}

	void TitleBar::hide()
	{
		mPanelOverlayElement->hide();
		mTextWidget->hide();
		mCloseButton->hide();
		
		Label::hide();
	}

	void TitleBar::hideCloseButton()
	{
		mCloseButton->hide();
	}

	void TitleBar::show()
	{
		mPanelOverlayElement->show();
		mTextWidget->show();
		mCloseButton->show();

		Label::show();
	}

	void TitleBar::showCloseButton()
	{
		mCloseButton->show();
	}

	void TitleBar::timeElapsed(Ogre::Real time)
	{
		mCloseButton->timeElapsed(time);
		Label::timeElapsed(time);
	}
}
