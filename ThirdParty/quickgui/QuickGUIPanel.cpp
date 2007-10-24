#include "QuickGUIManager.h"
#include "QuickGUIPanel.h"
#include "QuickGUISheet.h"

#ifdef WIN32
// Turn off warnings generated by using 'this' in base member initializer list.
#   pragma warning (disable : 4355)
#endif

namespace QuickGUI
{
	Panel::Panel(const Ogre::String& instanceName, const Size& pixelSize, Ogre::String texture, GUIManager* gm) :
		Image(instanceName,pixelSize,texture,gm),
		QuadContainer(this),
		mScrollingAllowed(false),
		mScrollPane(0),
		mRightScrollBar(0),
		mBottomScrollBar(0)
	{
		mWidgetType = TYPE_PANEL;

		addEventHandler(EVENT_CHILD_ADDED,&Panel::onChildAdded,this);
		addEventHandler(EVENT_CHILD_REMOVED,&Panel::onChildRemoved,this);
	}

	Panel::~Panel()
	{
		if(mScrollPane != NULL)
			delete mScrollPane;
		mScrollPane = NULL;

		Widget::removeAndDestroyAllChildWidgets();
		setQuadContainer(NULL);
	}

	void Panel::addChild(Widget* w)
	{
		if(w->getParentWidget() != NULL)
			return;

		mChildWidgets.push_back(w);
		
		// The setParent method sets the widget's quad container to the
		// quad container the parent belongs to.  However, Sheets, Panels, and Windows
		// are quad containers themselves. So temporarily set mQuadContainer to ourself,
		// make the call to setParent, and then restore the mQuadContainer reference.
		QuadContainer* temp = mQuadContainer;
		mQuadContainer = this;
		w->setParent(this);
		mQuadContainer = temp;

		// Convert Widget's position to be relative to new parent.
		if(mScrollPane != NULL)
			w->setPosition(w->getScreenPosition() - mScrollPane->getScreenPosition());
		else
			w->setPosition(w->getScreenPosition() - getScreenPosition());

		WidgetEventArgs args(w);
		fireEvent(EVENT_CHILD_ADDED,args);
	}

	void Panel::allowScrolling(bool allow)
	{
		mScrollingAllowed = allow;

		if(mScrollingAllowed)
		{
			if(mScrollPane == NULL)
			{
				mScrollPane = new ScrollPane(mInstanceName+".ScrollPane",mSize,mGUIManager);
				addChild(mScrollPane);
				mScrollPane->setPosition(0,0);

				mRightScrollBar = mScrollPane->mRightBar;
				addChild(mRightScrollBar);
				mRightScrollBar->setPosition(mSize.width - 20,0);
				
				mBottomScrollBar = mScrollPane->mBottomBar;
				addChild(mBottomScrollBar);
				mBottomScrollBar->setPosition(0,mSize.height - 20);

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

	Button* Panel::createButton()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_BUTTON);
		mGUIManager->notifyNameUsed(name);

		Button* newButton = new Button(name,Size(75,25),"qgui.button.png",mGUIManager);
		addChild(newButton);
		newButton->setPosition(0,0);
		newButton->setFont(mFontName,true);

		return newButton;
	}

	ComboBox* Panel::createComboBox()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_COMBOBOX);
		mGUIManager->notifyNameUsed(name);

		ComboBox* newComboBox = new ComboBox(name,Size(125,25),"qgui.combobox.png",mGUIManager);
		addChild(newComboBox);
		newComboBox->setPosition(0,0);
		newComboBox->setFont(mFontName,true);

