/** \file    NxOgreActor.h
*  \brief   Header for the ActorParams and Actor classes.
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

#ifndef __NXOGRE_ACTOR_H__
#define __NXOGRE_ACTOR_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreContainer.h"		// For: mCollisionList
#include "NxOgreParams.h"
#include "NxOgrePose.h"
#include "BetajaenCC.h"
#include "NxOgreShape.h"			// For: SharedList<Shape>

namespace NxOgre {

/** \brief ActorParams are a way to give arguments to an Actor's constructor in any order and with arguments
           omitted. Params can be given in a string format or passed along as a class reference.
    \example
           Simple ActorParams are created normally, each member variable is automatically given a default value
           and at any time can be passed onto an Actor's constructor.
           <code>
            ActorParams params;
            params.mMass = 10;
            mScene->createActor(..., params);
           </code>
           To make your code more readable and understandble, in most cases where the params are considered "light"
           you can use a string variation instead.
           <code>
            mScene->createActor(...., "mass: 10");
           </code>
           To know the string param name from the member variable name; it follows a simple rule.
           "Camel case is broken up by dashes, then lowercase and drop the "m" prefix".
            - mMass = mass
            - mAngularVelocity = angular-velocity
            - mInternalThreadCount = internal-thread-count
*/
class NxPublicClass ActorParams : public Params {

public:

  /** \brief Default Actorparams constructor, which makes every "param" it's default value.
  */
  ActorParams();

  /** \brief Alternative constructor for strings. Before the string is parsed, each param is given
  it's default value, so every param doesn't have to be included.
  */
  ActorParams(const char* p);

  /** \brief Alternative constructor for strings. Before the string is parsed, each param is given
  it's default value, so every param doesn't have to be included.
  */
  ActorParams(NxString p);

  /** \brief Set's the Params to default based on NxActorDesc, NxBodyDesc defaults and NxOgre policy.
  \note  Not necessary to use after a constructor, as the constructor already calls this.
  */
  void  setToDefault();

  /** \brief Tell's you if the Params have valid values.
  */
  bool isValid() const;

  /** \brief ActorFlags is a simple class wrapper for the NxActorFlags enum.
  */
  struct NxPublicClass ActorFlags : Flags {

    friend class ActorParams;

  public:

    /** \brief Normal constructor.
    */
    ActorFlags();

    /** \brief Alternate way of adding a flag using the NxBodyFlag enum.
        \example
        params.mFlags += NX_BF_KINEMATIC;
    */
    void operator+=(NxActorFlag);

    /** \brief Alternate way of removing a flag using the NxBodyFlag enum.
        \example
        params.mFlags -= NX_BF_KINEMATIC;
    */
    void operator-=(NxActorFlag);

    /** \brief  Disable collision detection
        \default false
        \see NX_AF_DISABLE_COLLISION
        \example
        String variations => +disable-collision, +ghost
    */
    bool mDisableCollision;

    /** \brief  Disable collision response
        \default false
        \see NX_AF_DISABLE_RESPONSE
        \example
        String variations => +disable-response, +silent
    */
    bool mDisableResponse;

    /** \brief  Disables COM update when computing inertial properties at creation time.
        \default false
        \see NX_AF_LOCK_COM
        \example
        String variations => +lock-com
    */
    bool mLockCOM;

    /** \brief  Disable collision with fluids.
        \default false
        \see NX_AF_FLUID_DISABLE_COLLISION
        \example
        String variations => +disable-fluid-collision, +no-fluids
    */
    bool mDisableFluidCollision;

    /** \brief  Turn on contact modification callback for the actor.
    \default false
    \see NX_AF_CONTACT_MODIFICATION, ContactModifier
    \example
    String variations => +contact-modification, +con-mod
    */
    bool mContactModification;

    /** \brief  Force cone friction to be used for this actor.
    \default false
    \see NX_AF_FORCE_CONE_FRICTION
    \example
    String variations => +force-cone-friction, +fcf
    */
    bool mForceConeFriction;

    /** \brief  Enable custom contact filtering
    \default false
    \see NX_AF_USER_ACTOR_PAIR_FILTERING
    \example
    String variations => +actor-pair-filtering, +apf
    */
    bool mActorPairFiltering;

    /** \brief Set the flags to their default state, according to the NxBodyDesc::setToDefault method.
    */
    void  setToDefault();

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


  struct NxPublicClass BodyFlags : Flags {

    friend class ActorParams;

  public:

    BodyFlags();

    /** \brief Alternate way of adding a flag using the NxBodyFlag enum.
    \example
    params.mFlags += NX_BF_KINEMATIC;
    */
    void operator+=(NxBodyFlag);

    /** \brief Alternate way of removing a flag using the NxBodyFlag enum.
    \example
    params.mFlags -= NX_BF_KINEMATIC;
    */
    void operator-=(NxBodyFlag);

    /** \brief  Set if gravity should not be applied on this body
    \default false
    \see NX_BF_DISABLE_GRAVITY
    \example
    String variations => +disable-gravity, +0g
    */
    bool mDisableGravity;

    /** \brief  Enables kinematic mode for the actor.
    \default false
    \see NX_BF_KINEMATIC
    \example
    String variations => +kinematic, +km
    */
    bool mIsKinematic;

