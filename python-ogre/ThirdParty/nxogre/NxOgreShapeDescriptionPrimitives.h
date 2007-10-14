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

#ifndef __NXOGRE_SHAPEDESCRIPTION_PRIMITIVES_H__
#define __NXOGRE_SHAPEDESCRIPTION_PRIMITIVES_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreShapeDescription.h"		// For: These classes inherit ShapeDescription
#include "NxOgreShape.h"				// For: Default Params

namespace NxOgre {

	class NxExport GroundShape : public ShapeDescription {

		public:

			GroundShape(NxReal distance = 0, Ogre::Vector3 normal = Ogre::Vector3(0,1,0), const ShapeParams& = "");
			~GroundShape();

			NxPlaneShapeDesc mShapeDescription;

		protected:

			virtual bool isDynamic() {return false;}
			virtual bool isStaticOnly(){return false;}
			virtual Shape* _bindToActorDescription(Actor*, NxArray<NxShapeDesc*>&);
			virtual Shape* _bindToNxActor(Actor*);


	};

	///////////////////////////////////////////////////////////////////////////////////////////

	class NxExport CubeShape : public ShapeDescription {

		public:

			CubeShape(NxReal width, NxReal height, NxReal length, const ShapeParams& = "");
			CubeShape(NxReal cubed, const ShapeParams& = "");

			~CubeShape();

			NxBoxShapeDesc mShapeDescription;

		protected:

			virtual bool isDynamic() {return false;}
			virtual bool isStaticOnly(){return false;}
			virtual Shape* _bindToActorDescription(Actor*, NxArray<NxShapeDesc*>&);
			virtual Shape* _bindToNxActor(Actor*);


		private:

			void _buildDescription(NxReal,NxReal,NxReal);


	};

	///////////////////////////////////////////////////////////////////////////////////////////

	class NxExport SphereShape : public ShapeDescription {

		public:

			SphereShape(NxReal radius, const ShapeParams& = "");
			~SphereShape();

			NxSphereShapeDesc mShapeDescription;

		protected:

			virtual bool isDynamic() {return false;}
			virtual bool isStaticOnly(){return false;}
			virtual Shape* _bindToActorDescription(Actor*, NxArray<NxShapeDesc*>&);
			virtual Shape* _bindToNxActor(Actor*);


	};

	///////////////////////////////////////////////////////////////////////////////////////////

	class NxExport CapsuleShape : public ShapeDescription {

		public:

			CapsuleShape(NxReal radius, NxReal height, const ShapeParams& = "");
			~CapsuleShape();

			NxCapsuleShapeDesc mShapeDescription;

		protected:

			virtual bool isDynamic() {return false;}
			virtual bool isStaticOnly(){return false;}
			virtual Shape* _bindToActorDescription(Actor*, NxArray<NxShapeDesc*>&);
			virtual Shape* _bindToNxActor(Actor*);


	};

	///////////////////////////////////////////////////////////////////////////////////////////

};

#endif
