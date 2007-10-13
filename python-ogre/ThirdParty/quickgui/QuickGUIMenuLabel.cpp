#include "QuickGUIMenuLabel.h"

namespace QuickGUI
{
	MenuLabel::MenuLabel(const Ogre::String& instanceName, const Size& pixelSize, Ogre::String textureName, GUIManager* gm) :
		Label(instanceName,pixelSize,textureName,gm),
		mIcon(0),
		mIconTextureName(""),
		mButton(0),
		mButtonTextureName("")
	{
		mWidgetType = TYPE_MENULABEL;
		mTextBoundsPixelOffset.x = mSize.height;
		mTextBoundsRelativeSize.width = (mSize.width - (mSize.height * 2.0)) / mSize.width;

		setPropogateEventFiring(EVENT_MOUSE_BUTTON_UP,true);
	}

	MenuLabel::~MenuLabel()
	{
	}

	Ogre::String MenuLabel::getButtonTexture()
	{
		if(mButton == NULL)
			return "";

		return mButton->getTextureName();
	}

	Ogre::String MenuLabel::getIconTexture()
	{
		if(mIcon == NULL)
			return "";

		return mIcon->getTextureName();
	}

	void MenuLabel::setMouseButtonUpOnButtonHandler(MemberFunctionSlot* function)
	{
		if(mButton == NULL)
			return;

		mButton->addEventHandler(EVENT_MOUSE_BUTTON_UP,function);
	}

	void MenuLabel::setButtonTexture(const Ogre::String& textureName)
	{
		mButtonTextureName = textureName;

		if(mButton != NULL)
		{
			mButton->setTexture(mButtonTextureName);
			return;
		}

		Point buttonPosition;
		HorizontalAnchor hAnchor;

		if(mLayout == LAYOUT_LEFT_TO_RIGHT)
		{
			buttonPosition = Point(mSize.width - mSize.height,0);
			hAnchor = ANCHOR_HORIZONTAL_LEFT;
		}
		else
		{
			buttonPosition = Point(0,0);
			hAnchor = ANCHOR_HORIZONTAL_RIGHT;
		}

		mButton = new Button(mInstanceName+".Icon",Size(mSize.height,mSize.height),mButtonTextureName,mGUIManager);
		addChild(mButton);
		mButton->setPosition(buttonPosition);
		mButton->setHorizontalAnchor(hAnchor);
		mButton->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		mButton->setPropogateEventFiring(EVENT_MOUSE_BUTTON_UP,true);
	}

	void MenuLabel::setIconTexture(const Ogre::String& textureName)
	{
		mIconTextureName = textureName;

		if(mIcon != NULL)
		{
			mIcon->setTexture(mIconTextureName);
			return;
		}

		Point iconPosition;
		HorizontalAnchor hAnchor;

		if(mLayout == LAYOUT_LEFT_TO_RIGHT)
		{
			iconPosition = Point(0,0);
			hAnchor = ANCHOR_HORIZONTAL_RIGHT;
		}
		else
		{
			iconPosition = Point(mSize.width - mSize.height,0);
			hAnchor = ANCHOR_HORIZONTAL_LEFT;
		}

		mIcon = new Image(mInstanceName+".Icon",Size(mSize.height,mSize.height),mIconTextureName,mGUIManager);
		addChild(mIcon);
		mIcon->setPosition(iconPosition);
		mIcon->setHorizontalAnchor(hAnchor);
		mIcon->setVerticalAnchor(ANCHOR_VERTICAL_TOP_BOTTOM);
		mIcon->setPropogateEventFiring(EVENT_MOUSE_BUTTON_UP,true);
	}

	void MenuLabel::onSizeChanged(const EventArgs& args)
	{
		Label::onSizeChanged(args);
		mTextBoundsPixelOffset.x = mSize.height;
		mTextBoundsRelativeSize.width = (mSize.width - (mSize.height * 2.0)) / mSize.width;
	}
}
