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

#ifndef __NXOGRE_ENUMS_BLUEPRINT_H__
#define __NXOGRE_ENUMS_BLUEPRINT_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreBlueprint.h"

namespace NxOgre {

	namespace Blueprints {

		class NxExport EnumsBlueprint : public Blueprint {
						
			public:
				
				enum EnumType {
					FT_Unknown,
					FT_NxActorFlag,
					FT_NxBodyFlag,
					FT_NxShapeFlag,
					FT_NxCapsuleShapeFlag,
					FT_NxWheelShapeFlags,
					FT_NxMeshPagingMode,
					FT_NxJointFlag,
					FT_NxD6JointMotion,
					FT_NxD6JointDriveType,
					FT_NxJointProjectionMode,
					FT_NxD6JointFlag,
					FT_NxDistanceJointFlag,
					FT_NxRevoluteJointFlag,
					FT_NxSphericalJointFlag,
					FT_NxPulleyJointFlag,
					FT_NxContactPairFlag,
					FT_NxFluidSimulationMethod,
					FT_NxFluidCollisionMethod,
					FT_NxFluidFlag,
					FT_NxEmitterType,
					FT_NxEmitterShape,
					FT_NxFluidEmitterFlag,
					FT_NxClothFlag,
					FT_NxForceFieldCoordinates,
					FT_NxSceneFlags,
					FT_NxSoftBodyMeshFlags,
					FT_NxSoftBodyVertexFlags,
					FT_NxSoftBodyFlag,
					FT_NxSoftBodyAttachmentFlag,
					FT_NxMeshDataFlags,
					FT_NxCombineMode,
					FT_NxMaterialFlag,
					FT_NxMeshShapeFlag,
					FT_NxPruningStructure,
					FT_NxThreadPriority,
					FT_NxSimulationType,
					FT_NxTimeStepMethod,
					FT_NxFilterOp
				};

				EnumsBlueprint(){mType = FT_Unknown; mEnum = 0;}
				EnumsBlueprint(EnumType);
				EnumsBlueprint(EnumType, NxU32 flag);

				void toDefault();
				void empty();
				void serialise();
				NxU32	getFlag() {return mEnum;}
				void	setFlag(EnumType t, NxU32 f) {mType = t;mEnum = f;}
				void	setFlag(NxU32 f) {mEnum = f;}

				StringList asStringPairList();

				StringList AsNxActorFlag();
				StringList AsNxBodyFlag();
				StringList AsNxShapeFlag();
				StringList AsNxCapsuleShapeFlag();
				StringList AsNxWheelShapeFlags();
				StringList AsNxMeshPagingMode();
				StringList AsNxJointFlag();
				StringList AsNxD6JointMotion();
				StringList AsNxD6JointDriveType();
				StringList AsNxJointProjectionMode();
				StringList AsNxD6JointFlag();
				StringList AsNxDistanceJointFlag();
				StringList AsNxRevoluteJointFlag();
				StringList AsNxSphericalJointFlag();
				StringList AsNxPulleyJointFlag();
				StringList AsNxContactPairFlag();
				StringList AsNxFluidSimulationMethod();
				StringList AsNxFluidCollisionMethod();
				StringList AsNxFluidFlag();
				StringList AsNxEmitterType();
				StringList AsNxEmitterShape();
				StringList AsNxFluidEmitterFlag();
				StringList AsNxClothFlag();
				StringList AsNxForceFieldCoordinates();
				StringList AsNxSceneFlags();
				StringList AsNxSoftBodyMeshFlags();
				StringList AsNxSoftBodyVertexFlags();
				StringList AsNxSoftBodyFlag();
				StringList AsNxSoftBodyAttachmentFlag();
				StringList AsNxMeshDataFlags();
				StringList AsNxCombineMode();
				StringList AsNxMaterialFlag();
				StringList AsNxMeshShapeFlag();
				StringList AsNxPruningStructure();
				StringList AsNxThreadPriority();
				StringList AsNxSimulationType();
				StringList AsNxTimeStepMethod();
				StringList AsNxFilterOp();


				NxU32					mEnum;
				EnumType				mType;
		};

	}; // End of namespace
};// End of namespace

#endif
