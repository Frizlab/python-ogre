/*
--------------------------------------------------------------------------------
This source file is part of Hydrax.
Visit ---

Copyright (C) 2007 Xavier Verguín González <xavierverguin@hotmail.com>
                                           <xavyiy@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA  02111-1307, USA, or go to
http://www.gnu.org/copyleft/gpl.html.
--------------------------------------------------------------------------------
*/

#include "Perlin.h"

namespace Hydrax
{

    Perlin::Perlin()
            : mTexturePerlinPtr(0)
            , mTextureNMPtr(0)
            , mTexturePerlinPtrActive(false)
            , mTextureNMPtrActive(false)
    {
    }

    Perlin::Perlin(int seed)
    {
        Perlin();

        mPerlinModule.SetSeed(seed);
    }

    Perlin::~Perlin()
    {
    }

    void Perlin::setSize(int width, int height)
    {
        mSize.setSize(width, height);
    }

    float** Perlin::getData(bool seamless)
    {
        noise::utils::NoiseMapBuilderPlane constructor;
        noise::utils::NoiseMap noiseMap;

        constructor.SetBounds (-1.0, 1.0, -1.0, 1.0);
        constructor.SetDestSize (mSize.Width,mSize.Height);
        constructor.EnableSeamless(seamless);
        constructor.SetSourceModule(mPerlinModule);
        constructor.SetDestNoiseMap(noiseMap);
        constructor.Build();

        float** Temp = getFreeData();

        for (int k = 0; k < mSize.Width; k++)
        {
            for (int u = 0; u < mSize.Height; u++)
            {
                // Convert a [-1,1] range double to [0,1] range float
                float To01 = (1+static_cast<float>(noiseMap.GetValue(k, u)))/2;

                if (To01 > 1)
                {
                    To01 = 1;
                }
                else if (To01 < 0)
                {
                    To01 = 0;
                }

                Temp[k][u] = To01;
            }
        }

        return Temp;
    }

    float** Perlin::getFreeData()
    {
        float** Temp = new float *[mSize.Width];

        for (int k = 0; k < mSize.Width; k++)
        {
            Temp[k] = new float[mSize.Height];
        }

        return Temp;
    }

    void Perlin::free(float** data)
    {
        if (data)
        {
            for (int k = 0; k < mSize.Width; k++)
            {
                delete[] data[k];
            }

            delete[] data;
        }
    }

    noise::utils::Image Perlin::getNormalMap(double bumpHeight, bool seamless)
    {
        noise::utils::NoiseMapBuilderPlane constructor;
        noise::utils::NoiseMap noiseMap;

        constructor.SetBounds(-1.0, 1.0, -1.0, 1.0);
        constructor.SetDestSize(mSize.Width, mSize.Height);
        constructor.EnableSeamless(seamless);
        constructor.SetSourceModule(mPerlinModule);
        constructor.SetDestNoiseMap(noiseMap);
        constructor.Build();

        noise::utils::RendererNormalMap mRenderer;
        noise::utils::Image mImage;
        mImage.SetSize(mSize.Width, mSize.Height);
        mRenderer.EnableWrap(seamless);
        mRenderer.SetBumpHeight(bumpHeight);
        mRenderer.SetSourceNoiseMap(noiseMap);
        mRenderer.SetDestImage(mImage);
        mRenderer.Render();

        return mImage;
    }

    void Perlin::createPerlinTexture(Ogre::String name, Ogre::String resourceGroup)
    {
        Ogre::TextureManager::getSingleton().remove(name);

        mTexturePerlinPtr = Ogre::TextureManager::getSingleton().
                            createManual(name,
                                         resourceGroup,
                                         Ogre::TEX_TYPE_2D,
                                         mSize.Width, mSize.Height,
                                         0,
                                         Ogre::PF_BYTE_BGRA,
                                         Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

        mTexturePerlinPtrActive = true;

        float** Temp = getData();
        updatePerlinTexture(Temp);
        free(Temp);

        mTexturePerlinPtr->load();
    }

    void Perlin::updatePerlinTexture(float** data)
    {
        if (!mTexturePerlinPtrActive)
        {
            return;
        }

        Ogre::HardwarePixelBufferSharedPtr pixelBuffer = mTexturePerlinPtr->getBuffer();

        pixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
        const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

        Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

        int x,y;

        for (x = 0; x < mSize.Width; x++)
        {
            for (y = 0; y < mSize.Height; y++)
            {
                *pDest++ =   255;                    // B
                *pDest++ =   255;                    // G
                *pDest++ =   255;                    // R
                *pDest++ =   data[x][y]*255;         // A
            }
        }

        pixelBuffer->unlock();
    }

    void Perlin::createNMTexture(Ogre::String name, Ogre::String resourceGroup)
    {
        Ogre::TextureManager::getSingleton().remove(name);

        mTextureNMPtr = Ogre::TextureManager::getSingleton().
                        createManual(name,
                                     resourceGroup,
                                     Ogre::TEX_TYPE_2D,
                                     mSize.Width, mSize.Height,
                                     0,
                                     Ogre::PF_BYTE_BGRA,
                                     Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

        mTextureNMPtrActive = true;

        updateNMTexture(getNormalMap(1));

        mTextureNMPtr->load();
    }

    void Perlin::updateNMTexture(noise::utils::Image image)
    {
        if (!mTextureNMPtrActive)
        {
            return;
        }

        Ogre::HardwarePixelBufferSharedPtr pixelBuffer = mTextureNMPtr->getBuffer();

        pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD); // for best performance use HBL_DISCARD!
        const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

        Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

        noise::utils::Color c;

        int x,y;

        for (x = 0; x < mSize.Width; x++)
        {
            for (y = 0; y < mSize.Height; y++)
            {
                c = image.GetValue(x ,y);

                *pDest++ =   static_cast<uint8>(c.blue);    // B
                *pDest++ =   static_cast<uint8>(c.green);   // G
                *pDest++ =   static_cast<uint8>(c.red);     // R
                *pDest++ =   255;                           // A
            }
        }

        pixelBuffer->unlock();
    }

}

