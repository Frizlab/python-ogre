#include "QuickGUIList.h"
#include "QuickGUIManager.h"
#include "QuickGUIMouseCursor.h"

namespace QuickGUI
{
	List::List(const Ogre::String& name, Type type, const Rect& dimensions, GuiMetricsMode pMode, GuiMetricsMode sMode, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Image(name,type,dimensions,pMode,sMode,texture,container,ParentWidget,gm),
		mAutoNameListItemCount(0),
		mAutoSizeHeight(false),
		mNumberOfVisibleItems(5),
		mItemPixelHeight(20)
	{
		mInheritClippingRect = false;

		if(mRelativeDimensions.height <= 0.001)
			mAutoSizeHeight = true;
		else
		{
			mNumberOfVisibleItems = ((mPixelDimensions.width / mItemPixelHeight) + 1.0);
		}

		mHighlightTexture = mParentSheet->getDefaultSkin() + ".list.highlight" + mTextureExtension;

		// create highlight container for the list
		mHighlightPanel = new Quad(mInstanceName+".HighlightPanel",mGUIManager);
		mHighlightPanel->setLayer(Quad::LAYER_MENU);
		mHighlightPanel->setTexture(mHighlightTexture);
		mHighlightPanel->setOffset(mOffset+1);
		mHighlightPanel->_notifyQuadContainer(mQuadContainer);

		addEventHandler(EVENT_SIZE_CHANGED,&List::onWidthChanged,this);

		mItems.clear();

		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_LIST)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);

			mScrollPane = new ScrollPane(mInstanceName+".ScrollPane",TYPE_SCROLL_PANE,mQuadContainer,this,mGUIManager);
			mScrollPane->disable();
		}
	}

	List::~List()
	{
		Widget::removeAndDestroyAllChildWidgets();
		mItems.clear();

		delete mHighlightPanel;
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
			setHeight((static_cast<int>(mItems.size()) + 1) * mItemPixelHeight,QGUI_GMM_PIXELS);

		ListItem* newListItem = new ListItem(name,TYPE_LISTITEM,Rect(0,static_cast<int>(mItems.size())* mItemPixelHeight,mPixelDimensions.width,mItemPixelHeight),QGUI_GMM_PIXELS,QGUI_GMM_PIXELS,mQuadContainer,this,mGUIManager);
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
		mHighlightPanel->setVisible(false);

		Image::hide();
	}

	void List::hideHighlight()
	{
		mHighlightPanel->setVisible(false);
	}

	void List::highlightListItem(ListItem* i)
	{
		Rect liAbsDimensions = i->getDimensions(QGUI_GMM_ABSOLUTE,QGUI_GMM_ABSOLUTE);
		mHighlightPanel->setDimensions(liAbsDimensions);
		mHighlightPanel->setVisible(true);
	}

	void List::onWidthChanged(const EventArgs& args)
	{
		std::vector<ListItem*>::iterator it;
		for( it = mItems.begin(); it != mItems.end(); ++it )
			(*it)->setWidth(1.0);
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
			(*it)->setYPosition(mItemPixelHeight * n,QGUI_GMM_PIXELS);
			(*it)->setHeight(mItemPixelHeight,QGUI_GMM_PIXELS);
			++n;
		}

		if(mAutoSizeHeight)
			setHeight(static_cast<int>(mItems.size()) * mItemPixelHeight,QGUI_GMM_PIXELS);
	}

	void List::setAutoSizeHeight()
	{
		mScrollPane->setSize(1,1);
		mScrollPane->disable();
		mAutoSizeHeight = true;
		setHeight(static_cast<int>(mItems.size()) * mItemPixelHeight,QGUI_GMM_PIXELS);
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
			(*it)->setYPosition(heightInPixels * counter,QGUI_GMM_PIXELS);
			(*it)->setHeight(mItemPixelHeight,QGUI_GMM_PIXELS);
			++counter;
		}

		mScrollPane->enable();
	}

	void List::setNumberOfVisibleItems(unsigned int number)
	{
		mNumberOfVisibleItems = number;

		setHeight(mItemPixelHeight * mNumberOfVisibleItems);
	}

	void List::setSize(const Ogre::Real& width, const Ogre::Real& height, GuiMetricsMode mode)
	{
		Image::setSize(width,height,mode);
	}

	void List::setSize(const Size& s, GuiMetricsMode mode)
	{
		Image::setSize(s,mode);
	}

	void List::setWidth(const Ogre::Real& relativeWidth)
	{
		mRelativeDimensions.width = relativeWidth;
	}

	void List::show()
	{
		mHighlightPanel->setVisible(false);

		Image::show();
	}
}