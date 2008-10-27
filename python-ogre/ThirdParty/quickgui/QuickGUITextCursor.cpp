#include "QuickGUITextCursor.h"
#include "QuickGUISkinDefinitionManager.h"

namespace QuickGUI
{
	const Ogre::String TextCursor::BACKGROUND = "background";

	void TextCursor::registerSkinDefinition()
	{
		SkinDefinition* d = new SkinDefinition("TextCursor");
		d->defineSkinElement(BACKGROUND);
		d->definitionComplete();

		SkinDefinitionManager::getSingleton().registerSkinDefinition("TextCursor",d);
	}

	TextCursor::TextCursor() :
		mSkinElement(NULL),
		mSkinType(NULL)
	{
		mVisible = false;

		setSkinType("default");
	}

	TextCursor::~TextCursor()
	{
	}

	Point TextCursor::getPosition()
	{
		return mDimensions.position;
	}

	Size TextCursor::getSize()
	{
		return mDimensions.size;
	}

	bool TextCursor::getVisible()
	{
		return mVisible;
	}

	void TextCursor::onDraw()
	{
		if(!mVisible)
			return;

		QuickGUI::Brush* brush = QuickGUI::Brush::getSingletonPtr();

		// Store Render Target
		Ogre::Viewport* currentRenderTarget = brush->getRenderTarget();

		// Set Render Target to Viewport
		brush->setRenderTarget(NULL);
		// Draw SkinElement
		brush->drawSkinElement(mDimensions,mSkinElement);

		// Restore Render Target
		brush->setRenderTarget(currentRenderTarget);
	}

	void TextCursor::setHeight(float heightInPixels)
	{
		float percentage = heightInPixels / mDimensions.size.height;
		mDimensions.size *= percentage;
	}

	void TextCursor::setPosition(Point& p)
	{
		mDimensions.position.x = p.x - (mDimensions.size.width / 2.0);
		mDimensions.position.y = p.y;
	}

	void TextCursor::setSkinType(const Ogre::String type)
	{
		mSkinType = SkinTypeManager::getSingleton().getSkinType("TextCursor",type);
		mSkinElement = mSkinType->getSkinElement(BACKGROUND);

		mDimensions.size.width = mSkinElement->getWidth();
		mDimensions.size.height = mSkinElement->getHeight();
	}

	void TextCursor::setVisible(bool visible)
	{
		mVisible = visible;
	}
}