    /** \brief  Enable debug renderer for this body.
    \default 
    \see NX_BF_VISUALIZATION
    \example
    String variations => +debug-visualisation, +dv
    */
    bool mHasDebugVisualisation;

    /** \brief  Filter velocities used keep body awake. The filter reduces rapid oscillations and transient spikes.
    \default false
    \see NX_BF_FILTER_SLEEP_VEL
    \example
    String variations => +filter-sleep-velocity, +fsv
    */
    bool mFilterSleepVelocity;

    /** \brief  Enables energy-based sleeping algorithm.
    \default true
    \see NX_BF_ENERGY_SLEEP_TEST
    \example
    String variations => +energy-sleep-test, +est
    */
    bool mEnergySleepTest;

    /** \brief Set the flags to their default state, according to the NxBodyDesc::setToDefault method.
    mDisableGravity       = false;
    mIsKinematic          = false;
    mDebugVisualisation   = true;
    mFilterSleepVelocity  = false;
    mEnergySleepTest      = true;
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

  /** \brief A wrapped class to represent the NX_BF_FROZEN_* flags
  */
  struct NxPublicClass FreezeFlags : Flags {

    friend class ActorParams;

  public:

    /**
    */
    FreezeFlags();

    /** \brief Shortcut function to freeze the position of the actor in every axis.
    */
    void FreezePosition();

    /** \brief Shortcut function to freeze the rotation of the actor in every axis.
    */
    void FreezeRotation();

    /** \brief Shortcut function to freeze the position and rotation of the actor in every axis.
    */
    void FreezeAll();

    /** \brief Full or partial frozen of the Actor,
    \default false, false, false
    \see NX_BF_FROZEN_POS_X, NX_BF_FROZEN_POS_Z, NX_BF_FROZEN_POS_Z
    \example
    String variations => +px +py +pz or +position-all or +all
    */
    struct
    {
      /** \brief X axis to freeze,
      \default false
      \see NX_BF_FROZEN_POS_X
      \example
      String variations => +px
      */
      bool x;

      /** \brief Y axis to freeze,
      \default false
      \see NX_BF_FROZEN_POS_Y
      \example
      String variations => +py
      */
      bool y;

      /** \brief Z axis to freeze,
      \default false
      \see NX_BF_FROZEN_POS_Z
      \example
      String variations => +pz
      */
      bool z;
    } mPosition;


    /** \brief Freeze some or all of the rotation that may happen in the future of the Actor.
    \default false of x,y,z
    \see NX_BF_FROZEN_ROT_X, NX_BF_FROZEN_ROT_Y, NX_BF_FROZEN_ROT_Z
    \example
    String variations => +rx +ry +rz or +rotation-all
    */
    struct
    {
      /** \brief X axis to freeze,
      \default false
      \see NX_BF_FROZEN_ROT_X
      \example
      String variations => +rotation-x, +rx
      */
      bool x;

      /** \brief Y axis to freeze,
      \default false
      \see NX_BF_FROZEN_POS_Y
      \example
      String variations => +rotation-y, +ry
      */
      bool y;

      /** \brief Z axis to freeze,
      \default false
      \see NX_BF_FROZEN_POS_Z
      \example
      String variations => +rotation-z, +pz
      */
      bool z;
    } mRotation;


    /** \brief Set's the flags to not frozen.
    */
    void setToDefault();

    /** \internal
    */
    void  fromNxU32(NxU32);

    /** \internal
    */
    NxU32 toNxU32() const;

  protected:

    void  parse(const NxString&);

  };

  /** \brief ContactFlags is a simple class wrapper for the Nx???? enum.
  */
  struct NxPublicClass ContactFlags : Flags {

    friend class ActorParams;

  public:

    /** \brief Normal constructor.
    */
    ContactFlags();

    /** \brief Alternate way of adding a flag using the NxBodyFlag enum.
    \example
    params.mFlags += NX_BF_KINEMATIC;
    */
    void operator+=(NxContactPairFlag);

    /** \brief Alternate way of removing a flag using the NxBodyFlag enum.
    \example
    params.mFlags -= NX_BF_KINEMATIC;
    */
    void operator-=(NxContactPairFlag);

    /** \brief  Notify contacts on everything
    \default false
    \see NX_NOTIFY_ALL
    \example
    String variations => +all
    */
    void All();

    /** \brief  Disable contact generation for this pair
    \default false
    \see NX_IGNORE_PAIR
    \example
    String variations => +ignore-pair, +ignore
    */
    bool mIgnorePair;

    /** \brief  Pair callback will be called when the pair starts to be in contact.
    \default false
    \see NX_NOTIFY_ON_START_TOUCH
    \example
    String variations => +start-touch, +start
    */
    bool mStartTouch;

    /** \brief  Pair callback will be called when the pair stops to be in contact
    \default false
    \see NX_NOTIFY_ON_END_TOUCH
    \example
    String variations => +start-touch, +end
    */
    bool mEndTouch;

