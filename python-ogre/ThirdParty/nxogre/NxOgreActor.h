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

#ifndef __NXOGRE_ACTOR_H__
#define __NXOGRE_ACTOR_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreContainer.h"		// For: mCollisionList
#include "NxOgreParams.h"

namespace NxOgre {

	/**
	 	\page actorparams ActorParams
		
		String base params are case and space insensitive.

		Examples:
		
		<code>
			"mass: 1"

			"mass: 1, group: myActors"

			"mass: 1, group: myActors, lineardamping: 3"
		</code>

		Non-String based params can be used as well, and offer access to some variables of the params
		that cannot be converted into a string (pointers, etc.). 

		Due to efficency purposes and insane string conversion. With string parameters more than four, 
		you should work with the non-string based params system. 

		<code>
			ActorParams params;
			params.setToDefault();
			params.mMass = 10;
			params.mGroupAsIndex = 1;
		</code>

		Is exactly the same as:

		<code>
			"mass: 10, group-index: 1"
		</code>
		
		Current Supported String Params
		-----------------------------
		
		-	density (float)
			- Description: Density of this Actor. Use this or mass, do not use both.
			- Example: "Density: 1.5"
			- See: ActorParams::mDensity and NxActorDesc::density for more information.	
	
		-	group	(string)
			- Description: Actor Group name of this actor. Use this or group-index, but not both.
			- Example: "group: myGroup"
	 		- See: ActorParams::mGroupAsName
	 	
	 	-	group-index	(unsigned int)
	 		- Description: Actor Group ID of this actor. Use this or group, but not both.
	 		- Example: "group-index: 0"
	 		- See: ActorParams::mGroupAsIndex
	 	
	 	-	dominancegroup (unsigned int)
	 		- Description: Dominance Group ID of this actor.
	 		- Example:	"dominancegroup: 0"
	 		- See:	ActorParams::mDominanceGroup
	 	
	 	-	massspaceinertia (Vector3)
	 	 	- Description: Mass Space Inertia of this Actor
	 	 	- Example: "massspaceinertia: 1 0 1"
	 	 	- See:	ActorParams::mMassSpaceInertia
	 	 
	 	-	linearvelocity (Vector3)
	 	 	- Description: Linear Velocity of this Actor
	 	 	- Example: "linearvelocity: 1 0 1"
	 	 	- See:	ActorParams::mLinearVelocity
	 	 	
	 	-	angularvelocity (Vector3)
	 	 	- Description: Angular Velocity of this Actor
	 	 	- Example: "angularvelocity: 1 0 1"
	 	 	- See:	ActorParams::mAngularVelocity
	 	 	
	 	-	wakeupcounter (float)
	 	 	- Description: Wake up time counter
	 	 	- Example: "wakeupcounter: 1000.0"
	 	 	- See:	ActorParams::mWakeupCounter
	 	 		
		-	mass (float)
			- Description: Mass of this Actor. Use this or density, do not use both. 
			- Example: "mass: 10"	
			- See: ActorParams::mMass and NxBodyDesc::mass for more information.
			
		-	lineardamping (float)
			- Description: Linear Damping of the Actor
			- Example: "lineardamping: 0.1"
			- See:	ActorParams::mLinearDamping
		
		-	angulardamping (float)
			- Description: Angular Damping of the Actor
			- Example: "angulardamping: 0.1"
			- See:	ActorParams::mAngularDamping
				
		-	maxangularvelocity (float)
			- Description: Maximum allowed angular velocity
			- Example: "maxangularvelocity: 10"
			- See:	ActorParams::mMaxAngularVelocity
				
		-	sleepangularvelocity (float)
			- Description: Maximum angular velocity at which the Actor can go to sleep. 
			- Example: "sleepangularvelocity: 20"
			- See:	ActorParams:mSleepAngularVelocity
				
		-	ccdmotionthreshold (float)
			- Description: When CCD is globally enabled, it is still not performed if the 
						   motion distance of all points on the Actor is below this threshold. 
			- Example: "ccdmotionthreshold: 5"
			- See:	ActorParams::mCCDMotionThreshold
				
		-	solveriterationcount (unsigned int)
			- Description:   Number of solver iterations performed when processing joint/contacts
							 connected to this Actor.
			- Example: "solveriterationcount: 5"
			- Range: 1 to 255
			- See: ActorParams::mSolverIterationCount
		
		-	sleepenergytheshold (float) 
			- Description:	Threshold for the energy-based sleeping algorithm. Only used when
							the NX_BF_ENERGY_SLEEP_TEST flag is set
 			- Example: "sleepenergythreshold: 0.006"
 			- See: ActorParams::mSleepEnergyThreshold
 		
 		-	sleepdamping (float)
			- Description:	Damping factor for bodies that are about to sleep. 
			- Example: "sleepdamping: 0.5"
			- See: ActorParams::mSleepDamping
		
		-	node-scale (Vector3)
			- Applicable to Body and Actors that use a visual representation of the NxActor.
			- Description: Scale factor of the SceneNode
			- Example: "node-slace: 2 2 2"
			- See: ActorParams::mNodeScale
		
		-	node-shadows (bool)
			- Applicable to Body and Actors that use a visual representation of the NxActor.
			- Description: Apply shadows to the first entity (Body class). See other custom Actors
						   to see how shadows apply to it.
			- Example: "node-shadows: yes"
			- See: ActorParams::mNodeShaodws
				
		-	node (string)
			- Applicable to Body and Actors that use a visual representation of the NxActor.
			- Description: Name of the SceneNode, or name of an existing SceneNode to use (Body only).
			- Example: "node: myNodeName"			
			- See: ActorParams::mNodeName
						
	*/
	
