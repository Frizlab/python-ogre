#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUISkinSetManager.h"

#include "QuickGUIButton.h"
#include "QuickGUIManager.h"
#include "OgreFontManager.h"
#include "OgreFont.h"

namespace QuickGUI
{
	Button::Button(const std::string& name, GUIManager* gm) :
		Label(name,gm),
		mButtonDown(false),
		mDefaultSkinComponent("")
	{
		mWidgetType = TYPE_BUTTON;
		mSkinComponent = ".button";
		mSize = Size(75,25);

		addEventHandler(EVENT_MOUSE_ENTER,&Button::onMouseEnters,this);
		addEventHandler(EVENT_MOUSE_LEAVE,&Button::onMouseLeaves,this);
		addEventHandler(EVENT_MOUSE_BUTTON_DOWN,&Button::onMouseButtonDown,this);
		addEventHandler(EVENT_MOUSE_BUTTON_UP,&Button::onMouseButtonUp,this);
	}

	Button::~Button()
	{
	}

	void Button::applyButtonDownTexture()
	{
		// apply button ".down" texture
		mQuad->setTextureCoordinates(mSkinSet->getTextureCoordinates(mSkinName + mSkinComponent + ".down" + mSkinSet->getImageExtension()));

		for(std::vector<Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if((*it)->getWidgetType() == TYPE_BORDER)
			{
				dynamic_cast<Border*>(*it)->_notifyParentSkinComponent(mSkinComponent + ".down");
			}
		}
	}

	void Button::applyButtonOverTexture()
	{
		// apply button ".over" texture
		mQuad->setTextureCoordinates(mSkinSet->getTextureCoordinates(mSkinName + mSkinComponent + ".over" + mSkinSet->getImageExtension()));

		for(std::vector<Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if((*it)->getWidgetType() == TYPE_BORDER)
			{
				dynamic_cast<Border*>(*it)->_notifyParentSkinComponent(mSkinComponent + ".over");
			}
		}
	}

	void Button::applyDefaultTexture()
	{
		mQuad->setTextureCoordinates(mSkinSet->getTextureCoordinates(mSkinName + mSkinComponent + mSkinSet->getImageExtension()));

		for(std::vector<Widget*>::iterator it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if((*it)->getWidgetType() == TYPE_BORDER)
			{
				dynamic_cast<Border*>(*it)->_notifyParentSkinComponent(mSkinComponent);
			}
		}
	}

	bool Button::isDown()
	{
		return mButtonDown;
	}

	void Button::onMouseButtonDown(const EventArgs& args) 
	{
		if(dynamic_cast<const MouseEventArgs&>(args).button == MB_Left)
		{
			applyButtonDownTexture();
			mButtonDown = true;
		}
	}

	void Button::onMouseButtonUp(const EventArgs& args) 
	{ 
		if(dynamic_cast<const MouseEventArgs&>(args).button == MB_Left)
		{
			applyButtonOverTexture();
			mButtonDown = false;
		}
	}

	void Button::onMouseEnters(const EventArgs& args) 
	{ 
		if(mGrabbed) 
		{
			applyButtonDownTexture();
			mButtonDown = true;
		}
		else 
			applyButtonOverTexture();
	}

	void Button::onMouseLeaves(const EventArgs& args) 
	{ 
		applyDefaultTexture();
		mButtonDown = false;
	}

	void Button::setSkinComponent(const std::string& skinComponent)
	{
		Widget::setSkinComponent(skinComponent);
		mDefaultSkinComponent = skinComponent;
	}
}
