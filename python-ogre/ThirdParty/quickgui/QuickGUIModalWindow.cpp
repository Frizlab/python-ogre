#include "QuickGUIModalWindow.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String ModalWindow::BACKGROUND = "background";

	void ModalWindow::registerSkinDefinition()
	{
		SkinDefinition* d = OGRE_NEW_T(SkinDefinition,Ogre::MEMCATEGORY_GENERAL)("ModalWindow");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("ModalWindow",d);
	}

	ModalWindowDesc::ModalWindowDesc(const Ogre::String& id) :
		WindowDesc(id)
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

		setSkinType(d->widget_skinTypeName);
	}

	Ogre::String ModalWindow::getClass()
	{
		return "ModalWindow";
	}
}
