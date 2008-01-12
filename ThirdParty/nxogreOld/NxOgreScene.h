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

#ifndef __NXOGRE_SCENE_H__
#define __NXOGRE_SCENE_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreContainer.h"		// For: Scene::mActors
#include "NxOgreActor.h"			// For: ActorParams
#include "NxOgreJoint.h"			// For: JointParams
#include "NxOgreParams.h"			// For: Params
#include "NxOgrePose.h"
#include "NxOgreNodeRenderable.h"	// For: NodeRenderableParams (for Body)

namespace NxOgre {

		/**
	 	\page sceneparams SceneParams
		
		String base params are case and space insensitive.

		Examples:
		
		<code>
			"floor: yes"

			"floor: yes, gravity: 0 -9 0"

			"floor: yes, gravity: 0 -9 0, defaultmaterialresitition: 0.9"
		</code>

		Non-String based params can be used as well, and offer access to some variables of the params
		that cannot be converted into a string (pointers, etc.). 

		Due to efficency purposes and insane string conversion. With string parameters more than four, 
		you should work with the non-string based params system. 

		<code>
			ActorParams params;
			params.setToDefault();
			params.floor = true
			params.gravity = NxVec3(0, -9, 0);
		</code>

		Is exactly the same as:

		<code>
			"floor: yes, gravity: 0 -9 0"
		</code>
		
		Current Supported String Params
		-----------------------------
		
		-	density (float)
			- Description: Density of this Actor. Use this or mass, do not use both.
			- Example: "Density: 1.5"
			- See: ActorParams::mDensity and NxActorDesc::density for more information.	

	*/
	
	/** SceneParams

		See \ref sceneparams for the full string argument documentation.
	*/	
	class NxExport SceneParams : Params {

		public:


			SceneParams() {}
			SceneParams(const char* p)  {process(p);}
			SceneParams(NxString p) {process(p);}

			void setToDefault();
			void parse(Parameters);

			/**	@brief An infinite ground plane on the +Y axis.
				@default false;
			 */
			bool				floor;

			/** @brief The acceleration of gravity in a scene.
				@default 0 -9.80665 0
			 */
			NxVec3				gravity;

			/** @brief The restitution (bounce) value of the default material.
				@default 0
			 */
			NxReal				defaultMaterialRestitution;

			/** @brief The dynamic friction value of the default material.
				@default 0.5
			 */
			NxReal				defaultMaterialDynamicFriction;

			/** @brief The static friction value of the default material.
				@default 0.5
			 */
			NxReal				defaultMaterialStaticFriction;

			
			/** @brief maximum Timestep
				@default 0.016667
			 */
			NxReal				max_timestep;
			
			
			/** @brief maximum Iterator
				@default 8
			 */
			NxU32				max_iter;

			
			/** @brief maximum bound
				@default NULL
			 */
			NxBounds3*			max_bounds;

			
			/** @brief scene limits
				@default NULL
			 */
			NxSceneLimits*		limits;

			
			/** @brief Is the simulation on a CPU (Software) or on a PPU (Hardware)
				@default NX_SIMULATION_SW
			 */
			NxSimulationType	sim_type;

			
			/** @brief Has bounds planes
				@default false
			 */
			bool				bounds_planes;


			/** @brief Initial flags of the scene
				@default NX_SF_SIMULATE_SEPARATE_THREAD | NX_SF_DISABLE_SCENE_MUTEX
			 */
			NxU32				flags;


			/** @brief Internal thread count
				@default 0
			 */
			NxU32				internal_thread_count;
			

			/** @brief Background thread count
				@default 0
			 */
			NxU32				background_thread_count;


			/** @brief Custom Scheduler
				@default NULL
			 */
			NxUserScheduler*	custom_scheduler;
			

			/** @brief Thread Mask
				@default 0x55555554
			 */
			NxU32				thread_mask;
						

			/** @brief Background Thread Mask
				@default 0x55555554
			 */
			NxU32				background_thread_mask;
						

			/** @brief Simulation thread stack size
				@default 0
			 */
			NxU32				sim_thread_stack_size;
									

			/** @brief Simulation thread priority
				@default NX_TP_NORMAL
				@see NxSceneDesc::simThreadPriority
			 */
			NxThreadPriority	sim_thread_priority;
						

			/** @brief Worker thread stack size
				@default 0
				@see NxSceneDesc::workerThreadStackSize
			 */
			NxU32				worker_thread_stack_size;
									

