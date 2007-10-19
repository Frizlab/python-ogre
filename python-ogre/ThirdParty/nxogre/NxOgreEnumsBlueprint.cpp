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
#include "NxOgreEnumsBlueprint.h"
#include "NxOgreContainer.h"

namespace NxOgre {
namespace Blueprints {

////////////////////////////////////////////////////////////////////////////////////////////////

EnumsBlueprint::EnumsBlueprint(EnumType t) : mType(t), mEnum(0) {}

////////////////////////////////////////////////////////////////////////////////////////////////

EnumsBlueprint::EnumsBlueprint(EnumType t, NxU32 f) : mType(t), mEnum(f) {}

////////////////////////////////////////////////////////////////////////////////////////////////

void EnumsBlueprint::toDefault() {
	empty();
}

////////////////////////////////////////////////////////////////////////////////////////////////

void EnumsBlueprint::empty() {
	mEnum = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void EnumsBlueprint::serialise() {

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::asStringPairList() {

	switch(mType) {
		default:							{StringList l;return l;}				break;
		case FT_NxActorFlag:				return AsNxActorFlag();					break;
		case FT_NxShapeFlag:				return AsNxShapeFlag();					break;
		case FT_NxCapsuleShapeFlag:			return AsNxCapsuleShapeFlag();			break;
		case FT_NxWheelShapeFlags:			return AsNxWheelShapeFlags();			break;
		case FT_NxMeshPagingMode:			return AsNxMeshPagingMode();			break;
		case FT_NxJointFlag:				return AsNxJointFlag();					break;
		case FT_NxD6JointMotion:			return AsNxD6JointMotion();				break;
		case FT_NxD6JointDriveType:			return AsNxD6JointDriveType();			break;
		case FT_NxJointProjectionMode:		return AsNxJointProjectionMode();		break;
		case FT_NxD6JointFlag:				return AsNxD6JointFlag();				break;
		case FT_NxDistanceJointFlag:		return AsNxDistanceJointFlag();			break;
		case FT_NxRevoluteJointFlag:		return AsNxRevoluteJointFlag();			break;
		case FT_NxSphericalJointFlag:		return AsNxSphericalJointFlag();		break;
		case FT_NxPulleyJointFlag:			return AsNxPulleyJointFlag();			break;
		case FT_NxContactPairFlag:			return AsNxContactPairFlag();			break;
		
#ifndef NX_DISABLE_FLUIDS
		case FT_NxFluidSimulationMethod:	return AsNxFluidSimulationMethod();		break;
		case FT_NxFluidCollisionMethod:		return AsNxFluidCollisionMethod();		break;
		case FT_NxFluidFlag:				return AsNxFluidFlag();					break;
		case FT_NxFluidEmitterFlag:			return AsNxFluidEmitterFlag();			break;
#endif		
		case FT_NxEmitterType:				return AsNxEmitterType();				break;
		case FT_NxEmitterShape:				return AsNxEmitterShape();				break;
		case FT_NxClothFlag:				return AsNxClothFlag();					break;
		case FT_NxForceFieldCoordinates:	return AsNxForceFieldCoordinates();		break;
		case FT_NxSceneFlags:				return AsNxSceneFlags();				break;
		case FT_NxSoftBodyMeshFlags:		return AsNxSoftBodyMeshFlags();			break;
		case FT_NxSoftBodyVertexFlags:		return AsNxSoftBodyVertexFlags();		break;
		case FT_NxSoftBodyFlag:				return AsNxSoftBodyFlag();				break;
		case FT_NxSoftBodyAttachmentFlag:	return AsNxSoftBodyAttachmentFlag();	break;
		case FT_NxMeshDataFlags:			return AsNxMeshDataFlags();				break;
	}

	StringList l;return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////


StringList EnumsBlueprint::AsNxActorFlag() {

	StringList l;

	 if (mEnum &  NX_AF_DISABLE_COLLISION)
		 l.insert(NxString("NX_AF_DISABLE_COLLISION"));

    if (mEnum &  NX_AF_DISABLE_RESPONSE)
		l.insert(NxString("NX_AF_DISABLE_RESPONSE"));

    if (mEnum &  NX_AF_LOCK_COM)
		l.insert(NxString("NX_AF_LOCK_COM"));
    if (mEnum &  NX_AF_FLUID_DISABLE_COLLISION)
		l.insert(NxString("NX_AF_FLUID_DISABLE_COLLISION"));

#if NX_SDK_VERSION_NUMBER < 250
    if (mEnum &  NX_AF_FLUID_ACTOR_REACTION)
		l.insert(NxString("NX_AF_FLUID_ACTOR_REACTION"));
#endif

#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum &  NX_AF_CONTACT_MODIFICATION)
		l.insert(NxString("NX_AF_CONTACT_MODIFICATION"));
#endif

#if NX_SDK_VERSION_NUMBER >= 261
    if (mEnum &  NX_AF_FORCE_CONE_FRICTION)l.insert(NxString("NX_AF_FORCE_CONE_FRICTION"));
#endif

#if NX_SDK_VERSION_NUMBER >= 270
    if (mEnum &  NX_AF_USER_ACTOR_PAIR_FILTERING)
		l.insert(NxString("NX_AF_USER_ACTOR_PAIR_FILTERING"));
#endif

	return l;
}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxBodyFlag() {

	StringList l;

    if (mEnum & NX_BF_DISABLE_GRAVITY)
		l.insert(NxString("NX_BF_DISABLE_GRAVITY"));
    
	if (mEnum & NX_BF_FROZEN_POS_X)
		l.insert(NxString("NX_BF_FROZEN_POS_X"));
    
	if (mEnum & NX_BF_FROZEN_POS_Y)
		l.insert(NxString("NX_BF_FROZEN_POS_Y"));
    
	if (mEnum & NX_BF_FROZEN_POS_Z)
		l.insert(NxString("NX_BF_FROZEN_POS_Z"));
    
	if (mEnum & NX_BF_FROZEN_ROT_X)
		l.insert(NxString("NX_BF_FROZEN_ROT_X"));
    
	if (mEnum & NX_BF_FROZEN_ROT_Y)
		l.insert(NxString("NX_BF_FROZEN_ROT_Y"));
    
	if (mEnum & NX_BF_FROZEN_ROT_Z)
		l.insert(NxString("NX_BF_FROZEN_ROT_Z"));
    
	if (mEnum & NX_BF_KINEMATIC)
		l.insert(NxString("NX_BF_KINEMATIC"));
    
	if (mEnum & NX_BF_VISUALIZATION)
		l.insert(NxString("NX_BF_VISUALIZATION"));
    
	if (mEnum & NX_BF_POSE_SLEEP_TEST)
		l.insert(NxString("NX_BF_POSE_SLEEP_TEST"));
    
	if (mEnum & NX_BF_FILTER_SLEEP_VEL)
		l.insert(NxString("NX_BF_FILTER_SLEEP_VEL"));

#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum & NX_BF_ENERGY_SLEEP_TEST)
		l.insert(NxString("NX_BF_ENERGY_SLEEP_TEST"));
#endif

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxShapeFlag() {

	StringList l;

	if (mEnum & NX_TRIGGER_ON_ENTER)
		l.insert(NxString("NX_TRIGGER_ON_ENTER"));

	if (mEnum & NX_TRIGGER_ON_LEAVE)
		l.insert(NxString("NX_TRIGGER_ON_LEAVE"));

	if (mEnum & NX_TRIGGER_ON_STAY)
		l.insert(NxString("NX_TRIGGER_ON_STAY"));

	if (mEnum & NX_SF_VISUALIZATION)
		l.insert(NxString("NX_SF_VISUALIZATION"));

	if (mEnum & NX_SF_DISABLE_COLLISION)
		l.insert(NxString("NX_SF_DISABLE_COLLISION"));

	if (mEnum & NX_SF_FEATURE_INDICES)
		l.insert(NxString("NX_SF_FEATURE_INDICES"));

	if (mEnum & NX_SF_DISABLE_RAYCASTING)
		l.insert(NxString("NX_SF_DISABLE_RAYCASTING"));

	if (mEnum & NX_SF_POINT_CONTACT_FORCE)
		l.insert(NxString("NX_SF_POINT_CONTACT_FORCE"));

	if (mEnum & NX_SF_FLUID_DRAIN)
		l.insert(NxString("NX_SF_FLUID_DRAIN"));
#if 0
	if (mEnum & NX_SF_FLUID_DUMMY9)
		l.insert(NxString("NX_SF_FLUID_DUMMY9"));
#endif
	if (mEnum & NX_SF_FLUID_DISABLE_COLLISION)
		l.insert(NxString("NX_SF_FLUID_DISABLE_COLLISION"));

#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum & NX_SF_FLUID_TWOWAY)
		l.insert(NxString("NX_SF_FLUID_TWOWAY"));
#endif

	if (mEnum & NX_SF_DISABLE_RESPONSE)
		l.insert(NxString("NX_SF_DISABLE_RESPONSE"));

	if (mEnum & NX_SF_DYNAMIC_DYNAMIC_CCD)
		l.insert(NxString("NX_SF_DYNAMIC_DYNAMIC_CCD"));

#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum & NX_SF_DISABLE_SCENE_QUERIES)
		l.insert(NxString("NX_SF_DISABLE_SCENE_QUERIES"));
#endif

#if NX_SDK_VERSION_NUMBER >= 270
    if (mEnum & NX_SF_CLOTH_DRAIN)
		l.insert(NxString("NX_SF_CLOTH_DRAIN"));
#endif

#if NX_SDK_VERSION_NUMBER >= 270
    if (mEnum & NX_SF_CLOTH_DISABLE_COLLISION)
		l.insert(NxString("NX_SF_CLOTH_DISABLE_COLLISION"));
#endif

#if NX_SDK_VERSION_NUMBER >= 270
    if (mEnum & NX_SF_CLOTH_TWOWAY)
		l.insert(NxString("NX_SF_CLOTH_TWOWAY"));
#endif

#if NX_USE_SOFTBODY_API
    if (mEnum & NX_SF_SOFTBODY_DRAIN)
		l.insert(NxString("NX_SF_SOFTBODY_DRAIN"));
#endif

#if NX_USE_SOFTBODY_API
    if (mEnum & NX_SF_SOFTBODY_DISABLE_COLLISION)
		l.insert(NxString("NX_SF_SOFTBODY_DISABLE_COLLISION"));
#endif

#if NX_USE_SOFTBODY_API
    if (mEnum & NX_SF_SOFTBODY_TWOWAY)
		l.insert(NxString("NX_SF_SOFTBODY_TWOWAY"));
#endif

	return l;
}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxCapsuleShapeFlag() {

	StringList l;

	if (mEnum & NX_SWEPT_SHAPE)
		l.insert(NxString("NX_SWEPT_SHAPE"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxWheelShapeFlags() {

	StringList l;


	if (mEnum & NX_WF_WHEEL_AXIS_CONTACT_NORMAL)
		l.insert(NxString("NX_WF_WHEEL_AXIS_CONTACT_NORMAL"));
    
	if (mEnum & NX_WF_INPUT_LAT_SLIPVELOCITY)
		l.insert(NxString("NX_WF_INPUT_LAT_SLIPVELOCITY"));
    
	if (mEnum & NX_WF_INPUT_LNG_SLIPVELOCITY)
		l.insert(NxString("NX_WF_INPUT_LNG_SLIPVELOCITY"));
    
	if (mEnum & NX_WF_UNSCALED_SPRING_BEHAVIOR)
		l.insert(NxString("NX_WF_UNSCALED_SPRING_BEHAVIOR"));
    
	if (mEnum & NX_WF_AXLE_SPEED_OVERRIDE)
		l.insert(NxString("NX_WF_AXLE_SPEED_OVERRIDE"));

#if NX_SDK_VERSION_NUMBER >= 264
    if (mEnum & NX_WF_EMULATE_LEGACY_WHEEL)
		l.insert(NxString("NX_WF_EMULATE_LEGACY_WHEEL"));
#endif

#if NX_SDK_VERSION_NUMBER >= 264
    if (mEnum & NX_WF_CLAMPED_FRICTION)
		l.insert(NxString("NX_WF_CLAMPED_FRICTION"));
#endif

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxMeshPagingMode() {

	StringList l;

	if (mEnum & NX_MESH_PAGING_MANUAL)
		l.insert(NxString("NX_MESH_PAGING_MANUAL"));
	
	if (mEnum & NX_MESH_PAGING_FALLBACK)
		l.insert(NxString("NX_MESH_PAGING_FALLBACK"));
	
	if (mEnum & NX_MESH_PAGING_AUTO)
		l.insert(NxString("NX_MESH_PAGING_AUTO"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxJointFlag() {

	StringList l;

	if (mEnum & NX_JF_COLLISION_ENABLED)
		l.insert(NxString("NX_JF_COLLISION_ENABLED"));

	if (mEnum & NX_JF_VISUALIZATION)
		l.insert(NxString("NX_JF_VISUALIZATION"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxD6JointMotion() {

	StringList l;

    if (mEnum & NX_D6JOINT_MOTION_LOCKED)
		l.insert(NxString("NX_D6JOINT_MOTION_LOCKED"));

    if (mEnum & NX_D6JOINT_MOTION_LIMITED)
		l.insert(NxString("NX_D6JOINT_MOTION_LIMITED"));

   if (mEnum & NX_D6JOINT_MOTION_FREE)
		l.insert(NxString("NX_D6JOINT_MOTION_FREE"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxD6JointDriveType() {

	StringList l;

    if (mEnum & NX_D6JOINT_DRIVE_POSITION)
		l.insert(NxString("NX_D6JOINT_DRIVE_POSITION"));

   if (mEnum & NX_D6JOINT_DRIVE_VELOCITY)
		l.insert(NxString("NX_D6JOINT_DRIVE_VELOCITY"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxJointProjectionMode() {

	StringList l;
 
	if (mEnum & NX_JPM_NONE)
		l.insert(NxString("NX_JPM_NONE"));

   if (mEnum & NX_JPM_POINT_MINDIST)
		l.insert(NxString("NX_JPM_POINT_MINDIST"));

#if NX_SDK_VERSION_NUMBER >= 260
    if (mEnum & NX_JPM_LINEAR_MINDIST)
		l.insert(NxString("NX_JPM_LINEAR_MINDIST"));
#endif

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxD6JointFlag() {

	StringList l;

	if (mEnum & NX_D6JOINT_SLERP_DRIVE)
		l.insert(NxString("NX_D6JOINT_SLERP_DRIVE"));

    if (mEnum & NX_D6JOINT_GEAR_ENABLED)
		l.insert(NxString("NX_D6JOINT_GEAR_ENABLED"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxDistanceJointFlag() {

	StringList l;

	if (mEnum & NX_DJF_MAX_DISTANCE_ENABLED)
		l.insert(NxString("NX_DJF_MAX_DISTANCE_ENABLED"));
    
    if (mEnum & NX_DJF_MIN_DISTANCE_ENABLED)
		l.insert(NxString("NX_DJF_MIN_DISTANCE_ENABLED"));
    
    if (mEnum & NX_DJF_SPRING_ENABLED)
		l.insert(NxString("NX_DJF_SPRING_ENABLED"));
    
	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxRevoluteJointFlag() {

	StringList l;

	if (mEnum & NX_RJF_LIMIT_ENABLED)
		l.insert(NxString("NX_RJF_LIMIT_ENABLED"));
	    
	if (mEnum & NX_RJF_MOTOR_ENABLED)
		l.insert(NxString("NX_RJF_MOTOR_ENABLED"));
	    
	if (mEnum & NX_RJF_SPRING_ENABLED)
		l.insert(NxString("NX_RJF_SPRING_ENABLED"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxSphericalJointFlag() {

	StringList l;

	if (mEnum & NX_SJF_TWIST_LIMIT_ENABLED)
		l.insert(NxString("NX_SJF_TWIST_LIMIT_ENABLED"));

	if (mEnum & NX_SJF_SWING_LIMIT_ENABLED)
		l.insert(NxString("NX_SJF_SWING_LIMIT_ENABLED"));

	if (mEnum & NX_SJF_TWIST_SPRING_ENABLED)
		l.insert(NxString("NX_SJF_TWIST_SPRING_ENABLED"));

	if (mEnum & NX_SJF_SWING_SPRING_ENABLED)
		l.insert(NxString("NX_SJF_SWING_SPRING_ENABLED"));

	if (mEnum & NX_SJF_JOINT_SPRING_ENABLED)
		l.insert(NxString("NX_SJF_JOINT_SPRING_ENABLED"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxPulleyJointFlag() {

	StringList l;

   if (mEnum & NX_PJF_IS_RIGID)
		l.insert(NxString("NX_PJF_IS_RIGID"));

   if (mEnum & NX_PJF_MOTOR_ENABLED)
		l.insert(NxString("NX_PJF_MOTOR_ENABLED"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxContactPairFlag() {

	StringList l;

    if (mEnum &  NX_IGNORE_PAIR)
		l.insert(NxString("NX_IGNORE_PAIR"));

    if (mEnum &  NX_NOTIFY_ON_START_TOUCH)
		l.insert(NxString("NX_NOTIFY_ON_START_TOUCH"));

    if (mEnum &  NX_NOTIFY_ON_END_TOUCH)
		l.insert(NxString("NX_NOTIFY_ON_END_TOUCH"));

    if (mEnum &  NX_NOTIFY_ON_TOUCH)
		l.insert(NxString("NX_NOTIFY_ON_TOUCH"));

    if (mEnum &  NX_NOTIFY_ON_IMPACT)
		l.insert(NxString("NX_NOTIFY_ON_IMPACT"));

    if (mEnum &  NX_NOTIFY_ON_ROLL)
		l.insert(NxString("NX_NOTIFY_ON_ROLL"));

    if (mEnum &  NX_NOTIFY_ON_SLIDE)
		l.insert(NxString("NX_NOTIFY_ON_SLIDE"));

#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum &  NX_NOTIFY_FORCES)
		l.insert(NxString("NX_NOTIFY_FORCES"));
#endif

#if 0
	if (mEnum &  NX_NOTIFY_DUMMY8)
		l.insert(NxString("NX_NOTIFY_DUMMY8"));

    if (mEnum &  NX_NOTIFY_DUMMY9)
		l.insert(NxString("NX_NOTIFY_DUMMY9"));

    if (mEnum &  NX_NOTIFY_DUMMY10)
		l.insert(NxString("NX_NOTIFY_DUMMY10"));

    if (mEnum &  NX_NOTIFY_DUMMY11)
		l.insert(NxString("NX_NOTIFY_DUMMY11"));

    if (mEnum &  NX_NOTIFY_DUMMY12)
		l.insert(NxString("NX_NOTIFY_DUMMY12"));

    if (mEnum &  NX_NOTIFY_DUMMY13)
		l.insert(NxString("NX_NOTIFY_DUMMY13"));

    if (mEnum &  NX_NOTIFY_DUMMY14)
		l.insert(NxString("NX_NOTIFY_DUMMY14"));

    if (mEnum &  NX_NOTIFY_DUMMY15)
		l.insert(NxString("NX_NOTIFY_DUMMY15"));
#endif
#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum &  NX_NOTIFY_CONTACT_MODIFICATION)
		l.insert(NxString("NX_NOTIFY_CONTACT_MODIFICATION"));
#endif
	return l;

}



#ifndef NX_DISABLE_FLUIDS

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxFluidSimulationMethod() {

	StringList l;

	if (mEnum & NX_F_SPH)
		l.insert(NxString("NX_F_SPH"));

    if (mEnum & NX_F_NO_PARTICLE_INTERACTION)
		l.insert(NxString("NX_F_NO_PARTICLE_INTERACTION"));

    if (mEnum & NX_F_MIXED_MODE)
		l.insert(NxString("NX_F_MIXED_MODE"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxFluidCollisionMethod() {

	StringList l;

    if (mEnum & NX_F_STATIC)
		l.insert(NxString("NX_F_STATIC"));

    if (mEnum & NX_F_DYNAMIC)
		l.insert(NxString("NX_F_DYNAMIC"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxFluidFlag() {

	StringList l;

	if (mEnum & NX_FF_VISUALIZATION)
		l.insert(NxString("NX_FF_VISUALIZATION"));

    if (mEnum & NX_FF_DISABLE_GRAVITY)
		l.insert(NxString("NX_FF_DISABLE_GRAVITY"));

#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum & NX_FF_COLLISION_TWOWAY)
		l.insert(NxString("NX_FF_COLLISION_TWOWAY"));
#endif

#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum & NX_FF_ENABLED)
		l.insert(NxString("NX_FF_ENABLED"));
#endif

#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum & NX_FF_HARDWARE)
		l.insert(NxString("NX_FF_HARDWARE"));
#endif

	return l;

}
////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxFluidEmitterFlag() {

	StringList l;

	if (mEnum & NX_FEF_VISUALIZATION)
		l.insert(NxString("NX_FEF_VISUALIZATION"));

#if NX_SDK_VERSION_NUMBER < 250
    if (mEnum & NX_FEF_BROKEN_ACTOR_REF)
		l.insert(NxString("NX_FEF_BROKEN_ACTOR_REF"));
#endif

#if 0
#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum & NX_FEF_DUMMY1)
		l.insert(NxString("NX_FEF_DUMMY1"));
#endif
#endif

#if NX_SDK_VERSION_NUMBER < 260
    if (mEnum & NX_FEF_FORCE_ON_ACTOR)
		l.insert(NxString("NX_FEF_FORCE_ON_ACTOR"));
#endif

#if NX_SDK_VERSION_NUMBER >= 260
    if (mEnum & NX_FEF_FORCE_ON_BODY)
		l.insert(NxString("NX_FEF_FORCE_ON_BODY"));
#endif

#if NX_SDK_VERSION_NUMBER < 260
    if (mEnum & NX_FEF_ADD_ACTOR_VELOCITY)
		l.insert(NxString("NX_FEF_ADD_ACTOR_VELOCITY"));
#endif

#if NX_SDK_VERSION_NUMBER >= 260
    if (mEnum & NX_FEF_ADD_BODY_VELOCITY)
		l.insert(NxString("NX_FEF_ADD_BODY_VELOCITY"));
#endif

    if (mEnum & NX_FEF_ENABLED)
		l.insert(NxString("NX_FEF_ENABLED"));

	return l;

}

#endif  // NX_DISABLE_FLUIDS


////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxEmitterType() {

	StringList l;

	if (mEnum & NX_AF_DISABLE_COLLISION)
		l.insert(NxString("NX_AF_DISABLE_COLLISION"));

    if (mEnum & NX_FE_CONSTANT_PRESSURE)
		l.insert(NxString("NX_FE_CONSTANT_PRESSURE"));
    
	if (mEnum & NX_FE_CONSTANT_FLOW_RATE)
		l.insert(NxString("NX_FE_CONSTANT_FLOW_RATE"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxEmitterShape() {

	StringList l;

	if (mEnum & NX_FE_RECTANGULAR)
		l.insert(NxString("NX_FE_RECTANGULAR"));
    
	if (mEnum & NX_FE_ELLIPSE)
		l.insert(NxString("NX_FE_ELLIPSE"));
	
	return l;

}


////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxClothFlag() {

	StringList l;


	if (mEnum & NX_CLF_PRESSURE)
		l.insert(NxString("NX_CLF_PRESSURE"));

    if (mEnum & NX_CLF_STATIC)
		l.insert(NxString("NX_CLF_STATIC"));

    if (mEnum & NX_CLF_DISABLE_COLLISION)
		l.insert(NxString("NX_CLF_DISABLE_COLLISION"));

    if (mEnum & NX_CLF_SELFCOLLISION)
		l.insert(NxString("NX_CLF_SELFCOLLISION"));

    if (mEnum & NX_CLF_VISUALIZATION)
		l.insert(NxString("NX_CLF_VISUALIZATION"));

    if (mEnum & NX_CLF_GRAVITY)
		l.insert(NxString("NX_CLF_GRAVITY"));

    if (mEnum & NX_CLF_BENDING)
		l.insert(NxString("NX_CLF_BENDING"));

    if (mEnum & NX_CLF_BENDING_ORTHO)
		l.insert(NxString("NX_CLF_BENDING_ORTHO"));

    if (mEnum & NX_CLF_DAMPING)
		l.insert(NxString("NX_CLF_DAMPING"));

    if (mEnum & NX_CLF_COLLISION_TWOWAY)
		l.insert(NxString("NX_CLF_COLLISION_TWOWAY"));

#if 0
	if (mEnum & NX_CLF_DUMMY1)
		l.insert(NxString("NX_CLF_DUMMY1"));
#endif

	if (mEnum & NX_CLF_TRIANGLE_COLLISION)
		l.insert(NxString("NX_CLF_TRIANGLE_COLLISION"));

    if (mEnum & NX_CLF_TEARABLE)
		l.insert(NxString("NX_CLF_TEARABLE"));

    if (mEnum & NX_CLF_HARDWARE)
		l.insert(NxString("NX_CLF_HARDWARE"));

#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum & NX_CLF_COMDAMPING)
		l.insert(NxString("NX_CLF_COMDAMPING"));
#endif
#if NX_SDK_VERSION_NUMBER >= 270
    if (mEnum & NX_CLF_VALIDBOUNDS)
		l.insert(NxString("NX_CLF_VALIDBOUNDS"));
#endif

#if NX_SDK_VERSION_NUMBER >= 270
    if (mEnum & NX_CLF_FLUID_COLLISION)
		l.insert(NxString("NX_CLF_FLUID_COLLISION"));
#endif

#if NX_SDK_VERSION_NUMBER >= 280
    if (mEnum & NX_CLF_ADHERE)
		l.insert(NxString("NX_CLF_ADHERE"));
#endif

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxForceFieldCoordinates() {

	StringList l;

    if (mEnum & NX_FFC_CARTESIAN)
		l.insert(NxString("NX_FFC_CARTESIAN"));

    if (mEnum & NX_FFC_SPHERICAL)
		l.insert(NxString("NX_FFC_SPHERICAL"));

    if (mEnum & NX_FFC_CYLINDRICAL)
		l.insert(NxString("NX_FFC_CYLINDRICAL"));

    if (mEnum & NX_FFC_TOROIDAL)
		l.insert(NxString("NX_FFC_TOROIDAL"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxSceneFlags() {

	StringList l;

    if (mEnum & NX_SF_DISABLE_SSE)
		l.insert(NxString("NX_SF_DISABLE_SSE"));

    if (mEnum & NX_SF_DISABLE_COLLISIONS)
		l.insert(NxString("NX_SF_DISABLE_COLLISIONS"));

    if (mEnum & NX_SF_SIMULATE_SEPARATE_THREAD)
		l.insert(NxString("NX_SF_SIMULATE_SEPARATE_THREAD"));

    if (mEnum & NX_SF_ENABLE_MULTITHREAD)
		l.insert(NxString("NX_SF_ENABLE_MULTITHREAD"));

#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum & NX_SF_ENABLE_ACTIVETRANSFORMS)
		l.insert(NxString("NX_SF_ENABLE_ACTIVETRANSFORMS"));
#endif

#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum & NX_SF_RESTRICTED_SCENE)
		l.insert(NxString("NX_SF_RESTRICTED_SCENE"));
#endif

#if NX_SDK_VERSION_NUMBER >= 250
    if (mEnum & NX_SF_DISABLE_SCENE_MUTEX)
		l.insert(NxString("NX_SF_DISABLE_SCENE_MUTEX"));
#endif

#if NX_SDK_VERSION_NUMBER >= 261
    if (mEnum & NX_SF_FORCE_CONE_FRICTION)
		l.insert(NxString("NX_SF_FORCE_CONE_FRICTION"));
#endif

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxSoftBodyMeshFlags() {

	StringList l;

#if 0
	if (mEnum & NX_SOFTBODY_MESH_DUMMY)
		l.insert(NxString("NX_SOFTBODY_MESH_DUMMY"));
#endif

    if (mEnum & NX_SOFTBODY_MESH_16_BIT_INDICES)
		l.insert(NxString("NX_SOFTBODY_MESH_16_BIT_INDICES"));
    
	if (mEnum & NX_SOFTBODY_MESH_TEARABLE)
		l.insert(NxString("NX_SOFTBODY_MESH_TEARABLE"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxSoftBodyVertexFlags() {

	StringList l;

#if 0
	if (mEnum & NX_SOFTBODY_VERTEX_DUMMY0)
		l.insert(NxString("NX_SOFTBODY_VERTEX_DUMMY0"));

    if (mEnum & NX_SOFTBODY_VERTEX_DUMMY1)
		l.insert(NxString("NX_SOFTBODY_VERTEX_DUMMY1"));

    if (mEnum & NX_SOFTBODY_VERTEX_DUMMY2)
		l.insert(NxString("NX_SOFTBODY_VERTEX_DUMMY2"));

    if (mEnum & NX_SOFTBODY_VERTEX_DUMMY3)
		l.insert(NxString("NX_SOFTBODY_VERTEX_DUMMY3"));

    if (mEnum & NX_SOFTBODY_VERTEX_DUMMY4)
		l.insert(NxString("NX_SOFTBODY_VERTEX_DUMMY4"));

    if (mEnum & NX_SOFTBODY_VERTEX_DUMMY5)
		l.insert(NxString("NX_SOFTBODY_VERTEX_DUMMY5"));

    if (mEnum & NX_SOFTBODY_VERTEX_DUMMY6)
		l.insert(NxString("NX_SOFTBODY_VERTEX_DUMMY6"));
#endif

	if (mEnum & NX_SOFTBODY_VERTEX_TEARABLE)
		l.insert(NxString("NX_SOFTBODY_VERTEX_TEARABLE"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxSoftBodyFlag() {

	StringList l;


#if 0
	if (mEnum & NX_SBF_DUMMY0)
		l.insert(NxString("NX_SBF_DUMMY0"));
#endif

	if (mEnum & NX_SBF_STATIC)
		l.insert(NxString("NX_SBF_STATIC"));

	if (mEnum & NX_SBF_DISABLE_COLLISION)
		l.insert(NxString("NX_SBF_DISABLE_COLLISION"));

	if (mEnum & NX_SBF_SELFCOLLISION)
		l.insert(NxString("NX_SBF_SELFCOLLISION"));

	if (mEnum & NX_SBF_VISUALIZATION)
		l.insert(NxString("NX_SBF_VISUALIZATION"));

	if (mEnum & NX_SBF_GRAVITY)
		l.insert(NxString("NX_SBF_GRAVITY"));

	if (mEnum & NX_SBF_VOLUME_CONSERVATION)
		l.insert(NxString("NX_SBF_VOLUME_CONSERVATION"));

	if (mEnum & NX_SBF_DAMPING)
		l.insert(NxString("NX_SBF_DAMPING"));

	if (mEnum & NX_SBF_COLLISION_TWOWAY)
		l.insert(NxString("NX_SBF_COLLISION_TWOWAY"));

	if (mEnum & NX_SBF_TEARABLE)
		l.insert(NxString("NX_SBF_TEARABLE"));

	if (mEnum & NX_SBF_HARDWARE)
		l.insert(NxString("NX_SBF_HARDWARE"));

	if (mEnum & NX_SBF_COMDAMPING)
		l.insert(NxString("NX_SBF_COMDAMPING"));

	if (mEnum & NX_SBF_VALIDBOUNDS)
		l.insert(NxString("NX_SBF_VALIDBOUNDS"));

	if (mEnum & NX_SBF_FLUID_COLLISION)
		l.insert(NxString("NX_SBF_FLUID_COLLISION"));

#if NX_SDK_VERSION_NUMBER >= 280
    if (mEnum & NX_SBF_ADHERE)
		l.insert(NxString("NX_SBF_ADHERE"));
#endif

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxSoftBodyAttachmentFlag() {

	StringList l;

	if (mEnum & NX_SOFTBODY_ATTACHMENT_TWOWAY)
		l.insert(NxString("NX_SOFTBODY_ATTACHMENT_TWOWAY"));

    if (mEnum & NX_SOFTBODY_ATTACHMENT_TEARABLE)
		l.insert(NxString("NX_SOFTBODY_ATTACHMENT_TEARABLE"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxMeshDataFlags() {

	StringList l;

	if (mEnum & NX_MDF_16_BIT_INDICES)
		l.insert(NxString("NX_MDF_16_BIT_INDICES"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxCombineMode() {

	StringList l;

	if (mEnum & NX_CM_AVERAGE)
		l.insert(NxString("NX_CM_AVERAGE"));

	if (mEnum & NX_CM_MIN)
		l.insert(NxString("NX_CM_MIN"));

	if (mEnum & NX_CM_MULTIPLY)
		l.insert(NxString("NX_CM_MULTIPLY"));

	if (mEnum & NX_CM_MAX)
		l.insert(NxString("NX_CM_MAX"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxMaterialFlag() {

	StringList l;

	if (mEnum & NX_MF_ANISOTROPIC)
		l.insert(NxString("NX_MF_ANISOTROPIC"));

#if 0
	if (mEnum & NX_MF_DUMMY1)
		l.insert(NxString("NX_MF_DUMMY1"));

	if (mEnum & NX_MF_DUMMY2)
		l.insert(NxString("NX_MF_DUMMY2"));

	if (mEnum & NX_MF_DUMMY3)
		l.insert(NxString("NX_MF_DUMMY3"));
#endif

	if (mEnum & NX_MF_DISABLE_FRICTION)
		l.insert(NxString("NX_MF_DISABLE_FRICTION"));

	if (mEnum & NX_MF_DISABLE_STRONG_FRICTION)
		l.insert(NxString("NX_MF_DISABLE_STRONG_FRICTION"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxMeshShapeFlag() {

	StringList l;

	if (mEnum & NX_MESH_SMOOTH_SPHERE_COLLISIONS)
		l.insert(NxString("NX_MESH_SMOOTH_SPHERE_COLLISIONS"));
    
	if (mEnum & NX_MESH_DOUBLE_SIDED)
		l.insert(NxString("NX_MESH_DOUBLE_SIDED"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxPruningStructure() {

	StringList l;

	if (mEnum & NX_PRUNING_NONE)
		l.insert(NxString("NX_PRUNING_NONE"));
    
	if (mEnum & NX_PRUNING_OCTREE)
		l.insert(NxString("NX_PRUNING_OCTREE"));

	if (mEnum & NX_PRUNING_QUADTREE)
		l.insert(NxString("NX_PRUNING_QUADTREE"));

	if (mEnum & NX_PRUNING_DYNAMIC_AABB_TREE)
		l.insert(NxString("NX_PRUNING_DYNAMIC_AABB_TREE"));

	if (mEnum & NX_PRUNING_STATIC_AABB_TREE)
		l.insert(NxString("NX_PRUNING_STATIC_AABB_TREE"));


	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxThreadPriority() {

	StringList l;

	if (mEnum & NX_TP_HIGH)
		l.insert(NxString("NX_TP_HIGH"));
    
	if (mEnum & NX_TP_ABOVE_NORMAL)
		l.insert(NxString("NX_TP_ABOVE_NORMAL"));

	if (mEnum & NX_TP_NORMAL)
		l.insert(NxString("NX_TP_NORMAL"));

	if (mEnum & NX_TP_BELOW_NORMAL)
		l.insert(NxString("NX_TP_BELOW_NORMAL"));

	if (mEnum & NX_TP_LOW)
		l.insert(NxString("NX_TP_LOW"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxSimulationType() {
	
	StringList l;

	if (mEnum & NX_SIMULATION_HW)
		l.insert(NxString("NX_SIMULATION_HW"));
    
	if (mEnum & NX_SIMULATION_SW)
		l.insert(NxString("NX_SIMULATION_SW"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxTimeStepMethod() {
	
	StringList l;

	if (mEnum & NX_TIMESTEP_FIXED)
		l.insert(NxString("NX_TIMESTEP_FIXED"));

	if (mEnum & NX_TIMESTEP_VARIABLE)
		l.insert(NxString("NX_TIMESTEP_VARIABLE"));
    
	if (mEnum & NX_TIMESTEP_INHERIT)
		l.insert(NxString("NX_TIMESTEP_INHERIT"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

StringList EnumsBlueprint::AsNxFilterOp() {
	
	StringList l;

	if (mEnum & NX_FILTEROP_AND)
		l.insert(NxString("NX_FILTEROP_AND"));
    
	if (mEnum & NX_FILTEROP_OR)
		l.insert(NxString("NX_FILTEROP_OR"));
    
	if (mEnum & NX_FILTEROP_XOR)
		l.insert(NxString("NX_FILTEROP_XOR"));
    
	if (mEnum & NX_FILTEROP_NAND)
		l.insert(NxString("NX_FILTEROP_NAND"));
    
	if (mEnum & NX_FILTEROP_NOR)
		l.insert(NxString("NX_FILTEROP_NOR"));
    
	if (mEnum & NX_FILTEROP_NXOR)
		l.insert(NxString("NX_FILTEROP_NXOR"));
    
	if (mEnum & NX_FILTEROP_SWAP_AND)
		l.insert(NxString("NX_FILTEROP_SWAP_AND"));

	return l;

}

////////////////////////////////////////////////////////////////////////////////////////////////

}  //End of Serialiser namespace.
}  //End of NxOgre namespace.
