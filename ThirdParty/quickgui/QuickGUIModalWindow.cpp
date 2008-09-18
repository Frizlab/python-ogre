#include "QuickGUIModalWindow.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String ModalWindow::BACKGROUND = "background";

	void ModalWindow::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("ModalWindow");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ModalWindow",d);
	}

	ModalWindowDesc::ModalWindowDesc() :
		WindowDesc()
	{
	}

	ModalWindow::ModalWindow(const Ogre::String& name) :
		Window(name)
	{
		mSkinElementName = BACKGROUND;
	}

	ModalWindow::~ModalWindow()
	{
	}

	void ModalWindow::_initialize(WidgetDesc* d)
	{
		Window::_initialize(d);

		setSkinType(d->skinTypeName);
	}

	Widget* ModalWindow::factory(const Ogre::String& widgetName)
	{
		Widget* newWidget = new ModalWindow(widgetName);

		newWidget->_createDescObject("ModalWindowDesc");

		return newWidget;
	}

	Ogre::String ModalWindow::getClass()
	{
		return "ModalWindow";
	}
}