			/** @brief Worker thread priority
				@default NX_TP_NORMAL
				@see NxSceneDesc::workerThreadPriority
			 */
			NxThreadPriority	worker_thread_priority;
			
									
			/** @brief What is up?
				@default 0
				@see NxSceneDesc::upAxis
			 */
			NxU32				up_axis;

									
			/** @brief Subdivision level
				@default 5
			 */
			NxU32				subdivision_level;
			
									
			/** @brief Static structure
				@default  NX_PRUNING_DYNAMIC_AABB_TREE
			 */
			NxPruningStructure	static_structure;
			
									
			/** @brief Dynamic structure
				@default NX_PRUNING_NONE
			 */
			NxPruningStructure	dynamic_structure;
			
			enum SceneControllerType {
				CN_NULL,
				CN_FIXED,
				CN_VARIABLE,
				CN_ACCUMULATOR
			};

			/** @brief Scene Controller to use (Fixed, Variable, Accumulator or Null)
				@note If your using your own scenecontroller, supply a Null controller as a param first.
				@default CN_FIXED (Fixed timestep)
			*/
			SceneControllerType		controller;

			enum SceneRendererType {
				RN_NULL,
				RN_OGRE
			};

			/** @brief Scene Renderer to use (Ogre or Null)
				@note If your using your own scene renderer, supply a Null renderer as a param first.
				@default RN_GENERIC
			*/
			SceneRendererType		renderer;


			/** @brief Scene Renderer UserData (RenderSystem specific)
			*/
			NxString				rendererUserData;

	};

	/** A Scene is a sub-section of a World. Although a Scene is infinite in size like World. Scenes are 
		considered to be seperate from each other. Actors from one scene cannot interact with ones from
		another. Scenes have also their own Materials, Groups, etc. which cannot be shared.

		@warning <strong>Containers and Garbage Collection</strong> \n
			Scenes are used in containers. Creating a Scene through the World factory method makes the Scene "owned"
			by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
			Manually creating the Scene "new Scene()" will only add a presence, it is up to you to delete it at the
			correct time.

	*/
	class NxExport Scene {

		friend class Actor;
		friend class ActorGroup;
		friend class ShapeGroup;
		friend class CharacterController;
#if (NX_USE_CLOTH_API == 1)
		friend class Cloth;
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
#if (NX_SDK_VERSION_NUMBER >= 272)
		friend class DominanceGroup;
#endif
		public:

			/** @brief Used to specify when an batched action will happen.
			*/
			enum BatchActionTimeFrame {
				BT_NOW,	
				BT_NEXT_AVAILABLE_BATCH,
				BT_NEXT_SIMULATION,
				BT_WITHIN_SECONDS,
				BT_NEVER
			};

			/** @brief Renderable types
			*/
			enum RenderableType {
				RT_EVERYTHING	= 0,
				RT_MOVED		= 1,
				RT_TRANSFORM	= 2
			};

			/**@brief Scene Constructor
			
				 Create's a Scene, and NxScene withing physics. It also creates default material, actor groups, 
				 shape groups and dominance groups. Based on the SceneParams, Gravity may be set, and an actor
				 created with a GroundShape to represent the ground plane or the floor.
 
			   @warning <strong>Containers and Garbage Collection</strong> \n
			   	 Scenes are used in containers. Creating a Scene through the World factory method makes the Scene "owned"
			     by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
			     Manually creating the Scene "<code>new Scene()</code>" will only add a presence, it is up to you to delete it at the
			     correct time.
			   
			    @param Identifier				Unique identifier of the Scene
			   	@param World*					World pointer which would contain the Scene.
			    @param SceneParams				SceneParams for this Scene to use.
			   
			    @see World::createScene
			    @see SceneParams
			*/  	
			Scene(const NxString& identifier, World*, SceneParams = SceneParams());


			/** Alternate Scene Constructor. Used for to bind a Scene to a previously made NxScene.
					
				@param Identifier				Unique identifier of the Scene
			   	@param Ogre::SceneManager*		SceneManger for this Scene to Use.
			    @param World*					World pointer which would contain the Scene.
			    @param NxScene*					NxScene to bind to.
			   
			    @see World::createScene
			    @see SceneParams
			 */ 
			Scene(const NxString& identifier, World*, Ogre::SceneManager*, NxScene*);

			/** Scene Destructor
			   
			    @note Containers and Garbage Collection
			     If the Scene is owned by NxOgre use World::SceneDestroy, if not then use "delete mScene".
			   
			    @see World::destroyScene
			*/
			~Scene();

			/**  @brief Returns the name of the Scene.

			     @return NxString Scene identifier
			 */
			NxString getName()								{
																return mName;
															}

			/**  @brief Returns the SceneManager instance the Scene is using

			     @return SceneManager SceneManager pointer.
			 */
			Ogre::SceneManager* getSceneManager()			{
																return mSceneMgr;
															}

			/**  @brief Returns a copy of a pointer of the NxScene which the Scene works with.

			     @return NxScene The NxScene's pointer
			 */
			NxScene* getNxScene()							{
																return mScene;
															}

