#include "QuickGUIWindow.h"
// included to get access to default font/fontsize
#include "QuickGUIManager.h"

namespace QuickGUI
{
	Window::Window(const Ogre::String& name, Type type, const Rect& pixelDimensions, Ogre::String texture, QuadContainer* container, Widget* parentWidget, GUIManager* gm) :
		Panel(name,type,pixelDimensions,texture,container,parentWidget,gm),
		mTitleBar(0),
		mBringToFrontOnFocus(true)
	{
		mShowWithParent = false;
		addEventHandler(EVENT_GAIN_FOCUS,&Window::onGainFocus,this);

		// Create TitleBar - tradition titlebar dimensions: across the top of the window
		mTitleBar = new TitleBar(mInstanceName+".Titlebar",TYPE_TITLEBAR,Rect(0,0,mSize.width,25),mTextureName + ".titlebar" + mTextureExtension,this,this,mGUIManager);
		mTitleBar->enableDragging(true);
		mTitleBar->setDraggingWidget(this);

		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_WINDOW)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);
			mQuad->_notifyQuadContainer(this);
			mQuadContainer->addChildWindowContainer(this);

			mScrollPane = new ScrollPane(mInstanceName+".ScrollPane",TYPE_SCROLL_PANE,this,this,mGUIManager);
		}
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
	}

	void Window::setBringToFrontOnFocus(bool BringToFront)
	{
		mBringToFrontOnFocus = BringToFront;
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
	}
}