    /** \brief  Pair callback will keep getting called while the pair is in contact
    \default false
    \see NX_NOTIFY_ON_TOUCH
    \example
    String variations => +touch
    */
    bool mTouch;

#ifdef NX_WHEN_EVER_THESE_GET_IMPLEMENTED
    /** \brief  Pair callback will be called when it may be appropriate for the pair to play an impact sound.
    \default false
    \see NX_NOTIFY_ON_IMPACT
    \example
    String variations => +impact
    */
    bool mImpact;

    /** \brief  Pair callback will be called when the pair is in contact and rolling.
    \default false
    \see NX_NOTIFY_ON_ROLL
    \example
    String variations => +roll
    */
    bool mRoll;

    /** \brief  Pair callback will be called when the pair is in contact and sliding (and not rolling).
    \default false
    \see NX_NOTIFY_ON_SLIDE
    \example
    String variations => +slide
    */
    bool mSlide;

#endif

    /** \brief  The (summed total) friction force and normal force will be given in the NxContactPair variable in the contact report.
    \default false
    \see NX_NOTIFY_FORCES
    \example
    String variations => +forces
    */
    bool mForces;

    /** \brief  Pair callback will be called when the contact force between two actors exceeds one of the actor-defined force thresholds.
    \default false
    \see NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD
    \example
    String variations => +start-force-threshold or +sft
    */
    bool mStartForceThreshold;

    /** \brief  Pair callback will be called when the contact force between two actors falls below the actor-defined force thresholds.
    \default false
    \see NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD
    \example
    String variations => +end-force-threshold or +eft
    */
    bool mEndForceThreshold;

    /** \brief  Pair callback will keep getting called while the contact force between two actors exceeds one of the actor-defined force thresholds.
    \default false
    \see NX_NOTIFY_ON_TOUCH_FORCE_THRESHOLD
    \example
    String variations => +touch-force-threshold or +tft
    */
    bool mTouchForceThreshold;

    /** \brief  Generate a callback for all associated contact constraints, making it possible to edit the constraint. 
    This flag is not included in All() for performance reasons.
    \default false
    \see NX_NOTIFY_CONTACT_MODIFICATION
    \example
    String variations => +contact-modification or +cm
    */
    bool mContactModification;

  protected:

    /** \brief Set the flags to their default state, according to the NxBodyDesc::setToDefault method.
    */
    void  setToDefault();

    /** \internal
    */
    void  fromNxU32(NxU32);

    /** \internal
    */
    NxU32 toNxU32() const;

    /** \internal
    */
    void  parse(const NxString&);

  };


  /** \brief  Make the actor static (never move and no body)
  \default false
  \see mMass, mDensity and the Body concept in PhysX.
  \example
  C++     ->  mActorParams.static(true)
  String  ->  "static: yes";
  \note Using "Static(false)" is completely pointless.
  */
  void Static(bool);

#if NX_SDK_VERSION_NUMBER >= 260

  /** \brief Compartment.
  \default NULL
  \example
  C++     ->  mActorParams.mCompartment = CompartmentPtr;
  String  ->  N/A
  \see NxActorDesc::compartment for more information.
  */
  NxCompartment*  mCompartment;

#endif

  /** \brief Density (mass calculated by volume).
  \default 0
  \example
  C++     ->  mActorParams.density = 10.0f;
  String  ->  "density: 10"
  \note
  Due how PhysX works then the mass of the Actor must be zero, else
  it uses Mass instead. If you wish to make the Actor static then both
  the density and the mass of the actor must be zero, or if you use
  string params then "static: yes" is equilvent to "mass: 0, density: 0".
  \see NxActorDesc::density for more information.
  \see ActorParams::mMass
  */
  NxReal  mDensity;

#if NX_SDK_VERSION_NUMBER >= 272 

  /** \brief DominanceGroup (as Name).
  \default 0
  \example
  C++     ->  mActorParams.mDominanceGroup = 1 or mActorPArams.mDominanceGroup = "blue";
  String  ->  "dominance-group: 1" or "dominance-group: blue"
  \see NxActorDesc::compartment for more information.
  */
  DualIdentifier<NxDominanceGroup, NxString> mDominanceGroup;

#endif

  /** \brief Flags of the Actor.
  \default 0
  \example
  C++     ->  mActorParams.mActorFlags = NX_AF_DISABLE_COLLISION | NX_AF_DISABLE_RESPONSE;
  String  ->  N/A
  \see ActorFlags
  */
  ActorFlags  mActorFlags;

  /** \brief ActorGroup this Actor belongs to.
  \default 0 or "default"
  \example
  C++     ->  mActorParams.mGroup = 1 or mActorParams.mGroup = "red"
  String  ->  "group: 1" or "group: red"
  \see ActorGroup
  */
  GroupIdentifier mGroup;

  /** \brief Angular Damping.
  \default 0.05f;
  \example
  C++     ->  mActorParams.mAngularDamping = 1.0f;
  String  ->  "angular-damping: 1"
  \see NxActorDesc::angular for more information.
  */
  NxReal  mAngularDamping;

  /** \brief The initial angular velocity of the actor.
  \default NxVec3Zero
  \example
  C++     ->  mActorParams.mAngularVelocity.set(1.0f,1.0f,1.0f)
  String  ->  "angular-velocity: 1 1 1"
  \see ActorParams::mLinearVelocity
  */
  NxVec3  mAngularVelocity;

