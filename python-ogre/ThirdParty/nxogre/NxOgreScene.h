/** \file    NxOgreScene.h
*  \brief   Header for the Scene and SceneParams classes.
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

#ifndef __NXOGRE_SCENE_H__
#define __NXOGRE_SCENE_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreContainer.h"        // For: Scene::mActors
#include "NxOgreActor.h"            // For: ActorParams
#include "NxOgreJoint.h"            // For: JointParams
#include "NxOgreSceneParams.h"      // For: SceneParams
#include "NxOgrePose.h"
#include "NxOgreNodeRenderable.h"   // For: NodeRenderableParams (for Body)
#include "NxOgreSimpleActor.h"      // For: BetajaenCC SharedList<SimpleActor>
#include "NxOgreDeletionCallback.h" // For: BetajaenCC SharedList<DelayedActor>
#include "NxOgreTimeStep.h"         // For: TimeStep
#include "NxOgreRaycaster.h"        // For: Raycasters
#include "NxOgreRenderableActorFactory.h"

namespace NxOgre
{


/** \brief A Scene is a sub-section of a World. Although a Scene is infinite in size like World. Scenes are 
          considered to be seperate from each other. Actors from one scene cannot interact with ones from
          another. Scenes have also their own Materials, Groups, etc. which cannot be shared.

    \warning <strong>Containers and Garbage Collection</strong> \n
      Scenes are used in containers. Creating a Scene through the World factory method makes the Scene "owned"
      by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
      Manually creating the Scene "new Scene()" will only add a presence, it is up to you to delete it at the
      correct time.
*/
class NxPublicClass Scene : public NxUserNotify, public NxUserContactReport 
{

  friend class Actor;
  friend class ActorGroup;
  friend class ShapeGroup;

#if (NX_USE_CLOTH_API == 1)
  friend class Cloth;
#endif
  friend class CharacterSystem::Character;

#if (NX_SDK_VERSION_NUMBER >= 272)
  friend class DominanceGroup;
#endif

#if (NX_USE_FLUID_API == 1)
  friend class Fluid;
#endif
#if (NX_USE_FORCEFIELD_API == 1)
  friend class ForceField;
#endif
  friend class Material;
  friend class Trigger;
  friend class World;

  public:

    /** \brief Used to specify when an batched action will happen.
    */
    enum BatchActionTimeFrame {
      BT_NOW,	
      BT_NEXT_AVAILABLE_BATCH,
      BT_NEXT_SIMULATION,
      BT_WITHIN_SECONDS,
      BT_NEVER
    };

    /** \brief Renderable types
    */
    enum RenderableType {
      RT_EVERYTHING = 0,
      RT_MOVED      = 1,
      RT_TRANSFORM  = 2
    };

    /** \brief ActorTypes
    */
    enum ActorTypes
    {
      AT_StaticActors  = 0,
      AT_DynamicActors = 1,
      AT_AllActors     = 2
    };

    /** \brief Scene Constructor
        Create's a Scene, and NxScene withing physics. It also creates default material, actor groups, 
        shape groups and dominance groups. Based on the SceneParams, Gravity may be set, and an actor
        created with a GroundShape to represent the ground plane or the floor.

        \warning <strong>Containers and Garbage Collection</strong> \n
        Scenes are used in containers. Creating a Scene through the World factory method makes the Scene "owned"
        by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
        Manually creating the Scene "<code>new Scene()</code>" will only add a presence, it is up to you to delete it at the
        correct time.

        \param Identifier				Unique identifier of the Scene
        \param World*					World pointer which would contain the Scene.
        \param SceneParams				SceneParams for this Scene to use.
        \see World::createScene
        \see SceneParams
    */
    Scene(const NxString& identifier, World*, SceneParams = SceneParams());


    /** \brief Alternate Scene Constructor. Used for to bind a Scene to a previously made NxScene.

        \param Identifier             Unique identifier of the Scene                       
        \param Ogre::SceneManager*    SceneManger for this Scene to Use.                   
        \param World*                 World pointer which would contain the Scene.         
        \param NxScene*               NxScene to bind to.                                  

        \see World::createScene
        \see SceneParams
    */
    Scene(const NxString& identifier, World*, NxScene*);

    /** \brief Scene Destructor
        \note Containers and Garbage Collection
        If the Scene is owned by NxOgre use World::SceneDestroy, if not then use "delete mScene".
        \see World::destroyScene
    */
    ~Scene();

    /**  \brief Returns the name of the Scene.
         \return NxString Scene identifier
    */
    NxString  getName();

    /**  \brief Returns a copy of a pointer of the NxScene which the Scene works with.
         \return NxScene The NxScene's pointer
    */
    NxScene*  getNxScene();

    /** \brief Returns a copy of the World which the Scene is part of.
        \return World The World itself.
    */
    World*  getWorld();


