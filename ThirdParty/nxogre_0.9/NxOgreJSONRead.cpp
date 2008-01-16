//
//	NxOgre a wrapper for the PhysX (formerly Novodex) physics library and the Ogre 3D rendering engine.
//	Copyright (C) 2005 - 2007 Robin Southern and NxOgre.org http://www.nxogre.org
//
//	This library is free software{ you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation{ either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY{ without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library{ if not, write to the Free Software
//	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "NxOgreStable.h"
#include "NxOgreJSON.h"

#if (NX_USE_JSON == 1)

#include "NxOgreJSONSchema.h"
#include "NxuStream2/NXU_schema.h"
#include "json/json.h"
#include "NxOgrescene.h"
#include <ostream>
#include "NxOgreHelpers.h"
#include "NxOgreContainer.h"
#include "NxOgreWorld.h"
#include "NxOgrescene.h"
#include "NxOgreGroup.h"
#include "NxOgreMaterial.h"

#include "string.h"

namespace NxOgre {

//////////////////////////////////////////////////////////////////////

void NxJSONCollection::importCollection(NXU::NxuPhysicsCollection* collection, NxString file) {

	mCollection = collection;

	Json::Reader reader;
	std::ifstream json_file;
	json_file.open(file.c_str());
	reader.parse(json_file, mRoot, false);
	json_file.close();

	__importCollection();
}

///////////////////////////////////////////

void NxJSONCollection::importCollectionFromString(NXU::NxuPhysicsCollection* collection, NxString str) {

	mCollection = collection;
	Json::Reader reader;
	reader.parse(str, mRoot, false);

	__importCollection();
}

///////////////////////////////////////////

void NxJSONCollection::__importCollection() {


//			if (__checkVersions()) {			
//				return NxJSONCollection;
//			}

	//__readClothMeshes(NxJSONCollection->mClothMeshes);
	if (!mRoot["Meshes"].isNull()) {
		__readConvexMeshes();
		__readTriangluarMeshes();
		// __readHeightFields(NxJSONCollection->mHeightFields);
		// __readSoftBody(NxJSONCollection->mSoftBodyMeshes);
	}
	__readScenes();
	
	// __readParameters(NxJSONCollection->mParameters);
	// __readSkeletons(NxJSONCollection->mSkeletons);
	// __readUserProperties(NxJSONCollection->mUserProperties);

}

///////////////////////////////////////////

void NxJSONCollection::__readConvexMeshes() {

	Json::Value section = mRoot[NX_JSON_SECTION_Meshes][NX_JSON_SECTION_Convex];
	
	
	if (section.isNull())
		return;

	NxConvexMeshDesc defaultMesh;

	for (NxU32 index = 0; index < section.size(); ++index )  {

		NXU::NxConvexMeshDesc* mesh = new NXU::NxConvexMeshDesc;

		Json::Value convex = section[index];

		if (!convex[NX_JSON_ID].isNull()) {
			mStrings.push_back(convex[NX_JSON_ID].asString());
			mesh->mId = mStrings.last->data.c_str();
		}
		else {
			mesh->mId = 0;
		}

		// (TODO) User properties


		{
			Json::Value points = convex[NX_JSON_Vertices];
			for (NxU32 i=0; i < points.size();i++) {
				Json::Value p = points[i];
				NxVec3 vertex;
				vertex.x = p[NxU32(0)].asDouble();
				vertex.y = p[NxU32(1)].asDouble();
				vertex.z = p[NxU32(2)].asDouble();

				mesh->mPoints.push_back(vertex);
			}
		}

		{
			Json::Value triangles = convex[NX_JSON_Triangles];
			for (NxU32 i=0; i < triangles.size();i++) {
				Json::Value p = triangles[i];
				NXU::NxTri tri;
				tri.a = p[NxU32(0)].asUInt();
				tri.b = p[NxU32(1)].asUInt();
				tri.c = p[NxU32(2)].asUInt();

				mesh->mTriangles.push_back(tri);
			}
		}
		
		if (!convex[NX_JSON_Flags].isNull()) {
			Json::Value flags;
			for (NxU32 ii = 0; ii < flags.size();++ii) {
			

			}
		}


		mCollection->mConvexMeshes.push_back(mesh);
	}
		


#if 0
		
stream.load(mUserProperties,"mUserProperties",false);    // {NXOGRE}

#endif
}

/////////////////////////////////////////////////

void NxJSONCollection::__readTriangluarMeshes() {

	Json::Value section = mRoot[NX_JSON_SECTION_Meshes][NX_JSON_SECTION_TriangleMesh];
	
	
	if (section.isNull())
		return;

	NxTriangleMeshDesc defaultMesh;

	for (NxU32 index = 0; index < section.size(); ++index )  {

		NXU::NxTriangleMeshDesc* mesh = new NXU::NxTriangleMeshDesc;

		Json::Value def = section[index];

		if (!def[NX_JSON_ID].isNull()) {
			mStrings.push_back(def[NX_JSON_ID].asString());
			mesh->mId = mStrings.last->data.c_str();
		}
		else {
			mesh->mId = 0;
		}

		// (TODO) User properties


		{
			Json::Value points = def[NX_JSON_Vertices];
			for (NxU32 i=0; i < points.size();i++) {
				Json::Value p = points[i];
				NxVec3 vertex;
				vertex.x = p[NxU32(0)].asDouble();
				vertex.y = p[NxU32(1)].asDouble();
				vertex.z = p[NxU32(2)].asDouble();

				mesh->mPoints.push_back(vertex);
			}
		}

		{
			Json::Value triangles = def[NX_JSON_Triangles];
			for (NxU32 i=0; i < triangles.size();i++) {
				Json::Value p = triangles[i];
				NXU::NxTri tri;
				tri.a = p[NxU32(0)].asUInt();
				tri.b = p[NxU32(1)].asUInt();
				tri.c = p[NxU32(2)].asUInt();

				mesh->mTriangles.push_back(tri);
			}
		}
		
		if (!def[NX_JSON_Flags].isNull()) {
			Json::Value flags;
			for (NxU32 ii = 0; ii < flags.size();++ii) {
			

			}
		}

		mCollection->mTriangleMeshes.push_back(mesh);

	}
		


#if 0
		
stream.load(mUserProperties,"mUserProperties",false);    // {NXOGRE}

#endif
}

/////////////////////////////////////////////////

void NxJSONCollection::__readScenes() {

	Json::Value section = mRoot[NX_JSON_SECTION_Scenes];
	
	if (section.isNull())
		return;

	NxSceneDesc defaultScene;
	defaultScene.setToDefault();
				
	for (NxU32 index=0;index < section.size();index++) {
		
		NXU::NxSceneDesc* scene = new NXU::NxSceneDesc;
		mCollection->mCurrentScene = scene;
		Json::Value def = section[index];



		__fetchString(scene->mId, NX_JSON_ID, def);
		std::cout << "SceneID = " << scene->mId << std::endl;

#if 0
		if (def->mUserProperties) {
			std::cout << "!!! userProperties" << std::endl;
			StringPairList l;
			l.parseCSString(def->mUserProperties);
			
			// Broken????
			for (StringPairList::StringPair sp = l.begin();l.hasNext();) {
				sp = l.next();
				scene[sp.first] = sp.second;
			}

		}
#endif


		if (!def[NX_JSON_FilterOp0].isNull()) {
			NXU::StringToEnum(def[NX_JSON_FilterOp0].asCString(), scene->mFilterOp0);
		}

		if (!def[NX_JSON_FilterOp1].isNull()) {
			NXU::StringToEnum(def[NX_JSON_FilterOp1].asCString(), scene->mFilterOp1);
		}

		if (!def[NX_JSON_FilterOp2].isNull()) {
			NXU::StringToEnum(def[NX_JSON_FilterOp2].asCString(), scene->mFilterOp2);
		}

		{
			scene->mGroupMask0.bits0 = def[NX_JSON_GroupMask0][NxU32(0)].asUInt();
			scene->mGroupMask0.bits1 = def[NX_JSON_GroupMask0][NxU32(1)].asUInt();
			scene->mGroupMask0.bits2 = def[NX_JSON_GroupMask0][NxU32(2)].asUInt();
			scene->mGroupMask0.bits3 = def[NX_JSON_GroupMask0][NxU32(3)].asUInt();
		}

		{
			scene->mGroupMask1.bits0 = def[NX_JSON_GroupMask1][NxU32(0)].asUInt();
			scene->mGroupMask1.bits1 = def[NX_JSON_GroupMask1][NxU32(1)].asUInt();
			scene->mGroupMask1.bits2 = def[NX_JSON_GroupMask1][NxU32(2)].asUInt();
			scene->mGroupMask1.bits3 = def[NX_JSON_GroupMask1][NxU32(3)].asUInt();
		}

		if (!def[NX_JSON_Gravity].isNull()) {
			scene->gravity.x = def[NX_JSON_Gravity][NxU32(0)].asDouble();
			scene->gravity.y = def[NX_JSON_Gravity][NxU32(1)].asDouble();
			scene->gravity.z = def[NX_JSON_Gravity][NxU32(2)].asDouble();
		}

		if (!def[NX_JSON_MaxTimeStep].isNull())
			scene->maxTimestep = def[NX_JSON_MaxTimeStep].asDouble();

		if (!def[NX_JSON_MaxIter].isNull())
			scene->maxIter = def[NX_JSON_MaxIter].asUInt();

		if (!def[NX_JSON_TimeStepMethod].isNull())
			NXU::StringToEnum(def[NX_JSON_TimeStepMethod].asCString(), scene->timeStepMethod);
		
		if (!def[NX_JSON_MaxBounds].isNull()) {
			scene->mMaxBounds.min.x = def[NX_JSON_MaxBounds][NxU32(0)].asDouble();
			scene->mMaxBounds.min.y = def[NX_JSON_MaxBounds][NxU32(1)].asDouble();
			scene->mMaxBounds.min.z = def[NX_JSON_MaxBounds][NxU32(2)].asDouble();
			scene->mMaxBounds.max.x = def[NX_JSON_MaxBounds][NxU32(3)].asDouble();
			scene->mMaxBounds.max.y = def[NX_JSON_MaxBounds][NxU32(4)].asDouble();
			scene->mMaxBounds.max.z = def[NX_JSON_MaxBounds][NxU32(5)].asDouble();
		}

		if (!def[NX_JSON_SimType].isNull())
			NXU::StringToEnum(def[NX_JSON_SimType].asCString(), scene->simType);

		if (!def[NX_JSON_GroundPlane].isNull())
			scene->groundPlane = def[NX_JSON_GroundPlane].asBool();

		if (!def[NX_JSON_BoundsPlanes].isNull())
			scene->boundsPlanes = def[NX_JSON_BoundsPlanes].asBool();

		// TODO
#if 0
		if (def->flags != defaultScene->flags) {
			Json::Value fl;


			for (int i=0; i<32; i++) {
				int shift = (1<<i);
				const char *str = NXU::EnumToString( (NXU::NxSceneFlags) shift);
				if (!str)
					break;
			
				const char *dummy = strstr(str,"DUMMY");
				
				if (dummy)
					continue;
			
				bool b = false;
				if ( def->flags & shift )
					b = true;

				fl[str] = (bool) b;

				scene[NX_JSON_Flags] = fl;
			}

		}
#endif

		if (!def[NX_JSON_InternalThreadCount].isNull())
			scene->internalThreadCount = def[NX_JSON_InternalThreadCount].asUInt();
		
		if (!def[NX_JSON_BackgroundThreadCount].isNull())
			scene->backgroundThreadCount = def[NX_JSON_BackgroundThreadCount].asUInt();
		
		if (!def[NX_JSON_ThreadMask].isNull())
			scene->threadMask = def[NX_JSON_ThreadMask].asUInt();
		
		if (!def[NX_JSON_BackgroundThreadMask].isNull())
			scene->backgroundThreadMask = def[NX_JSON_BackgroundThreadMask].asUInt();

		#if NX_SDK_VERSION_NUMBER >= 250
		if (!def[NX_JSON_SimThreadStackSize].isNull())
			scene->simThreadStackSize = def[NX_JSON_SimThreadStackSize].asUInt();
		#endif

		#if NX_SDK_VERSION_NUMBER >= 260
		if (!def[NX_JSON_SimThreadPriority].isNull())
			NXU::StringToEnum(def[NX_JSON_SimThreadPriority].asCString(), scene->simThreadPriority);
		#endif
		
		#if NX_SDK_VERSION_NUMBER >= 250
		 if (!def[NX_JSON_WorkerThreadStackSize].isNull())
			 scene->workerThreadStackSize = def[NX_JSON_WorkerThreadStackSize].asUInt();
		#endif

		#if NX_SDK_VERSION_NUMBER >= 250
		   if (!def[NX_JSON_WorkerThreadPriority].isNull())
			   NXU::StringToEnum(def[NX_JSON_WorkerThreadPriority].asCString(), scene->workerThreadPriority);
		#endif

		#if NX_SDK_VERSION_NUMBER >= 262
		  if (!def[NX_JSON_UpAxis].isNull())
			 scene->upAxis = def[NX_JSON_UpAxis].asUInt();
		#endif

		#if NX_SDK_VERSION_NUMBER >= 262
		if (!def[NX_JSON_SubdivisionLevel].isNull())
			scene->subdivisionLevel = def[NX_JSON_SubdivisionLevel].asUInt();
		#endif

		#if NX_SDK_VERSION_NUMBER >= 262
		if (!def[NX_JSON_StaticStructure].isNull())
			NXU::StringToEnum(def[NX_JSON_StaticStructure].asCString(), scene->staticStructure);
		#endif


		#if NX_SDK_VERSION_NUMBER >= 262
		if (!def[NX_JSON_DynamicStructure].isNull())
			NXU::StringToEnum(def[NX_JSON_DynamicStructure].asCString(), scene->dynamicStructure);
		#endif



		//////////////////////////////////////////////////////

		Json::Value materials = def[NX_JSON_SECTION_Materials];

		for (NxU32 i=0; i< materials.size(); i++) {
			scene->mMaterials.push_back(__readMaterial(materials[i]));
		}

		//////////////////////////////////////////////////////


		Json::Value actors = def[NX_JSON_SECTION_Actors];

		for (Json::ValueIterator it = actors.begin(); it != actors.end(); it++) {
			scene->mActors.push_back(__readActor((*it), it.key()));
		}

		//////////////////////////////////////////////////////

		

		mCollection->mScenes.push_back(scene);


	}

	mCollection->mCurrentScene = 0;
}


//////////////////////////////////////////////////////

NXU::NxMaterialDesc* NxJSONCollection::__readMaterial(Json::Value& def) {
	
	NXU::NxMaterialDesc* material = new NXU::NxMaterialDesc;

	std::cout << "Material Name = " << def[NX_JSON_ID].asCString() << std::endl;
	__fetchString(material->mId, NX_JSON_ID, def);


	if (!def[NX_JSON_Spring].isNull()) {
		material->mHasSpring = true;
		// material->mSpring = __readSpring(def[NX_JSON_Spring]);
	}
	else {
		material->mHasSpring = false;
	}

	material->mMaterialIndex = def[NX_JSON_MaterialIndex].asUInt();

	if (!def[NX_JSON_DynamicFriction].isNull())
		material->dynamicFriction = def[NX_JSON_DynamicFriction].asDouble();

	if (!def[NX_JSON_StaticFriction].isNull())
		material->staticFriction = def[NX_JSON_StaticFriction].asDouble();

	if (!def[NX_JSON_Restitution].isNull())
		material->restitution = def[NX_JSON_Restitution].asDouble();

	if (!def[NX_JSON_DynamicFrictionV].isNull())
		material->dynamicFrictionV = def[NX_JSON_DynamicFrictionV].asDouble();

	if (!def[NX_JSON_StaticFrictionV].isNull())
		material->staticFrictionV = def[NX_JSON_StaticFrictionV].asDouble();

	if (!def[NX_JSON_FrictionCombineMode].isNull())
		NXU::StringToEnum(def[NX_JSON_FrictionCombineMode].asCString(), material->frictionCombineMode);

	if (!def[NX_JSON_RestitutionCombineMode].isNull())
		NXU::StringToEnum(def[NX_JSON_RestitutionCombineMode].asCString(), material->restitutionCombineMode);

	if (!def[NX_JSON_DirOfAnisotropy].isNull())
		material->dirOfAnisotropy = __toNxVec3(def[NX_JSON_DirOfAnisotropy]);

#if 0

	 if (def->flags != defaultMaterial.flags) {
		Json::Value fl;
		

		for (int i=0; i<32; i++) {
			int shift = (1<<i);
			const char *str = NXU::EnumToString( (NXU::NxMaterialFlag) shift);
			if (!str)
				break;
		
			const char *dummy = strstr(str,"DUMMY");
			
			if (dummy)
				continue;
		
			bool b = false;
			if ( def->flags & shift )
				b = true;

			fl[str] = (bool) b;

			material[NX_JSON_Flags] = fl;
		}

	}

#endif

	return material;
}

//////////////////////////////////////////////////////

NXU::NxActorDesc* NxJSONCollection::__readActor(Json::Value& def, Json::Value& key) {

	NXU::NxActorDesc* actor = new NXU::NxActorDesc;

	__fetchString(actor->mId, key);
	
	//if (!def[NX_JSON_InternalThreadCount].isNull())
	//	scene->internalThreadCount = def[NX_JSON_InternalThreadCount].asUInt();

	if (!def[NX_JSON_SECTION_User].isNull()) {
		
		Json::Value userproperties = def[NX_JSON_SECTION_User];

		NxString up;

		for (Json::ValueIterator it = userproperties.begin(); it != userproperties.end(); it++) {
			up.append(it.key().asString());
			up.append(":");
			up.append((*it).asString());
			up.append(",");
		}

		// Remove last ,
		up = up.substr(0, up.length() - 1);

		mStrings.push_back(up);
		actor->mUserProperties = mStrings.last->data.c_str();
		//actor->mUserProperties = mStrings.getCString(mStrings.addString(up));
		
	}

	if (!def[NX_JSON_GlobalPose].isNull())
		actor->globalPose = __toNxMat34(def[NX_JSON_GlobalPose]);

	if (!def[NX_JSON_GlobalPose].isNull())
		actor->density = def[NX_JSON_Density].asDouble();
/*
	if (def->flags) {
		Json::Value fl;


		for (int i=0; i<32; i++) {
			int shift = (1<<i);
			const char *str = NXU::EnumToString( (NXU::NxActorFlag) shift);
			if (!str)
				break;
		
			const char *dummy = strstr(str,"DUMMY");
			
			if (dummy)
				continue;
		
			bool b = false;
			if ( def->flags & shift )
				b = true;

			fl[str] = (bool) b;

			actor[NX_JSON_Flags] = fl;
		}

	}
*/
	if (!def[NX_JSON_ActorGroupName].isNull()) {
		//actor->group = __getGroupID(mCollection->mCurrentScene->mId, def[NX_JSON_ActorGroupName]);
	}

	#if NX_SDK_VERSION_NUMBER >= 260
	if (!def[NX_JSON_CompartmentName].isNull()) {
		__fetchString(actor->mCompartment, NX_JSON_CompartmentName, def);
	}
	#endif

	  
	#if NX_SDK_VERSION_NUMBER >= 272
	if (!def[NX_JSON_DominanceGroupName].isNull()) {
		//actor->group = __getDominanceGroupID(mCollection->mCurrentScene->mId, def[NX_JSON_DominanceGroupName]);
		//__fetchString(actor->dominanceGroup, NX_JSON_CompartmentName, def);
	}
	#endif

	/////////////////////
	  
	Json::Value shapes = def[NX_JSON_SECTION_Shapes];

	for (Json::ValueIterator it = shapes.begin(); it != shapes.end(); it++) {

		if ((*it)[NX_JSON_Type].isNull())
			continue;

		NxString type = (*it)[NX_JSON_Type].asString();

		
		if (type == NX_JSON_PlaneShape) {
			actor->mShapes.push_back(__readPlaneShape((*it)));
			continue;
		}

		if (type == NX_JSON_SphereShape) {
			actor->mShapes.push_back(__readSphereShape((*it)));
			continue;
		}

		if (type == NX_JSON_BoxShape) {
			actor->mShapes.push_back(__readBoxShape((*it)));
			continue;
		}

		if (type == NX_JSON_CapsuleShape) {
			actor->mShapes.push_back(__readCapsuleShape((*it)));
			continue;
		}

		if (type == NX_JSON_ConvexShape) {
			actor->mShapes.push_back(__readConvexShape((*it)));
			continue;
		}

		if (type == NX_JSON_TriangleMesh) {
			actor->mShapes.push_back(__readTriangleMeshShape((*it)));
			continue;
		}

		/*

			case NXU::SC_NxWheelShapeDesc:
			case NXU::SC_NxConvexShapeDesc:
			case NXU::SC_NxTriangleMeshShapeDesc:
			case NXU::SC_NxHeightFieldShapeDesc:
		*/

	}
	
	

	if (def[NX_JSON_Body].isNull()) {
		actor->mHasBody = false;
	}
	else {

		actor->mBody = NXU::NxBodyDesc();
		actor->mHasBody = true;

		Json::Value bodyDef = def[NX_JSON_Body];

		if (!bodyDef[NX_JSON_MassLocalPose].isNull()) 
			actor->mBody.massLocalPose = __toNxMat34(bodyDef[NX_JSON_MassLocalPose]);

		if (!bodyDef[NX_JSON_Mass].isNull())
			actor->mBody.mass = bodyDef[NX_JSON_Mass].asDouble();

		if (!bodyDef[NX_JSON_MassSpaceInertia].isNull())
			actor->mBody.massSpaceInertia = __toNxVec3(bodyDef[NX_JSON_MassSpaceInertia]);

		if (!bodyDef[NX_JSON_LinearVelocity].isNull())
			actor->mBody.linearVelocity = __toNxVec3(bodyDef[NX_JSON_LinearVelocity]);

		if (!bodyDef[NX_JSON_AngularVelocity].isNull())
			actor->mBody.angularVelocity = __toNxVec3(bodyDef[NX_JSON_AngularVelocity]);

		if (!bodyDef[NX_JSON_WakeUpCounter].isNull())
			actor->mBody.wakeUpCounter = bodyDef[NX_JSON_WakeUpCounter].asDouble();

		if (!bodyDef[NX_JSON_LinearDamping].isNull())
			actor->mBody.linearDamping = bodyDef[NX_JSON_LinearDamping].asDouble();

		if (!bodyDef[NX_JSON_AngularDamping].isNull())
			actor->mBody.angularDamping = bodyDef[NX_JSON_AngularDamping].asDouble();

		if (!bodyDef[NX_JSON_MaxAngularVelocity].isNull())
			actor->mBody.maxAngularVelocity = bodyDef[NX_JSON_MaxAngularVelocity].asDouble();

		if (!bodyDef[NX_JSON_CCDMotionThreshold].isNull())
			actor->mBody.CCDMotionThreshold = bodyDef[NX_JSON_CCDMotionThreshold].asDouble();


		// (TODO) Flags
#if 0

		if (bodyDef->flags) {
			Json::Value fl;
			for (int i=0; i<32; i++) {
				int shift = (1<<i);
				const char *str = NXU::EnumToString( (NXU::NxBodyFlag) shift);
				if (!str)
					break;
		
				const char *dummy = strstr(str,"DUMMY");
				
				if (dummy)
					continue;
			
				bool b = false;
				if ( bodyDef->flags & shift )
					b = true;

				fl[str] = (bool) b;

				body[NX_JSON_Flags] = fl;
			}

		}
#endif
		if (!bodyDef[NX_JSON_SleepLinearVelocity].isNull())
			actor->mBody.sleepLinearVelocity = bodyDef[NX_JSON_SleepLinearVelocity].asDouble();

		if (!bodyDef[NX_JSON_SleepAngularVelocity].isNull())
			actor->mBody.sleepAngularVelocity = bodyDef[NX_JSON_SleepAngularVelocity].asDouble();

		if (!bodyDef[NX_JSON_SolverIterationCount].isNull())
			actor->mBody.solverIterationCount = bodyDef[NX_JSON_SolverIterationCount].asUInt();

	#if NX_SDK_VERSION_NUMBER >= 250
		if (!bodyDef[NX_JSON_SleepEnergyThreshold].isNull())
			actor->mBody.sleepEnergyThreshold = bodyDef[NX_JSON_SleepEnergyThreshold].asDouble();
	#endif

	#if NX_SDK_VERSION_NUMBER >= 250
		if (!bodyDef[NX_JSON_SleepDamping].isNull())
			actor->mBody.sleepDamping = bodyDef[NX_JSON_SleepDamping].asDouble();
	#endif

	}


	return actor;

}

///////////////////////////////////////////////////////////////////

void NxJSONCollection::__readShape(Json::Value& def, NXU::NxShapeDesc* shape) {

	
#if 0
	if (def->mUserProperties) {
		StringPairList l;
		l.parseCSString(def->mUserProperties);
		
		Json::Value user;
		for (StringPairList::StringPair sp = l.begin();l.hasNext();) {
			sp = l.next();
			user[sp.first] = sp.second;
		}

		shape[NX_JSON_SECTION_User] = user;
	}
#endif

	if (!def[NX_JSON_ID].isNull())
		__fetchString(shape->name, NX_JSON_ID, def);
	
	if (!def[NX_JSON_LocalPose].isNull())
		shape->localPose = __toNxMat34(def[NX_JSON_LocalPose]);
	
#if 0
	/// Fix ish.
	if (def->shapeFlags != defaultShape.shapeFlags) {

		Json::Value fl;

		for (int i=0; i<32; i++) {
			int shift = (1<<i);
			const char *str = NXU::EnumToString( (NXU::NxShapeFlag) shift);
			if (!str)
				break;
		
			const char *dummy = strstr(str,"DUMMY");
			
			if (dummy)
				continue;
		
			bool b = false;
			if ( def->shapeFlags & shift )
				b = true;

			fl[str] = (bool) b;

			shape[NX_JSON_Flags] = fl;
		}

	}
#endif

	if (!def[NX_JSON_ShapeGroup].isNull()) {
		//shape->group = __getGroupID(mCollection->mCurrentScene->mId, def[NX_JSON_ShapeGroup]);
	}

	if (!def[NX_JSON_Material].isNull()) {
		//shape->material = __getMaterialID(mCollection->mCurrentScene->mId, def[NX_JSON_Material]);
	}

	
	if (!def[NX_JSON_Skeleton].isNull())
		__fetchString(shape->mCCDSkeleton, NX_JSON_Skeleton, def);

	if (!def[NX_JSON_Density].isNull())
		shape->mShapeDensity = def[NX_JSON_Density].asDouble();

	if (!def[NX_JSON_Mass].isNull())
		shape->mShapeMass = def[NX_JSON_Mass].asDouble();

	if (!def[NX_JSON_SkinWidth].isNull())
		shape->skinWidth = def[NX_JSON_SkinWidth].asDouble();

	if (!def[NX_JSON_GroupsMask].isNull()) {
		Json::Value gm = def[NX_JSON_GroupsMask];
		if (gm.size() == 4) {
			shape->groupsMask.bits0 = gm[NxU32(0)].asUInt();
			shape->groupsMask.bits1 = gm[NxU32(1)].asUInt();
			shape->groupsMask.bits2 = gm[NxU32(2)].asUInt();
			shape->groupsMask.bits3 = gm[NxU32(3)].asUInt();
		}
	}


#if 0
	if (def->groupsMask.bits0 !=0 && def->groupsMask.bits0 !=1 && def->groupsMask.bits2 !=0 || def->groupsMask.bits3 !=0) {
		Json::Value gm;
		gm.append(Json::Value(def->groupsMask.bits0));
		gm.append(Json::Value(def->groupsMask.bits1));
		gm.append(Json::Value(def->groupsMask.bits2));
		gm.append(Json::Value(def->groupsMask.bits3));
		shape[NX_JSON_GroupsMask] = gm;
	}
#endif

}

///////////////////////////////////////////////////////////////////

NXU::NxPlaneShapeDesc*	NxJSONCollection::__readPlaneShape(Json::Value& def) {

	NXU::NxPlaneShapeDesc* shape = new NXU::NxPlaneShapeDesc;
	__readShape(def, shape);
	
	if (!def[NX_JSON_Distance].isNull())
		shape->d = def[NX_JSON_Distance].asDouble();

	if (!def[NX_JSON_Normal].isNull())
		shape->normal = __toNxVec3(def[NX_JSON_Normal]);

	return shape;
}

///////////////////////////////////////////////////////////////////

NXU::NxSphereShapeDesc* NxJSONCollection::__readSphereShape(Json::Value& def) {
	
	NXU::NxSphereShapeDesc* shape = new NXU::NxSphereShapeDesc;
	__readShape(def, shape);

	if (!def[NX_JSON_Radius].isNull())
		shape->radius = def[NX_JSON_Radius].asDouble();

	return shape;
}

///////////////////////////////////////////////////////////////////

NXU::NxBoxShapeDesc*	NxJSONCollection::__readBoxShape(Json::Value& def) {

	NXU::NxBoxShapeDesc* shape = new NXU::NxBoxShapeDesc;
	__readShape(def, shape);

	if (!def[NX_JSON_Dimensions].isNull())
		shape->dimensions = __toNxVec3(def[NX_JSON_Dimensions]);

	return shape;
}

///////////////////////////////////////////////////////////////////

NXU::NxCapsuleShapeDesc* NxJSONCollection::__readCapsuleShape(Json::Value &def) {
	
	NXU::NxCapsuleShapeDesc* shape = new NXU::NxCapsuleShapeDesc;
	__readShape(def, shape);

	if (!def[NX_JSON_Radius].isNull())
		shape->radius = def[NX_JSON_Radius].asDouble();

	if (!def[NX_JSON_Height].isNull())
		shape->height = def[NX_JSON_Height].asDouble();

	// (TODO) Flags here.


	return shape;
}

///////////////////////////////////////////////////////////////////

NXU::NxConvexShapeDesc*	NxJSONCollection::__readConvexShape(Json::Value& def)  {

	NXU::NxConvexShapeDesc* shape = new NXU::NxConvexShapeDesc;
	__readShape(def, shape);

	if (!def[NX_JSON_MeshName].isNull())
		__fetchString(shape->mMeshData, NX_JSON_MeshName, def);

	// (TODO) Flags here.


	return shape;
}

///////////////////////////////////////////////////////////////////

NXU::NxTriangleMeshShapeDesc*	NxJSONCollection::__readTriangleMeshShape(Json::Value& def)  {

	NXU::NxTriangleMeshShapeDesc* shape = new NXU::NxTriangleMeshShapeDesc;
	__readShape(def, shape);

	if (!def[NX_JSON_MeshName].isNull())
		__fetchString(shape->mMeshData, NX_JSON_MeshName, def);

	// (TODO) Flags here.


	return shape;
}

//////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.

#endif