  /** \brief CCD Motion Threshold.
  \default 0.0f
  \example
  C++     ->  mActorParams.mCCDMotionThreshold = ?
  String  ->  "ccd-motion-threshold: ?"
  */
  NxReal  mCCDMotionThreshold;

  /** \brief Initial flags of the body (assuming the Actor is not static).
  \default +debug-visualisation
  \examples
  C++     ->  mActorParams.mBodyFlags.mKinematic = true;
  String  ->  "body-flags: +kinematic"
  */
  BodyFlags   mBodyFlags;


  /** \brief Initial flags for reporting contact events.
  \default (none) - I think.
  \examples
  C++     ->  mActorParams.mBodyFlags.mKinematic = true;
  String  ->  "contact-flags: +kinematic"
  */
  ContactFlags   mContactFlags;


  /** \brief The force threshold for contact reports.
  \default NX_MAX_REAL
  \examples
  C++     ->  mSceneParams.mContactReportThreshold = 4.0f;
  String  ->  "contact-report-threshold: 4.0"
  */
  NxReal  mContactReportThreshold;

  /** \brief Initial flags of the body (assuming the Actor is not static).
  \default none.
  \examples
  C++     ->  mSceneParams.mFreeze.mPosition.y = true;
  String  ->  "freeze: +px +pz +rx +ry" or "freeze: rotation", or "freeze: position", or "freeze: all"
  */
  FreezeFlags   mFreeze;

  /** \brief Force Field Material Index.
  \default 0
  \examples
  C++     ->  mSceneParams.mForceFieldMaterial = 3 or mSceneParams.mForceFieldMaterial = "green";
  String  ->  "force-field-material: 3" or "force-field-material: green"
  */
  DualIdentifier<NxU16, NxString>  mForceFieldMaterial;

  /** \brief The number of SDK managed worker threads used when running 
  the simulation in parallel.
  \default 0
  \examples
  C++     ->  mSceneParams.mInternalThreadCount = 0
  String  ->  "internal-thread-count: 0"
  */
  NxU32   mInternalThreadCount;

  /** \brief Linear Damping of the Actor
  \default 0.0f
  \examples
  C++     ->  mSceneParams.mLinearDamping = 0.1f
  String  ->  "linear-damping: 0.1"
  */
  NxReal  mLinearDamping;

  /** \brief Initial linear velocity of the actor
  \default NxVec3Zero
  \examples
  C++     ->  mSceneParams.mLinearVelocity.set(0, 100, 0)
  String  ->  "linear-velocity: 0 100 0"
  */
  NxVec3  mLinearVelocity;

  /** \brief Mass of the Actor.
  \default 1.0
  \examples
  C++     ->  mSceneParams.mMass = 25
  String  ->  "linear-mass: 25"
  \note When adjusting the mass; the density of the Actor is
  automatically set to zero. To make the actor "static"
  you can use the shortcut param "static: yes", to set
  the mass and density to 0.
  */
  NxReal  mMass;

  /** \brief Mass Local Pose of the body portion of the Actor
  \default NxVec3Zero
  \examples
  C++     ->  mSceneParams.mMassLocalPose.t.set(0, -3, 0);
  */
  NxMat34  mMassLocalPose;

  /** \brief Diagonal mass space inertia tensor in bodies mass frame.
  \default 0 0 0
  \examples
  C++     ->  mSceneParams.mMassSpaceInertia.set(0,-3,0)
  String  ->  "mass-space-inertia: 0 -3 0"
  \note Leave it as NxVec3Zero to let the SDK calculate it.
  */
  NxVec3  mMassSpaceInertia;

  /** \brief Maximum angular velocity of the Actor
  \default -1.0f (PhysX assumes this to be NX_MAX_ANGULAR_VELOCITY)
  \see NxActorDesc::maxAngularVelocity
  \examples
  C++     ->  mSceneParams.mMaxAngularVelocity = 7.5f;
  String  ->  "max-angular-velocity: 7.5"
  */
  NxReal  mMaxAngularVelocity;

  /** \brief Maximum angular velocity at which body can go to sleep.
  \default -1.0f (the Global default)
  \see NxActorDesc::sleepAngularVelocity
  \examples
  C++     ->  mSceneParams.mSleepAngularVelocity = 2.5f;
  String  ->  "sleep-angular-velocity: 2.5"
  */
  NxReal  mSleepAngularVelocity;

  /** \brief Damping factor for bodies that are about to sleep.
  \default 0
  \see NxActorDesc::sleepDamping
  \examples
  C++     ->  mSceneParams.mSleepDamping = 1.5f;
  String  ->  "sleep-damping: 1.5"
  */
  NxReal  mSleepDamping;

  /** \brief Threshold for the energy-based sleeping algorithm.
  \default 0.005
  \see NxActorDesc::sleepEnergyThreshold
  \examples
  C++     ->  mSceneParams.mSleepEnergyThreshold = 0.1f;
  String  ->  "sleep-energy-threshold: 0.1"
  \note Only used when the mBodyFlags.mEnergySleepTest flag is set.
  */
  NxReal  mSleepEnergyThreshold;

