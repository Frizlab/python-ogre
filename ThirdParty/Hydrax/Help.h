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

#ifndef _Help_H_
#define _Help_H_

#include "Prerequisites.h"

#include "../noise/noise.h"
#include "../noise/noiseutils.h"

namespace Hydrax
{
    /** Struct wich contains an especific width and height value
    */
    struct DllExport Size
    {
        /// Width value
        int Width;
        /// Height value
        int Height;

        /** Default constructor
        */
        Size()
        {
            Width   = 0;
            Height  = 0;
        }

        /** Constructor
        	@param size The width and height values
        */
        Size(int size)
        {
            Width   = size;
            Height  = size;
        }

        /** Constructor
        	@param width Width value
        	@param height Height value
        */
        Size(int width, int height)
        {
            Width = width;
            Height  = height;
        }

        /** Destructor
        */
        ~Size()
        {
        }

        /** Sets the same width and height value
        	@param size The width and height values
        */
        void setSize(int size)
        {
            Width   = size;
            Height  = size;
        }

        /** Sets the especified values
        	@param width Width value
        	@param height Height value
        */
        void setSize(int width, int height)
        {
            Width   = width;
            Height  = height;
        }
    };

    /** Struct for store a float** data pointer, its size and its normal map
    */
    struct DllExport NoiseBuffer
    {
        /// Our float pointer data
        float**             mData;
        /// Size
        Size                mSize;
        /// Normal map(its use is optional)
        noise::utils::Image mNormalMap;

        /** Default constructor
         */
        NoiseBuffer() : mData(0)
        {
        }

        /** Destructor
         */
        ~NoiseBuffer()
        {
            if (mData)
            {
                for (int k = 0; k < mSize.Width; k++)
                {
                    delete[] mData[k];
                }

                delete[] mData;
            }
        }

        /** Constructor
        	@param Data Data pointer
        	@param Size Data bidimensional array width/height size
         */
        NoiseBuffer(float **Data, Size Size)
        {
            mData = Data;
            mSize = Size;
        }

        /** Constructor
        	@param Data Data pointer
        	@param Size Data bidimensional array width/height size
        	@param NormalMap noise::utils::Image that contains our data image normal map
         */
        NoiseBuffer(float **Data, Size Size, noise::utils::Image NormalMap)
        {
            mData = Data;
            mSize = Size;
            mNormalMap = NormalMap;
        }

        /** Set data and size
        	@param Data Data pointer
        	@param Size Data bidimensional array width/height size
         */
        void Set(float **Data, Size Size)
        {
            mData = Data;
            mSize = Size;
        }

        /** Set data, size and normal map image
        	@param Data Data pointer
        	@param Size Data bidimensional array width/height size
        	@param NormalMap noise::utils::Image that contains our data image normal map
         */
        void Set(float **Data, Size Size, noise::utils::Image NormalMap)
        {
            mData = Data;
            mSize = Size;
            mNormalMap = NormalMap;
        }

        /** Get our data pointer
            @return Data float** pointer
         */
        float** getData()
        {
            return mData;
        }

        /** Get float value from an especific x/y coordinates
            @return x/y value of our data
         */
        float getData(int x, int y)
        {
            return mData[x][y];
        }

    };
}

#endif
