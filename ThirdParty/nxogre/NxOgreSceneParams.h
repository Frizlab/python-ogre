/** \file   NxOgreSceneParams.h
*  \brief   Header for the Scene and SceneParams classes.
*  \version 1.0-22T4
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

#ifndef __NXOGRE_SCENE_PARAMS_H__
#define __NXOGRE_SCENE_PARAMS_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreParams.h"

namespace NxOgre
{

/** \brief A collection of optional things that are passed around when the Scene is constructed.
    \example
      <code>
      "gravity: yes"
      </code>
    \see \ref sceneparams for the full string argument documentation.
*/
class NxPublicClass SceneParams : public Params
{

public:

  /** \brief SceneParams constructor for "class params".
  */
  SceneParams();

  /** \brief SceneParams constructor for "string params".
  \param _params Full string of params.
  */
  SceneParams(const char* _params);

  /** \brief SceneParams constructor for "string params" using an NxString.
  \param _params Full string of params.
  */
  SceneParams(const NxString& _params);

  /** \brief Set's the member variables to the default values
  according to the NxSceneDesc class and NxOgre's
  definition of default.
  */
  void  setToDefault();

  /** \brief
  */
  void  toNxSceneDescription(NxSceneDesc&);

  /** \brief
  */
  void  fromNxSceneDescription(NxSceneDesc&);

public:

  /** \brief An infinite ground plane on the +Y axis.
  \default false
  \example
  C++     ->  mSceneParams.mFloor = true;
  String  ->  "floor: yes"
  */
  bool   mFloor;


  /** \brief The constant acceleration of gravity (in metres per second)
  \default  NxVec3.zero()
  \example
  C++     ->  mSceneParams.mGravity.set(0, -9.8, 0);
  String  ->  "gravity: yes" <em>When set to yes. 0 -9.80665 0 is used.</em>
  String  ->  "gravity: 0 -9.8 0"
  */
  NxVec3   mGravity;


  /** \brief The default restitution and friction of the default material (0).
  \default  mRestitution 0, mStaticFriction 0.5 and mDynamicFriction 0.5.
  \examples
  C++     ->  mSceneParams.mDefaultMaterial.mRestitution = 0.7f
  String  ->  "material-restitution: 0.5"
  String  ->  "material-dynamic-friction: 1.0"
  String  ->  "material-static-friction: 0.1"
  */
  struct  SimpleMaterialParams {
    NxReal  mRestitution;
    NxReal  mStaticFriction;
    NxReal  mDynamicFriction;
    void toDefault() {
      mRestitution = 0;
      mDynamicFriction = 0.5f;
      mStaticFriction = 0.5f;
    }
  }   mDefaultMaterial;


  /** \brief Maximum substep size. 
  \default 1.0/60.0
  \examples
  C++     ->  mSceneParams.mMaxTimeStep = 1.0/80.0
  String  ->  "max-timestep: 0.0125"
  */
  NxReal   mMaxTimestep;


  /** \brief Maximum number of substeps to take. 
  \default 8
  \examples
  C++     ->  mSceneParams.mMaxIterator = 10
  String  ->  "max-iterator: 10"
  */
  NxU32   mMaxIterator;


  /** \brief Max scene bounds. 
  \default NULL
  \examples
  C++     ->  mSceneParams.mMaxBounds = bounds
  */
  NxBounds3*   mMaxBounds;


  /** \brief Expected scene limits
  \default NULL
  \examples
  C++     ->  mSceneParams.mLimits = limits
  */
  NxSceneLimits*   mLimits;


  /** \brief Is the simulation on a CPU (Software) or on a PPU (Hardware)
  \default NX_SIMULATION_SW
  \examples
  C++     ->  mSceneParams.mSimulationType = NX_SIMULATION_HW
  String  ->  "simulation-type: hardware"
  */
  NxU32   mSimulationType;

  struct NxPublicClass SceneFlags : Flags {

    void toDefault() {
      mDisableSSE              = false;
      mDisableCollisions       = false;
      mSimulateSeperateThread  = true;
      mEnableMultithread       = false;
      mEnableActivetransforms  = false;
      mRestrictedScene         = false;
      mDisableSceneMutex       = true;
      mForceConeFriction       = false;
      mSequentialPrimary       = false;
      mFluidPerformanceHint    = false;
    }

