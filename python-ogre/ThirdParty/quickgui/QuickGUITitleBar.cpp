#include "QuickGUITitleBar.h"
#include "QuickGUIManager.h"

#include "QuickGUIWindow.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	TitleBar::TitleBar(const Ogre::String& name, Type type, const Rect& dimensions, GuiMetricsMode pMode, GuiMetricsMode sMode, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Label(name,type,dimensions,pMode,sMode,texture,container,ParentWidget,gm)
	{
		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_TITLEBAR)
		{
			mQuad->setLayer(Quad::LAYER_MENU);
		}

		mText->setLayer(Quad::LAYER_MENU);
		mHorizontalAlignment = QGUI_HA_LEFT;

		// Create CloseButton
		// Button has same height as width - Make the button slightly smaller that the titlebar height
		Ogre::Real buttonHeight = 0.8;
		Ogre::Real buttonPixelHeight = 0.8 * mPixelDimensions.height;
		Ogre::Real verticalPixelSpace = 0.1 * mPixelDimensions.height;
		mRelativeButtonWidth = buttonPixelHeight / mPixelDimensions.width;
		Ogre::Real horizontalPixelSpace = verticalPixelSpace / mPixelDimensions.width;

		Rect cbDimensions = Rect((1 - (mRelativeButtonWidth + horizontalPixelSpace)),0.1,mRelativeButtonWidth,buttonHeight);
		mCloseButton = new Button(mInstanceName+".CloseButton",TYPE_BUTTON,cbDimensions,QGUI_GMM_RELATIVE,QGUI_GMM_RELATIVE,mTextureName + ".button" + mTextureExtension,mQuadContainer,this,mGUIManager);
		mCloseButton->getQuad()->setLayer(Quad::LAYER_MENU);
		mCloseButton->addEventHandler(Widget::EVENT_MOUSE_CLICK,&Window::hide,dynamic_cast<Window*>(mParentWidget));
		mCloseButton->addEventHandler(Widget::EVENT_MOUSE_BUTTON_UP,&Window::hide,dynamic_cast<Window*>(mParentWidget));

		mTextBoundsRelativeSize = Size(1.0 - mRelativeButtonWidth,1);
	}

	TitleBar::~TitleBar()
	{
		Widget::removeAndDestroyAllChildWidgets();
		mCloseButton = NULL;
	}

	Button* TitleBar::getCloseButton()
	{
		return mCloseButton;
	}

	void TitleBar::hideCloseButton()
	{
		mCloseButton->hide();
		mTextBoundsRelativeSize.width = 1;
		mText->refresh();
	}

	void TitleBar::setCaption(const Ogre::UTFString& caption)
	{
		mText->setCaption(caption);
	}

	void TitleBar::showCloseButton()
	{
		mCloseButton->show();
		mTextBoundsRelativeSize.width = 1.0 - mRelativeButtonWidth;
	}
}
