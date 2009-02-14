#include "QuickGUIRoot.h"

#include "QuickGUIButton.h"
#include "QuickGUICheckBox.h"
#include "QuickGUIColorPicker.h"
#include "QuickGUIComboBox.h"
#include "QuickGUIConsole.h"
#include "QuickGUIContextMenu.h"
#include "QuickGUIHScrollBar.h"
#include "QuickGUIImage.h"
#include "QuickGUILabel.h"
#include "QuickGUIList.h"
#include "QuickGUIListTextItem.h"
#include "QuickGUIListImageItem.h"
#include "QuickGUIMenu.h"
#include "QuickGUIMenuLabel.h"
#include "QuickGUIMenuPanel.h"
#include "QuickGUIProgressBar.h"
#include "QuickGUIPropertyGrid.h"
#include "QuickGUIPropertyGridSection.h"
#include "QuickGUIPropertyGridTextProperty.h"
#include "QuickGUIPropertyGridBoolProperty.h"
#include "QuickGUIPropertyGridComboBoxProperty.h"
#include "QuickGUIRadioButton.h"
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
		OGRE_NEW_T(EventHandlerManager,Ogre::MEMCATEGORY_GENERAL)();
		OGRE_NEW_T(FactoryManager,Ogre::MEMCATEGORY_GENERAL)();
		OGRE_NEW_T(SkinDefinitionManager,Ogre::MEMCATEGORY_GENERAL)();
		OGRE_NEW_T(TimerManager,Ogre::MEMCATEGORY_GENERAL)();
		OGRE_NEW_T(SheetManager,Ogre::MEMCATEGORY_GENERAL) ();

		// Register default supported Widget types via FactoryManager
		Factory<Widget>* widgetFactory = FactoryManager::getSingletonPtr()->getWidgetFactory();
		widgetFactory->registerClass<Button>("Button");
		widgetFactory->registerClass<CheckBox>("CheckBox");
		widgetFactory->registerClass<ColorPicker>("ColorPicker");
		widgetFactory->registerClass<ComboBox>("ComboBox");
		widgetFactory->registerClass<Console>("Console");
		widgetFactory->registerClass<ContextMenu>("ContextMenu");
		widgetFactory->registerClass<HScrollBar>("HScrollBar");
		widgetFactory->registerClass<Image>("Image");
		widgetFactory->registerClass<Label>("Label");
		widgetFactory->registerClass<List>("List");
		widgetFactory->registerClass<ListImageItem>("ListImageItem");
		widgetFactory->registerClass<ListTextItem>("ListTextItem");
		widgetFactory->registerClass<Menu>("Menu");
		widgetFactory->registerClass<MenuLabel>("MenuLabel");
		widgetFactory->registerClass<MenuPanel>("MenuPanel");
		widgetFactory->registerClass<ModalWindow>("ModalWindow");
		widgetFactory->registerClass<Panel>("Panel");
		widgetFactory->registerClass<ProgressBar>("ProgressBar");
		widgetFactory->registerClass<PropertyGrid>("PropertyGrid");
		widgetFactory->registerClass<PropertyGridSection>("PropertyGridSection");
		widgetFactory->registerClass<PropertyGridTextProperty>("PropertyGridTextProperty");
		widgetFactory->registerClass<PropertyGridBoolProperty>("PropertyGridBoolProperty");
		widgetFactory->registerClass<PropertyGridComboBoxProperty>("PropertyGridComboBoxProperty");
		widgetFactory->registerClass<RadioButton>("RadioButton");
		widgetFactory->registerClass<TabControl>("TabControl");
		widgetFactory->registerClass<Tab>("Tab");
		widgetFactory->registerClass<TabPage>("TabPage");
		widgetFactory->registerClass<TextArea>("TextArea");
		widgetFactory->registerClass<TextBox>("TextBox");
		widgetFactory->registerClass<TitleBar>("TitleBar");
		widgetFactory->registerClass<ToolBar>("ToolBar");
		widgetFactory->registerClass<TreeView>("TreeView");
		widgetFactory->registerClass<TreeViewNode>("TreeViewNode");
		widgetFactory->registerClass<TreeViewCheckBoxNode>("TreeViewCheckBoxNode");
		widgetFactory->registerClass<TreeViewRadioButtonNode>("TreeViewRadioButtonNode");
		widgetFactory->registerClass<VScrollBar>("VScrollBar");
		widgetFactory->registerClass<Window>("Window");

		// Register default supported Widget desc types
		Factory<Desc>* descFactory = FactoryManager::getSingletonPtr()->getDescFactory();
		descFactory->registerClass<ButtonDesc>("ButtonDesc");
		descFactory->registerClass<CheckBoxDesc>("CheckBoxDesc");
		descFactory->registerClass<ColorPickerDesc>("ColorPickerDesc");
		descFactory->registerClass<ComboBoxDesc>("ComboBoxDesc");
		descFactory->registerClass<ConsoleDesc>("ConsoleDesc");
		descFactory->registerClass<ContextMenuDesc>("ContextMenuDesc");
		descFactory->registerClass<HScrollBarDesc>("HScrollBarDesc");
		descFactory->registerClass<ImageDesc>("ImageDesc");
		descFactory->registerClass<LabelDesc>("LabelDesc");
		descFactory->registerClass<ListDesc>("ListDesc");
		descFactory->registerClass<ListImageItemDesc>("ListImageItemDesc");
		descFactory->registerClass<ListTextItemDesc>("ListTextItemDesc");
		descFactory->registerClass<MenuDesc>("MenuDesc");
		descFactory->registerClass<MenuLabelDesc>("MenuLabelDesc");
		descFactory->registerClass<MenuPanelDesc>("MenuPanelDesc");
		descFactory->registerClass<ModalWindowDesc>("ModalWindowDesc");
		descFactory->registerClass<PanelDesc>("PanelDesc");
		descFactory->registerClass<ProgressBarDesc>("ProgressBarDesc");
		descFactory->registerClass<PropertyGridDesc>("PropertyGridDesc");
		descFactory->registerClass<PropertyGridSectionDesc>("PropertyGridSectionDesc");
		descFactory->registerClass<PropertyGridTextPropertyDesc>("PropertyGridTextPropertyDesc");
		descFactory->registerClass<PropertyGridBoolPropertyDesc>("PropertyGridBoolPropertyDesc");
		descFactory->registerClass<PropertyGridComboBoxPropertyDesc>("PropertyGridComboBoxPropertyDesc");
		descFactory->registerClass<RadioButtonDesc>("RadioButtonDesc");
		descFactory->registerClass<SheetDesc>("SheetDesc");
		descFactory->registerClass<TabControlDesc>("TabControlDesc");
		descFactory->registerClass<TabDesc>("TabDesc");
		descFactory->registerClass<TabPageDesc>("TabPageDesc");
		descFactory->registerClass<TextAreaDesc>("TextAreaDesc");
		descFactory->registerClass<TextBoxDesc>("TextBoxDesc");
		descFactory->registerClass<TitleBarDesc>("TitleBarDesc");
		descFactory->registerClass<ToolBarDesc>("ToolBarDesc");
		descFactory->registerClass<TreeViewDesc>("TreeViewDesc");
		descFactory->registerClass<TreeViewNodeDesc>("TreeViewNodeDesc");
		descFactory->registerClass<TreeViewCheckBoxNodeDesc>("TreeViewCheckBoxNodeDesc");
		descFactory->registerClass<TreeViewRadioButtonNodeDesc>("TreeViewRadioButtonNodeDesc");
		descFactory->registerClass<VScrollBarDesc>("VScrollBarDesc");
		descFactory->registerClass<WindowDesc>("WindowDesc");

		// Create default Descs for use throughout apps using QuickGUI
		descFactory->createInstance<ButtonDesc>("ButtonDesc","DefaultButtonDesc");
		descFactory->createInstance<CheckBoxDesc>("CheckBoxDesc","DefaultCheckBoxDesc");
		descFactory->createInstance<ColorPickerDesc>("ColorPickerDesc","DefaultColorPickerDesc");
		descFactory->createInstance<ComboBoxDesc>("ComboBoxDesc","DefaultComboBoxDesc");
		descFactory->createInstance<ConsoleDesc>("ConsoleDesc","DefaultConsoleDesc");
		descFactory->createInstance<ContextMenuDesc>("ContextMenuDesc","DefaultContextMenuDesc");
		descFactory->createInstance<HScrollBarDesc>("HScrollBarDesc","DefaultHScrollBarDesc");
		descFactory->createInstance<ImageDesc>("ImageDesc","DefaultImageDesc");
		descFactory->createInstance<LabelDesc>("LabelDesc","DefaultLabelDesc");
		descFactory->createInstance<ListDesc>("ListDesc","DefaultListDesc");
		descFactory->createInstance<ListImageItemDesc>("ListImageItemDesc","DefaultListImageItemDesc");
		descFactory->createInstance<ListTextItemDesc>("ListTextItemDesc","DefaultListTextItemDesc");
		descFactory->createInstance<MenuDesc>("MenuDesc","DefaultMenuDesc");
		descFactory->createInstance<MenuLabelDesc>("MenuLabelDesc","DefaultMenuLabelDesc");
		descFactory->createInstance<MenuPanelDesc>("MenuPanelDesc","DefaultMenuPanelDesc");
		descFactory->createInstance<ModalWindowDesc>("ModalWindowDesc","DefaultModalWindowDesc");
		descFactory->createInstance<PanelDesc>("PanelDesc","DefaultPanelDesc");
		descFactory->createInstance<ProgressBarDesc>("ProgressBarDesc","DefaultProgressBarDesc");
		descFactory->createInstance<PropertyGridDesc>("PropertyGridDesc","DefaultPropertyGridDesc");
		descFactory->createInstance<PropertyGridSectionDesc>("PropertyGridSectionDesc","DefaultPropertyGridSectionDesc");
		descFactory->createInstance<PropertyGridTextPropertyDesc>("PropertyGridTextPropertyDesc","DefaultPropertyGridTextPropertyDesc");
		descFactory->createInstance<PropertyGridBoolPropertyDesc>("PropertyGridBoolPropertyDesc","DefaultPropertyGridBoolPropertyDesc");
		descFactory->createInstance<PropertyGridComboBoxPropertyDesc>("PropertyGridComboBoxPropertyDesc","DefaultPropertyGridComboBoxPropertyDesc");
		descFactory->createInstance<RadioButtonDesc>("RadioButtonDesc","DefaultRadioButtonDesc");
		descFactory->createInstance<SheetDesc>("SheetDesc","DefaultSheetDesc");
		descFactory->createInstance<TabControlDesc>("TabControlDesc","DefaultTabControlDesc");
		descFactory->createInstance<TabDesc>("TabDesc","DefaultTabDesc");
		descFactory->createInstance<TabPageDesc>("TabPageDesc","DefaultTabPageDesc");
		descFactory->createInstance<TextAreaDesc>("TextAreaDesc","DefaultTextAreaDesc");
		descFactory->createInstance<TextBoxDesc>("TextBoxDesc","DefaultTextBoxDesc");
		descFactory->createInstance<TitleBarDesc>("TitleBarDesc","DefaultTitleBarDesc");
		descFactory->createInstance<ToolBarDesc>("ToolBarDesc","DefaultToolBarDesc");
		descFactory->createInstance<TreeViewDesc>("TreeViewDesc","DefaultTreeViewDesc");
		descFactory->createInstance<TreeViewNodeDesc>("TreeViewNodeDesc","DefaultTreeViewNodeDesc");
		descFactory->createInstance<TreeViewCheckBoxNodeDesc>("TreeViewCheckBoxNodeDesc","DefaultTreeViewCheckBoxNodeDesc");
		descFactory->createInstance<TreeViewRadioButtonNodeDesc>("TreeViewRadioButtonNodeDesc","DefaultTreeViewRadioButtonNodeDesc");
		descFactory->createInstance<VScrollBarDesc>("VScrollBarDesc","DefaultVScrollBarDesc");
		descFactory->createInstance<WindowDesc>("WindowDesc","DefaultWindowDesc");

		// Register Skin definitions
		MouseCursor::registerSkinDefinition();
		Button::registerSkinDefinition();
		CheckBox::registerSkinDefinition();
		ColorPicker::registerSkinDefinition();
		ComboBox::registerSkinDefinition();
		Console::registerSkinDefinition();
		ContextMenu::registerSkinDefinition();
		HScrollBar::registerSkinDefinition();
		Image::registerSkinDefinition();
		Label::registerSkinDefinition();
		List::registerSkinDefinition();
		ListImageItem::registerSkinDefinition();
		ListTextItem::registerSkinDefinition();
		Menu::registerSkinDefinition();
		MenuLabel::registerSkinDefinition();
		MenuPanel::registerSkinDefinition();
		ModalWindow::registerSkinDefinition();
		Panel::registerSkinDefinition();
		ProgressBar::registerSkinDefinition();
		PropertyGrid::registerSkinDefinition();
		PropertyGridSection::registerSkinDefinition();
		PropertyGridTextProperty::registerSkinDefinition();
		PropertyGridBoolProperty::registerSkinDefinition();
		PropertyGridComboBoxProperty::registerSkinDefinition();
		RadioButton::registerSkinDefinition();
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
		TreeViewNode::registerSkinDefinition();
		TreeViewCheckBoxNode::registerSkinDefinition();
		TreeViewRadioButtonNode::registerSkinDefinition();
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
		OGRE_DELETE_T(EventHandlerManager::getSingletonPtr(),EventHandlerManager,Ogre::MEMCATEGORY_GENERAL);
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

		Ogre::String previousName = d.name;

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

		// restore name
		d.name = previousName;

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
