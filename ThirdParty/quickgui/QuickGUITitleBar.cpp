#include "QuickGUITitleBar.h"
#include "QuickGUIManager.h"

#include "QuickGUIWindow.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	TitleBar::TitleBar(const Ogre::String& name, const Rect& pixelDimensions, Ogre::String texture, GUIManager* gm) :
		Label(name,pixelDimensions,texture,gm)
	{		
		setQuadLayer(Quad::LAYER_MENU);
		mWidgetType = TYPE_TITLEBAR;
		mHorizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
		mHorizontalAlignment = HA_LEFT;

		// Create CloseButton
		Ogre::Real ButtonSize = mSize.height - 3;
		mCloseButton = new Button(mInstanceName+".CloseButton",Rect(mSize.width - ButtonSize,0,ButtonSize,ButtonSize),mTextureName + ".button" + mTextureExtension,mGUIManager);
		addChild(mCloseButton);
		mCloseButton->setHorizontalAnchor(ANCHOR_HORIZONTAL_RIGHT);
		mCloseButton->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);

		mTextBoundsRelativeSize = Size(mSize.width - ButtonSize,mSize.height) / mSize;
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
		mText->redraw();
	}

	void TitleBar::setCaption(const Ogre::UTFString& caption)
	{
		mText->setCaption(caption);
	}

	void TitleBar::setParent(Widget* parent)
	{
		mParentWidget = parent;

		if(mParentWidget != NULL)
		{
			_detectHierarchy();
			// set the correct offset
			setOffset(mParentWidget->getOffset() + 1);
			setSize(mSize);
			setPosition(mPosition);
			// calculated properties
			Size parentSize = mParentWidget->getSize();
			mPixelsFromParentRight = parentSize.width - (mPosition.x + mSize.width);
			mPixelsFromParentBottom = parentSize.height - (mPosition.y + mSize.height);
			setClippingWidget(mParentWidget,true);
			// inheritted properties
			if(!mParentWidget->isVisible())
				hide();
			mGainFocusOnClick = mParentWidget->getGainFocusOnClick();

			mCloseButton->addEventHandler(Widget::EVENT_MOUSE_CLICK,&Window::hide,dynamic_cast<Window*>(mParentWidget));
			mCloseButton->addEventHandler(Widget::EVENT_MOUSE_BUTTON_UP,&Window::hide,dynamic_cast<Window*>(mParentWidget));
		}
	}

	void TitleBar::showCloseButton()
	{
		mCloseButton->show();
		mTextBoundsRelativeSize.width = (mSize.width - mCloseButton->getWidth()) / mSize.width;
	}
}