  /** \brief Maximum linear velocity at which body can go to sleep.
  \default -1.0f (the Global default)
  \see NxActorDesc::sleepLinearVelocity
  \examples
  C++     ->  mSceneParams.mMaxLinearVelocity = 2.5f;
  String  ->  "sleep-linear-velocity: 2.5"
  */
  NxReal  mSleepLinearVelocity;

  /** \brief Number of solver iterations performed when processing joint/contacts connected to this body.
  \default 4
  \see NxActorDesc::solverIterationCount
  \examples
  C++     ->  mSceneParams.mSolverIterationCount = 6;
  String  ->  "solver-iteration-count: 6"
  */
  NxU32   mSolverIterationCount;

  /** \brief The body's initial wake up counter.
  \default 0.02
  \see NxActorDesc::wakeUpCounter
  \examples
  C++     ->  mSceneParams.mWakeUpCounter = 0.05;
  String  ->  "wake-up-counter: 0.05"
  */
  NxReal  mWakeUpCounter;

protected:

  /** \brief Set's the Params based from an NxActorDesc
  */
  void  fromNxActorDesc(NxActorDesc&);

  /** \brief Set's the Params based from a NxBodyDesc
  */
  void  fromNxBodyDesc(NxBodyDesc&);

  /** \internal
  */
  void  parse(Parameters);

}; // End of ActorParams class

/** \brief An Actor is a single unit of matter in the Scene.
It is physically represented by a "Collision Model" or a shape. If the actor
has some mass then it is known as a Dynamic Actor, capable of scurrying around
in the Scene. If the Actor has no mass then it is a static actor which never
ever moves.

\note
By default the Actor class has no form of visualisation it is completely
invisible. If your looking for a Actor class that does require visualisation
then you can use the Body class, or your own.

*/
class NxPublicClass Actor {

public:

  /** \brief Main actor constructor. 
  \note  For many purposes, Actors must be created through its parent's
  createActor function.
  \param Identifier     Unique identifier of the Actor.
  \param Scene          Parent scene to use.
  \param Shape          Shape/Collision Model of the actor
  \param Pose           First pose of the Actor
  \param ActorParams    Additional creation information wrapped in a ActorParams class.
  */
  Actor(const NxString& Identifier, Scene*, Shape*, const Pose&, const ActorParams& = ActorParams());

  /** \brief Pretty much obvious Actor destructor
  */
  virtual ~Actor();

protected:

  /** \brief Constructor that must be used for inherited actors only.
  */
  Actor(const NxString& Identifier, Scene*, bool isActorBased = true);

public:

  /** \brief A list of Shapes used in an Actor
  */
  typedef Betajaen::SharedList<Shape>		CollisionModel;

public:

  Scene* getScene()  {
    return mOwner;
  }

  /** \brief Get's a copy of the NxScene pointer that Scene represents.
  */
  NxScene* getNxScene();

  /** \brief Unique Hash of this class as a string (using getStringType)
  */
  virtual NxShortHashIdentifier getType() const {
    return NxOgreClass_Actor; /* "NxOgre-Actor" */
  }

  /** \brief Unique class identifier.
  */
  virtual NxString getStringType() const {
    return "NxOgre-Actor";
  }

  /** \brief Is the actor dead, usually if the NxActor is missing or null. But this 
  "deadness" is can be specified by the inherited actor.
  */
  virtual bool isDead() const {
    return (mActor == NULL);
  }

  /** \brief Duplicate this actor
  \param	name	Name of the new Actor
  \param	pose	Position/Orientation of the new actor.
  \note
  This function is rather slow and inefficent when used repeatedly. Use a
  ActorBlueprint instead to keep a single blueprint of the actor and create
  as many as you want from that.
  */
  Actor* duplicate(const NxString& name, const Pose& p);


  /** \internal
  \brief Creates the NxActor. Must be used by inherited actors in their constructor
  or create method.
  */
  void _createActor(Shape*, const Pose&,ActorParams params);

  /** \internal
  \brief Destroys the NxActor. Must be used by interited actors in their destructor
  or destroy method.
  */
  void _destroyActor();

  /** \brief Get's a copy of the NxActor which this Actor is representing.
  */
  NxActor* getNxActor() {
    return mActor;
  }

  /** \brief Renames this Actor to something else, it also updates the container
  in Scene parent mScene container.
  */
  void setName(NxString);

  /** \brief Get's the name of this Actor
  */
  NxString getName() const;

  /** \brief Set's the pose of the actor in the global frame.
  */
  void setGlobalPose(const Pose&);

  /** \brief Get's the pose fo the actor from the global frame.
  */
  Pose getGlobalPose() const;

#if (NX_USE_OGRE == 1)
  /** \brief Get's the orientiation only of this actor as a Ogre quaternion.
  \note Ogre::Quaternion's w, x, y, z represent w, x, y, z in order given.
  */
  Ogre::Quaternion  getGlobalOrientationAsOgreQuaternion() const;

  /** \brief Set's the orientation of this actor from a Ogre quaternion.
  */
  void setGlobalOrientation(const Ogre::Quaternion&);

  /** \brief Get's the global position only of this actor as a Ogre Vector3
  */
  Ogre::Vector3  getGlobalPositionAsOgreVector3() const;

