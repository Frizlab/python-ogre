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

#ifndef _Structs_H_
#define _Structs_H_

#include "Enums.h"
#include "Help.h"

namespace Hydrax
{
    /** Struct wich contains Hydrax perlin options
     */
    struct DllExport PerlinOptions
    {
        /// Our height and normal map texture sizes
        TextureQuality TexQuality;
        /// Perlin noise frecuency value
        double Frecuency;
        /// Perlin noise persistence value
        double Persistence;
        /// Perlin noise octave value
        int Octave;
        /// Perlin noise lacunarity value
        double Lacunarity;
        /// Normal map height value
        double NMHeight;

        /** Default constructor
         */
        PerlinOptions()
            : TexQuality(TEX_QUA_128)
            , Frecuency(5)
            , Persistence(0.7)
            , Octave(1)
            , Lacunarity(1)
            , NMHeight(1)
        {
        }

        /** Constructor
            @param _TextureQuality Height and normal map texture sizes
            @param _Frecuency Perlin noise frecuency value
            @param _Persistence Perlin noise persistence value
            @param _Octave Perlin noise octave value
            @param _Lacunarity Perlin noise lacunarity value
            @param _NMHeight Normal map height value
         */
        PerlinOptions(TextureQuality  _TextureQuality,
                      double          _Frecuency,
                      double          _Persistence,
                      int             _Octave,
                      double          _Lacunarity,
                      double          _NMHeight)

            : TexQuality(_TextureQuality)
            , Frecuency(_Frecuency)
            , Persistence(_Persistence)
            , Octave(_Octave)
            , Lacunarity(_Lacunarity)
            , NMHeight(_NMHeight)
        {
        }
    };

    /** Struct wich contains Hydrax mesh options
     */
    struct DllExport MeshOptions
    {
        /// Water size
        Size MeshSize;
        /// N*N mesh grid complexity
        int Complexity;

        /** Default constructor
         */
        MeshOptions()
            : MeshSize(Size(256, 256))
            , Complexity(64)
        {
        }

        /** Constructor
            @param _MeshSize Water size
            @param _Complexity N*N mesh grid complexity
         */
        MeshOptions(Size  _MeshSize,
                    int   _Complexity)

            : MeshSize(_MeshSize)
            , Complexity(_Complexity)
        {
        }
    };

    /** Struct wich contains Rtts quality
     */
    struct DllExport RttOptions
    {
        /// Reflection texture quality
        TextureQuality ReflectionQuality;
        /// Refraction texture quality
        TextureQuality RefractionQuality;
        /// Depth texture quality
        TextureQuality DepthQuality;

        /** Default constructor
         */
        RttOptions()
            : ReflectionQuality(TEX_QUA_512)
            , RefractionQuality(TEX_QUA_512)
            , DepthQuality(TEX_QUA_512)
        {
        }

        /** Constructor
            @param _ReflectionQuality Reflection texture quality
            @param _RefractionQuality Refraction texture quality
            @param _DepthQuality N*N Depth texture quality
         */
        RttOptions(TextureQuality  _ReflectionQuality,
                   TextureQuality  _RefractionQuality,
                   TextureQuality  _DepthQuality)

            : ReflectionQuality(_ReflectionQuality)
            , RefractionQuality(_RefractionQuality)
            , DepthQuality(_DepthQuality)
        {
        }
    };
}

#endif
