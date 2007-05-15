#include "SimpleGUIButton.h"
#include "SimpleGUIManager.h"
#include "OgreFontManager.h"
#include "OgreFont.h"

namespace SimpleGUI
{
	Button::Button(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, bool show, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Label(name,dimensions,material,show,overlayContainer,ParentWidget)
	{
		mWidgetType = Widget::SGUI_BUTTON;
		setCharacterHeight(0.5);

		Ogre::MaterialManager* mm = Ogre::MaterialManager::getSingletonPtr();

		mOverMaterialExists = mm->resourceExists(mMaterial+".over");
		mDownMaterialExists = mm->resourceExists(mMaterial+".down");
	}

	Button::~Button()
	{
	}

	void Button::applyButtonDownMaterial()
	{
		if(mDownMaterialExists) 
		{
			// apply button ".down" material
			mPanelOverlayElement->setMaterialName(mMaterial+".down");
		}
	}

	void Button::applyDefaultMaterial()
	{
		mPanelOverlayElement->setMaterialName(mMaterial);
	}

	bool Button::onMouseButtonDown(MouseEventArgs& e) 
	{
		applyButtonDownMaterial();

		return Label::onMouseButtonDown(e);
	}

	bool Button::onMouseButtonUp(MouseEventArgs& e) 
	{ 
		if(mOverMaterialExists) mPanelOverlayElement->setMaterialName(mMaterial+".over");

		return Label::onMouseButtonUp(e);
	}

	bool Button::onMouseEnters(MouseEventArgs& e) 
	{ 
		if(mOverMaterialExists) mPanelOverlayElement->setMaterialName(mMaterial+".over");

		return Label::onMouseEnters(e);
	}

	bool Button::onMouseLeaves(MouseEventArgs& e) 
	{ 
		mPanelOverlayElement->setMaterialName(mMaterial);

		return Label::onMouseLeaves(e);
	}
}
