#include "SimpleGUIList.h"
#include "SimpleGUIManager.h"

namespace SimpleGUI
{
	List::List(const Ogre::String& name, const Ogre::Vector3& dimensions, const Ogre::String& material, const Ogre::String& highlightMaterial, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,Ogre::Vector4(dimensions.x,dimensions.y,dimensions.z,0),material,show,overlayContainer,ParentWidget),
		mAutoNameListItemCount(0),
		mAutoNameListCount(0),
		mHighlightMaterial(highlightMaterial),
		mListItemHorizontalAlignment(SimpleGUI::SGUI_HA_MID),
		mListItemVerticalAlignment(SimpleGUI::SGUI_VA_MID)
	{
		mWidgetType = Widget::SGUI_LIST;
		mDefaultListItemHeight = mParentWidget->getAbsoluteDimensions().w;

		mHighlightPanel = createPanelOverlayElement(mInstanceName+"_highlightPanel",mPixelDimensions,mHighlightMaterial);
		mOverlayContainer->addChild(mHighlightPanel);
		mHighlightPanel->show();

		mItems.clear();
		mChildLists.clear();
	}

	List::~List()
	{
		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		mOverlayContainer->removeChild(mHighlightPanel->getName());
		om->destroyOverlayElement(mHighlightPanel);

		std::vector<ListItem*>::iterator it1;
		for( it1 = mItems.begin(); it1 != mItems.end(); ++it1 )
			delete (*it1);
		mItems.clear();

		std::vector<List*>::iterator it2;
		for( it2 = mChildLists.begin(); it2 != mChildLists.end(); ++it2 )
			delete (*it2);
		mChildLists.clear();
	}

	void List::_notifyDimensionsChanged()
	{
		Label::_notifyDimensionsChanged();

		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
			(*it)->_notifyDimensionsChanged();
	}

	ListItem* List::addListItem(const Ogre::String& text)
	{
		Ogre::Vector4 OriginalAbsoluteDimensions = mAbsoluteDimensions;

		// Calculate the current absolute height of the list
		Ogre::Real currentListHeight = 0.0;
		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			currentListHeight = currentListHeight + (*it)->getAbsoluteDimensions().w;
		}

		// Grow the list to accept the new incoming list item
		Ogre::Real calculatedListHeight = currentListHeight + mDefaultListItemHeight;
		// funny, this should almost always be more than 1.0, which defies relative coordinates
		// IMPORTANT! Do not call _notifyDimensionsChanged, otherwise all child ListItems will have their dimensions auto adjusted.
		// We need to keep them as is, and later use the unchanged values to calculate new values, as the list height has changed.
		// Essentially, we want to maintain the ListItems' height while adjusting the List's height.
		mRelativeDimensions.w = calculatedListHeight / (mParentWidget->getAbsoluteDimensions().w);
		_updateDimensions(mRelativeDimensions);
		
		// Shrink the previous list Items back to the correct size
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			Ogre::Vector2 pos = (*it)->getAbsolutePosition();
			Ogre::Vector2 size = (*it)->getAbsoluteSize();

