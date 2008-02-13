#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUIList.h"
#include "QuickGUIManager.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	List::List(const std::string& name, GUIManager* gm) :
		Widget(name,gm),
		mScrollPane(0),
		mScrollingAllowed(false),
		mItemHeight(20),
		mAutoSizeListItems(true),
		mVPixelPadHeight(6)
	{
		mWidgetType = TYPE_LIST;
		mSkinComponent = ".list";
		mSize = Size(100,100);

		mTextHelper = new TextHelper();

		addEventHandler(EVENT_CHILD_ADDED,&List::onChildAdded,this);
		addEventHandler(EVENT_CHILD_REMOVED,&List::onChildRemoved,this);

		mItems.clear();
	}

	List::~List()
	{
		mScrollPane = NULL;
		delete mTextHelper;

		mItems.clear();
	}

	int List::_getNextInstanceCounter()
	{
		int counter = 0;
		while(mItems.find(counter) != mItems.end())
			++counter;

		return counter;
	}

	MenuLabel* List::addMenuLabel()
	{
		int counter = _getNextInstanceCounter();
		// Add items to the end of the list
		Point p(0,(static_cast<int>(mItems.size()) * mItemHeight));
		Size s(mSize.width,mItemHeight);

		std::string name = mInstanceName + ".Item" + Ogre::StringConverter::toString(counter);
		mGUIManager->notifyNameUsed(name);

		MenuLabel* newMenuLabel = dynamic_cast<MenuLabel*>(_createChild(mInstanceName+".ChildMenuLabel" + Ogre::StringConverter::toString(mItems.size()),TYPE_MENULABEL));
		newMenuLabel->setSize(s);
		newMenuLabel->setPosition(p);
		newMenuLabel->setAutoSize(false);
		newMenuLabel->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		newMenuLabel->setUseTransparencyPicking(false);

		mItems[counter] = newMenuLabel;

		return newMenuLabel;
	}

	TextBox* List::addTextBox()
	{
		int counter = _getNextInstanceCounter();
		// Add items to the end of the list
		Point p(0,(static_cast<int>(mItems.size()) * mItemHeight));
		Size s(mSize.width,mItemHeight);

		std::string name = mInstanceName+".Item"+Ogre::StringConverter::toString(counter);
		mGUIManager->notifyNameUsed(name);

		TextBox* newTextBox = dynamic_cast<TextBox*>(_createChild(mInstanceName+".ChildTextBox" + Ogre::StringConverter::toString(mItems.size()),TYPE_TEXTBOX));
		newTextBox->setSize(s);
		newTextBox->setUseBorders(false);
		newTextBox->hideSkin();
		newTextBox->setPosition(p);
		newTextBox->setAutoSize(false);
		newTextBox->setHorizontalAnchor(ANCHOR_HORIZONTAL_LEFT_RIGHT);
		newTextBox->setUseTransparencyPicking(false);

		mItems[counter] = newTextBox;

		return newTextBox;
	}

	void List::allowScrolling(bool allow)
	{
		mScrollingAllowed = allow;

		if(mScrollingAllowed)
		{
			if(mScrollPane == NULL)
			{
				mScrollPane = dynamic_cast<ScrollPane*>(_createComponent(mInstanceName+".ScrollPane",TYPE_SCROLL_PANE));
				mScrollPane->setSize(mSize);

				mScrollPane->removeChild(mScrollPane->mRightBar);
				// store reference to the scroll bar
				mRightScrollBar = mScrollPane->mRightBar;
				addChild(mRightScrollBar);
				mRightScrollBar->setPosition(mSize.width - 20,0);
				
				mScrollPane->removeChild(mScrollPane->mBottomBar);
				// store reference to the scroll bar
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

				removeAndDestroyChild(mRightScrollBar);
				mRightScrollBar = NULL;

				removeAndDestroyChild(mBottomScrollBar);
				mBottomScrollBar = NULL;
			}
		}
	}

	void List::clear()
	{
		std::map<int,Widget*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
			mGUIManager->destroyWidget((*it).second);
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
		std::string name = w->getInstanceName();

		int counter = 0;
		std::map<int,Widget*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			if( name == (*it).second->getInstanceName() )
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
		Widget::onSizeChanged(args);

		if(mScrollPane != NULL)
			mScrollPane->onParentSizeChanged(args);
	}

	void List::removeItem(unsigned int index)
	{
		if( index >= mItems.size() )
			return;

		Widget* w = NULL;

		unsigned int counter = 0;
		std::map<int,Widget*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			if( counter == index )
			{
				w = (*it).second;
				mItems.erase(it);
				break;
			}

			++counter;
		}

		if(w != NULL)
		{
			for( ; counter < mItems.size(); ++counter )
			{
				(*it).second->setYPosition(counter * mItemHeight);
			}

			// Remove child so that ScrollPane, if exists, will be notified.
			// Make sure this is done after all remaining items are correctly positioned,
			// since ScrollPane will adjust dimensions and scrollbars according to managed widgets.
			removeChild(w);
			mGUIManager->destroyWidget(w);
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
			setItemPixelHeight(mTextHelper->getGlyphHeight());
			mAutoSizeListItems = true;

			if(mScrollPane != NULL)
				mScrollPane->_determinePaneBounds();
		}
	}

	void List::setFont(const std::string& fontScriptName, bool recursive)
	{
		if(fontScriptName == "")
			return;

		Widget::setFont(fontScriptName,recursive);
		mTextHelper->setFont(fontScriptName);

		if(mAutoSizeListItems)
		{
			setItemPixelHeight(mTextHelper->getGlyphHeight() + mVPixelPadHeight);
			mAutoSizeListItems = true;

			if(mScrollPane != NULL)
				mScrollPane->_determinePaneBounds();
		}
	}

	void List::setItemPixelHeight(const float& heightInPixels)
	{
		mItemHeight = heightInPixels;
		mAutoSizeListItems = false;

		float counter = 0;
		std::map<int,Widget*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			(*it).second->setYPosition(mItemHeight * counter);
			(*it).second->setHeight(mItemHeight);
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
		Widget::show();

		if(mScrollPane != NULL)
			mScrollPane->_syncBarWithParentDimensions();
	}
}
