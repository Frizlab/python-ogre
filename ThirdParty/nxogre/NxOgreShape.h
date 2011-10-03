/** \file    NxOgreShape.h
 *  \brief   Header for the ShapeParams and Shape classes.
 *  \version 1.0-21
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

#ifndef __NXOGRE_SHAPE_H__
#define __NXOGRE_SHAPE_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreParams.h"
#include "NxOgreDualIdentifier.h"
#include "BetajaenCC.h"

namespace NxOgre {

/** \brief ShapeParams are a way to give arguments to an Shape's constructor in any order and with arguments
           omitted. Params can be given in a string format or passed along as a class reference.
    \example
          Simple ShapeParams are created normally, each member variable is automatically given a default value
          and at any time can be passed onto an Actor's constructor.
          <code>
             ShapeParams params;
             params.mMaterial = 2;
             mScene->createActor(..., new Shape(..., params), ...);
          </code>
          To make your code more readable and understandble, in most cases where the params are considered "light"
          you can use a string variation instead.
          <code>
             mScene->createActor(...., new Shape(..., "material: 2"), ...);
          </code>
          To know the string param name from the member variable name; it follows a simple rule.
             "Camel case is broken up by dashes, then lowercase and drop the "m" prefix".
              => mMass = mass
              => mAngularVelocity = angular-velocity
              => mInternalThreadCount = internal-thread-count
*/
class NxPublicClass ShapeParams : public Params {

  public:

    /** \brief Default Shapeparams constructor, which makes every "param" it's default value.
    */
    ShapeParams();

    /** \brief Alternative constructor for strings. Before the string is parsed, each param is given
               it's default value, so every param doesn't have to be included.
    */
    ShapeParams(const char* p);

    /** \brief Alternative constructor for strings. Before the string is parsed, each param is given
               it's default value, so every param doesn't have to be included.
    */
    ShapeParams(NxString p);

    /** \brief Set's the Params to default based on NxShapeDesc, NxBodyDesc defaults and NxOgre policy.
        \note  Not necessary to use after a constructor, as the constructor already calls this.
    */
    void  setToDefault();

    /** \brief Pose of the Shape relative to the Actor's centre.
        \default NxMat34Id;
        \example
          C++     ->  mActorParams.mLocalPose.t = NxVec3(0, 1, 0);
          String  ->  "position: 0 1 0", "orientation: 1 0 1 1"
        \see NxShapeDesc::localPose
    */
    NxMat34 mLocalPose;

    /** \brief Pre-made CCD skeleton to use.
        \note  NxString is the name of the mesh to use, where as Mesh* is the pointer of the mesh.
        \default NULL and NxString()
        \example
          C++     ->  mActorParams.mSkeleton = skeleton_mesh;
          String  ->  "skeleton: skeleton-name" (Only name available).
        \see NxShapeDesc::ccdSkeleton
        \see NxOgre::Resources::Mesh
    */
    NxEither<NxOgre::Resources::Mesh*, NxString> mSkeleton;

    /** \brief Material to use on the shape.
        \default 0 and "Default"
        \example
          C++     ->  mActorParams.mMaterial = spongy_material;
          String  ->  "material: 1" or "material: spongy"
        \see NxShapeDesc::materialIndex
    */
    MaterialIdentifier mMaterial;

    /** \brief ShapeGroup that this shape is in.
        \default 0 and "Default"
        \example
          C++     ->  mShapeParams.mGroup = sponge_group
          String  ->  "group: 1" or "group: sponge"
        \see NxShapeDesc::group
    */
    GroupIdentifier mGroup;

    /** \brief Group bit mask used for filtering.
        \default 0
        \example
          C++     ->  mShapeParams.mGroup = 1
          String  ->  "groups-mask: 1"
        \see NxShapeDesc::group
    */
    NxGroupsMask mGroupsMask;

    /** \brief How much shapes can interpenetrate.
        \default 0
        \example
          C++     ->  mShapeParams.mGroupsMask = 1
          String  ->  "groups-mask: 1"
        \see NxShapeDesc::skinWidth
    */
    NxReal mSkinWidth;

    /** \brief Mass of this individual shape when computing mass inertial properties for a rigidbody.
               When mass<=0.0 then density and volume determine the mass. Note that this will only be
               used if the body has a zero inertia tensor, or if you call Actor::updateMassFromShapes
               explicitly.
        \default -1.0 (Calculate from density)
        \example
          C++     ->  mShapeParams.mMasss = 1
          String  ->  "mass: 1"
        \see NxShapeDesc::mass
    */
    NxReal mMass;

