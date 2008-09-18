#include "QuickGUIPanel.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String Panel::BACKGROUND = "background";

	void Panel::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("Panel");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(HSCROLLBAR);
		d->defineComponent(VSCROLLBAR);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Panel",d);
	}

	PanelDesc::PanelDesc() :
		ContainerWidgetDesc()
	{
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
			setSkinType(d->skinTypeName);
	}

	Widget* Panel::createCustomWidget(const Ogre::String& className, WidgetDesc& d)
	{
		if((className == "TitleBar") || (className == "Window") || (className == "Sheet"))
			throw Exception(Exception::ERR_UNSUPPORTED_WIDGET,"This widget does not support creation of widget's of class \"" + className + "\".","Panel::createCustomWidget");

		Widget* newCustomWidget = mPanelDesc->guiManager->createWidget(className,d);

		addChild(newCustomWidget);

		return newCustomWidget;
	}

	Button* Panel::createButton(ButtonDesc& d)
	{
		Button* newButton = dynamic_cast<Button*>(mPanelDesc->guiManager->createWidget("Button",d));

		addChild(newButton);

		return newButton;
	}

	HScrollBar* Panel::createHScrollBar(HScrollBarDesc& d)
	{
		HScrollBar* newHScrollBar = dynamic_cast<HScrollBar*>(mPanelDesc->guiManager->createWidget("HScrollBar",d));

		addChild(newHScrollBar);

		return newHScrollBar;
	}

	Image* Panel::createImage(ImageDesc& d)
	{
		Image* newImage = dynamic_cast<Image*>(mPanelDesc->guiManager->createWidget("Image",d));

		addChild(newImage);

		return newImage;
	}

	Label* Panel::createLabel(LabelDesc& d)
	{
		Label* newLabel = dynamic_cast<Label*>(mPanelDesc->guiManager->createWidget("Label",d));

		addChild(newLabel);

		return newLabel;
	}

	List* Panel::createList(ListDesc& d)
	{
		List* newList = dynamic_cast<List*>(mPanelDesc->guiManager->createWidget("List",d));

		addChild(newList);

		return newList;
	}

	Panel* Panel::createPanel(PanelDesc& d)
	{
		Panel* newPanel = dynamic_cast<Panel*>(mPanelDesc->guiManager->createWidget("Panel",d));
		
		addChild(newPanel);

		return newPanel;
	}

	ProgressBar* Panel::createProgressBar(ProgressBarDesc& d)
	{
		ProgressBar* newProgressBar = dynamic_cast<ProgressBar*>(mPanelDesc->guiManager->createWidget("ProgressBar",d));

		addChild(newProgressBar);

		return newProgressBar;
	}

	TextArea* Panel::createTextArea(TextAreaDesc& d)
	{
		TextArea* newTextArea = dynamic_cast<TextArea*>(mPanelDesc->guiManager->createWidget("TextArea",d));

		addChild(newTextArea);

		return newTextArea;
	}

	TextBox* Panel::createTextBox(TextBoxDesc& d)
	{
		TextBox* newTextBox = dynamic_cast<TextBox*>(mPanelDesc->guiManager->createWidget("TextBox",d));

		addChild(newTextBox);

		return newTextBox;
	}

	VScrollBar* Panel::createVScrollBar(VScrollBarDesc& d)
	{
		VScrollBar* newVScrollBar = dynamic_cast<VScrollBar*>(mPanelDesc->guiManager->createWidget("VScrollBar",d));

		addChild(newVScrollBar);

		return newVScrollBar;
	}

	Widget* Panel::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new Panel(widgetName);

		newWidget->_createDescObject("PanelDesc");

		return newWidget;
	}

	Ogre::String Panel::getClass()
	{
		return "Panel";
	}

	void Panel::onDraw()
	{
		SkinType* st = mSkinType;
		if(!mWidgetDesc->enabled && mWidgetDesc->disabledSkinType != "")
			st = SkinTypeManager::getSingleton().getSkinType(getClass(),mWidgetDesc->disabledSkinType);

		Brush* brush = Brush::getSingletonPtr();

		brush->setFilterMode(mWidgetDesc->brushFilterMode);
			
		brush->drawSkinElement(Rect(mTexturePosition,mWidgetDesc->dimensions.size),st->getSkinElement(mSkinElementName));
	}
}
