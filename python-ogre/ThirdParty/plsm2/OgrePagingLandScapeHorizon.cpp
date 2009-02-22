/***************************************************************************
	OgrePagingLandScapeHorizon.cpp  -  description
	-------------------
	begin                : Sat Mar 08 2003
	copyright            : (C) 2003-2006 by Jose A. Milan and Tuan Kuranes
	email                : spoke2@supercable.es && tuan.kuranes@free.fr
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Lesser General Public License as        *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
***************************************************************************/

#include "OgrePagingLandScapePrecompiledHeaders.h"


#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreMovableObject.h"
#include "OgreAxisAlignedBox.h"

#include "OgreCamera.h"

#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreTechnique.h"
#include "OgrePass.h"
#include "OgreTextureUnitState.h"

#include "OgrePagingLandScapeTileInfo.h"

#include "OgrePagingLandScapeOptions.h"
#include "OgrePagingLandScapeHorizon.h"
#include "OgrePagingLandScapeCamera.h"



namespace Ogre
{

    //-----------------------------------------------------------------------
    PagingLandScapeHorizon::PagingLandScapeHorizon(const PagingLandScapeOptions * const options):
        mOptions(options)
    {
        mPageWidth = options->world_width;
        mPageHeight = options->world_height;

        mNumTilesPage = options->NumTiles;

        mTileWidth = mPageWidth * mNumTilesPage;
        mTileHeight = mPageHeight * mNumTilesPage;

        size_t p_memssize = mPageWidth*mPageHeight;

        mMinPageHeights = new Real[p_memssize];
        memset (mMinPageHeights, 0, p_memssize * sizeof(Real));

        mMaxPageHeights = new Real[p_memssize];
        memset (mMaxPageHeights, 0, p_memssize * sizeof(Real));

        size_t t_memssize = mTileWidth*mTileHeight;

        mMinTileHeights = new Real[t_memssize];
        memset (mMinTileHeights, 0, t_memssize * sizeof(Real));

        mMaxTileHeights = new Real[t_memssize];
        memset (mMaxTileHeights, 0, t_memssize * sizeof(Real));
        
        material_enabled = false;
        mVisData = 0;
    }    
    //-----------------------------------------------------------------------
    PagingLandScapeHorizon::~PagingLandScapeHorizon()
    {
	   delete [] mMinPageHeights;
	   delete [] mMaxPageHeights;

	   delete [] mMinTileHeights;
	   delete [] mMaxTileHeights;

       mVisibilityMaterial.setNull();
    }
   //-----------------------------------------------------------------------
    void PagingLandScapeHorizon::prepare(const PagingLandScapeCamera* cam)
    {        
        if (material_enabled)
        {
            //reset.
            memset (mVisData, 0, mTileWidth * mTileHeight * 4);

            //add camera point        
            const Real tileX = cam->mCurrentCameraPageX * mNumTilesPage + cam->mCurrentCameraTileX;
            const Real tileZ = cam->mCurrentCameraPageZ * mNumTilesPage + cam->mCurrentCameraTileZ;
            const unsigned int tilePos = static_cast <unsigned int> ((tileX + tileZ * mTileWidth) * 4);
            mVisData [ tilePos ]    = 0;
            mVisData [ tilePos + 1] = 0;
            mVisData [ tilePos + 2] = 255;
            mVisData [ tilePos + 3] = 255;
        }
    }
   //-----------------------------------------------------------------------
    void PagingLandScapeHorizon::update()
    { 
        if (material_enabled)
        {
            const PixelBox srcBox = mVisImage.getPixelBox();

            HardwarePixelBufferSharedPtr Texbuffer = mVisTex->getBuffer (0, 0);	
            const PixelBox lock = Texbuffer->lock (srcBox, HardwareBuffer::HBL_DISCARD); 
            // lock.data can now be freely accessed  
            PixelUtil::bulkPixelConversion(srcBox, lock); 

            Texbuffer->unlock();  
        }
    }
    //-----------------------------------------------------------------------
    MaterialPtr PagingLandScapeHorizon::getVisibilityMaterial()
    {
        if (mVisibilityMaterial.isNull())
        {        
            const PagingLandScapeOptions * const opt = mOptions;
            const String filename = opt->LandScape_filename;
            const String name = filename +
                "Visibility";
            mVisibilityMaterial = MaterialManager::getSingleton().getByName (name);
            if (mVisibilityMaterial.isNull())
            {
                mVisibilityMaterial = MaterialManager::getSingleton().create (name, 
                                                                              opt->groupName);

                TextureUnitState *tu0 = mVisibilityMaterial->getTechnique (0)->
                                        getPass (0)->createTextureUnitState ();
 
                tu0->setTextureName (filename + ".Small." + 
                                     opt->TextureExtension);

                
                tu0->setAlphaOperation (LBX_BLEND_MANUAL, LBS_MANUAL, LBS_CURRENT, 0.7, 1, 1);
                //tu0->setTextureAddressingMode (TextureUnitState::TextureAddressingMode::TAM_CLAMP);
		        tu0->setTextureAddressingMode (TextureUnitState::TAM_CLAMP);
		        mVisibilityMaterial->setSceneBlending (SBT_TRANSPARENT_ALPHA);
                //mVisibilityMaterial->setDepthCheckEnabled (false);
                //mVisibilityMaterial->setDepthWriteEnabled (false);

                const size_t s = mTileWidth * mTileHeight * 4;
                uchar *TexData = new uchar [s];
                memset (TexData, 0, s * sizeof(uchar));
		        // Assign the texture to the alpha map		        
		        mVisImage.loadDynamicImage(TexData, mTileWidth, mTileHeight, 1, PF_R8G8B8A8, true);
                mVisData = mVisImage.getData();
                const String texname = filename + ".Visibility";
                mVisTex = TextureManager::getSingleton().loadImage(texname, 
                                        opt->groupName,
                                        mVisImage, 
                                        TEX_TYPE_2D, 0, 1.0f);
                TextureUnitState *tu1 = mVisibilityMaterial->getTechnique (0)->
                                        getPass (0)->createTextureUnitState(texname);
                tu1->setTextureAddressingMode (TextureUnitState::TAM_CLAMP);
            }
            else
            {
                const size_t s = mTileWidth * mTileHeight * 4;
                uchar *TexData = new uchar [s];
                memset (TexData, 0, s * sizeof(uchar));
		        // Assign the texture to the alpha map
		        mVisImage.loadDynamicImage(TexData, mTileWidth, mTileHeight, 1, PF_R8G8B8A8, true);
                mVisData = mVisImage.getData();
                mVisTex = TextureManager::getSingleton().getByName (filename + ".Visibility");
            }
        }
        mVisibilityMaterial->load();
        material_enabled = true;
        return mVisibilityMaterial;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeHorizon::AddVisibleTile (const unsigned int Tilex, const unsigned int Tilez,
        const bool visible)
    {
        if (material_enabled)
        {
            const unsigned int tilePos = (Tilex + Tilez * mTileWidth) * 4;
            if (visible) 
            {
                mVisData [ tilePos ]    = 0;
                mVisData [ tilePos + 1] = 255;
                mVisData [ tilePos + 2] = 0;
                mVisData [ tilePos + 3] = 255;
            }
            else
            {
                mVisData [ tilePos ]    = 255;
                mVisData [ tilePos + 1] = 0;
                mVisData [ tilePos + 2] = 0;
                mVisData [ tilePos + 3] = 255;
            }
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeHorizon::AddVisibleTile (const PagingLandScapeTileInfo *info, 
        const bool visible)
    {
        if (material_enabled)
        {
			const unsigned int TileX = info->mPageX*mNumTilesPage + info->mTileX;
            const unsigned int TileZ = info->mPageZ*mNumTilesPage + info->mTileZ;

            AddVisibleTile (TileX, TileZ, visible);      
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeHorizon::registerMinMaxHeightPage (const unsigned int pageX, const unsigned int pageZ,
                                                        const Real minHeight, const Real maxHeight)
    { 
       const size_t pos = mPageWidth*mPageHeight;
       mMinPageHeights[pos] = minHeight;
       mMaxPageHeights[pos] = maxHeight;       
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeHorizon::registerMinMaxHeightTile (const PagingLandScapeTileInfo *info,
                                                        const Real minHeight, const Real maxHeight)        
    {   
		const size_t tilePos = (info->mPageX*mNumTilesPage + info->mTileX
           + ((info->mPageZ*mNumTilesPage)  + info->mTileZ) * mTileWidth); 

       PLSM2_ASSERT (tilePos < mTileWidth*mTileHeight);

       mMinTileHeights[tilePos] = minHeight;
       mMaxTileHeights[tilePos] = maxHeight;
       
       const size_t pagePos = info->mPageX + info->mPageZ * mPageWidth; 

       PLSM2_ASSERT (pagePos < mPageWidth*mPageHeight);

       // if tile height is maximum or minimum height page
        if (mMaxPageHeights[pagePos] < maxHeight)
            mMaxPageHeights[pagePos] = maxHeight;
        if (mMinPageHeights[pagePos] > minHeight)
            mMinPageHeights[pagePos] = minHeight;
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeHorizon::IsPageVisible(const PagingLandScapeCamera* cam,
                                               const unsigned int destpageX, const unsigned int destpageZ)
    {
        const Real PageX = cam->mCurrentCameraPageX;
        const Real PageZ = cam->mCurrentCameraPageZ;

        // test if there is potential occluders
        if (fabs (PageX - destpageX) < 2 && fabs (PageZ - destpageZ) < 2)
            return true;

       PLSM2_ASSERT (destpageX + destpageZ*mPageWidth < mPageWidth*mPageHeight);

        return calcVis(Vector3 (PageX, cam->getDerivedPosition ().y, PageZ), 
                       Vector3 (destpageX, mMaxPageHeights[destpageX + destpageZ*mPageWidth], destpageZ), 
                       mMinPageHeights, mPageWidth, mPageHeight);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeHorizon::IsTileVisible(const PagingLandScapeCamera* cam,
                                               const PagingLandScapeTileInfo *destinfo)
    {         


        const Real RsrcTileX = cam->mCurrentCameraPageX * mNumTilesPage + cam->mCurrentCameraTileX;
        const Real RsrcTileZ = cam->mCurrentCameraPageZ * mNumTilesPage + cam->mCurrentCameraTileZ;

		const Real RdestTileX = destinfo->mPageX*mNumTilesPage + destinfo->mTileX;
		const Real RdestTileZ = destinfo->mPageZ*mNumTilesPage + destinfo->mTileZ;
       
        // test if there is potential occluders
        if (fabs (RsrcTileX - RdestTileX) < 2.0f && fabs (RsrcTileZ - RdestTileZ) < 2.0f)
            return true; 

        const size_t pos = static_cast <size_t> (RdestTileX + RdestTileZ*mTileWidth);
        
       PLSM2_ASSERT (pos < mTileWidth*mTileHeight); 

        return calcVis(Vector3 (RsrcTileX, cam->getDerivedPosition ().y, RsrcTileZ), 
                       Vector3 (RdestTileX, mMaxTileHeights[pos], RdestTileZ), 
                        mMinTileHeights, mTileWidth, mTileHeight);
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeHorizon::calcVis(const Vector3 &src, const Vector3 &dest, 
                                         const Real * const heightMap, 
                                         const unsigned int mapWidth, const unsigned int mapHeight)
    {
        /* Make sure the ray is normalised */
        const Real x = dest.x - src.x;
        const Real z = dest.z - src.z;
        /*normalise only on x and z*/
        const Real fLength = Math::Sqrt(x * x + z * z);
        // Will also work for zero-sized vectors, but will change nothing
        if (fLength < 1e-08)
            return true;

        const Real fInvLength = 1.0 / fLength;
        const Vector3 direction (x * fInvLength,
                                 (dest.y - src.y) * fInvLength,
                                 z * fInvLength);          
        

        Real lasty = src.y;

        Vector3 currpos (src + direction); // fetch new tile

        /* For each heightmap location in the ray */ 
        while (currpos.x >= 0 &&
                currpos.z >= 0 &&
                currpos.x < mapWidth &&
                currpos.z < mapHeight)
        {
            const size_t posx = static_cast <size_t> (currpos.x + 0.5f);
            const size_t posz = static_cast <size_t> (currpos.z + 0.5f);
            if (posx == dest.x && posz == dest.z)
                break;

            const Real curry = currpos.y;
            currpos = currpos + direction;  // fetch new tile
            const Real nexty = currpos.y;   // fetch next tile height
            
            const Real h = heightMap[posx + posz*mapWidth];
            if (h > nexty && h > lasty)
            {
                AddVisibleTile (static_cast <unsigned int> (dest.x),
				                static_cast <unsigned int> (dest.z),
						        false);
                return false; // line of sight is occluded               
            }
            lasty = curry;           
        }
        AddVisibleTile (static_cast <unsigned int> (dest.x),
			            static_cast <unsigned int> (dest.z),
			            true);
        return true;
    }
} //namespace
