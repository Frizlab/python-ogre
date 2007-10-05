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
	class Widget;

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
		Quad(const Ogre::String& id, Widget* owner);
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

		Rect getClippingRect();
		Widget* getClippingWidget();
		Rect getDimensions();
		Ogre::String getID();
		Layer getLayer();
		Ogre::String getTextureName();
		std::vector<Vertex>* getVertices();
		int getNumberOfVertices();
		int getOffset();
		Point getPosition();
		Size getSize();

		bool isPointWithinBounds(const Point& pixelPosition);

		void removeFromRenderObjectGroup();

		void setClippingWidget(Widget* w);
		void setColor(const Ogre::ColourValue& color);
		void setColor(const Ogre::ColourValue& topColor, const Ogre::ColourValue& botColor);
		void setDimensions(const Rect& pixelDimensions);
		void setGUIManager(GUIManager* gm);
		void setHeight(Ogre::Real pixelHeight);
		void setLayer(Layer l);
		void setOffset(int offset);
		void setPosition(const Point& pixelPosition);
		void setSize(const Size& pixelSize);
		void setTexture(const Ogre::String& textureName);
		void setTextureCoordinates(const Ogre::Vector4& textureCoordinates);
		void setTextureCoordinates(const Ogre::FloatRect& textureCoordinates);
		void setVisible(bool visible);
		void setWidth(Ogre::Real pixelWidth);
		void setXPosition(Ogre::Real pixelX);
		void setYPosition(Ogre::Real pixelY);

		bool textureChanged();

		void updateClippingWidget();

		bool visible();

		bool offsetChanged();

	protected:
		Widget*				mOwner;
		GUIManager*			mGUIManager;
		QuadContainer*		mQuadContainer;
		Ogre::RenderSystem* mRenderSystem;

		Layer				mLayer;

		// Name of Widget
		Ogre::String		mID;
		Ogre::String		mTextureName;
		// number of parent iterations away from sheet quad.
		int					mOffset;
		Rect				mPixelDimensions;
		// UV coordinates: (left,top,right,bottom)
		Ogre::Vector4		mTextureCoordinates;
		bool				mVisible;
		Ogre::ColourValue	mTopColor;
		Ogre::ColourValue	mBottomColor;

		Widget*				mClippingWidget;
		// Store changes made via Clipping.
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
