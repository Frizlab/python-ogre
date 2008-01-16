//
//	NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org
//
//	This library is free software; you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation; either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library; if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "NxOgreStable.h"
#include "NxOgreFluidMesh.h"

#if (NX_USE_FLUID_API == 1)
#include "NxOgrePose.h"					// For conversions
#include "NxOgreHelpers.h"				// For conversions
#include "NxOgreScene.h"				// For Actor::mOwner
#include "NxOgreFluid.h"				// For Fluid

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

FluidMesh::FluidMesh(Fluid* fluid, Scene* scene, Ogre::Camera* camera) : mFluid(fluid), mOwner(scene), mCamera(camera) {
	mNxFluid = mFluid->getNxFluid();
	__createMesh();
}

FluidMesh::~FluidMesh() {
	mNxFluid->releaseScreenSurfaceMesh(*mFluidMesh);
	// Kill Entity and Node here.

	// Destroy Mesh.
}

void FluidMesh::__createMesh() {
	NxImplicitScreenMeshDesc desc;
	desc.setToDefault();
	desc.width = 512;
	desc.height = 512;
		
	NxReal* ProjMatrix = new NxReal[16];

	Ogre::Matrix4 m = mCamera->getViewMatrix();
/*
		<pre>
M =	| 0  4  8  12 |
	| 1  5  9  13 |
	| 2  6 10  14 |
	| 3  7 11  15 |</pre>
*/
	ProjMatrix[0] = 1.732051;
	ProjMatrix[1] = 0;
	ProjMatrix[2] = 0;
	ProjMatrix[3] = 0;
	ProjMatrix[4] = 0;
	ProjMatrix[5] = 1.732051;
	ProjMatrix[6] = 0;
	ProjMatrix[7] = 0;
	ProjMatrix[8] = 0;
	ProjMatrix[9] = 0;
	ProjMatrix[10] = -1.0002;
	ProjMatrix[11] = -1;
	ProjMatrix[12] = 0;
	ProjMatrix[13] = -4.330127;
	ProjMatrix[14] = 13.002800;
	ProjMatrix[15] = 15.0;
	/*
	ProjMatrix[0] = m[0][0];
	ProjMatrix[4] = m[0][1];
	ProjMatrix[8] = m[0][2];
	ProjMatrix[12] = m[0][3];
	ProjMatrix[1] = m[1][0];
	ProjMatrix[5] = m[1][1];
	ProjMatrix[9] = m[1][2];
	ProjMatrix[13] = m[1][3];
	ProjMatrix[2] = m[2][0];
	ProjMatrix[6] = m[2][1];
	ProjMatrix[10] = m[2][2];
	ProjMatrix[14] = m[2][3];
	ProjMatrix[3] = m[3][0];
	ProjMatrix[7] = m[3][1];
	ProjMatrix[11] = m[3][2];
	ProjMatrix[15] = m[3][3];*/

	/*
	int i = 0;
	for (int x=0;x < 4;x++) {
		for (int y=0;y < 4;y++) {
			ProjMatrix[i] = m[x][y];
			i++;
		}
	}*/

	/*

        dest = matrix;

        // Convert depth range from [-1,+1] to [0,1]
        dest[2][0] = (dest[2][0] + dest[3][0]) / 2;
        dest[2][1] = (dest[2][1] + dest[3][1]) / 2;
        dest[2][2] = (dest[2][2] + dest[3][2]) / 2;
        dest[2][3] = (dest[2][3] + dest[3][3]) / 2;

	*/

	/*
	// Don't ask...
	int i = 0;
	for (int x=0;x < 4;x++) {
		for (int y=0;y < 4;y++) {
			ProjMatrix[i] = m[x][y];
			i++;
		}
	}*/


	desc.projectionMatrix = ProjMatrix;


	mVertices = new NxVec3[CN_MAX_VERTS_AND_INDICES];
	mNormals = new NxVec3[CN_MAX_VERTS_AND_INDICES];
	mIndices = new NxU32[CN_MAX_VERTS_AND_INDICES];
	mNbVertices = 1337;
	mNbIndices = 1337;

	desc.meshData.verticesPosBegin = mVertices;
	desc.meshData.verticesPosByteStride = sizeof(NxVec3);
	desc.meshData.numVerticesPtr = &mNbVertices;
	desc.meshData.maxVertices = CN_MAX_VERTS_AND_INDICES;
   	
	desc.meshData.verticesNormalBegin = mNormals;
	desc.meshData.verticesNormalByteStride = sizeof(NxVec3);

    desc.meshData.indicesBegin = mIndices;
    desc.meshData.indicesByteStride = sizeof(NxU32);
    desc.meshData.maxIndices = CN_MAX_VERTS_AND_INDICES;
    desc.meshData.numIndicesPtr = &mNbIndices;

	desc.particleRadius = 1.0f;
	desc.screenSpacing = 5;
    desc.triangleWindingPositive = 1;
    desc.threshold = 0.5f;
	//desc.particleRadius = 0.1f;

	mFluidMesh = mNxFluid->createScreenSurfaceMesh(desc);

}

