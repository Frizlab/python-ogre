/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10

Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)



This program is free software; you can redistribute it and/or modify it under
the terms of the GPL General Public License with runtime exception as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GPL General Public License with runtime exception for more details.

You should have received a copy of the GPL General Public License with runtime exception along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
-----------------------------------------------------------------------------
*/

#include "OgreBulletCollisions.h"

#include "Shapes/OgreBulletCollisionsGImpactShape.h"

/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10

Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)



This program is free software; you can redistribute it and/or modify it under
the terms of the GPL General Public License with runtime exception as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GPL General Public License with runtime exception for more details.

You should have received a copy of the GPL General Public License with runtime exception along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
-----------------------------------------------------------------------------
*/

#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Debug/OgreBulletCollisionsDebugLines.h"
#include "Utils/OgreBulletConverter.h"

using namespace Ogre;
using namespace OgreBulletCollisions;

namespace OgreBulletCollisions
{
	GImpactConcaveShape::GImpactConcaveShape(
		Ogre::Vector3 *_vertices,
		unsigned int _vertex_count,
		unsigned int *_indices,
		unsigned int int_index_count) : CollisionShape(), mTriMesh(0)
	{

#ifndef BVH
		mTriMesh = new btTriangleMesh();

		unsigned int numFaces = int_index_count / 3;

		btVector3    vertexPos[3];
		for (size_t n = 0; n < numFaces; ++n)
		{
			for (unsigned int i = 0; i < 3; ++i)
			{
				const Vector3 &vec = _vertices[*_indices];
				vertexPos[i][0] = vec.x;
				vertexPos[i][1] = vec.y;
				vertexPos[i][2] = vec.z;
				*_indices++;
			}

			mTriMesh->addTriangle(vertexPos[0], vertexPos[1], vertexPos[2]);
		}

		btGImpactMeshShape * trimesh = new btGImpactMeshShape(mTriMesh);
		trimesh->setLocalScaling(btVector3(1, 1, 1));
		trimesh->setMargin(0.0f);
		trimesh->updateBound();
#else

		btTriangleIndexVertexArray *m_indexVertexArrays = new btTriangleIndexVertexArray(
			int_index_count,
			_indices,
			4,
			_vertex_count,
			(btScalar*) &_vertices[0][0], 
			12);

		bool useQuantizedAabbCompression = true;

		//comment out the next line to read the BVH from disk (first run the demo once to create the BVH)
#define SERIALIZE_TO_DISK 1
#ifdef SERIALIZE_TO_DISK
		btVector3 aabbMin(-1000,-1000,-1000),aabbMax(1000,1000,1000);

		btBvhTriangleMeshShape * trimeshShape  = new btBvhTriangleMeshShape(m_indexVertexArrays,useQuantizedAabbCompression,aabbMin,aabbMax);
		
		///we can serialize the BVH data 
		void* buffer = 0;
		int numBytes = trimeshShape->getOptimizedBvh()->calculateSerializeBufferSize();
		buffer = btAlignedAlloc(numBytes,16);
		bool swapEndian = false;
		trimeshShape->getOptimizedBvh()->serialize(buffer,numBytes,swapEndian);
		FILE* file = fopen("bvh.bin","wb");
		fwrite(buffer,1,numBytes,file);
		fclose(file);
		btAlignedFree(buffer);



#else

		btBvhTriangleMeshShape *trimesh  = new btBvhTriangleMeshShape(m_indexVertexArrays,useQuantizedAabbCompression,false);

		char* fileName = "bvh.bin";

		FILE* file = fopen(fileName,"rb");
		int size=0;
		btOptimizedBvh* bvh = 0;

		if (fseek(file, 0, SEEK_END) || (size = ftell(file)) == EOF || fseek(file, 0, SEEK_SET)) {        /* File operations denied? ok, just close and return failure */
			printf("Error: cannot get filesize from %s\n", fileName);
			exit(0);
		} else
		{

			fseek(file, 0, SEEK_SET);

			int buffersize = size+btOptimizedBvh::getAlignmentSerializationPadding();

			void* buffer = btAlignedAlloc(buffersize,16);
			int read = fread(buffer,1,size,file);
			fclose(file);
			bool swapEndian = false;
			bvh = btOptimizedBvh::deSerializeInPlace(buffer,buffersize,swapEndian);
		}

		trimesh->setOptimizedBvh(bvh);
#endif
#endif
		mShape = trimesh;
	}

	GImpactConcaveShape::~GImpactConcaveShape()
	{
	}
	// -------------------------------------------------------------------------
	bool GImpactConcaveShape::drawWireFrame(DebugLines *wire, 
		const Ogre::Vector3 &pos, 
		const Ogre::Quaternion &quat) const
	{
		const int numTris = mTriMesh->getNumTriangles ();
		if (numTris > 0)
		{

			const int numSubParts = mTriMesh->getNumSubParts ();
			for (int currSubPart = 0; currSubPart < numSubParts; currSubPart++)
			{
				const unsigned char* vertexBase = NULL;
				int numVerts;
				PHY_ScalarType vertexType;
				int vertexStride;
				const unsigned char* indexBase = NULL;
				int indexStride;
				int numFaces;
				PHY_ScalarType indexType;

				mTriMesh->getLockedReadOnlyVertexIndexBase (&vertexBase, numVerts, 
					vertexType, vertexStride, 
					&indexBase, indexStride, numFaces, indexType, currSubPart);

				float* p;
				btVector3 vert0;
				btVector3 vert1;
				btVector3 vert2;
				for (int t = 0; t < numFaces; t++)
				{
#define setVector(A, B) {A.setX(B[0]);A.setY(B[1]);A.setZ(B[2]);};

					if (indexType == PHY_SHORT)
					{
						short int* index = (short int*)(indexBase + t*indexStride);

						p = (float*)(vertexBase + index[0]*vertexStride);
						setVector(vert0, p);						
						p = (float*)(vertexBase + index[1]*vertexStride);
						setVector(vert1, p);			
						p = (float*)(vertexBase + index[2]*vertexStride);
						setVector(vert2, p);		
					} 
					else
					{
						int* index = (int*)(indexBase + t*indexStride);

						p = (float*)(vertexBase + index[0]*vertexStride);
						setVector(vert0, p);						
						p = (float*)(vertexBase + index[1]*vertexStride);
						setVector(vert1, p);			
						p = (float*)(vertexBase + index[2]*vertexStride);
						setVector(vert2, p);		
					}
#undef setVector

					wire->addLine (BtOgreConverter::to(vert0), BtOgreConverter::to(vert1));
					wire->addLine (BtOgreConverter::to(vert1), BtOgreConverter::to(vert2));
					wire->addLine (BtOgreConverter::to(vert2), BtOgreConverter::to(vert0));
				}
			}
			return true;
		}
		return false;
	}
}