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
#include "NxOgreCloth.h"

//#if (NX_USE_CLOTH_API == 1)
#include "NxOgrePose.h"					// For conversions
#include "NxOgreHelpers.h"				// For conversions
#include "NxOgreScene.h"				// For Cloth::mOwner
#include "NxOgreCooking.h"				// For Cooking NxClothMesh and Ogre::Mesh
#include "NxOgreUserStream.h"
#include "NxCooking.h"
namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////


void ClothParams::setToDefault() {
	
	width = 1.0f;
	height = 1.0f;
	vertexDistance = 0.25f;
	meshMaterial = "";

	thickness = 0.01f;
	density = 1.0f;
	bendingStiffness = 1.0f;
	stretchingStiffness = 1.0f;
    dampingCoefficient = 0.5f;
	friction = 0.5f;
	pressure = 1.0f;
	tearFactor = 1.5f;
	attachmentTearFactor = 1.5f;
	attachmentResponseCoefficient = 0.2f;
	collisionResponseCoefficient = 0.2f;
	toFluidResponseCoefficient = 1.0f;
	fromFluidResponseCoefficient = 1.0f;
	flags = NX_CLF_GRAVITY;
    solverIterations = 5;
	wakeUpCounter = NX_SLEEP_INTERVAL;
	sleepLinearVelocity = -1.0f;
	collisionGroup = 0;
	externalAcceleration.set(0.0f, 0.0f, 0.0f);
	groupsMask.bits0 = 0;
	groupsMask.bits1 = 0;
	groupsMask.bits2 = 0;
	groupsMask.bits3 = 0;
	validBounds.setEmpty();

	tear = false;
	bending = true;
	bending_ortho = false;
	gravity = true;
	collision_self = false;
	collision_twoway = false;
	is_static = false;
	pressurised = false;
	tear_buffer_multiplier = 2;

}

////////////////////////////////////////////////////////////////////////////////////////////////

void ClothParams::parse(Parameters P) {
	for (Parameters::iterator p = P.begin(); p != P.end();p++) {
		if (Set("mesh-material", (*p), meshMaterial)) continue;
		if (Set("width", (*p), width)) continue;
		if (Set("height", (*p), height)) continue;
		if (Set("vertex-distance", (*p), vertexDistance)) continue;
		if (Set("thickness", (*p), thickness)) continue;
		if (Set("density", (*p), density)) continue;
		if (Set("bending-stiffness", (*p), bendingStiffness)) continue;
		if (Set("stretching-stiffness", (*p), stretchingStiffness)) continue;
		if (Set("damping-coefficient", (*p), dampingCoefficient)) continue;
		if (Set("friction", (*p), friction)) continue;
		if (Set("pressure", (*p), pressure)) continue;
		if (Set("tear-factor", (*p), tearFactor)) continue;
		if (Set("attachment-tear-factor", (*p), attachmentTearFactor)) continue;
		if (Set("attachment-response-coefficient", (*p), attachmentResponseCoefficient)) continue;
		if (Set("collision-response-coefficient", (*p), collisionResponseCoefficient)) continue;
		if (Set("to-fluid-response-coefficient", (*p), toFluidResponseCoefficient)) continue;
		if (Set("from-fluid-response-coefficient", (*p), fromFluidResponseCoefficient)) continue;
		if (Set("solver-iterations", (*p), solverIterations)) continue;
		if (Set("wakeup-counter", (*p), wakeUpCounter)) continue;
		if (Set("sleep-linear-velocity", (*p), sleepLinearVelocity)) continue;
//		if (Set("collision-group", (*p), collisionGroup)) continue;
		if (Set("external-acceleration", (*p), externalAcceleration)) continue;
		if (Set("tear", (*p), tear)) continue;
		if (Set("gravity", (*p), gravity)) continue;
		if (Set("collision-twoway", (*p), collision_twoway)) continue;
		if (Set("collision-self", (*p), collision_self)) continue;
		if (Set("static", (*p), is_static)) continue;
		if (Set("pressurised", (*p), pressurised)) continue;
		if (Set("tear-buffer-multiplier", (*p), tear_buffer_multiplier)) continue;
		
		if ((*p).first == "bending") {
			if ((*p).second.substr(0,1) == "y" || (*p).second.substr(0,1) == "Y") {
				bending = true;
				continue;
			}
			else if ((*p).second.substr(0,1) == "o" || (*p).second.substr(0,1) == "O") {
				bending = true;
				bending_ortho = true;
				continue;
			}
		}

	}
}

//////////////////////////////////////////////////////////

Cloth::Cloth(const NxString& identifier, Scene* scene, const Pose& pose, ClothParams params) : mName(identifier), mOwner(scene), mUpdateBB(true) {
	__ParamsToDescription(params, pose);
	mClothDescription.clothMesh = __createClothMesh(params.width,params.height,params.vertexDistance);
	__createCloth(params);
	mOwner->_registerCloth(mName, this);
}

//////////////////////////////////////////////////////////

Cloth::Cloth(const NxString& identifier, const NxString& meshName, Scene* scene, const Pose& pose, ClothParams params) : mName(identifier), mOwner(scene), mUpdateBB(true) {
	__ParamsToDescription(params, pose);
	mClothDescription.clothMesh = __createClothMeshFromOgreMesh(mName, meshName);
	__createCloth(params);
	mOwner->_registerCloth(mName, this);
}

//////////////////////////////////////////////////////////

