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

#ifndef _Perlin_H_
#define _Perlin_H_

#include "Prerequisites.h"

#include "Help.h"

#include "../noise/noise.h"
#include "../noise/noiseutils.h"

namespace Hydrax
{
    /** Class for provide a solid object for use perlin noise data and generate ogre textures.
        @remarks This class is used too for generate Ogre textures, wich can be used in shaders,
        or something like that.
     */
    class DllExport Perlin
    {
    public:

        /** Default constructor
         */
        Perlin();

        /** Constructor
            @param seed Perlin noise sedd
         */
        Perlin(int seed);

        /** Destructor
         */
        ~Perlin();

        /** Sets the especified values
         	@param width Width value
         	@param height Height value
         */
        void setSize(int width, int height);

        /** Gets a bidimensional float array pointer with the object size
            @return bidimensional float array pointer with the object size
         */
        float** getFreeData();

        /** Free dinamyc memory of a bidimensional array pointer
            @param data Float pointer
            @remarks Use ONLY for data from the same object
         */
        void free(float ** data);

        /** Gets perlin noise bidimensional float pointer with the data
            @param seamless Is seamless texture? default = false
         */
        float** getData(bool seamless = false);

        /** Gets noise::utils::Image normal map with the last object configuration
            @param bumpHeight Power of the bump
            @param seamless Is seamless texture? default = false
            @remarks use getData() and just then call this funtion
         */
        noise::utils::Image Perlin::getNormalMap(double bumpHeight, bool seamless = false);

        /** Creates an Ogre texture
            @param name Texture name
            @param resourceGroup Ogre resource group, default = "Hydrax"
            @remarks It creates the texture with getData(false) data
         */
        void createPerlinTexture(Ogre::String name, Ogre::String resourceGroup = "Hydrax");

        /** Creates an Ogre texture
            @param name Texture name
            @param resourceGroup Ogre resource group, default = "Hydrax"
            @remarks It creates the texture with getNormalMap(1) data
         */
        void createNMTexture(Ogre::String name, Ogre::String resourceGroup = "Hydrax");

        /** Update our perlin noise Ogre texture
            @param data new Data to update
            @remarks data will be from this object(same size)
         */
        void updatePerlinTexture(float** data);

        /** Update our perlin noise normal map Ogre texture
            @param image new Data to update
            @remarks image will be from this object(same size)
         */
        void updateNMTexture(noise::utils::Image image);

        /// Our noise::module::Perlin for generate data
        noise::module::Perlin mPerlinModule;

        /// Our object width and height size
        Size mSize;

    private:
        /// Our Ogre tex. for perlin noise image
        Ogre::TexturePtr mTexturePerlinPtr;

        /// Our Ogre tex. for perlin noise normal map image
        Ogre::TexturePtr mTextureNMPtr;

        /// Is Ogre tex. perlin noise active?
        bool mTexturePerlinPtrActive;

        /// Is Ogre tex. perlin noise normal map active?
        bool mTextureNMPtrActive;
    };
};

#endif
