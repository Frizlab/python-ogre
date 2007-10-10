#include "QuickGUIVertexBuffer.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	VertexBuffer::VertexBuffer(unsigned int size, GUIManager* gm) :
		mGUIManager(gm),
		mUpdateBeforeRender(false),
		mVertexBufferSize(size),
		mVertexBufferUsage(0),
		mRenderObjectList(0)
	{
		mRenderSystem = Ogre::Root::getSingleton().getRenderSystem();
		mTextureManager = Ogre::TextureManager::getSingletonPtr();
		_createVertexBuffer();

		// Initialise blending modes to be used. We use these every frame, so we'll set them up now to save time later.
		mColorBlendMode.blendType	= Ogre::LBT_COLOUR;
		mColorBlendMode.source1		= Ogre::LBS_TEXTURE;
		mColorBlendMode.source2		= Ogre::LBS_DIFFUSE;
		mColorBlendMode.operation	= Ogre::LBX_MODULATE;

		mAlphaBlendMode.blendType	= Ogre::LBT_ALPHA;
		mAlphaBlendMode.source1		= Ogre::LBS_TEXTURE;
		mAlphaBlendMode.source2		= Ogre::LBS_DIFFUSE;
		mAlphaBlendMode.operation	= Ogre::LBX_MODULATE;

		mTextureAddressMode.u = Ogre::TextureUnitState::TAM_CLAMP;
		mTextureAddressMode.v = Ogre::TextureUnitState::TAM_CLAMP;
		mTextureAddressMode.w = Ogre::TextureUnitState::TAM_CLAMP;
	}

	VertexBuffer::~VertexBuffer()
	{
		_destroyVertexBuffer();
	}

	void VertexBuffer::_createVertexBuffer()
	{
		mRenderOperation.vertexData = new Ogre::VertexData();
		mRenderOperation.vertexData->vertexStart = 0;
		mVertexBufferUsage = 0;

		_declareVertexStructure();

		// Create the Vertex Buffer, using the Vertex Structure we previously declared in _declareVertexStructure.
		mVertexBuffer = Ogre::HardwareBufferManager::getSingleton( ).createVertexBuffer(
			mRenderOperation.vertexData->vertexDeclaration->getVertexSize(0), // declared Vertex used
			mVertexBufferSize,
			Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
			false );

		// Bind the created buffer to the renderOperation object.  Now we can manipulate the buffer, and the RenderOp keeps the changes.
		mRenderOperation.vertexData->vertexBufferBinding->setBinding( 0, mVertexBuffer );
		mRenderOperation.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		mRenderOperation.useIndexes = false;
	}

	void VertexBuffer::_declareVertexStructure()
	{
		Ogre::VertexDeclaration* vd = mRenderOperation.vertexData->vertexDeclaration;

		// Add position - Ogre::Vector3 : 4 bytes per float * 3 floats = 12 bytes

		vd->addElement( 0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );

		// Add color - Ogre::RGBA : 8 bits per channel (1 byte) * 4 channels = 4 bytes

		vd->addElement( 0, Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 ), Ogre::VET_COLOUR, Ogre::VES_DIFFUSE );

		// Add texture coordinates - Ogre::Vector2 : 4 bytes per float * 2 floats = 8 bytes

		vd->addElement( 0, Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 ) +
						   Ogre::VertexElement::getTypeSize( Ogre::VET_COLOUR ),
						   Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES );

		/* Our structure representing the Vertices used in the buffer (24 bytes):
			struct Vertex
			{
				Ogre::Vector3 pos;
				Ogre::RGBA color;
				Ogre::Vector2 uv;
			};
		*/
	}

	void VertexBuffer::_destroyVertexBuffer()
	{
		delete mRenderOperation.vertexData;
		mRenderOperation.vertexData = NULL;
		mVertexBuffer.setNull();
	}

	void VertexBuffer::_initRenderState()
	{
		// make sure we're rendering to the correct viewport
		mRenderSystem->_setViewport(mGUIManager->getViewport());

		// set-up matrices
		mRenderSystem->_setWorldMatrix(Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setViewMatrix(Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setProjectionMatrix(Ogre::Matrix4::IDENTITY);

		// initialise render settings
		mRenderSystem->setLightingEnabled(false);
		mRenderSystem->_setDepthBufferParams(false, false);
		mRenderSystem->_setDepthBias(0, 0);
		mRenderSystem->_setCullingMode(Ogre::CULL_NONE);
		mRenderSystem->_setFog(Ogre::FOG_NONE);
		mRenderSystem->_setColourBufferWriteEnabled(true, true, true, true);
		mRenderSystem->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
		mRenderSystem->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
		mRenderSystem->setShadingType(Ogre::SO_GOURAUD);
		mRenderSystem->_setPolygonMode(Ogre::PM_SOLID);

		// initialise texture settings
		mRenderSystem->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
		mRenderSystem->_setTextureCoordSet(0, 0);
		mRenderSystem->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
		mRenderSystem->_setTextureAddressingMode(0, mTextureAddressMode);
		mRenderSystem->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, 0);
		mRenderSystem->_setTextureBlendMode(0, mColorBlendMode);
		mRenderSystem->_setTextureBlendMode(0, mAlphaBlendMode);
		mRenderSystem->_disableTextureUnitsFrom(1);

		// enable alpha blending
		mRenderSystem->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
	}

	void VertexBuffer::_renderVertexBuffer()
	{
		if( mRenderObjectList == NULL ) return;

		bool shadowsEnabled = mGUIManager->getViewport()->getShadowsEnabled();
		mGUIManager->getViewport()->setShadowsEnabled(false);
		
		size_t bufferPosition = 0;

		/*
		* Since mRenderList is sorted by zOrder and by Texture, we can send quads with similar textures into one renderOperation.
		* Everything rendered in one _render call will receive the texture set previously by _setTexture.
		*/
		std::list<Quad*>::iterator it = mVisibleRenderObjectList.begin();
		while( it != mVisibleRenderObjectList.end() )
		{
			Ogre::String currentTexture = (*it)->getTextureName();
			// tell render operation the where to start reading vertex data.
			mRenderOperation.vertexData->vertexStart = bufferPosition;
			// Iterate over quads with the same texture.  At least 1 iteration will occur, advancing the iterator.
			while( (it != mVisibleRenderObjectList.end()) && 
				((*it)->getTextureName() == currentTexture) )
			{
				bufferPosition += (*it)->getNumberOfVertices();
				++it;
			}
			// tell the render operation how many vertices to read.
			mRenderOperation.vertexData->vertexCount = bufferPosition - mRenderOperation.vertexData->vertexStart;
			
			// make sure texture is loaded
			if(!mTextureManager->resourceExists(currentTexture)) 
				mTextureManager->load(currentTexture,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

			// set texture that will be applied to all vertices rendered.
			mRenderSystem->_setTexture(0,true,currentTexture);
			// set render properties prior to rendering.
			_initRenderState();
			// perform the rendering.
			mRenderSystem->_render(mRenderOperation);
		}

		mGUIManager->getViewport()->setShadowsEnabled(shadowsEnabled);
	}

	bool VertexBuffer::empty()
	{
		return (mVertexBufferUsage == 0);
	}

	bool VertexBuffer::full()
	{
		return (mVertexBufferSize == mVertexBufferUsage);
	}

	size_t VertexBuffer::getBufferSize()
	{
		return mVertexBufferSize;
	}

	bool VertexBuffer::getUpdateBeforeRender()
	{
		return mUpdateBeforeRender;
	}

	void VertexBuffer::render()
	{
		if(mUpdateBeforeRender) update();
		_renderVertexBuffer();
	}

	void VertexBuffer::resizeVertexBuffer( size_t numberOfVertices )
	{
		_destroyVertexBuffer();
		mVertexBufferSize = numberOfVertices;
		_createVertexBuffer();
	}

	void VertexBuffer::setData(std::list<Quad*>* l)
	{
		mRenderObjectList = l;
	}

	void VertexBuffer::setUpdateBeforeRender(bool update)
	{
		mUpdateBeforeRender = update;
	}

	void VertexBuffer::update()
	{
		if(mRenderObjectList == NULL) return;
		
		mVisibleRenderObjectList.clear();

		// Note that locking with HBL_DISCARD will give us new, blank memory.
		mVertexBufferPtr = (Vertex*)mVertexBuffer->lock ( Ogre::HardwareVertexBuffer::HBL_DISCARD );

		size_t vertexCount = 0;

		std::list<Quad*>::iterator it;
		for( it = mRenderObjectList->begin(); it != mRenderObjectList->end(); ++it )
		{
			Quad* o = (*it);
			// skip all invisible RenderObjects
			if( (!o->visible()) || (o->getTextureName() == "") ) continue;

			std::vector<Vertex>* vList = o->getVertices();
			int vertIndex = 0;
			for( std::vector<Vertex>::iterator vItr = vList->begin(); vItr != vList->end(); ++vItr, ++vertIndex )
			{
				mVertexBufferPtr[vertIndex].pos = vItr->pos;
				mVertexBufferPtr[vertIndex].color = vItr->color;
				mVertexBufferPtr[vertIndex].uv = vItr->uv;

				++vertexCount;
			}
			mVertexBufferPtr += static_cast<int>(vList->size());
			mVisibleRenderObjectList.push_back(*it);
		}

		mVertexBuffer->unlock();
		mVertexBufferPtr = NULL;

		mVertexBufferUsage = vertexCount;
	}
}