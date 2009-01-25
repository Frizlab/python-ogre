#ifndef QUICKGUIBRUSH_H
#define QUICKGUIBRUSH_H

#include "QuickGUIBrushEnums.h"
#include "QuickGUIException.h"
#include "QuickGUIExportDLL.h"
#include "QuickGUIRect.h"
#include "QuickGUISkinElement.h"
#include "QuickGUIUVRect.h"
#include "QuickGUIVertex.h"

#include "OgreColourValue.h"
#include "OgreHardwareBufferManager.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreHardwareVertexBuffer.h"
#include "OgreRenderOperation.h"
#include "OgreRenderSystem.h"
#include "OgreRoot.h"
#include "OgreSingleton.h"
#include "OgreTexture.h"
#include "OgreTextureManager.h"
#include "OgreTextureUnitState.h"

#include <vector>

namespace QuickGUI
{
	// forward declarations
	class Root;

	// 2730 = 65536 bytes / 24 bytes per vertex. (~64kb allocated for vertex buffer)
	const int DEFAULT_VERTEX_BUFFER_SIZE = 2730;

	/*
	* The brush class represents the tool required to draw 2d textures to the screen.
	*/
	class Brush :
		public Ogre::Singleton<Brush>
	{
	public:
		friend class Root;
	public:
		static Brush& getSingleton(void); 
		static Brush* getSingletonPtr(void);

		/**
		* Clears the current target using the current color
		*/
		void clear();

		/**
		* Draws the currently set texture to the Render Target.  The uv coordinates define
		* the portion of the currently set texture to draw to the rectangle area.  Texture
		* is stretched to fit the dimensions.
		*/
		void drawRectangle(Rect r, UVRect ur);
		/**
		* Draws the SkinElement to the Render Target.  The texture drawn is supplied by the
		* Skin Element.  The Skin Element also defines border thickness, which is drawn as expected.
		*/
		void drawSkinElement(Rect r, SkinElement* e);
		/**
		* Draws a tiled texture, defined by the uv coordinates, within the provided rectangle area.
		*/
		void drawTiledRectangle(Rect r, UVRect ur);
		/**
		* Draws a line
		*/
		void drawLine(const Point& p1, const Point& p2);
		/**
		* Draws a rectangle made of lines
		*/
		void drawLineRectangle(const Rect& r);

		/**
		* Renders all objects queued for drawing. (1 batch)
		*/
		void emptyQueue();

		/** 
		* Gets the current clip region rectangle, in pixels
		*/
		Rect getClipRegion();
		/**
		* Gets the current color used for drawing operations.
		*/
		Ogre::ColourValue getColour();
		/**
		* Gets filtering mode used for drawing.
		*/
		BrushFilterMode getFilterMode();
		/**
		* Gets the target currently being drawn to.
		*/
		Ogre::Viewport* getRenderTarget();
		/**
		* Retrieves the current texture used for drawing operations.
		*/
		Ogre::TexturePtr getTexture();

		/**
		* Configures the Rendering system preparing it to draw in 2D.  Must be called
		* every frame, before any 2D drawing is done.
		*/
		void prepareToDraw();

		/**
		* Queues a non-tiled rectangle for drawing.  All Queued rects
		* will have the same texture.
		*/
		void queueRect(Rect r, UVRect ur);

		/**
		* Restores rendering system configuration.  Called after all drawing operations
		* for the given frame.
		*/
		void restore();

		/**
		* Sets the blend mode to use. (See BrushBlendMode)
		*/
		void setBlendMode(BrushBlendMode m);
		/**
		* Sets the current color to draw with
		*/
		void setColor(const Ogre::ColourValue& cv);
		/**
		* Sets the current clip region rectangle, in pixels
		*/
		void setClipRegion(const Rect& r);
		/**
		* Sets the filtering mode to use (See BrushFilterMode)
		*/
		void setFilterMode(BrushFilterMode m);
		/**
		* This function specifies where to draw to.
		*/
		void setRenderTarget(Ogre::TexturePtr p);
		/**
		* This function specifies where to draw to.  If NULL is passed in, the
		* default viewport is written to.
		*/
		void setRenderTarget(Ogre::Viewport* vp);
		/**
		* Sets the current texture to draw with
		*/
		void setTexture(const Ogre::String& textureName);
		/**
		* Sets the current texture to draw with
		*/
		void setTexture(Ogre::TexturePtr p);

		/**
		* Updates the SceneManager, used to render any Materials.
		*/
		void updateSceneManager(Ogre::SceneManager* sceneManager);
		/**
		* Updates the viewport initially drawn to at the beginning of every frame.
		*/
		void updateViewport(Ogre::Viewport* viewport);

	protected:
		Ogre::RenderSystem* mRenderSystem;
		Ogre::SceneManager* mSceneManager;
		Ogre::Viewport* mDefaultViewport;

		// Used to set the pass before rendering.
		Ogre::Pass* mEmptyPass;

		Ogre::Viewport* mRenderTarget;
		float mTargetWidth;
		float mTargetHeight;
		
		Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;
		
		Ogre::RenderOperation mRenderOperation;
		Ogre::TexturePtr mDefaultTexture;

		/// Texture used for draw operations.
		Ogre::TexturePtr mTexture;

		Ogre::ColourValue mColourValue;

		Rect mClipRegion;

		/// Cache Data
		Ogre::TextureUnitState::UVWAddressingMode mTextureAddressMode; //we cache this to save cpu time

		bool mUsingOpenGL;

		float mHorizontalTexelOffset;
		float mVerticalTexelOffset;

		/// member to keep track of whether or not we are queuing rects or lines for rendering
		bool mQueuedItems;
		Vertex* mBufferPtr;

	private:
		Brush();
		~Brush();

		BrushFilterMode mFilterMode;

		void _createVertexBuffer();
		/*
		* Define the size and data types that form a *Vertex*, to be used in the VertexBuffer.
		* NOTE: For ease of use, we define a structure QuickGUI::Vertex, with the same exact data types.
		*/
		void _declareVertexStructure();
		void _destroyVertexBuffer();

		// Vertex functions
		/*
		* Populates and fills vertex and uv information for rendering.
		* NOTE: verts and uv should be arrays of size 6.
		*/
		void _buildQuadVertices(const Rect& dimensions, const UVRect& uvCoords, Ogre::Vector3* verts, Ogre::Vector2* uv);
	};
}

#endif
