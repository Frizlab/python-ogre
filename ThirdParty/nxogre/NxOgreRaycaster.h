/** \file    NxOgreRaycaster.h
 *  \brief   Header for the Raycaster class and ActorRaycastHit struct.
 *  \version 1.0-22T2
 *
 *  \licence NxOgre a wrapper for the PhysX physics library.
 *           Copyright © 2005-2008 Robin Southern of NxOgre.org http://www.nxogre.org
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

#ifndef __NXOGRE_RAYCASTER_H__
#define __NXOGRE_RAYCASTER_H__

#include "NxOgrePrerequisites.h"
#include "BetajaenCC.h"

namespace NxOgre {

	/** \brief Full details of a raycast hit of an Actor
	*/
	class NxPublicClass ActorRaycastHit
	{

		public:

			ActorRaycastHit();
			ActorRaycastHit(const NxRaycastHit&);
			~ActorRaycastHit();

			operator Actor*();

			NxVec3  getImpact() const;
			float3  getImpactAsFloat3() const;
#if (NX_USE_OGRE == 1)
			Ogre::Vector3  getImpactAsOgreVector3() const;
#endif

			NxVec3  getNormal() const;
			float3  getNormalAsFloat3() const;
#if (NX_USE_OGRE == 1)
			Ogre::Vector3  getNormalAsOgreVector3() const;
#endif

			/** \brief Actor the raycast hit
			*/
			Actor*  mActor;

			/** \brief Touched NxShape of the Actor(associated flags: NX_RAYCAST_SHAPE)
			    \see flags
			*/
			NxShape*  mShape;

			/** \brief Impact point in world space (associated flags: NX_RAYCAST_IMPACT)
			    \see flags
			*/
			NxVec3  mWorldImpact;
							
			/** \brief Impact normal in world space (associated flags: NX_RAYCAST_NORMAL / NX_RAYCAST_FACE_NORMAL)

			    For #NxConvexShape and #NxTriangleMeshShape NX_RAYCAST_NORMAL generates a smooth normal. If a true
			    face normal is required use NX_RAYCAST_FACE_NORMAL instead when specifying hint flags to a raycast method.

				\see flags
			*/
			NxVec3  mWorldNormal;

			/** \brief Index of touched face (associated flags: NX_RAYCAST_FACE_INDEX)

			    The face index is for the mesh <b>before</b> cooking. During the cooking process faces are moved 
			    around which changes there index. However the SDK stores a table which is used to map the index to the original
			    mesh before it is returned for a raycast hit.

			    \see flags
			*/
			NxU32  mFaceID;

			/** \brief Same as mFaceID
			*/
			NxTriangleID  mInternalFaceID;

			/** \brief Distance from ray start to impact point (associated flags: NX_RAYCAST_DISTANCE)
			    \see flags
			*/
			NxReal  mDistance;
			
			/** \brief Impact barycentric coordinates (associated flags: NX_RAYCAST_UV)
			    \see flags
			*/
			NxReal mU;

			/** \brief Impact barycentric coordinates (associated flags: NX_RAYCAST_UV)
			    \see flags
			*/
			NxReal mV;

			/** \brief Index of touched material (associated flags: NX_RAYCAST_MATERIAL)
			    \see flags
			*/
			NxMaterialIndex  mMaterialIndex;

			/** \brief Combination of ::NxRaycastBit, when a corresponding flag is set, then the member is valid.
			::NxRaycastBit flags can be passed to raycasting functions, as an optimization, to cause the SDK to
			only generate specific members of this structure.
			*/
			NxU32  mFlags;

	};

	/** \brief A Raycaster which will cast a ray to the bounding box or polygon level
	*/
	class NxPublicClass Raycaster : public NxUserRaycastReport {

	friend class Scene;
	friend class Betajaen::Flat<Raycaster*, Betajaen::SharedAllocator>;
	friend class Betajaen::SharedList<Raycaster, Betajaen::SharedAllocator>;

	public:

		enum RaycastType {
			RCT_Any        = 0,
			RCT_Closest    = 1,
			RCT_All        = 2
		};

		enum ActorFilter {
			AF_Static      = NX_STATIC_SHAPES,
			AF_Dynamic     = NX_DYNAMIC_SHAPES,
			AF_None        = NX_ALL_SHAPES,
		};
		
		enum Accuracy {
			/** Cast to the bounding box level */
			AC_Fast        = 0,
			/** Cast to the polygon level */
			AC_Accurate    = 1,
		};

		/** \brief Set to Type: Any, Filter: None, Accuracy: Accurate.
		*/
		void setToDefault();

		/** \brief   To specify which field of NxRaycastHit they are interested in. See NxRaycastBit
		    \default 0xffffffff
		    \note    This only applies to (AC_Fast) bound raycast
		*/
		void hint(NxU32);

		/** \brief Maximum distance to raycast to.
		    \default NX_MAX_F32
		*/
		void distance(NxReal = NX_MAX_F32);

		/** \brief Accuracy
		    \default AC_Fast
		*/
		void accuracy(Accuracy = AC_Fast);

		/** \brief Type of Raycast (Any, Closest or All)
		    \default RCT_Any
		*/
		void type(RaycastType = RCT_Any);
		
		/** \brief Apply a filter
		    \default NULL
		*/
		void filter(ActorFilter = AF_None);

		/** \brief Mask used to filter shape objects.
		    \default 0xFFFFFFFF
		*/
		void group(NxU32 = 0xFFFFFFFF);

		/** \brief Add a group mask (ignores, and sets groupMask to 0xffffffff)
		    \default NULL
		*/
		void groupMask(NxGroupsMask);

		/** \brief Set origin of the ray
		*/
		void origin(const NxVec3&);

		/** \brief Set the origin of the ray as a float3
		*/
		void origin(const float3&);

#if (NX_USE_OGRE == 1)
		/** \brief Set the origin of the ray as a Ogre Vector3
		*/
		void origin(const Ogre::Vector3&);
#endif

		/** \brief Direction of the ray (normalised)
			\param NxVec3& Direction of the ray.
			\param isNormalised If false, the direction will normalise it for you.
		*/
		void normal(const NxVec3&, bool normalise = false);

		/** \brief Direction of the ray (normalised)
			\param NxVec3& Direction of the ray.
			\param isNormalised If false, the direction will normalise it for you.
		*/
		void normal(const float3&, bool normalise = false);

#if (NX_USE_OGRE == 1)
		/** \brief Direction of the ray (normalised)
			\param NxVec3& Direction of the ray.
			\param isNormalised If false, the direction will normalise it for you.
		*/
		void normal(const Ogre::Vector3&, bool normalise = true);
#endif
		/** \brief Cast function
		*/
		bool cast();

		/** \brief Get a iterator to the actors found in the cast
		    \example
		      <code>
		        mRay->cast();
		        ActorRaycastHitNodeIterator actors = mRay->Actors();
		        for (ActorRaycastHit* hit = actors.Begin();hit = actors.Next();) {
		          // code
		        }
		      </code>
		*/
		ActorRaycastHitNodeIterator Actors();

	protected:

		/** \brief Default constructor.
		*/
		Raycaster();

		/** \brief Pretty obvious, is it not?
		*/
		~Raycaster();

///NX_PROTECTED_BLOCK_BEGIN

		/// \internal
		bool _castBounds(const NxRay&);

		/// \internal
		bool _castShape(const NxRay&);

		/// \internal
		bool onHit(const NxRaycastHit&);

///NX_PROTECTED_BLOCK_END

	protected:

///NX_PROTECTED_BLOCK_BEGIN

		Scene*            mScene;
		RaycastType       mType;
		ActorFilter       mFilter;
		Accuracy          mAccuracy;
		NxRay             mRay;
		NxReal            mDistance;
		NxVec3            mOrigin;
		NxVec3            mNormal;
		NxU32             mGroup;
		NxGroupsMask      mGroupMask;
		NxU32             mHint;
		ActorRaycastHits  mActors;

		// Betajaen::SharedList<CharacterRaycast> mCharacter;
		// Betajaen::SharedList<SimpleActorRaycast> mSimpleActors;

///NX_PROTECTED_BLOCK_END

	};

};

#endif
