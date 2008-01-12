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

#ifndef __NXOGRE_SHAPE_PRIMITIVES_H__
#define __NXOGRE_SHAPE_PRIMITIVES_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreShape.h"				// For: Primitives inherits Shape

namespace NxOgre {
	
	//////////////////////////////////////////////////////////////////////////////////////

	class NxExport Ground : public Shape {
	
		friend class GroundShape;

		public:

			Ground(GroundShape&, Actor*, NxArray<NxShapeDesc*>&);
			Ground(GroundShape&, Actor*);
			~Ground();
	
			virtual void copyTo(Actor*);
			virtual void moveTo(Actor*);

			virtual NxShape* getNxShape()	{return mShape;}
			virtual void releaseShape();


		protected:

			virtual ShapeBlueprint*	getBlueprint();


			virtual bool isDynamic() {return false;}
			virtual bool isStaticOnly(){return false;}
			void _bindNxShapeToShape(NxShape*);

			NxPlaneShape *mShape;

		private:
	};

	//////////////////////////////////////////////////////////////////////////////////////

	class NxExport Cube : public Shape {
	
		friend class CubeShape;

		public:

			Cube(CubeShape&, Actor*, NxArray<NxShapeDesc*>&, Skeleton* s = 0);
			Cube(CubeShape&, Actor*, Skeleton* s = 0);

			~Cube();

			virtual void copyTo(Actor*);
			virtual void moveTo(Actor*);

			virtual NxShape* getNxShape()	{return mShape;}
			virtual void releaseShape();

		protected:

			virtual ShapeBlueprint*	getBlueprint();

			virtual bool isDynamic()		{return false;}
			virtual bool isStaticOnly()		{return false;}
			void _bindNxShapeToShape(NxShape*);

			NxBoxShape *mShape;	

		private:

	};

	//////////////////////////////////////////////////////////////////////////////////////

	class NxExport Sphere : public Shape {
	
		friend class SphereShape;

		public:

			Sphere(SphereShape&, Actor*, NxArray<NxShapeDesc*>&);
			Sphere(SphereShape&, Actor*);
			~Sphere();
/*
			virtual void copyTo(Actor*);
			virtual void moveTo(Actor*);

			virtual NxShape* getNxShape()	{return mShape;}
			virtual void releaseShape();
*/
		protected:

			virtual ShapeBlueprint*	getBlueprint();

			virtual bool isDynamic() {return false;}
			virtual bool isStaticOnly(){return false;}
			void _bindNxShapeToShape(NxShape*);

			NxSphereShape *mShape;		
		
		private:

	};

	//////////////////////////////////////////////////////////////////////////////////////

	class NxExport Capsule : public Shape {
	
		friend class CapsuleShape;

		public:

			Capsule(CapsuleShape&, Actor*, NxArray<NxShapeDesc*>&);
			Capsule(CapsuleShape&, Actor*);
			~Capsule();
/*
			virtual void copyTo(Actor*);
			virtual void moveTo(Actor*);

			virtual NxShape* getNxShape()	{return mShape;}
			virtual void releaseShape();
*/
		protected:

			virtual ShapeBlueprint*	getBlueprint();

			virtual bool isDynamic() {return false;}
			virtual bool isStaticOnly(){return false;}


			void _bindNxShapeToShape(NxShape*);

			NxCapsuleShape *mShape;			

		private:

	};

};

#endif
