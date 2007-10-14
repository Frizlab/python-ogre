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

			/**	floor
				 An infinite ground plane on the +Y axis.
				@default false;
			 */
			bool				floor;

			/** gravity
				 Specificies the gravity in a scene (if there is any).
				@default 0 -9.80665 0
			 */
			NxVec3				gravity;

			/** defaultMaterialRestitution
				 The Resitituion (Bounce) value of the default material.
				@default 0
			 */
			NxReal				defaultMaterialRestitution;
			NxReal				defaultMaterialDynamicFriction;
			NxReal				defaultMaterialStaticFriction;
			NxReal				max_timestep;
			NxU32				max_iter;
			NxBounds3*			max_bounds;
			NxSceneLimits*		limits;
			NxSimulationType	sim_type;
			bool				bounds_planes;
			NxU32				flags;
			NxU32				internal_thread_count;
			NxU32				background_thread_count;
			NxUserScheduler*	custom_scheduler;
			NxU32				thread_mask;
			NxU32				background_thread_mask;
			NxU32				sim_thread_stack_size;
			NxThreadPriority	sim_thread_priority;
			NxU32				worker_thread_stack_size;
			NxThreadPriority	worker_thread_priority;
			NxU32				up_axis;
			NxU32				subdivision_level;
			NxPruningStructure	static_structure;
			NxPruningStructure	dynamic_structure;
			
			enum scenecontroller {
				CN_FIXED,
				CN_VARIABLE
#if (NX_UNSTABLE == 1)
				,CN_SUPER_FIXED
				,CN_ASYNC
#endif
			};

			scenecontroller		time_step_method;

	};

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

			enum BatchActionTimeFrame {
				BT_NOW,
				BT_NEXT_AVAILABLE_BATCH,
				BT_NEXT_SIMULATION,
				BT_WITHIN_SECONDS,
				BT_NEVER
			};

			/**  Scene
			   	 A Scene is a sub-section of a World. Although a Scene is infinite in size like World. Scenes are 
			     considered to be seperate from each other. Actors from one scene cannot interact with ones from
			     another. Scenes have also their own Materials, Groups, etc. which cannot be shared.
			   
			    Containers and Garbage Collection
			   	 Scenes are used in containers. Creating a Scene through the World factory method makes the Scene "owned"
			     by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
			     Manually creating the Scene "new Scene()" will only add a presence, it is up to you to delete it at the
			     correct time.
			   
			    @params	Identifier				Unique identifier of the Scene
			   	@params Ogre::SceneManager*		SceneManger for this Scene to Use.
			    @params World*					World pointer which would contain the Scene.
			    @params SceneParams				SceneParams for this Scene to use.
			   
			    @see World::createScene
			    @see SceneParams
			*/  	
			Scene(const NxString& identifier, Ogre::SceneManager*, World*, SceneParams = "");


			/** Alternate Scene Constructor
				Used for to bind a Scene to a previously made NxScene.
					
				@params	Identifier				Unique identifier of the Scene
			   	@params Ogre::SceneManager*		SceneManger for this Scene to Use.
			    @params World*					World pointer which would contain the Scene.
			    @params NxScene*				NxScene to bind to.
			   
			    @see World::createScene
			    @see SceneParams
			 */ 
			Scene(const NxString& identifier, World*, Ogre::SceneManager*, NxScene*);

			/** Scene Destructor
			   
			    Containers and Garbage Collection
			     If the Scene is owned by NxOgre use World::SceneDestroy, if not then use "delete mScene".
			   
			    @see World::destroyScene
			*/
			~Scene();

			/**  getName()
			     Returns the name of the Scene.
			   
			     @return NxString Scene identifier
			 */
			NxString getName()								{
																return mName;
															}

			/**  getSceneManager
			     Returns the SceneManager instance the Scene is using
			   
			     @return SceneManager SceneManager pointer.
			 */
			Ogre::SceneManager* getSceneManager()			{
																return mSceneMgr;
															}

			/**  getNxScene
			     Returns a copy of a pointer of the NxScene which the Scene works with.
			   
			     @return NxScene The NxScene's pointer
			 */
			NxScene* getNxScene()							{
																return mScene;
															}

			/** getWorld
			    Returns a copy of the World which the Scene is part of.
			   
			    @return World The World itself.
			 */
			World* getWorld()								{
																return mOwner;
															}


			/**	setSceneController
			   	 Sets the Controller used for Simulation and Rendering of the NxScene. These can be Variable, Fixed,
			   	 or SuperFixed. You can use your own SceneController if you wish, otherwise the SceneController should
			     be specified in the SceneParams "time-step-method" with Fixed, Variable or SuperFixed as an option.
			   
			   	Garbage Collection
			   	 SceneController instance will be deleted on the destruction of the Scene.	
			   
			     @params SceneController Instance of the SceneController to use.
			 */  	
			void setSceneController(SceneController*);
			

			/**  Simulate
			   	  Advances the Simulation of the Scene, The World will automatically perform this operation for you. 
			   
			     @params NxReal(Time/Seconds)	Time to advance/Time since last Simulation		
			 */  	
			void simulate(NxReal);


			/**  Render
			   	  Moves and Orientates all Scene Nodes, Update Meshes and anything related to updating the visual
			   	  aspect of the Scene. World will automatically perform this operation for you.
			   
			      @params NxReal(Time/Seconds)	Time to advance/Time since last Render
			 */   	
			void render(NxReal);


			/**	 save
				  Save the current Scene and contents to a SceneBlueprint
			 */
			Blueprints::SceneBlueprint  save();

			/**	 getSimType
			   	  Returns the NxSimulationType (Hardware/Software) of the Scene.
			   
			   	 @returns NxSimulationType		Simulation Type
			 */	 	
			NxSimulationType getSimType() const;


			/**	  setGravity
				   Sets the gravitional constant for the Scene. 
			
				  @params	Ogre::Vector3(Acceleration/metres per second)	Gravitional Constant
			 */  	
			void setGravity(const Ogre::Vector3&);


			/**   getGravity
			   	   Gets the gravitional constant for the Scene. 
			   
			   	  @return	Ogre::Vector3(Acceleration/metres per second)	Gravitional Constant
			 */	  	
			Ogre::Vector3 getGravity() const;


			/**	  createActor
			   	   Text
			   
			   	  Containers and Garbage Collection
			   	   Text
			   
			   	  @params identifier		Unique name of the Actor. (Leave blank for a auto-generated identifier)
			   	  @params ShapeBlueprint*	The first shape to use for the Actor (new ShapeName(...))
			   	  @params Pose				Global Pose of the Actor
			   	  @params ActorParams		ActorParams for this actor
			   
			   	 @return The created Actor
			 */	 	
			Actor*	createActor(const NxString& identifier, ShapeBlueprint*, const Pose&, ActorParams = ActorParams());


			// Ex
			void	batchCreateActors(const NxString& identifierPattern, NxU32 nbActors, ShapeBlueprint*, const Pose& poseOrigin, const Pose& poseDifference, BatchActionTimeFrame = BT_NOW, ActorParams = ActorParams());
			
			/**   destroyActor
			   	   Text
			   
			   	  Containers and Garbage Collection
			   	   Text
			   
			   	  @params identifier		Unique name of the Actor to destroy.
			 */	  	
			void	destroyActor(const NxString&);


			// Ex
			void	batchDestroyActor(const NxString& identifier, BatchActionTimeFrame = BT_NEXT_AVAILABLE_BATCH);
			
			// Ex
			void	batchDestroyActors(const NxString& identifierPattern, NxU32 nbActors, BatchActionTimeFrame = BT_NEXT_AVAILABLE_BATCH);
	
			// Ex
			void	batchDestroyActors(Actors, BatchActionTimeFrame = BT_NEXT_AVAILABLE_BATCH);

			/**   	  getActor
			   	   Returns a pointer to the Actor. If the Actor does not exist, then the Pointer value is NULL.
			   
			   	  @params identifier		Unique name of the Actor to fetch.
			   	  @return Actor				The Actor or NULL.
			 */	  	
			Actor*	getActor(const NxString&);


			/**	  getActors
			   	   Returns a pointer to the list of Actors in this Scene. Since your working with the "Master" list
			   	   of actors, you should be careful when handling with those Actors during a FrameSimulation. When
			   	   when adding or cloning Actors within a loop with the Master Loop may make the application freeze.
			   
			   	  @Hint
			   	   Use "Actors actor; mScene->getActors()->copy(actors);" to create a copy of the list of Actors.
			   
			   	  @params Actors The Master list of Actors.
			 */	  	
			Actors*	getActors();


			// Ex
			Actors getActorsFromIdentifier(NxStringOperator, const NxString& identifierPattern);
			
			// Ex
			Actors getActorsFromFrame(NxComparisonOperator, NxU32 frame);

			// Ex
			Actors& getActorsFromRegion(SimpleShape*, NxShapesType);
			
			// Ex
			Actors& getActorsFromLastRegion(NxShapesType);

			// Ex
			Actors getActorsFromRaycast(Ogre::Vector3 position, Ogre::Vector3 normal, NxReal range);

			/**   getNbActors
			   	   Gets the number of NxOgre Actors within the Scene. This does not keep account of NxActors with
			   	   a different UserData class such as Triggers or ForceFields, or manually created NxActors.
			   
			   	  @params NxU32		Total count of NxOgre Actors in this Scene.
			 */  	  	
			NxU32	getNbActors() const;


			/**   	   createBody
			   		Creates a Body (An Actor with an SceneNode) within a Scene. See Scene::createActor for more information.
			   
			   	  Containers and Garbage Collection
			   	   Text
			   
			   	  @params meshIdentifier	Mesh of the first Entity in the SceneNode to use. (Leave blank for no mesh)
			   	  @see	  Scene::createActor for other Arguments.
			   
			   	 @return The created Body
			  */
			Body*	createBody(const NxString& meshIdentifier, ShapeBlueprint*, const Pose&, ActorParams = "");
			
			// Ex
			void	batchCreateBodies(const NxString& identifierPattern, const NxString& visualData, NxU32 nbActors, ShapeBlueprint*, const Pose& poseOrigin, const Pose& poseDifference, BatchActionTimeFrame = BT_NOW, ActorParams = ActorParams());
			

			/**   destroyBody
			   	   Destroys a Body. This function is the same as destroyActor.
			   
			   	  Containers and Garbage Collection
			   	   Text
			   
			   	  @params identifier		Unique name of the Body to destroy.
			 */  	
			void	destroyBody(const NxString&);
			
			//////////////////////////////////////////////////////////////////////////////
			
			/**   createTrigger
			   	   Text
			   
			   	  Containers and Garbage Collection
			   	   Text
			   
			   	  @params identifier		Unique name of the Body to destroy.
			 */
			Trigger* createTrigger(const NxString& identifier, ShapeBlueprint*, const Pose&, ActorParams = "");


			/**   destroyTrigger
			   	   Destroys a Trigger. This function is the same as destroyActor.
			   
			   	  Containers and Garbage Collection
			   	   Text
			   
			   	  @params identifier		Unique name of the Body to destroy.
			 */
			void	 destroyTrigger(const NxString&);
			
			Trigger* getTrigger(const NxString&);
			Triggers* getTriggers();
			NxU32	 getNbTriggers();

			//////////////////////////////////////////////////////////////////////////////

			SphericalJoint*		createSphericalJoint(Actor*, Actor*, const Ogre::Vector3 &anchor, JointParams = JointParams());
			SphericalJoint*		createSphericalJoint(Actor*, const Ogre::Vector3 &anchor, JointParams = JointParams());

			RevoluteJoint*		createRevoluteJoint(Actor*, Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
			RevoluteJoint*		createRevoluteJoint(Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
		
			PrismaticJoint*		createPrismaticJoint(Actor*, Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
			PrismaticJoint*		createPrismaticJoint(Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
	
			CylindricalJoint*	createCylindricalJoint(Actor*, Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
			CylindricalJoint*	createCylindricalJoint(Actor*, const Ogre::Vector3 &axis, const Ogre::Vector3 &anchor, JointParams = JointParams());
		
			FixedJoint*		createFixedJoint(Actor*, Actor*, JointParams = JointParams());
			FixedJoint*		createFixedJoint(Actor*, JointParams = JointParams());
			

			//////////////////////////////////////////////////////////////////////////////

			void releaseJoint(Joint*);
			void releaseJoint(NxJointID);

			//////////////////////////////////////////////////////////////////////////////

#if (NX_USE_CHARACTER_API == 1)
			Character* createCharacter(const NxString &name, Pose p, CharacterParams cp);
			void  destroyCharacter(const NxString&);
#endif

/*
			NxEffector*	createEffector(const NxEffectorDesc&);
			void	releaseEffector(NxEffector&);
*/
			//////////////////////////////////////////////////////////////////////////////

			Material*		createMaterial(const NxString& name);
			void			destroyMaterial(const NxString& name);
			Material*		getMaterial(const NxString&);
			Material*		getMaterial(NxMaterialIndex);
			MaterialList*	getMaterials();
			NxU32			getNbMaterials();
			NxMaterialIndex getMaterialIndex(const NxString&);

			
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

			/*
			NxCompartment* createCompartment(const NxCompartmentDesc &compDesc);
			NxU32 getNbCompartments();
			NxU32 getCompartmentArray(NxCompartment ** userBuffer, NxU32 bufferSize, NxU32 & usersIterator);
*/
			//////////////////////////////////////////////////////////////////////////////
/*
			void setActorPairFlags(NxActor& actorA, NxActor& actorB, NxU32 nxContactPairFlag);
			NxU32 getActorPairFlags(NxActor& actorA, NxActor& actorB) const;
*/
			//////////////////////////////////////////////////////////////////////////////
/*
			void setShapePairFlags(NxShape& shapeA, NxShape& shapeB, NxU32 nxContactPairFlag) ;
			NxU32 getShapePairFlags(NxShape& shapeA, NxShape& shapeB) const ;
			NxU32 getNbPairs() const;
			NxU32 getPairFlagArray(NxPairFlag* userArray, NxU32 numPairs) const;

			void setGroupCollisionFlag(NxCollisionGroup group1, NxCollisionGroup group2, bool enable);
			bool getGroupCollisionFlag(NxCollisionGroup group1, NxCollisionGroup group2) const;

			void setActorGroupPairFlags(NxActorGroup group1, NxActorGroup group2, NxU32 flags);
			NxU32 getActorGroupPairFlags(NxActorGroup group1, NxActorGroup group2) const;
			NxU32 getNbActorGroupPairs() const;
			NxU32 getActorGroupPairArray(NxActorGroupPair * userBuffer, NxU32 bufferSize, NxU32 & userIterator) const;
			void setFilterOps(NxFilterOp op0, NxFilterOp op1, NxFilterOp op2);
			void setFilterBool(bool flag);
			void setFilterConstant0(const NxGroupsMask& mask);
			void setFilterConstant1(const NxGroupsMask& mask);
			void getFilterOps(NxFilterOp& op0, NxFilterOp& op1, NxFilterOp& op2) const;

			bool getFilterBool() const;
			NxGroupsMask getFilterConstant0() const;
			NxGroupsMask getFilterConstant1() const;
*/
			//////////////////////////////////////////////////////////////////////////////

/*
			NxActor** getActors();
			NxActiveTransform* getActiveTransforms(NxU32 &nbTransformsOut);
			NxU32 getNbStaticShapes() const;
			NxU32 getNbDynamicShapes() const;
			NxU32 getTotalNbShapes() const;
			NxU32 getNbJoints()	const;
			void resetJointIterator();
			NxJoint* getNextJoint();
			NxU32 getNbEffectors() const;
			void resetEffectorIterator();
			NxEffector* getNextEffector();
			NxU32 getBoundForIslandSize(NxActor& actor); 
			NxU32 getIslandArrayFromActor(NxActor& actor, NxActor** userBuffer, NxU32 bufferSize, NxU32& userIterator);
*/

			//////////////////////////////////////////////////////////////////////////////
/*
			NxU32 getNbMaterials() const;
			NxU32 getMaterialArray(NxMaterial ** userBuffer, NxU32 bufferSize, NxU32 & usersIterator);
			NxMaterialIndex	 getHighestMaterialIndex() const;
			NxMaterial*	getMaterialFromIndex(NxMaterialIndex matIndex) ;
*/


			//////////////////////////////////////////////////////////////////////////////
			/*
			void setTiming(NxReal maxTimestep=1.0f/60.0f, NxU32 maxIter=8, NxTimeStepMethod method=NX_TIMESTEP_FIXED);
			void getTiming(NxReal& maxTimestep, NxU32& maxIter, NxTimeStepMethod& method, NxU32* numSubSteps=NULL) const;
			const NxDebugRenderable* getDebugRenderable();

			NxPhysicsSDK& getPhysicsSDK();*/


/*
			void getStats(NxSceneStats& stats) const;

#ifdef NX_ENABLE_SCENE_STATS2
			const NxSceneStats2* getStats2() const;
#endif

			void getLimits(NxSceneLimits& limits) const;
			void setMaxCPUForLoadBalancing(NxReal cpuFraction) ;
			NxReal getMaxCPUForLoadBalancing() ;
*/
			//////////////////////////////////////////////////////////////////////////////

/*
			void setUserNotify(NxUserNotify* callback);
			NxUserNotify* getUserNotify() const;

#if (NX_USE_FLUID_API == 1)

			void setFluidUserNotify(NxFluidUserNotify* callback);
			NxFluidUserNotify* getFluidUserNotify() const;

#endif //NX_USE_FLUID_API

			void setUserContactModify(NxUserContactModify* callback);
			NxUserContactModify* getUserContactModify() const;
			void setUserTriggerReport(NxUserTriggerReport* callback);
			NxUserTriggerReport* getUserTriggerReport() const ;

			void setUserContactReport(NxUserContactReport* callback);
			NxUserContactReport*		getUserContactReport() const;
*/
			//////////////////////////////////////////////////////////////////////////////
			/*
			bool raycastAnyBounds(const NxRay& worldRay, NxShapesType shapesType, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, const NxGroupsMask* groupsMask=NULL) const;
			bool raycastAnyShape(const NxRay& worldRay, NxShapesType shapesType, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, const NxGroupsMask* groupsMask=NULL, NxShape** cache=NULL) const;
			NxU32 raycastAllBounds(const NxRay& worldRay, NxUserRaycastReport& report, NxShapesType shapesType, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask* groupsMask=NULL) const;
			NxU32 raycastAllShapes(const NxRay& worldRay, NxUserRaycastReport& report, NxShapesType shapesType, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask* groupsMask=NULL) const;

			NxShape* raycastClosestBounds(const NxRay& worldRay, NxShapesType shapeType, NxRaycastHit& hit, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask* groupsMask=NULL) const;
			NxShape* raycastClosestShape(const NxRay& worldRay, NxShapesType shapeType, NxRaycastHit& hit, NxU32 groups=0xffffffff, NxReal maxDist=NX_MAX_F32, NxU32 hintFlags=0xffffffff, const NxGroupsMask* groupsMask=NULL, NxShape** cache=NULL) const;
*/
			//////////////////////////////////////////////////////////////////////////////
		/*
			NxU32 overlapSphereShapes(const NxSphere& worldSphere, NxShapesType shapeType, NxU32 nbShapes, NxShape** shapes, NxUserEntityReport<NxShape*>* callback, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL, bool accurateCollision=false);
			NxU32 overlapAABBShapes(const NxBounds3& worldBounds, NxShapesType shapeType, NxU32 nbShapes, NxShape** shapes, NxUserEntityReport<NxShape*>* callback, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL, bool accurateCollision=false);
			NxU32 overlapOBBShapes(const NxBox& worldBox, NxShapesType shapeType, NxU32 nbShapes, NxShape** shapes, NxUserEntityReport<NxShape*>* callback, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL, bool accurateCollision=false);
			NxU32 overlapCapsuleShapes(const NxCapsule& worldCapsule, NxShapesType shapeType, NxU32 nbShapes, NxShape** shapes, NxUserEntityReport<NxShape*>* callback, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL, bool accurateCollision=false);
*/
			//////////////////////////////////////////////////////////////////////////////
		/*
#if NX_SUPPORT_SWEEP_API

			NxSweepCache* createSweepCache();
			void releaseSweepCache(NxSweepCache* cache);
			NxU32 linearOBBSweep(const NxBox& worldBox, const NxVec3& motion, NxU32 flags, void* userData, NxU32 nbShapes, NxSweepQueryHit* shapes, NxUserEntityReport<NxSweepQueryHit>* callback, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL);
			NxU32 linearCapsuleSweep(const NxCapsule& worldCapsule, const NxVec3& motion, NxU32 flags, void* userData, NxU32 nbShapes, NxSweepQueryHit* shapes, NxUserEntityReport<NxSweepQueryHit>* callback, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL);
#endif
*/
			//////////////////////////////////////////////////////////////////////////////
		/*
			NxU32 cullShapes(NxU32 nbPlanes, const NxPlane* worldPlanes, NxShapesType shapeType, NxU32 nbShapes, NxShape** shapes, NxUserEntityReport<NxShape*>* callback, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL);
			bool checkOverlapSphere(const NxSphere& worldSphere, NxShapesType shapeType=NX_ALL_SHAPES, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL);
			bool checkOverlapAABB(const NxBounds3& worldBounds, NxShapesType shapeType=NX_ALL_SHAPES, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL);
			bool checkOverlapOBB(const NxBox& worldBox, NxShapesType shapeType=NX_ALL_SHAPES, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL);
			bool checkOverlapCapsule(const NxCapsule& worldCapsule, NxShapesType shapeType=NX_ALL_SHAPES, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL);
*/
			//////////////////////////////////////////////////////////////////////////////
		
#if (NX_USE_FLUID_API == 1)

			Fluid*	createFluid(const Pose&, FluidParams);
			void	destroyFluid(const NxString&);
			NxU32	getNbFluids()	const;
			Fluids*	getFluids();
			/*
			NxFluid* createFluid(const NxFluidDescBase& fluidDesc);
			void releaseFluid(NxFluid& fluid);
			
			NxFluid** getFluids();
			bool cookFluidMeshHotspot(const NxBounds3& bounds, NxU32 packetSizeMultiplier, NxReal restParticlesPerMeter, NxReal kernelRadiusMultiplier, NxReal motionLimitMultiplier, NxReal collisionDistanceMultiplier);
*/
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

			void		_registerHelper(const NxString& name, Helper*);
			void		_unregisterHelper(const NxString&);

			void		idle();

		protected:

			//////////////////////////////////////////////////////////////////////////////

			void		setName(const NxString& s) {mName = s;}

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
		
			Helpers							mHelpers;

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