    /**\brief Sets the Scene Controller for simulation/rendering control of the Scene.
              Sets the Controller used for Simulation and Rendering of the NxScene. These can be Variable, Fixed,
              or Accumulator. You can use your own SceneController if you wish, otherwise the SceneController should
              be specified in the SceneParams "time-step-method" with Fixed, Variable or Accumulator as an option.
       \warning <strong>Containers and Garbage Collection</strong> \n
       SceneController instance will be deleted on the destruction of the Scene.	
       \param SceneController Instance of the SceneController to use.
    */
    void  setSceneController(SceneController*);


    /** \brief Gets the Scene Controller which is controls of the Physics simulation.
    */
    SceneController*  getSceneController();


    /** \brief Gets the last known deltaTime injected into the controller.
    */
    NxReal  getLastDeltaTime() const;


    /** \brief Gets the last known alpha value of the controller.
    */
    NxReal  getLastAlphaValue() const;


    /** \brief Gets the Scene Renderer which renders the results of the Physics simulation
    */
    SceneRenderer*  getSceneRenderer();


    /** \brief Set's the Scene Renderer, and will delete the previous one
    */
    void  setSceneRenderer(SceneRenderer*);


    /** \brief Get a list of renderables based on time
    */
    Renderables	getRenderable(RenderableType);


    /**  \brief Returns the NxSimulationType (Hardware/Software) of the Scene.
    \returns NxSimulationType		Simulation Type
    */
    NxSimulationType  getSimType() const;


    /**  \brief Sets the gravitional constant for the Scene. 
    \param Ogre::Vector3(Acceleration/metres per second)	Gravitional Constant
    */
    void  setGravity(const Ogre::Vector3&);


    /**  \brief Gets the gravitional constant for the Scene. 
    \return Ogre::Vector3(Acceleration/metres per second)	Gravitional Constant
    */
    Ogre::Vector3  getGravity() const;


    /**\brief Creates an Actor within the Scene.

    \warning <strong>Containers and Garbage Collection</strong> \n
    Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
    by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
    Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
    correct time.

    \param identifier Unique name of the Actor. (Leave blank for a auto-generated identifier)
    \param Shape  The first shape to use for the Actor (new ShapeName(...))
    \param Pose  Global Pose of the Actor
    \param ActorParams  ActorParams for this actor

    \return The created Actor
    */
    Actor*  createActor(const NxString& identifier, Shape*, const Pose&, const ActorParams& = ActorParams());


    /**\brief Creates many actors within the Scene now or sometime in the future.

    \warning <strong>Containers and Garbage Collection</strong> \n
    Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
    by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
    Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
    correct time.

    \param identifierPattern		identifierPattern of the Actors; "Cake" -> "Cake, Cake1, Cake2"
    \param nbActors				Number of Actors to create
    \param ShapeBlueprint*		The first shape to use for the Actor (new ShapeName(...))
    \param Pose					Global Pose of the first actor
    \param Pose					Pose difference between the next actor
    \param BatchActionTimeFrame	When to create the Actors
    \param ActorParams			ActorParams for the actors
    */
    void  batchCreateActors(const NxString& identifierPattern, NxU32 nbActors, Shape*, const Pose& poseOrigin, const Pose& poseDifference, BatchActionTimeFrame = BT_NOW, ActorParams = ActorParams());


    /**\brief Destroys an Actor (or any inherited Actor) within the scene.

      \warning <strong>Containers and Garbage Collection</strong> \n       
      Actor are used in containers. Creating a Actor through the Scene factory
      method makes the Actor "owned" by NxOgre, on deletion of the Scene it
      will remove the presence of the Scene and delete the pointer.
      Manually creating the Scene "new Actor()" will only add a presence,
      it is up to you to delete it at the correct time.

      \param identifier Unique name of the Actor to destroy.
    */
    void  destroyActor(const NxString&);

    /**  \brief Destroy an Actor at the nearest best oppunitnity. Best used within callbacks when
    the state of the simulation cannot be changed. If there is no deletioncallback
    then the actor is deleted now.
    \param Actor Actor to eventually destroy.
    \param killActorFirst Set the actor to "dead"; non-movable, non-collidable, not effected by gravity
    */
    void  destroyActorDelayed(Actor*, bool killActorFirst = true);

    /** \brief Destroy all Actors now
    */
    void  destroyActors();

    /** \brief
    */
    void  batchDestroyActor(const NxString& identifier, BatchActionTimeFrame = BT_NEXT_AVAILABLE_BATCH);


    /** \brief
    */
    void  batchDestroyActors(const NxString& identifierPattern, NxU32 nbActors, BatchActionTimeFrame = BT_NEXT_AVAILABLE_BATCH);


    /** \brief
    */
    void  batchDestroyActors(Actors, BatchActionTimeFrame = BT_NEXT_AVAILABLE_BATCH);


    /* \brief Register a Renderable Actor Factory for use with the createBody functions.
    */
    void  registerRenderableActorFactory(unsigned int FactoryIdentifier, RenderableActorFactory*, bool garbage_collect);

