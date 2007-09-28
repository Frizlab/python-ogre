#include "QuickGUIQuad.h"
#include "QuickGUIQuadContainer.h"
#include "QuickGUIManager.h"
// for min/max
#include <algorithm>

namespace QuickGUI
{
	Quad::Quad(const Ogre::String& id, GUIManager* gm) :
		mID(id),
		mGUIManager(gm),
		mQuadContainer(NULL),
		mLayer(LAYER_CHILD),
		mColorChanged(false),
		mDimensionsChanged(false),
		mTextureChanged(false),
		mTextureCoordsChanged(false),
		mOffsetChanged(false),
		mAddedToRenderGroup(false),
		mTextureName(""),
		mOffset(0),
		mPixelDimensions(Rect::ZERO),
		mTextureCoordinates(Ogre::Vector4(0,0,1,1)),
		mVisible(true),
		mTopColor(Ogre::ColourValue::White),
		mBottomColor(Ogre::ColourValue::White),
		mHiddenViaClipping(false),
		mClippingRect(Rect(0,0,gm->getViewportWidth(),gm->getViewportHeight()))
	{
		mRenderSystem = Ogre::Root::getSingleton().getRenderSystem();
		mVertices.resize(6);
		_updateVertexColor();
	}

	Quad::~Quad()
	{
		if(mAddedToRenderGroup) 
			removeFromRenderObjectGroup();
	}

	void Quad::_clip()
	{
		if(insideRect(mClippingRect))
		{
			mDimensionsViaClipping = mPixelDimensions;
			_computeVertices();

			mTextureCoordinatesViaClipping = mTextureCoordinates;
			_updateTextureCoords();

			if(mHiddenViaClipping)
			{
				setVisible(true);
				mHiddenViaClipping = false;
			}
		}
		else if(intersectsRect(mClippingRect))
		{
			mDimensionsViaClipping = getRectIntersection(mClippingRect);
			_computeVertices();

			// calculate distance between top/bottom and left/right of the UV coords.
			Ogre::Real uvWidth = mTextureCoordinates.z - mTextureCoordinates.x;
			Ogre::Real uvHeight = mTextureCoordinates.w - mTextureCoordinates.y;
			
			mTextureCoordinatesViaClipping.x = mTextureCoordinates.x + (((mDimensionsViaClipping.x - mPixelDimensions.x) / mPixelDimensions.width) * uvWidth);
			mTextureCoordinatesViaClipping.y = mTextureCoordinates.y + (((mDimensionsViaClipping.y - mPixelDimensions.y) / mPixelDimensions.height) * uvHeight);
			mTextureCoordinatesViaClipping.z = mTextureCoordinates.z - ((((mPixelDimensions.x + mPixelDimensions.width) - (mDimensionsViaClipping.x + mDimensionsViaClipping.width)) / mPixelDimensions.width) * uvWidth);
			mTextureCoordinatesViaClipping.w = mTextureCoordinates.w - ((((mPixelDimensions.y + mPixelDimensions.height) - (mDimensionsViaClipping.y + mDimensionsViaClipping.height)) / mPixelDimensions.height) * uvHeight);
			
			_updateTextureCoords();

			if(mHiddenViaClipping)
			{
				setVisible(true);
				mHiddenViaClipping = false;
			}
		}
		else // Quad is outside clipping region.
		{
			mDimensionsViaClipping = mPixelDimensions;
			_computeVertices();

			mTextureCoordinatesViaClipping = mTextureCoordinates;
			_updateTextureCoords();

			if(mVisible)
			{
				mHiddenViaClipping = true;
				setVisible(false);
			}
		}
	}

