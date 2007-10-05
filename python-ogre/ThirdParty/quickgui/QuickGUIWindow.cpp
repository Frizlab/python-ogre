#include "QuickGUIWindow.h"
// included to get access to default font/fontsize
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Window::Window(const Ogre::String& name, const Rect& pixelDimensions, Ogre::String texture, GUIManager* gm) :
		Panel(name,Rect(pixelDimensions.x,pixelDimensions.y,pixelDimensions.width,pixelDimensions.height),texture,gm),
		mTitleBar(0),
		mBringToFrontOnFocus(true)
	{
		mWidgetType = TYPE_WINDOW;
		mShowWithParent = false;
		addEventHandler(EVENT_GAIN_FOCUS,&Window::onGainFocus,this);

		// Create TitleBar - tradition titlebar dimensions: across the top of the window
		mTitleBar = new TitleBar(mInstanceName+".Titlebar",Rect(0,0,mSize.width,25),mTextureName + ".titlebar" + mTextureExtension,mGUIManager);
		addChild(mTitleBar);
		mTitleBar->enableDragging(true);
		mTitleBar->setDraggingWidget(this);

		setUseBorders(true);
	}

	Window::~Window()
	{
	}

	void Window::onGainFocus(const EventArgs& args)
	{
		if(mBringToFrontOnFocus)
			mQuadContainer->moveWindowGroupToEnd(this);
	}

	void Window::bringToFront()
	{
		mQuadContainer->moveWindowGroupToEnd(this);
	}

	bool Window::getBringToFrontOnFocus()
	{
		return mBringToFrontOnFocus;
	}

	TitleBar* Window::getTitleBar()
	{
		return mTitleBar;
	}

	void Window::hide()
	{
		Panel::hide();
	}

	void Window::hide(const EventArgs& args)
	{
		Panel::hide();
	}

	void Window::hideCloseButton()
	{
		mTitleBar->hideCloseButton();
	}

	void Window::hideTitlebar()
	{
		mTitleBar->hide();
		mTitleBar->setShowWithParent(false);

		Ogre::Real titlebarHeight = mTitleBar->getHeight();
		setYPosition(mPosition.y - titlebarHeight);
		setHeight(mSize.height + titlebarHeight);
	}

	void Window::setBringToFrontOnFocus(bool BringToFront)
	{
		mBringToFrontOnFocus = BringToFront;
	}

	void Window::setQuadContainer(QuadContainer* container)
	{
		if((mQuadContainer != NULL) && (mQuadContainer->getID() != mQuadContainer->getID()))
			mQuadContainer->removeChildWindowContainer(QuadContainer::getID());

		mQuadContainer = container;
		mQuadContainer->addChildWindowContainer(this);
	}

	void Window::show()
	{
		Panel::show();
	}

	void Window::showCloseButton()
	{
		mTitleBar->showCloseButton();
	}

	void Window::showTitlebar()
	{
		mTitleBar->show();
		mTitleBar->setShowWithParent(true);

		Ogre::Real titlebarHeight = mTitleBar->getHeight();
		setYPosition(mPosition.y + titlebarHeight);
		setHeight(mSize.height - titlebarHeight);
	}
}
