#ifndef QUICKGUIQUAD_H
#define QUICKGUIQUAD_H

#include "OgreCommon.h"
#include "OgreRenderSystem.h"
#include "OgreVector4.h"

#include "QuickGUIExportDLL.h"
#include "QuickGUIRect.h"
#include "QuickGUIVertex.h"

#include <vector>

namespace QuickGUI
{
	// forward declaration
	class QuadContainer;
	class GUIManager;

	class _QuickGUIExport Quad
	{
	public:
		/**
		* Used to determine Render Ordering in Quad Container.  Menu Layer is rendered last.
		*/
		typedef enum Layer
		{
			LAYER_CHILD		=  0,
			LAYER_MENU
		};
	public:
		Quad(const Ogre::String& id, GUIManager* gm);
		~Quad();
	
		void _notifyAddedToRenderObjectGroup();
		// Allows the renderer to notify the quad its changes were accepted.
		void _notifyChangesHandled();
		void _notifyRemovedFromQuadContainer();
		void _notifyQuadContainer(QuadContainer* container);
		void _notifyQuadContainerNeedsUpdate();

		void addToRenderObjectGroup();

		bool dimensionsChanged();

		Ogre::String getID();
		Layer getLayer();
		Rect getRectIntersection(const Rect& r);
		Ogre::String getTextureName();
		std::vector<Vertex>* getVertices();
		int getNumberOfVertices();
		int getOffset();
		Point getPosition();
		Size getSize();

		bool insideRect(const Rect& r);
		bool intersectsRect(const Rect& r);
		bool isPointWithinBounds(const Point& absPosition);

		void removeFromRenderObjectGroup();

		void setColor(const Ogre::ColourValue& color);
		void setColor(const Ogre::ColourValue& topColor, const Ogre::ColourValue& botColor);
		void setClippingRect(const Rect& r);
		void setDimensions(const Rect& absoluteDimensions);
		void setHeight(Ogre::Real absoluteHeight);
		void setLayer(Layer l);
		void setOffset(int offset);
		void setPosition(const Point& absolutePosition);
		void setSize(const Size& absoluteSize);
		void setTexture(const Ogre::String& textureName);
		void setTextureCoordinates(const Ogre::Vector4& textureCoordinates);
		void setTextureCoordinates(const Ogre::FloatRect& textureCoordinates);
		void setVisible(bool visible);
		void setWidth(Ogre::Real absoluteWidth);
		void setXPosition(Ogre::Real absoluteX);
		void setYPosition(Ogre::Real absoluteY);

		bool textureChanged();

		bool visible();

		bool offsetChanged();

	protected:
		GUIManager*			mGUIManager;
		QuadContainer*		mQuadContainer;
		Ogre::RenderSystem* mRenderSystem;

		Layer				mLayer;

		// Name of Widget
		Ogre::String		mID;
		Ogre::String		mTextureName;
		// number of parent iterations away from sheet quad.
		int					mOffset;
		Rect				mAbsoluteDimensions;
		// UV coordinates: (left,top,right,bottom)
		Ogre::Vector4		mTextureCoordinates;
		bool				mVisible;
		Ogre::ColourValue	mTopColor;
		Ogre::ColourValue	mBottomColor;

		Rect				mClippingRect;
		// Store changes made via Clipping.
		bool				mHiddenViaClipping;
		Rect				mDimensionsViaClipping;
		Ogre::Vector4		mTextureCoordinatesViaClipping;

		bool				mAddedToRenderGroup;
		bool				mColorChanged;
		bool				mDimensionsChanged;
		bool				mTextureChanged;
		bool				mTextureCoordsChanged;
		bool				mOffsetChanged;

		std::vector<Vertex> mVertices;

		/*
		* Resizes and adjusts texture coords to display any visible portions that lie
		* inside Rect r.
		* NOTE: If Quad lies outside Rect r, it will not be clipped, but hidden.
		* NOTE: If Quad lies inside Rect r, no clipping will occur.
		*/
		void _clip();
		void _computeVertices();
		void _updateTextureCoords();
		void _updateVertexColor();

		void _setTexture(const Ogre::String& textureName);
	};
}

#endif
