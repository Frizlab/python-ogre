#ifndef QUICKGUIVERTEXBUFFER_H
#define QUICKGUIVERTEXBUFFER_H

#include "OgreHardwareBufferManager.h"
#include "OgreHardwareVertexBuffer.h"
#include "OgrePrerequisites.h"
#include "OgreRenderOperation.h"
#include "OgreRenderSystem.h"
#include "OgreRoot.h"
#include "OgreTextureManager.h"
#include "OgreTextureUnitState.h"

#include "QuickGUIExportDLL.h"
#include "QuickGUIQuad.h"
#include "QuickGUIVertex.h"

#include <list>
#include <map>
#include <utility>

namespace QuickGUI
{
	// Forward declarations
	class GUIManager;

	class _QuickGUIExport VertexBuffer
	{
	public:
		VertexBuffer(unsigned int size, GUIManager* gm);
		~VertexBuffer();

		bool empty();

		bool full();

		size_t getBufferSize();
		bool getUpdateBeforeRender();

		void render();
		void resizeVertexBuffer( size_t numberOfVertices );

		void setData(std::list<Quad*>* l);
		void setUpdateBeforeRender(bool update);

		void update();

	protected:
		GUIManager* mGUIManager;

		Ogre::RenderSystem* mRenderSystem;
		std::list<Quad*>* mRenderObjectList;
		std::list<Quad*> mVisibleRenderObjectList;
		bool mUpdateBeforeRender;

		// Stores the Texture of a quad, and the index of the first quad following it with a different texture.
		std::vector< std::pair<Ogre::String,int> > mTextureChangeList;

		// required to ensure textures are loaded before used
		Ogre::TextureManager* mTextureManager;

		Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;
		size_t mVertexBufferSize;	// the maximum number of vertices the buffer can hold.
		size_t mVertexBufferUsage;	// the current number of vertices stored in the buffer.
		Vertex* mVertexBufferPtr;	// pointer used to iterate through vertices in the buffer. NULL when buffer unlocked. (must be locked to work with)

		// Every RenderSystem::_render(mRenderOperation) constitutes a batch.
		Ogre::RenderOperation mRenderOperation;

		// Cache Data
		Ogre::TextureUnitState::UVWAddressingMode mTextureAddressMode; //we cache this to save cpu time
		Ogre::LayerBlendModeEx mColorBlendMode; // we cache this to save cpu time
		Ogre::LayerBlendModeEx mAlphaBlendMode; // we cache this to save cpu time

	private:
		void _initRenderState();

		// NOTE: we do not need a _clearVertexBuffer function, since we get new, blank memory when locking with HBL_DISCARD.
		void _createVertexBuffer();
		/*
		* Define the size and data types that form a *Vertex*, to be used in the VertexBuffer.
		* NOTE: For ease of use, we define a structure QuickGUI::Vertex, with the same exact data types.
		*/
		void _declareVertexStructure();
		void _destroyVertexBuffer();
		void _renderVertexBuffer();
	};
}

#endif