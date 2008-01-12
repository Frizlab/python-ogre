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

/////////////////////////////////////////////////

void NxJSONCollection::exportCollection(NXU::NxuPhysicsCollection* collection) {
	mRoot.setComment("// NxJSON Exported World.", Json::commentBefore);
	
	__writeVersion();
	//__writeClothMeshes(NxJSONCollection->mClothMeshes);
	__writeConvexMeshes(collection->mConvexMeshes);
	__writeScenes(collection->mScenes);
	// __writeHeightFields(NxJSONCollection->mHeightFields);
	// __writeParameters(NxJSONCollection->mParameters);
	// __writeSkeletons(NxJSONCollection->mSkeletons);
	// __writeSoftBody(NxJSONCollection->mSoftBodyMeshes);
	__writeTriangluarMeshes(collection->mTriangleMeshes);
	// __writeUserProperties(NxJSONCollection->mUserProperties);
	
	mRoot[NX_JSON_SECTION_Meshes] = mMeshSection;
}

/////////////////////////////////////////////////

void NxJSONCollection::exportCollection(NxString file, NXU::NxuPhysicsCollection* collection) {
	
	Json::StyledWriter writer;

	exportCollection(collection);

	NxString str = writer.write(mRoot);
	
	std::fstream fileOut;
	fileOut.open(file.c_str(), std::ios::out);
	fileOut << writer.write(mRoot);
	fileOut.close();
}

/////////////////////////////////////////////////

void NxJSONCollection::__writeVersion() {

	Json::Value section;

	section[NX_JSON_NxOgre] = Json::Value(Nx_Version_Full);

	std::stringstream nxjsonVersion;
	nxjsonVersion << NX_JSON_SCHEMA_ID;
	section[NX_JSON_NxJSON]  = Json::Value(nxjsonVersion.str());

	std::stringstream ogreVersion;
	ogreVersion << OGRE_VERSION_MAJOR << "." << OGRE_VERSION_MINOR << "." << OGRE_VERSION_PATCH << " '" << OGRE_VERSION_NAME << "'";
	section[NX_JSON_Ogre]  = Json::Value(ogreVersion.str());

	std::stringstream physxVersion;
	physxVersion << NX_SDK_VERSION_NUMBER;
	section[NX_JSON_PhysX] = Json::Value(physxVersion.str());

	mRoot[NX_JSON_SECTION_Version] = section;
}

/////////////////////////////////////////////////