	/** ActorParams

		See \ref actorparams for the full string argument documentation.
	*/		
	class NxExport ActorParams : public Params {

		public:


			ActorParams(){setToDefault();}
			ActorParams(const char* p) {process(p);}
			ActorParams(NxString p) {process(p);}

			void			setToDefault();
			void			parse(Parameters);			
			void			fromNxActorDesc(NxActorDesc&);
			void			fromNxBodyDesc(NxBodyDesc&);

			////////////////////////////////////////////////////////////////

#if NX_SDK_VERSION_NUMBER >= 260
			/// NxCompartment of the Actor to sit in.
			///	@todo NxOgre-ify
			/// @default NULL
			NxCompartment*		mCompartment;
#endif

			/// Density, mass must be 0 when density is used.
			NxReal				mDensity;
			
#if NX_SDK_VERSION_NUMBER >= 272 

			/// DominanceGroup (as Name).
			/// @default ""
			NxString			mDominanceGroupAsName;

			/// DominanceGroup (as Index)
			/// @default 0
			NxDominanceGroup	mDominanceGroupAsIndex;
#endif

			/// Flags for the NxActor
			/// @see NxActorFlags
			/// @default 0
			NxU32				mActorFlags;
			

			/// Group to use as index. GroupAsName must be blank.
			/// @default 0
			NxActorGroup		mGroupAsIndex;
			

			/// Group to use from name. GroupAsIndex must be 0.
			/// @default Blank String
			NxString			mGroupAsName;


			////////////////////////////////////////////////////////////////

			
			/// Angular Damping
			NxReal				mAngularDamping;


			/// Angular Velocity
			NxVec3				mAngularVelocity;


			///
			NxReal				mCCDMotionThreshold;


			///
			NxU32				mBodyFlags;


			///
			NxReal				mLinearDamping;

			
			///
			NxVec3				mLinearVelocity;


			/// Mass of the Actor. When used, make sure density is zero.
			NxReal				mMass;


			///
			NxMat34				mMassLocalPose;
			
			///
			NxVec3				mMassSpaceInertia;


			///
			NxReal				mMaxAngularVelocity;
			
			///
			NxReal				mSleepAngularVelocity;
			

			///
			NxReal				mSleepDamping;
			

			///
			NxReal				mSleepEnergyThreshold;


			///
			NxReal				mSleepLinearVelocity;


			///
			NxU32				mSolverIterationCount;


			///
			NxReal				mWakeUpCounter;


			////////////////////////////////////////////////////////////////


			/// SceneNode for Visual based Actors. If the node already exists then it will use it.
			NxString		mNodeName;

			/// Scale the SceneNode for Visual based Actors
			Ogre::Vector3	mNodeScale;

			/// Set the first Entity of the Visual based Actors to have shadows or not.
			bool			mNodeShadows;



	}; // End of ActorParams class

	class NxExport Actor {

		public:

			Actor(const NxString& Identifier, Scene*, bool isActorBased = true);
			Actor(const NxString& Identifier, Scene*, ShapeBlueprint*, const Pose&, ActorParams = ActorParams());
			virtual ~Actor();

			//////////////////////////////////////////////////////////

			Scene*		getScene()			{
												return mOwner;
											}

			NxScene*	getNxScene();

			//////////////////////////////////////////////////////////

