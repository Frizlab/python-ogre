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

#ifndef __NXOGRE_FORCEFIELD_H__
#define __NXOGRE_FORCEFIELD_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreActor.h"
#include "NxOgreParams.h"
#include "NxOgrePose.h"

namespace NxOgre {

	// Helper Class for ForceFieldParams.
	// fi = [K + Mp(P-p) + Mv(V-v) + N anoise]i / [1 + li|Pi-pi| + qi(Pi-pi)^2]
	struct NxExport ForceFieldFormula {

		// p is the position of the object, in the force field coordinate system 
		// v is the velocity of the object, in the force field coordinate system 

		/////////////////////////////////////////////////////////////////

		NxVec3 K;		//K is the constant part, from NxForceFieldDesc.constant 

		NxMat33 Mp;		//Mp is the position dependence matrix, from NxForceFieldDesc.position 

		NxVec3 P;		//P is the position target, from NxForceFieldDesc.positionTarget 
		 
		NxMat33 Mv;		//Mv is the velocity dependence matrix, from NxForceFieldDesc.velocityMultiplier

		NxVec3 V;		//V is the velocity target, from NxForceFieldDesc.velocityTarget 
		
		NxVec3 Anoise;	//anoise is the noise multiplier, from NxForceFieldDesc.noise 
		
		NxVec3 l;		//l is the linear falloff term, from NxForceFieldDesc.falloffLinear 

		NxVec3 q;		//q is the quadratic falloff term, from NxForceFieldDesc.falloffQuadratic 


		NxForceFieldCoordinates coords;

		/////////////////////////////////////////////////////////////////

		void zero() {
			K.zero();
			Mp.zero();
			P.zero();
			Mv.zero();
			V.zero();
			Anoise.zero();
			l.zero();
			q.zero();
			coords = NX_FFC_CARTESIAN;
		}

		void asExplosive(NxReal explosiveConstant, Ogre::Vector3 velocityTarget, Ogre::Vector3 noise = Ogre::Vector3(1,1,1));

	};

	/////////////////////////////////////////////////////////////////

	class NxExport ForceFieldParams : public Params {

		public:

			ForceFieldParams() {}
			ForceFieldParams(const char* p) {process(p);}
			ForceFieldParams(NxString p) {process(p);}
			ForceFieldParams(const ForceFieldFormula& f){setToDefault();parseFormula(f);}

			void			setToDefault();
			void			parse(Parameters);			
			void			parseFormula(const ForceFieldFormula&);
			
			NxMat34					pose;					//!< Global or (if actor is set) actor relative transformation of the force field.
			NxActor*				actor;					//!< The field is attached to move with an actor. If null, the field is static.
			NxForceFieldCoordinates	coordinates;			//!< Coordinate space of the field.

			// Filtering:
			NxCollisionGroup		group;					//!< Collision group used for collision filtering.
			NxGroupsMask			groupsMask;				//!< Groups mask used for collision filtering. 

			// Function parameters:
			NxVec3					constant;				//!< Constant part of force field function
			NxMat33					positionMultiplier;		//!< Coefficient of force field function position term
			NxVec3					positionTarget;			//!< Force field position target			
			NxMat33					velocityMultiplier;		//!< Coefficient of force field function velocity term
			NxVec3					velocityTarget;			//!< Force field velocity target

			NxVec3					falloffLinear;			//!< Force field velocity target
			NxVec3					falloffQuadratic;		//!< Force field velocity target

			NxReal					torusRadius;			//!< Radius for NX_FFC_TOROIDAL type coordinates.

			NxVec3					noise;					//<! Noise scaling

			NxReal					fluidScale;				//!< Force scale factor for fluids
			NxReal					clothScale;				//!< Force scale factor for cloth
			NxReal					softBodyScale;			//!< Force scale factor for soft bodies
			NxReal					rigidBodyScale;			//!< Force scale factor for rigid bodies