void NxJSONCollection::__writeScenes(NxArray<NXU::NxSceneDesc*> &scenes) {
	
	Json::Value section;

	NxSceneDesc defaultScene;
	defaultScene.setToDefault();
				
	for (NxU32 i=0;i < scenes.size();i++) {
		NXU::NxSceneDesc* def = scenes[i];
		Json::Value scene;	

		Scene* workingScene = mWorld->getScenes()->get(def->mId);

		std::stringstream sceneComment;
		sceneComment << "// Scene '" << NxString(def->mId) << "'.";
		scene.setComment(sceneComment.str(), Json::commentBefore);

		if (def->mId)
			scene[NX_JSON_ID] = def->mId;

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

/*				// Could be duplicated data
	
		if (def->mHasMaxBounds)
			scene[NX_JSON_HasMaxBounds] = (bool) def->mHasMaxBounds;
		
		if (def->mHasLimits)
			scene[NX_JSON_HasLimits] = (bool) def->mHasLimits;

		if (def->mHasFilter)
			scene[NX_JSON_HasFilter] = (bool) def->mHasFilter;

		if (def->mFilterBool)
			scene[NX_JSON_FilterBool] = (bool) def->mFilterBool;
*/
		if (def->mFilterOp0)
			scene[NX_JSON_FilterOp0] = NXU::EnumToString(def->mFilterOp0);

		if (def->mFilterOp1)
			scene[NX_JSON_FilterOp1] = NXU::EnumToString(def->mFilterOp1);

		if (def->mFilterOp2)
			scene[NX_JSON_FilterOp2] = NXU::EnumToString(def->mFilterOp2);

		{
				Json::Value gm0;
				gm0.append(Json::Value(def->mGroupMask0.bits0));
				gm0.append(Json::Value(def->mGroupMask0.bits1));
				gm0.append(Json::Value(def->mGroupMask0.bits2));
				gm0.append(Json::Value(def->mGroupMask0.bits3));
				scene[NX_JSON_GroupMask0] = gm0;
		}

		{
			Json::Value gm1;
			gm1.append(Json::Value(def->mGroupMask1.bits0));
			gm1.append(Json::Value(def->mGroupMask1.bits1));
			gm1.append(Json::Value(def->mGroupMask1.bits2));
			gm1.append(Json::Value(def->mGroupMask1.bits3));
			scene[NX_JSON_GroupMask1] = gm1;
		}

		if (!def->gravity.isZero())
			scene[NX_JSON_Gravity] = __fromNxVec3(def->gravity);

		if (def->maxTimestep != defaultScene.maxTimestep)
			scene[NX_JSON_MaxTimeStep] = def->maxTimestep;

		if (def->maxIter != defaultScene.maxIter)
			scene[NX_JSON_MaxIter] = def->maxIter;

		if (def->timeStepMethod != defaultScene.timeStepMethod)
			scene[NX_JSON_TimeStepMethod] = NXU::EnumToString(def->timeStepMethod);
		
		{
			Json::Value mb;
			mb.append(def->mMaxBounds.min.x);
			mb.append(def->mMaxBounds.min.y);
			mb.append(def->mMaxBounds.min.z);
			mb.append(def->mMaxBounds.max.x);
			mb.append(def->mMaxBounds.max.y);
			mb.append(def->mMaxBounds.max.z);
			scene[NX_JSON_MaxBounds] = mb;
		}

		if (def->simType != defaultScene.simType)
			scene[NX_JSON_SimType] = NXU::EnumToString(def->simType);

		if (def->groundPlane != defaultScene.groundPlane)
			scene[NX_JSON_GroundPlane] = (bool) def->groundPlane;

		if (def->boundsPlanes != defaultScene.boundsPlanes)
			scene[NX_JSON_BoundsPlanes] = (bool) def->boundsPlanes;

		if (def->flags != defaultScene.flags)
		{
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

		if (def->internalThreadCount != defaultScene.internalThreadCount)
			scene[NX_JSON_InternalThreadCount] = def->internalThreadCount;
		
		if (def->backgroundThreadCount != defaultScene.backgroundThreadCount)
			scene[NX_JSON_BackgroundThreadCount] = def->backgroundThreadCount;
		
		if (def->threadMask != defaultScene.threadMask)
			scene[NX_JSON_ThreadMask] = def->threadMask;
		
		if (def->backgroundThreadMask != defaultScene.backgroundThreadMask)
			scene[NX_JSON_BackgroundThreadMask] = def->backgroundThreadMask;

		#if NX_SDK_VERSION_NUMBER >= 250
		if (def->simThreadStackSize != defaultScene.simThreadStackSize)
			scene[NX_JSON_SimThreadStackSize] = def->simThreadStackSize;
		#endif

		#if NX_SDK_VERSION_NUMBER >= 260
		if (def->simThreadPriority != defaultScene.simThreadPriority)
			scene[NX_JSON_SimThreadPriority] = NXU::EnumToString(def->simThreadPriority);
		#endif
		
		#if NX_SDK_VERSION_NUMBER >= 250
		  if (def->workerThreadStackSize != defaultScene.workerThreadStackSize)
			scene[NX_JSON_WorkerThreadStackSize] = def->workerThreadStackSize;
		#endif

		#if NX_SDK_VERSION_NUMBER >= 250
		   if (def->workerThreadPriority != defaultScene.workerThreadPriority)
			   scene[NX_JSON_WorkerThreadPriority] = NXU::EnumToString(def->workerThreadPriority);
		#endif

		#if NX_SDK_VERSION_NUMBER >= 262
		  if (def->upAxis != defaultScene.upAxis)
			scene[NX_JSON_UpAxis] = def->upAxis;
		#endif

		#if NX_SDK_VERSION_NUMBER >= 262
		  if (def->subdivisionLevel != defaultScene.subdivisionLevel)
			scene[NX_JSON_SubdivisionLevel] = def->subdivisionLevel;
		#endif

		#if NX_SDK_VERSION_NUMBER >= 262
		if (def->staticStructure != defaultScene.staticStructure)
			scene[NX_JSON_StaticStructure] = NXU::EnumToString(def->staticStructure);
		#endif

		#if NX_SDK_VERSION_NUMBER >= 262
		if (def->dynamicStructure != defaultScene.dynamicStructure)
			scene[NX_JSON_DynamicStructure] = EnumToString(def->dynamicStructure);
		#endif


		//////////////////////////////////////////////////////

		Json::Value materials;

		for (NxU32 i=0; i< def->mMaterials.size(); i++) {
			
			Json::Value material = __writeMaterial(def->mMaterials[i], workingScene);
			materials.append(material);
		}

		scene[NX_JSON_SECTION_Materials] = materials;

		//////////////////////////////////////////////////////


		Json::Value actors;

		for (NxU32 i=0; i< def->mActors.size(); i++) {
		
			Json::Value actor = __writeActor(def->mActors[i], workingScene);
			
			NxString actorID = actor.get(NX_JSON_ID,"Default").asString();
			actor.removeMember(NX_JSON_ID);
		
			{	std::stringstream ss;
				ss << "// Actor '" << def->mId << "'";
				actor.setComment(ss.str(), Json::commentBefore);
			}
			actors[actorID] = actor;
		}

		scene[NX_JSON_SECTION_Actors] = actors;

		//////////////////////////////////////////////////////

		section.append(scene);

	}

	mRoot["Scenes"] = section;
}

//////////////////////////////////////////////////////

Json::Value NxJSONCollection::__writeMaterial(NXU::NxMaterialDesc* def, Scene* scene) {
	Json::Value material;
	
	NxMaterialDesc defaultMaterial;
	defaultMaterial.setToDefault();

	if (def->mId == 0)
		material[NX_JSON_ID] = def->mMaterialIndex;
	else
		material[NX_JSON_ID] = def->mId;

	if (def->mHasSpring)
		material[NX_JSON_Spring] = __writeSpring(&def->mSpring);
	
	material[NX_JSON_MaterialIndex] = def->mMaterialIndex;
	
	if (def->dynamicFriction != defaultMaterial.dynamicFriction)
		material[NX_JSON_DynamicFriction] = def->dynamicFriction;
	
	if (def->staticFriction != defaultMaterial.staticFriction)
		material[NX_JSON_StaticFriction] = def->staticFriction;
	
	if (def->restitution != defaultMaterial.restitution)
		material[NX_JSON_Restitution] = def->restitution;

	if (def->dynamicFrictionV != defaultMaterial.dynamicFrictionV)
		material[NX_JSON_DynamicFrictionV] = def->dynamicFrictionV;

	if (def->staticFrictionV != defaultMaterial.staticFrictionV)
		material[NX_JSON_StaticFrictionV] = def->staticFrictionV;
	
	if (def->frictionCombineMode != defaultMaterial.frictionCombineMode)
		material[NX_JSON_FrictionCombineMode] = NXU::EnumToString(def->frictionCombineMode);

	if (def->restitutionCombineMode != defaultMaterial.restitutionCombineMode)
		 material[NX_JSON_RestitutionCombineMode] = NXU::EnumToString(def->restitutionCombineMode);

	 if (!def->dirOfAnisotropy.isZero())
		material[NX_JSON_DirOfAnisotropy] = __fromNxVec3(def->dirOfAnisotropy);

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

	return material;
}

//////////////////////////////////////////////////////

Json::Value NxJSONCollection::__writeSpring(NXU::NxSpringDesc* def) {

	Json::Value spring;

	if (def->spring)
		spring[NX_JSON_Spring] = def->spring;

	if (def->damper)
		spring[NX_JSON_Damper] = def->damper;

	if (def->targetValue)
		spring[NX_JSON_TargetValue] = def->targetValue;

	return spring;
}

//////////////////////////////////////////////////////

Json::Value NxJSONCollection::__writeActor(NXU::NxActorDesc* def, Scene* scene) {

	Json::Value actor;

	if (def->mId)
		actor[NX_JSON_ID] = def->mId;

	if (def->mUserProperties) {
		StringPairList l;
		l.parseCSString(def->mUserProperties);
		
		Json::Value user;
		for (StringPairList::StringPair sp = l.begin();l.hasNext();) {
			sp = l.next();
			user[sp.first] = sp.second;
		}

		actor[NX_JSON_SECTION_User] = user;
	}

	actor[NX_JSON_GlobalPose] = __fromNxMat34(def->globalPose);

	if (def->density)
		actor[NX_JSON_Density] = def->density;

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

	if (def->group)
		actor[NX_JSON_ActorGroupName] = scene->getActorGroupsByIndex()->get(def->group)->getName();

	#if NX_SDK_VERSION_NUMBER >= 260
	  if (def->mCompartment)
		actor[NX_JSON_CompartmentName] = def->mCompartment;
	#endif

	  
	#if NX_SDK_VERSION_NUMBER >= 272
	  if (def->dominanceGroup)
		actor[NX_JSON_DominanceGroupName] = def->dominanceGroup;	// For Future turn to a string name.
	#endif

	/////////////////////
	  
	Json::Value shapes;


	for (NxU32 i=0; i<def->mShapes.size(); i++) {
		NXU::NxShapeDesc *v = def->mShapes[i];
		switch ( v->mType ) {
			case NXU::SC_NxPlaneShapeDesc:
				{
					NXU::NxPlaneShapeDesc *p = static_cast<NXU::NxPlaneShapeDesc *>(v);
					shapes.append(__writePlaneShape(p, scene));
				}
			break;

			case NXU::SC_NxSphereShapeDesc:
				{
					NXU::NxSphereShapeDesc *p = static_cast<NXU::NxSphereShapeDesc *>(v);
					shapes.append(__writeSphereShape(p, scene));
				}
			break;

			case NXU::SC_NxBoxShapeDesc:
				{
					NXU::NxBoxShapeDesc *p = static_cast<NXU::NxBoxShapeDesc *>(v);
					shapes.append(__writeBoxShape(p, scene));
				}
			break;

			case NXU::SC_NxCapsuleShapeDesc:
				{
					NXU::NxCapsuleShapeDesc *p = static_cast<NXU::NxCapsuleShapeDesc *>(v);
					shapes.append(__writeCapsuleShape(p, scene));
				}
			break;

			case NXU::SC_NxWheelShapeDesc:
				{
					NXU::NxWheelShapeDesc *p = static_cast<NXU::NxWheelShapeDesc *>(v);
					shapes.append(__writeWheelShape(p, scene));
				}
			break;
			
			case NXU::SC_NxConvexShapeDesc:
				{
					NXU::NxConvexShapeDesc *p = static_cast<NXU::NxConvexShapeDesc *>(v);
					shapes.append(__writeConvexShape(p, scene));
				}
			break;
			
			case NXU::SC_NxTriangleMeshShapeDesc:
				{
					NXU::NxTriangleMeshShapeDesc *p = static_cast<NXU::NxTriangleMeshShapeDesc *>(v);
					shapes.append(__writeTriangleMeshShape(p, scene));
				}
			break;	

			case NXU::SC_NxHeightFieldShapeDesc:
				{
					NXU::NxHeightFieldShapeDesc *p = static_cast<NXU::NxHeightFieldShapeDesc *>(v);
					shapes.append(__writeHeightFieldShape(p, scene));
				}
			break;

		}
	}

	actor[NX_JSON_SECTION_Shapes] = shapes;

	/////////////////

	if (def->mHasBody) {

		Json::Value body;
		NxBodyDesc bodyDefault;
		bodyDefault.setToDefault();

		if (!__sameAs(def->mBody.massLocalPose, bodyDefault.massLocalPose))
			body[NX_JSON_MassLocalPose] = __fromNxMat34(def->mBody.massLocalPose);

		if (def->mBody.massSpaceInertia != bodyDefault.massSpaceInertia)
			body[NX_JSON_MassSpaceInertia] = __fromNxVec3(def->mBody.massSpaceInertia);

		if (def->mBody.mass != bodyDefault.mass)
			body[NX_JSON_Mass] = def->mBody.mass;

		if (def->mBody.linearVelocity != bodyDefault.linearVelocity)
			body[NX_JSON_LinearVelocity] = __fromNxVec3(def->mBody.linearVelocity);

		if (def->mBody.angularVelocity != bodyDefault.angularVelocity)
			body[NX_JSON_AngularVelocity] = __fromNxVec3(def->mBody.angularVelocity);

		if (def->mBody.wakeUpCounter != bodyDefault.wakeUpCounter)
			body[NX_JSON_WakeUpCounter] = def->mBody.wakeUpCounter;

		if (def->mBody.wakeUpCounter != bodyDefault.wakeUpCounter)
			body[NX_JSON_WakeUpCounter] = def->mBody.wakeUpCounter;

		if (def->mBody.linearDamping != bodyDefault.linearDamping)
			body[NX_JSON_LinearDamping] = def->mBody.linearDamping;

		if (def->mBody.angularDamping != bodyDefault.angularDamping)
			body[NX_JSON_AngularDamping] = def->mBody.angularDamping;

		if (def->mBody.maxAngularVelocity != bodyDefault.maxAngularVelocity)
			body[NX_JSON_MaxAngularVelocity] = def->mBody.maxAngularVelocity;

		if (def->mBody.CCDMotionThreshold != bodyDefault.CCDMotionThreshold)
			body[NX_JSON_CCDMotionThreshold] = def->mBody.CCDMotionThreshold;

		if (def->flags) {

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
				if ( def->flags & shift )
					b = true;

				fl[str] = (bool) b;

				body[NX_JSON_Flags] = fl;
			}

		}

		if (def->mBody.sleepLinearVelocity != bodyDefault.sleepLinearVelocity)
			body[NX_JSON_SleepLinearVelocity] = def->mBody.sleepLinearVelocity;

		if (def->mBody.sleepAngularVelocity != bodyDefault.sleepAngularVelocity)
			body[NX_JSON_SleepAngularVelocity] = def->mBody.sleepAngularVelocity;

		if (def->mBody.solverIterationCount != bodyDefault.solverIterationCount)
			body[NX_JSON_SolverIterationCount] = def->mBody.solverIterationCount;

	#if NX_SDK_VERSION_NUMBER >= 250
		if (def->mBody.sleepEnergyThreshold != bodyDefault.sleepEnergyThreshold)
			body[NX_JSON_SleepEnergyThreshold] = def->mBody.sleepEnergyThreshold;
	#endif

	#if NX_SDK_VERSION_NUMBER >= 250
		if (def->mBody.sleepDamping != bodyDefault.sleepDamping)
			body[NX_JSON_SleepDamping] = def->mBody.sleepDamping;
	#endif


		actor[NX_JSON_Body] = body;
	}

	////////////////

	return actor;
}

