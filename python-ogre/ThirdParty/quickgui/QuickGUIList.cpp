#include "QuickGUIList.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"

#include "OgreStringConverter.h"

namespace QuickGUI
{
	const Ogre::String List::BACKGROUND = "background";

	void List::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("List");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("List",d);
	}

	ListDesc::ListDesc(const Ogre::String& id) :
		ContainerWidgetDesc(id)
	{
		resetToDefault();
	}

	void ListDesc::resetToDefault()
	{
		ContainerWidgetDesc::resetToDefault();

		list_listItemHeight = 25;
		list_supportMultiSelect = false;
	}

	void ListDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		b->IO("ListItemHeight",&list_listItemHeight);
		b->IO("MultiSelect",&list_supportMultiSelect);
	}

	List::List(const Ogre::String& name) :
		ContainerWidget(name),
		mPrevSelectedIndex(0)
	{
		mSkinElementName = BACKGROUND;

		addWidgetEventHandler(WIDGET_EVENT_CLIENTSIZE_CHANGED,&List::onClientSizeChanged,this);
	}

	List::~List()
	{
		// Clean up all user defined event handlers.
		for(int index = 0; index < LIST_EVENT_COUNT; ++index)
		{
			for(std::vector<EventHandlerSlot*>::iterator it = mListEventHandlers[index].begin(); it != mListEventHandlers[index].end(); ++it)
				OGRE_DELETE_T((*it),EventHandlerSlot,Ogre::MEMCATEGORY_GENERAL);
		}
	}

	void List::_clearSelection()
	{
		for(std::list<ListItem*>::iterator it = mSelectedItems.begin(); it != mSelectedItems.end(); ++it)
		{
			(*it)->setSelected(false);
		}
		mSelectedItems.clear();
	}

	unsigned int List::_getItemIndex(ListItem* li)
	{
		int count = 0;
		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
		{
			if(li == (*it))
				return count;

			++count;
		}

		return 0;
	}

	void List::_initialize(WidgetDesc* d)
	{
		ListDesc* ld = dynamic_cast<ListDesc*>(d);

		ContainerWidget::_initialize(d);

		mDesc = dynamic_cast<ListDesc*>(mWidgetDesc);

		setListItemHeight(ld->list_listItemHeight);
		setMultiSelect(ld->list_supportMultiSelect);

		setSkinType(ld->widget_skinTypeName);
	}

	void List::addChild(Widget* w)
	{
		ContainerWidget::addChild(w);

		ListItem* li = dynamic_cast<ListItem*>(w);
		// This version of addChild is only called in serialization.  Since all ListItems are named in alphabetical order,
		// we can just push each item onto the list.
		mListItems.push_back(li);
	}

	void List::addListEventHandler(ListEvent EVENT, EventHandlerSlot* function)
	{
		mListEventHandlers[EVENT].push_back(function);
	}

	void List::clearItems()
	{
		mSelectedItems.clear();

		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
			OGRE_DELETE_T((*it),ListItem,Ogre::MEMCATEGORY_GENERAL);
		mListItems.clear();

		mChildren.clear();

		redraw();
	}

	void List::clearSelection()
	{
		_clearSelection();

		WidgetEventArgs args(this);
		fireListEvent(LIST_EVENT_SELECTION_CHANGED,args);
	}

	ListItem* List::createItem(ListItemDesc* d)
	{
		return createItem(-1,d);
	}

	ListItem* List::createItem(int index, ListItemDesc* d)
	{
		d->widget_horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;

		ListItem* newListItem = dynamic_cast<ListItem*>(Widget::create(d->getWidgetClass(),d));

		newListItem->setHeight(mDesc->list_listItemHeight);
		newListItem->setWidth(mClientDimensions.size.width);

		ContainerWidget::addChild(newListItem);

		int counter = 0;
		bool added = false;
		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
		{
			if(counter == index)
			{
				mListItems.insert(it,newListItem);
				added = true;
			}

			++counter;
		}
		// If index was invalid, add to end of list.
		if(!added)
			mListItems.push_back(newListItem);

		// Set all positions of ListItems.
		float y = 0;
		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
		{
			(*it)->setPosition(Point(0,y));

			y += mDesc->list_listItemHeight;
		}

		updateItemNamesAndIndices();

		return newListItem;
	}

	void List::deselectItem(unsigned int index)
	{
		if(index >= mListItems.size())
			return;

		ListItem* deselected = NULL;

		unsigned int count = 0;
		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
		{
			if(count == index)
			{
				deselected = (*it);
				(*it)->setSelected(false);
				break;
			}

			++count;
		}

		if(deselected == NULL)
			return;

		for(std::list<ListItem*>::iterator it = mSelectedItems.begin(); it != mSelectedItems.end(); ++it)
		{
			if((*it) == deselected)
			{
				mSelectedItems.erase(it);
				break;
			}
		}

		WidgetEventArgs args(this);
		fireListEvent(LIST_EVENT_SELECTION_CHANGED,args);
	}

	void List::destroyItem(unsigned int index)
	{
		int count = 0;
		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
		{
			if(count == index)
			{
				OGRE_DELETE_T((*it),ListItem,Ogre::MEMCATEGORY_GENERAL);
				mListItems.erase(it);
			}
		}

		// Update names and Indices
		updateItemNamesAndIndices();
	}

	bool List::fireListEvent(ListEvent e, EventArgs& args)
	{
		if(mListEventHandlers[e].empty())
			return false;

		// Execute registered handlers
		std::vector<EventHandlerSlot*>* userEventHandlers = &(mListEventHandlers[e]);
		for(std::vector<EventHandlerSlot*>::iterator it = userEventHandlers->begin(); it != userEventHandlers->end(); ++it )
			(*it)->execute(args);

		return true;
	}

	Ogre::String List::getClass()
	{
		return "List";
	}

	ListItem* List::getItem(unsigned int index)
	{
		if(index >= mListItems.size())
			return NULL;

		unsigned int count = 0;
		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
		{
			if(count == index)
				return (*it);

			++count;
		}

		return NULL;
	}

	float List::getListItemHeight()
	{
		return mDesc->list_listItemHeight;
	}

	bool List::getMultiSelect()
	{
		return mDesc->list_supportMultiSelect;
	}

	int List::getNumberOfItems()
	{
		return static_cast<int>(mListItems.size());
	}

	std::list<ListItem*> List::getSelection()
	{
		return mSelectedItems;
	}

	void List::onClientSizeChanged(const EventArgs& args)
	{
		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
		{
			(*it)->setWidth(mClientDimensions.size.width);
		}
	}

	void List::onDraw()
	{
		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mDesc->widget_brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));
	}

	void List::selectItem(const MouseEventArgs& mea)
	{
		ListItem* li = dynamic_cast<ListItem*>(mea.widget);

		// Single Selection
		if(!mDesc->list_supportMultiSelect)
		{
			bool selected = li->getSelected();

			_clearSelection();

			li->setSelected(!selected);

			if(li->getSelected())
				mSelectedItems.push_back(li);
			else
				li->mSkinElementName = ListItem::OVER;
		}
		// Multi Selection
		else
		{
			bool ctrlDown = (mea.keyModifiers & CTRL) > 0;
			bool shiftDown = (mea.keyModifiers & SHIFT) > 0;

			if(ctrlDown)
			{
				if(shiftDown)
				{
					// Select everything in the range that is unselected
					unsigned int currentIndex = _getItemIndex(li);
					if(currentIndex < mPrevSelectedIndex)
					{
						unsigned int count = 0;
						for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
						{
							if(count > mPrevSelectedIndex)
								break;

							if(count >= currentIndex)
							{
								mSelectedItems.push_back(*it);
								(*it)->setSelected(true);
							}

							++count;
						}
					}
					else
					{
						unsigned int count = 0;
						for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
						{
							if(count > currentIndex)
								break;

							if(count >= mPrevSelectedIndex)
							{
								mSelectedItems.push_back(*it);
								(*it)->setSelected(true);
							}

							++count;
						}
					}
				}
				else
				{
					// toggle selection
					li->setSelected(!li->getSelected());

					// if selected, add to list
					if(li->getSelected())
						mSelectedItems.push_back(li);
					// otherwise remove from selected list
					else
					{
						for(std::list<ListItem*>::iterator it = mSelectedItems.begin(); it != mSelectedItems.end(); ++it)
						{
							if(li == (*it))
							{
								mSelectedItems.erase(it);
								break;
							}
						}

						li->mSkinElementName = ListItem::OVER;
					}
				}
			}
			else
			{
				if(shiftDown)
				{
					_clearSelection();

					// Select everything in the range that is unselected
					unsigned int currentIndex = _getItemIndex(li);
					if(currentIndex < mPrevSelectedIndex)
					{
						unsigned int count = 0;
						for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
						{
							if(count > mPrevSelectedIndex)
								break;

							if(count >= currentIndex)
							{
								mSelectedItems.push_back(*it);
								(*it)->setSelected(true);
							}

							++count;
						}
					}
					else
					{
						unsigned int count = 0;
						for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
						{
							if(count > currentIndex)
								break;

							if(count >= mPrevSelectedIndex)
							{
								mSelectedItems.push_back(*it);
								(*it)->setSelected(true);
							}

							++count;
						}
					}
				}
				else
				{
					bool selected = li->getSelected();

					_clearSelection();

					// toggle selection
					li->setSelected(!selected);

					// if selected, add to list
					if(li->getSelected())
						mSelectedItems.push_back(li);
					else
						li->mSkinElementName = ListItem::OVER;
				}
			}
		}

		mPrevSelectedIndex = _getItemIndex(li);

		redraw();

		WidgetEventArgs args(this);
		fireListEvent(LIST_EVENT_SELECTION_CHANGED,args);
	}

	void List::selectItem(unsigned int index)
	{
		_clearSelection();

		if(index >= mListItems.size())
			return;

		unsigned int count = 0;
		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
		{
			if(count == index)
			{
				(*it)->setSelected(true);

				WidgetEventArgs args(this);
				fireListEvent(LIST_EVENT_SELECTION_CHANGED,args);

				return;
			}

			++count;
		}
	}

	void List::setMultiSelect(bool MultiSelect)
	{
		mDesc->list_supportMultiSelect = MultiSelect;
	}

	void List::setListItemHeight(float height)
	{
		mDesc->list_listItemHeight = height;

		float y = 0;
		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
		{
			(*it)->setPosition(Point(0,y));
			(*it)->setHeight(mDesc->list_listItemHeight);

			y += mDesc->list_listItemHeight;
		}
	}

	void List::updateItemNamesAndIndices()
	{
		Ogre::String ListName = getName();
		unsigned int counter = 0;

		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
		{
			(*it)->setIndex(counter);
			(*it)->mWidgetDesc->widget_name = ListName + Ogre::StringConverter::toString(counter);

			++counter;
		}
	}
}