#if 0
// Streaming saved cloth meshes disabled until it's feasible to load in texture and uv data from the same file.
Cloth::Cloth(const NxString& NXS_Filename, ClothParams params) {
	__ParamsToDescription(params, pose);
	
	UserStream rbuf(fileName, true);
	mClothMesh = mOwner->getNxScene()->getPhysicsSDK().createClothMesh(rbuf);
	fclose(rbuf.fp);

	__createCloth(params, pose, meshName);
	
}
#endif

//////////////////////////////////////////////////////////

Cloth::~Cloth() {
	mOwner->_unregisterCloth(mName);
	__destroyCloth();
}

//////////////////////////////////////////////////////////

NxScene* Cloth::getNxScene() {
	return mOwner->getNxScene();
}

//////////////////////////////////////////////////////////

void Cloth::__ParamsToDescription(ClothParams params, const Pose& pose) {
	mClothDescription.setToDefault();
	mClothDescription.attachmentResponseCoefficient = params.attachmentResponseCoefficient;
	mClothDescription.attachmentTearFactor = params.attachmentTearFactor;
	mClothDescription.bendingStiffness = params.bendingStiffness;
	mClothDescription.collisionGroup = params.collisionGroup;
	mClothDescription.collisionResponseCoefficient = params.collisionResponseCoefficient;
	//mClothDescription.flags = params.flags;
	mClothDescription.friction = params.friction;
	mClothDescription.dampingCoefficient = params.dampingCoefficient;
	mClothDescription.density = params.density;
	mClothDescription.externalAcceleration = params.externalAcceleration;
	mClothDescription.fromFluidResponseCoefficient = params.fromFluidResponseCoefficient;
	mClothDescription.groupsMask = params.groupsMask;
	mClothDescription.pressure = params.pressure;
	mClothDescription.sleepLinearVelocity = params.sleepLinearVelocity;
	mClothDescription.solverIterations = params.solverIterations;
	mClothDescription.stretchingStiffness = params.stretchingStiffness;
	mClothDescription.tearFactor = params.tearFactor;
	mClothDescription.thickness = params.thickness;
	mClothDescription.toFluidResponseCoefficient = params.toFluidResponseCoefficient;
	mClothDescription.validBounds = params.validBounds;
	mClothDescription.wakeUpCounter = params.wakeUpCounter;
	mClothDescription.globalPose.t = pose;
	mClothDescription.globalPose.M.fromQuat(pose);

	mClothDescription.flags = 0;
	
	if (params.gravity) 
		mClothDescription.flags |= NX_CLF_GRAVITY;

	if (params.bending)
		mClothDescription.flags |= NX_CLF_BENDING;

	if (params.bending_ortho)
		mClothDescription.flags |= NX_CLF_BENDING_ORTHO;

	if (params.collision_twoway)
		mClothDescription.flags |= NX_CLF_COLLISION_TWOWAY;

	if (params.collision_self)
		mClothDescription.flags |= NX_CLF_SELFCOLLISION;

	if (params.is_static)
		mClothDescription.flags |= NX_CLF_STATIC;

	if (params.pressurised)
		mClothDescription.flags |= NX_CLF_PRESSURE;

	mIsTearable = false;
	mTearFactor = params.tear_buffer_multiplier;

	if (params.tear) {
		mClothDescription.flags |= NX_CLF_TEARABLE;
		mIsTearable=true;
	}

}

//////////////////////////////////////////////////////////

void Cloth::__createCloth(ClothParams params) {

	NxU32 maxVertices = mVertexCount * mTearFactor;
	
	mReceiveBuffers.verticesPosBegin = (NxVec3*)malloc(sizeof(NxVec3)*maxVertices);
	mReceiveBuffers.verticesNormalBegin = (NxVec3*)malloc(sizeof(NxVec3)*maxVertices);
	mReceiveBuffers.verticesPosByteStride = sizeof(NxVec3);
	mReceiveBuffers.verticesNormalByteStride = sizeof(NxVec3);
	mReceiveBuffers.maxVertices = maxVertices;
	mReceiveBuffers.numVerticesPtr = (NxU32*)malloc(sizeof(NxU32));
	
	// the number of triangles is constant, even if the cloth is torn
	NxU32 maxIndices = 3 * mTriangleCount;
	mReceiveBuffers.indicesBegin = (NxU32*)malloc(sizeof(NxU32)*maxIndices);
	mReceiveBuffers.indicesByteStride = sizeof(NxU32);
	mReceiveBuffers.maxIndices = maxIndices;
	mReceiveBuffers.numIndicesPtr = (NxU32*)malloc(sizeof(NxU32));

	// the parent index information would be needed if we used textured cloth
	NxU32 maxParentIndices = maxVertices;
	mReceiveBuffers.parentIndicesBegin = (NxU32*)malloc(sizeof(NxU32)*maxParentIndices);
	mReceiveBuffers.parentIndicesByteStride = sizeof(NxU32);
	mReceiveBuffers.maxParentIndices = maxParentIndices;
	mReceiveBuffers.numParentIndicesPtr = (NxU32*)malloc(sizeof(NxU32));

	// init the buffers in case we want to draw the mesh 
	// before the SDK as filled in the correct values
	*mReceiveBuffers.numVerticesPtr = 0;
	*mReceiveBuffers.numIndicesPtr = 0;
	mClothDescription.meshData = mReceiveBuffers;
	
	if (!mClothDescription.isValid())
		NxThrow_Error("Cloth description is invalid");
	mCloth = mOwner->getNxScene()->createCloth(mClothDescription);
	

	if (!mCloth)
		NxThrow_Error("Cloth failed to be created.");

	__createMesh(mName);
	
	if (params.meshMaterial.length() > 0)
		mSubMesh->setMaterialName(params.meshMaterial);


	mNode = mOwner->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	mEntity = mOwner->getSceneManager()->createEntity(mName, mMesh->getName());
	mNode->attachObject(mEntity);

}