    void  fromNxU32(NxU32);
    NxU32 toNxU32() const;
    void  parse(const NxString&);

    /** \brief Used to disable use of SSE in the solver.
    \default false
    \see NX_SF_DISABLE_SSE
    \example
    String variations => +disable-sse, +no-sse
    */
    bool mDisableSSE;

    /** \brief Disable all collisions in a scene.
    \default false
    \see NX_SF_DISABLE_COLLISIONS
    \example
    String variations => +disable-collisions, +no-collisions
    */
    bool mDisableCollisions;

    /** \brief Perform the simulation in a separate thread.
    \default true
    \example
    String variations => +simulate-seperate-thread, +sst
    */
    bool mSimulateSeperateThread;

    /** \brief Enable internal multi threading.
    \default false
    \example
    String variations => +enable-multithread, +multithread
    */
    bool mEnableMultithread;

    /** \brief Enable Active Transform Notification.
    \default false
    \example
    String variations => +enable-activetransforms, +activetransforms
    */
    bool mEnableActivetransforms;

    /** \brief Enable Restricted Scene.
    \default false
    \see NX_SF_RESTRICTED_SCENE
    \example
    String variations => +restricted-scene, +xxx-scene
    */
    bool mRestrictedScene;

    /** \brief Disable the mutex which serializes scene execution.
    \default true
    \see NX_SF_DISABLE_SCENE_MUTEX
    \example
    String variations => +disable-scene-mutex, +no-mutex
    */
    bool mDisableSceneMutex;

    /** \brief Force the friction model to cone friction.
    \default false
    \see NX_SF_FORCE_CONE_FRICTION
    \example
    String variations => +force-cone-friction, +fcf
    */
    bool mForceConeFriction;

    /** \brief Compartments are all executed before the primary scene is executed.
    \default false
    \see NX_SF_SEQUENTIAL_PRIMARY
    \example
    String variations => +sequential-primary, +sp
    */
    bool mSequentialPrimary;

    /** \brief Enables faster but less accurate fluid collision with static geometry.
    \default false
    \see FLUID_PERFORMANCE_HINT
    \example
    String variations => +fluid-performance, +fph
    */
    bool mFluidPerformanceHint;

  };

  /** \brief Initial flags of the scene
  \default +seperate-scene-thread +disable-scene-mutex
  \examples
  C++     ->  mSceneParams.mSceneFlags.mDisableSSE = true;
  String  ->  "scene-flags: +disable-sse +disable-scene-mutex"
  */
  SceneFlags   mSceneFlags;


  /** \brief The number of SDK managed worker threads used when running 
  the simulation in parallel.
  \default 0
  \examples
  C++     ->  mSceneParams.mInternalThreadCount = 0
  String  ->  "internal-thread-count: 0"
  */
  NxU32   mInternalThreadCount;


  /** \brief The number of SDK managed threads which will be processing
  background tasks.
  \default 0
  \examples
  C++     ->  mSceneParams.mBackgroundThreadCount = 0
  String  ->  "background-thread-count: 0"
  */
  NxU32   mBackgroundThreadCount;


  /** \brief A custom scheduler to completely take over the allocation
  of work items among threads.
  \default NULL
  \examples
  C++     ->  mSceneParams.mCustomScheduler = scheduler_ptr;
  */
  NxUserScheduler*   mCustomScheduler;


  /** \brief Thread Mask
  \default 0
  \examples
  C++     ->  mSceneParams.mThreadMask = 0x55555554;
  String  ->  "thread-mask: 1431655764"
  */
  NxU32   mThreadMask;


  /** \brief Background thread mask
  \default 0
  \examples
  C++     ->  mSceneParams.mThreadMask = 0x55555554;
  String  ->  "background-thread-mask: 1431655764"
  */
  NxU32   mBackgroundThreadMask;


  /** \brief Simulation thread stack size
  \default 0 (OS Default)
  \examples
  C++     ->  mSceneParams.mSimThreadStackSize = 8196
  String  ->  "sim-thread-stack-size: 8196"
  */
  NxU32   mSimThreadStackSize;