///////////////////////////////////////////////////////////////////

void NxJSONCollection::__writeShape(NXU::NxShapeDesc* def, Json::Value& shape, NxShapeDesc& defaultShape, Scene* scene) {

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

	if (def->name)
		shape[NX_JSON_ID] = def->name;

	
	if (!__sameAs(def->localPose, defaultShape.localPose))
		shape[NX_JSON_LocalPose] =__fromNxMat34(def->localPose);
	
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

	if (def->group != defaultShape.group) {
		shape[NX_JSON_ShapeGroup] =	scene->getShapeGroupsByIndex()->get(def->group)->getName();
	}

	if (def->materialIndex != defaultShape.materialIndex) {
		shape[NX_JSON_Material] = scene->getMaterial(def->materialIndex)->getName();
	}

	if (def->mCCDSkeleton)
		shape[NX_JSON_Skeleton] = def->mCCDSkeleton;

	if (def->mShapeDensity != defaultShape.density)
		shape[NX_JSON_Density] = def->mShapeDensity;

	if (def->mShapeMass != defaultShape.mass)
		shape[NX_JSON_Mass] = def->mShapeMass;

	if (def->skinWidth != defaultShape.skinWidth)
		shape[NX_JSON_SkinWidth] = def->skinWidth;

	if (def->groupsMask.bits0 !=0 && def->groupsMask.bits0 !=1 && def->groupsMask.bits2 !=0 || def->groupsMask.bits3 !=0) {
		Json::Value gm;
		gm.append(Json::Value(def->groupsMask.bits0));
		gm.append(Json::Value(def->groupsMask.bits1));
		gm.append(Json::Value(def->groupsMask.bits2));
		gm.append(Json::Value(def->groupsMask.bits3));
		shape[NX_JSON_GroupsMask] = gm;
	}

}

