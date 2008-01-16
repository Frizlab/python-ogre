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

#ifndef __NXOGRE_SHAPE_H__
#define __NXOGRE_SHAPE_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreParams.h"

#include "OgreVector3.h"

namespace NxOgre {

	/**
	 	\page shapeparams ShapeParams
		
		String base params are case and space insensitive.

		Examples:
		
		<code>
			"offset: 1 0 1"

			"offset: 1 0 1, material: myMaterial"

			"offset: 1 0 1, material-index: 1, group: myGroup"
		</code>

		-	offset	(Vector3)
			- Description: Local offset of the shape compared to center of the Actor
			- Example:	"offset: 1 0 1"
			- See: ShapeParams::mLocalPose
		-	orientation (Quaternion)
			- Description: Local orientation of the shape.
			- Example:	"orientation: 1 0 0 1"
			- See: ShapeParams::mLocalPose
		-	generateCCD (bool)
			- Description: Generate a CCD skeleton for the shape.
			- Example:	"generateCCD: yes"
			- See: ShapeParams::mGenerateCCD
		-	generateCCDDelta (float)
			- Description: Delta value for the CCD skeleton generator
			- Example:	"generateCCDDelta: 0.8"
			- See: ShapeParams::mGenerateCCDDelta
		-	CCDDynamicFlag (bool)
			- Description: Enable Dynamic to Dynamic CCD.
			- Example:	"CCDDynamicFlag: true"
			- See: ShapeParams::mDynamicDynamicCCD
		-	material-index (unsigned int)
			- Description: NxMaterialIndex of the material to use. Use this or material, but not both.
			- Example:	"material-index: 1"
			- See: ShapeParams::mMaterialAsIndex
		-	material (string)
			- Description: Name of the material to use. Use this or material-index, but not both.
			- Example:	"material: myMaterial"
			- See: ShapeParams::mMaterialAsName
		-	group-index (unsigned int)
			- Description: NxCollisionGroup of the group to use. Use this or group, but not both.
			- Example:	"group-index: 1"
			- See: ShapeParams::mGroupAsIndex
		-	group (string)
			- Description: Name of the group to use. Use this or group-index, but not both.
			- Example:	"group: myCollisionGroup"
			- See: ShapeParams::mGroupAsName							
		-	skin-width (float)
			- Description: Skin Width of the shape
			- Example:	"skinwidth: -0.5"
			- See: ShapeParams::mSkinWidth		
		-	mesh-scale (Vector3)
			- Description: Scale any meshes (triangle, convex or heightmap) use with this shape.
			- Example:	"mesh-scale: 2 2 2"
			- See: ShapeParams::mMeshScale
		-	mass (float)
			- Description: Mass of this shape. Use this or density, do not use both. 
			- Example: "mass: 10"
			- See: ShapeParams::mMass and NxShape::mass for more information.
		-	density (float)
			- Description: Density of this shape. Use this or mass, do not use both.
			- Example: "Density: 1.5"
			- See: ShapeParams::mDensity and NxShape::density for more information.				
	*/
	
	/** ShapeParams
	 	Parameters for the ShapeBlueprint system.
		

		See \ref shapeparams for the full string argument documentation.
	*/		
	class NxExport ShapeParams : public Params {

		public:



			////////////////////////////////////////////////////////////

			/** LocalPose of the shape from the Actors center.
				@note				
					For Quaternions use: m.M.fromQuat(NxQuat(w,x,y,z));
					For Vector3's use: m.t = NxVec3(x,y,z)
				@default mLocalPose.id()
			 */
			NxMat34				mLocalPose;

			/**  Automatically generate a CCD skeleton
				 @default	false
			 */
			bool				mGenerateCCD;
			
			/**  Delta value for the CCD generator (How much the mesh will be scaled).
				 @default 0.8
			  */
			NxReal				mGenerateCCDDelta;

			/** Allow Dynamic Dynamic CCD. (Sets NX_SF_DYNAMIC_DYNAMIC_CCD in mFlag to true).
				@default false
			 */
			bool				mDynamicDynamicCCD;
			
			/**  Use a CCDSkeleton, but with this skeleton only.
			 	 @default NULL
			*/
			Skeleton*			mCCDSkeleton;
			