  /** \brief Set's the global position only of this actor from a Ogre Vector3
  */
  void setGlobalPosition(const Ogre::Vector3&);
#endif

  /** \brief Get's the orientation only of this actor as a 3x3 matrix placed
  into a NxMat33 class.
  */
  NxMat33 getGlobalOrientation() const;

  /** \brief Set's the orientation only of this actor as a 3x3 matrix placed
  into a NxMat33 class.
  */
  void setGlobalOrientation(const NxMat33& matrix);

  /** \brief Get's the orientation only of this actor as a quaternion placed
  inside a float4 vector.

  \note float4's i, j, k, l represent w, x, y, z in order given.
  */
  float4 getGlobalOrientationAsFloat4() const;

  /** \brief Set's the orientation only of this actor as a quaternion placed
  inside a float4 vector.

  \note float4's i, j, k, l represent w, x, y, z in order given.
  */
  void setGlobalOrientation(const float4& quaternion);

  /** \brief Get's the global position only of this Actor as a float3
  \note  float3's i, j, k represent x, y, z in order given.
  */
  float3 getGlobalPositionAsFloat3() const;

  /** \brief Set's the global position only of this Actor as a float3
  \note  float3's i, j, k represent x, y, z in order given.
  */
  void setGlobalPosition(const float3& vector);


  /** \brief Get's the orientation only of this actor as a quaternion placed
  inside a NxQuat.
  \note NxQuat's w, x, y, z represent w, x, y, z in order given.
  */
  NxQuat getGlobalOrientationAsNxQuat() const;


  /** \brief Set's the orientation only of this actor as a quaternion placed
  inside a NxQuat.
  \note NxQuat's w, x, y, z represent w, x, y, z in order given.
  */
  void setGlobalOrientation(const NxQuat& quaternion);

  /** \brief Get's the global position only of this Actor as a NxVec3
  */
  NxVec3 getGlobalPositionAsNxVec3() const;

  /** \brief Set's the global position only of this Actor as a float3
  */
  void setGlobalPosition(const NxVec3& vector);

  /** \brief 
  */
  void moveGlobalPose(const Pose&);

  /** \brief
  */
  void moveGlobalPosition(const Ogre::Vector3&);

  /** \brief
  */
  void moveGlobalOrientation(const Ogre::Quaternion&);

  /** \brief Calculates a rough force and applies it to this actor towards a 
  specific point in global space.
  \param Pose  The point in space to move towards too.
  \param Force The "oomph" to move it.
  */
  void moveTowards(const Pose&, float force = 10.0f);


  /** \brief Add a shape to this Actor.
  */
  Shape* addShape(Shape*);

  /** \brief Remove a shape from this Actor.
  \note Removing the last shape from this actor will cause PhysX to go boom.
  */
  bool removeShape(Shape*);

  /** \brief Remove a shape from this Actor.
  \note Removing the last shape from this actor will cause PhysX to go boom.
  */
  bool removeShape(NxShapeIndex);

  /** \brief Returns the number of shapes used with this Actor
  */
  NxU32 getNbShapes() const;

  /** \brief Get's a copy of the container of the pointer of Shapes that are
  used with this Actor
  */
  CollisionModel getCollisionModel();

  /** \brief Set's this Actor to another ActorGroup.
  */
  void setGroup(ActorGroup*);

  /** \brief Set's this Actor to another ActorGroup.
  */
  void setGroup(const NxString& GroupIdentifier);


  /** \brief Set's this Actor to another ActorGroup.
  */
  void setGroup(NxActorGroup);

  /** \brief Get's a copy of the ActorGroup that this Actor is under.
  */
  ActorGroup* getGroup() const;

  /** \brief
  */
  void raiseActorFlag(NxActorFlag);

  /** \brief
  */
  void clearActorFlag(NxActorFlag);

  /** \brief
  */
  bool hasActorFlag(NxActorFlag) const;

  /** \brief Does the Actor have any mass, hence movable and reacts to forces.
  */
  bool isDynamic() const;

  void setCMassOffsetLocalPose(const Pose&);
  void setCMassOffsetLocalPosition(const Ogre::Vector3&);
  void setCMassOffsetLocalOrientation(const NxMat33&);
  void setCMassOffsetLocalOrientation(const Ogre::Matrix3&);
  void setCMassOffsetGlobalPose(const Pose&);
  void setCMassOffsetGlobalPosition(const Ogre::Vector3&);
  void setCMassOffsetGlobalOrientation(const NxMat33&);
  void setCMassOffsetGlobalOrientation(const Ogre::Matrix3&);
  void setCMassGlobalPose(const Pose&);
  void setCMassGlobalPosition(const Ogre::Vector3&);
  void setCMassGlobalOrientation(const Ogre::Quaternion&);

  Pose							getCMassLocalPose()	const;
  Ogre::Vector3 					getCMassLocalPosition()	const;
  Ogre::Quaternion 				getCMassLocalOrientation() const;
  Pose							getCMassGlobalPose() const;
  Ogre::Vector3 					getCMassGlobalPosition() const;
  Ogre::Quaternion				getCMassGlobalOrientation()	const;

  //////////////////////////////////////////////////////////

  void							setMass(NxReal);
  NxReal							getMass() const;