			/** @brief Returns a copy of the World which the Scene is part of.
			   
			    @return World The World itself.
			 */
			World* getWorld()								{
																return mOwner;
															}


			/**@brief Sets the Scene Controller for simulation/rendering control of the Scene.

				Sets the Controller used for Simulation and Rendering of the NxScene. These can be Variable, Fixed,
			   	or Accumulator. You can use your own SceneController if you wish, otherwise the SceneController should
				be specified in the SceneParams "time-step-method" with Fixed, Variable or Accumulator as an option.
			   
			   @warning <strong>Containers and Garbage Collection</strong> \n
			   		SceneController instance will be deleted on the destruction of the Scene.	
			   
			   @param SceneController Instance of the SceneController to use.
			 */  	
			void setSceneController(SceneController*);


			/** @brief Gets the Scene Controller which is controls of the Physics simulation.
			*/
			SceneController* getSceneController() {return mSceneController;}

			/** @brief Gets the last known deltaTime injected into the controller.
			*/
			NxReal getLastDeltaTime() const;

			/** @brief Gets the last known alpha value of the controller.
			*/
			NxReal getLastAlphaValue() const;

			/** @brief Gets the Scene Renderer which renders the results of the Physics simulation
			*/
			SceneRenderer* getSceneRenderer() {return mSceneRenderer;}

			/** @brief Set's the Scene Renderer, and will delete the previous one
			*/
			void setSceneRenderer(SceneRenderer*);

			/** @brief Get a list of renderables based on time
			*/
			Renderables	getRenderable(RenderableType);

			/**	 @brief Save the current Scene and contents to a SceneBlueprint
			 */
			Blueprints::SceneBlueprint  save();

			/**	 @brief Returns the NxSimulationType (Hardware/Software) of the Scene.
			   	 @returns NxSimulationType		Simulation Type
			 */	 	
			NxSimulationType getSimType() const;


			/**	 @brief Sets the gravitional constant for the Scene. 
				 @param Ogre::Vector3(Acceleration/metres per second)	Gravitional Constant
			 */  	
			void setGravity(const Ogre::Vector3&);


			/**  @brief Gets the gravitional constant for the Scene. 
				 @return	Ogre::Vector3(Acceleration/metres per second)	Gravitional Constant
			 */	  	
			Ogre::Vector3 getGravity() const;


			/**@brief Creates an Actor within the Scene.

			   @warning <strong>Containers and Garbage Collection</strong> \n
			   	   Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
			       by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
			       Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
			       correct time.
			   
			   	 @param identifier			Unique name of the Actor. (Leave blank for a auto-generated identifier)
			   	 @param ShapeBlueprint*	The first shape to use for the Actor (new ShapeName(...))
			   	 @param Pose				Global Pose of the Actor
			   	 @param ActorParams		ActorParams for this actor
			   
			   	 @return The created Actor
			 */	 	
			Actor*	createActor(const NxString& identifier, ShapeBlueprint*, const Pose&, ActorParams = ActorParams());


			/**@brief Creates many actors within the Scene now or sometime in the future.
				
			   @warning <strong>Containers and Garbage Collection</strong> \n
			   	   Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
			       by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
			       Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
			       correct time.
			   
			   	 @param identifierPattern		identifierPattern of the Actors; "Cake" -> "Cake, Cake1, Cake2"
				 @param nbActors				Number of Actors to create
			   	 @param ShapeBlueprint*		The first shape to use for the Actor (new ShapeName(...))
			   	 @param Pose					Global Pose of the first actor
				 @param Pose					Pose difference between the next actor
				 @param BatchActionTimeFrame	When to create the Actors
			   	 @param ActorParams			ActorParams for the actors
			   

			*/

			void	batchCreateActors(const NxString& identifierPattern, NxU32 nbActors, ShapeBlueprint*, const Pose& poseOrigin, const Pose& poseDifference, BatchActionTimeFrame = BT_NOW, ActorParams = ActorParams());
			
			/**@brief Destroys an Actor (or any inherited Actor) within the scene.
		
				  @warning <strong>Containers and Garbage Collection</strong> \n
			   	   Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
			       by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
			       Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
			       correct time.
			   
			   	  @param identifier Unique name of the Actor to destroy.
			 */	  	
			void	destroyActor(const NxString&);


			/**

			*/
			void	batchDestroyActor(const NxString& identifier, BatchActionTimeFrame = BT_NEXT_AVAILABLE_BATCH);
			
			/**

			*/
			void	batchDestroyActors(const NxString& identifierPattern, NxU32 nbActors, BatchActionTimeFrame = BT_NEXT_AVAILABLE_BATCH);
	
			/**

			*/
			void	batchDestroyActors(Actors, BatchActionTimeFrame = BT_NEXT_AVAILABLE_BATCH);