//////////////////////////////////////////////////////////

void Cloth::__destroyCloth() {

	///////////////////////////////


	mOwner->getNxScene()->releaseCloth(*mCloth);
	mOwner->getNxScene()->getPhysicsSDK().releaseClothMesh(*mClothMesh);
	
	///////////////////////////////

	NxVec3* vp;
	NxU32* up; 
	vp = (NxVec3*)mReceiveBuffers.verticesPosBegin; free(vp);
	vp = (NxVec3*)mReceiveBuffers.verticesNormalBegin; free(vp);
	up = (NxU32*)mReceiveBuffers.numVerticesPtr; free(up);

	up = (NxU32*)mReceiveBuffers.indicesBegin; free(up);
	up = (NxU32*)mReceiveBuffers.numIndicesPtr; free(up);

	up = (NxU32*)mReceiveBuffers.parentIndicesBegin; free(up);
	up = (NxU32*)mReceiveBuffers.numParentIndicesPtr; free(up);

	///////////////////////////////

	if (mNode) {

		while (mNode->numAttachedObjects() > 0) {
			Ogre::Entity* ent = (Ogre::Entity*) mNode->getAttachedObject(0);
			mOwner->getSceneManager()->destroyEntity(ent);
		}

		mNode->detachAllObjects();
		mNode->removeAndDestroyAllChildren();

		mOwner->getSceneManager()->destroySceneNode(mNode->getName());
	}

	mMesh->unload();
}

//////////////////////////////////////////////////////////

NxClothMesh* Cloth::__createClothMeshFromOgreMesh(const NxString& name, const NxString& meshName) {

	NxClothMeshDesc desc;
	desc.setToDefault();
	mVertexCount = 0;
	mTriangleCount = 0;
	mTextCoordCount = 0;

	bool added_shared = false;

	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;


	Ogre::Root::getSingleton().getMeshManager()->load(meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::Mesh* mesh = reinterpret_cast<Ogre::Mesh *>(Ogre::Root::getSingleton().getMeshManager()->getByName(meshName).get());


	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
		Ogre::SubMesh* submesh = mesh->getSubMesh( i );
		if(submesh->useSharedVertices) {
			if( !added_shared ) {
				mVertexCount += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else {
			mVertexCount += submesh->vertexData->vertexCount;
		}

		mTriangleCount += submesh->indexData->indexCount;
	}

	mVertices  = new NxVec3[mVertexCount];
	mTriangles = new NxU32[mTriangleCount * 3];
	mNormals = new Ogre::Vector3[mVertexCount];
	mTextCoords = new Ogre::Vector2[mVertexCount];

	added_shared = false;
	bool use32bitindexes;
	
	NxVec3 *p = mVertices;
	Ogre::Vector3 *n = mNormals;
	Ogre::Vector2 *t = mTextCoords;

	for (unsigned short i=0;i<mesh->getNumSubMeshes();++i) {

		Ogre::SubMesh* submesh = mesh->getSubMesh(i);
		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;


		if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared)) {

			if(submesh->useSharedVertices) {
				added_shared = true;
				shared_offset = current_offset;
			}
		
			///////////////////// Vertex
			{
				const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
				Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
				unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
				Ogre::Real* pReal;

				for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) {
					posElem->baseVertexPointerToElement(vertex, &pReal);
					p->set(pReal[0], pReal[1], pReal[2]); 
					*p++;
				}
				vbuf->unlock();
			}
			///////////////////// Normals
			{
				const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_NORMAL);
				Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
				unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			
				Ogre::Real* pReal;

				for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) {
					posElem->baseVertexPointerToElement(vertex, &pReal);
					*n = Ogre::Vector3(pReal[0], pReal[1], pReal[2]); 
					*n++;
				}

				vbuf->unlock();
			}

			////////////////////// Texture Coords
			{
				const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);
				Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
				unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

				Ogre::Real* pReal;

				for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) {
					posElem->baseVertexPointerToElement(vertex, &pReal);
					*t = Ogre::Vector2(pReal[0], pReal[1]); 
					*t++;
				}
				
				vbuf->unlock();
			}

			next_offset += vertex_data->vertexCount;

		}
		
		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
		use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		if ( use32bitindexes )	{
			unsigned int*  pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
			for ( size_t k = 0; k < mTriangleCount; ++k) {
				mTriangles[index_offset++] = pInt[k] + static_cast<unsigned int>(offset);			
			}
		}
		else {

			unsigned short* pShort = reinterpret_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

			for ( size_t k = 0; k < mTriangleCount; ++k) {

				mTriangles[index_offset++] = static_cast<unsigned int>(pShort[k]) + static_cast<unsigned int>(offset);

			}

		}


		ibuf->unlock();
		current_offset = next_offset;
	}

	// Safe-Vertices
	NxVec3* mSafeVertices = new NxVec3[mVertexCount];
	Ogre::Vector2* mSafeTextCoords = new Ogre::Vector2[mVertexCount];
	Ogre::Vector3* mSafeNormals = new Ogre::Vector3[mVertexCount];
	NxU32 mSafeVertexCount = 0;
	NxU32 mSafeTextCount = 0;
	NxU32 mSafeNormalCount = 0;

	{
		NxVec3 vertex;
		Ogre::Vector3 normal;
		Ogre::Vector2 texture;
		bool d=false;
		for (NxU32 i=0;i < mVertexCount;++i) {
			vertex = mVertices[i];
			texture = mTextCoords[i];
			normal = mNormals[i];
			d = false;
			for (NxU32 j=0;j < mSafeVertexCount;++j) {
				if ((vertex == mSafeVertices[j])) {
					texture = mTextCoords[j];
					normal = mNormals[j];
					d = true;
				}
			}
			if (!d)	{
				mSafeVertices[mSafeVertexCount++] = vertex;
				mSafeTextCoords[mSafeTextCount++] = texture;
				mSafeNormals[mSafeNormalCount++] = normal;
			}
		}
	}

	// Safe-Triangles
	NxU32* mSafeTriangles = new NxU32[mTriangleCount];
	NxU32  mSafeTriangleCount = 0;
	{
		NxVec3 vertex;
		for (NxU32 i=0;i < mTriangleCount;++i) {
			vertex = mVertices[mTriangles[i]];
			for (NxU32 j=0;j < mSafeVertexCount;++j) {
				if (vertex == mSafeVertices[j]) {
					mSafeTriangles[mSafeTriangleCount++] = j;
				}
			}
		}
	}

	
	delete []mTriangles;
	delete []mVertices;
	delete []mTextCoords;
	delete []mNormals;

	mTriangles = new NxU32[mSafeTriangleCount];
	mVertices = new NxVec3[mSafeVertexCount];
	mTextCoords = new Ogre::Vector2[mSafeTextCount];
	mNormals = new Ogre::Vector3[mSafeNormalCount];

	memcpy(mVertices, mSafeVertices, mSafeVertexCount * sizeof(NxVec3));
	memcpy(mTriangles, mSafeTriangles, mSafeTriangleCount * sizeof(NxU32));
	memcpy(mTextCoords, mSafeTextCoords, mSafeTextCount * sizeof(Ogre::Vector2));
	memcpy(mNormals, mSafeNormals, mSafeNormalCount * sizeof(NxVec3));

	delete []mSafeTriangles;
	delete []mSafeVertices;
	delete []mSafeTextCoords;
	delete []mSafeNormals;

	mVertexCount = mSafeVertexCount;
	mTriangleCount = mSafeTriangleCount;
	mTextCoordCount = mSafeTextCount;
	mNormalCount = mSafeNormalCount;
	

	desc.numVertices = mVertexCount;
	desc.numTriangles = mTriangleCount / 3;
	desc.pointStrideBytes			= sizeof(NxVec3);
	desc.triangleStrideBytes		= 3*sizeof(NxU32);
	desc.vertexMassStrideBytes		= sizeof(NxReal);
	desc.vertexFlagStrideBytes		= sizeof(NxU32);
	desc.points = mVertices;
	desc.triangles = mTriangles;
	desc.vertexMasses            = 0;
	desc.vertexFlags            = 0;