  void							setMassSpaceInertiaTensor(const Ogre::Vector3&);
  Ogre::Vector3					getMassSpaceInertiaTensor()	const;
  Ogre::Matrix3					getGlobalInertiaTensor() const;
  Ogre::Matrix3					getGlobalInertiaTensorInverse()	const;

  void							updateMassFromShapeFromDensity(NxReal);
  void							updateMassFromShapeAsMass(NxReal);

  //////////////////////////////////////////////////////////

  void							setLinearDamping(NxReal);
  NxReal							getLinearDamping() const;

  void							setAngularDamping(NxReal);
  NxReal							getAngularDamping() const;

  //////////////////////////////////////////////////////////

  void							setLinearVelocity(const Ogre::Vector3&);
  void							setAngularVelocity(const Ogre::Vector3&);

  Ogre::Vector3					getLinearVelocity()	const;
  Ogre::Vector3					getAngularVelocity() const;

  void							setMaxAngularVelocity(NxReal);
  NxReal							getMaxAngularVelocity()	const;


  //////////////////////////////////////////////////////////

  void							setCCDMotionThreshold(NxReal);
  NxReal							getCCDMotionThreshold()	const;

  //////////////////////////////////////////////////////////

  void							setLinearMomentum(const Ogre::Vector3&);
  void							setAngularMomentum(const Ogre::Vector3&);
  Ogre::Vector3					getLinearMomentum() const;
  Ogre::Vector3					getAngularMomentum() const;

  //////////////////////////////////////////////////////////

  void							addForceAtPos(const Ogre::Vector3& force, const Ogre::Vector3& pos, NxForceMode mode = NX_FORCE, bool wakeup = true);
  void							addForceAtLocalPos(const Ogre::Vector3& force, const Ogre::Vector3& pos, NxForceMode mode = NX_FORCE, bool wakeup = true);
  void							addLocalForceAtPos(const Ogre::Vector3& force, const Ogre::Vector3& pos, NxForceMode mode = NX_FORCE, bool wakeup = true);
  void							addLocalForceAtLocalPos(const Ogre::Vector3& force, const Ogre::Vector3& pos, NxForceMode mode = NX_FORCE, bool wakeup = true);


  void							addForce(const Ogre::Vector3& force, NxForceMode mode = NX_FORCE, bool wakeup = true);
  void							addForce(const NxVec3& force, NxForceMode = NX_FORCE, bool wakeup = true);
  void							addForce(NxReal x, NxReal y, NxReal z, NxForceMode = NX_FORCE, bool wakeup = true);
  void							addLocalForce(const Ogre::Vector3& force, NxForceMode mode = NX_FORCE, bool wakeup = true);

  void							addTorque(NxReal x, NxReal y, NxReal z, NxForceMode mode = NX_FORCE, bool wakeup = true) {
    addTorque(NxVec3(x,y,z), mode, wakeup);
  }
  void							addTorque(const NxVec3& torque, NxForceMode mode = NX_FORCE, bool wakeup = true); 
  void							addTorque(const Ogre::Vector3& torque, NxForceMode mode = NX_FORCE, bool wakeup = true);
  void							addLocalTorque(const Ogre::Vector3& torque, NxForceMode mode = NX_FORCE, bool wakeup = true);
  NxReal							computeKineticEnergy() const;

  //////////////////////////////////////////////////////////

  Ogre::Vector3					getPointVelocity(const Ogre::Vector3& point) const;
  Ogre::Vector3					getLocalPointVelocity(const Ogre::Vector3& point) const;

  //////////////////////////////////////////////////////////

  bool							isGroupSleeping() const;
  bool							isSleeping() const;
  NxReal							getSleepLinearVelocity() const;
  void							setSleepLinearVelocity(NxReal threshold);
  NxReal							getSleepAngularVelocity() const;
  void							setSleepAngularVelocity(NxReal threshold);
  NxReal							getSleepEnergyThreshold() const;
  void							setSleepEnergyThreshold(NxReal threshold);
  void							wakeUp(NxReal wakeCounterValue=NX_SLEEP_INTERVAL);
  void							putToSleep();

  //////////////////////////////////////////////////////////

  void							raiseBodyFlag(NxBodyFlag);
  void							clearBodyFlag(NxBodyFlag);
  bool							hasBodyFlag(NxBodyFlag) const;
  void							setSolverIterationCount(NxU32);
  NxU32							getSolverIterationCount() const;

  //////////////////////////////////////////////////////////

#if NX_SUPPORT_SWEEP_API
  NxU32							linearSweep(const Ogre::Vector3& motion, NxU32 flags, void* VoidPointer, NxU32 nbShapeDescriptions, NxSweepQueryHit* ShapeDescriptions, NxUserEntityReport<NxSweepQueryHit>* callback, const NxSweepCache* sweepCache=NULL);
#endif

  //////////////////////////////////////////////////////////
#if NX_SDK_VERSION_NUMBER >= 260
  Compartment*					getCompartment() const;
#endif
  //////////////////////////////////////////////////////////

  CollisionModel					mCollisionModel;

  VoidPointer*						       	getVoidPointer();
  void*						getUserData();

  /** joinWith

  Creates a fixedJoint with this actor and the referenced actor.

  */
  FixedJoint*						joinWith(Actor*, JointParams);

