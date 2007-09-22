#include "QuickGUIMouseCursor.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	MouseCursor::MouseCursor(const Size& size, GuiMetricsMode sizeMode, const Ogre::String& textureName, GUIManager* gm) :
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
		setSize(size.width,size.height,sizeMode);
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
			if(mVisible) show();
			else hide();
		}

		// Perform the actual moving of the mouse quad
		mAbsolutePosition.x = mPixelPosition.x / mGUIManager->getViewportWidth();
		mAbsolutePosition.y = mPixelPosition.y / mGUIManager->getViewportHeight();
		mQuad->setPosition(mAbsolutePosition);
	}

	bool MouseCursor::getHideWhenOffScreen()
	{
		return mHideWhenOffScreen;
	}

	Point MouseCursor::getOriginOffset(GuiMetricsMode mode)
	{
		switch(mode)
		{
			case QGUI_GMM_ABSOLUTE:
			case QGUI_GMM_RELATIVE:
				return Point(mOriginOffset.x / mGUIManager->getViewportWidth(),mOriginOffset.y / mGUIManager->getViewportHeight());
			case QGUI_GMM_PIXELS:
			default:
				return mOriginOffset;
		}
		return mOriginOffset;
	}

	Point MouseCursor::getPosition(GuiMetricsMode mode)
	{
		switch(mode)
		{
			case QGUI_GMM_ABSOLUTE:
			case QGUI_GMM_RELATIVE:
				return Point(mAbsolutePosition.x + (mOriginOffset.x / mGUIManager->getViewportWidth()),mAbsolutePosition.y + (mOriginOffset.y / mGUIManager->getViewportHeight()));
			case QGUI_GMM_PIXELS:
			default:
				return Point(mPixelPosition.x + mOriginOffset.x,mPixelPosition.y + mOriginOffset.y);
		}
	}

	Size MouseCursor::getSize(GuiMetricsMode mode)
	{
		switch(mode)
		{
			case QGUI_GMM_ABSOLUTE:
			case QGUI_GMM_RELATIVE:
				return mAbsoluteSize;
			case QGUI_GMM_PIXELS:
			default:
				return mPixelSize;
		}
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

	void MouseCursor::offsetOrigin(int xOffset, int yOffset, GuiMetricsMode mode)
	{
		switch(mode)
		{
			case QGUI_GMM_ABSOLUTE:
			case QGUI_GMM_RELATIVE:
				mOriginOffset.x = xOffset * mGUIManager->getViewportWidth();
				mOriginOffset.y = yOffset * mGUIManager->getViewportHeight();
				break;
			case QGUI_GMM_PIXELS:
				mOriginOffset.x = xOffset;
				mOriginOffset.y = yOffset;
				break;
		}
	}

	void MouseCursor::offsetPosition(const int& xOffset, const int& yOffset, GuiMetricsMode mode)
	{
		switch(mode)
		{
			case QGUI_GMM_ABSOLUTE:
			case QGUI_GMM_RELATIVE:
				mPixelPosition.x += xOffset * mGUIManager->getViewportWidth();
				mPixelPosition.y += yOffset * mGUIManager->getViewportHeight();
				break;
			case QGUI_GMM_PIXELS:
				mPixelPosition.x += xOffset;
				mPixelPosition.y += yOffset;
				break;
		}

		mAbsolutePosition.x = mPixelPosition.x / mGUIManager->getViewportWidth();
		mAbsolutePosition.y = mPixelPosition.y / mGUIManager->getViewportHeight();
		
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

	void MouseCursor::setPosition(Ogre::Real xPosition, Ogre::Real yPosition, GuiMetricsMode mode)
	{
		switch(mode)
		{
			case QGUI_GMM_ABSOLUTE:
			case QGUI_GMM_RELATIVE:
				mPixelPosition.x = xPosition * mGUIManager->getViewportWidth();
				mPixelPosition.y = yPosition * mGUIManager->getViewportHeight();
				break;
			case QGUI_GMM_PIXELS:
				mPixelPosition.x = xPosition;
				mPixelPosition.y = yPosition;
				break;
		}

		mAbsolutePosition.x = mPixelPosition.x / mGUIManager->getViewportWidth();
		mAbsolutePosition.y = mPixelPosition.y / mGUIManager->getViewportHeight();

		constrainPosition();
	}

	void MouseCursor::setSize(Ogre::Real width, Ogre::Real height, GuiMetricsMode mode)
	{
		switch(mode)
		{
			case QGUI_GMM_ABSOLUTE:
			case QGUI_GMM_RELATIVE:
				mPixelSize.width = width * mGUIManager->getViewportWidth();
				mPixelSize.height = height * mGUIManager->getViewportHeight();
				break;
			case QGUI_GMM_PIXELS:
				mPixelSize.width = width;
				mPixelSize.height = height;
				break;
		}

		mAbsoluteSize.width = mPixelSize.width / static_cast<float>(mGUIManager->getViewportWidth());
		mAbsoluteSize.height = mPixelSize.height / static_cast<float>(mGUIManager->getViewportHeight());
		mQuad->setSize(mAbsoluteSize);
	}

	void MouseCursor::show()
	{
		mQuad->setVisible(true);
		mVisible = true;
	}
}