#ifndef NX_DEBUG

	MemoryWriteBuffer buf;
	if (!NxCookClothMesh(desc, buf)) {
		std::stringstream s;
		s << "Cloth Mesh failed to cook";

		NxThrow_Error(s.str());
	}

	mClothMesh = mOwner->getNxScene()->getPhysicsSDK().createClothMesh(MemoryReadBuffer(buf.data));

#else

	NxString filename;
	if (Ogre::StringUtil::endsWith(meshName, ".mesh")) {
		filename = meshName.substr(0, meshName.length() - 5) + ".ClothShape.nxs";	
	}
	else {
		filename = meshName + ".ClothShape.nxs";
	}

	UserStream buf(filename.c_str(),false);

	if (!NxCookClothMesh(desc, buf)) {
		std::stringstream s;
		s << "Cloth Mesh failed to cook";
		NxThrow_Error(s.str());
	}
	fclose(buf.fp);

	UserStream rbuf(filename.c_str(), true);
	mClothMesh = mOwner->getNxScene()->getPhysicsSDK().createClothMesh(rbuf);
	fclose(rbuf.fp);

#endif

	return mClothMesh;

}

//////////////////////////////////////////////////////////

NxClothMesh* Cloth::__createClothMesh(NxReal width, NxReal height, NxReal vDistance) {

	NxClothMeshDesc desc;

	NxU32 numX = (NxU32)(width / vDistance) + 1;
	NxU32 numY = (NxU32)(height / vDistance) + 1;

	mCorners.topLeft = 0;
	mCorners.topRight = numX;
	mCorners.bottomLeft = numX * (numY - 1);
	mCorners.bottomRight = numX * numY;

	desc.numVertices = (numX+1) * (numY+1);
	desc.numTriangles = numX * numY * 2;

	mVertexCount = desc.numVertices;
	mTriangleCount = desc.numTriangles * 3;
	mTextCoordCount = desc.numVertices;

	if (mIsTearable)
		mTearFactor = 2;
	
	mVertices  = new NxVec3[mVertexCount * mTearFactor];
	mTriangles = new NxU32[mTriangleCount * 3 * mTearFactor];
	mNormals = new Ogre::Vector3[mTriangleCount * mTearFactor];
	mTextCoords = new Ogre::Vector2[mTextCoordCount * mTearFactor];

	desc.pointStrideBytes			= sizeof(NxVec3);
	desc.triangleStrideBytes		= 3*sizeof(NxU32);
	desc.vertexMassStrideBytes		= sizeof(NxReal);
	desc.vertexFlagStrideBytes		= sizeof(NxU32);
	desc.points = mVertices;
	desc.triangles = mTriangles;
	desc.flags = 0;
			
	int k=0;

	NxVec3 *p = mVertices;
	NxVec3 vertex;
	
	for (NxU32 i = 0; i <= numY; i++) {

		const Ogre::Real y_coord = static_cast<Ogre::Real> (i)  / (numY);
		
		for (NxU32 j = 0; j <= numX; j++) {
			vertex.set(vDistance*j, 0.0f, vDistance*i);
			p->set(vertex);
			
			vertex = vertex.cross(vertex);
			mNormals[k].x = vertex.x;
			mNormals[k].y = vertex.y;
			mNormals[k].z = vertex.z;
			mNormals[k].normalise();

			mTextCoords[k] = Ogre::Vector2 (1 - (static_cast<Ogre::Real> (j)  / (numX)),  1 - y_coord);

			p++;
			k++;
		}
	}

	NxU32 *id = mTriangles;
	NxU32 kp = 0;
	for (NxU32 i = 0; i < numY; i++) {
		for (NxU32 j = 0; j < numX; j++) {
			// mTriangles
			NxU32 i0 =  i * (numX+1) + j;
			NxU32 i1 = i0 + 1;
			NxU32 i2 = i0 + (numX+1);
			NxU32 i3 = i2 + 1;
			
			if ( (j + i) % 2) {
				*id++ = i0;		*id++ = i2;		*id++ = i1;
				*id++ = i1;		*id++ = i2;		*id++ = i3;
			}
			else {
				*id++ = i0;		*id++ = i2;		*id++ = i3;
				*id++ = i0;		*id++ = i3;		*id++ = i1;
			}
			
			////////////////////////////////////
			
		}
	}

	if (mIsTearable) {
		
		for (NxU32 j = kp; j < mVertexCount * mTearFactor; j++) {
				mVertices[k] = NxVec3(0,0,0);
				mNormals[k] = Ogre::Vector3::UNIT_X;
				mTextCoords[k] = 1.0f;
		}

		// allocate flag buffer
		if(desc.vertexFlags == 0)
			desc.vertexFlags = malloc(sizeof(NxU32)*desc.numVertices);

		// create tear lines
		NxU32* flags = (NxU32*)desc.vertexFlags;
		NxU32 y;
		NxU32 w = numX + 1;
		NxU32 h = numY + 1;
		for(y = 0; y < h; y++)
		{
			NxU32 x;
			for(x = 0; x < w; x++)
			{
				if(((x + y) % 16 == 0) || ((x - y + 16) % 16 == 0))
					flags[y * w + x] = NX_CLOTH_VERTEX_TEARABLE;
				else
					flags[y * w + x] = 0;
			}
		}

		desc.flags |= NX_CLOTH_VERTEX_TEARABLE;
	}


#ifndef NX_DEBUG

	MemoryWriteBuffer buf;
	if (!NxCookClothMesh(desc, buf)) {
		std::stringstream s;
		s << "Cloth Mesh failed to cook";

		NxThrow_Error(s.str());
	}

	mClothMesh = mOwner->getNxScene()->getPhysicsSDK().createClothMesh(MemoryReadBuffer(buf.data));
	
#else

	UserStream buf("cloth.cooked.nxs",false);

	if (!NxCookClothMesh(desc, buf)) {
		std::stringstream s;
		s << "Cloth Mesh failed to cook";
		NxThrow_Error(s.str());
	}
	fclose(buf.fp);

	UserStream rbuf("cloth.cooked.nxs", true);
	mClothMesh = mOwner->getNxScene()->getPhysicsSDK().createClothMesh(rbuf);
	fclose(rbuf.fp);
#endif

	return mClothMesh;
}