			/** @brief Returns a pointer to the Actor

				In the case of a unknown actor a NULL pointer is return.
				
				@note Identifiers are case-sensitve.

			   	@param identifier		Unique name of the Actor to fetch.
			   	@return Actor			The Actor or NULL.
			 */	  	
			Actor*	getActor(const NxString&);


			/**	 @brief Get's all Actors in the Scene.

				 Returns a container of of the pointers of all the Actors in this Scene. Since your working with the 
				 "Master" list of actors, you should be careful when handling with those Actors during a FrameSimulation. 
				 When when adding or cloning Actors within a loop with the Master Loop may make the application freeze.
			   
			   	 @param Actors The Master list of Actors.
			 */	  	
			Actors*	getActors();


			/** @brief Gets all Actors with a similar identifier; Cake1, Cake2, Cake3, etc.
				@param NxStringOperator Does the identifier start or end with identifierPattern
				@param identifierPattern The pattern to look for
				@return The container of actors that matches.
			*/
			Actors getActorsFromIdentifier(NxStringOperator, const NxString& identifierPattern);
			
			/** @brief Gets all Actors created that have been created, not created, newer
					   or older than a frame number
				@param NxComparisonOperator The operator to use on the frame number
				@param frame The frame number in question.
			*/
			Actors getActorsFromFrame(NxComparisonOperator, NxU32 frame);

			/** @brief Gets all Actors from a specific region in the Scene.
				@param SimpleShape	The Shape used to specificy the region in space
				@param NxShapeType	The types of shapes to look for (Dynamic, Static or both)
				@return The Actors within the region.
			*/
			Actors& getActorsFromRegion(SimpleShape*, NxShapesType);
			
			/** @brief Gets All actors from a specific region in the Scene, without re-specificing a Shape.
				@param NxShapeType	The types of shapes to look for (Dynamic, Static or both)
				@return The Actors within the region.
			*/
			Actors& getActorsFromLastRegion(NxShapesType);

			/**	@brief Gets All actors from a raycast in the Scene.
				@param Position	Origin of the Raycast
				@param normal	Normalised Direction of the Raycast
				@param range	Maximum range of the ray can travel.
				@return The Actors within the sight of the ray.
			*/
			Actors getActorsFromRaycast(Ogre::Vector3 position, Ogre::Vector3 normal, NxReal range);

			/**@brief Gets the number of Actors in the scene.
			   	
				Gets the number of NxOgre Actors within the Scene. This does not keep account of NxActors with
			   	a different UserData class such as Triggers or ForceFields, or manually created NxActors.
			   
			   	@return NxU32 Total count of NxOgre Actors in this Scene.
			 */  	  	
			NxU32	getNbActors() const;


			/**@brief Creates a Actor with some visualation.
			   
			   Creates a Body (An Actor with an SceneNode) within a Scene. 

			   @see Scene::createActor for more information.
			   
			   @warning <strong>Containers and Garbage Collection</strong> \n
			   	 Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
			     by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
			     Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
			     correct time.
			   
			   @param meshIdentifier	Visual Identifier of the Body.
			   @see	  Scene::createActor for other Arguments.
			   @see	  Body::Body
			   @return The created Body
			*/
			Body*	createBody(const NxString& visualIdentifier, ShapeBlueprint*, const Pose&, ActorParams = "");
			
			/**@brief Creates a Actor with some visualation.
			   
			   Creates a Body (An Actor with an SceneNode) within a Scene. 

			   @see Scene::createActor for more information.
			   
			   @warning <strong>Containers and Garbage Collection</strong> \n
			   	 Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
			     by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
			     Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
			     correct time.
			   
			   @param meshIdentifier	Visual Identifier of the Body.
			   @see	  Scene::createActor for other Arguments.
			   @see	  Body::Body
			   @return The created Body
			*/
			Body*	createBody(const NxString& visualIdentifier, ShapeBlueprint*, const Pose&, NodeRenderableParams, ActorParams = "");
			

			/**@brief Creates many bodies within the Scene now or sometime in the future.
				
			   @warning <strong>Containers and Garbage Collection</strong> \n
			   	   Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
			       by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
			       Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
			       correct time.
			   
			   	 @param identifierPattern		identifierPattern of the Actors; "Cake" -> "Cake, Cake1, Cake2"
				 @param visualData				visualData of the Body "(entity) entityName" or "box.mesh"
				 @param nbActors				Number of Actors to create
			   	 @param ShapeBlueprint*		The first shape to use for the Actor (new ShapeName(...))
			   	 @param Pose					Global Pose of the first actor
				 @param Pose					Pose difference between the next actor
				 @param BatchActionTimeFrame	When to create the Actors
			   	 @param ActorParams			ActorParams for the actors
			   
			*/
			void	batchCreateBodies(const NxString& identifierPattern, const NxString& visualData, NxU32 nbActors, ShapeBlueprint*, const Pose& poseOrigin, const Pose& poseDifference, BatchActionTimeFrame = BT_NOW, ActorParams = ActorParams());
			