    /* \brief Unregister (and delete if garbage collected) a Renderable Actor Factory
    */
    void  unregisterRenderableActorFactory(unsigned int FactoryIdentifier);

    /** \brief Add a generic ActorContactCallback, which is called with any callback
    */
    void  addContactCallback(ActorContactCallback*, GarbageCollectionPolicy);

    /** \brief Generate a Actor contact pair, and call up all generic ActorCallbacks when they make contact.
    */
    void  createActorContactPair(Actor*, Actor*, NxU32 collision_flags);

    /** \brief Pair up an actor to actor contact, and callup a specific ContactCallback (as well as the generic
               callbacks) when they contact.
    */
    void  createActorContactPair(Actor*, Actor*, bool exclusive, NxU32 collision_flags, ActorContactCallback*, GarbageCollectionPolicy = GC_Delete);

    /** \brief This iterates through all _current_ actors and generates contacts pairing with them, this function
               can be quite heavy going in a large scene, so it may be useful to use ActorGroups instead.
    */
    void  createActorContactPair(Actor*, ActorTypes, bool exclusive, NxU32 collision_flags, ActorContactCallback*, GarbageCollectionPolicy = GC_Delete);

    /** \brief Remove a generic ActorContactCallback, which is called with any callback. If you use many callbacks
               then they all will be removed.
        \note  Removal will delete the ActorContactCallback assuming it has GC_Delete.
    */
    void  removeContactCallback(ActorContactCallback*);

    /** \brief A more specific version of Scene::removeCallback.
    */
    void  removeContactCallback(Actor*, Actor*, ActorContactCallback*);

    /** \brief A more specific version of Scene::removeCallback.
    */
    void  removeContactCallback(Actor*, ActorTypes, ActorContactCallback*);

    /** \brief Returns a pointer to a Actor with a specified identifier.
              In the case of a unknown actor a NULL pointer is return.
              \note Identifiers are case-sensitve.
              \param identifier  Unique name of the Actor to fetch.
              \return Actor   The Actor or NULL.
    */
    Actor*  getActor(const NxString&);


    /** \brief Get the Actor that is the floor, or NULL if it does not exist.
    */
    Actor*  getFloorActor(void);


    /**  \brief Get's all Actors in the Scene.
                Returns a container of of the pointers of all the Actors in this Scene. Since your working with the 
                "Master" list of actors, you should be careful when handling with those Actors during a FrameSimulation. 
                When when adding or cloning Actors within a loop with the Master Loop may make the application freeze.
    \param Actors The Master list of Actors.
    */
    Actors*  getActors();


    /** \brief Gets all Actors with a similar identifier; Cake1, Cake2, Cake3, etc.
        \param NxStringOperator Does the identifier start or end with identifierPattern
        \param identifierPattern The pattern to look for
        \return The container of actors that matches.
    */
    Actors  getActorsFromIdentifier(NxStringOperator, const NxString& identifierPattern);

    /** \brief Gets all Actors created that have been created, not created, newer
               or older than a frame number
        \param NxComparisonOperator The operator to use on the frame number
        \param frame The frame number in question.
    */
    Actors  getActorsFromFrame(NxComparisonOperator, NxU32 frame);

    /** \brief Gets all Actors from a specific region in the Scene.
        \param SimpleShape  The Shape used to specificy the region in space
        \param NxShapeType  The types of shapes to look for (Dynamic, Static or both)
        \return The Actors within the region.
    */
    Actors&  getActorsFromRegion(SimpleShape*, NxShapesType);

    /** \brief Gets All actors from a specific region in the Scene, without re-specificing a Shape.
        \param NxShapeType  The types of shapes to look for (Dynamic, Static or both)
        \return The Actors within the region.
    */
    Actors&  getActorsFromLastRegion(NxShapesType);

    /** \brief Gets All actors from a raycast in the Scene.
        \param Position  Origin of the Raycast
        \param normal    Normalised Direction of the Raycast
        \param range     Maximum range of the ray can travel.
        \return The Actors within the sight of the ray.
    */
    ActorRaycastHitNodeIterator  getActorsFromRaycast(Ogre::Vector3 position, Ogre::Vector3 normal, NxReal range);

    /** \brief Gets the number of Actors in the scene.
        Gets the number of NxOgre Actors within the Scene. This does not keep account of NxActors with
        a different VoidPointer class such as Triggers or ForceFields, or manually created NxActors.
        \return NxU32 Total count of NxOgre Actors in this Scene.
    */
    NxU32  getNbActors() const;

