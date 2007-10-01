#include "QuickGUIList.h"
#include "QuickGUIManager.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	List::List(const Ogre::String& name, Type type, const Rect& pixelDimensions, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Image(name,type,pixelDimensions,texture,container,ParentWidget,gm),
		mAutoNameListItemCount(0),
		mAutoSizeHeight(false),
		mNumberOfVisibleItems(5),
		mItemPixelHeight(20),
		mScrollPane(NULL)
	{
		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_LIST)
		{
			mQuad->setLayer(mParentWidget->getQuad()->getLayer());
		}

		if(mSize.height <= 0)
			mAutoSizeHeight = true;
		else
		{
			mNumberOfVisibleItems = ((mSize.width / mItemPixelHeight) + 1.0);
			setHeight(mNumberOfVisibleItems * mItemPixelHeight);
			mScrollPane = new ScrollPane(mInstanceName+".ScrollPane",TYPE_SCROLL_PANE,mQuadContainer,this,mGUIManager);
		}

		mHighlightTexture = mParentSheet->getDefaultSkin() + ".list.highlight" + mTextureExtension;

		// create highlight container for the list
		mHighlightPanel = new Quad(mInstanceName+".HighlightPanel",this);
		mHighlightPanel->setLayer(mQuad->getLayer());
		mHighlightPanel->setTexture(mHighlightTexture);
		// offset + 3, to be able to show over ListItems with Images and Buttons and Text
		mHighlightPanel->setOffset(mOffset+3);
		mHighlightPanel->_notifyQuadContainer(mQuadContainer);

		addEventHandler(EVENT_SIZE_CHANGED,&List::onWidthChanged,this);

		mItems.clear();
	}

	List::~List()
	{
		Widget::removeAndDestroyAllChildWidgets();
		mItems.clear();

		delete mHighlightPanel;
	}

	void List::_setClippingWidget(Widget* w)
	{
		mQuad->setClippingWidget(w);
		mHighlightPanel->setClippingWidget(w);
	}

	ListItem* List::addListItem(const Ogre::UTFString& text)
	{
		Ogre::String name = mInstanceName + ".ListItem" + Ogre::StringConverter::toString(mAutoNameListItemCount);
		++mAutoNameListItemCount;

		return addListItem(name,text);
	}

	ListItem* List::addListItem(const Ogre::String& name, const Ogre::UTFString& text)
	{
		// If using auto sizing of list, make sure to set new height before creating child, since Child Widgets will be relative to parent
		if(mAutoSizeHeight)
			setHeight((static_cast<int>(mItems.size()) + 1) * mItemPixelHeight);

		ListItem* newListItem = new ListItem(name,TYPE_LISTITEM,Rect(0,static_cast<int>(mItems.size())* mItemPixelHeight,mSize.width,mItemPixelHeight),mQuadContainer,this,mGUIManager);
		newListItem->getText()->setCaption(text);

		if(!mVisible)
			newListItem->hide();

		mItems.push_back(newListItem);

		return newListItem;
	}

	void List::clearList()
	{
		mItems.clear();
		Widget::removeAndDestroyAllChildWidgets();

		if(mAutoSizeHeight)
			setHeight(0);

		// if this is a drop down list that is part of a combobox, clear the combobox's "selection".
		if( (mParentWidget != NULL) && (mParentWidget->getWidgetType() == Widget::TYPE_COMBOBOX) )
			dynamic_cast<ComboBox*>(mParentWidget)->setCaption("");
	}

	bool List::getAutoSizeHeight()
	{
		return mAutoSizeHeight;
	}

	ListItem* List::getListItem(unsigned int index)
	{
		if( (static_cast<int>(mItems.size()) - 1) < static_cast<int>(index) ) return NULL;
		return mItems[index];
	}

	ListItem* List::getListItem(const Ogre::String& name)
	{
		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) return (*it);
		}

		return NULL;
	}

	void List::hide()
	{
		Image::hide();
		mHighlightPanel->setVisible(false);
	}

	void List::hideHighlight()
	{
		mHighlightPanel->setVisible(false);
	}

	void List::highlightListItem(ListItem* i)
	{
		mHighlightPanel->setPosition(i->getScreenPosition() + i->getScrollOffset());
		mHighlightPanel->setSize(i->getSize());
		mHighlightPanel->setVisible(true);
	}

	void List::onWidthChanged(const EventArgs& args)
	{
		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
			(*it)->setWidth(mSize.width);
	}

	void List::removeListItem(unsigned int index)
	{
		if( (mItems.empty()) || ((mItems.size() - 1) < index) ) 
			return;

		// Delete the List Item
		int counter = 0;
		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			if( counter == index )
			{
				ListItem* li = (*it);
				it = mItems.erase(it);
				mGUIManager->destroyWidget(li);
				break;
			}

			++counter;
		}

		// See if list items need to be re-positioned
		Ogre::Real n = 0;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			(*it)->setYPosition(mItemPixelHeight * n);
			(*it)->setHeight(mItemPixelHeight);
			++n;
		}

		if(mAutoSizeHeight)
			setHeight(static_cast<int>(mItems.size()) * mItemPixelHeight);
	}

	void List::setAutoSizeHeight()
	{
		if(mScrollPane != NULL)
		{
			mScrollPane->setSize(mSize);
			mScrollPane->disable();
		}

		mAutoSizeHeight = true;
		setHeight(static_cast<int>(mItems.size()) * mItemPixelHeight);
	}

	void List::setHighlightTexture(const Ogre::String& texture)
	{
		mHighlightTexture = texture;
		mHighlightPanel->setTexture(mHighlightTexture);
	}

	void List::setListItemPixelHeight(const Ogre::Real& heightInPixels)
	{
		mItemPixelHeight = heightInPixels;

		Ogre::Real counter = 0;
		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			(*it)->setYPosition(heightInPixels * counter);
			(*it)->setHeight(mItemPixelHeight);
			++counter;
		}

		mScrollPane->enable();
	}

	void List::setNumberOfVisibleItems(unsigned int number)
	{
		mAutoSizeHeight = false;

		mNumberOfVisibleItems = number;

		setHeight(mItemPixelHeight * mNumberOfVisibleItems);

		if(mScrollPane == NULL)
		{
			mScrollPane = new ScrollPane(mInstanceName+".ScrollPane",TYPE_SCROLL_PANE,mQuadContainer,this,mGUIManager);

			// Add any already existing children to ScrollPane's list of managed widgets.
			std::vector<Widget*>::iterator it;
			for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
			{
				mScrollPane->manageWidget((*it));
			}
		}
		else
		{
			mScrollPane->enable();
			mScrollPane->_determinePaneBounds();
		}
	}

	void List::setSize(const Ogre::Real& pixelWidth, const Ogre::Real& pixelHeight)
	{
		Image::setSize(pixelWidth,pixelHeight);
	}

	void List::setSize(const Size& pixelSize)
	{
		Image::setSize(pixelSize);
	}

	void List::show()
	{
		mHighlightPanel->setVisible(false);

		Image::show();
	}
}