//////////////////////////////////////////////////////////


void Cloth::__createMesh(const NxString& name) {

	mMesh = Ogre::MeshManager::getSingleton().createManual(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	Ogre::HardwareBuffer::Usage HWUsage = Ogre::HardwareBuffer::HBU_STATIC; 

    if (mIsTearable) {	
		HWUsage = Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE;	
	}


	Ogre::HardwareIndexBufferSharedPtr indexHardBuffer = 
                    Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
                    Ogre::HardwareIndexBuffer::IT_32BIT, 
					(size_t) mTriangleCount * mTearFactor, 
                    HWUsage, 
                    false);


	indexHardBuffer->writeData(0, sizeof(NxU32)*mTriangleCount, mTriangles, true);

	mIndexData = new Ogre::IndexData;
    mIndexData->indexStart = 0;
    mIndexData->indexCount = mTriangleCount;
    mIndexData->indexBuffer = indexHardBuffer;

	//Init vertex buffer they will change size if cloth is tearable
    mVertexData= new Ogre::VertexData;
    mVertexData->vertexStart = 0;
    mVertexData->vertexCount = mVertexCount;

	Ogre::VertexDeclaration* decl = mVertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = mVertexData->vertexBufferBinding;

	decl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    decl->addElement(1, 0, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
    decl->addElement(2, 0, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

    Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;
    Ogre::HardwareVertexBufferSharedPtr mTextureCoordsBuffer;
    Ogre::HardwareVertexBufferSharedPtr mNormalBuffer;

	// Create shared vertex buffer
    mVertexBuffer =
        Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        sizeof(NxVec3),//decl->getVertexSize(VERTEX_BINDING),
		mVertexCount * sizeof(NxVec3) * mTearFactor, 
        HWUsage,
        false); // use shadow buffer
    bind->setBinding(0, mVertexBuffer);


    // Prepare buffer for mNormals - write only
    mNormalBuffer = 
        Ogre::HardwareBufferManager::getSingleton().createVertexBuffer( 
        sizeof(Ogre::Vector3),//decl->getVertexSize(NORMAL_BINDING), 
		mVertexCount * sizeof(NxVec3) * mTearFactor, 
        HWUsage,
        false); // use shadow buffer
    bind->setBinding(1, mNormalBuffer);

    mTextureCoordsBuffer =
        Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        sizeof(Ogre::Vector2),//decl->getVertexSize(TEXCOORDBINDING),
		mTextCoordCount * sizeof(Ogre::Vector2) * mTearFactor, 
        HWUsage,
        false); // use shadow buffer
	
    bind->setBinding(2, mTextureCoordsBuffer);

	// and bring it to hardware
    mVertexBuffer->writeData(0, 
        mVertexCount * sizeof(NxVec3) * mTearFactor, // size 
        mVertices, // source
        true); // discard?

    mNormalBuffer->writeData(0, 
        mVertexCount * sizeof(NxVec3) * mTearFactor, // size 
        mNormals, // source
        true); // discard?

    mTextureCoordsBuffer->writeData(0, 
		sizeof(Ogre::Vector2) * mTextCoordCount * mTearFactor, // size 
		mTextCoords, // source
        true); // discard?

	mSubMesh = mMesh->createSubMesh();
    mSubMesh->useSharedVertices = false;
    mSubMesh->vertexData = mVertexData;
    mSubMesh->indexData = mIndexData;

    mSubMesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

	
	Ogre::AxisAlignedBox aa = Ogre::AxisAlignedBox();

	NxBounds3 bounds;
	mCloth->getWorldBounds(bounds);
	aa.setMaximum(toVector3(bounds.max));
	aa.setMinimum(toVector3(bounds.min));

	mMesh->_setBounds(aa);    

    mMesh->load();
    mMesh->touch();

}