    /** \brief Creates a Actor with some visualation.
        Creates a Body (An Actor with an SceneNode) within a Scene.
        \see Scene::createActor for more information.
        \warning <strong>Containers and Garbage Collection</strong> \n
        Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
        by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
        Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
        correct time.
        \param meshIdentifier	Visual Identifier of the Body.
        \see	  Scene::createActor for other Arguments.
        \see	  Body::Body
        \return The created Body
    */
    template<class BodyClass>
    Actor*  createBody(const VisualIdentifier& vid, Shape* shape, const Pose& pose, const ActorParams& a_params)
    {
      return mRenderableActorFactories[BodyClass::FactoryIdentifier]->createRendereredActor(
        this, vid, a_params, pose, shape, BodyClass::FactoryIdentifier
        );
    }


    /** \brief Creates a Actor with some visualation.
        Creates a Body (An Actor with an SceneNode) within a Scene. 
        \see Scene::createActor for more information.
        \warning <strong>Containers and Garbage Collection</strong> \n
        Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
        by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
        Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
        correct time.
        \param meshIdentifier Visual Identifier of the Body.
        \see  Scene::createActor for other Arguments.
        \see  Body::Body
        \return The created Body
    */
    template<class BodyClass>
    Actor*  createBody(const NxString& id, Shape* shape, const Pose& pose, const NodeRenderableParams& nr_params, const ActorParams& a_params)
    {
      return mRenderableActorFactories[BodyClass::FactoryIdentifier]->createRendereredActor(
        this, id, a_params, nr_params, pose, shape, BodyClass::FactoryIdentifier
        );
    }


    /** \brief Creates many bodies within the Scene now or sometime in the future.
        \warning <strong>Containers and Garbage Collection</strong> \n
        Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
        by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
        Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
        correct time.
        \param identifierPattern    identifierPattern of the Actors; "Cake" -> "Cake, Cake1, Cake2"
        \param visualData           visualData of the Body "(entity) entityName" or "box.mesh"
        \param nbActors             Number of Actors to create
        \param ShapeBlueprint*      The first shape to use for the Actor (new ShapeName(...))
        \param Pose                 Global Pose of the first actor
        \param Pose                 Pose difference between the next actor
        \param BatchActionTimeFrame When to create the Actors
        \param ActorParams          ActorParams for the actors

    */
    void  batchCreateBodies(const NxString& identifierPattern, const NxString& visualData, NxU32 nbActors, Shape*, const Pose& poseOrigin, const Pose& poseDifference, BatchActionTimeFrame = BT_NOW, ActorParams = ActorParams());


    /** \brief Destroy a Body in the scene.
    \note This function is the same as destroyActor.
    \warning <strong>Containers and Garbage Collection</strong> \n
    Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
    by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
    Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
    correct time.
    \param  identifier  Unique name of the Body to destroy.
    */
    void  destroyBody(const NxString&);


    /** \brief Creates a portion of the Scene where when Actors or Characters enter, leave or exist within it
    calls some code.
    \see Trigger::Trigger
    \see Trigger::setCallback
    */
    Trigger*  createTrigger(SimpleShape*, TriggerContactCallback*);


    /** \brief Destroy a Trigger in the scene.
    \note This function is the same as destroyActor.
    \warning <strong>Containers and Garbage Collection</strong> \n
    Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
    by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
    Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
    correct time.
    \param identifier Unique name of the Body to destroy.
    */
    void  destroyTrigger(Trigger*);

    /** \brief Gets the number of Actors in the scene.
    \return NxU32 Total count of NxOgre Triggers in this Scene.
    */
    NxU32  getNbTriggers();


    /** \brief Create a spherical joint between two actors
    \param Actor        First Actor
    \param Actor        Second Actor
    \param anchor       Global Anchor of the joint
    \param JointParams  Parameters for the Joint
    \return The joint
    */
    SphericalJoint*  createSphericalJoint(Actor*, Actor*, const Ogre::Vector3 &anchor, JointParams = JointParams());


    /** \brief Create a spherical joint between an actor and the world
    \param Actor        First Actor
    \param anchor       Global Anchor of the joint
    \param JointParams  Parameters for the Joint
    \return The joint
    */
    SphericalJoint*  createSphericalJoint(Actor*, const Ogre::Vector3 &anchor, JointParams = JointParams());


    /** \brief Create a spherical joint between two actors
    \param Actor        First Actor
    \param Actor        Second Actor
    \param axis         Axis of the joint
    \param anchor       Global Anchor of the joint
    \param JointParams  Parameters for the Joint
    \return The joint
    */
    RevoluteJoint*  createRevoluteJoint(Actor*, Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());


    /** \brief Create a revolute joint between an actor and the world
    \param Actor         First Actor
    \param axis          Axis of the joint
    \param anchor        Global Anchor of the joint
    \param JointParams   Parameters for the Joint
    \return The joint
    */
    RevoluteJoint*  createRevoluteJoint(Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());


    /** \brief Create a prismatic joint  between two actors
    \param Actor        First Actor
    \param Actor        Second Actor
    \param axis         Axis of the joint
    \param anchor       Global Anchor of the joint
    \param JointParams  Parameters for the Joint
    \return The joint
    */
    PrismaticJoint*  createPrismaticJoint(Actor*, Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());


