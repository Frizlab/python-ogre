#include "QuickGUIButton.h"
#include "QuickGUIManager.h"
#include "OgreFontManager.h"
#include "OgreFont.h"

namespace QuickGUI
{
	Button::Button(const Ogre::String& name, Type type, const Rect& dimensions, GuiMetricsMode pMode, GuiMetricsMode sMode, Ogre::String texture, QuadContainer* container, Widget* ParentWidget, GUIManager* gm) :
		Label(name,type,dimensions,pMode,sMode,texture,container,ParentWidget,gm),
		mButtonDown(false)
	{
		// Other widgets call this constructor, and they handle quad/quadcontainer their own way.
		if(mWidgetType == TYPE_BUTTON)
		{
			mQuad->setLayer(Quad::LAYER_CHILD);
		}

		mButtonDownTexture = mTextureName + ".down" + mTextureExtension;
		mButtonOverTexture = mTextureName + ".over" + mTextureExtension;

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
		setTexture(mButtonDownTexture,false);
	}

	void Button::applyButtonOverTexture()
	{
		// apply button ".over" texture
		setTexture(mButtonOverTexture,false);
	}

	void Button::applyDefaultTexture()
	{
		setTexture(mFullTextureName,false);
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

	void Button::setBaseTexture(const Ogre::String& textureName)
	{
		Label::setBaseTexture(textureName);
		mButtonDownTexture = mTextureName + ".down" + mTextureExtension;
		mButtonOverTexture = mTextureName + ".over" + mTextureExtension;
	}

	void Button::setButtonDownTexture(const Ogre::String& textureName)
	{
		mButtonDownTexture = textureName;
	}

	void Button::setButtonOverTexture(const Ogre::String& textureName)
	{
		mButtonOverTexture = textureName;
	}
}