	void Quad::_computeVertices()
	{
		// Convert pixel values into absolute coordinates.
		Ogre::Real viewportWidth = mGUIManager->getViewportWidth();
		Ogre::Real viewportHeight = mGUIManager->getViewportHeight();

		Rect absDimensions(
			mDimensionsViaClipping.x / viewportWidth,
			mDimensionsViaClipping.y / viewportHeight,
			mDimensionsViaClipping.width / viewportWidth,
			mDimensionsViaClipping.height / viewportHeight);

		/* Convert positions into -1, 1 coordinate space (homogenous clip space).
			- Left / right is simple range conversion
			- Top / bottom also need inverting since y is upside down - this means
			that top will end up greater than bottom and when computing texture
			coordinates, we have to flip the v-axis (ie. subtract the value from
			1.0 to get the actual correct value).
		*/
		Ogre::Real left = (absDimensions.x * 2) - 1;
		Ogre::Real right = left + (absDimensions.width * 2);
		Ogre::Real top = -((absDimensions.y * 2) - 1);
		Ogre::Real bottom = top - (absDimensions.height * 2);

		// TRIANGLE 1
		mVertices[0].pos = Ogre::Vector3(left,bottom,0);	// left-bottom
		mVertices[1].pos = Ogre::Vector3(right,bottom,0);	// right-bottom
		mVertices[2].pos = Ogre::Vector3(left,top,0);		// left-top

		// TRIANGLE 2
		mVertices[3].pos = Ogre::Vector3(right,bottom,0);	// right-bottom
		mVertices[4].pos = Ogre::Vector3(right,top,0);		// right-top
		mVertices[5].pos = Ogre::Vector3(left,top,0);		// left-top
	}

	void Quad::_notifyAddedToRenderObjectGroup()
	{
		mAddedToRenderGroup = true;
	}

	void Quad::_notifyChangesHandled()
	{
		mColorChanged = false;
		mDimensionsChanged = false;
		mTextureChanged = false;
		mOffsetChanged = false;
	}

	void Quad::_notifyRemovedFromQuadContainer()
	{
		mAddedToRenderGroup = false;
	}

	void Quad::_notifyQuadContainer(QuadContainer* container)
	{
		if( (mQuadContainer != NULL) && (mQuadContainer->getID() == container->getID()) ) 
			return;

		if(mAddedToRenderGroup && (mQuadContainer != NULL))
			removeFromRenderObjectGroup();

		mQuadContainer = container;

		if(mQuadContainer != NULL)
			addToRenderObjectGroup();
	}

	void Quad::_notifyQuadContainerNeedsUpdate()
	{
		// notify QuadContainer of change (if exists)
		if( (mQuadContainer != NULL) && (mAddedToRenderGroup) ) 
		{
			switch(mLayer)
			{
			case Quad::LAYER_CHILD:
				mQuadContainer->notifyChildRenderableChanged(this);	
				break;
			case Quad::LAYER_MENU:
				mQuadContainer->notifyMenuRenderableChanged(this);
				break;
			}
		}
	}

	void Quad::_setTexture(const Ogre::String& textureName)
	{
		mTextureName = textureName;
		mTextureChanged = true;

		_notifyQuadContainerNeedsUpdate();
	}

	void Quad::_updateTextureCoords()
	{
		Ogre::Vector4 actualTextureCoords = mTextureCoordinatesViaClipping;

		// TRIANGLE 1 : bot-left, bot-right, top-left vertices
		mVertices[0].uv = Ogre::Vector2(actualTextureCoords.x,actualTextureCoords.w);
		mVertices[1].uv = Ogre::Vector2(actualTextureCoords.z,actualTextureCoords.w);
		mVertices[2].uv = Ogre::Vector2(actualTextureCoords.x,actualTextureCoords.y);
		// TRIANGLE 2 : bot-right, top-right, top-left vertices
		mVertices[3].uv = Ogre::Vector2(actualTextureCoords.z,actualTextureCoords.w);
		mVertices[4].uv = Ogre::Vector2(actualTextureCoords.z,actualTextureCoords.y);
		mVertices[5].uv = Ogre::Vector2(actualTextureCoords.x,actualTextureCoords.y);
	}

	void Quad::_updateVertexColor()
	{
		// TRIANGLE 1
		mRenderSystem->convertColourValue(mBottomColor,&( mVertices[0].color ));
		mRenderSystem->convertColourValue(mBottomColor,&( mVertices[1].color ));
		mRenderSystem->convertColourValue(mTopColor,&( mVertices[2].color ));
		// TRIANGLE 2
		mRenderSystem->convertColourValue(mBottomColor,&( mVertices[3].color ));
		mRenderSystem->convertColourValue(mTopColor,&( mVertices[4].color ));
		mRenderSystem->convertColourValue(mTopColor,&( mVertices[5].color ));
	}

