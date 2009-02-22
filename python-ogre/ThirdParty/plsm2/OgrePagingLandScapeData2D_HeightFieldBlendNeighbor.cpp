/***************************************************************************
  OgrePagingLandScapeData2D_HeightFieldBlendNeighbor.cpp  -  description
  -------------------
  begin                : Mon Oct 13 2003
  copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
  email                : spoke@supercable.es & tuan.kuranes@free.fr
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

#include "OgreLogManager.h"
#include "OgreVector3.h"
#include "OgreColourValue.h"

#include "OgreImage.h"
#include "OgreStringConverter.h"

#include "OgreResourceManager.h"


#include "OgreException.h"

#include "OgrePagingLandScapeData2D.h"
#include "OgrePagingLandScapeData2DManager.h"
#include "OgrePagingLandScapeOptions.h"

#include "OgrePagingLandScapeData2D_HeightFieldBlendNeighbor.h"

#include "fileutils.h"

namespace Ogre
{

    //-----------------------------------------------------------------------
	PagingLandScapeData2D* PagingLandScapeData2D_HeightFieldBlendNeighbor::newPage()
    {
       return new PagingLandScapeData2D_HeightFieldBlendNeighbor(mParent);
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldBlendNeighbor::PagingLandScapeData2D_HeightFieldBlendNeighbor(PagingLandScapeData2DManager *dataMgr)
    : PagingLandScapeData2D(dataMgr)
    {
        mShadow = 0;
        mMaxheight = mParent->getOptions()->scale.y;
    }

    //-------------------------------------------------------------------
    const Real PagingLandScapeData2D_HeightFieldBlendNeighbor::getMaxAbsoluteHeight(void) const
    { 
        return mParent->getOptions()->scale.y;
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldBlendNeighbor::~PagingLandScapeData2D_HeightFieldBlendNeighbor()
    {
        PagingLandScapeData2D::unload ();
    }

    //-----------------------------------------------------------------------
    const Real PagingLandScapeData2D_HeightFieldBlendNeighbor::getShadow (const Real mX, const Real mZ,
                                                            const bool &positive)
    {
        if (mShadow)
        {
            unsigned int Pos = static_cast<unsigned int> ((mZ * mShadow->getWidth() + mX) * 3);//3 bytes (mImage is RGBA)
            if (mShadow->getSize () > Pos)
            {
                if (positive)
                    return static_cast<Real> (mShadow->getData()[ Pos + 0]) / 255;
                else
                    return static_cast<Real> (mShadow->getData()[ Pos + 1]) / 255;
            }
            else
            {
                return 0.0f;
            }
        }
        else
        {
            return 0.0f;
        }
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldBlendNeighbor::_save()
    {
		// Disabled for now
		return;
        uchar *data = new uchar[ mXDimension * mZDimension * 2 ];   

        const double divider = 65535.0 / mParent->getOptions()->scale.y;
        
        unsigned int j = 0;
        for (unsigned int i = 0; i < mMaxArrayPos; i++)
        {             
            const ushort syn = ushort  (mHeightData[i] * divider);
            #if OGRE_ENDIAN == ENDIAN_BIG
                data[j] = uchar ((syn >> 8) & 0xff);
		        data[j+ 1] = uchar (syn & 0xff);
            #else
		        data[j] = uchar (syn & 0xff);
		        data[j+ 1] = uchar ((syn >> 8) & 0xff);
            #endif
            j += 2;            
        }

        const String fname = mParent->getOptions()->LandScape_filename + "." +
                                    StringConverter::toString(mPageZ) + "." +
			                        StringConverter::toString(mPageX) + ".";
        const String extname = mParent->getOptions()->LandScape_extension;


        FileInfoListPtr finfo =  ResourceGroupManager::getSingleton().findResourceFileInfo (
                mParent->getOptions()->groupName, 
                fname + extname);
        FileInfoList::iterator it = finfo->begin();
        if (it != finfo->end())
        {
            //FileInfo *inf = &(*it);
            char *olddir = ChangeToDir (const_cast< char * > (((it)->archive->getName()).c_str()));
            //FileSystemArchive::pushDirectory()
            
            std::ofstream outfile;
            String FileNameRaw;
            
            DataStreamPtr image_chunk(new MemoryDataStream ((void*)data,
                        mXDimension * mZDimension * 2 * sizeof (uchar), 
                        true)); 

            outfile.open (const_cast< char * > ((fname + "modif." + extname).c_str()),
                            std::ios::binary);
            // Write out
            outfile << image_chunk->getAsString ();
            outfile.close ();


            //FileSystemArchive::pushDirectory();
            RetablishDir (olddir);
        }
    }
    //-----------------------------------------------------------------------
    bool PagingLandScapeData2D_HeightFieldBlendNeighbor::_load(const unsigned int mX, const unsigned int mZ)
    {
        const PagingLandScapeOptions *Options = mParent->getOptions();
        const String fileName = Options->LandScape_filename;
        const String ext      = Options->LandScape_extension;
        const String extModif = "modif." + ext;

        const String pageExtZ    = "." + StringConverter::toString(mZ) + ".";
        const String pageExtX    = StringConverter::toString(mX) + ".";
        const String pageExtZone = "." + StringConverter::toString(mZ + 1) + ".";
        const String pageExtXone = StringConverter::toString(mX + 1) + ".";

        // Load data
        const String strFileName   = fileName + pageExtZ    + pageExtX;
        const String strRightName  = fileName + pageExtZ    + pageExtXone;
        const String strBottomName = fileName + pageExtZone + pageExtX;
        const String strBRName     = fileName + pageExtZone + pageExtXone;


		Image *tex = new Image();
		Image *t_right = new Image();
		Image *t_bottom = new Image();
		Image *t_br = new Image();

        if (Options->Deformable && 
            ResourceGroupManager::getSingleton().resourceExists(Options->groupName, strFileName + extModif))
        {    
        
            tex->load (strFileName + extModif,  Options->groupName); 


			if (tex->getData() == 0)
            { 
                LogManager::getSingleton().logMessage(LML_CRITICAL, String("PLSM2 : Cannot find map named ") + strFileName + extModif);

				delete tex;
				delete t_right;
				delete t_bottom;
				delete t_br;

                return false;
            }
            if (ResourceGroupManager::getSingleton().resourceExists(Options->groupName, strRightName + extModif))
			    t_right->load (strRightName + extModif,  Options->groupName); 
			
            if (ResourceGroupManager::getSingleton().resourceExists(Options->groupName, strBottomName + extModif))
			    t_bottom ->load (strBottomName + extModif,  Options->groupName); 
			
            if (ResourceGroupManager::getSingleton().resourceExists(Options->groupName, strBRName + extModif))
			    t_br->load (strBRName + extModif,  Options->groupName); 
			

        }
        else
        {
		    tex->load (strFileName + ext,  Options->groupName); 


			if (tex->getData() == 0)
            { 
                LogManager::getSingleton().logMessage(LML_CRITICAL, String("PLSM2 : Cannot find map named ") + strFileName + ext);

				delete tex;
				delete t_right;
				delete t_bottom;
				delete t_br;

                return false;
            }
            if (ResourceGroupManager::getSingleton().resourceExists(Options->groupName, strRightName + extModif))
			    t_right->load (strRightName + ext,  Options->groupName); 
			
            if (ResourceGroupManager::getSingleton().resourceExists(Options->groupName, strBottomName + extModif))
			    t_bottom ->load (strBottomName + ext,  Options->groupName); 
			
            if (ResourceGroupManager::getSingleton().resourceExists(Options->groupName, strBRName + extModif))
			    t_br->load (strBRName + ext,  Options->groupName); 
        }
        //DataStreamPtr RawData;
        //RawData = ResourceGroupManager::getSingleton().openResource(finalName, 
                    //mParent->getOptions()->groupName);
      
        // Validate size
        // Image size comes from setting (since RAW is not self-describing)
        // here 16 bits Raw file
        mXDimension = mSize;
        mZDimension = mXDimension;
		mBpp = (unsigned int)PixelUtil::getNumElemBytes (tex->getFormat ());; 
        mMaxArrayPos = static_cast <unsigned int> (mSize * mSize);
        const size_t numBytes = mMaxArrayPos * mBpp;

        if (tex->getWidth() != tex->getHeight() ||
			tex->getWidth() != mSize - 1)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                "Texture size (" + StringConverter::toString(static_cast<unsigned int> (tex->getSize())) + 
                ") does not agree with configuration settings.", 
                "PagingLandScapeData2D_HeightFieldBlendNeighbor::_load");
        } 

		uchar *pSrc   = tex->getData();
		uchar *pRight = t_right->getData();
		uchar *pBottom = t_bottom->getData();
		uchar *pBr = t_br->getData();

		mMax = static_cast<unsigned int> (numBytes) + 1;
        mHeightData = new Real[mMaxArrayPos];
        const double scale = (1.0 / 255) * Options->scale.y;
        mMaxheight = 0.0f;

		unsigned int j = 0;
		

		for (unsigned int y = 0; y < tex->getHeight(); y++)
		{
			for (unsigned int x = 0; x < tex->getWidth(); x++)
	        {
				const uchar val = pSrc[ y * (mSize-1) + x];

		        const Real h = val * scale;
				mMaxheight = std::max (h, mMaxheight);
				mHeightData[j++] = h;
			}  

			// grab the Right Column
			if (t_right->getData() == 0)
			{
				// Copy the pixel if we have no right border
				const uchar val = pSrc[ y * (mSize-1) + (mSize-2)];
				const Real h = val * scale;
				mMaxheight = std::max (h, mMaxheight);
				mHeightData[j++] = h;
			} 
            else
			{
				// Copy the right border
				const uchar val = pRight[ y * (mSize-1)];
				const Real h = val * scale;
				mMaxheight = std::max (h, mMaxheight);
				mHeightData[j++] = h;
			}
		}
		// Generate the bottom row
		for (unsigned int x = 0; x < tex->getWidth(); x++)
		{
			if (t_bottom->getData() == 0)
			{
				const uchar val = pSrc[ (mSize-2) * (mSize-1) + x];

		        const Real h = val * scale;
				mMaxheight = std::max (h, mMaxheight);
				mHeightData[j++] = h;
			} else
			{
				const uchar val = pBottom[  x];

		        const Real h = val * scale;
				mMaxheight = std::max (h, mMaxheight);
				mHeightData[j++] = h;
			}
		}
		// Corner Pixel
		if (t_br->getData() == 0)
		{
			const uchar val = pSrc[ (mSize-2) * (mSize-1) + (mSize-2)];

		    const Real h = val * scale;
			mMaxheight = std::max (h, mMaxheight);
			mHeightData[j++] = h;
		} 
        else
		{
			const uchar val = pBr[0];

			const Real h = val * scale;
			mMaxheight = std::max (h, mMaxheight);
			mHeightData[j++] = h;
		}

		delete  tex;
		delete  t_right;
		delete  t_bottom;
		delete  t_br;
    
        if (Options->vertex_shadowed)
        {
            mShadow = new Image();
            mShadow->load(fileName +  ".HS" + pageExtZ    + pageExtX + "png", 
                            Options->groupName);
                            //mParent->getOptions()->LandScape_extension);
        }
        return true;
    }

    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldBlendNeighbor::_load()
    {
        // Load data
        DataStreamPtr RawData = ResourceGroupManager::getSingleton().openResource(mParent->getOptions()->LandScape_filename +
                    "." + 
                    mParent->getOptions()->LandScape_extension, 
                    mParent->getOptions()->groupName);

        // Validate size
        // Image size comes from setting (since RAW is not self-describing)
        // here 16 bits Raw file

        mXDimension = mParent->getOptions()->RawWidth;
        mZDimension = mParent->getOptions()->RawHeight;

        mBpp = 2;
        
        const size_t sourceHeight = mZDimension;
        const size_t sourceWidth = mXDimension;

        computePowerof2PlusOneSize ();       
		mSize = mXDimension;

        if (RawData->size() != sourceHeight*sourceWidth*2)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                "RAW size (" + StringConverter::toString(static_cast<unsigned int> (RawData->size())) + 
                ") does not agree with configuration settings.", 
                "PagingLandScapeData2D_HeightFieldBlendNeighbor::_load");
        }
    
        mMaxArrayPos = static_cast <unsigned int> (mXDimension * mZDimension);
        const size_t numBytes = mMaxArrayPos * mBpp;
		mMax = static_cast<unsigned int> (numBytes) + 1;

        mHeightData = new Real[mMaxArrayPos];
        const double scale = (1.0f / 65535.0f) * mParent->getOptions()->scale.y;
        mMaxheight = 0.0f;


        MemoryDataStream dc(RawData, 
            true);
        const uchar *pSrc = dc.getPtr ();     

        const unsigned int shift_fill = static_cast <unsigned int> (mXDimension - sourceWidth);
        unsigned int dest_pos = 0;
        //for some reason water is 65035 in SRTM files...
        const bool srtm_water = mParent->getOptions()->SRTM_water;
        for (unsigned int i = 0; i < sourceHeight; ++i)
        {
            for (unsigned int j = 0; j < sourceWidth; ++j)
            {
                #if OGRE_ENDIAN == ENDIAN_BIG
                            ushort val = *pSrc++ <<8;
                            val += *pSrc++;
                #else
                            ushort val = *pSrc++;
                            val += *pSrc++ <<8;
                #endif

                if (srtm_water && (val - 65035) > 0)
                    val = 0;

                const Real h = val * scale;
                mMaxheight = std::max (h, mMaxheight);
                mHeightData[dest_pos++] = h;
            }
            memset (&mHeightData[dest_pos], 0, shift_fill);
            dest_pos+= shift_fill; 
        }        
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldBlendNeighbor::_unload()
    {	
        delete mShadow;
        mShadow = 0;
    }
} //namespace