//////////////////////////////////////////////////////////

void Cloth::simulate(NxReal) {
	// No.
}

//////////////////////////////////////////////////////////

void Cloth::render(NxReal t) {

	if (mCloth->isSleeping()) {
		return;
	}


	NxU32 mVec3Buffsize = mVertexCount * sizeof(NxVec3);

	if (mIsTearable)
		mVec3Buffsize *= mTearFactor;

	// Copy Vertices
	memcpy(mVertices, mReceiveBuffers.verticesPosBegin, mVec3Buffsize);
	mVertexData->vertexCount = mVertexCount;
	mSubMesh->vertexData = mVertexData;
	 
	// Create Vertices/Normal Binding.
	Ogre::VertexBufferBinding* bind = mVertexData->vertexBufferBinding;
    
	// Send to Hardware
	Ogre::HardwareVertexBufferSharedPtr HardBuffer = bind->getBuffer(0);
    HardBuffer->writeData(0, mVec3Buffsize, mVertices, true);

	// Copy Normals
	memcpy(mNormals,mReceiveBuffers.verticesNormalBegin, mVec3Buffsize);

	// Send to Hardware.
	HardBuffer = bind->getBuffer (1);
    HardBuffer->writeData(0, mVec3Buffsize, mNormals, true);


	if (mIsTearable) {


		// Update the Texture Coordinates
		NxU32 *parent = (NxU32 *)mReceiveBuffers.parentIndicesBegin + mTextCoordCount;
		for (NxU32 i = mTextCoordCount; i < mVertexCount; i++) {
			mTextCoords[i] = mTextCoords[(*parent)];
		}
		mTextCoordCount = mVertexCount;

		
		// Write it.
		HardBuffer = bind->getBuffer(2);
		HardBuffer->writeData(0,mTextCoordCount * sizeof(Ogre::Vector2) * mTearFactor, // size 
								mTextCoords, // source
								true); // discard?

  
		// Copy the new triangle data over.
		memcpy(mTriangles,mReceiveBuffers.indicesBegin, sizeof(NxU32) * mTriangleCount * mTearFactor);

		// And send it to hardware.
		Ogre::HardwareIndexBufferSharedPtr mIndexBuffer = mIndexData->indexBuffer;
		mIndexBuffer->writeData(0, 
            sizeof(NxU32) * mTriangleCount * mTearFactor, // size 
            mTriangles, // source
            true);

	}

	// Update Bounding Box.
	if (mUpdateBB) {
		mCloth->getWorldBounds(mNxBounds);
		mAABox.setMaximum(toVector3(mNxBounds.max));
		mAABox.setMinimum(toVector3(mNxBounds.min));

		mMesh->_setBounds(mAABox);
		mNode->_updateBounds();
		mUpdateBB = false;	
	}
	else {
		mUpdateBB = true;
	}


}

//////////////////////////////////////////////////////////

NxClothMesh* Cloth::getClothMesh() {
	return mCloth->getClothMesh();
}

//////////////////////////////////////////////////////////

void Cloth::setBendingStiffness(NxReal stiffness) {
	mCloth->setBendingStiffness(stiffness);
}

//////////////////////////////////////////////////////////

NxReal Cloth::getBendingStiffness() const {
	return mCloth->getBendingStiffness();
}

//////////////////////////////////////////////////////////

void Cloth::setStretchingStiffness(NxReal stiffness) {
	mCloth->setStretchingStiffness(stiffness);
}

//////////////////////////////////////////////////////////

NxReal		 Cloth::getStretchingStiffness() const {
	return mCloth->getStretchingStiffness();
}

//////////////////////////////////////////////////////////

void		 Cloth::setDampingCoefficient(NxReal dampingCoefficient) {
	mCloth->setDampingCoefficient(dampingCoefficient);
}

//////////////////////////////////////////////////////////