	void Quad::addToRenderObjectGroup()
	{
		if( mAddedToRenderGroup || (mQuadContainer == NULL) ) 
			return;

		switch(mLayer)
		{
		case Quad::LAYER_CHILD:
			mQuadContainer->addChildRenderable(this);
			break;
		case Quad::LAYER_MENU:
			mQuadContainer->addMenuRenderable(this);
			break;
		}

		mAddedToRenderGroup = true;
	}

	bool Quad::dimensionsChanged()
	{
		return mDimensionsChanged;
	}

	Rect Quad::getDimensions()
	{
		return mPixelDimensions;
	}

	Ogre::String Quad::getID()
	{
		return mID;
	}

	Quad::Layer Quad::getLayer()
	{
		return mLayer;
	}

	Rect Quad::getRectIntersection(const Rect& r)
	{
		Rect retVal = Rect::ZERO;

		if(intersectsRect(r))
		{
			retVal.x = std::max(mPixelDimensions.x,r.x);
			retVal.y = std::max(mPixelDimensions.y,r.y);
			retVal.width = std::min(mPixelDimensions.x + mPixelDimensions.width, r.x + r.width) - retVal.x;
			retVal.height = std::min(mPixelDimensions.y + mPixelDimensions.height, r.y + r.height) - retVal.y;
		}

		return retVal;
	}

	bool Quad::insideRect(const Rect& r)
	{
		if( (mPixelDimensions.x >= r.x) &&
			(mPixelDimensions.y >= r.y) &&
			((mPixelDimensions.x + mPixelDimensions.width) <= (r.x + r.width)) &&
			((mPixelDimensions.y + mPixelDimensions.height) <= (r.y + r.height)) )
			return true;

		return false;
	}

	bool Quad::intersectsRect(const Rect& r)
	{
		// if our left side is greater than r's right side, or our right side is less than r's left side, intersection is not possible.
		if( (mPixelDimensions.x > (r.x + r.width)) || ((mPixelDimensions.x + mPixelDimensions.width) < r.x) )
			return false;

		// if our top is greater than r's bottom, or our bottom is less than r's top, intersection is not possible.
		if( (mPixelDimensions.y > (r.y + r.height)) || ((mPixelDimensions.y + mPixelDimensions.height) < r.y) )
			return false;

		// If the above conditions are not met, than there must be overlap between our dimensions and r's dimensions.
		return true;
	}

	bool Quad::isPointWithinBounds(const Point& pixelPosition)
	{
		float xPos = pixelPosition.x;
		float yPos = pixelPosition.y;

		if( (xPos < mPixelDimensions.x) || (xPos > (mPixelDimensions.x + mPixelDimensions.width)) )
			return false;

		if( (yPos < mPixelDimensions.y) || (yPos > (mPixelDimensions.y + mPixelDimensions.height)) )
			return false;

		return true;
	}

	Ogre::String Quad::getTextureName()
	{
		return mTextureName;
	}

	std::vector<Vertex>* Quad::getVertices()
	{
		return &mVertices;
	}

	int Quad::getNumberOfVertices()
	{
		return static_cast<int>(mVertices.size());
	}

	int Quad::getOffset()
	{
		return mOffset;
	}

	Point Quad::getPosition()
	{
		return Point(mPixelDimensions.x,mPixelDimensions.y);
	}

	Size Quad::getSize()
	{
		return Size(mPixelDimensions.width,mPixelDimensions.height);
	}

	void Quad::removeFromRenderObjectGroup()
	{
		if( !mAddedToRenderGroup || (mQuadContainer == NULL) ) return;

		switch(mLayer)
		{
		case Quad::LAYER_CHILD:
			mQuadContainer->removeChildRenderable(mID);
			break;
		case Quad::LAYER_MENU:
			mQuadContainer->removeMenuRenderable(mID);
			break;
		}

		mAddedToRenderGroup = false;
	}

	void Quad::setClippingRect(const Rect& r)
	{
		mClippingRect = r;
		_clip();
	}

	void Quad::setColor(const Ogre::ColourValue& color)
	{
		setColor(color,color);
	}

	void Quad::setColor(const Ogre::ColourValue& topColor, const Ogre::ColourValue& botColor)
	{
		mTopColor = topColor;
		mBottomColor = botColor;

		mColorChanged = true;

		_updateVertexColor();

		_notifyQuadContainerNeedsUpdate();
	}

