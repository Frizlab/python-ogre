#include "QuickGUIPrecompiledHeaders.h"

#include "QuickGUIMouseCursor.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinSetManager.h"

namespace QuickGUI
{
	MouseCursor::MouseCursor(const Size& size, const std::string& skinName, GUIManager* gm) :
		mGUIManager(gm),
		mVisible(true),
		mHideWhenOffScreen(true),
		mHideSkin(false),
		mOnTopBorder(0),
		mOnBotBorder(0),
		mOnLeftBorder(0),
		mOnRightBorder(0),
		mCursorState(CURSOR_STATE_NORMAL),
		mSkinComponent(".cursor")
	{
		mQuad = new Quad(mGUIManager);
		mQuad->setClipMode(Quad::CLIPMODE_NONE);
		setSize(size.width,size.height);
		setSkin(skinName);
		mRenderObjectList.push_back(mQuad);

		mVertexBuffer = new VertexBuffer(VERTICES_PER_QUAD, mGUIManager);
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

		if (mPixelPosition.x >= (mGUIManager->getViewportWidth() - 1 - (mPixelSize.width / 2)))
		{
			mPixelPosition.x = mGUIManager->getViewportWidth() - 1 - (mPixelSize.width / 2);
			mOnRightBorder = true;
			offScreen = true;
		}

		if (mPixelPosition.y >= (mGUIManager->getViewportHeight() - 1 - (mPixelSize.height / 2)))
		{
			mPixelPosition.y = mGUIManager->getViewportHeight() - 1 - (mPixelSize.height / 2);
			mOnBotBorder = true;
			offScreen = true;
		}

		if (mPixelPosition.y <= (-mPixelSize.height / 2))
		{
			mPixelPosition.y = (-mPixelSize.height / 2);
			mOnTopBorder = true;
			offScreen = true;
		}

		if (mPixelPosition.x <= (-mPixelSize.width / 2))
		{
			mPixelPosition.x = (-mPixelSize.width / 2);
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
		Point p = mPixelPosition;
		mQuad->setPosition(p);
	}

	MouseCursor::CursorState MouseCursor::getCursorState()
	{
		return mCursorState;
	}

	bool MouseCursor::getHideWhenOffScreen()
	{
		return mHideWhenOffScreen;
	}

	std::string MouseCursor::getTexture()
	{
		return mTextureName;
	}

	Point MouseCursor::getPosition()
	{
		return Point(mPixelPosition.x + (mPixelSize.width/2.0),mPixelPosition.y + (mPixelSize.height/2.0));
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

	void MouseCursor::hideSkin()
	{
		mHideSkin = true;
		mQuad->setMaterial("");
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

	void MouseCursor::setCursorState(CursorState s)
	{
		SkinSet* ss = SkinSetManager::getSingleton().getSkinSet(mSkinName);
		mCursorState = s;

		switch(s)
		{
		case CURSOR_STATE_NORMAL:					mSkinComponent = ".cursor";							break;
		case CURSOR_STATE_TEXTSELECT:				mSkinComponent = ".cursor.textselect";				break;
		case CURSOR_STATE_RESIZE_DIAGONAL_1:		mSkinComponent = ".cursor.resize.diagonal1";		break;
		case CURSOR_STATE_RESIZE_DIAGONAL_2:		mSkinComponent = ".cursor.resize.diagonal2";		break;
		case CURSOR_STATE_RESIZE_HORIZONTAL:		mSkinComponent = ".cursor.resize.leftright";		break;
		case CURSOR_STATE_RESIZE_VERTICAL:			mSkinComponent = ".cursor.resize.updown";			break;
		}

		std::string textureName = ss->getSkinName() + mSkinComponent + ss->getImageExtension();
		mQuad->setMaterial(ss->getMaterialName());
		mQuad->setTextureCoordinates(ss->getTextureCoordinates(textureName));

		setSize(ss->getImageWidth(textureName),ss->getImageHeight(textureName));
	}

	void MouseCursor::setHideCursorWhenOSCursorOffscreen(bool hide)
	{
		mHideWhenOffScreen = hide;
	}

	void MouseCursor::setPosition(float pixelX, float pixelY)
	{
		mPixelPosition.x = pixelX - (mPixelSize.width/2.0);
		mPixelPosition.y = pixelY - (mPixelSize.height/2.0);

		constrainPosition();
	}

	void MouseCursor::setSize(float pixelWidth, float pixelHeight)
	{
		Point currentPosition = getPosition();

		mPixelSize.width = pixelWidth;
		mPixelSize.height = pixelHeight;

		mQuad->setSize(mPixelSize);

		mPixelPosition.x = currentPosition.x - (mPixelSize.width/2.0);
		mPixelPosition.y = currentPosition.y - (mPixelSize.height/2.0);

		mQuad->setPosition(mPixelPosition);
	}

	void MouseCursor::setSkin(const std::string& skinName)
	{
		SkinSet* ss = SkinSetManager::getSingleton().getSkinSet(skinName);
		if(ss == NULL)
			throw Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND,"Skin \"" + skinName + "\" was not found!","MouseCursor::setSkin");

		mSkinName = skinName;

		std::string textureName = mSkinName + mSkinComponent + ss->getImageExtension();
		mQuad->setMaterial(ss->getMaterialName());
		mQuad->setTextureCoordinates(ss->getTextureCoordinates(textureName));

		setSize(ss->getImageWidth(textureName),ss->getImageHeight(textureName));

		if(mHideSkin)
			hideSkin();
	}

	void MouseCursor::show()
	{
		mQuad->setVisible(true);
		mVisible = true;
	}

	void MouseCursor::showSkin()
	{
		mHideSkin = false;
		setSkin(mSkinName);
	}
}