///////////////////////////////////////////////////////////////////

Json::Value NxJSONCollection::__writePlaneShape(NXU::NxPlaneShapeDesc* def, Scene* scene) {

	Json::Value shape;

	shape[NX_JSON_Type] = NX_JSON_PlaneShape;

	NxPlaneShapeDesc defaultShape;
	defaultShape.setToDefault();
	__writeShape(def, shape, defaultShape, scene);

	if (def->normal != defaultShape.normal)
		shape[NX_JSON_Normal] = __fromNxVec3(def->normal);
	
	if (def->d != defaultShape.d)
		shape[NX_JSON_Distance] = def->d;

	return shape;
}


///////////////////////////////////////////////////////////////////

Json::Value NxJSONCollection::__writeSphereShape(NXU::NxSphereShapeDesc* def, Scene* scene) {

	Json::Value shape;

	shape[NX_JSON_Type] = NX_JSON_SphereShape;
		
	NxSphereShapeDesc defaultShape;
	defaultShape.setToDefault();
	__writeShape(def, shape, defaultShape, scene);

	if (def->radius != defaultShape.radius)
		shape[NX_JSON_Radius] = def->radius;

	return shape;
}

///////////////////////////////////////////////////////////////////

Json::Value NxJSONCollection::__writeBoxShape(NXU::NxBoxShapeDesc* def, Scene* scene) {

	Json::Value shape;

	shape[NX_JSON_Type] = NX_JSON_BoxShape;

	NxBoxShapeDesc defaultShape;
	defaultShape.setToDefault();
	__writeShape(def, shape, defaultShape, scene);

	if (def->dimensions != defaultShape.dimensions)
		shape[NX_JSON_Dimensions] = __fromNxVec3(def->dimensions);

	return shape;
}