    /** \brief Density of this individual shape when computing mass inertial properties for a rigidbody
               (unless a valid mass >0.0 is provided). Note that this will only be used if the body has
                a zero inertia tensor, or if you call #NxActor::updateMassFromShapes explicitly.
        \default 1.0
        \example
          C++     ->  mShapeParams.mMass = 2.5
          String  ->  "density: 2.5"
        \see NxShapeDesc::density
    */
    NxReal mDensity;

  struct NxPublicClass ShapeFlags : Flags {

    friend class ShapeParams;

  public:

    ShapeFlags();

    /** \brief Alternate way of adding a flag using the NxShapeFlag enum.
    \example
    params.mFlags += NX_SF_VISUALIZATION;
    */
    void operator+=(NxShapeFlag);

    /** \brief Alternate way of removing a flag using the NxShapeFlag enum.
    \example
    params.mFlags -= NX_SF_VISUALIZATION;
    */
    void operator-=(NxShapeFlag);

    /** \brief  Visualise the actor with the debug renderer.
        \default true
        \see NX_SF_VISUALIZATION
        \example
        String variations => +visualisation, +vis
    */
    bool mVisualisation;

    /** \brief  Disable collision detection for this shape (counterpart of ActorParams::ActorFlags::mDisableCollision)
        \warning IMPORTANT: this is only used for compound objects! Use ActorParams::ActorFlags::mDisableCollision otherwise.
        \default false
        \see NX_SF_DISABLE_COLLISION
        \example
        String variations => +disable-collision, +no-col
    */
    bool mDisableCollision;


    /** \brief  Enable feature indices in contact stream.
        \default false
        \see NX_SF_FEATURE_INDICES
        \example
        String variations => +feature-indices, +fi
    */
    bool mFeatureIndices;


    /** \brief  Disable raycasting for this shape.
        \default false
        \see NX_SF_DISABLE_RAYCASTING
        \example
        String variations => +disable-raycasting, +no-ray
    */
    bool mDisableRaycasting;


    /** \brief  Enable contact force reporting per contact point in contact stream (otherwise we only report force per actor pair).
        \default false
        \see NX_SF_POINT_CONTACT_FORCE
        \example
        String variations => +point-contact-force, +pcf
    */
    bool mPointContactForce;


    /** \brief  Disable collision response for this shape (counterpart of ActorParams::ActorFlags::mDisableResponse)
        \warning not supported by cloth / soft bodies
        \default false
        \see NX_SF_DISABLE_RESPONSE
        \example
        String variations => +disable-response, +no-res
    */
    bool mDisableResponse;


    /** \brief  Enable dynamic-dynamic CCD for this shape. Used only when CCD is globally enabled and shape have a CCD skeleton.
        \default false
        \see NX_SF_DYNAMIC_DYNAMIC_CCD
        \example
        String variations => +dynamic-dynamic-ccd, +ccd
    */
    bool mDynamicDynamicCCD;

    /** \brief  Disable participation in ray casts, overlap tests and sweeps.
        \note   Setting this flag for static non-trigger shapes may cause incorrect behavior for 
                colliding fluid and cloth.
        \default false
        \see NX_SF_DISABLE_SCENE_QUERIES
        \example
        String variations => +disable-scene-queries, +no-sq
    */
    bool mDisableSceneQueries;


    /** \brief Set the flags to their default state, according to the NxShapeDesc::setToDefault method.
        mVisualisation = true
        mDisableCollision = false
        mFeatureIndices = false
        mDisableRaycasting = false
        mPointContactForce = false
        mDisableResponse = false
        mDynamicDynamicCCD = false
        mDisableSceneQueries = false
    */
    void setToDefault();

    /** \internal
    */
    void  fromNxU32(NxU32);

    /** \internal
    */
    NxU32 toNxU32() const;

  protected:

    /** \internal
    */
    void  parse(const NxString&);

  };

  /*
  */
  ShapeFlags mShapeFlags;


  struct NxPublicClass ShapeFluidFlags : Flags {

    friend class ShapeParams;

  public:

    ShapeFluidFlags();

    /** \brief Alternate way of adding a flag using the NxShapeFlag enum.
    \example
    params.mFlags += NX_SF_FLUID_DISABLE_COLLISION;
    */
    void operator+=(NxShapeFlag);

    /** \brief Alternate way of removing a flag using the NxShapeFlag enum.
    \example
    params.mFlags -= NX_SF_FLUID_DISABLE_COLLISION;
    */
    void operator-=(NxShapeFlag);

    /** \brief  Disable collision with fluids.
        \default false
        \see NX_SF_FLUID_DISABLE_COLLISION
        \example
        String variations => +disable-fluid-collision, +no-fc
    */
    bool mDisableFluidCollision;

    /** \brief  Enables the reaction of the shapes actor on fluid collision.
        \default false
        \see NX_SF_FLUID_TWOWAY
        \example
        String variations => +two-way-fluid-collision, +2wfc
    */
    bool mTwoWayFluidCollision;


