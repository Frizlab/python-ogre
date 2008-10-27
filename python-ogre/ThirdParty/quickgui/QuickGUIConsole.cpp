#include "QuickGUIConsole.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String Console::BACKGROUND = "background";
	const Ogre::String Console::TEXTBOX = "textbox";
	const Ogre::String Console::TEXTAREA = "textarea";

	void Console::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("Console");
		d->defineSkinElement(BACKGROUND);
		d->defineComponent(TEXTBOX);
		d->defineComponent(TEXTAREA);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("Console",d);
	}

	ConsoleDesc::ConsoleDesc() :
		ComponentWidgetDesc()
	{
	}

	void ConsoleDesc::serialize(SerialBase* b)
	{
	}

	Console::Console(const Ogre::String& name) :
		ComponentWidget(name)
	{
		mSkinElementName = BACKGROUND;
	}

	Console::~Console()
	{
	}

	void Console::_initialize(WidgetDesc* d)
	{
		ComponentWidget::_initialize(d);
	}

	Widget* Console::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new Console(widgetName);

		newWidget->_createDescObject("ConsoleDesc");

		return newWidget;
	}

	Ogre::String Console::getClass()
	{
		return "Console";
	}

	void Console::onDraw()
	{
	}
}
