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

#ifndef __NXOGRE_JSON_SERIALISER_H__
#define __NXOGRE_JSON_SERIALISER_H__

#include "NxOgrePrerequisites.h"

#if (NX_USE_JSON == 1)

#include "json/json.h"
#include "NXU_Schema.h"

namespace NxOgre {

	class NxExport NxJSONCollection {

		public:

			//////////////////////////////////////////////////////
			
			class StringLinkedList {
			
				public:

					class Node {
						public:
							Node(NxString d, Node* p) : data(d), prev(p), next(0) {}
							NxString data;
							Node* next;
							Node* prev;
					};

					StringLinkedList() {
						first = 0;
						last = 0;
					}

					~StringLinkedList() {
						Node *n = first;
						while(n) {
							Node *N = n->next;
							delete n;
							n = N;
						}
					}

					void pop_front() {
						Node* n = first->next;
						delete first;
						n->prev = last;
						last->next = n;
						first = n;
					}

					void pop_back() {
						Node* p = last->prev;
						delete last;
						p->next = first;
						first->prev = p;
					}

					void push_front(NxString str) {
						Node* n = new Node(str, 0);
						n->next = first;
						n->next->prev = n;
						n->prev = last;
						first = n;
					}

					void push_back(NxString str) {
						if (first == 0) {
							first = new Node(str, 0);
							last = first;
							first->prev = last;
						}
						else {
							Node* n = new Node(str, last);
							last->next = n;
							last = n;
						}
					}

					void dump() {
						Node *n = first;
						std::cout << "** Dump" << std::endl;
						while(n) {
							std::cout << "\t" << n->data << std::endl;
							n = n->next;
						}
						
						std::cout << "*** Done." << std::endl;
					}


					Node* first;
					Node* last;

			};

			////////////////////////////////////////////////////////

			NxJSONCollection(World* w) {
				mWorld=w;

			}
			~NxJSONCollection() {

			}

			//////////////////////////////////////////////////////
			
			void importCollection(NXU::NxuPhysicsCollection*, NxString file);
			void importCollectionFromString(NXU::NxuPhysicsCollection*, NxString str);
			void exportCollection(NxString file, NXU::NxuPhysicsCollection*);
			void exportCollection(NXU::NxuPhysicsCollection*);

			NXU::NxuPhysicsCollection* mCollection;

			World* getWorld() {return mWorld;}
			
			Json::Value mRoot;
	protected:

			//////////////////////////////////////////////////////

			void __importCollection();
			void __readConvexMeshes();
			void __readScenes();
			void __readTriangluarMeshes();

			//////////////////////////////////////////////////////
			
			void __writeVersion();
			void __writeScenes(NxArray<NXU::NxSceneDesc*>&);
			void __writeConvexMeshes(NxArray<NXU::NxConvexMeshDesc*>&);
			void __writeTriangluarMeshes(NxArray<NXU::NxTriangleMeshDesc*> &meshes);

			//////////////////////////////////////////////////////
			
			Json::Value		__writeMaterial(NXU::NxMaterialDesc*, Scene*);
			Json::Value		__writeSpring(NXU::NxSpringDesc*);
			Json::Value		__writeActor(NXU::NxActorDesc*, Scene*);
			void			__writeShape(NXU::NxShapeDesc*, Json::Value&, NxShapeDesc&, Scene*);
			Json::Value		__writePlaneShape(NXU::NxPlaneShapeDesc*, Scene*);
			Json::Value		__writeSphereShape(NXU::NxSphereShapeDesc*, Scene*);
			Json::Value		__writeBoxShape(NXU::NxBoxShapeDesc*, Scene*);
			Json::Value		__writeConvexShape(NXU::NxConvexShapeDesc*, Scene*);
			Json::Value		__writeTriangleMeshShape(NXU::NxTriangleMeshShapeDesc*, Scene*);
			Json::Value		__writeHeightFieldShape(NXU::NxHeightFieldShapeDesc*, Scene*);
			Json::Value		__writeCapsuleShape(NXU::NxCapsuleShapeDesc*, Scene*);
			Json::Value		__writeWheelShape(NXU::NxWheelShapeDesc*, Scene*);
			Json::Value		__writeTireFunction(NXU::NxTireFunctionDesc*, Scene*);

			//////////////////////////////////////////////////////
			
			NXU::NxMaterialDesc*			__readMaterial(Json::Value&);
			NXU::NxActorDesc*				__readActor(Json::Value& v, Json::Value& k);
			void							__readShape(Json::Value&, NXU::NxShapeDesc*);
			NXU::NxPlaneShapeDesc*			__readPlaneShape(Json::Value&);
			NXU::NxSphereShapeDesc*			__readSphereShape(Json::Value&);
			NXU::NxBoxShapeDesc*			__readBoxShape(Json::Value&);
			NXU::NxCapsuleShapeDesc*		__readCapsuleShape(Json::Value&);
			NXU::NxConvexShapeDesc*			__readConvexShape(Json::Value&);
			NXU::NxTriangleMeshShapeDesc*	__readTriangleMeshShape(Json::Value&);

			//////////////////////////////////////////////////////
			
			Json::Value __fromNxVec3(const NxVec3&);
			NxVec3		__toNxVec3(Json::Value&);
			Json::Value __fromNxMat33(const NxMat33&);
			NxMat33		__toNxMat33(Json::Value&);
			Json::Value __fromNxMat34(const NxMat34&);
			NxMat34		__toNxMat34(Json::Value&);
			Json::Value __fromNxTri(const NXU::NxTri&);
			NXU::NxTri	__toNxTri(Json::Value&);

			//////////////////////////////////////////////////////
			
			void __fetchString(const char *&v,const char *name,Json::Value&);
			void __fetchString(const char *&v, Json::Value&);

			//////////////////////////////////////////////////////
			
			bool __sameAs(const NxMat34 &a,const NxMat34 &b) {
				bool ret = false;

				float m1[16];
				float m2[16];
				a.getColumnMajor44(m1);
				b.getColumnMajor44(m2);
				if ( memcmp(m1,m2,sizeof(float)*16) == 0 )
					ret = true;

			  return ret;
			}

			//////////////////////////////////////////////////////
			
			bool __sameAs(const NxMat33 &a,const NxMat33 &b) {
				bool ret = false;

				float m1[9];
				float m2[9];
				a.getColumnMajor(m1);
				b.getColumnMajor(m2);
				if ( memcmp(m1,m2,sizeof(float)*9) == 0 )
					ret = true;

			  return ret;
			}

			//////////////////////////////////////////////////////
			
			bool __sameAs(const NxQuat &a,const NxQuat &b) {
				bool ret = false;

				float q1[4];
				float q2[4];

				a.getXYZW(q1);
				b.getXYZW(q2);

				if ( q1[0] == q2[0] &&
				q1[1] == q2[1] &&
				q1[2] == q2[2] &&
				q1[3] == q2[3] )
				{
					ret = true;
				}
				return ret;
			}

			//////////////////////////////////////////////////////
			
			bool __sameAs(const NxBounds3 &a,const NxBounds3 &b) {
				bool ret = false;

				if ( a.min == b.min && a.max == b.max )
					ret = true;

				return ret;
			}

			//////////////////////////////////////////////////////
			
			Json::Value mMeshSection;

			World* mWorld;
			

			StringLinkedList			mStrings;

	};

};
#endif

#endif