NxReal		 Cloth::getDampingCoefficient() const {
	return mCloth->getDampingCoefficient();
}

//////////////////////////////////////////////////////////

void		 Cloth::setFriction(NxReal friction) {
	mCloth->setFriction(friction);
}

//////////////////////////////////////////////////////////

NxReal		 Cloth::getFriction() const {
	return mCloth->getFriction();
}

//////////////////////////////////////////////////////////

void		 Cloth::setPressure(NxReal pressure) {
	mCloth->setPressure(pressure);
}

//////////////////////////////////////////////////////////

NxReal		 Cloth::getPressure() const {
	return mCloth->getPressure();
}

//////////////////////////////////////////////////////////

void		 Cloth::setTearFactor(NxReal factor) {
	mCloth->setTearFactor(factor);
}

//////////////////////////////////////////////////////////

NxReal		 Cloth::getTearFactor() const {
	return mCloth->getTearFactor();
}

//////////////////////////////////////////////////////////

void		 Cloth::setAttachmentTearFactor(NxReal factor) {
	mCloth->setAttachmentTearFactor(factor);
}

//////////////////////////////////////////////////////////

NxReal		 Cloth::getAttachmentTearFactor() const {
	return mCloth->getAttachmentTearFactor();
}

//////////////////////////////////////////////////////////

void		 Cloth::setThickness(NxReal thickness) {
	mCloth->setThickness(thickness);
}

//////////////////////////////////////////////////////////

NxReal		 Cloth::getThickness() const {
	return mCloth->getThickness();
}

//////////////////////////////////////////////////////////

NxReal		 Cloth::getDensity() const {
	return mCloth->getDensity();
}

//////////////////////////////////////////////////////////

NxU32		 Cloth::getSolverIterations() const {
	return mCloth->getSolverIterations();
}

//////////////////////////////////////////////////////////

void		 Cloth::setSolverIterations(NxU32 iterations) {
	mCloth->setSolverIterations(iterations);
}

//////////////////////////////////////////////////////////

void		 Cloth::getWorldBounds(NxBounds3& bounds) const {
	mCloth->getWorldBounds(bounds);
}

//////////////////////////////////////////////////////////

void Cloth::attachToShape(const NxShape *shape, NxU32 attachmentFlags) {
	mCloth->attachToShape(shape, attachmentFlags);
}

//////////////////////////////////////////////////////////

void Cloth::attachToCollidingShapes(NxU32 attachmentFlags) {
	mCloth->attachToCollidingShapes(attachmentFlags);
}

//////////////////////////////////////////////////////////

void Cloth::detachFromShape(const NxShape *shape)  {
	mCloth->detachFromShape(shape);
}

//////////////////////////////////////////////////////////

void Cloth::attachVertexToShape(NxU32 vertexId, const NxShape *shape, const NxVec3 &localPos, NxU32 attachmentFlags) {
	mCloth->attachVertexToShape(vertexId, shape, localPos, attachmentFlags);
}

//////////////////////////////////////////////////////////

void Cloth::attachVertexToGlobalPosition(const NxU32 vertexId, const NxVec3 &pos) {
	mCloth->attachVertexToGlobalPosition(vertexId, pos);
}

//////////////////////////////////////////////////////////

void Cloth::freeVertex(const NxU32 vertexId) {
	mCloth->freeVertex(vertexId);
}

//////////////////////////////////////////////////////////

void Cloth::attachToCore(NxActor *actor, NxReal impulseThreshold, NxReal penetrationDepth) {
	mCloth->attachToCore(actor, impulseThreshold, penetrationDepth);
}

//////////////////////////////////////////////////////////

bool Cloth::tearVertex(const NxU32 vertexId, const NxVec3 &normal) {
	return mCloth->tearVertex(vertexId, normal);
}

//////////////////////////////////////////////////////////

bool Cloth::raycast(const NxRay& worldRay, NxVec3 &hit, NxU32 &vertexId) {
	return mCloth->raycast(worldRay, hit, vertexId);
}

//////////////////////////////////////////////////////////

void Cloth::setGroup(NxCollisionGroup collisionGroup) {
	mCloth->setGroup(collisionGroup);
}

//////////////////////////////////////////////////////////

NxCollisionGroup Cloth::getGroup() const {
	return mCloth->getGroup();
}

//////////////////////////////////////////////////////////

void Cloth::setGroupsMask(const NxGroupsMask& groupsMask) {
	mCloth->setGroupsMask(groupsMask);
}

//////////////////////////////////////////////////////////

const NxGroupsMask Cloth::getGroupsMask() const {
	return mCloth->getGroupsMask();
}

//////////////////////////////////////////////////////////

void Cloth::setMeshData(NxMeshData& meshData) {
	mCloth->setMeshData(meshData);
}

//////////////////////////////////////////////////////////

NxMeshData Cloth::getMeshData() {
	return mCloth->getMeshData();
}

//////////////////////////////////////////////////////////

void Cloth::setValidBounds(const NxBounds3& validBounds) {
	mCloth->setValidBounds(validBounds);
}

//////////////////////////////////////////////////////////

void Cloth::getValidBounds(NxBounds3& validBounds) const {
	mCloth->getValidBounds(validBounds);
}

//////////////////////////////////////////////////////////

void Cloth::setPositions(void* buffer, NxU32 byteStride) {
	mCloth->setPositions(buffer, byteStride);
}

//////////////////////////////////////////////////////////

void Cloth::getPositions(void* buffer, NxU32 byteStride) {
	mCloth->getPositions(buffer, byteStride);
}