			/**@brief Destroy a Body in the scene.

			   @note This function is the same as destroyActor.
			   
			   @warning <strong>Containers and Garbage Collection</strong> \n
			   	   Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
			       by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
			       Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
			       correct time.
			   
			   @param identifier		Unique name of the Body to destroy.
			 */  	
			void	destroyBody(const NxString&);
			
			//////////////////////////////////////////////////////////////////////////////
			
			/**@brief Creates a portion of the Scene where when Actors or Characters enter, leave or exist within it
					  calls some code.

			   Triggers are pretty much the same as Actors, so the creation and usage is nearly the same. 
			   To assign a callback use the see Trigger::setCallback after the trigger has been ceated.

			   @warning <strong>Containers and Garbage Collection</strong> \n
			   	   Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
			       by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
			       Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
			       correct time.
				@see Trigger::Trigger
				@see Trigger::setCallback
			    @param identifier Unique name of the Body to destroy.
			*/
			Trigger* createTrigger(const NxString& identifier, ShapeBlueprint*, const Pose&, ActorParams = "");


			/**@brief Destroy a Trigger in the scene.

			   @note This function is the same as destroyActor.
			   
			   @warning <strong>Containers and Garbage Collection</strong> \n
			   	   Actor are used in containers. Creating a Actor through the Scene factory method makes the Actor "owned"
			       by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
			       Manually creating the Scene "new Actor()" will only add a presence, it is up to you to delete it at the
			       correct time.
			   
			   @param identifier		Unique name of the Body to destroy.
			 */ 
			void	 destroyTrigger(const NxString&);
			
			/** @brief Returns a pointer to the Trigger

				In the case of a unknown actor a NULL pointer is return. Identifiers are case-sensitve.

			   	@param identifier		Unique name of the Actor to fetch.
			   	@return Trigger			The Trigger or NULL.
			 */	  
			Trigger* getTrigger(const NxString&);

			/**	 @brief Get's all Trigger in the Scene.

				 Returns a container of of the pointers of all the Triggers in this Scene. Since your working with the 
				 "Master" list of triggers, you should be careful when handling with those Triggers during a FrameSimulation. 
				 When when adding or cloning Triggers within a loop with the Master Loop may make the application freeze.
			   
			   	 @param Actors The Master list of Triggers.
			 */	  	
			Triggers* getTriggers();
			

			/**@brief Gets the number of Actors in the scene.
		       @return NxU32 Total count of NxOgre Triggers in this Scene.
			 */ 
			NxU32	 getNbTriggers();

			//////////////////////////////////////////////////////////////////////////////

			/**	@brief Create a spherical joint between two actors
				@param Actor		First Actor
				@param Actor		Second Actor
				@param anchor		Global Anchor of the joint
				@param JointParams	Parameters for the Joint
				@return The joint
			*/
			SphericalJoint*		createSphericalJoint(Actor*, Actor*, const Ogre::Vector3 &anchor, JointParams = JointParams());
			
			/**	@brief Create a spherical joint between an actor and the world
				@param Actor		First Actor
				@param anchor		Global Anchor of the joint
				@param JointParams	Parameters for the Joint
				@return The joint
			*/
			SphericalJoint*		createSphericalJoint(Actor*, const Ogre::Vector3 &anchor, JointParams = JointParams());