    /** \brief Create a prismatic joint between an actor and the world
    \param Actor        First Actor
    \param axis         Axis of the joint
    \param anchor       Global Anchor of the joint
    \param JointParams  Parameters for the Joint
    \return The joint
    */
    PrismaticJoint*  createPrismaticJoint(Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());


    /** \brief Create a cylindrical joint between two actors
    \param Actor        First Actor
    \param Actor        Second Actor
    \param axis         Axis of the joint
    \param anchor       Global Anchor of the joint
    \param JointParams  Parameters for the Joint
    \return The joint
    */
    CylindricalJoint*  createCylindricalJoint(Actor*, Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());


    /** \brief Create a cylindrical joint between an actor and the world
    \param Actor        First Actor
    \param axis         Axis of the joint
    \param anchor       Global Anchor of the joint
    \param JointParams  Parameters for the Joint
    \return The joint
    */
    CylindricalJoint*  createCylindricalJoint(Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());


    /** \brief Create a fixed joint between two actors
    \param Actor		First Actor
    \param Actor		Second Actor
    \param JointParams	Parameters for the Joint
    \return The joint
    */
    FixedJoint*  createFixedJoint(Actor*, Actor*, JointParams = JointParams());


    /** \brief Create a fixed joint between two actors
    \param Actor		First Actor
    \param JointParams	Parameters for the Joint
    \return The joint
    */
    FixedJoint*  createFixedJoint(Actor*, JointParams = JointParams());


    /** \brief Destroys a joint 
    \param Joint The Joint to destroy
    */
    void releaseJoint(Joint*);

    /** \brief Destroys a joint 
    \param NxJoint The ID of the joint to destroy
    */
    void releaseJoint(NxJointID);

    /** \brief Creates a Character (Intended to simulate up-right human characters and movement)
    \note  Characters have no form of visualisation
    \param identifier  The identifier of the character
    \param pose        The pose to place the character
    \param params      Character params for the character
    \see Character::Character
    \see Performer
    \see Character::CharacterParams
    \return A fresh new, unpacked, with that new car-smell character
    */
    CharacterSystem::Character*  createCharacter(const NxString &identifier, Pose pose, CharacterSystem::CharacterModel*, CharacterSystem::CharacterParams params);

    /** \brief Creates a Character (A raycasted kinematic actor intended to simulate human characters)
    \note  Performers are exactly like Characters but they have a form of NodeRenderable visualisation.
    \param VisualIdentifier  The identifier of the character with basic visualisation information
    \param pose              The pose to place the character
    \param params            Character params for the character

    \see Character::Character
    \see CharacterParams
    \return A fresh new, unpacked, with that new car-smell character
    */
    CharacterSystem::Performer*  createPerformer(const VisualIdentifier&, const Pose&, CharacterSystem::CharacterModel*, const CharacterSystem::CharacterParams&);

    /** \brief Creates a Character (A raycasted kinematic actor intended to simulate human characters)
    \note  Performers are exactly like Characters but they have a form of NodeRenderable visualisation.
    \param identifier            The identifier of the character
    \param pose                  The pose to place the character
    \param CharacterParams       Character params for the character
    \param NodeRenderableParams  Visualisation params for the performer
    \see Character::Character
    \see CharacterParams
    \return A fresh new, unpacked, with that new car-smell character
    */
    CharacterSystem::Performer*  createPerformer(const NxString&, const Pose&, CharacterSystem::CharacterModel*, const CharacterSystem::CharacterParams&, const NodeRenderableParams&);


    /** \brief Destroys a character from the Scene
    \param identifier  The character to destroy
    */
    void  destroyCharacter(const NxString& identifer);

    /** \brief Creates a material for a shape
    \params identifier The identifier of the material
    \return The material
    */
    Material*  createMaterial(const NxString& identifier);

    /** \brief Destroys a material from a scene.
    \note  This will destroy the material from the Scene class, but any Shapes using the material
    index will not change. In most cases it is better to leave the material until the very
    end of the scene.
    \param identifier The material name to delete
    */
    void  destroyMaterial(const NxString& identifier);

    /** \brief Gets a material from it's identifier
    \return the material, or a null pointer if the material does not exist.
    */
    Material*  getMaterial(const NxString& identifier);

    /** \brief Gets a material from it's material index
    \return the material, or a null pointer if the material does not exist.
    */
    Material*  getMaterial(NxMaterialIndex);

    /** \brief Gets all materials in the scene
    \return All of the materials
    */
    MaterialList*  getMaterials();

    /** \brief Gets the total number of materials in the scene
    \return The number of materials
    */
    NxU32  getNbMaterials();

    /** \brief Gets a material index based of the material's identifier
    \return The material index, or 0 if the material does not exist.
    */
    NxMaterialIndex  getMaterialIndex(const NxString& identifier);

