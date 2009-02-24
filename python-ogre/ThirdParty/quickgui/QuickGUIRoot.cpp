#include "QuickGUIRoot.h"

#include "QuickGUIButton.h"
#include "QuickGUICheckBox.h"
#include "QuickGUIComboBox.h"
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
#include "QuickGUITreeView.h"
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
		OGRE_NEW_T(Brush,Ogre::MEMCATEGORY_GENERAL)();
		OGRE_NEW_T(FactoryManager,Ogre::MEMCATEGORY_GENERAL)();
		OGRE_NEW_T(SkinDefinitionManager,Ogre::MEMCATEGORY_GENERAL)();
		OGRE_NEW_T(TimerManager,Ogre::MEMCATEGORY_GENERAL)();
		OGRE_NEW_T(SheetManager,Ogre::MEMCATEGORY_GENERAL) ();

		// Register default supported Widget types via FactoryManager
		WidgetFactory<Widget>* widgetFactory = FactoryManager::getSingletonPtr()->getWidgetFactory();
		widgetFactory->registerClass<Button>("Button");
		widgetFactory->registerClass<CheckBox>("CheckBox");
		widgetFactory->registerClass<ComboBox>("ComboBox");
		widgetFactory->registerClass<Console>("Console");
		widgetFactory->registerClass<HScrollBar>("HScrollBar");
		widgetFactory->registerClass<Image>("Image");
		widgetFactory->registerClass<Label>("Label");
		widgetFactory->registerClass<List>("List");
		widgetFactory->registerClass<ListTextItem>("ListTextItem");
		widgetFactory->registerClass<Menu>("Menu");
		widgetFactory->registerClass<MenuLabel>("MenuLabel");
		widgetFactory->registerClass<MenuPanel>("MenuPanel");
		widgetFactory->registerClass<ModalWindow>("ModalWindow");
		widgetFactory->registerClass<Panel>("Panel");
		widgetFactory->registerClass<ProgressBar>("ProgressBar");
		widgetFactory->registerClass<TabControl>("TabControl");
		widgetFactory->registerClass<Tab>("Tab");
		widgetFactory->registerClass<TabPage>("TabPage");
		widgetFactory->registerClass<TextArea>("TextArea");
		widgetFactory->registerClass<TextBox>("TextBox");
		widgetFactory->registerClass<TitleBar>("TitleBar");
		widgetFactory->registerClass<ToolBar>("ToolBar");
		widgetFactory->registerClass<TreeView>("TreeView");
		widgetFactory->registerClass<VScrollBar>("VScrollBar");
		widgetFactory->registerClass<Window>("Window");

		// Register default supported Widget desc types
		Factory<WidgetDesc>* widgetDescFactory = FactoryManager::getSingletonPtr()->getWidgetDescFactory();
		widgetDescFactory->registerClass<ButtonDesc>("ButtonDesc");
		widgetDescFactory->registerClass<CheckBoxDesc>("CheckBoxDesc");
		widgetDescFactory->registerClass<ComboBoxDesc>("ComboBoxDesc");
		widgetDescFactory->registerClass<ConsoleDesc>("ConsoleDesc");
		widgetDescFactory->registerClass<HScrollBarDesc>("HScrollBarDesc");
		widgetDescFactory->registerClass<ImageDesc>("ImageDesc");
		widgetDescFactory->registerClass<LabelDesc>("LabelDesc");
		widgetDescFactory->registerClass<ListDesc>("ListDesc");
		widgetDescFactory->registerClass<ListTextItemDesc>("ListTextItemDesc");
		widgetDescFactory->registerClass<MenuDesc>("MenuDesc");
		widgetDescFactory->registerClass<MenuLabelDesc>("MenuLabelDesc");
		widgetDescFactory->registerClass<MenuPanelDesc>("MenuPanelDesc");
		widgetDescFactory->registerClass<ModalWindowDesc>("ModalWindowDesc");
		widgetDescFactory->registerClass<PanelDesc>("PanelDesc");
		widgetDescFactory->registerClass<ProgressBarDesc>("ProgressBarDesc");
		widgetDescFactory->registerClass<SheetDesc>("SheetDesc");
		widgetDescFactory->registerClass<TabControlDesc>("TabControlDesc");
		widgetDescFactory->registerClass<TabDesc>("TabDesc");
		widgetDescFactory->registerClass<TabPageDesc>("TabPageDesc");
		widgetDescFactory->registerClass<TextAreaDesc>("TextAreaDesc");
		widgetDescFactory->registerClass<TextBoxDesc>("TextBoxDesc");
		widgetDescFactory->registerClass<TitleBarDesc>("TitleBarDesc");
		widgetDescFactory->registerClass<ToolBarDesc>("ToolBarDesc");
		widgetDescFactory->registerClass<TreeViewDesc>("TreeViewDesc");
		widgetDescFactory->registerClass<VScrollBarDesc>("VScrollBarDesc");
		widgetDescFactory->registerClass<WindowDesc>("WindowDesc");

		// Create default Descs for use throughout apps using QuickGUI
		widgetDescFactory->createInstance("ButtonDesc","DefaultButtonDesc");
		widgetDescFactory->createInstance("CheckBoxDesc","DefaultCheckBoxDesc");
		widgetDescFactory->createInstance("ComboBoxDesc","DefaultComboBoxDesc");
		widgetDescFactory->createInstance("ConsoleDesc","DefaultConsoleDesc");
		widgetDescFactory->createInstance("HScrollBarDesc","DefaultHScrollBarDesc");
		widgetDescFactory->createInstance("ImageDesc","DefaultImageDesc");
		widgetDescFactory->createInstance("LabelDesc","DefaultLabelDesc");
		widgetDescFactory->createInstance("ListDesc","DefaultListDesc");
		widgetDescFactory->createInstance("ListTextItemDesc","DefaultListTextItemDesc");
		widgetDescFactory->createInstance("MenuDesc","DefaultMenuDesc");
		widgetDescFactory->createInstance("MenuLabelDesc","DefaultMenuLabelDesc");
		widgetDescFactory->createInstance("MenuPanelDesc","DefaultMenuPanelDesc");
		widgetDescFactory->createInstance("ModalWindowDesc","DefaultModalWindowDesc");
		widgetDescFactory->createInstance("PanelDesc","DefaultPanelDesc");
		widgetDescFactory->createInstance("ProgressBarDesc","DefaultProgressBarDesc");
		widgetDescFactory->createInstance("SheetDesc","DefaultSheetDesc");
		widgetDescFactory->createInstance("TabControlDesc","DefaultTabControlDesc");
		widgetDescFactory->createInstance("TabDesc","DefaultTabDesc");
		widgetDescFactory->createInstance("TabPageDesc","DefaultTabPageDesc");
		widgetDescFactory->createInstance("TextAreaDesc","DefaultTextAreaDesc");
		widgetDescFactory->createInstance("TextBoxDesc","DefaultTextBoxDesc");
		widgetDescFactory->createInstance("TitleBarDesc","DefaultTitleBarDesc");
		widgetDescFactory->createInstance("ToolBarDesc","DefaultToolBarDesc");
		widgetDescFactory->createInstance("TreeViewDesc","DefaultTreeViewDesc");
		widgetDescFactory->createInstance("VScrollBarDesc","DefaultVScrollBarDesc");
		widgetDescFactory->createInstance("WindowDesc","DefaultWindowDesc");

		// Register Skin definitions
		MouseCursor::registerSkinDefinition();
		Button::registerSkinDefinition();
		CheckBox::registerSkinDefinition();
		ComboBox::registerSkinDefinition();
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
		TreeView::registerSkinDefinition();
		VScrollBar::registerSkinDefinition();
		Window::registerSkinDefinition();

		// Initialize other Singleton classes
		OGRE_NEW_T(ScriptWriter,Ogre::MEMCATEGORY_GENERAL)();
		OGRE_NEW_T(SerialWriter,Ogre::MEMCATEGORY_GENERAL)();
		OGRE_NEW_T(SerialReader,Ogre::MEMCATEGORY_GENERAL)();
	}

	Root::~Root()
	{
		for(std::map<std::string,GUIManager*>::iterator it = mGUIManagers.begin(); it != mGUIManagers.end(); ++it)
			OGRE_DELETE_T((*it).second,GUIManager,Ogre::MEMCATEGORY_GENERAL);
		mGUIManagers.clear();

		OGRE_DELETE_T(ScriptReader::getSingletonPtr(),ScriptReader,Ogre::MEMCATEGORY_GENERAL);
		OGRE_DELETE_T(ScriptWriter::getSingletonPtr(),ScriptWriter,Ogre::MEMCATEGORY_GENERAL);
		OGRE_DELETE_T(SerialWriter::getSingletonPtr(),SerialWriter,Ogre::MEMCATEGORY_GENERAL);
		OGRE_DELETE_T(SerialReader::getSingletonPtr(),SerialReader,Ogre::MEMCATEGORY_GENERAL);

		OGRE_DELETE_T(SheetManager::getSingletonPtr(),SheetManager,Ogre::MEMCATEGORY_GENERAL);
		OGRE_DELETE_T(TimerManager::getSingletonPtr(),TimerManager,Ogre::MEMCATEGORY_GENERAL);
		OGRE_DELETE_T(FactoryManager::getSingletonPtr(),FactoryManager,Ogre::MEMCATEGORY_GENERAL);
		OGRE_DELETE_T(Brush::getSingletonPtr(),Brush,Ogre::MEMCATEGORY_GENERAL);
		OGRE_DELETE_T(SkinDefinitionManager::getSingletonPtr(),SkinDefinitionManager,Ogre::MEMCATEGORY_GENERAL);
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

		GUIManager* newGUIManager = OGRE_NEW_T(GUIManager,Ogre::MEMCATEGORY_GENERAL)(d);
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
		OGRE_DELETE_T(gm,GUIManager,Ogre::MEMCATEGORY_GENERAL);
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

	void Root::setDefaultFontName(const Ogre::String& fontName)
	{
		mDefaultFont = Text::getFont(fontName);
	}

	void Root::setDefaultHoverTime(float seconds)
	{
		mDefaultHoverTime = seconds;
	}
}
