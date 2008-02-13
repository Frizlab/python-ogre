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

#ifndef _Mesh_H_
#define _Mesh_H_

#include "Prerequisites.h"

#include "Help.h"
#include "Structs.h"

namespace Hydrax
{
    /** Class wich contains all funtions/variables related to
        Hydrax water mesh
     */
    class DllExport Mesh
    {
    public:
        /** Constructor
            @param sm Ogre SceneManager
         */
        Mesh(Ogre::SceneManager *sm);

        /** Destructor
         */
        ~Mesh();

        /** Set mesh options
            @param MeshOptions MeshOptions structure
            @remarks Call before create()
         */
        void setMeshOptions(MeshOptions MeshOptions);

        /** Set mesh material
            @param MaterialName The material name
         */
        void setMaterialName(Ogre::String MaterialName);

        /** Create our water mesh, geometry, entity, etc...
            @param SceneNode Ogre SceneNode where entity will be attach
            @remarks Call it after setMeshOptions() and setMaterialName()
         */
        void create(Ogre::SceneNode *SceneNode);

        /** Update our mesh grid from a NoiseBuffer data
            @param NoiseBuffer NoiseBuffer wich contains the data
            @remarks Only call it after create() !
         */
        void update(const NoiseBuffer &NoiseBuffer);

        /** Set water strength(Y axis multiplier)
            @param Strength Strength
         */
        void setStrength(Ogre::Real Strength);

        /** Get mesh options
            @return Mesh options
         */
        inline MeshOptions getMeshOptions()
        {
            return mMeshOptions;
        }

        /** Get mesh
            @return Mesh
         */
        inline Ogre::MeshPtr getMesh()
        {
            return mMesh;
        }

        /** Get sub mesh
            @return Sub mesh
         */
        inline Ogre::SubMesh* getSubMesh()
        {
            return mSubMesh;
        }

        /** Get entity
            @return Entity
         */
        inline Ogre::Entity* getEntity()
        {
            return mEntity;
        }

        /** Get complexity
            @return Complexity
         */
        inline int getComplexity()
        {
            return mComplexity;
        }

        /** Get size
            @return Size
         */
        inline Size getSize()
        {
            return mSize;
        }

        /** Get number of faces
            @return Number of faces
         */
        inline int getNumFaces()
        {
            return mNumFaces;
        }

        /** Get number of vertices
            @return Number of vertices
         */
        inline int getNumVertices()
        {
            return mNumVertices;
        }

        /** Get material name
            @return Material name
         */
        inline Ogre::String getMaterialName()
        {
            return mMaterialName;
        }

        /** Get strength
            @return Sterngth
         */
        inline Ogre::Real getStrength()
        {
            return mStrength;
        }

    private:
        /// Mesh options
        MeshOptions mMeshOptions;
        /// Ogre::MeshPtr
        Ogre::MeshPtr mMesh;
        /// Ogre::Submesh pointer
        Ogre::SubMesh *mSubMesh;
        /// Ogre::Entity pointer
        Ogre::Entity *mEntity;
        /// Grid complexity
        int mComplexity;
        /// Grid size
        Size mSize;
        /// Number of faces
        int mNumFaces;
        /// Number of vertices
        int mNumVertices;

        /// Position vertex buffer
        Ogre::HardwareVertexBufferSharedPtr mPosVertexBuffer;
        /// Texture coordinate vertex buffer
        Ogre::HardwareVertexBufferSharedPtr mTexcoordsVertexBuffer;
        /// Index buffer
        Ogre::HardwareIndexBufferSharedPtr  mIndexBuffer;

        /// Water strength(Y axis multiplier)
        Ogre::Real mStrength;
        /// Material name
        Ogre::String mMaterialName;

        /// Pointer to Ogre::SceneManager
        Ogre::SceneManager *mSceneManager;
    };
}

#endif