    /** \brief  Sets the shape to be a fluid drain.
        \default false
        \see NX_SF_FLUID_DRAIN
        \example
        String variations => +fluid-drain, +fld
    */
    bool mFluidDrain;

    /** \brief Set the flags to their default state, according to the NxShapeDesc::setToDefault method.
        mDisableFluidCollision = false
        mTwoWayFluidCollision = true
        mFluidDrain = false
    */
    void setToDefault();

    /** \internal
    */
    void  fromNxU32(NxU32);

    /** \internal
    */
    NxU32 toNxU32() const;

  protected:

    /** \internal
    */
    void  parse(const NxString&);

  };

  /*
  */
  ShapeFluidFlags  mFluidFlags;


  struct NxPublicClass ShapeClothFlags : Flags {

    friend class ShapeParams;

  public:

    ShapeClothFlags();

    /** \brief Alternate way of adding a flag using the NxShapeFlag enum.
    \example
    params.mFlags += NX_SF_CLOTH_DRAIN;
    */
    void operator+=(NxShapeFlag);

    /** \brief Alternate way of removing a flag using the NxShapeFlag enum.
    \example
    params.mFlags -= NX_SF_CLOTH_DRAIN;
    */
    void operator-=(NxShapeFlag);

    /** \brief  Sets the shape to be a cloth drain.
        \default false
        \see NX_SF_CLOTH_DRAIN
        \example
        String variations => +cloth-drain, +cld
    */
    bool mClothDrain;


    /** \brief  Disable collision with cloths.
        \default false
        \see NX_SF_CLOTH_DISABLE_COLLISION
        \example
        String variations => +disable-cloth-collision, +no-cl
    */
    bool mDisableClothCollision;


    /** \brief  Enables the reaction of the shapes actor on cloth collision.
        \warning Compound objects cannot use a different value for each constituent shape.
        \default false
        \see NX_SF_CLOTH_TWOWAY
        \example
        String variations => +two-way-cloth-collision, +2wclc
    */
    bool mTwoWayClothCollision;

    /** \brief Set the flags to their default state, according to the NxShapeDesc::setToDefault method.
        mFluidDrain = false
    */
    void setToDefault();

    /** \internal
    */
    void  fromNxU32(NxU32);

    /** \internal
    */
    NxU32 toNxU32() const;

  protected:

    /** \internal
    */
    void  parse(const NxString&);

  };

  /*
  */
  ShapeClothFlags mClothFlags;


  struct NxPublicClass ShapeSoftBodyFlags : Flags {

    friend class ShapeParams;

  public:

    ShapeSoftBodyFlags();

    /** \brief Alternate way of adding a flag using the NxShapeFlag enum.
    \example
    params.mFlags += NX_SF_SOFTBODY_DRAIN;
    */
    void operator+=(NxShapeFlag);

    /** \brief Alternate way of removing a flag using the NxShapeFlag enum.
    \example
    params.mFlags -= NX_SF_SOFTBODY_DRAIN;
    */
    void operator-=(NxShapeFlag);

    /** \brief  Sets the shape to be a softbody drain.
        \default false
        \see NX_SF_SOFTBODY_DRAIN
        \example
        String variations => +softbody-drain, +sbd
    */
    bool mSoftBodyDrain;


    /** \brief  Disable collision with softbodys.
        \default false
        \see NX_SF_SOFTBODY_DISABLE_COLLISION
        \example
        String variations => +disable-softbody-collision, +no-sb
    */
    bool mDisableSoftBodyCollision;


    /** \brief  Enables the reaction of the shapes actor on softbody collision.
        \warning Compound objects cannot use a different value for each constituent shape.
        \default false
        \see NX_SF_SOFTBODY_TWOWAY
        \example
        String variations => +two-way-softbody-collision, +2wsbc
    */
    bool mTwoWaySoftBodyCollision;

    /** \brief Set the flags to their default state, according to the NxShapeDesc::setToDefault method.
        mFluidDrain = false
    */
    void setToDefault();

    /** \internal
    */
    void  fromNxU32(NxU32);

    /** \internal
    */
    NxU32 toNxU32() const;

  protected:

    /** \internal
    */
    void  parse(const NxString&);

  };

  /*
  */
  ShapeSoftBodyFlags mSoftBodyFlags;


  /** \brief The callback to trigger which is called another shape enters, leaves or exists within this shape.
      \default NULL (No trigger)
      \example
        C++     ->  mShapeParams.mTriggerCallback = trigger_ptr
      \see Trigger
  */
  TriggerContactCallback* mTrigger;

  void parse(Parameters);


};// End of ShapeParams class

////////////////////////////////////////////////////////////////////////////////////////////////

/** \brief Shapes are wonderful things, they make matter spongy.
*/
class NxPublicClass Shape {