		return newComboBox;
	}

	Console* Panel::createConsole()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_CONSOLE);
		mGUIManager->notifyNameUsed(name);

		Console* newConsole = new Console(name,Size(200,100),"qgui.console.png",mGUIManager);
		addChild(newConsole);
		newConsole->setPosition(0,0);
		newConsole->setFont(mFontName,true);

		return newConsole;
	}

	HorizontalScrollBar* Panel::createHorizontalScrollBar()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_SCROLLBAR_HORIZONTAL);
		mGUIManager->notifyNameUsed(name);

		HorizontalScrollBar* newHorizontalScrollBar = new HorizontalScrollBar(name,Size(100,50),"qgui.scrollbar.horizontal.png",mGUIManager);
		addChild(newHorizontalScrollBar);
		newHorizontalScrollBar->setPosition(0,0);
		newHorizontalScrollBar->setFont(mFontName,true);

		return newHorizontalScrollBar;
	}

	HorizontalTrackBar* Panel::createHorizontalTrackBar()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_SCROLLBAR_VERTICAL);
		mGUIManager->notifyNameUsed(name);

		HorizontalTrackBar* newHorizontalTrackBar = new HorizontalTrackBar(name,Size(80,20),"qgui.trackbar.horizontal.png",mGUIManager);
		addChild(newHorizontalTrackBar);
		newHorizontalTrackBar->setPosition(0,0);
		newHorizontalTrackBar->setFont(mFontName,true);

		return newHorizontalTrackBar;
	}

	Image* Panel::createImage()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_IMAGE);
		mGUIManager->notifyNameUsed(name);

		Image* newImage = new Image(name,Size(50,50),"qgui.image.png",mGUIManager);
		addChild(newImage);
		newImage->setPosition(0,0);
		newImage->setFont(mFontName,true);

		return newImage;
	}

	Label* Panel::createLabel()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_LABEL);
		mGUIManager->notifyNameUsed(name);

		Label* newLabel = new Label(name,Size::ZERO,"qgui.label.png",mGUIManager);
		addChild(newLabel);
		newLabel->setPosition(0,0);
		newLabel->setFont(mFontName,true);

		return newLabel;
	}

	List* Panel::createList()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_LIST);
		mGUIManager->notifyNameUsed(name);

		List* newList = new List(name,Size(120,100),"qgui.list.png",mGUIManager);
		addChild(newList);
		newList->setPosition(0,0);
		newList->setFont(mFontName,true);

		return newList;
	}

	LabelArea* Panel::createMultiLineLabel()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_LABELAREA);
		mGUIManager->notifyNameUsed(name);

		LabelArea* newMultiLineLabel = new LabelArea(name,Size(300,100),"qgui.LabelArea.png",mGUIManager);
		addChild(newMultiLineLabel);
		newMultiLineLabel->setPosition(0,0);
		newMultiLineLabel->setFont(mFontName,true);

		return newMultiLineLabel;
	}

	NStateButton* Panel::createNStateButton()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_NSTATEBUTTON);
		mGUIManager->notifyNameUsed(name);

		NStateButton* newNStateButton = new NStateButton(name,Size(75,25),mGUIManager);
		addChild(newNStateButton);
		newNStateButton->setPosition(0,0);
		newNStateButton->setFont(mFontName,true);

		return newNStateButton;
	}

	Panel* Panel::createPanel()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_PANEL);
		mGUIManager->notifyNameUsed(name);

		Panel* newPanel = new Panel(name,Size(200,100),"qgui.panel.png",mGUIManager);
		addChild(newPanel);
		newPanel->setPosition(0,0);
		newPanel->setFont(mFontName,true);

		return newPanel;
	}

	ProgressBar* Panel::createProgressBar()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_PROGRESSBAR);
		mGUIManager->notifyNameUsed(name);

		ProgressBar* newProgressBar = new ProgressBar(name,Size(100,25),"qgui.progressbar.png",mGUIManager);
		addChild(newProgressBar);
		newProgressBar->setPosition(0,0);
		newProgressBar->setFont(mFontName,true);

		return newProgressBar;
	}

	TextBox* Panel::createTextBox()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_TEXTBOX);
		mGUIManager->notifyNameUsed(name);

		TextBox* newTextBox = new TextBox(name,Size(100,0),"qgui.textbox.png",mGUIManager);
		addChild(newTextBox);
		newTextBox->setPosition(0,0);
		newTextBox->setFont(mFontName,true);

		return newTextBox;
	}

	VerticalScrollBar* Panel::createVerticalScrollBar()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_SCROLLBAR_VERTICAL);
		mGUIManager->notifyNameUsed(name);

		VerticalScrollBar* newVerticalScrollBar = new VerticalScrollBar(name,Size(50,100),"qgui.scrollbar.vertical.png",mGUIManager);
		addChild(newVerticalScrollBar);
		newVerticalScrollBar->setPosition(0,0);
		newVerticalScrollBar->setFont(mFontName,true);

		return newVerticalScrollBar;
	}

	VerticalTrackBar* Panel::createVerticalTrackBar()
	{
		Ogre::String name = mGUIManager->generateName(TYPE_TRACKBAR_VERTICAL);
		mGUIManager->notifyNameUsed(name);

		VerticalTrackBar* newVerticalTrackBar = new VerticalTrackBar(name,Size(20,80),"qgui.trackbar.vertical.png",mGUIManager);
		addChild(newVerticalTrackBar);
		newVerticalTrackBar->setPosition(0,0);
		newVerticalTrackBar->setFont(mFontName,true);

		return newVerticalTrackBar;
	}

	Widget* Panel::getTargetWidget(const Point& pixelPosition)
	{
		if( !mVisible || !mEnabled )
			return NULL;

		if( mCanResize )
		{
			// Check borders
			for(int i = 0; i < 8; ++i)
			{
				if((mBorders[i] != NULL) && (mBorders[i]->isPointWithinBounds(pixelPosition)))
					return mBorders[i];
			}
		}

		// If position is not inside this widget, it can't be inside a child widget. (except menus, which are handled differently)
		if(!isPointWithinBounds(pixelPosition))
			return NULL;

		Widget* w = NULL;

		// Iterate through Menu Layer Child Widgets.
		int widgetOffset = 0;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getQuadLayer() == Quad::LAYER_CHILD )
				continue;

			Widget* temp = (*it)->getTargetWidget(pixelPosition);
			if( (temp != NULL) && (temp->getOffset() > widgetOffset) )
			{
				widgetOffset = temp->getOffset();
				w = temp;
			}
		}
		if(w != NULL)
			return w;

		// Iterate through Panels, from highest offset to lowest.
		std::list<QuadContainer*>* panelList = QuadContainer::getPanelList();
		std::list<QuadContainer*>::reverse_iterator rit;
		for( rit = panelList->rbegin(); rit != panelList->rend(); ++rit )
		{
			if( (w = (*rit)->getOwner()->getTargetWidget(pixelPosition)) != NULL )
				return w;
		}

		// Iterate through Child Layer Child Widgets.
		widgetOffset = 0;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getQuadLayer() == Quad::LAYER_MENU )
				continue;

			Widget* temp = (*it)->getTargetWidget(pixelPosition);
			if( (temp != NULL) && (temp->getOffset() > widgetOffset) )
			{
				widgetOffset = temp->getOffset();
				w = temp;
			}
		}
		if(w != NULL)
			return w;

		return this;
	}

	void Panel::onChildAdded(const EventArgs& args)
	{
		if(mScrollPane != NULL)
			mScrollPane->onChildAddedToParent(args);
	}

	void Panel::onChildRemoved(const EventArgs& args)
	{
		if(mScrollPane != NULL)
			mScrollPane->onChildRemovedFromParent(args);
	}

	void Panel::onSizeChanged(const EventArgs& args)
	{
		Image::onSizeChanged(args);

		if(mScrollPane != NULL)
			mScrollPane->onParentSizeChanged(args);
	}

	void Panel::setQuadContainer(QuadContainer* c)
	{
		if((mQuadContainer != NULL) && (c != mQuadContainer))
			mQuadContainer->removeChildPanelContainer(this);

		mQuadContainer = c;

		if(mQuadContainer != NULL)
			mQuadContainer->addChildPanelContainer(this);
	}

	bool Panel::scrollingAllowed()
	{
		return mScrollingAllowed;
	}

	void Panel::show()
	{
		Image::show();

		if(mScrollPane != NULL)
			mScrollPane->_syncBarWithParentDimensions();
	}
}
