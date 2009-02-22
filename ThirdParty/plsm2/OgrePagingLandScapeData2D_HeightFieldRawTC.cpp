/***************************************************************************
  OgrePagingLandScapeData2D_HeightFieldRawTC.cpp  -  description
  -------------------
  begin                : Mon Oct 13 2003
  copyright            : (C) 2003-2006 by Jose A Milan && Tuan Kuranes
  email                : spoke@supercable.es & tuan.kuranes@free.fr
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

#include "OgrePagingLandScapeData2D_HeightFieldRawTC.h"

#include "fileutils.h"


namespace Ogre
{

    //-----------------------------------------------------------------------
	PagingLandScapeData2D* PagingLandScapeData2D_HeightFieldRawTC::newPage()
    {
       return new PagingLandScapeData2D_HeightFieldRawTC(mParent);
    }
    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldRawTC::PagingLandScapeData2D_HeightFieldRawTC(PagingLandScapeData2DManager *dataMgr)
    : PagingLandScapeData2D(dataMgr)
    {
	    mImage = 0;
	    input_max =  3000.0f;
        input_min = 0.0f;
        mMaxheight = _decodeRawTC (1.0f) * mParent->getOptions()->scale.y;
    }
    //-------------------------------------------------------------------
    const Real PagingLandScapeData2D_HeightFieldRawTC::getMaxAbsoluteHeight(void) const
    { 
        return _decodeRawTC (1.0f) * mParent->getOptions()->scale.y;
    }

    //-----------------------------------------------------------------------
    PagingLandScapeData2D_HeightFieldRawTC::~PagingLandScapeData2D_HeightFieldRawTC()
    {
    }
    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2D_HeightFieldRawTC::getBase (const Real mX, const Real mZ)
    {
        return ColourValue::White;
    }

    //-----------------------------------------------------------------------
    const ColourValue PagingLandScapeData2D_HeightFieldRawTC::getCoverage (const Real mX, const Real mZ)
    {
        return ColourValue::White;
    }
    //-----------------------------------------------------------------------
    const Vector3 PagingLandScapeData2D_HeightFieldRawTC::getNormalAt (const Real x, const Real z)
    {
        #ifndef _LOADEDNORM
            return PagingLandScapeData2D::getNormal(x,z);
        #else
            if (mImage)
            {
                unsigned int Pos = static_cast<unsigned int> ((z * mSize  + x) * mBpp);//4 bytes (mImage is RGBA)

                if (mMax > Pos)
                {
                    const Real normalscale = 1.0f / 127.0f;
                    return Vector3 (((Real)(mImage->getData()[Pos + 0]) - 128.0f) * normalscale,
                                    ((Real)(mImage->getData()[Pos + 1]) - 128.0f) * normalscale,
                                    ((Real)(mImage->getData()[Pos + 2]) - 128.0f) * normalscale);
                }
                else
                {
                    return Vector3::UNIT_Y;
                }
            }
            else
            {
                return Vector3::UNIT_Y;
            }
        #endif //_NOLOAD
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldRawTC::_save()
    {
        uchar *data = new uchar[ mXDimension * mZDimension * 2 ];   

        const double divider = 65535.0 / mParent->getOptions()->scale.y;
        
        unsigned int j = 0;
        for (unsigned int i = 0; i < mXDimension*mZDimension; i++)
        {             
            ushort syn = static_cast <ushort>  (_encodeRawTC (mHeightData[i]) * divider);
            #if OGRE_ENDIAN == OGRE_ENDIAN_BIG
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
    bool PagingLandScapeData2D_HeightFieldRawTC::_load(const unsigned int mX, const unsigned int mZ)
    {
        // Load data
        //mRawData.clear();

        const String strFileName = mParent->getOptions()->LandScape_filename + "." + 
                            StringConverter::toString(mZ) + "." +
			                StringConverter::toString(mX) + ".";

            
        String finalName = strFileName + 
                        "modif." + 
                        mParent->getOptions()->LandScape_extension;
        if (!(mParent->getOptions()->Deformable && 
            ResourceGroupManager::getSingleton().resourceExists(mParent->getOptions()->groupName,finalName)))
        {    
            finalName = strFileName + 
                mParent->getOptions()->LandScape_extension;   
            if (!ResourceGroupManager::getSingleton().resourceExists(mParent->getOptions()->groupName,finalName))
            { 
                LogManager::getSingleton().logMessage(LML_CRITICAL, String("PLSM2 : Cannot find map named ") + finalName);
                return false;
            }
        }
        DataStreamPtr RawData;
        RawData = ResourceGroupManager::getSingleton().openResource(finalName, 
                    mParent->getOptions()->groupName);
        
        // Validate size
        // Image size comes from setting (since RAW is not self-describing)
        // here 16 bits Raw file
        mXDimension = mSize;
        mZDimension = mXDimension;
        mBpp = 2;

        size_t numBytes = mSize * mSize * mBpp;
        if (RawData->size() != numBytes)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
                ") does not agree with configuration settings.", 
                "PagingLandScapeData2D_HeightFieldRaw::_load");
        }
		mMax = static_cast<unsigned int> (numBytes) + 1;

        mMaxArrayPos = static_cast <unsigned int> (mSize * mSize);
        mHeightData = new Real[mMaxArrayPos];            

        const Real divider = 1.0f / 65535.0f;
        const Real scale  = mParent->getOptions()->scale.y;
        
        MemoryDataStream dc (RawData, 
				                true);
        const uchar *pSrc = dc.getPtr ();     

        mMaxheight = 0.0f;
        unsigned int j = 0;
        for (unsigned int i = 0; i < mMax - 1;  i += mBpp)
        {                  
            #if OGRE_ENDIAN == OGRE_ENDIAN_BIG
                        ushort val = *pSrc++ <<8;
                        val += *pSrc++;
            #else
                        ushort val = *pSrc++;
                        val += *pSrc++ <<8;
            #endif
            const Real h = _decodeRawTC (val * divider)* scale;
            mMaxheight = std::max (h, mMaxheight);
            mHeightData[j++] = h;
        }
    
//            if (mParent->getOptions()->vertex_shadowed)
//            {
//                mShadow = new Image();
//                mShadow->load(mParent->getOptions()->LandScape_filename +
//                                ".HS." +
//                                StringConverter::toString(mZ) + "." +
//                                StringConverter::toString(mX) + "." +
//                                "png");
//                                //mParent->getOptions()->LandScape_extension);
//            }
        return true;
    }
    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldRawTC::_load()
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
                "PagingLandScapeData2D_HeightFieldRaw::_load");
        }
    
        mMaxArrayPos = static_cast <unsigned int> (mXDimension * mZDimension);
        const size_t numBytes = mMaxArrayPos * mBpp;
		mMax = static_cast<unsigned int> (numBytes) + 1;

        mHeightData = new Real[mMaxArrayPos];

        mMaxheight = 0.0f;


        MemoryDataStream dc(RawData, 
            true);
        const uchar *pSrc = dc.getPtr ();     

        const Real divider = 1.0f / 65535.0f;
        const Real scale  = mParent->getOptions()->scale.y;
        const unsigned int shift_fill = static_cast <unsigned int> (mXDimension - sourceWidth);
        unsigned int dest_pos = 0;
        //for some reason water is 65035 in SRTM files...
        const bool srtm_water = mParent->getOptions()->SRTM_water;
        for (unsigned int i = 0; i < sourceHeight; ++i)
        {
            for (unsigned int j = 0; j < sourceWidth; ++j)
            {
                #if OGRE_ENDIAN == OGRE_ENDIAN_BIG
                            ushort val = *pSrc++ <<8;
                            val += *pSrc++;
                #else
                            ushort val = *pSrc++;
                            val += *pSrc++ <<8;
                #endif

                if (srtm_water && (val - 65035) > 0)
                    val = 0;

                const Real h = _decodeRawTC (val * divider) * scale;
                mMaxheight = std::max (h, mMaxheight);
                mHeightData[dest_pos++] = h;
            }
            memset (&mHeightData[dest_pos], 0, shift_fill);
            dest_pos+= shift_fill; 
        }
    }
  

    //-----------------------------------------------------------------------
    void PagingLandScapeData2D_HeightFieldRawTC::_unload()
    {
	    delete mImage;
		mImage = 0;
	 }

    //-----------------------------------------------------------------------
    inline Real PagingLandScapeData2D_HeightFieldRawTC::_decodeRawTC(const Real encoded) const
    {
	    return ((Real) (encoded + 0.5f)) * (input_max - input_min) + input_min;
    }
    //-----------------------------------------------------------------------
    inline ushort PagingLandScapeData2D_HeightFieldRawTC::_encodeRawTC(const Real decoded) const
    {
        return static_cast <short> ((decoded - input_min) / (input_max - input_min) - 0.5f);
    }

} //namespace
