#include "QuickGUIManager.h"
#include "QuickGUIPanel.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	Panel::Panel(const Ogre::String& name, Type type, const Rect& pixelDimensions, Ogre::String texture, QuadContainer* container, Widget* parentWidget, GUIManager* gm) :
		Image(name,type,pixelDimensions,texture,container,parentWidget,gm),
		QuadContainer(this),
		mAutoNameWidgetCounter(0),
		mNumButtons(0),
		mNumComboBoxes(0),
		mNumImages(0),
		mNumLabels(0),
		mNumLists(0),
		mNumMenus(0),
		mNumNStateButtons(0),
		mNumPanels(0),
		mNumProgressBars(0),
		mNumTextBoxes(0),
		mNumHorizontalTrackBars(0),
		mNumVerticalTrackBars(0),
		mNumHorizontalScrollBars(0),
		mNumVerticalScrollBars(0)
	{
		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_PANEL)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);
			// This will remove if from parent QuadContainer and add it to this one.
			mQuad->_notifyQuadContainer(this);
			mQuadContainer->addChildPanelContainer(this);

			mScrollPane = new ScrollPane(mInstanceName+".ScrollPane",TYPE_SCROLL_PANE,this,this,mGUIManager);
		}
	}

	Panel::~Panel()
	{
		// Must be done before removing from parent Container! 
		// (Cannot destroy parent container and then have children remove themself from it...)
		Widget::removeAndDestroyAllChildWidgets();

		if(mQuadContainer != NULL)
		{
			if(mWidgetType == Widget::TYPE_WINDOW)
				mQuadContainer->removeChildWindowContainer(QuadContainer::getID());
			else if(mWidgetType == Widget::TYPE_PANEL)
				mQuadContainer->removeChildPanelContainer(QuadContainer::getID());
		}
	}

	void Panel::_removeFromChildList(Widget* w)
	{
		if(!Widget::isChild(w))
			return;

		switch( w->getWidgetType() )
		{
		case Widget::TYPE_BUTTON:
			--mNumButtons; 
			break;
		case Widget::TYPE_COMBOBOX:
			--mNumComboBoxes;
			break;
		case Widget::TYPE_IMAGE:
			--mNumImages;
			break;
		case Widget::TYPE_LABEL:
			--mNumLabels;
			break;
		case Widget::TYPE_MENU:
			--mNumMenus;
			break;
		case Widget::TYPE_NSTATEBUTTON:
			--mNumNStateButtons;
			break;
		case Widget::TYPE_PANEL:
			--mNumPanels;
			break;
		case Widget::TYPE_PROGRESSBAR:
			--mNumProgressBars;
			break;
		case Widget::TYPE_TEXTBOX:
			--mNumTextBoxes;
			break;
		case Widget::TYPE_TRACKBAR_HORIZONTAL:
			--mNumHorizontalTrackBars;
			break;
		case Widget::TYPE_TRACKBAR_VERTICAL:
			--mNumVerticalTrackBars;
			break;
		}

		Widget::_removeFromChildList(w);
	}

	Button* Panel::_createButton(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// update count
		++mNumButtons;

		Button* newButton = new Button(name,TYPE_BUTTON,pixelDimensions,texture,this,this,mGUIManager);
		if(!mVisible) 
			newButton->hide();
		
		return newButton;
	}

	ComboBox* Panel::_createComboBox(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// update count
		++mNumComboBoxes;

		ComboBox* newComboBox = new ComboBox(name,TYPE_COMBOBOX,pixelDimensions,texture,this,this,mGUIManager);
		if(!mVisible) 
			newComboBox->hide();
		
		return newComboBox;
	}

	HorizontalScrollBar* Panel::_createHorizontalScrollBar(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// update count;
		++mNumHorizontalScrollBars;

		HorizontalScrollBar* newScrollBar = new HorizontalScrollBar(name,TYPE_SCROLLBAR_HORIZONTAL,pixelDimensions,texture,this,this,mGUIManager);
		if(!mVisible)
			newScrollBar->hide();

		return newScrollBar;
	}

	Image* Panel::_createImage(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// update count
		++mNumImages;

		Image* newImage = new Image(name,TYPE_IMAGE,pixelDimensions,texture,this,this,mGUIManager);
		if(!mVisible) 
			newImage->hide();

		return newImage;
	}

	Label* Panel::_createLabel(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// update count
		++mNumLabels;

		Label* newLabel = new Label(name,TYPE_LABEL,pixelDimensions,texture,this,this,mGUIManager);
		if(!mVisible) 
			newLabel->hide();

		return newLabel;
	}

	List* Panel::_createList(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// update count
		++mNumLists;

		List* newList = new List(name,TYPE_LIST,pixelDimensions,texture,this,this,mGUIManager);
		if(!mVisible) 
			newList->hide();

		return newList;
	}

	Menu* Panel::_createMenu(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// update count
		++mNumMenus;

		Menu* newMenu = new Menu(name,TYPE_MENU,pixelDimensions,texture,this,this,mGUIManager);
		if(!mVisible) 
			newMenu->hide();

		return newMenu;
	}

	NStateButton* Panel::_createNStateButton(const Ogre::String& name, const Rect& pixelDimensions)
	{
		// update count
		++mNumNStateButtons;

		NStateButton* newNStateButton = new NStateButton(name,TYPE_NSTATEBUTTON,pixelDimensions,this,this,mGUIManager);
		if(!mVisible) 
			newNStateButton->hide();

		return newNStateButton;
	}

	Panel* Panel::_createPanel(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// update count
		++mNumPanels;

		Panel* newPanel = new Panel(name,TYPE_PANEL,pixelDimensions,texture,this,this,mGUIManager);
		if(!mVisible) 
			newPanel->hide();
	
		return newPanel;
	}

	ProgressBar* Panel::_createProgressBar(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// update count
		++mNumProgressBars;

		ProgressBar* newProgressBar = new ProgressBar(name,TYPE_PROGRESSBAR,pixelDimensions,texture,this,this,mGUIManager);
		if(!mVisible) 
			newProgressBar->hide();

		return newProgressBar;
	}

	TextBox* Panel::_createTextBox(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// update count
		++mNumTextBoxes;

		TextBox* newTextBox = new TextBox(name,TYPE_TEXTBOX,pixelDimensions,texture,this,this,mGUIManager);
		if(!mVisible) 
			newTextBox->hide();

		return newTextBox;
	}

	HorizontalTrackBar* Panel::_createHorizontalTrackBar(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// update count;
		++mNumHorizontalTrackBars;

		HorizontalTrackBar* newTrackBar = new HorizontalTrackBar(name,TYPE_TRACKBAR_HORIZONTAL,pixelDimensions,texture,this,this,mGUIManager);
		if(!mVisible) 
			newTrackBar->hide();

		return newTrackBar;
	}

	VerticalScrollBar* Panel::_createVerticalScrollBar(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// update count;
		++mNumVerticalScrollBars;

		VerticalScrollBar* newScrollBar = new VerticalScrollBar(name,TYPE_SCROLLBAR_VERTICAL,pixelDimensions,texture,this,this,mGUIManager);
		if(!mVisible)
			newScrollBar->hide();

		return newScrollBar;
	}

	VerticalTrackBar* Panel::_createVerticalTrackBar(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		// update count;
		++mNumVerticalTrackBars;

		VerticalTrackBar* newTrackBar = new VerticalTrackBar(name,TYPE_TRACKBAR_VERTICAL,pixelDimensions,texture,this,this,mGUIManager);
		if(!mVisible) 
			newTrackBar->hide();

		return newTrackBar;
	}

	Widget* Panel::_getWidget(Widget::Type TYPE, unsigned int index)
	{
		int counter = -1;
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getWidgetType() == TYPE )
			{
				++counter;
				if( counter == index ) return (*it);
			}
		}
		return NULL;
	}

	Widget* Panel::_getWidget(const Ogre::String& name)
	{
		std::vector<Widget*>::iterator it;
		for( it = mChildWidgets.begin(); it != mChildWidgets.end(); ++it )
		{
			if( (*it)->getInstanceName() == name ) return (*it);
		}
		return NULL;
	}

	Button* Panel::createButton(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createButton(name,pixelDimensions,texture);
	}

	Button* Panel::createButton(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createButton(name,pixelDimensions,texture);
	}

	Button* Panel::createButton(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		Ogre::String texture = getDefaultSkin() + ".button.png";

		return _createButton(name,pixelDimensions,texture);
	}

	Button* Panel::createButton(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String texture = getDefaultSkin() + ".button.png";

		return _createButton(name,pixelDimensions,texture);
	}

	ComboBox* Panel::createComboBox(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createComboBox(name,pixelDimensions,texture);
	}

	ComboBox* Panel::createComboBox(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createComboBox(name,pixelDimensions,texture);
	}

	ComboBox* Panel::createComboBox(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;
		
		Ogre::String texture = getDefaultSkin() + ".combobox.png";

		return _createComboBox(name,pixelDimensions,texture);
	}

	ComboBox* Panel::createComboBox(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;
		
		Ogre::String texture = getDefaultSkin() + ".combobox.png";

		return _createComboBox(name,pixelDimensions,texture);
	}

	HorizontalScrollBar* Panel::createHorizontalScrollBar(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createHorizontalScrollBar(name,pixelDimensions,texture);
	}

	HorizontalScrollBar* Panel::createHorizontalScrollBar(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createHorizontalScrollBar(name,pixelDimensions,texture);
	}

	HorizontalScrollBar* Panel::createHorizontalScrollBar(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		Ogre::String texture = getDefaultSkin() + ".scrollbar.horizontal.png";

		return _createHorizontalScrollBar(name,pixelDimensions,texture);
	}

	HorizontalScrollBar* Panel::createHorizontalScrollBar(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String texture = getDefaultSkin() + ".scrollbar.horizontal.png";

		return _createHorizontalScrollBar(name,pixelDimensions,texture);
	}

	Image* Panel::createImage(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createImage(name,pixelDimensions,texture);
	}

	Image* Panel::createImage(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createImage(name,pixelDimensions,texture);
	}

	Image* Panel::createImage(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		Ogre::String texture = getDefaultSkin() + ".image.png";

		return _createImage(name,pixelDimensions,texture);
	}

	Image* Panel::createImage(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String texture = getDefaultSkin() + ".image.png";

		return _createImage(name,pixelDimensions,texture);
	}

	Label* Panel::createLabel(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createLabel(name,pixelDimensions,texture);
	}

	Label* Panel::createLabel(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createLabel(name,pixelDimensions,texture);
	}

	Label* Panel::createLabel(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		Ogre::String texture = getDefaultSkin() + ".label.png";

		return _createLabel(name,pixelDimensions,texture);
	}
	
	Label* Panel::createLabel(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String texture = getDefaultSkin() + ".label.png";

		return _createLabel(name,pixelDimensions,texture);
	}

	List* Panel::createList(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createList(name,pixelDimensions,texture);
	}

	List* Panel::createList(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createList(name,pixelDimensions,texture);
	}

	List* Panel::createList(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		Ogre::String texture = getDefaultSkin() + ".list.png";

		return _createList(name,pixelDimensions,texture);
	}

	List* Panel::createList(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String texture = getDefaultSkin() + ".list.png";

		return _createList(name,pixelDimensions,texture);
	}

	Menu* Panel::createMenu(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createMenu(name,pixelDimensions,texture);
	}

	Menu* Panel::createMenu(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createMenu(name,pixelDimensions,texture);
	}

	Menu* Panel::createMenu(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		Ogre::String texture = getDefaultSkin() + ".menu.png";

		return _createMenu(name,pixelDimensions,texture);
	}

	Menu* Panel::createMenu(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String texture = getDefaultSkin() + ".menu.png";

		return _createMenu(name,pixelDimensions,texture);
	}

	NStateButton* Panel::createNStateButton(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createNStateButton(name,pixelDimensions);
	}

	NStateButton* Panel::createNStateButton(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createNStateButton(name,pixelDimensions);
	}

	Panel* Panel::createPanel(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createPanel(name,pixelDimensions,texture);
	}

	Panel* Panel::createPanel(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createPanel(name,pixelDimensions,texture);
	}

	Panel* Panel::createPanel(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		Ogre::String texture = getDefaultSkin() + ".panel.png";

		return _createPanel(name,pixelDimensions,texture);
	}

	Panel* Panel::createPanel(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String texture = getDefaultSkin() + ".panel.png";

		return _createPanel(name,pixelDimensions,texture);
	}

	ProgressBar* Panel::createProgressBar(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createProgressBar(name,pixelDimensions,texture);
	}

	ProgressBar* Panel::createProgressBar(const Rect& pixelDimensions,  const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createProgressBar(name,pixelDimensions,texture);
	}

	ProgressBar* Panel::createProgressBar(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		Ogre::String texture = getDefaultSkin() + ".progressbar.png";

		return _createProgressBar(name,pixelDimensions,texture);
	}

	ProgressBar* Panel::createProgressBar(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String texture = getDefaultSkin() + ".progressbar.png";

		return _createProgressBar(name,pixelDimensions,texture);
	}

	TextBox* Panel::createTextBox(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createTextBox(name,pixelDimensions,texture);
	}

	TextBox* Panel::createTextBox(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createTextBox(name,pixelDimensions,texture);
	}

	TextBox* Panel::createTextBox(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		Ogre::String texture = getDefaultSkin() + ".textbox.png";

		return _createTextBox(name,pixelDimensions,texture);
	}

	TextBox* Panel::createTextBox(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String texture = getDefaultSkin() + ".textbox.png";

		return _createTextBox(name,pixelDimensions,texture);
	}

	HorizontalTrackBar* Panel::createHorizontalTrackBar(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createHorizontalTrackBar(name,pixelDimensions,texture);
	}

	HorizontalTrackBar* Panel::createHorizontalTrackBar(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createHorizontalTrackBar(name,pixelDimensions,texture);
	}

	HorizontalTrackBar* Panel::createHorizontalTrackBar(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		Ogre::String texture = getDefaultSkin() + ".trackbar.horizontal.png";

		return _createHorizontalTrackBar(name,pixelDimensions,texture);
	}

	HorizontalTrackBar* Panel::createHorizontalTrackBar(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String texture = getDefaultSkin() + ".trackbar.horizontal.png";

		return _createHorizontalTrackBar(name,pixelDimensions,texture);
	}

	VerticalScrollBar* Panel::createVerticalScrollBar(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createVerticalScrollBar(name,pixelDimensions,texture);
	}

	VerticalScrollBar* Panel::createVerticalScrollBar(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createVerticalScrollBar(name,pixelDimensions,texture);
	}

	VerticalScrollBar* Panel::createVerticalScrollBar(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		Ogre::String texture = getDefaultSkin() + ".scrollbar.vertical.png";

		return _createVerticalScrollBar(name,pixelDimensions,texture);
	}

	VerticalScrollBar* Panel::createVerticalScrollBar(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String texture = getDefaultSkin() + ".scrollbar.vertical.png";

		return _createVerticalScrollBar(name,pixelDimensions,texture);
	}

	VerticalTrackBar* Panel::createVerticalTrackBar(const Ogre::String& name, const Rect& pixelDimensions, const Ogre::String& texture)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		return _createVerticalTrackBar(name,pixelDimensions,texture);
	}

	VerticalTrackBar* Panel::createVerticalTrackBar(const Rect& pixelDimensions, const Ogre::String& texture)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		return _createVerticalTrackBar(name,pixelDimensions,texture);
	}

	VerticalTrackBar* Panel::createVerticalTrackBar(const Ogre::String& name, const Rect& pixelDimensions)
	{
		if( !(mGUIManager->validWidgetName(name)) ) return NULL;

		Ogre::String texture = getDefaultSkin() + ".trackbar.vertical.png";

		return _createVerticalTrackBar(name,pixelDimensions,texture);
	}

	VerticalTrackBar* Panel::createVerticalTrackBar(const Rect& pixelDimensions)
	{
		Ogre::String name = mInstanceName + ".ChildWidget" + Ogre::StringConverter::toString(mAutoNameWidgetCounter);
		++mAutoNameWidgetCounter;

		Ogre::String texture = getDefaultSkin() + ".trackbar.vertical.png";

		return _createVerticalTrackBar(name,pixelDimensions,texture);
	}

	Button* Panel::getButton(unsigned int index)
	{
		if( index >= mNumButtons ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_BUTTON,index);
		if( w != NULL ) return dynamic_cast<Button*>(w);
		else return NULL;
	}

	Button* Panel::getButton(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Button*>(w);
		else return NULL;
	}

	ComboBox* Panel::getComboBox(unsigned int index)
	{
		if( index >= mNumComboBoxes ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_COMBOBOX,index);
		if( w != NULL ) return dynamic_cast<ComboBox*>(w);
		else return NULL;
	}

	ComboBox* Panel::getComboBox(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<ComboBox*>(w);
		else return NULL;
	}

	HorizontalScrollBar* Panel::getHorizontalScrollBar(unsigned int index)
	{
		if( index >= mNumHorizontalScrollBars ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_SCROLLBAR_HORIZONTAL,index);
		if( w != NULL ) return dynamic_cast<HorizontalScrollBar*>(w);
		else return NULL;
	}

	HorizontalScrollBar* Panel::getHorizontalScrollBar(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<HorizontalScrollBar*>(w);
		else return NULL;
	}

	Image* Panel::getImage(unsigned int index)
	{
		if( index >= mNumImages ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_IMAGE,index);
		if( w != NULL ) return dynamic_cast<Image*>(w);
		else return NULL;
	}

	Image* Panel::getImage(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Image*>(w);
		else return NULL;
	}

	Label* Panel::getLabel(unsigned int index)
	{
		if( index >= mNumLabels ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_LABEL,index);
		if( w != NULL ) return dynamic_cast<Label*>(w);
		else return NULL;
	}

	Label* Panel::getLabel(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Label*>(w);
		else return NULL;
	}

	List* Panel::getList(unsigned int index)
	{
		if( index >= mNumLists ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_LIST,index);
		if( w != NULL ) return dynamic_cast<List*>(w);
		else return NULL;
	}

	List* Panel::getList(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<List*>(w);
		else return NULL;
	}

	Menu* Panel::getMenu( unsigned int index )
	{
		if( index >= mNumMenus ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_MENU,index);
		if( w != NULL ) return dynamic_cast<Menu*>(w);
		else return NULL;
	}

	Menu* Panel::getMenu(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Menu*>(w);
		else return NULL;
	}

	NStateButton* Panel::getNStateButton(unsigned int index)
	{
		if( index >= mNumNStateButtons ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_NSTATEBUTTON,index);
		if( w != NULL ) return dynamic_cast<NStateButton*>(w);
		else return NULL;
	}
		
	NStateButton* Panel::getNStateButton(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<NStateButton*>(w);
		else return NULL;
	}

	Panel* Panel::getPanel(unsigned int index)
	{
		if( index >= mNumPanels ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_PANEL,index);
		if( w != NULL ) return dynamic_cast<Panel*>(w);
		else return NULL;
	}

	Panel* Panel::getPanel(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<Panel*>(w);
		else return NULL;
	}

	ProgressBar* Panel::getProgressBar(unsigned int index)
	{
		if( index >= mNumProgressBars ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_PROGRESSBAR,index);
		if( w != NULL ) return dynamic_cast<ProgressBar*>(w);
		else return NULL;
	}

	ProgressBar* Panel::getProgressBar(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<ProgressBar*>(w);
		else return NULL;
	}

	ScrollPane* Panel::getScrollPane()
	{
		return mScrollPane;
	}

	TextBox* Panel::getTextBox(unsigned int index)
	{
		if( index >= mNumTextBoxes ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_TEXTBOX,index);
		if( w != NULL ) return dynamic_cast<TextBox*>(w);
		else return NULL;
	}

	TextBox* Panel::getTextBox(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<TextBox*>(w);
		else return NULL;
	}

	HorizontalTrackBar* Panel::getHorizontalTrackBar(unsigned int index)
	{
		if( index >= mNumHorizontalTrackBars ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_TRACKBAR_HORIZONTAL,index);
		if( w != NULL ) return dynamic_cast<HorizontalTrackBar*>(w);
		else return NULL;
	}

	HorizontalTrackBar* Panel::getHorizontalTrackBar(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<HorizontalTrackBar*>(w);
		else return NULL;
	}

	VerticalScrollBar* Panel::getVerticalScrollBar(unsigned int index)
	{
		if( index >= mNumVerticalScrollBars ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_SCROLLBAR_VERTICAL,index);
		if( w != NULL ) return dynamic_cast<VerticalScrollBar*>(w);
		else return NULL;
	}

	VerticalScrollBar* Panel::getVerticalScrollBar(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<VerticalScrollBar*>(w);
		else return NULL;
	}

	VerticalTrackBar* Panel::getVerticalTrackBar(unsigned int index)
	{
		if( index >= mNumVerticalTrackBars ) return NULL;

		Widget* w = _getWidget(Widget::TYPE_TRACKBAR_VERTICAL,index);
		if( w != NULL ) return dynamic_cast<VerticalTrackBar*>(w);
		else return NULL;
	}

	VerticalTrackBar* Panel::getVerticalTrackBar(const Ogre::String& name)
	{
		Widget* w = _getWidget(name);
		if( w != NULL ) return dynamic_cast<VerticalTrackBar*>(w);
		else return NULL;
	}
}