  /** Disable

  Nearly kills the actor in the Scene. Disables collision, movement,
  rotation, reporting. If the Actor is subclassed the visualisation
  will be removed (or hidden). This is best used with a batch
  delete of Actors (just before a batch deletion is done) to hide
  the Actor from the screen, and just before it is deleted.

  If you want to undisable this actor, you will need to loose the 
  NX_AF_DISABLE_COLLISION, NX_AF_COLLISION_RESPONSE, NX_BF_FROZEN,
  flags. See the code in disable for more details.

  */
  void							disable();
  NxU32							getBirthFrame() {return mBirthFrame;}

  //////////////////////////////////////////////////////////

  virtual StringPairList			saveCustom();
  virtual void					restoreCustom(StringPairList)	{}

protected:

  Scene						   *mOwner;
  NxActor*						mActor;
  NxString						mName;
  VoidPointer*					mVoidPointer;
  NxU32							mBirthFrame;

private:


};// End of Actor Class


/**

\page customactors Custom Actors

Using your own Actor class with NxOgre:

The Header of "IActor", the inherited Actor.

<code>
class NxPublicClass IActor : public Actor {

public:

// Constructors similar to. Used for creating a NxActor by hand through a blueprint,
// or the NxuStream tool
IActor(const NxString& name, Scene*);

// Constructors similar to. Normal Everyday Constructor.
// Name, Scene, ShapeBlueprint, Pose, ActorParams are required by Actor, so they
// must be part of your constructor.
IActor(const NxString& name, Scene*, ShapeBlueprint*, const Pose&, ActorParams = "");

// Destructor.
virtual ~IActor();

// Your IActor may have some visualisation.
virtual bool			hasVisualisation() const	{return true;}

// to call ShapeSimulate
void					simulate(float);

// to call ShapeRender, and to render your visualisation.
void					render(float);

// Save any custom data related to your Inherited Actor to a StringPairList
StringPairList			saveCustom();

// Restore any custom data to your Inherited Actor.
void					restoreCustom(StringPairList);

// Disable Visualisation, for when it is "disabled". This hides things, it does not delete things.
void					disableVisualisation();

};
</code>


The code side of the IActor:

<code>
IActor::IActor(const NxString& name, Scene* scene) : Actor(name, scene) {
// Do nothing. Not even your own visualisation. RestoreCustom will kick in soon to handle that.
}

IActor::IActor(const NxString& identifier, Scene* scene, ShapeBlueprint *firstShapeDescription, const Pose& pose, ActorParams params)
: Actor(identifier, scene, firstShapeDescription, pose, params) {
// Identifier, Scene, firstShapeDescription,  pose, params are all required by Actor. 
// So they should be part of your constructor.

// Setup visualisation stuff here.
//  You can access the SceneManager from mOwner->getSceneMgr()
}

IActor::~IActor {
// Clean up your visualisation here. The destructor in Actor will clean up the NxActor stuff.
}


void IActor::simulate(float dT) {
// Required...
if (mActor) {
shapeSimulate(dT);
}
}

void IActor::render(float dT) {
// You can get the Position and Orientation of the NxActor using:
//	getGlobalPosition();
//  getGlobalOrientation();
//
// If you have NX_UNSTABLE_USE_SCENE_ACTIVE_TRANSFORM on, render will only be called
// if the NxActor has moved.
}


StringPairList IActor::saveCustom() {

// Required...
StringPairList l;
l.insert("ActorType", "IActor"); // IActor or the name of your class; *Not* Actor or Body.
l.insert("Key", "Value");
// You can have multiple keys with the same name.
}

void IActor::restoreCustom(StringPairList spl) {

for (StringPairList::StringPair sp = spl.begin();spl.hasNext();) {
sp = spl.next();

NxString key = sp.first;
NxString value = sp.second;
Ogre::StringUtil::toLowerCase(key);

if (key == "x") {
mY = value;
}
}

void IActor::disableVisualisation() {
if (mNode->getParent() != NULL) {
mNode->getParent()->removeChild(mNode);
}
}

</code>

/////////////////////////////////////////////////////////////////

Using a NxOgre Container to store your IActor.

<code>
class SomeGameManagerClass {

typedef NxOgre::Container<NxString, IActor*> IActors;

IActors mActors;

};
</code>

/////////////////////////////////////////////////////////////////

Creating an IActor and storing the pointer.

<code>
IActor* iactor = new IActor("name", mScene, new CubeShape(1), Vector3::ZERO, "mass: 10");
mIActors.insert(iactor->getName(), iactor);
// A copy of the IActor has been placed in mScene::mActors. NxOgre does not "own" the class
// we do. So we are responsible for deleting it.

// To own the IActor, we must "lock" it.
IActors.lock(iactor->getName(), true);
</code>

//////////////////////////////////////////////////////////////////

When the time is right, we must delete the IActor before the destruction of the Scene.

Either:
<code>
mIActors.destroyAllOwned();
mWorld->destroyScene(mScene->getName());
</code>		
or:
<code>
mIActors.destroyAllOwned();
delete mWorld;
</code>
//////////////////////////////////////////////////////////////////

*/

};// End of namespace

#endif