			/**	@brief Create a spherical joint between two actors
				@param Actor		First Actor
				@param Actor		Second Actor
				@param axis			Axis of the joint
				@param anchor		Global Anchor of the joint
				@param JointParams	Parameters for the Joint
				@return The joint
			*/
			RevoluteJoint*		createRevoluteJoint(Actor*, Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
			
			
			/**	@brief Create a revolute joint between an actor and the world
				@param Actor		First Actor
				@param axis			Axis of the joint
				@param anchor		Global Anchor of the joint
				@param JointParams	Parameters for the Joint
				@return The joint
			*/
			RevoluteJoint*		createRevoluteJoint(Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
		
			
			/**	@brief Create a prismatic joint  between two actors
				@param Actor		First Actor
				@param Actor		Second Actor
				@param axis			Axis of the joint
				@param anchor		Global Anchor of the joint
				@param JointParams	Parameters for the Joint
				@return The joint
			*/
			PrismaticJoint*		createPrismaticJoint(Actor*, Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
			
			
			/**	@brief Create a prismatic joint between an actor and the world
				@param Actor		First Actor
				@param axis			Axis of the joint
				@param anchor		Global Anchor of the joint
				@param JointParams	Parameters for the Joint
				@return The joint
			*/
			PrismaticJoint*		createPrismaticJoint(Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
	
			
			/**	@brief Create a cylindrical joint between two actors
				@param Actor		First Actor
				@param Actor		Second Actor
				@param axis			Axis of the joint
				@param anchor		Global Anchor of the joint
				@param JointParams	Parameters for the Joint
				@return The joint
			*/
			CylindricalJoint*	createCylindricalJoint(Actor*, Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
			
			
			/**	@brief Create a cylindrical joint between an actor and the world
				@param Actor		First Actor
				@param axis			Axis of the joint
				@param anchor		Global Anchor of the joint
				@param JointParams	Parameters for the Joint
				@return The joint
			*/
			CylindricalJoint*	createCylindricalJoint(Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
		
					
			/**	@brief Create a fixed joint between two actors
				@param Actor		First Actor
				@param Actor		Second Actor
				@param JointParams	Parameters for the Joint
				@return The joint
			*/
			FixedJoint*			createFixedJoint(Actor*, Actor*, JointParams = JointParams());
		
					
			/**	@brief Create a fixed joint between two actors
				@param Actor		First Actor
				@param JointParams	Parameters for the Joint
				@return The joint
			*/
			FixedJoint*			createFixedJoint(Actor*, JointParams = JointParams());
			

			//////////////////////////////////////////////////////////////////////////////

			/** @brief Destroys a joint 
				@param Joint The Joint to destroy
			*/
			void releaseJoint(Joint*);

			/** @brief Destroys a joint 
				@param NxJoint The ID of the joint to destroy
			*/
			void releaseJoint(NxJointID);

			//////////////////////////////////////////////////////////////////////////////

#if (NX_USE_LEGACY_CHARACTER_API == 0)

			/** @brief Creates a Character (A raycasted kinematic actor intended to simulate human characters)
				
				@param identifier	The identifier of the character
				@param pose			The pose to place the character
				@param params		Character params for the character
			
				@see Character::Character
				@see CharacterParams
				@return A fresh new, unpacked, with that new car-smell character
			*/
			Character* createCharacter(const NxString &identifier, Pose pose, CharacterModel*, CharacterParams params);


			/** @brief Destroys a character from the Scene
				@param identifier	The character to destroy
			*/
			void  destroyCharacter(const NxString& identifer);

#else
#if (NX_USE_CHARACTER_API)

			/** @brief Creates a Character (A raycasted kinematic actor intended to simulate human characters)
				
				@param identifier	The identifier of the character
				@param pose			The pose to place the character
				@param params		Character params for the character
			
				@see Character::Character
				@see CharacterParams
				@return A fresh new, unpacked, with that new car-smell character
			*/
			Character* createCharacter(const NxString &identifier, Pose pose, CharacterParams params);


			/** @brief Destroys a character from the Scene
				@param identifier	The character to destroy
			*/
			void  destroyCharacter(const NxString& identifer);
#endif
#endif

			//////////////////////////////////////////////////////////////////////////////

			/**	@brief Creates a material for a shape
				@params identifier The identifier of the material
				@return The material
			*/
			Material*		createMaterial(const NxString& identifier);

			/**	@brief Destroys a material from a scene.
				@note This will destroy the material from the Scene class, but any Shapes using the material
				      index will not change. In most cases it is better to leave the material until the very
					  end of the scene.
				@identifier The material name to delete
			*/
			void			destroyMaterial(const NxString& identifier);
			
			/** @brief Gets a material from it's identifier
				@return the material, or a null pointer if the material does not exist.
			*/
			Material*		getMaterial(const NxString& identifier);
			
			/** @brief Gets a material from it's material index
				@return the material, or a null pointer if the material does not exist.
			*/
			Material*		getMaterial(NxMaterialIndex);
			
			/** @brief Gets all materials in the scene
				@return All of the materials
			*/
			MaterialList*	getMaterials();
			
			/** @brief Gets the total number of materials in the scene
				@return The number of materials
			*/
			NxU32			getNbMaterials();
			
			/** @brief Gets a material index based of the material's identifier
				@return The material index, or 0 if the material does not exist.
			*/
			NxMaterialIndex getMaterialIndex(const NxString& identifier);

			
			void addMaterialPair(Ogre::Material*, NxOgre::Material*);
			void addMaterialPair(const NxString& ogre, const NxString& nxogre);
			void addMaterialPairsFromFile(const NxString& ogre, const NxString& nxogre);

			//////////////////////////////////////////////////////////////////////////////

			ActorGroup*	createActorGroup(const NxString& name);
			void		destroyActorGroup(const NxString& name);
			NxU32		getNbActorGroups() const;
			ActorGroups* getActorGroups();
			ActorGroupsByIndex* getActorGroupsByIndex();
			ActorGroup*	getActorGroup(const NxString& name);

			//////////////////////////////////////////////////////////////////////////////
			
			ShapeGroup*	createShapeGroup(const NxString& name);
			void		destroyShapeGroup(const NxString& name);
			NxU32		getNbShapeGroups() const;
			ShapeGroups* getShapeGroups();
			ShapeGroupsByIndex* getShapeGroupsByIndex();
			ShapeGroup*	getShapeGroup(const NxString& name);

			//////////////////////////////////////////////////////////////////////////////
			
#if (NX_SDK_VERSION_NUMBER >= 272) 
			DominanceGroup*	createDominanceGroup(const NxString& name);
			void		setDominanceGroupPair(const NxString& AGroup, const NxString& BGroup, NxReal ADominance, NxReal BDominance);
			void		setDominanceGroupPair(DominanceGroup* AGroup, DominanceGroup* BGroup, NxReal ADominance, NxReal BDominance);
			void		destroyDominanceGroup(const NxString& name);
			NxU32		getNbDominanceGroups() const;
			DominanceGroups* getDominanceGroups();
			DominanceGroupsByIndex* getDominanceGroupsByIndex();
			DominanceGroup*	getDominanceGroup(const NxString& name);
#endif
			//////////////////////////////////////////////////////////////////////////////

			void addFloor();

			//////////////////////////////////////////////////////////////////////////////

			// setStaticGeometry
			//  All future Actors with visualisations that are static, will now use the StaticGeometry
			//  Remember to build the static-geometry after adding all your static actors.
			// @param Ogre::StaticGeometry Current StaticGeometry to work with.
			void setStaticGeometry(Ogre::StaticGeometry*);

			// getStaticGeometry
			//  Returns the current StaticGeometry
			Ogre::StaticGeometry* getStaticGeometry();
			
			// removeStaticGeometry
			//  Removes the StaticGeometry pointer from the Scene. It does not adjust the static actors
			//  or deletes the StaticGeometry, it just sets the Scene's pointer to your StaticGeometry
			//  to null.
			void removeStaticGeometry();

			//////////////////////////////////////////////////////////////////////////////
		
#if (NX_USE_FLUID_API == 1)

			Fluid*	createFluid(const Pose&, FluidParams);
			void	destroyFluid(const NxString&);
			NxU32	getNbFluids()	const;
			Fluids*	getFluids();

#endif

			//////////////////////////////////////////////////////////////////////////////
		
#if (NX_USE_CLOTH_API == 1)
			Actor*	createDeformableActor(const NxString& meshName, ShapeBlueprint* roughShape, const Pose&, ActorParams, ClothParams, NxReal impulseThreshold, NxReal penetrationDepth = 0.0f);
			Cloth*	createCloth(const NxString& identifier, const Pose&, ClothParams);
			Cloth*	createCloth(const NxString& identifier, const NxString& meshName, const Pose&, ClothParams);
			//Cloth*	createCloth(const NxString& identifier, NxClothDesc, NxMeshData, Ogre::MeshPtr, const Pose&);
			void	destroyCloth(const NxString&);
			Cloths*	getCloths();
			NxU32 getNbCloths();
#endif
	
			//////////////////////////////////////////////////////////////////////////////

#if (NX_USE_SOFTBODY_API == 1)
			SoftBody*	createSoftBody(const NxString& TetAndMeshFilePrefix, const Pose&, SoftBodyParams);
			SoftBody*	createSoftBody(NxReal width, NxReal height, NxReal, const Pose&, SoftBodyParams);
			void		destroySoftBody(const NxString& identifier);
			NxU32		getNbSoftBodies();
			SoftBodies*	getSoftBodies();
#endif
	
			//////////////////////////////////////////////////////////////////////////////

#if (NX_USE_FORCEFIELD_API == 1)
			ForceField*		createForceField(const NxString& identifier, const Pose&, ShapeBlueprint* actorShape, ActorParams, ForceFieldParams);
			void			destroyForceField(const NxString&);
			NxU32			getNbForceFields();
			ForceFields*	getForceFields();
#endif

			//////////////////////////////////////////////////////////////////////////////

			void			registerMachine(Machine*);
			void			unregisterMachine(Machine*);

			//////////////////////////////////////////////////////////////////////////////

			void		idle();

		protected:

			/**  @brief Advances the state of the Scene
				 @note Do not control by hand. World will automatically perform this operation for you. 
			     @param time	Time to advance since last Simulation		
			 */  	
			void simulate(NxReal time);


			/** @brief Reflects the changes of the simulation to Ogre
				@see SceneRenderer
			 */
			void render();


			//////////////////////////////////////////////////////////////////////////////

			void		setName(const NxString& s);

			void		_paramsToDescription(SceneParams);
			void		_createDefaultActorGroup();
			void		_createDefaultShapeGroup();
			void		_createDefaultDominanceGroup();

			void		_createDefaultMaterial(NxReal r, NxReal d, NxReal s);

			void		_registerActor(const NxString& name, Actor*);
			void		_unregisterActor(const NxString&);

			void		_registerActorGroup(const NxString& name, ActorGroup*);
			void		_unregisterActorGroup(const NxString& name);

			void		_registerMaterial(const NxString& name, Material*);
			void		_unregisterMaterial(const NxString&);

			void		_registerShapeGroup(const NxString& name, ShapeGroup*);
			void		_unregisterShapeGroup(const NxString& name);

#if (NX_SDK_VERSION_NUMBER >= 272) 
			void		_registerDominanceGroup(const NxString& name, DominanceGroup*);
			void		_unregisterDominanceGroup(const NxString& name);
#endif

			void		_registerTrigger(const NxString& name, Trigger*);
			void		_unregisterTrigger(const NxString& name);

#if (NX_USE_CHARACTER_API == 1)
			void		_registerCharacter(const NxString& name, Character*);
			void		_unregisterCharacter(const NxString&);
#endif

#if (NX_USE_CLOTH_API == 1)
			void		_registerCloth(const NxString& name, Cloth*);
			void		_unregisterCloth(const NxString&);
#endif

#if (NX_USE_FLUID_API == 1)
			void		_registerFluid(const NxString& name, Fluid*);
			void		_unregisterFluid(const NxString&);
#endif
			
#if (NX_USE_SOFTBODY_API == 1)
			void		_registerSoftBody(const NxString& name, SoftBody*);
			void		_unregisterSoftBody(const NxString&);
#endif

#if (NX_USE_FORCEFIELD_API == 1)
			void		_registerForceField(const NxString& name, ForceField*);
			void		_unregisterForceField(const NxString&);
#endif
			//////////////////////////////////////////////////////////////////////////////

			World*							mOwner;
			Ogre::SceneManager*				mSceneMgr;

			//////////////////////////////////////////////////////////////////////////////

			NxSceneDesc						mDescription;
			NxScene*						mScene;
			NxString						mName;
			const char*						mNxID;

			//////////////////////////////////////////////////////////////////////////////

			Actors							mActors;
			ActorGroups						mActorGroups;
			ActorGroupsByIndex				mActorGroupsByIndex;
			Joints							mJoints;
			Triggers						mTriggers;


#if (NX_USE_CLOTH_API == 1)
			Cloths							mCloths;
#endif

#if (NX_USE_CHARACTER_API == 1)
			Characters						mCharacters;
#endif

#if (NX_USE_FLUID_API == 1)
			Fluids							mFluids;
#endif

#if (NX_USE_FORCEFIELD_API == 1)
			ForceFields						mForceFields;
#endif

#if (NX_USE_SOFTBODY_API == 1)
			SoftBodies						mSoftBodies;
#endif

			MaterialList					mMaterials;
			MaterialListByIndex				mMaterialsByIndex;
			MaterialPair					mMaterialPair;
			Machines						mMachines;

			ShapeGroups						mShapeGroups;
			ShapeGroupsByIndex				mShapeGroupsByIndex;

#if (NX_SDK_VERSION_NUMBER >= 272) 
			DominanceGroups					mDominanceGroups;
			DominanceGroupsByIndex			mDominanceGroupsByIndex;
#endif

			NxReal							mControllers_MaxTimestep;
			NxTimeStepMethod				mControllers_Method;
			NxU32							mControllers_MaxIter;
			NxU32							mControllers_NumSubSteps;

			//////////////////////////////////////////////////////////////////////////////

			SceneController*				mSceneController;
			SceneRenderer*					mSceneRenderer;

			SceneContactController*			mSceneContactController;
			SceneTriggerController*			mSceneTriggerController;
			
			//////////////////////////////////////////////////////////////////////////////

			RayCaster*						mSceneRayCaster;
			Intersection*					mSceneIntersection;

			//////////////////////////////////////////////////////////////////////////////

			Ogre::StaticGeometry*			mStaticGeometry;

		

		private:

			struct BatchActorBlueprint {
				NxString			identifierPattern;
				NxString			visualData;
				NxU32				nbActors;
				ShapeBlueprint*		collisionModel;
				Pose				poseOrigin;
				Pose				poseDelta;
				ActorParams			params;
			};

			std::vector< std::pair< BatchActionTimeFrame, BatchActorBlueprint > > mBatchActorCreation;
			std::vector< std::pair< BatchActionTimeFrame, Actors > > mBatchActorDestruction;

			bool mBatchProcessSimulate;
			
			void _batchActorCreate(BatchActorBlueprint&);
			void _batchBodyCreate(BatchActorBlueprint&);
			void _batchActorDestroy(Actors&);

	};

};

#endif
