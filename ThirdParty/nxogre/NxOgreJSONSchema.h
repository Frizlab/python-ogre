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

#ifndef __NXOGRE_JSON_SCHEMA_H__
#define __NXOGRE_JSON_SCHEMA_H__

#include "NxOgrePrerequisites.h"

#if (NX_USE_JSON == 1)

namespace NxOgre {

		// NxJSON Schema 1
		#define NX_JSON_SCHEMA_ID "2";

		#define NX_JSON_ID "Name"
		#define NX_JSON_UserProperties "UserProperties"
		#define NX_JSON_Flags "Flags"
		#define NX_JSON_SECTION_User "Custom"
		#define NX_JSON_User "UserProperties"
		#define	NX_JSON_Type "Type"
		#define NX_JSON_Mass "Mass"
		#define NX_JSON_Density "Density"

		
		#define	NX_JSON_SECTION_Version "Versions"
		#define NX_JSON_NxOgre "NxOgre"
		#define NX_JSON_PhysX "PhysX"
		#define NX_JSON_NxJSON "NxJSON"
		#define NX_JSON_Ogre "Ogre"

		#define NX_JSON_SECTION_Meshes "Meshes"
		#define NX_JSON_SECTION_Scenes "Scenes"
	
		#define NX_JSON_HasMaxBounds "HasMaxBounds"
		#define NX_JSON_HasLimits "HasLimits"
		#define NX_JSON_HasFilter "HasFilter"
		#define NX_JSON_FilterBool "FilterBool"
		#define NX_JSON_FilterOp0 "FilterOp0"
		#define NX_JSON_FilterOp1 "FilterOp1"
		#define NX_JSON_FilterOp2 "FilterOp2"
		#define NX_JSON_GroupMask0 "GroupMask0"
		#define NX_JSON_GroupMask1 "GroupMask1"
		#define NX_JSON_Gravity "Gravity"
		#define NX_JSON_MaxTimeStep "MaxTimeStep"
		#define NX_JSON_MaxIter "MaxIter"
		#define NX_JSON_TimeStepMethod "TimeStepMethod"
		#define NX_JSON_MaxBounds "MaxBounds"
		#define NX_JSON_SimType "SimType"
		#define NX_JSON_GroundPlane "GroundPlane"
		#define NX_JSON_BoundsPlanes "BoundsPlanes"
		#define NX_JSON_InternalThreadCount "InternalThreadCount"
		#define NX_JSON_BackgroundThreadCount "BackgroundThreadCount"
		#define NX_JSON_ThreadMask "ThreadMask"
		#define NX_JSON_BackgroundThreadMask "BackgroundThreadMask"
		#define NX_JSON_SimThreadStackSize "SimThreadStackSize"
		#define NX_JSON_SimThreadPriority "SimThreadPriority"
		#define NX_JSON_WorkerThreadStackSize "WorkerThreadStackSize"
		#define NX_JSON_WorkerThreadPriority "WorkerThreadPriority"
		#define NX_JSON_UpAxis "UpAxis"
		#define NX_JSON_SubdivisionLevel "SubdivisionLevel"
		#define NX_JSON_StaticStructure "StaticStructure"
		#define NX_JSON_DynamicStructure "DynamicStructure"
		#define NX_JSON_Body "Body"
		
		#define NX_JSON_SECTION_Materials "Materials"
		#define NX_JSON_Material "Material"
		#define NX_JSON_HasSpring "HasSpring"
		#define NX_JSON_MaterialIndex "MaterialIndex"
		#define NX_JSON_DynamicFriction "DynamicFriction"
		#define NX_JSON_StaticFriction "StaticFriction"
		#define NX_JSON_Restitution "Restitution"
		#define NX_JSON_DynamicFrictionV "DynamicFrictionV"
		#define NX_JSON_StaticFrictionV "StaticFrictionV"
		#define NX_JSON_FrictionCombineMode "FrictionCombineMode"
		#define NX_JSON_RestitutionCombineMode "RestitutionCombineMode"
		#define NX_JSON_DirOfAnisotropy "DirOfAnisotropy"
		#define NX_JSON_Spring "Spring"
		#define NX_JSON_Damper "Damper"
		#define NX_JSON_TargetValue "TargetValue"