void FluidMesh::__createOgreMesh() {
	
	if (mFluidMesh->generateMeshIsDone()) {
		std::cout << "-Done" << std::endl;
	}
	else {
		std::cout << "-Not Done" << std::endl;
	}

	std::cout << "Waiting for mesh." << std::endl;
	
	if (!mFluidMesh->getMesh())
		std::cout << "..get mesh failed" << std::endl;
	
	std::cout << "Indices = " << mNbIndices << std::endl;
	std::cout << "Vertices = " << mNbVertices << std::endl;
	
	std::cout << (*mFluidMesh->getMeshData().numIndicesPtr) << std::endl;
	if (mNbVertices == 0)
		return;
	
	std::cout << "a" << std::endl;

	mMesh = Ogre::MeshManager::getSingleton().createManual(mFluid->getName() + ".fm", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::HardwareBuffer::Usage HWUsage = Ogre::HardwareBuffer::HBU_STATIC; 
std::cout << "b" << std::endl;

	Ogre::HardwareIndexBufferSharedPtr indexHardBuffer = 
                    Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
                    Ogre::HardwareIndexBuffer::IT_32BIT, 
					(size_t) mNbIndices, 
                    HWUsage, 
                    false);

std::cout << "c" << std::endl;

	indexHardBuffer->writeData(0, sizeof(NxU32)*mNbIndices, mIndices, true);
std::cout << "d" << std::endl;

	mIndexData = new Ogre::IndexData;
    mIndexData->indexStart = 0;
	mIndexData->indexCount = mNbIndices;
    mIndexData->indexBuffer = indexHardBuffer;
std::cout << "e" << std::endl;

	//Init vertex buffer they will change size if cloth is tearable
    mVertexData= new Ogre::VertexData;
    mVertexData->vertexStart = 0;
	mVertexData->vertexCount = mNbVertices;
std::cout << "f" << std::endl;

	Ogre::VertexDeclaration* decl = mVertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = mVertexData->vertexBufferBinding;
std::cout << "g" << std::endl;

	// #define VERTEX_BINDING 0
	// #define NORMAL_BINDING 1
	// #define TEXCOORDBINDING 2
	// #define TEAR_MEMORY_FACTOR 2

	decl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    decl->addElement(1, 0, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
std::cout << "h" << std::endl;

    Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;
    Ogre::HardwareVertexBufferSharedPtr mNormalBuffer;
std::cout << "i" << std::endl;

	// Create shared vertex buffer
    mVertexBuffer =
        Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        sizeof(NxVec3),//decl->getVertexSize(VERTEX_BINDING),
		mNbVertices, 
        //HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
        Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
        false); // use shadow buffer
    bind->setBinding(0, mVertexBuffer);
std::cout << "j" << std::endl;

    // Prepare buffer for mNormals - write only
    mNormalBuffer = 
        Ogre::HardwareBufferManager::getSingleton().createVertexBuffer( 
        sizeof(NxVec3),//decl->getVertexSize(NORMAL_BINDING), 
		mNbVertices, 
        Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
        false); // use shadow buffer
    bind->setBinding(1, mNormalBuffer);
std::cout << "k" << std::endl;

	size_t mVec3Buffsize = mNbVertices * sizeof(NxVec3);

std::cout << "l" << std::endl;

	// and bring it to hardware
    mVertexBuffer->writeData(0, 
        mVec3Buffsize, // size 
        mVertices, // source
        true); // discard?
std::cout << "m" << std::endl;

    mNormalBuffer->writeData(0, 
        mVec3Buffsize, // size 
        mNormals, // source
        true); // discard?
std::cout << "n" << std::endl;

	mSubMesh = mMesh->createSubMesh();
    mSubMesh->useSharedVertices = false;
    mSubMesh->vertexData = mVertexData;
    mSubMesh->indexData = mIndexData;
std::cout << "o" << std::endl;

    mSubMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

	std::cout << "p" << std::endl;

	//mSubMesh->setMaterialName(

	// note: this box will not be updated for performs reasons
	// if you would like to update it you have to do it in draw method 
	// during copying the vertexes
std::cout << "q" << std::endl;

	Ogre::AxisAlignedBox aa = Ogre::AxisAlignedBox();
	aa.setMaximum(Ogre::Vector3(1000,1000,1000));
	aa.setMinimum(Ogre::Vector3(-1000,-1000,-1000));
std::cout << "r" << std::endl;

	mMesh->_setBounds(aa);    
std::cout << "s" << std::endl;

    mMesh->load();
    mMesh->touch();
	std::cout << "t" << std::endl;

}

void FluidMesh::__addToScene() {
	std::cout << "v" << std::endl;

	mNode = mOwner->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	mEntity = mOwner->getSceneManager()->createEntity(mFluid->getName(), mMesh->getName());
	mNode->attachObject(mEntity);
	std::cout << "z" << std::endl;

}

void FluidMesh::render(NxReal) {

}

void FluidMesh::__preRender() {
	

	NxReal		ProjMatrix[16];

	Ogre::Matrix4 m = mCamera->getProjectionMatrix() * mCamera->getViewMatrix();
	
	ProjMatrix[0] = 1.732051;
	ProjMatrix[1] = 0;
	ProjMatrix[2] = 0;
	ProjMatrix[3] = 0;
	ProjMatrix[4] = 0;
	ProjMatrix[5] = 1.732051;
	ProjMatrix[6] = 0;
	ProjMatrix[7] = 0;
	ProjMatrix[8] = 0;
	ProjMatrix[9] = 0;
	ProjMatrix[10] = -1.0002;
	ProjMatrix[11] = -1;
	ProjMatrix[12] = 0;
	ProjMatrix[13] = -4.330127;
	ProjMatrix[14] = 13.002800;
	ProjMatrix[15] = 15.0;

	mFluidMesh->setProjectionMatrix(ProjMatrix);


}

//////////////////////////////////////////////////////////

}  //End of NxOgre namespace.
#endif
