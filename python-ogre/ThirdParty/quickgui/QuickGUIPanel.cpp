#include "QuickGUIPanel.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"
#include "QuickGUITabControl.h"

namespace QuickGUI
{
	const Ogre::String Panel::BACKGROUND = "background";

	void Panel::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("Panel");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Panel",d);
	}

	Panel::Panel(const Ogre::String& name) :
		ContainerWidget(name)
	{
		mSkinElementName = BACKGROUND;
	}

	Panel::~Panel()
	{
	}

	void Panel::_initialize(WidgetDesc* d)
	{
		ContainerWidget::_initialize(d);

		mPanelDesc = dynamic_cast<PanelDesc*>(mWidgetDesc);

		if(d->getWidgetClass() == "Panel")
			setSkinType(d->widget_skinTypeName);
	}

	Widget* Panel::createCustomWidget(const Ogre::String& className, WidgetDesc* d)
	{
		if((className == "TitleBar") || (className == "Window") || (className == "Sheet"))
			throw Exception(Exception::ERR_UNSUPPORTED_WIDGET,"This widget does not support creation of widget's of class \"" + className + "\".","Panel::createCustomWidget");

		Widget* newCustomWidget = Widget::create(className,d);

		addChild(newCustomWidget);

		return newCustomWidget;
	}

	Button* Panel::createButton(ButtonDesc* d)
	{
		Button* newButton = dynamic_cast<Button*>(Widget::create("Button",d));

		addChild(newButton);

		return newButton;
	}

	CheckBox* Panel::createCheckBox(CheckBoxDesc* d)
	{
		CheckBox* newCheckBox = dynamic_cast<CheckBox*>(Widget::create("CheckBox",d));

		addChild(newCheckBox);

		return newCheckBox;
	}

	ColorPicker* Panel::createColorPicker(ColorPickerDesc* d)
	{
		ColorPicker* newColorPicker = dynamic_cast<ColorPicker*>(Widget::create("ColorPicker",d));

		addChild(newColorPicker);

		return newColorPicker;
	}

	ComboBox* Panel::createComboBox(ComboBoxDesc* d)
	{
		ComboBox* newComboBox = dynamic_cast<ComboBox*>(Widget::create("ComboBox",d));

		addChild(newComboBox);

		return newComboBox;
	}

	Console* Panel::createConsole(ConsoleDesc* d)
	{
		Console* newConsole = dynamic_cast<Console*>(Widget::create("Console",d));

		addChild(newConsole);

		return newConsole;
	}

	HScrollBar* Panel::createHScrollBar(HScrollBarDesc* d)
	{
		HScrollBar* newHScrollBar = dynamic_cast<HScrollBar*>(Widget::create("HScrollBar",d));

		addChild(newHScrollBar);

		return newHScrollBar;
	}

	Image* Panel::createImage(ImageDesc* d)
	{
		Image* newImage = dynamic_cast<Image*>(Widget::create("Image",d));

		addChild(newImage);

		return newImage;
	}

	Label* Panel::createLabel(LabelDesc* d)
	{
		Label* newLabel = dynamic_cast<Label*>(Widget::create("Label",d));

		addChild(newLabel);

		return newLabel;
	}

	List* Panel::createList(ListDesc* d)
	{
		List* newList = dynamic_cast<List*>(Widget::create("List",d));

		addChild(newList);

		return newList;
	}

	Panel* Panel::createPanel(PanelDesc* d)
	{
		Panel* newPanel = dynamic_cast<Panel*>(Widget::create("Panel",d));
		
		addChild(newPanel);

		return newPanel;
	}

	ProgressBar* Panel::createProgressBar(ProgressBarDesc* d)
	{
		ProgressBar* newProgressBar = dynamic_cast<ProgressBar*>(Widget::create("ProgressBar",d));

		addChild(newProgressBar);

		return newProgressBar;
	}

	PropertyGrid* Panel::createPropertyGrid(PropertyGridDesc* d)
	{
		PropertyGrid* newPropertyGrid = dynamic_cast<PropertyGrid*>(Widget::create("PropertyGrid",d));

		addChild(newPropertyGrid);

		return newPropertyGrid;
	}

	RadioButton* Panel::createRadioButton(RadioButtonDesc* d)
	{
		RadioButton* newRadioButton = dynamic_cast<RadioButton*>(Widget::create("RadioButton",d));

		addChild(newRadioButton);

		return newRadioButton;
	}

	TabControl* Panel::createTabControl(TabControlDesc* d)
	{
		TabControl* newTabControl = dynamic_cast<TabControl*>(Widget::create("TabControl",d));

		addChild(newTabControl);

		return newTabControl;
	}

	TextArea* Panel::createTextArea(TextAreaDesc* d)
	{
		TextArea* newTextArea = dynamic_cast<TextArea*>(Widget::create("TextArea",d));

		addChild(newTextArea);

		return newTextArea;
	}

	TextBox* Panel::createTextBox(TextBoxDesc* d)
	{
		TextBox* newTextBox = dynamic_cast<TextBox*>(Widget::create("TextBox",d));

		addChild(newTextBox);

		return newTextBox;
	}

	TreeView* Panel::createTreeView(TreeViewDesc* d)
	{
		TreeView* newTreeView = dynamic_cast<TreeView*>(Widget::create("TreeView",d));

		addChild(newTreeView);

		return newTreeView;
	}

	VScrollBar* Panel::createVScrollBar(VScrollBarDesc* d)
	{
		VScrollBar* newVScrollBar = dynamic_cast<VScrollBar*>(Widget::create("VScrollBar",d));

		addChild(newVScrollBar);

		return newVScrollBar;
	}

	Ogre::String Panel::getClass()
	{
		return "Panel";
	}

	void Panel::onDraw()
	{
		SkinType* st = mSkinType;
		if(!mWidgetDesc->widget_enabled && mWidgetDesc->widget_disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->widget_disabledSkinType);

		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mWidgetDesc->widget_brushFilterMode);
			
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->widget_dimensions.size),st->getSkinElement(mSkinElementName));
	}
}