			virtual bool					hasVisualisation() const {return false;}
			virtual void					simulate(float);
			virtual void					render(float);
			virtual void					shapeSimulate(float);
			virtual void					shapeRender(float);
			bool							isDead() { return (mActor == NULL); }


			/** Duplicate this actor
				
				@param	name	Name of the new Actor
				@param	pose	Position/Orientation of the new actor.

				@note
					This function is rather slow and inefficent when used repeatedly. Use a ActorBlueprint instead to
					keep a single blueprint of the actor and create as many as you want from that.

			 */
			Actor*							duplicate(const NxString& name, const Pose& p);

			//////////////////////////////////////////////////////////

			void							_createActor(ShapeBlueprint*,const Pose&,ActorParams params);
			void							_destroyActor();

			NxActor*						getNxActor(){return mActor;}

			//////////////////////////////////////////////////////////

			void							setName(NxString);
			NxString						getName();

			////////////////////////////////////////////////////////

			void							setGlobalPose(const Pose&);
			Pose							getGlobalPose();

			Ogre::Quaternion				getGlobalOrientation();
			void							setGlobalOrientation(const Ogre::Quaternion&);

			Ogre::Vector3					getGlobalPosition();
			void							setGlobalPosition(const Ogre::Vector3&);

			void							moveGlobalPose(const Pose&);
			void							moveGlobalPosition(const Ogre::Vector3&);
			void							moveGlobalOrientation(const Ogre::Quaternion&);

			void							moveTowards(const Pose&, float force = 10.0f);

			//////////////////////////////////////////////////////////

			Shape*							addShape(ShapeBlueprint*);
			void							removeShape(Shape*);
			void							removeShape(NxShapeIndex);
			NxU32							getNbShapes() const;
			CollisionModel*					getCollisionModel();
			CollisionModel*					getDynamicCollisionModel();

			//////////////////////////////////////////////////////////

			void							setGroup(ActorGroup*);
			void							setGroup(const NxString& GroupIdentifier);
			void							setGroup(NxActorGroup);
			ActorGroup*						getGroup()	const;

			//////////////////////////////////////////////////////////

			void							raiseActorFlag(NxActorFlag);
			void							clearActorFlag(NxActorFlag);
			bool							hasActorFlag(NxActorFlag)	const;
			bool							isDynamic()	const;

			//////////////////////////////////////////////////////////

			void							setCMassOffsetLocalPose(const Pose&);
			void							setCMassOffsetLocalPosition(const Ogre::Vector3&);
			void							setCMassOffsetLocalOrientation(const NxMat33&);
			void							setCMassOffsetLocalOrientation(const Ogre::Matrix3&);
			void							setCMassOffsetGlobalPose(const Pose&);
			void							setCMassOffsetGlobalPosition(const Ogre::Vector3&);
			void							setCMassOffsetGlobalOrientation(const NxMat33&);
			void							setCMassOffsetGlobalOrientation(const Ogre::Matrix3&);
			void							setCMassGlobalPose(const Pose&);
			void							setCMassGlobalPosition(const Ogre::Vector3&);
			void							setCMassGlobalOrientation(const Ogre::Quaternion&);

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
			void							addLocalForce(const Ogre::Vector3& force, NxForceMode mode = NX_FORCE, bool wakeup = true);
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
			NxU32							linearSweep(const Ogre::Vector3& motion, NxU32 flags, void* userData, NxU32 nbShapeDescriptions, NxSweepQueryHit* ShapeDescriptions, NxUserEntityReport<NxSweepQueryHit>* callback, const NxSweepCache* sweepCache=NULL);
#endif

			//////////////////////////////////////////////////////////
#if NX_SDK_VERSION_NUMBER >= 260
			Compartment*					getCompartment() const;
#endif
			//////////////////////////////////////////////////////////

			CollisionModel					mCollisionModel,
											mDynamicCollisionModel;

			void*							getNxActorUserData();

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
			virtual void					disableVisualisation() {}

			NxU32							getBirthFrame() {return mBirthFrame;}

			//////////////////////////////////////////////////////////

			virtual StringPairList			saveCustom();
			virtual void					restoreCustom(StringPairList)	{}

		protected:

			Scene						   *mOwner;
			NxActor*						mActor;
			NxString						mName;
			NxActorUserData*				mNxActorUserData;
			NxString						mVisualIdentifier;
			NxU32							mBirthFrame;

		private:


	};// End of Actor Class


	/**
		
		\page customactors Custom Actors

	Using your own Actor class with NxOgre:

	The Header of "IActor", the inherited Actor.

	<code>
	class NxExport IActor : public Actor {

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