	public:


    /** \brief Shape Constructor.
        This is an empty class, create a shape using the constructor
        of the shape you wish to create; new Cube(), new Sphere(), etc.
    */
    Shape(NxShapeDesc* shape_description, const ShapeParams& = ShapeParams());

    /** \brief Alternate Shape constructor.
               This is an empty class, create a shape using the constructor
               of the shape you wish to create; new Cube(), new Sphere(), etc.
        \note  For usage when you don't want the params to be transfered to the NxShapeDesc
               straight away.
    */
    Shape();

    /** \brief Shape Destructor.
    */
    virtual  ~Shape();


    /** \brief Copy this shape to another Actor
    */
    virtual void  copyTo(Actor*, ShapeParams = ShapeParams()) = 0;


    /** \brief Move this shape to another Actor
    */
    virtual void  moveTo(Actor*, ShapeParams = ShapeParams()) = 0;


    /** \brief Get the abstract NxShape.
    */
    virtual NxShape*  getNxShape();


    /** \brief Get the CCD skeleton used with this shape
    */
    virtual NxOgre::Resources::Mesh*  getSkeleton();


    /** \brief Get the index assigned to this shape of an actor.
    */
    NxShapeIndex  getIndex() const;


    /** \brief Set the index assigned to this shape of an actor.
    */
    void  setIndex(NxShapeIndex index);


    /** \brief Get the String type of this shape
    */
    virtual NxString  getShapeAsString()	const;

    /** \brief Get the Hash type of this shape.
    */
    virtual NxShortHashIdentifier  getTypeHash() const;

    /** \brief Has the shape been attached to an actor yet?
    */
    virtual bool  isAttached() const;


    /** \brief Get's a copy of the TriggerCallback, otherwise NULL.
    */
    TriggerContactCallback*  getTriggerCallback();


    /** \brief Implement the shape based on description and parameters to an existing
               actor.
    */
    virtual void  createShape(NxActor* actor, NxShapeIndex, Scene* scene) = 0;


    /** \brief Implement the shape based on description and parameters to an actor
               description.
    */
    virtual void  createShape(NxArray<NxShapeDesc*>& shapes, NxShapeIndex, Scene* scene) = 0;


    /** \brief Recieve the copy of the NxShape, and it's index to the actor.
        \note  Used after creating the NxShape, the actor gives the shape the nxshape
               and it's index.
    */
    virtual void  setNxShape(NxShape*);


    /** \brief Release the shape based on description and parameters
    */
    virtual void  releaseShape() = 0;


    /** \brief Shape params to description
    */
    virtual void  paramsToDescription(NxShapeDesc*, ShapeParams*);


    /** \brief Extended Shape params to description
    */
    void  extendedParamsToDescription(Scene*, const ShapeParams& params, NxShapeDesc*);

    virtual void  setLocalPose(const Pose&);
    virtual void  setSkeleton(Resources::Mesh*);
    virtual void  setMaterial(NxMaterialIndex);
    virtual void  setGroup(NxU32);
    virtual void  setSkinWidth(NxReal);
    virtual void  setFlag(NxShapeFlag, bool value);



    /** \brief Is the shape description valid? In Debug mode details of why it's invalid
	    is reported to the error.
    */
    bool  isValid(NxShapeDesc&) const;

    NxShapeIndex                mShapeIndex;
    ShapeParams                 mParams;
    NxActor*                    mActor;
    NxShape*                    mNxShape;
    Resources::Mesh*            mSkeleton;
    VoidPointer*                mVoidPointer;
    TriggerContactCallback*     mTriggerCallback;

};

/////////////////////////////////////////////////////////


/** \brief A Compound Shape, isn't really a shape. It just groups many shapes together that
           can be used as one. Usually Compound shapes are used to make complicated actors
           that can't be represented solely by one shape; Tables are a good example of this.
    \note  After the CompoundShape has been passed to the Actor, the contents are taken out
           and the CompoundShape pointer is deleted.
*/
class NxPublicClass CompoundShape : public Shape {

		friend class Actor;

		public:
			
			CompoundShape();
			~CompoundShape();

			void add(Shape*);

			virtual void copyTo(Actor*, ShapeParams = ShapeParams()) {NxUnderConstruction;}
			virtual void moveTo(Actor*, ShapeParams = ShapeParams()) {NxUnderConstruction;}

			NxShortHashIdentifier  getTypeHash() const;

		protected:

			virtual void createShape(NxActor* actor, NxShapeIndex, Scene* scene);
			virtual void createShape(NxArray<NxShapeDesc*>& shapes, NxShapeIndex, Scene* scene);
			virtual void releaseShape();

			unsigned int                 mNbShapes;
			Betajaen::SharedList<Shape>  mShapes;

	};
};

#endif
