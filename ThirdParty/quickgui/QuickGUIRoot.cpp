#include "QuickGUIRoot.h"

#include "QuickGUIButton.h"
#include "QuickGUICheckBox.h"
#include "QuickGUIConsole.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIImage.h"
#include "QuickGUILabel.h"
#include "QuickGUIList.h"
#include "QuickGUIListTextItem.h"
#include "QuickGUIMenu.h"
#include "QuickGUIMenuLabel.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUIProgressBar.h"
#include "QuickGUISheet.h"
#include "QuickGUITab.h"
#include "QuickGUITabControl.h"
#include "QuickGUITabPage.h"
#include "QuickGUITextArea.h"
#include "QuickGUITextBox.h"
#include "QuickGUIToolBar.h"
#include "QuickGUIVScrollBar.h"

template<> QuickGUI::Root* Ogre::Singleton<QuickGUI::Root>::ms_Singleton = 0;

namespace QuickGUI
{
	Root::Root() :
		mGUIManagerCounter(-1),
		mDefaultHoverTime(3)
	{
		// Get the default font
		Ogre::ResourceManager::ResourceMapIterator it = Ogre::FontManager::getSingleton().getResourceIterator();
		if(it.hasMoreElements())
			mDefaultFont = static_cast<Ogre::FontPtr>(it.getNext());
		else
			throw Exception(Exception::ERR_INVALID_STATE,"No fonts have been found! At least one font must exist for QuickGUI use!","Root::Root");

		// Initialize all Singleton Manager classes
		new Brush();
		new SkinDefinitionManager();
		new TimerManager();
		new WidgetFactoryManager();
		new WidgetDescFactoryManager();

		// Register default supported Widget types via FactoryManager
		WidgetFactoryManager* wfm = WidgetFactoryManager::getSingletonPtr();
		wfm->registerWidgetFactory("Button",Button::factory);
		wfm->registerWidgetFactory("CheckBox",CheckBox::factory);
		wfm->registerWidgetFactory("Console",Console::factory);
		wfm->registerWidgetFactory("HScrollBar",HScrollBar::factory);
		wfm->registerWidgetFactory("Image",Image::factory);
		wfm->registerWidgetFactory("Label",Label::factory);
		wfm->registerWidgetFactory("List",List::factory);
		wfm->registerWidgetFactory("ListTextItem",ListTextItem::factory);
		wfm->registerWidgetFactory("Menu",Menu::factory);
		wfm->registerWidgetFactory("MenuLabel",MenuLabel::factory);
		wfm->registerWidgetFactory("MenuPanel",MenuPanel::factory);
		wfm->registerWidgetFactory("ModalWindow",ModalWindow::factory);
		wfm->registerWidgetFactory("Panel",Panel::factory);
		wfm->registerWidgetFactory("ProgressBar",ProgressBar::factory);
		wfm->registerWidgetFactory("TabControl",TabControl::factory);
		wfm->registerWidgetFactory("Tab",Tab::factory);
		wfm->registerWidgetFactory("TabPage",TabPage::factory);
		wfm->registerWidgetFactory("TextArea",TextArea::factory);
		wfm->registerWidgetFactory("TextBox",TextBox::factory);
		wfm->registerWidgetFactory("TitleBar",TitleBar::factory);
		wfm->registerWidgetFactory("ToolBar",ToolBar::factory);
		wfm->registerWidgetFactory("VScrollBar",VScrollBar::factory);
		wfm->registerWidgetFactory("Window",Window::factory);

		// Register default supported Widget desc types via FactoryManager
		WidgetDescFactoryManager* wdfm = WidgetDescFactoryManager::getSingletonPtr();
		wdfm->registerWidgetDescFactory("ButtonDesc",ButtonDesc::factory);
		wdfm->registerWidgetDescFactory("CheckBoxDesc",CheckBoxDesc::factory);
		wdfm->registerWidgetDescFactory("ConsoleDesc",ConsoleDesc::factory);
		wdfm->registerWidgetDescFactory("HScrollBarDesc",HScrollBarDesc::factory);
		wdfm->registerWidgetDescFactory("ImageDesc",ImageDesc::factory);
		wdfm->registerWidgetDescFactory("LabelDesc",LabelDesc::factory);
		wdfm->registerWidgetDescFactory("ListDesc",ListDesc::factory);
		wdfm->registerWidgetDescFactory("ListTextItemDesc",ListTextItemDesc::factory);
		wdfm->registerWidgetDescFactory("MenuDesc",MenuDesc::factory);
		wdfm->registerWidgetDescFactory("MenuLabelDesc",MenuLabelDesc::factory);
		wdfm->registerWidgetDescFactory("MenuPanelDesc",MenuPanelDesc::factory);
		wdfm->registerWidgetDescFactory("ModalWindowDesc",ModalWindowDesc::factory);
		wdfm->registerWidgetDescFactory("PanelDesc",PanelDesc::factory);
		wdfm->registerWidgetDescFactory("ProgressBarDesc",ProgressBarDesc::factory);
		wdfm->registerWidgetDescFactory("SheetDesc",SheetDesc::factory);
		wdfm->registerWidgetDescFactory("TabDesc",TabDesc::factory);
		wdfm->registerWidgetDescFactory("TabControlDesc",TabControlDesc::factory);
		wdfm->registerWidgetDescFactory("TabPageDesc",TabPageDesc::factory);
		wdfm->registerWidgetDescFactory("TextAreaDesc",TextAreaDesc::factory);
		wdfm->registerWidgetDescFactory("TextBoxDesc",TextBoxDesc::factory);
		wdfm->registerWidgetDescFactory("TitleBarDesc",TitleBarDesc::factory);
		wdfm->registerWidgetDescFactory("ToolBarDesc",ToolBarDesc::factory);
		wdfm->registerWidgetDescFactory("VScrollBarDesc",VScrollBarDesc::factory);
		wdfm->registerWidgetDescFactory("WindowDesc",WindowDesc::factory);

		// Register Skin definitions
		MouseCursor::registerSkinDefinition();
		Button::registerSkinDefinition();
		CheckBox::registerSkinDefinition();
		Console::registerSkinDefinition();
		HScrollBar::registerSkinDefinition();
		Image::registerSkinDefinition();
		Label::registerSkinDefinition();
		List::registerSkinDefinition();
		ListTextItem::registerSkinDefinition();
		Menu::registerSkinDefinition();
		MenuLabel::registerSkinDefinition();
		MenuPanel::registerSkinDefinition();
		ModalWindow::registerSkinDefinition();
		Panel::registerSkinDefinition();
		ProgressBar::registerSkinDefinition();
		Sheet::registerSkinDefinition();
		Tab::registerSkinDefinition();
		TabControl::registerSkinDefinition();
		TabPage::registerSkinDefinition();
		TextArea::registerSkinDefinition();
		TextBox::registerSkinDefinition();
		TextCursor::registerSkinDefinition();
		TitleBar::registerSkinDefinition();
		ToolBar::registerSkinDefinition();
		VScrollBar::registerSkinDefinition();
		Window::registerSkinDefinition();

		// Initialize other Singleton classes
		new ScriptWriter();
		new SerialWriter();
		new SerialReader();
	}

