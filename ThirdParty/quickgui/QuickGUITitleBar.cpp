#include "QuickGUITitleBar.h"
#include "QuickGUIManager.h"

#include "QuickGUIWindow.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	TitleBar::TitleBar(const Ogre::String& instanceName, const Size& pixelSize, Ogre::String texture, GUIManager* gm) :
		Label(instanceName,pixelSize,texture,gm)
	{	
		mWidgetType = TYPE_TITLEBAR;
		mScrollPaneAccessible = false;
		setQuadLayer(Quad::LAYER_MENU);
		mInheritQuadLayer = false;
		mHorizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;
		mHorizontalAlignment = HA_LEFT;

		// Create CloseButton
		Ogre::Real ButtonSize = mSize.height - 3;
		mCloseButton = new Button(mInstanceName+".CloseButton",Size(ButtonSize,ButtonSize),mTextureName + ".button" + mTextureExtension,mGUIManager);
		addChild(mCloseButton);
		mCloseButton->setPosition(mSize.width - ButtonSize,0);
		mCloseButton->setAutoSize(false);
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
			setQuadContainer(parent->getQuadContainer());
			setGUIManager(parent->getGUIManager());

			_detectHierarchy();
			// set the correct offset
			setOffset(mParentWidget->getOffset() + 1);
			// calculated properties
			_deriveAnchorValues();
			// inheritted properties
			if(mInheritClippingWidget)
				setClippingWidget(mParentWidget,true);
			if(!mParentWidget->isVisible())
				hide();
			if(mInheritQuadLayer)
				setQuadLayer(mParentWidget->getQuadLayer());
			mGainFocusOnClick = mParentWidget->getGainFocusOnClick();

			mCloseButton->addEventHandler(Widget::EVENT_MOUSE_CLICK,&Window::onMouseUpOverCloseButton,dynamic_cast<Window*>(mParentWidget));
			mCloseButton->addEventHandler(Widget::EVENT_MOUSE_BUTTON_UP,&Window::onMouseUpOverCloseButton,dynamic_cast<Window*>(mParentWidget));
		}

		WidgetEventArgs args(this);
		fireEvent(EVENT_PARENT_CHANGED,args);
	}

	void TitleBar::showCloseButton()
	{
		mCloseButton->show();
		mTextBoundsRelativeSize.width = (mSize.width - mCloseButton->getWidth()) / mSize.width;
	}
}