    /** \brief Creates a MaterialAlias containing their names and NxMaterialIndex, for use with
    cooking.
    */
    Resources::MaterialAlias*   createMaterialAliasFromMaterials(void);

    /** \brief
    */
    void  addMaterialPair(Ogre::Material*, NxOgre::Material*);

    /** \brief
    */
    void  addMaterialPair(const NxString& ogre, const NxString& nxogre);

    /** \brief
    */
    void  addMaterialPairsFromFile(const NxString& ogre, const NxString& nxogre);

    /** \brief
    */
    ActorGroup*  createActorGroup(const NxString& name);

    /** \brief
    */
    void  destroyActorGroup(const NxString& name);

    /** \brief
    */
    NxU32  getNbActorGroups() const;

    /** \brief
    */
    ActorGroups*  getActorGroups();

    /** \brief
    */
    ActorGroupsByIndex*  getActorGroupsByIndex();

    /** \brief
    */
    ActorGroup*  getActorGroup(const NxString& name);

    /** \brief
    */
    ShapeGroup*  createShapeGroup(const NxString& name);

    /** \brief
    */
    void  destroyShapeGroup(const NxString& name);

    /** \brief
    */
    NxU32  getNbShapeGroups() const;

    /** \brief
    */
    ShapeGroups*  getShapeGroups();

    /** \brief
    */
    ShapeGroupsByIndex*  getShapeGroupsByIndex();

    /** \brief
    */
    ShapeGroup*  getShapeGroup(const NxString& name);

#if (NX_SDK_VERSION_NUMBER >= 272) 

    /** \brief
    */
    DominanceGroup*  createDominanceGroup(const NxString& name);

    /** \brief
    */
    void  setDominanceGroupPair(const NxString& AGroup, const NxString& BGroup, NxReal ADominance, NxReal BDominance);

    /** \brief
    */
    void  setDominanceGroupPair(DominanceGroup* AGroup, DominanceGroup* BGroup, NxReal ADominance, NxReal BDominance);

    /** \brief
    */
    void  destroyDominanceGroup(const NxString& name);

    /** \brief
    */
    NxU32  getNbDominanceGroups() const;

    /** \brief
    */
    DominanceGroups*  getDominanceGroups();

    /** \brief
    */
    DominanceGroupsByIndex*  getDominanceGroupsByIndex();

    /** \brief
    */
    DominanceGroup*  getDominanceGroup(const NxString& name);

#endif

    /** \brief Add an Actor with a Ground (shape) attached; equilivent to the SceneParams "floor: yes".
    */
    void  addFloor();

    /** \brief
    */
    Raycaster*  createRaycaster();

    /** \brief
    */
    void  destroyRaycaster(Raycaster*);


#if (NX_USE_CLOTH_API == 0)

    /** \brief
    */
    Actor*  createDeformableActor(const NxString& meshName, ShapeBlueprint* roughShape, const Pose&, ActorParams, ClothParams, NxReal impulseThreshold, NxReal penetrationDepth = 0.0f);

    /** \brief
    */
    Cloth*  createCloth(const NxString& identifier, const Pose&, ClothParams);

    /** \brief
    */
    Cloth*  createCloth(const NxString& identifier, const NxString& meshName, const Pose&, ClothParams);

    /** \brief
    */
    //Cloth*  createCloth(const NxString& identifier, NxClothDesc, NxMeshData, Ogre::MeshPtr, const Pose&);

    /** \brief
    */
    void  destroyCloth(const NxString&);

    /** \brief
    */
    Cloths*  getCloths();

    /** \brief
    */
    NxU32  getNbCloths();

#endif

#if (NX_USE_SOFTBODY_API == 0)

    /** \brief
    */
    SoftBody*  createSoftBody(const NxString& TetAndMeshFilePrefix, const Pose&, SoftBodyParams);

    /** \brief
    */
    SoftBody*  createSoftBody(NxReal width, NxReal height, NxReal, const Pose&, SoftBodyParams);

    /** \brief
    */
    void  destroySoftBody(const NxString& identifier);

    /** \brief
    */
    NxU32  getNbSoftBodies();

    /** \brief
    */
    SoftBodies*  getSoftBodies();

#endif


#if (NX_USE_FORCEFIELD_API == 1)

    /** \brief
    */
    ForceField*  createForceField();

    /** \brief
    */
    void  destroyForceField();

    /** \brief
    */
    NxU32  getNbForceFields();

    /** \brief
    */
    ForceFields*  getForceFields();
#endif

    /** \brief
    */
    void  registerMachine(Machine*);

    /** \brief
    */
    void  unregisterMachine(Machine*);

    /** \brief Set the sleep callback, and destruction policy;
    */
    void  setSleepCallback(SleepCallback*, GarbageCollectionPolicy);

    /** \brief Get the sleep callback
    */
    SleepCallback*  getSleepCallback();

    /** \brief
    */
    void  onContactNotify(NxContactPair &,NxU32);

