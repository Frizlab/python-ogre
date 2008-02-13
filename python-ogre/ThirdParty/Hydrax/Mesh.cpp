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

#include "Mesh.h"

namespace Hydrax
{
    Mesh::Mesh(Ogre::SceneManager *sm)
            : mSceneManager(sm)
            , mStrength(20)
            , mMesh(0)
            , mSubMesh(0)
            , mEntity(0)
            , mComplexity(0)
            , mNumFaces(0)
            , mNumVertices(0)
            , mPosVertexBuffer(0)
            , mTexcoordsVertexBuffer(0)
            , mIndexBuffer(0)
            , mMaterialName("HydraxShader1")
    {
    }

    Mesh::~Mesh()
    {
        Ogre::MeshManager::getSingleton().remove("HydraxMesh");

        mSceneManager->destroyEntity(mEntity);
    }

    void Mesh::setMeshOptions(MeshOptions MeshOptions)
    {
        mMeshOptions = MeshOptions;
    }

    void Mesh::setMaterialName(Ogre::String MaterialName)
    {
        mMaterialName = MaterialName;

        if (mEntity)
        {
            mEntity->setMaterialName(mMaterialName);
        }
    }

    void Mesh::create(Ogre::SceneNode *SceneNode)
    {
        int x,y;

        mComplexity  = mMeshOptions.Complexity;
        mSize    = mMeshOptions.MeshSize;
        mNumFaces    = 2 * mComplexity * mComplexity;
        mNumVertices = (mComplexity + 1) * (mComplexity + 1) ;

        // Create mesh and submesh
        mMesh = Ogre::MeshManager::getSingleton().createManual("HydraxMesh",
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        mSubMesh = mMesh->createSubMesh();
        mSubMesh->useSharedVertices = false;

        // Vertex buffers
        mSubMesh->vertexData = new Ogre::VertexData();
        mSubMesh->vertexData->vertexStart = 0;
        mSubMesh->vertexData->vertexCount = mNumVertices;

        Ogre::VertexDeclaration*   vdecl = mSubMesh->vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* vbind = mSubMesh->vertexData->vertexBufferBinding;

        vdecl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        vdecl->addElement(1, 0, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

        // --- Position vertices ---
        // Create vertex buffer
        mPosVertexBuffer =
            Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                3*sizeof(float),
                mNumVertices,
                Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

        // Set bingind
        vbind->setBinding(0, mPosVertexBuffer);

        // Lock vertex buffer
        float *posVertexBuffer =
            (float*)mPosVertexBuffer->
            lock(0,
                 mNumVertices,
                 Ogre::HardwareBuffer::HBL_DISCARD);

        // Set values
        for (y=0;y<=mComplexity;y++)
        {
            for (x=0;x<=mComplexity;x++)
            {
                int numPoint = y*(mComplexity+1) + x ;

                posVertexBuffer[3*numPoint+0] = (float)(x) / (float)(mComplexity) * (float) mSize.Width;
                posVertexBuffer[3*numPoint+1] = 0;
                posVertexBuffer[3*numPoint+2] = (float)(y) / (float)(mComplexity) * (float) mSize.Height;
            }
        }

        // Unlock the buffer
        mPosVertexBuffer->unlock();

        // --- Texcoords ---
        // Create vertex buffer
        mTexcoordsVertexBuffer =
            Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                2*sizeof(float),
                mNumVertices,
                Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        // Set binding
        vbind->setBinding(1, mTexcoordsVertexBuffer);

        // Lock our vertices
        float *texcoordsBufData =
            (float*)mTexcoordsVertexBuffer->
            lock(0,
                 mNumVertices,
                 Ogre::HardwareBuffer::HBL_DISCARD);
        // Asign values
        for (y=0;y<=mComplexity;y++)
        {
            for (x=0;x<=mComplexity;x++)
            {
                texcoordsBufData[2*(y*(mComplexity+1)+x)+0] = (float)y / mComplexity ;
                texcoordsBufData[2*(y*(mComplexity+1)+x)+1] = (float)x / mComplexity ;
            }
        }

        // Unlock
        mTexcoordsVertexBuffer->unlock();

        // --- Index buffer ---
        // Find what we need, 16 or 32 bit buffer
        bool is32Bits = false;
        Ogre::HardwareIndexBuffer::IndexType IT_BIT = Ogre::HardwareIndexBuffer::IT_16BIT;

        if (mComplexity > 255)
        {
            is32Bits = true;
            IT_BIT = Ogre::HardwareIndexBuffer::IT_32BIT;
        }

        // Create index buffer
        mIndexBuffer =
            Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
                IT_BIT,
                3*mNumFaces,
                Ogre::HardwareBuffer::HBU_STATIC, true);

        // Lock
        unsigned short *faceVertexIndices16Bits;
        unsigned long  *faceVertexIndices32Bits;

        if (!is32Bits)
        {
            // 16 Bit buffer
            faceVertexIndices16Bits =
                (unsigned short*)mIndexBuffer->
                lock(0,
                     mNumFaces*3*2,
                     Ogre::HardwareBuffer::HBL_DISCARD);
        }
        else
        {
            // 32 bits buffer
            faceVertexIndices32Bits =
                (unsigned long*)mIndexBuffer->
                lock(0,
                     mNumFaces*3*2,
                     Ogre::HardwareBuffer::HBL_DISCARD);
        }

        // Set values
        unsigned short *twoface16Bits;
        unsigned long  *twoface32Bits;

        for (y=0 ; y<mComplexity ; y++)
        {
            for (int x=0 ; x<mComplexity ; x++)
            {
                int p0 = y*(mComplexity+1) + x ;
                int p1 = y*(mComplexity+1) + x + 1 ;
                int p2 = (y+1)*(mComplexity+1) + x ;
                int p3 = (y+1)*(mComplexity+1) + x + 1 ;

                if (!is32Bits)
                {
                    // 16 bits buffer
                    twoface16Bits = faceVertexIndices16Bits + (y*mComplexity+x)*2*3;

                    // First triangle
                    twoface16Bits[0]=p2;
                    twoface16Bits[1]=p1;
                    twoface16Bits[2]=p0;
                    // Second triangle
                    twoface16Bits[3]=p2;
                    twoface16Bits[4]=p3;
                    twoface16Bits[5]=p1;
                }
                else
                {
                    // 32 bits buffer
                    twoface32Bits = faceVertexIndices32Bits + (y*mComplexity+x)*2*3;

                    // First triangle
                    twoface32Bits[0]=p2;
                    twoface32Bits[1]=p1;
                    twoface32Bits[2]=p0;
                    // Second triangle
                    twoface32Bits[3]=p2;
                    twoface32Bits[4]=p3;
                    twoface32Bits[5]=p1;
                }
            }
        }

        // Unlock
        mIndexBuffer->unlock();

        // Set index buffer for this submesh
        mSubMesh->indexData->indexBuffer = mIndexBuffer;
        mSubMesh->indexData->indexStart = 0;
        mSubMesh->indexData->indexCount = 3*mNumFaces;

        // End mesh creation
        Ogre::AxisAlignedBox meshBounds(0,0,0,
                                        mSize.Width, mStrength, mSize.Height);

        mMesh->_setBounds(meshBounds, false);
        mMesh->_setBoundingSphereRadius (1);
        mMesh->load();
        mMesh->touch();

        mEntity = mSceneManager->createEntity("HydraxMeshEnt", "HydraxMesh");
        mEntity->setMaterialName(mMaterialName);
        mEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
        Ogre::MaterialPtr m = Ogre::MaterialManager::getSingleton().getByName(mMaterialName);
        m->setReceiveShadows(false);

        SceneNode->attachObject(mEntity);
        SceneNode->setPosition(-mSize.Width/2,0,-mSize.Height/2);
    }

    void Mesh::update(const NoiseBuffer &NoiseBuffer)
    {
        float *posVertexBuffer =
            (float*)mPosVertexBuffer->
            lock(0,
                 mNumVertices,
                 Ogre::HardwareBuffer::HBL_NORMAL);

        int x, y, xcoord, ycoord;
        float* row;

        float *buf = posVertexBuffer+1 ; // +1 for Y coordinate

        for (y=1;y<=mComplexity-1;y++)
        {
            ycoord = (int)(((float)y/mComplexity)*NoiseBuffer.mSize.Height);

            row = buf + 3*y*(mComplexity+1);

            for (x=1;x<=mComplexity-1;x++)
            {
                xcoord = (int)(((float)x/mComplexity)*NoiseBuffer.mSize.Width);

                row[3*x] = NoiseBuffer.mData[xcoord][ycoord]*mStrength;
            }
        }

        mPosVertexBuffer->unlock();
    }

    void Mesh::setStrength(Ogre::Real Strength)
    {
        mStrength = Strength;

        Ogre::AxisAlignedBox meshBounds(0,0,0,
                                        mSize.Width, mStrength, mSize.Height);

        mMesh->_setBounds (meshBounds, false);
    }
}
