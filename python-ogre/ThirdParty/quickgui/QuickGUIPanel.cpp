#include "QuickGUIManager.h"
#include "QuickGUIPanel.h"
#include "QuickGUISheet.h"

namespace QuickGUI
{
	Panel::Panel(const Ogre::String& name, const Rect& pixelDimensions, Ogre::String texture, GUIManager* gm) :
		Image(name,pixelDimensions,texture,gm),
		QuadContainer(this),
		mAutoNameWidgetCounter(-1)
	{
		mWidgetType = TYPE_PANEL;
		mScrollPane = new ScrollPane(mInstanceName+".ScrollPane",Rect(0,0,mSize.width,mSize.height),mGUIManager);
		addChild(mScrollPane);
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

	void Panel::addChild(Widget* w)
	{
		if(w->getParentWidget() != NULL)
			return;

		mChildWidgets.push_back(w);
		// GUIManager must be set first.
		w->setGUIManager(mGUIManager);
		w->setParent(this);
		w->setQuadContainer(this);

		WidgetEventArgs args(w);
		fireEvent(EVENT_CHILD_ADDED,args);
	}

	Button* Panel::createButton()
	{
		++mAutoNameWidgetCounter;

		Button* newButton = new Button(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,75,25),"qgui.button.png",mGUIManager);
		addChild(newButton);

		return newButton;
	}

	ComboBox* Panel::createComboBox()
	{
		++mAutoNameWidgetCounter;

		ComboBox* newComboBox = new ComboBox(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,125,25),"qgui.combobox.png",mGUIManager);
		addChild(newComboBox);

		return newComboBox;
	}

	HorizontalScrollBar* Panel::createHorizontalScrollBar()
	{
		++mAutoNameWidgetCounter;

		HorizontalScrollBar* newHorizontalScrollBar = new HorizontalScrollBar(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,100,50),"qgui.scrollbar.horizontal.png",mGUIManager);
		addChild(newHorizontalScrollBar);

		return newHorizontalScrollBar;
	}

	HorizontalTrackBar* Panel::createHorizontalTrackBar()
	{
		++mAutoNameWidgetCounter;

		HorizontalTrackBar* newHorizontalTrackBar = new HorizontalTrackBar(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,80,20),"qgui.trackbar.horizontal.png",mGUIManager);
		addChild(newHorizontalTrackBar);

		return newHorizontalTrackBar;
	}

	Image* Panel::createImage()
	{
		++mAutoNameWidgetCounter;

		Image* newImage = new Image(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,50,50),"qgui.image.png",mGUIManager);
		addChild(newImage);

		return newImage;
	}

	Label* Panel::createLabel()
	{
		++mAutoNameWidgetCounter;

		Label* newLabel = new Label(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,35,15),"qgui.label.png",mGUIManager);
		addChild(newLabel);

		return newLabel;
	}

	List* Panel::createList()
	{
		++mAutoNameWidgetCounter;

		List* newList = new List(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,120,100),"qgui.list.png",mGUIManager);
		addChild(newList);

		return newList;
	}

	Menu* Panel::createMenu()
	{
		++mAutoNameWidgetCounter;

		Menu* newMenu = new Menu(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,mSize.width,25),"qgui.menu.png",mGUIManager); 
		addChild(newMenu);

		return newMenu;
	}

	NStateButton* Panel::createNStateButton()
	{
		++mAutoNameWidgetCounter;

		NStateButton* newNStateButton = new NStateButton(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,75,25),mGUIManager);
		addChild(newNStateButton);

		return newNStateButton;
	}

	Panel* Panel::createPanel()
	{
		++mAutoNameWidgetCounter;

		Panel* newPanel = new Panel(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,200,100),"qgui.panel.png",mGUIManager);
		addChild(newPanel);

		return newPanel;
	}

	ProgressBar* Panel::createProgressBar()
	{
		++mAutoNameWidgetCounter;

		ProgressBar* newProgressBar = new ProgressBar(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,100,25),"qgui.progressbar.png",mGUIManager);
		addChild(newProgressBar);

		return newProgressBar;
	}

	TextBox* Panel::createTextBox()
	{
		++mAutoNameWidgetCounter;

		TextBox* newTextBox = new TextBox(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,100,20),"qgui.textbox.png",mGUIManager);
		addChild(newTextBox);

		return newTextBox;
	}

	VerticalScrollBar* Panel::createVerticalScrollBar()
	{
		++mAutoNameWidgetCounter;

		VerticalScrollBar* newVerticalScrollBar = new VerticalScrollBar(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,50,100),"qgui.scrollbar.vertical.png",mGUIManager);
		addChild(newVerticalScrollBar);

		return newVerticalScrollBar;
	}

	VerticalTrackBar* Panel::createVerticalTrackBar()
	{
		++mAutoNameWidgetCounter;

		VerticalTrackBar* newVerticalTrackBar = new VerticalTrackBar(mInstanceName+".Child"+Ogre::StringConverter::toString(mAutoNameWidgetCounter),Rect(0,0,20,80),"qgui.trackbar.vertical.png",mGUIManager);
		addChild(newVerticalTrackBar);

		return newVerticalTrackBar;
	}

	void Panel::setQuadContainer(QuadContainer* container)
	{
		if((mQuadContainer != NULL) && (mQuadContainer->getID() != mQuadContainer->getID()))
			mQuadContainer->removeChildPanelContainer(QuadContainer::getID());

		mQuadContainer = container;
		mQuadContainer->addChildPanelContainer(this);
	}
}