    /** \brief
    */
    bool  onJointBreak(NxReal,NxJoint &);

    /** \brief
    */
    void  onWake(NxActor **,NxU32);

    /** \brief
    */
    void  onSleep(NxActor **,NxU32);

    /** \brief Set deletion callback
    */
    void  setDeletionCallback(DeletionCallback*, GarbageCollectionPolicy policy = GC_Delete);

    /** \brief Get deletion callback
    */
    DeletionCallback*  getDeletionCallback();

  protected:

    /**  \brief Advances the state of the Scene
    \note Do not control by hand. World will automatically perform this operation for you. 
    \param time Time to advance since last Simulation		
    */
    void  simulate(NxReal time);


    /** \brief Reflects the changes of the simulation to the SceneRenderer.
    \see SceneRenderer
    */
    void  render();


    /** \brief
    */
    typedef void (Scene::*SceneIdlePtr)(const TimeStep&);

    /** \brief
    */
    enum  SimulationMode {
      SM_Idle,
      SM_Batching
    };


    /** \brief
    */
    void  setSimulationMode(SimulationMode);


    /** \brief Used when the Scene isn't
    */
    void  simulateIdle(const TimeStep&) {}


    /** \brief Batching function when the Scene requires it.
    */
    void  simulateBatching(const TimeStep&);


    /** \brief
    */
    void  setName(const NxString& s);


    /** \brief
    */
    void  _createDefaultActorGroup();


    /** \brief
    */
    void  _createDefaultShapeGroup();


    /** \brief
    */
    void  _createDefaultDominanceGroup();


    /** \brief
    */
    void  _createDefaultMaterial(NxReal r, NxReal d, NxReal s);


    /** \brief
    */
    void  _registerActor(const NxString& name, Actor*);


    /** \brief
    */
    void  _unregisterActor(const NxString&);


    /** \brief
    */
    void  _registerActorGroup(const NxString& name, ActorGroup*);


    /** \brief
    */
    void  _unregisterActorGroup(const NxString& name);


    /** \brief
    */
    void  _registerMaterial(const NxString& name, Material*);


    /** \brief
    */
    void  _unregisterMaterial(const NxString&);


    /** \brief
    */
    void  _registerShapeGroup(const NxString& name, ShapeGroup*);


    /** \brief
    */
    void  _unregisterShapeGroup(const NxString& name);


#if (NX_SDK_VERSION_NUMBER >= 272)
    /** \brief
    */
    void  _registerDominanceGroup(const NxString& name, DominanceGroup*);
#endif


#if (NX_SDK_VERSION_NUMBER >= 272)
    /** \brief
    */
    void  _unregisterDominanceGroup(const NxString& name);
#endif


    /** \brief
    */
    void  _registerTrigger(const NxString& name, Trigger*);


    /** \brief
    */
    void  _unregisterTrigger(const NxString& name);


    /** \brief
    */
    void  _registerCharacter(const NxString& name, CharacterSystem::Character*);


    /** \brief
    */
    void  _unregisterCharacter(const NxString&);


#if (NX_USE_CLOTH_API == 1)
    /** \brief
    */
    void  _registerCloth(const NxString& name, Cloth*);
#endif


#if (NX_USE_CLOTH_API == 1)
    /** \brief
    */
    void  _unregisterCloth(const NxString&);
#endif


#if (NX_USE_FLUID_API == 1)
    /** \brief
    */
    void  _registerFluid(const NxString& name, Fluid*);
#endif


#if (NX_USE_FLUID_API == 1)
    /** \brief
    */
    void  _unregisterFluid(const NxString&);
#endif


#if (NX_USE_SOFTBODY_API == 1)
    /** \brief
    */
    void  _registerSoftBody(const NxString& name, SoftBody*);
#endif

#if (NX_USE_SOFTBODY_API == 1)
    /** \brief
    */
    void  _unregisterSoftBody(const NxString&);
#endif


#if (NX_USE_FORCEFIELD_API == 1)
    /** \brief
    */
    void  _registerForceField(const NxString& name, ForceField*);
#endif


#if (NX_USE_FORCEFIELD_API == 1)
    /** \brief
    */
    void  _unregisterForceField(const NxString&);
#endif

    typedef Betajaen::SharedList<Raycaster>  Raycasters;

    /** \brief
    \internal
    */
    World*                               mOwner;


    /** \brief
    \internal
    */
    NxSceneDesc                          mDescription;


    /** \brief
    \internal
    */
    NxScene*                             mScene;


    /** \brief
    \internal
    */
    NxString                             mName;


    /** \brief
    \internal
    */
    const char*                          mNxID;


    /** \brief
        \internal
    */
    Actor*                               mFloorActor;

    /** \brief
    \internal
    */
    Actors                               mActors;


    /** \brief
    \internal
    */
    ActorGroups                          mActorGroups;