			NxU32					flags;					//!< Force field flags; @see NxForceFieldShapeFlags


	}; // End of ActorParams class
#if 0
	//////////////////////////////////////////////////////////

	class NxExport ForceFieldShape {
		
		friend class ForceField;

		protected:

			virtual NxForceFieldShape* __bindToForceField(NxForceField*, bool Inclusion = true)		{
																										return 0;
																									}

	};

	//////////////////////////////////////////////////////////

	class NxExport ForceFieldCubeShape : public ForceFieldShape {
		
		public:

			ForceFieldCubeShape(NxReal w, NxReal h, NxReal d, const Pose& p = Pose())
			{
				createShape(NxVec3(w*0.5f,h*0.5f,d*0.5f), p);
			}

			ForceFieldCubeShape(NxReal c, const Pose& p = Pose())
			{
				createShape(NxVec3(c*0.5f,c*0.5f,c*0.5f), p);
			}

			void	createShape(NxVec3 size, const Pose& p)
			{
				mDescription.setToDefault();
				mDescription.dimensions = size;
				mDescription.pose = p.toMat34();
			}
			
			NxBoxForceFieldShapeDesc mDescription;

		protected:

			virtual NxForceFieldShape* __bindToForceField(NxForceField*, bool Inclusion = true);

	};

	//////////////////////////////////////////////////////////

	class NxExport ForceFieldSphereShape : public ForceFieldShape {
		
		public:

			ForceFieldSphereShape(NxReal radius, const Pose& p = Pose())
			{
				mDescription.setToDefault();
				mDescription.radius = radius;
				mDescription.pose = p.toMat34();
			}
			
			NxSphereForceFieldShapeDesc mDescription;
		
		protected:

			virtual NxForceFieldShape* __bindToForceField(NxForceField*, bool Inclusion = true);

	};

	//////////////////////////////////////////////////////////

	class NxExport ForceFieldCapsuleShape : public ForceFieldShape {
		
		public:

			ForceFieldCapsuleShape(NxReal height, NxReal radius, const Pose& p = Pose())
			{
				mDescription.setToDefault();
				mDescription.height = height;
				mDescription.radius = radius;
				mDescription.pose = p.toMat34();
			}

			NxCapsuleForceFieldShapeDesc mDescription;
		
		protected:

			virtual NxForceFieldShape* __bindToForceField(NxForceField*, bool Inclusion = true);

	};

	//////////////////////////////////////////////////////////

	class NxExport ForceFieldConvexShape : public ForceFieldShape {
		
		public:

			ForceFieldConvexShape(NxString meshName, Scene* scene, const Pose& p = Pose(), const Ogre::Vector3& scale = Ogre::Vector3(1,1,1));
			NxConvexForceFieldShapeDesc mDescription;
		
		protected:

			virtual NxForceFieldShape* __bindToForceField(NxForceField*, bool Inclusion = true);

	};
#endif

	//////////////////////////////////////////////////////////

	class NxExport ForceField : public Actor {

		friend class Scene;

		public:

			ForceField(const NxString& Identifier, Scene*);
			// identifier, this, pose, actorShape, aparams, fparams
			ForceField(const NxString& identifier, Scene*, const Pose&, ShapeBlueprint* actorShape, ActorParams = "", ForceFieldParams = "");
			~ForceField();

			//////////////////////////////////////////////////////////

			void _createActor(ShapeBlueprint *shape, const Pose& pose, ActorParams params);
			void _destroyActor();
			void _createForceField(ForceFieldParams params);
			void _destroyForceField();

			//////////////////////////////////////////////////////////

			void addForceFieldShape(SimpleShape*, bool Inclusion);

			//////////////////////////////////////////////////////////

			
			StringPairList saveCustom();
			void restoreCustom(StringPairList);

			NxForceField* mForceField;

	};
	
	//////////////////////////////////////////////////////////



};

#endif