	Root::~Root()
	{
		for(std::map<std::string,GUIManager*>::iterator it = mGUIManagers.begin(); it != mGUIManagers.end(); ++it)
			delete (*it).second;
		mGUIManagers.clear();		

		delete WidgetDescFactoryManager::getSingletonPtr();
		delete WidgetFactoryManager::getSingletonPtr();
		delete TimerManager::getSingletonPtr();
		delete Brush::getSingletonPtr();
		delete SkinDefinitionManager::getSingletonPtr();
		delete ScriptReader::getSingletonPtr();
		delete ScriptWriter::getSingletonPtr();
		delete SerialWriter::getSingletonPtr();
		delete SerialReader::getSingletonPtr();
	}

	Root* Root::getSingletonPtr(void) 
	{ 
		return ms_Singleton; 
	}

	Root& Root::getSingleton(void) 
	{ 
		assert( ms_Singleton );  
		return ( *ms_Singleton ); 
	}

	GUIManager* Root::createGUIManager(GUIManagerDesc& d)
	{
		++mGUIManagerCounter;

		// If name string is empty, generate a name for the GUIManager.
		if(d.name == "")
			d.name = "GUIManager." + Ogre::StringConverter::toString(mGUIManagerCounter);
		else
		{
			// make sure name does not already exist.
			if(mGUIManagers.find(d.name) != mGUIManagers.end())
				throw Ogre::Exception(Ogre::Exception::ERR_DUPLICATE_ITEM,"A GUIManager with name \"" + d.name + "\" already exists!","Root::createGUIManager");
		}

		GUIManager* newGUIManager = new GUIManager(d);
		mGUIManagers[d.name] = newGUIManager;

		return newGUIManager;
	}

	void Root::destroyGUIManager(GUIManager* gm)
	{
		if(gm == NULL)
			return;

		destroyGUIManager(gm->getName());
	}

	void Root::destroyGUIManager(const std::string& name)
	{
		GUIManager* gm = mGUIManagers[name];
		mGUIManagers.erase(mGUIManagers.find(name));
		delete gm;
	}

	GUIManager* Root::getGUIManager(const std::string& name)
	{
		if(mGUIManagers.find(name) == mGUIManagers.end())
			return NULL;

		return mGUIManagers[name];
	}

	Ogre::String Root::getDefaultFontName()
	{
		if(mDefaultFont.isNull())
			return "";
		else
			return mDefaultFont->getName();
	}

	float Root::getDefaultHoverTime()
	{
		return mDefaultHoverTime;
	}

	void Root::setDefaultHoverTime(float seconds)
	{
		mDefaultHoverTime = seconds;
	}
}