	void Quad::setDimensions(const Rect& pixelDimensions)
	{
		mPixelDimensions = pixelDimensions;
		mDimensionsChanged = true;

		_clip();

		_notifyQuadContainerNeedsUpdate();
	}

	void Quad::setHeight(Ogre::Real pixelHeight)
	{
		mPixelDimensions.height = pixelHeight;

		mDimensionsChanged = true;

		_clip();

		_notifyQuadContainerNeedsUpdate();
	}

	void Quad::setLayer(Layer l)
	{
		if( (l == mLayer) && mAddedToRenderGroup ) return;

		removeFromRenderObjectGroup();
		mLayer = l;
		addToRenderObjectGroup();
	}

	void Quad::setOffset(int offset)
	{
		mOffset = offset;

		mOffsetChanged = true;

		_notifyQuadContainerNeedsUpdate();
	}

	void Quad::setPosition(const Point& pixelPosition)
	{
		mPixelDimensions.x = pixelPosition.x;
		mPixelDimensions.y = pixelPosition.y;

		mDimensionsChanged = true;

		_clip();

		_notifyQuadContainerNeedsUpdate();
	}

	void Quad::setSize(const Size& pixelSize)
	{
		mPixelDimensions.width = pixelSize.width;
		mPixelDimensions.height = pixelSize.height;

		mDimensionsChanged = true;

		_clip();

		_notifyQuadContainerNeedsUpdate();
	}

	void Quad::setTexture(const Ogre::String& textureName)
	{
		if( mTextureName == textureName ) 
			return;

		if(!mGUIManager->textureExists(textureName)) 
		{
			setTextureCoordinates(Ogre::Vector4(0,0,1,1));
			_setTexture("");
			return;
		}

		// check for RTT or Manually created Textures
		if(Ogre::TextureManager::getSingletonPtr()->resourceExists(textureName)) // render to texture, or manually created texture
		{
			setTextureCoordinates(Ogre::Vector4(0,0,1,1));
			_setTexture(textureName);
			return;
		}

		// check for textures embedded in a skin SkinSet
		if(mGUIManager->embeddedInSkinImageset(textureName.substr(0,textureName.find_first_of('.')),textureName))
		{
			SkinSet* s = mGUIManager->getSkinImageSet(textureName.substr(0,textureName.find_first_of('.')));
			setTextureCoordinates(s->getTextureCoordinates(textureName));
			_setTexture(s->getTextureName());
		}
		else
		{
			// If we make it here, the texture is referring to a particular image file on disk,
			// that is not embedded in any skin ImageSets.
			setTextureCoordinates(Ogre::Vector4(0,0,1,1));
			_setTexture(textureName);
		}
	}

	void Quad::setTextureCoordinates(const Ogre::Vector4& textureCoordinates)
	{
		mTextureCoordinates = textureCoordinates;

		mTextureCoordsChanged = true;

		_clip();
		
		_notifyQuadContainerNeedsUpdate();
	}

	void Quad::setTextureCoordinates(const Ogre::FloatRect& textureCoordinates)
	{
		setTextureCoordinates(Ogre::Vector4(textureCoordinates.left,textureCoordinates.top,textureCoordinates.right,textureCoordinates.bottom));
	}

	void Quad::setVisible(bool visible)
	{
		mVisible = visible;

		_notifyQuadContainerNeedsUpdate();
	}

	void Quad::setWidth(Ogre::Real pixelWidth)
	{
		mPixelDimensions.width = pixelWidth;

		mDimensionsChanged = true;

		_clip();

		_notifyQuadContainerNeedsUpdate();
	}

	void Quad::setXPosition(Ogre::Real pixelX)
	{
		mPixelDimensions.x = pixelX;

		mDimensionsChanged = true;

		_clip();

		_notifyQuadContainerNeedsUpdate();
	}

	void Quad::setYPosition(Ogre::Real pixelY)
	{
		mPixelDimensions.y = pixelY;

		mDimensionsChanged = true;

		_clip();

		_notifyQuadContainerNeedsUpdate();
	}

	bool Quad::textureChanged()
	{
		return mTextureChanged;
	}

	bool Quad::visible()
	{
		return mVisible;
	}

	bool Quad::offsetChanged()
	{
		return mOffsetChanged;
	}
}