		#define NX_JSON_SECTION_Actors "Actors"
		#define NX_JSON_GlobalPose "GlobalPose"
		#define NX_JSON_LocalPose "LocalPose"
		#define NX_JSON_Density "Density"
		#define NX_JSON_CompartmentName "Compartment"
		#define	NX_JSON_DominanceGroupName "DominanceGroup"

		#define NX_JSON_SECTION_ActorGroups "ActorGroups"
		#define NX_JSON_ActorGroupName "ActorGroup"

		#define NX_JSON_SECTION_Shapes "Shapes"

		#define	NX_JSON_PlaneShape "Plane"
		#define	NX_JSON_SphereShape "Sphere"
		#define	NX_JSON_BoxShape "Box"
		#define	NX_JSON_CapsuleShape "Capsule"
		#define	NX_JSON_ConvexShape "Convex"
		#define	NX_JSON_TriangleMesh "TriangleMesh"				
		#define	NX_JSON_ShapeGroup "ShapeGroup"
						
		#define NX_JSON_Normal "Normal"
		#define NX_JSON_Distance "Distance"
		#define NX_JSON_Skeleton "Skeleton"
		#define NX_JSON_SkinWidth "SkinWidth"
		#define NX_JSON_GroupsMask "GroupsMask"

		#define NX_JSON_Radius "Radius"
		#define NX_JSON_Height "Height"
		#define NX_JSON_Width "Width"
		#define NX_JSON_Depth "Depth"
		#define NX_JSON_Dimensions "Dimensions"
		#define NX_JSON_WheelShape "WheelShape"
		#define NX_JSON_SuspensionTravel "SuspensionTravel"
		#define NX_JSON_Suspension "Suspension"
		#define NX_JSON_LongitudalTireForceFunction "LongitudalTireForceFunction"
		#define NX_JSON_LateralTireForceFunction "LateralTireForceFunction"
		#define NX_JSON_ExtremumSlip "ExtremumSlip"
		#define NX_JSON_ExtremumValue "ExtremumValue"
		#define NX_JSON_AsymptoteSlip "AsymptoteSlip"
		#define NX_JSON_AsymptoteValue "AsymptoteValue"
		#define NX_JSON_StiffnessFactor "StiffnessFactor"
		#define NX_JSON_InverseWheelMass "InverseWheelMass"
		#define NX_JSON_MotorTorque "MotorTorque"
		#define NX_JSON_BrakeTorque "BrakeTorque"
		#define NX_JSON_SteerAngle "SteerAngle"
		#define NX_JSON_MeshName "MeshName"
		#define NX_JSON_Vertices "Vertices"
		#define NX_JSON_Triangles "Triangles"
		#define NX_JSON_PagingMode "PagingMode"
		#define NX_JSON_MaterialIndices "MaterialIndices"
		#define NX_JSON_HeightFieldVerticalAxis "HeightFieldVerticalAxis"
		#define NX_JSON_HeightFieldVerticalExtent "HeightFieldVerticalExtent"
		#define NX_JSON_ConvexEdgeThreshold "ConvexEdgeThreshold"
		#define NX_JSON_CookedDataSize "CookedDataSize"
		#define NX_JSON_CookedData "CookedData"

		#define NX_JSON_SECTION_Convex "Convex"
		#define NX_JSON_SECTION_TriangleMesh "Mesh"

		#define NX_JSON_MassLocalPose "MassLocalPose"
		#define NX_JSON_MassSpaceInertia "MassSpaceInertia"
		#define NX_JSON_LinearVelocity "LinearVelocity"

		#define NX_JSON_AngularVelocity "AngularVelocity"
		#define NX_JSON_WakeUpCounter "WakeUpCounter"
		#define NX_JSON_LinearDamping "LinearDamping"
		#define NX_JSON_AngularDamping "AngularDamping"
		#define NX_JSON_MaxAngularVelocity "MaxAngularVelocity"
		#define NX_JSON_CCDMotionThreshold "CCDMotionThreshold"
		#define NX_JSON_SleepLinearVelocity "SleepLinearVelocity"
		#define NX_JSON_SleepAngularVelocity "SleepAngularVelocity"
		#define NX_JSON_SolverIterationCount "SolverIterationCount"
		#define NX_JSON_SleepEnergyThreshold "SleepEnergyThreshold"
		#define NX_JSON_SleepDamping "SleepDamping"

};
#endif
#endif
