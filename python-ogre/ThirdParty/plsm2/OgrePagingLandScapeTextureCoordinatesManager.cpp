/***************************************************************************
OgrePagingLandScapeTextureCoordinatesManager.cpp  -  description
-------------------
begin                : Mon Jun 16 2003
copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "OgrePagingLandScapePrecompiledHeaders.h"

#include "OgrePagingLandScapeSceneManager.h"
#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeTextureCoordinatesManager.h"

namespace Ogre
{
	//-----------------------------------------------------------------------
	PagingLandScapeTextureCoordinatesManager::PagingLandScapeTextureCoordinatesManager(PagingLandScapeSceneManager * scnMgr) :	
		mPageSize (0),
        mTileSize (0),
		mOptions (scnMgr->getOptions())
	{
	}
	//-----------------------------------------------------------------------
	PagingLandScapeTextureCoordinatesManager::~PagingLandScapeTextureCoordinatesManager()
	{
		clear();
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTextureCoordinatesManager::clear()
	{
		// Unload the Tiles
		if (!mTexBuffs.empty())
		{
			HardwareTextureBuffersCol::iterator iend = mTexBuffs.end();
			for (HardwareTextureBuffersCol::iterator i = mTexBuffs.begin(); 
				i != iend; 
				++i)
			{
				//         std::for_each(i->begin (), 
				//                        i->end (),  
				//                        delete_object());

				i->clear();   
			} 
			mTexBuffs.clear();    
		}
	}
	//-----------------------------------------------------------------------
	void PagingLandScapeTextureCoordinatesManager::load()
	{
		const unsigned int pSize = mOptions->PageSize;
		const unsigned int tSize = mOptions->TileSize;
		if (mPageSize != pSize || 
			mTileSize != tSize)
		{     
			clear();

			mPageSize = pSize;
			mTileSize = tSize;
			const unsigned int NumTiles = mOptions->NumTiles;

			mTexBuffs.reserve (NumTiles);
			mTexBuffs.resize (NumTiles);
			for (unsigned int  i = 0; i < NumTiles; ++i)
			{
				mTexBuffs[i].reserve (NumTiles);
				mTexBuffs[i].resize (NumTiles);
			}
		}
	}
	//-----------------------------------------------------------------------
	HardwareVertexBufferSharedPtr PagingLandScapeTextureCoordinatesManager::getBuffer(
		const unsigned int tilex, 
		const unsigned int tilez)
	{
		PLSM2_ASSERT (tilex < mOptions->NumTiles && 
				tilez < mOptions->NumTiles);

		if (mTexBuffs [tilex][tilez].isNull ())
		{
			const unsigned int tileSize = mOptions->TileSize;

			const VertexElementType t = VET_FLOAT2;
			//const VertexElementType t = VET_SHORT2;
			const size_t vertexSize = VertexElement::getTypeSize (t);
			HardwareVertexBufferSharedPtr vbuf = 
				HardwareBufferManager::getSingleton ().createVertexBuffer(
														vertexSize, 
														tileSize * tileSize,
														HardwareBuffer::HBU_STATIC_WRITE_ONLY);

			float* pTex = static_cast<float*> (vbuf->lock(HardwareBuffer::HBL_DISCARD));
			//ushort* pSecond = static_cast<ushort*> (vbuf->lock(HardwareBuffer::HBL_DISCARD));

			// Calculate the offset in the texture position
			const unsigned int offSetX = tilex * (tileSize - 1);
			const unsigned int offSetZ = tilez * (tileSize - 1);
			const unsigned int endx = offSetX + tileSize;
			const unsigned int endz = offSetZ + tileSize;

			const Real Aux1 =  1.0 / (mOptions->PageSize - 1);
			Real K_Tex2DataPos = offSetZ * Aux1;
			for (unsigned int k = offSetZ; k < endz; k ++)
			{
				Real K_Tex1DataPos = offSetX * Aux1;
				for (unsigned int i = offSetX; i < endx; i ++)
				{
					// textures
					//PLSM2_ASSERT (K_Tex1DataPos >= 0.0f && K_Tex1DataPos <= 1.0f);
					//PLSM2_ASSERT (K_Tex2DataPos >= 0.0f && K_Tex2DataPos <= 1.0f);
					if (K_Tex1DataPos > 1.0f) K_Tex1DataPos = 1.0f;
					if (K_Tex2DataPos > 1.0f) K_Tex2DataPos = 1.0f;
					*pTex++ = K_Tex1DataPos;	
					*pTex++ = K_Tex2DataPos;

					//            *pTex++ = static_cast<ushort> (K_Tex1DataPos * 65535);
					//            *pTex++ = static_cast<ushort> (K_Tex2DataPos * 65535);

					K_Tex1DataPos += Aux1;
				}
				K_Tex2DataPos += Aux1;
			}
			vbuf->unlock();   	
			mTexBuffs [tilex][tilez] = vbuf;
		}
		return mTexBuffs [tilex][tilez];
	}
} //namespace