  /** \brief Simulation thread priority
  \default NX_TP_NORMAL
  \examples
  C++     ->  mSceneParams.mThreadPriority = NX_TP_NORMAL
  String  ->  "sim-thread-priority: normal", "sim-thread-priority: above-normal"
  */
  NxThreadPriority   mSimThreadPriority;


  /** \brief Worker thread stack size
  \default 0 (OS Default)
  \examples
  C++     ->  mSceneParams.mSimThreadStackSize = 8196
  String  ->  "worker-thread-stack-size: 8196"
  */
  NxU32   mWorkerThreadStackSize;


  /** \brief Worker thread priority
  \default NX_TP_NORMAL
  \examples
  C++     ->  mSceneParams.mWorkedThreadPriority = NX_TP_NORMAL
  String  ->  "sim-thread-priority: normal", "sim-thread-priority: above-normal"
  */
  NxThreadPriority   mWorkerThreadPriority;


  /** \brief What is up? Accelerates some scene queries.
  \default 0
  \note 0 = Don't use, 1 = Y or 2 = Z (X cannot be used).
  \examples
  C++     ->  mSceneParams.mUp = 1
  String  ->  "up: y", "up: none"
  */
  NxU32   mUp;


  /** \brief Subdivision level
  \default 5
  \examples
  C++     ->  mSceneParams.mSubdivisionLevel = 6
  String  ->  "subdivision-level: 6"
  */
  NxU32   mSubdivisionLevel;


  /** \brief Static structure
  \default  NX_PRUNING_STATIC_AABB_TREE
  \examples
  C++     ->  mSceneParams.mStaticStructure = NX_PRUNING_STATIC_AABB_TREE
  String  ->  "static-structure: dynamic-aabb"
  { none, octree, quadtree, dynamic-aabb, static-aabb }
  */
  NxU32   mStaticStructure;


  /** \brief Dynamic structure
  \default NX_PRUNING_NONE
  \examples
  C++     ->  mSceneParams.mDynamicStructure = NX_PRUNING_STATIC_AABB_TREE
  String  ->  "dynamic-structure: dynamic-aabb"
  { none, octree, quadtree, dynamic-aabb, static-aabb }
  */
  NxU32   mDynamicStructure;


  enum SceneControllerType {
    CN_PTR,
    CN_FIXED,
    CN_VARIABLE,
    CN_ACCUMULATOR
  };

  /** \brief Scene Controller to use (Fixed, Variable, Accumulator or Null)
  \note If your using your own scenecontroller, supply a Ptr controller as a param first.
  then place the pointer of your controller in mControllerPtr
  \default CN_FIXED (Fixed timestep)
  \examples
  C++     ->  mSceneParams.mController = CN_VARIABLE;
  String  ->  "controller: accumulator"
  { null, fixed, variable, accumulator}
  */
  NxU32   mController;

  /** \brief Custom Scene Controller, used only when mController is CN_PTR
  \default NULL
  \examples
  C++     ->  mSceneParams.mControllerPtr = myControllerPtr;
  */
  SceneController*   mControllerPtr;


  enum SceneRendererType
  {
    RN_NULL,
    RN_PTR,
#if (NX_USE_OGRE == 1)
    RN_OGRE
#endif
  };

  /** \brief Scene Renderer to use.
  \note If your using your own scene renderer, supply RN_PTR renderer as a param first.
  \default RN_NULL
  \examples
  C++     ->  mSceneParams.mRenderer = RN_OGRE;
  String  ->  "renderer: ogre"
  {null, ogre}
  */
  NxU32  mRenderer;

  /** \brief Custom Scene Renderer, used only when mRenderer is CN_PTR
  \default NULL
  \examples
  C++     ->  mSceneParams.mRendererPtr = myRendererPtr;
  */
  SceneRenderer*  mRendererPtr;


  /** \brief Scene Renderer VoidPointer (RenderSystem specific)
  \examples
  C++     ->  mSceneParams.mRendererUserdata = "use-directx";
  String  ->  "renderer-VoidPointer: use-directx"
  */
  NxString  mRendererUserdata;

protected:

  void parse(Parameters);


};

}; // End of NxOgre namespace.

#endif