    /** \brief
    \internal
    */
    ActorGroupsByIndex                   mActorGroupsByIndex;


    /** \brief
    \internal
    */
    Joints                               mJoints;


    /** \brief
    \internal
    */
    Triggers                             mTriggers;


#if (NX_USE_CLOTH_API == 0)
    /** \brief
    \internal
    */
    Cloths                               mCloths;
#endif


#if (NX_USE_CHARACTER_API == 1)
    /** \brief
    \internal
    */
    Characters                           mCharacters;
#endif


#if (NX_USE_FLUID_API == 1)
    /** \brief
    \internal
    */
    //			Fluids                               mFluids;
#endif


#if (NX_USE_FORCEFIELD_API == 1)
    /** \brief
    \internal
    */
    ForceFields                          mForceFields;
#endif


#if (NX_USE_SOFTBODY_API == 0)
    /** \brief
    \internal
    */
    SoftBodies                           mSoftBodies;
#endif


    /** \brief
    \internal
    */
    MaterialList                         mMaterials;


    /** \brief
    \internal
    */
    MaterialListByIndex                  mMaterialsByIndex;


    /** \brief
    \internal
    */
    MaterialPair                         mMaterialPair;


    /** \brief
    \internal
    */
    Machines                             mMachines;


    /** \brief
    \internal
    */
    Betajaen::SharedList<SimpleActor>    mSimpleActors;


    /** \brief
    \internal
    */
    ShapeGroups                          mShapeGroups;


    /** \brief
    \internal
    */
    ShapeGroupsByIndex                   mShapeGroupsByIndex;


#if (NX_SDK_VERSION_NUMBER >= 272) 
    /** \brief
    \internal
    */
    DominanceGroups                      mDominanceGroups;
#endif


#if (NX_SDK_VERSION_NUMBER >= 272)
    /** \brief
    \internal
    */
    DominanceGroupsByIndex               mDominanceGroupsByIndex;
#endif


    /** \brief
    \internal
    */
    NxReal                               mControllers_MaxTimestep;


    /** \brief
    \internal
    */
    NxTimeStepMethod                     mControllers_Method;


    /** \brief
    \internal
    */
    NxU32                                mControllers_MaxIter;


    /** \brief
    \internal
    */
    NxU32                                mControllers_NumSubSteps;


    /** \brief
    \internal
    */
    SceneController*                     mSceneController;


    /** \brief
    \internal
    */
    SceneRenderer*                       mSceneRenderer;


    /** \brief
    \internal
    */
    SceneTriggerController*              mSceneTriggerController;


    /** \brief
    \internal
    */
    Raycaster*                           mSceneRaycaster;

    /** \brief
    \internal
    */
    Raycasters                           mRaycasters;

    /** \brief
    \internal
    */
    Intersection*                        mSceneIntersection;


    /** \brief
    \internal
    */
    SleepCallback*                       mSleepCallback;


    /** \brief
    \internal
    */
    GarbageCollectionPolicy              mSleepCallbackPolicy;


    /** \brief
    \internal
    */
    DeletionCallback*                    mDeletionCallback;


    /** \brief
    \internal
    */
    GarbageCollectionPolicy              mDeletionCallbackPolicy;


    /** \brief
    \internal
    */
    Betajaen::SharedList<DelayedActor>   mDelayedDeletionActors;


    /** \brief
    \internal
    */
    SceneIdlePtr                         mSimulationModePtr;


    /** \brief
    \internal
    */
    TimeStep                             mCurrentTimeStep;

    /** \brief
        \internal
    */
    Container<unsigned int, RenderableActorFactory*> mRenderableActorFactories;

    /** \brief
        \internal
    */
    Betajaen::SharedList<ActorContactCallbackAutoPtr> mActorContactCallbacks;

    /** \brief
        \internal
    */
    Betajaen::SharedList<ActorContactSpecificCallbackAutoPtr> mActorSpecificContactCallbacks;

  private:


    /** \brief
    \internal
    */
    struct BatchActorBlueprint
    {
      NxString      identifierPattern;
      NxString      visualData;
      NxU32         nbActors;
      Shape*        collisionModel;
      Pose          poseOrigin;
      Pose          poseDelta;
      ActorParams   params;
    };


    /** \brief
    \internal
    */
    std::vector< std::pair< BatchActionTimeFrame, BatchActorBlueprint > > mBatchActorCreation;


    /** \brief
    \internal
    */
    std::vector< std::pair< BatchActionTimeFrame, Actors > > mBatchActorDestruction;


    /** \brief
    \internal
    */
    bool mBatchProcessSimulate;


    /** \brief
    \internal
    */
    void _batchActorCreate(BatchActorBlueprint&);


    /** \brief
    \internal
    */
    void _batchBodyCreate(BatchActorBlueprint&);


    /** \brief
    \internal
    */
    void _batchActorDestroy(Actors&);


  }; // End of Scene class.

}; // End of NxOgre namespace.

#endif