//////////////////////////////////////////////////////////

void Cloth::setVelocities(void* buffer, NxU32 byteStride) {
	mCloth->setVelocities(buffer, byteStride);
}

//////////////////////////////////////////////////////////

void Cloth::getVelocities(void* buffer, NxU32 byteStride) {
	mCloth->getVelocities(buffer, byteStride);
}

//////////////////////////////////////////////////////////

NxU32 Cloth::getNumberOfParticles() {
	return mCloth->getNumberOfParticles();
}

//////////////////////////////////////////////////////////

NxU32 Cloth::queryShapePointers() {
	return mCloth->queryShapePointers();
}

//////////////////////////////////////////////////////////

NxU32 Cloth::getStateByteSize() {
	return mCloth->getStateByteSize();
}

//////////////////////////////////////////////////////////

void Cloth::getShapePointers(NxShape** shapePointers,NxU32 *flags) {
	mCloth->getShapePointers(shapePointers, flags);
}

//////////////////////////////////////////////////////////

void Cloth::setShapePointers(NxShape** shapePointers,unsigned int numShapes) {
	mCloth->setShapePointers(shapePointers, numShapes);
}

//////////////////////////////////////////////////////////

void Cloth::saveStateToStream(NxStream& stream) {
	mCloth->saveStateToStream(stream);
}

//////////////////////////////////////////////////////////

void Cloth::loadStateFromStream(NxStream& stream) {
	mCloth->loadStateFromStream(stream);
}

//////////////////////////////////////////////////////////

void Cloth::setCollisionResponseCoefficient(NxReal coefficient) {
	mCloth->setCollisionResponseCoefficient(coefficient);
}

//////////////////////////////////////////////////////////

NxReal Cloth::getCollisionResponseCoefficient() const {
	return mCloth->getCollisionResponseCoefficient();
}

//////////////////////////////////////////////////////////

void Cloth::setAttachmentResponseCoefficient(NxReal coefficient) {
	mCloth->setAttachmentResponseCoefficient(coefficient);
}

//////////////////////////////////////////////////////////

NxReal Cloth::getAttachmentResponseCoefficient() const {
	return mCloth->getAttachmentResponseCoefficient();
}

//////////////////////////////////////////////////////////

void Cloth::setFromFluidResponseCoefficient(NxReal coefficient) {
	mCloth->setFromFluidResponseCoefficient(coefficient);
}

//////////////////////////////////////////////////////////

NxReal Cloth::getFromFluidResponseCoefficient() const {
	return mCloth->getFromFluidResponseCoefficient();
}

//////////////////////////////////////////////////////////

void Cloth::setToFluidResponseCoefficient(NxReal coefficient) {
	mCloth->setToFluidResponseCoefficient(coefficient);
}

//////////////////////////////////////////////////////////

NxReal Cloth::getToFluidResponseCoefficient() const {
	return mCloth->getToFluidResponseCoefficient();
}

//////////////////////////////////////////////////////////

void Cloth::setExternalAcceleration(NxVec3 acceleration) {
	mCloth->setExternalAcceleration(acceleration);
}

//////////////////////////////////////////////////////////

NxVec3 Cloth::getExternalAcceleration() const {
	return mCloth->getExternalAcceleration();
}

//////////////////////////////////////////////////////////

bool Cloth::isSleeping() const {
	return mCloth->isSleeping();
}

//////////////////////////////////////////////////////////

NxReal Cloth::getSleepLinearVelocity() const {
	return mCloth->getSleepLinearVelocity();
}

//////////////////////////////////////////////////////////

void Cloth::setSleepLinearVelocity(NxReal threshold) {
	mCloth->setSleepLinearVelocity(threshold);
}

//////////////////////////////////////////////////////////

void Cloth::wakeUp(NxReal wakeCounterValue) {
	mCloth->wakeUp(wakeCounterValue);
}

//////////////////////////////////////////////////////////

void Cloth::putToSleep() {
	mCloth->putToSleep();
}

//////////////////////////////////////////////////////////

void Cloth::setFlags(NxU32 flags) {
	mCloth->setFlags(flags);
}

//////////////////////////////////////////////////////////

NxU32 Cloth::getFlags() const {
	return mCloth->getFlags();
}

//////////////////////////////////////////////////////////

void Cloth::addForceAtVertex(const Ogre::Vector3& force, NxU32 vertexId, NxForceMode mode) {
	mCloth->addForceAtVertex(toNxVec3(force), vertexId, mode);
}

//////////////////////////////////////////////////////////

void Cloth::addForceAtPos(const NxVec3& position, NxReal magnitude, NxReal radius, NxForceMode mode) {
	mCloth->addForceAtPos(position, magnitude, radius, mode);
}

//////////////////////////////////////////////////////////

bool Cloth::overlapAABBTriangles(const NxBounds3& bounds, NxU32& nb, const NxU32*& indices) const {
	return mCloth->overlapAABBTriangles(bounds, nb, indices);
}

//////////////////////////////////////////////////////////
#if NX_SDK_VERSION_NUMBER >= 260
NxCompartment*  Cloth::getCompartment() const {
	return mCloth->getCompartment();
}
#endif
//////////////////////////////////////////////////////////

unsigned int Cloth::getNbVertices() {
	return mVertexCount;
}

//////////////////////////////////////////////////////////

Ogre::Vector3 ClothVertex::getGlobalPosition() const {
	NxVec3* vertices = new NxVec3();
	cloth->getPositions((void*) vertices);
	return toVector3(vertices[vertex]);
}

}  //End of NxOgre namespace.
////#endif