			(*it)->setDimensions(
				Ogre::Vector4(
					(pos.x - mAbsoluteDimensions.x) / mAbsoluteDimensions.z,
					(pos.y - mAbsoluteDimensions.y) / mAbsoluteDimensions.w,
					size.x / mAbsoluteDimensions.z,
					size.y / mAbsoluteDimensions.w
					)
				);
		}

		// Add the List Item
		Ogre::String name = mInstanceName + "_ListItem" + Ogre::StringConverter::toString(mAutoNameListItemCount);
		++mAutoNameListItemCount;
		ListItem* newListItem = new ListItem(name,Ogre::Vector4(0,currentListHeight / calculatedListHeight,1.0,mDefaultListItemHeight / mAbsoluteDimensions.w),mVisible,mChildrenContainer,this);
		newListItem->setCharacterHeight(mCharacterHeight);
		newListItem->setFont(mFont);
		newListItem->setTextColor(mTextTopColor,mTextBotColor);
		newListItem->setText(text);
		newListItem->alignText(mListItemHorizontalAlignment,mListItemVerticalAlignment);
		mItems.push_back(newListItem);

		// Important!  Even though we have set the dimensions, we need to update the overlay elements (of the list, specifically)
		_notifyDimensionsChanged();
		
		return newListItem;
	}

	void List::alignListItemText(HorizontalAlignment ha, VerticalAlignment va)
	{
		mListItemHorizontalAlignment = ha;
		mListItemVerticalAlignment = va;

		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			(*it)->alignText(mListItemHorizontalAlignment,mListItemVerticalAlignment);
		}
	}

	void List::deactivate(EventArgs& e)
	{
		if(mParentWidget) mParentWidget->deactivate(e);

		Label::deactivate(e);
	}

	ListItem* List::getListItem(unsigned int index)
	{
		if( (static_cast<int>(mItems.size()) - 1) < static_cast<int>(index) ) return NULL;
		return mItems[index];
	}

	int List::getNumberOfListItems()
	{
		return static_cast<int>(mChildLists.size());
	}

	Widget* List::getTargetWidget(const Ogre::Vector2& p)
	{
		if(!mVisible) return NULL;

		// Check child Lists
		std::vector<List*>::iterator it1;
		for( it1 = mChildLists.begin(); it1 != mChildLists.end(); ++it1 )
		{
			if( (*it1)->getTargetWidget(p) ) return (*it1)->getTargetWidget(p);
		}

		// Check List Items
		std::vector<ListItem*>::iterator it2;
		for( it2 = mItems.begin(); it2 != mItems.end(); ++it2 )
		{
			if( (*it2)->getTargetWidget(p) ) return (*it2)->getTargetWidget(p);
		}

		if(isPointWithinBounds(p)) return this;

		return NULL;
	}

	void List::hide()
	{
		mHighlightPanel->hide();

		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			(*it)->hide();
		}

		Label::hide();
	}

	void List::hideHighlight()
	{
		mHighlightPanel->hide();
	}

	void List::highlightListItem(ListItem* i)
	{
		Ogre::Vector4 liPixelDimensions = i->getPixelDimensions();
		mHighlightPanel->setPosition(liPixelDimensions.x,liPixelDimensions.y);
		mHighlightPanel->setDimensions(liPixelDimensions.z,liPixelDimensions.w);
		mHighlightPanel->show();
	}

	void List::removeList(unsigned int index)
	{
		if( (static_cast<int>(mChildLists.size()) - 1) < static_cast<int>(index) ) return;
		
		int counter = 0;
		std::vector<List*>::iterator it;
		for( it = mChildLists.begin(); it != mChildLists.end(); ++it )
		{
			if( counter == index )
			{
				List* l = *(mChildLists.erase(it));
				delete l;
				return;
			}
			++counter;
		}
	}

	void List::removeListItem(unsigned int index)
	{
		if( (static_cast<int>(mChildLists.size()) - 1) < static_cast<int>(index) ) return;

		// Delete the List Item
		ListItem* li;
		int counter = 0;
		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			if( counter == index )
			{
				li = (*it);
				it = mItems.erase(it);
			}

			++counter;
		}

		Ogre::Real FreeHeight = li->getAbsoluteDimensions().w;
		delete li;

		// IMPORTANT! Do not call _notifyDimensionsChanged, otherwise all child ListItems will have their dimensions auto adjusted.
		// We need to keep them as is, and later use the unchanged values to calculate new values, as the list height has changed.
		// Essentially, we want to maintain the ListItems' height while adjusting the List's height.
		mRelativeDimensions.w = (mAbsoluteDimensions.w - FreeHeight) / (mParentWidget->getAbsoluteDimensions().w);
		_updateDimensions(mRelativeDimensions);

		Ogre::Real newYPos = 0.0;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			Ogre::Vector2 pos = (*it)->getAbsolutePosition();
			pos.y = newYPos;
			Ogre::Vector2 size = (*it)->getAbsoluteSize();

			(*it)->setDimensions(
				Ogre::Vector4(
					(pos.x - mAbsoluteDimensions.x) / mAbsoluteDimensions.z,
					(pos.y) / mAbsoluteDimensions.w,
					size.x / mAbsoluteDimensions.z,
					size.y / mAbsoluteDimensions.w
					)
				);

			newYPos = newYPos + (*it)->getAbsoluteSize().y;
		}
	}

	void List::setCharacterHeight(const Ogre::Real& relativeHeight)
	{
		Label::setCharacterHeight(relativeHeight);
		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			(*it)->setCharacterHeight(relativeHeight);
		}
	}

	void List::show()
	{
		mHighlightPanel->hide();

		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			(*it)->show();
		}

		Label::show();
	}

	void List::timeElapsed(Ogre::Real time)
	{
		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
		{
			(*it)->timeElapsed(time);
		}

		Label::timeElapsed(time);
	}
}