			/** Use the material from this materialIndex. Use this or mMaterialAsName do not set both.
				@default 0
			 */
			NxMaterialIndex		mMaterialAsIndex;
			
			/** Use the material from this name. Use this or mMaterialAsIndex do not set both.
			  	@default <blank string>
			  */
			NxString			mMaterialAsName;
			
			/** Use the ShapeGroup(NxCollisionGroup) from this name
			  	@default <blank string>
			 */
			NxString			mGroupAsName;
			
			/** Use the ShapeGroup(NxCollisionGroup) from this index
				 @default 0
			 */
			NxCollisionGroup	mGroupAsIndex;

			/**	GroupMask

			 */
			NxGroupsMask		mGroupsMask;
			
			/** SkinWidth of the shape

			 */
			NxReal				mSkinWidth;

			/** Scale all generated meshes used with this shape by this scale (default: 1 1 1)
			
			 */
			NxVec3				mMeshScale;
			
			/** Mass of this individual shape when computing mass inertial properties for a rigidbody.
			    default (-1.0, calculate from density). Make sure density is 0.0 before changing.
			 */
			NxReal				mMass;

			/** Density
				Density of this individual shape when computing mass inertial properties for a rigidbody.
				(Unless a valid mass >0.0 is provided). Make sure mass is -1.0 before changing.
				@default 1.0
			 */
			NxReal				mDensity;
			
			/**  Specific ShapeFlags to be appended to the shape (NxBoxShape, NxConvexShape, etc.) flags.
				@default 0
			 */
			NxU32				mFlags;
			
			
			////////////////////////////////////////////////////////////

			ShapeParams()				{}
			ShapeParams(const char* p)	{setToDefault();process(p);}
			ShapeParams(NxString p)		{setToDefault();process(p);}

			void setToDefault();
			void parse(Parameters);
			
			////////////////////////////////////////////////////////////


			
	};// End of ShapeParams class

	////////////////////////////////////////////////////////////////////////////////////////////////

	class NxExport Shape {

		friend class Actor;
		friend class Blueprints::ActorBlueprint;
		friend class ShapeBlueprint;

		public:

			/** Shape Constructor
				This is an empty class, create a shape using the constructor
				of the shape you wish to create; new CubeShape(), new SphereShape(), etc.
			*/
			Shape(Actor*);

			/** Shape Destructor
				
			*/
			virtual ~Shape();

			/**

			*/
			virtual ShapeBlueprint*	getBlueprint()			{
																NxThrow_Warning("Returning NULL shape blueprint.");
																return NULL;
															}

			/**

			*/
			virtual void copyTo(Actor*)						{
															}

			/**

			*/
			virtual void copyTo(Actor*, Ogre::Vector3 positionOverride)	{
																		}

			/**

			*/
			virtual void moveTo(Actor*)						{
															}
														

			/**

			*/
			virtual void moveTo(Actor*, Ogre::Vector3 positionOverride)	{
																		}

			/**

			*/
			virtual NxShape*	getNxShape()				{
																return NULL;
															}

			/**

			*/
			virtual void	releaseShape()					{
															}

			/**

			*/
			Skeleton*		getSkeleton();


			/**

			*/
			NxShapeIndex	getIndex()						{
																return mID;
															}


			/**

			*/
			void	setIndex(NxShapeIndex index)			{
																mID = index;
															}


		protected:

			/**

			*/
			virtual bool isDynamic()						{
																return false;
															}

			/**

			*/
			virtual bool isStaticOnly()						{
																return false;
															}
			

			/** _bindNxShapeToShape
				Called after NxActor is created, Shape is referred by userData.
				@param NxShape Shape to bind to.
			*/
			virtual void _bindNxShapeToShape(NxShape*)		{
															}

			/** __descriptionToParams
				Copies the all of the values of the ShapeParams to a NxShapeDesc
				@param desc NxShapeDesc to copy to.
				@param params Params to work with.
			*/
			void	__descriptionToParams(NxShapeDesc& desc, ShapeParams& params);

			////////////////////////////////////////////////////////////////////

			NxShapeIndex	mID;
			ShapeParams		mParams;
			Actor*			mActor;
			NxShape*		mBaseShape;
			Skeleton*		mSkeleton;
			
		private:

	};

};

#endif
