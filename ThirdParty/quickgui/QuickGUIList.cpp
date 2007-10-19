#include "QuickGUIList.h"
#include "QuickGUIManager.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	List::List(const Ogre::String& instanceName, const Size& pixelSize, Ogre::String texture, GUIManager* gm) :
		Image(instanceName,pixelSize,texture,gm),
		mScrollPane(0),
		mAutoNameWidgetCounter(0),
		mScrollingAllowed(false),
		mItemHeight(20),
		mAutoSizeListItems(true),
		mVPixelPadHeight(10)
	{
		mWidgetType = TYPE_LIST;

		mTextUtilities = new Text(mInstanceName+".TextUtilities",mQuadContainer,this);
		mTextUtilities->disable();

		addEventHandler(EVENT_CHILD_ADDED,&List::onChildAdded,this);
		addEventHandler(EVENT_CHILD_REMOVED,&List::onChildRemoved,this);

		mItems.clear();
	}

	List::~List()
	{
		delete mTextUtilities;

		mItems.clear();
	}

	MenuLabel* List::addMenuLabel()
	{
		Point p(0,(mAutoNameWidgetCounter * mItemHeight));
		Size s(mSize.width,mItemHeight);

		++mAutoNameWidgetCounter;

		Ogre::String name = mInstanceName+".Item"+Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		mGUIManager->notifyNameUsed(name);

		MenuLabel* newMenuLabel = new MenuLabel(name,s,"qgui.menulabel.png",mGUIManager);
		addChild(newMenuLabel);
		newMenuLabel->setPosition(p);
		newMenuLabel->setAutoSize(false);
		newMenuLabel->setFont(mFontName,true);

		if(!mVisible)
			newMenuLabel->hide();

		mItems.push_back(newMenuLabel);

		return newMenuLabel;
	}

	TextBox* List::addTextBox()
	{
		Point p(0,(mAutoNameWidgetCounter * mItemHeight));
		Size s(mSize.width,mItemHeight);

		++mAutoNameWidgetCounter;

		Ogre::String name = mInstanceName+".Item"+Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		mGUIManager->notifyNameUsed(name);

		TextBox* newTextBox = new TextBox(name,s,"qgui.textbox.png",mGUIManager);
		addChild(newTextBox);
		newTextBox->setUseBorders(false);
		newTextBox->setPosition(p);
		newTextBox->setAutoSize(false);
		newTextBox->setFont(mFontName,true);

		if(!mVisible)
			newTextBox->hide();

		mItems.push_back(newTextBox);

		return newTextBox;
	}

	void List::allowScrolling(bool allow)
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
				mRightScrollBar->setOffset(mOffset+2);
				addChild(mRightScrollBar);
				mRightScrollBar->setPosition(mSize.width - 20,0);
				
				mBottomScrollBar = mScrollPane->mBottomBar;
				mBottomScrollBar->setOffset(mOffset+2);
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

	void List::clear()
	{
		std::vector<Widget*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
			mGUIManager->destroyWidget((*it));
		mItems.clear();
	}

	bool List::getAutoSizeListItems()
	{
		return mAutoSizeListItems;
	}

	Widget* List::getItem(unsigned int index)
	{
		if( index >= mItems.size() )
			return NULL;

		return mItems[index];
	}

	int List::getItemIndex(Widget* w)
	{
		Ogre::String name = w->getInstanceName();

		int counter = 0;
		std::vector<Widget*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			if( name == (*it)->getInstanceName() )
				return counter;

			++counter;
		}

		return -1;
	}

	int List::getNumberOfItems()
	{
		return static_cast<int>(mItems.size());
	}

	ScrollPane* List::getScrollPane()
	{
		return mScrollPane;
	}

	int List::getVerticalPixelPadHeight()
	{
		return mVPixelPadHeight;
	}

	void List::onChildAdded(const EventArgs& args)
	{
		if(mScrollPane != NULL)
			mScrollPane->onChildAddedToParent(args);
	}

	void List::onChildRemoved(const EventArgs& args)
	{
		if(mScrollPane != NULL)
			mScrollPane->onChildRemovedFromParent(args);
	}

	void List::onSizeChanged(const EventArgs& args)
	{
		Image::onSizeChanged(args);

		if(mScrollPane != NULL)
			mScrollPane->onParentSizeChanged(args);
	}

	void List::removeItem(unsigned int index)
	{
		if( index >= mItems.size() )
			return;

		unsigned int counter = 0;
		std::vector<Widget*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			if( counter == index )
			{
				mGUIManager->destroyWidget((*it));
				mItems.erase(it);
				break;
			}

			++counter;
		}

		for( ; counter < mItems.size(); ++counter )
		{
			mItems[counter]->setYPosition(counter * mItemHeight);
		}
	}

	bool List::scrollingAllowed()
	{
		return mScrollingAllowed;
	}

	void List::setAutoSizeListItems(bool autoSize)
	{
		mAutoSizeListItems = autoSize;

		if(mAutoSizeListItems)
		{
			setItemPixelHeight(mTextUtilities->getNewlineHeight());
			mAutoSizeListItems = true;

			if(mScrollPane != NULL)
				mScrollPane->_determinePaneBounds();
		}
	}

	void List::setFont(const Ogre::String& fontScriptName, bool recursive)
	{
		Image::setFont(fontScriptName,recursive);
		mTextUtilities->setFont(fontScriptName);

		if(mAutoSizeListItems)
		{
			setItemPixelHeight(mTextUtilities->getNewlineHeight() + mVPixelPadHeight);
			mAutoSizeListItems = true;

			if(mScrollPane != NULL)
				mScrollPane->_determinePaneBounds();
		}
	}

	void List::setItemPixelHeight(const Ogre::Real& heightInPixels)
	{
		mItemHeight = heightInPixels;
		mAutoSizeListItems = false;

		Ogre::Real counter = 0;
		std::vector<Widget*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			(*it)->setYPosition(mItemHeight * counter);
			(*it)->setHeight(mItemHeight);
			++counter;
		}
	}

	void List::setVerticalPixelPadHeight(unsigned int height)
	{
		mVPixelPadHeight = height;

		if(mAutoSizeListItems)
		{
			setItemPixelHeight(mItemHeight + mVPixelPadHeight);
			mAutoSizeListItems = true;

			if(mScrollPane != NULL)
				mScrollPane->_determinePaneBounds();
		}
	}

	void List::show()
	{
		Image::show();

		if(mScrollPane != NULL)
			mScrollPane->_syncBarWithParentDimensions();
	}
}