#include "QuickGUITitleBar.h"
#include "QuickGUIManager.h"

#include "QuickGUIWindow.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	TitleBar::TitleBar(const Ogre::String& name, Type type, const Rect& pixelDimensions, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Label(name,type,pixelDimensions,texture,container,ParentWidget,gm)
	{
		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_TITLEBAR)
		{
			mQuad->setLayer(Quad::LAYER_MENU);
			mText->setLayer(Quad::LAYER_MENU);
		}
		
		mHorizontalAlignment = QGUI_HA_LEFT;

		// Create CloseButton
		// Button has same height as width - Make the button slightly smaller that the titlebar height
		Ogre::Real buttonHeight = 0.8;
		Ogre::Real buttonPixelHeight = 0.8 * mSize.height;
		Ogre::Real verticalPixelSpace = 0.1 * mSize.height;
		mRelativeButtonWidth = buttonPixelHeight / mSize.width;
		Ogre::Real horizontalPixelSpace = verticalPixelSpace / mSize.width;

		Ogre::Real ButtonSize = mSize.height - 3;
		mCloseButton = new Button(mInstanceName+".CloseButton",TYPE_BUTTON,Rect(mSize.width - ButtonSize,0,ButtonSize,ButtonSize),mTextureName + ".button" + mTextureExtension,mQuadContainer,this,mGUIManager);
		mCloseButton->getQuad()->setLayer(Quad::LAYER_MENU);
		mCloseButton->addEventHandler(Widget::EVENT_MOUSE_CLICK,&Window::hide,dynamic_cast<Window*>(mParentWidget));
		mCloseButton->addEventHandler(Widget::EVENT_MOUSE_BUTTON_UP,&Window::hide,dynamic_cast<Window*>(mParentWidget));

		mTextBoundsPixelSize = Size(getScreenPosition().x - ButtonSize,mSize.height);
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
		mTextBoundsPixelSize.width = mSize.width;
		mText->refresh();
	}

	void TitleBar::setCaption(const Ogre::UTFString& caption)
	{
		mText->setCaption(caption);
	}

	void TitleBar::showCloseButton()
	{
		mCloseButton->show();
		mTextBoundsPixelSize.width = mSize.width - mCloseButton->getWidth();
	}
}
