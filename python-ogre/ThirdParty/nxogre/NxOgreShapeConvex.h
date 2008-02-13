/** \file    NxOgreShapeConvex.h
 *  \brief   Header for the Convex and Prism classes.
 *  \version 1.0-20
 *
 *  \licence NxOgre a wrapper for the PhysX physics library.
 *           Copyright (C) 2005-8 Robin Southern of NxOgre.org http://www.nxogre.org
 *           This library is free software; you can redistribute it and/or
 *           modify it under the terms of the GNU Lesser General Public
 *           License as published by the Free Software Foundation; either
 *           version 2.1 of the License, or (at your option) any later version.
 *           
 *           This library is distributed in the hope that it will be useful,
 *           but WITHOUT ANY WARRANTY; without even the implied warranty of
 *           MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *           Lesser General Public License for more details.
 *           
 *           You should have received a copy of the GNU Lesser General Public
 *           License along with this library; if not, write to the Free Software
 *           Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __NXOGRE_SHAPE_CONVEX_H__
#define __NXOGRE_SHAPE_CONVEX_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreShape.h"				// For: Primitives inherits Shape

namespace NxOgre {

	/////////////////////////////////////////////////////////

	/** \brief Convex (NxConvexShape) provide a convex shape.
		\note For Concave shapes; a collection of Convex shapes is used to make up the various
		parts of that shape.
	*/
	class NxPublicClass Convex : public Shape {

		public:

			/** \brief Convex Constructor, using a NxConvexMesh pointer as a mesh to use.
			*/
			Convex(NxConvexMesh* convexMeshPtr, const ShapeParams& = ShapeParams());

			/** \brief Convex Constructor, using a direct transitionalMesh
			*/
			Convex(TransitionalMesh* transitionalMesh, const ShapeParams& = ShapeParams());

			/** \brief Convex Constructor, using a resource as a mesh.
			*/
			Convex(const NxString& convexMeshIdentifier, const ShapeParams& = ShapeParams());

#if NX_USE_OGRE
			/** \brief Convex Constructor, with scale (using Ogre::Vector3)
			*/
			Convex(const NxString& convexMeshIdentifier, Ogre::Vector3 scale, const ShapeParams& = ShapeParams());
#endif

			/** \brief Convex Constructor, with scale (using NxVec3)
			*/
			Convex(const NxString& convexMeshIdentifier, NxVec3 scale, const ShapeParams& = ShapeParams());


			/** \brief Convex destructor
			*/
			~Convex();

			virtual void copyTo(Actor*, ShapeParams = ShapeParams()) {NxUnderConstruction;}
			virtual void moveTo(Actor*, ShapeParams = ShapeParams()) {NxUnderConstruction;}

		protected:

			virtual void createShape(NxActor* actor, NxShapeIndex, Scene* scene);
			virtual void createShape(NxArray<NxShapeDesc*>& shapes, NxShapeIndex, Scene* scene);
			virtual void releaseShape();

		protected:

			NxConvexShapeDesc	mShapeDescription;
			NxString			mMeshIdentifier;
			NxConvexMesh*		mMesh;

	};

	//////////
	////////// TEMP
	//////////

#if 0

	/////////////////////////////////////////////////////////

	/** \brief Prism (NxConvexShape) provides a n-sided prism convex shape.
		\note This shape creates a mesh at will based on parameters, but will use any
			  mesh that has been created before hand with the same parameters.
	*/
	class NxPublicClass Prism : public Shape {
		
	
		public:

			/** \brief Prism shape to generate.
				\params height Height of the prism
				\params width Width of the prism
				\params nbSides Number of the sides (must be more or equal to three).
			*/
			Prism(NxReal height, NxReal width, NxU32 nbSides, const ShapeParams& = ShapeParams());

			/** \brief Convex destructor
			*/
			~Prism();

			virtual void copyTo(Actor*, ShapeParams = ShapeParams()) {NxUnderConstruction;}
			virtual void moveTo(Actor*, ShapeParams = ShapeParams()) {NxUnderConstruction;}

		protected:	

			virtual void createShape(NxActor* actor, NxShapeIndex);
			virtual void createShape(NxArray<NxShapeDesc*>& shapes, NxShapeIndex);
			virtual void releaseShape();
		
		protected:

			NxConvexShapeDesc	mShapeDescription;
			NxString			mMeshIdentifier;
			NxConvexMesh*		mMesh;
			NxReal				mWidth;
			NxReal				mHeight;
			NxU32				mNbSides;

	};

	/////////////////////////////////////////////////////////

#endif

} // End of namespace

#endif
 