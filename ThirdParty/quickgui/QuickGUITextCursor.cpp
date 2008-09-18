#include "QuickGUITextCursor.h"

namespace QuickGUI
{
	TextCursor::TextCursor()
	{
		mVisible = false;
		mSize = Size(15,26);
	}

	TextCursor::~TextCursor()
	{
	}

	Point TextCursor::getPosition()
	{
		return mPosition;
	}

	Size TextCursor::getSize()
	{
		return mSize;
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

		Ogre::Viewport* currentRenderTarget = brush->getRenderTarget();

		brush->setRenderTarget(NULL);

		brush->setTexture("qgui.cursor.textselect.png");
		brush->drawRectangle(Rect(mPosition,Size(15,26)),UVRect(0,0,1,1));

		brush->setRenderTarget(currentRenderTarget);
	}

	void TextCursor::setPosition(Point& p)
	{
		mPosition.x = p.x - (mSize.width / 2.0);
		mPosition.y = p.y;
	}

	void TextCursor::setVisible(bool visible)
	{
		mVisible = visible;
	}
}
