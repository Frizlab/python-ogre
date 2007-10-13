#include "QuickGUIWindow.h"
// included to get access to default font/fontsize
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Window::Window(const Ogre::String& instanceName, const Size& pixelSize, Ogre::String texture, GUIManager* gm) :
		Panel(instanceName,pixelSize,texture,gm),
		mTitleBar(0),
		mBringToFrontOnFocus(true)
	{
		mWidgetType = TYPE_WINDOW;
		mShowWithParent = false;
		mCanResize = true;
		addEventHandler(EVENT_GAIN_FOCUS,&Window::onGainFocus,this);

		// Create TitleBar - tradition titlebar dimensions: across the top of the window
		mGUIManager->notifyNameUsed(mInstanceName + ".TitleBar");
		mTitleBar = new TitleBar(mInstanceName + ".TitleBar",Size(mSize.width,25),mTextureName + ".titlebar" + mTextureExtension,mGUIManager);
		addChild(mTitleBar);
		mTitleBar->setPosition(0,0);
		mTitleBar->enableDragging(true);
		mTitleBar->setDraggingWidget(this);

		setUseBorders(true);
	}

	Window::~Window()
	{
	}

	void Window::allowScrolling(bool allow)
	{
		mScrollingAllowed = allow;

		if(mScrollingAllowed)
		{
			if(mScrollPane == NULL)
			{
				mScrollPane = new ScrollPane(mInstanceName+".ScrollPane",Size(mSize.width,mSize.height),mGUIManager);
				addChild(mScrollPane);
				mScrollPane->setPosition(0,0);

				mRightScrollBar = mScrollPane->mRightBar;
				addChild(mRightScrollBar);
				mRightScrollBar->setPosition(mSize.width - 20,0);

				mBottomScrollBar = mScrollPane->mBottomBar;
				addChild(mBottomScrollBar);
				mBottomScrollBar->setPosition(0,mSize.height - 20);

				if(mTitleBar->isVisible())
				{
					mRightScrollBar->setYPosition(mTitleBar->getHeight());
					mRightScrollBar->setHeight(mRightScrollBar->getHeight() - mTitleBar->getHeight());
				}

				mScrollPane->manageWidgets();
			}
		}
		else
		{
			if(mScrollPane != NULL)
			{
				delete mScrollPane;
				mScrollPane = NULL;

				mGUIManager->destroyWidget(mRightScrollBar);
				mRightScrollBar = NULL;
				mGUIManager->destroyWidget(mBottomScrollBar);
				mBottomScrollBar = NULL;
			}
		}
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

		if(mRightScrollBar)
		{
			mRightScrollBar->setYPosition(0);
			mRightScrollBar->setHeight(mRightScrollBar->getHeight() + mTitleBar->getHeight());
		}

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

		mRightScrollBar->setYPosition(mTitleBar->getHeight());
		mRightScrollBar->setHeight(mRightScrollBar->getHeight() - mTitleBar->getHeight());

		Ogre::Real titlebarHeight = mTitleBar->getHeight();
		setYPosition(mPosition.y + titlebarHeight);
		setHeight(mSize.height - titlebarHeight);
	}
}
