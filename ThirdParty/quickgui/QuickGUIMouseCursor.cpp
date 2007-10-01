#include "QuickGUIMouseCursor.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	MouseCursor::MouseCursor(const Size& size, const Ogre::String& textureName, GUIManager* gm) :
		mGUIManager(gm),
		mTextureName(textureName),
		mVisible(true),
		mHideWhenOffScreen(true),
		mOnTopBorder(0),
		mOnBotBorder(0),
		mOnLeftBorder(0),
		mOnRightBorder(0),
		mOriginOffset(Point::ZERO)
	{
		mQuad = new Quad("MouseCursor.Quad",gm);
		setSize(size.width,size.height);
		mQuad->setTexture(mTextureName);
		mRenderObjectList.push_back(mQuad);

		mVertexBuffer = new VertexBuffer(6,mGUIManager);
		mVertexBuffer->setData(&mRenderObjectList);
		mVertexBuffer->setUpdateBeforeRender(true);
	}
	
	MouseCursor::~MouseCursor()
	{
		mRenderObjectList.clear();
		delete mQuad;
		delete mVertexBuffer;
	}

	void MouseCursor::constrainPosition()
	{
		bool offScreen = false;
		mOnRightBorder = false;
		mOnBotBorder = false;
		mOnTopBorder = false;
		mOnLeftBorder = false;

		if (mPixelPosition.x >= (mGUIManager->getViewportWidth() - 1))
		{
			mPixelPosition.x = mGUIManager->getViewportWidth() - 1;
			mOnRightBorder = true;
			offScreen = true;
		}

		if (mPixelPosition.y >= (mGUIManager->getViewportHeight() - 1))
		{
			mPixelPosition.y = mGUIManager->getViewportHeight() - 1;
			mOnBotBorder = true;
			offScreen = true;
		}

		if (mPixelPosition.y <= 0)
		{
			mPixelPosition.y = 0;
			mOnTopBorder = true;
			offScreen = true;
		}

		if (mPixelPosition.x <= 0)
		{
			mPixelPosition.x = 0;
			mOnLeftBorder = true;
			offScreen = true;
		}

		if(offScreen) 
			mGUIManager->injectMouseLeaves();
		// For example, if the user wants the mouse hidden, we shouldn't show it
		// even if its within bounds.
		else 
		{
			if(mVisible) 
				show();
			else 
				hide();
		}

		// Perform the actual moving of the mouse quad
		mQuad->setPosition(mPixelPosition);
	}

	bool MouseCursor::getHideWhenOffScreen()
	{
		return mHideWhenOffScreen;
	}

	Point MouseCursor::getOriginOffset()
	{
		return mOriginOffset;
	}

	Point MouseCursor::getPosition()
	{
		return Point(mPixelPosition.x + mOriginOffset.x,mPixelPosition.y + mOriginOffset.y);
	}

	Size MouseCursor::getSize()
	{
		return mPixelSize;
	}

	void MouseCursor::hide()
	{
		mQuad->setVisible(false);
		mVisible = false;
	}

	void MouseCursor::_hide()
	{
		mQuad->setVisible(false);
	}

	bool MouseCursor::isVisible()
	{
		return mVisible;
	}

	bool MouseCursor::mouseOnBotBorder()
	{
		return mOnBotBorder;
	}

	bool MouseCursor::mouseOnLeftBorder()
	{
		return mOnLeftBorder;
	}

	bool MouseCursor::mouseOnRightBorder()
	{
		return mOnRightBorder;
	}

	bool MouseCursor::mouseOnTopBorder()
	{
		return mOnTopBorder;
	}

	void MouseCursor::offsetOrigin(int xPixelOffset, int yPixelOffset)
	{
		mOriginOffset.x = xPixelOffset;
		mOriginOffset.y = yPixelOffset;
	}

	void MouseCursor::offsetPosition(const int& xPixelOffset, const int& yPixelOffset)
	{
		mPixelPosition.x += xPixelOffset;
		mPixelPosition.y += yPixelOffset;
		
		constrainPosition();
	}

	void MouseCursor::render()
	{
		mVertexBuffer->render();
	}

	void MouseCursor::setTexture(const Ogre::String& textureName)
	{
		mTextureName = textureName;
		mQuad->setTexture(mTextureName);
	}

	void MouseCursor::setHideCursorWhenOSCursorOffscreen(bool hide)
	{
		mHideWhenOffScreen = hide;
	}

	void MouseCursor::setPosition(Ogre::Real pixelX, Ogre::Real pixelY)
	{
		mPixelPosition.x = pixelX;
		mPixelPosition.y = pixelY;

		constrainPosition();
	}

	void MouseCursor::setSize(Ogre::Real pixelWidth, Ogre::Real pixelHeight)
	{
		mPixelSize.width = pixelWidth;
		mPixelSize.height = pixelHeight;

		mQuad->setSize(mPixelSize);
	}

	void MouseCursor::show()
	{
		mQuad->setVisible(true);
		mVisible = true;
	}
}
