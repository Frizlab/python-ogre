#include "QuickGUIList.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUIWidgetFactoryManager.h"

namespace QuickGUI
{
	const Ogre::String List::BACKGROUND = "background";

	void List::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("List");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("List",d);
	}

	ListDesc::ListDesc() :
		ContainerWidgetDesc()
	{
		listItemHeight = 25;
		supportMultiSelect = false;
	}

	void ListDesc::serialize(SerialBase* b)
	{
		ContainerWidgetDesc::serialize(b);

		b->IO("ListItemHeight",&listItemHeight);
		b->IO("MultiSelect",&supportMultiSelect);
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

		setListItemHeight(ld->listItemHeight);
		setMultiSelect(ld->supportMultiSelect);

		setSkinType(ld->skinTypeName);
	}

	Widget* List::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new List(widgetName);
		
		newWidget->_createDescObject("ListDesc");

		return newWidget;
	}

	void List::addListEventHandler(ListEvent EVENT, EventHandlerSlot* function)
	{
		mListEventHandlers[EVENT].push_back(function);
	}

	ListItem* List::addItem(ListItemDesc& d)
	{
		return addItem(-1,d);
	}

	ListItem* List::addItem(int index, ListItemDesc& d)
	{
		d.horizontalAnchor = ANCHOR_HORIZONTAL_LEFT_RIGHT;

		ListItem* newListItem = dynamic_cast<ListItem*>(mDesc->guiManager->createWidget(d.getWidgetClass(),d));

		newListItem->setHeight(mDesc->listItemHeight);
		newListItem->setWidth(mClientDimensions.size.width);

		addChild(newListItem);

		int counter = 0;
		bool added = false;
		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
		{
			if(counter == index)
			{
				mListItems.insert(it,newListItem);
				added = true;
			}
		}
		// If index was invalid, add to end of list.
		if(!added)
			mListItems.push_back(newListItem);

		// Set all positions of ListItems.
		float y = 0;
		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
		{
			(*it)->setPosition(Point(0,y));

			y += mDesc->listItemHeight;
		}

		return newListItem;
	}

	void List::clearSelection()
	{
		_clearSelection();

		WidgetEventArgs args(this);
		fireListEvent(LIST_EVENT_SELECTION_CHANGED,args);
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
		return mDesc->listItemHeight;
	}

	bool List::getMultiSelect()
	{
		return mDesc->supportMultiSelect;
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

		brush->setFilterMode(mDesc->brushFilterMode);

		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);

		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));
	}

	void List::selectItem(const MouseEventArgs& mea)
	{
		ListItem* li = dynamic_cast<ListItem*>(mea.widget);

		// Single Selection
		if(!mDesc->supportMultiSelect)
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
		mDesc->supportMultiSelect = MultiSelect;
	}

	void List::setListItemHeight(float height)
	{
		mDesc->listItemHeight = height;

		float y = 0;
		for(std::list<ListItem*>::iterator it = mListItems.begin(); it != mListItems.end(); ++it)
		{
			(*it)->setPosition(Point(0,y));
			(*it)->setHeight(mDesc->listItemHeight);

			y += mDesc->listItemHeight;
		}
	}
}
