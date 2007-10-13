#include "QuickGUIList.h"
#include "QuickGUIManager.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	List::List(const Ogre::String& instanceName, const Size& pixelSize, Ogre::String texture, GUIManager* gm) :
		Image(instanceName,pixelSize,texture,gm),
		mScrollPane(0),
		mAutoNameWidgetCounter(-1),
		mScrollingAllowed(false),
		mItemHeight(20)
	{
		mWidgetType = TYPE_LIST;

		addEventHandler(EVENT_CHILD_ADDED,&List::onChildAdded,this);
		addEventHandler(EVENT_CHILD_REMOVED,&List::onChildRemoved,this);

		mItems.clear();
	}

	List::~List()
	{
		mItems.clear();
	}

	MenuLabel* List::addMenuLabel()
	{
		++mAutoNameWidgetCounter;

		Point p(0,(mAutoNameWidgetCounter * mItemHeight));
		Size s(mSize.width,mItemHeight);

		MenuLabel* newMenuLabel = new MenuLabel(mInstanceName+".Item"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),s,"qgui.menulabel.png",mGUIManager);
		addChild(newMenuLabel);
		newMenuLabel->setPosition(p);

		if(!mVisible)
			newMenuLabel->hide();

		mItems.push_back(newMenuLabel);

		return newMenuLabel;
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

	void List::removeItem(Widget* w)
	{
		Ogre::String name = w->getInstanceName();

		unsigned int counter = 0;
		std::vector<Widget*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			if( name == (*it)->getInstanceName() )
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

	void List::setItemPixelHeight(const Ogre::Real& heightInPixels)
	{
		mItemHeight = heightInPixels;

		Ogre::Real counter = 0;
		std::vector<Widget*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			(*it)->setYPosition(mItemHeight * counter);
			(*it)->setHeight(mItemHeight);
			++counter;
		}
	}

	void List::show()
	{
		Image::show();

		if(mScrollPane != NULL)
			mScrollPane->_syncBarWithParentDimensions();
	}
}