///////////////////////////////////////////////////////////////////

Json::Value NxJSONCollection::__writeCapsuleShape(NXU::NxCapsuleShapeDesc* def, Scene* scene) {

	Json::Value shape;

	shape[NX_JSON_Type] = NX_JSON_CapsuleShape;

	NxCapsuleShapeDesc defaultShape;
	defaultShape.setToDefault();
	__writeShape(def, shape, defaultShape, scene);

	if (def->height != defaultShape.height)
		shape[NX_JSON_Height] = def->height;

	if (def->radius != defaultShape.radius)
		shape[NX_JSON_Radius] = def->radius;

	if (def->shapeFlags != defaultShape.shapeFlags) {

		Json::Value fl;

		for (int i=0; i<32; i++) {
			int shift = (1<<i);
			const char *str = NXU::EnumToString( (NXU::NxCapsuleShapeFlag) shift);
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


	return shape;
}

///////////////////////////////////////////////////////////////////

Json::Value NxJSONCollection::__writeWheelShape(NXU::NxWheelShapeDesc* def, Scene* scene) {

	Json::Value shape;

	shape[NX_JSON_Type] = NX_JSON_WheelShape;

	NxWheelShapeDesc defaultShape;
	defaultShape.setToDefault();
	__writeShape(def, shape, defaultShape, scene);

	
	if (def->radius != defaultShape.radius)
		shape[NX_JSON_Radius] = def->radius;

	if (def->suspensionTravel != defaultShape.suspensionTravel)
		shape[NX_JSON_SuspensionTravel] = def->suspensionTravel;

	shape[NX_JSON_Suspension] = __writeSpring(&def->suspension);

	shape[NX_JSON_LongitudalTireForceFunction] = __writeTireFunction(&def->longitudalTireForceFunction, scene);
	shape[NX_JSON_LateralTireForceFunction] = __writeTireFunction(&def->lateralTireForceFunction, scene);

	if (def->inverseWheelMass != defaultShape.inverseWheelMass)
		shape[NX_JSON_InverseWheelMass] = def->inverseWheelMass;

	if (def->inverseWheelMass != defaultShape.inverseWheelMass)
		shape[NX_JSON_InverseWheelMass] = def->inverseWheelMass;

	if (def->wheelFlags != defaultShape.wheelFlags) {
		
		Json::Value fl;

		for (int i=0; i<32; i++) {
			int shift = (1<<i);
			const char *str = NXU::EnumToString( (NXU::NxWheelShapeFlags) shift);
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

	if (def->motorTorque != defaultShape.motorTorque)
		shape[NX_JSON_MotorTorque] = def->motorTorque;

	if (def->brakeTorque != defaultShape.brakeTorque)
		shape[NX_JSON_BrakeTorque] = def->brakeTorque;

	if (def->steerAngle != defaultShape.steerAngle)
		shape[NX_JSON_SteerAngle] = def->steerAngle;

	return shape;
}

///////////////////////////////////////////////////////////////////

Json::Value NxJSONCollection::__writeTireFunction(NXU::NxTireFunctionDesc* def, Scene* scene) {

	Json::Value tire;

	NxTireFunctionDesc defaultTire;
	defaultTire.setToDefault();

	if (def->extremumSlip != defaultTire.extremumSlip)
		tire[NX_JSON_ExtremumSlip] = def->extremumSlip;

	if (def->extremumValue != defaultTire.extremumValue)
		tire[NX_JSON_ExtremumValue] = def->extremumValue;

	if (def->asymptoteSlip != defaultTire.asymptoteSlip)
		tire[NX_JSON_AsymptoteSlip] = def->extremumSlip;

	if (def->asymptoteValue != defaultTire.asymptoteValue)
		tire[NX_JSON_AsymptoteValue] = def->extremumValue;

	if (def->stiffnessFactor != defaultTire.stiffnessFactor)
		tire[NX_JSON_StiffnessFactor] = def->stiffnessFactor;

	return tire;
}

///////////////////////////////////////////////////////////////////

Json::Value NxJSONCollection::__writeConvexShape(NXU::NxConvexShapeDesc* def, Scene* scene) {
	
	Json::Value shape;

	shape[NX_JSON_Type] = NX_JSON_ConvexShape;

	NxConvexShapeDesc defaultShape;
	defaultShape.setToDefault();
	__writeShape(def, shape, defaultShape, scene);

	shape[NX_JSON_MeshName] = def->mMeshData;

	if (def->meshFlags != defaultShape.meshFlags) {
		
		Json::Value fl;

		for (int i=0; i<32; i++) {
			int shift = (1<<i);
			const char *str = NXU::EnumToString( (NXU::NxMeshShapeFlag) shift);
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
	return shape;

}

///////////////////////////////////////////////////////////////////

void NxJSONCollection::__writeConvexMeshes(NxArray<NXU::NxConvexMeshDesc*> &meshes) {
	
	Json::Value section;

	NxConvexMeshDesc defaultMesh;
	defaultMesh.setToDefault();

	for (NxU32 i=0;i < meshes.size();i++) {
		NXU::NxConvexMeshDesc* def = meshes[i];
		Json::Value mesh;
		
		if (def->mId) {
			mesh[NX_JSON_ID] = def->mId;
		}
		else {
			std::stringstream ss;
			ss << "Unknown Convex #" << i;
			mesh[NX_JSON_ID] = ss.str();
		}

		if (def->mUserProperties) {
			std::cout << "!!! userProperties" << std::endl;
			StringPairList l;
			l.parseCSString(def->mUserProperties);
			
			// Broken????
			for (StringPairList::StringPair sp = l.begin();l.hasNext();) {
				sp = l.next();
				mesh[sp.first] = sp.second;
			}

		}

		{
			Json::Value points;
			
			for (NxU32 i=0; i < def->mPoints.size();i++)
				points[i] = __fromNxVec3(def->mPoints[i]);

			mesh[NX_JSON_Vertices] = points;
		}

		{
			Json::Value triangles;
			
			for (NxU32 i=0; i < def->mTriangles.size();i++)
				triangles[i] = __fromNxTri(def->mTriangles[i]);

			mesh[NX_JSON_Triangles] = triangles;
		}


		if (def->flags != defaultMesh.flags) {
		
			Json::Value fl;

			for (int i=0; i<32; i++) {
				int shift = (1<<i);
				const char *str = NXU::EnumToString( (NXU::NxConvexFlags) shift);
				if (!str)
					break;
			
				const char *dummy = strstr(str,"DUMMY");
				
				if (dummy)
					continue;
			
				bool b = false;
				if ( def->flags & shift )
					b = true;

				fl[str] = (bool) b;

				mesh[NX_JSON_Flags] = fl;
			}

		}

#if NXU_JSON_SAVEDCOOKED

		mesh[NX_JSON_CookedSize] = def->mCookedDataSize;

		Json::Value cookedData;
		for (NxU32 i=0;i < def->mCookedData.size();i++)
			cookedData.append(Json::Value(def->mCookedData[i]));

#endif
		////////////////////////////////////////

		section.append(mesh);

	}

	mMeshSection[NX_JSON_SECTION_Convex] = section;

}

///////////////////////////////////////////////////////////////////

void NxJSONCollection::__writeTriangluarMeshes(NxArray<NXU::NxTriangleMeshDesc*> &meshes) {
	
	Json::Value section;

	NxTriangleMeshDesc defaultMesh;
	defaultMesh.setToDefault();

	for (NxU32 i=0;i < meshes.size();i++) {
		NXU::NxTriangleMeshDesc* def = meshes[i];
		Json::Value mesh;
		
		std::cout << "** Processing tri-mesh" << def->mId << std::endl;

		if (def->mId) {
			mesh[NX_JSON_ID] = def->mId;
		}
		else {
			std::stringstream ss;
			ss << "Unknown Mesh #" << i;
			mesh[NX_JSON_ID] = ss.str();
		}

		if (def->mUserProperties) {

			StringPairList l;
			l.parseCSString(def->mUserProperties);
			
			// Broken????
			for (StringPairList::StringPair sp = l.begin();l.hasNext();) {
				sp = l.next();
				mesh[sp.first] = sp.second;
			}

		}

		{
			Json::Value materials;

			for (NxU32 i=0;i < def->mMaterialIndices.size();i++) {
			  materials[i] = def->mMaterialIndices[i];
			}

			mesh[NX_JSON_MaterialIndices] = materials;
		}

		if (def->heightFieldVerticalAxis != defaultMesh.heightFieldVerticalAxis) {
			mesh[NX_JSON_HeightFieldVerticalAxis] = def->heightFieldVerticalAxis;
		}

		if (def->heightFieldVerticalExtent != defaultMesh.heightFieldVerticalExtent) {
			mesh[NX_JSON_HeightFieldVerticalExtent] = def->heightFieldVerticalExtent;
		}
// Pmaps disabled forever
#if 0
		if (def->mPmapSize) {
			mesh[NX_JSON_PmapSize] = def->mPmapSize;
		}
		
		if (def->mPmapDensity) {
			mesh[NX_JSON_PmapDensity] = def->mPmapDensity;
		}

		if (def->mPmapData) {
			// ... Probably not.
		}
#endif
		
		{
			Json::Value points;
			
			for (NxU32 i=0; i < def->mPoints.size();i++)
				points[i] = __fromNxVec3(def->mPoints[i]);

			mesh[NX_JSON_Vertices] = points;
		}

		{
			Json::Value triangles;
			
			for (NxU32 i=0; i < def->mTriangles.size();i++)
				triangles[i] = __fromNxTri(def->mTriangles[i]);

			mesh[NX_JSON_Triangles] = triangles;
		}


		////////////////////////////////////////

		section.append(mesh);

	}

	mMeshSection[NX_JSON_SECTION_TriangleMesh] = section;

}
///////////////////////////////////////////////////////////////////

Json::Value NxJSONCollection::__writeTriangleMeshShape(NXU::NxTriangleMeshShapeDesc* def, Scene* scene) {

	Json::Value mesh;

	mesh[NX_JSON_Type] = NX_JSON_TriangleMesh;

	NxTriangleMeshShapeDesc defaultMesh;
	defaultMesh.setToDefault();

	if (def->mMeshData) {
		mesh[NX_JSON_MeshName] = def->mMeshData;
	}

	if (def->mUserProperties) {

		StringPairList l;
		l.parseCSString(def->mUserProperties);
		
		// Broken????
		for (StringPairList::StringPair sp = l.begin();l.hasNext();) {
			sp = l.next();
			mesh[sp.first] = sp.second;
		}

	}

	if (def->meshFlags != defaultMesh.meshFlags) {
		
			Json::Value fl;

			for (int i=0; i<32; i++) {
				int shift = (1<<i);
				const char *str = NXU::EnumToString( (NXU::NxMeshShapeFlag) shift);
				if (!str)
					break;
			
				const char *dummy = strstr(str,"DUMMY");
				
				if (dummy)
					continue;
			
				bool b = false;
				if ( def->meshFlags & shift )
					b = true;

				fl[str] = (bool) b;

				mesh[NX_JSON_Flags] = fl;
			}

		}

#if NX_SDK_VERSION_NUMBER >= 250
	if (def->meshPagingMode)
		mesh[NX_JSON_PagingMode] = EnumToString(def->meshPagingMode);
#endif

	return mesh;
}

//////////////////////////////////////////////////////////////

Json::Value NxJSONCollection::__writeHeightFieldShape(NXU::NxHeightFieldShapeDesc* def, Scene* scene) {

	Json::Value heightfield;

	return heightfield;
}

//////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.

#endif
