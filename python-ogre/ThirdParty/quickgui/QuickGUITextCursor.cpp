#include "QuickGUITextCursor.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	TextCursor::TextCursor(const Ogre::String& name, const Ogre::Vector4& dimensions, const Ogre::String& material, Ogre::OverlayContainer* overlayContainer, Widget* ParentWidget) :
		Widget(name,dimensions,material,overlayContainer,ParentWidget)
	{
		mWidgetType = Widget::QGUI_TYPE_TEXTCURSOR;

		Ogre::Real pixelWidth = QuickGUI::GUIManager::getSingleton().getRenderWindowWidth() * 0.01;
		mPixelDimensions.z = Ogre::Math::Ceil(pixelWidth);

		mPanelOverlayElement = createPanelOverlayElement(mInstanceName+"_Background",mPixelDimensions,mWidgetMaterial);
		mOverlayContainer->addChild(mPanelOverlayElement);
		mPanelOverlayElement->show();
	}

	TextCursor::~TextCursor()
	{
		Ogre::OverlayManager* om = Ogre::OverlayManager::getSingletonPtr();

		mOverlayContainer->removeChild(mPanelOverlayElement->getName());
		om->destroyOverlayElement(mPanelOverlayElement);
	}

	void TextCursor::_applyDimensions()
	{
		Ogre::Real pixelWidth = QuickGUI::GUIManager::getSingleton().getRenderWindowWidth() * 0.01;
		mPixelDimensions.z = Ogre::Math::Ceil(pixelWidth);

		mPanelOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
		mPanelOverlayElement->setDimensions(mPixelDimensions.z,mPixelDimensions.w);
	}

	void TextCursor::_notifyDimensionsChanged()
	{
		Widget::_notifyDimensionsChanged();

		_updateDimensions(mRelativeDimensions);
		_applyDimensions();
	}

	void TextCursor::hide()
	{
		mPanelOverlayElement->hide();
		Widget::hide();
	}

	void TextCursor::setPixelPosition(const Ogre::Real& xPixelPos, const Ogre::Real& yPixelPos)
	{
		mPixelDimensions.x = xPixelPos;
		mPixelDimensions.y = yPixelPos;
		mPanelOverlayElement->setPosition(mPixelDimensions.x,mPixelDimensions.y);
	}

	void TextCursor::setTiling(const Ogre::Real& horizontalRepetitions, const Ogre::Real& verticalRepetitions, Ogre::ushort layer)
	{
		mPanelOverlayElement->setTiling(horizontalRepetitions,verticalRepetitions,layer);
	}

	void TextCursor::show()
	{
		mPanelOverlayElement->show();
		Widget::show();
	}

	void TextCursor::toggleVisibility()
	{
		if(mVisible) mPanelOverlayElement->hide();
		else mPanelOverlayElement->show();

		mVisible = !mVisible;
	}
}
