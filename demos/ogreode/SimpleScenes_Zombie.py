# /*
# SimpleScenes_RagDoll.h
# ---------------------
# A demo RagDoll shooting demo, showing ragdolls and ray intersections.
# */
# #ifndef _SIMPLESCENES_ZOMBIE_H_
# #define _SIMPLESCENES_ZOMBIE_H_



# /*
# The zombie demo extends the base test class
# */
class SimpleScenes_Zombie ( SimpleScenes ):
 
    class ragDollData:
        def __init__:
            self.sSelectedMesh = 0
            self.timeSinceBirth=0
            self.timeSincePhysical=0
 
	def __init__ ( self )
	    SimpleScenes.__init__(self)
        self.xmlNames = [ "zombie", "zombie", "ninja", "robot" ]
        self.meshNames = ["zombie_small.mesh","zombie_small.mesh","ninja.mesh","robot.mesh"]
        self.meshAnimation = ["Walk1","Walk1","Walk","Walk"]
        self.meshScale = [  ogre.Vector3(1.0, 1.0, 1.0), ogre.Vector3(1.0, 1.0, 1.0),
                        ogre.Vector3(0.02, 0.02, 0.02), ogre.Vector3(0.05, 0.05, 0.05) ]
        self.ragdollFile = ["zombie.ogreode","zombie_auto.ogreode","ninja.ogreode","robot.ogreode"]
        self.sMaxRagdoll = 30;
		self._last_ragdoll = 0.0;
	
		
	self.myRagDolls = [];

		
        OgreOde_Prefab.Ragdoll *ragdoll = static_cast <OgreOde_Prefab.Ragdoll *> (dotOgreOdeLoader.loadObject(self.ragdollFile[self.sSelectedMesh],
                                                                             self.xmlNames[self.sSelectedMesh],
                                                                             "zombie" + StringConverter.toString(self._ragdoll_count++)));



		ragdoll.setCastShadows(True);		
		ragdoll.setQueryFlags (ZOMBIE_QUERY_MASK);

        Any a = ragdoll.getUserAny ();
        assert (a.isEmpty ());
        {
            ragDollData *b  = new ragDollData ();

            b.timeSinceBirth = 0.0f;
            b.timeSincePhysical = 0.0f;
            b.sSelectedMesh = self.sSelectedMesh;
			

            Any *newA = new Any(b);                        
            ragdoll.setUserAny (*newA);
        }


		## Add entity to the scene node
		SceneNode *ragdoll_node = self._mgr.getRootSceneNode().createChildSceneNode(ragdoll.getName() + "Node");
		ragdoll_node.attachObject(ragdoll);
		ragdoll_node.yaw(ogre.Degree(rand() % 360));
		ragdoll_node.setPosition(0.0,0.0,0);
        ragdoll_node.setScale( self.meshScale[self.sSelectedMesh]);

		self._last_node = ragdoll_node;

        
		ragdoll.getAnimationState(self.meshAnimation[self.sSelectedMesh]).setEnabled(True);

        self.myRagDolls.append (ragdoll);


        ##self.sSelectedMesh = (self.sSelectedMesh + 1) % 4;
	}

	## Constructor
    SimpleScenes_Zombie(OgreOde.World *world) :
        SimpleScenes(world)
	{
		self._over = (Overlay*)OverlayManager.getSingleton().getByName("OgreOdeDemos/Target");    
		self._over.show();

		self._gun = self._mgr.createEntity("gun","gun.mesh");
		self._gun.setCastShadows(False);

		self._gun_node = self._mgr.getRootSceneNode().createChildSceneNode("gun");
		self._gun_node.attachObject(self._gun);

		self._camera = self._mgr.getCamera("PlayerCam");

		self._animation_speed = 1.0;
		self._ragdoll_count = 0;
		createRagDoll();

		self._shot_time = 0.0;

		myOdeRay = new OgreOde.RayGeometry(1000.0, self._world, self._space);
		self.mRayQuery = self._mgr.createRayQuery(Ray());
		self.mRayQuery.setQueryMask (ZOMBIE_QUERY_MASK);
		self.mRayQuery.setQueryTypeMask(SceneManager.ENTITY_TYPE_MASK);
	}
	
	## Destructor
	virtual ~SimpleScenes_Zombie()
    {    
        for ( self.RagDollList.iterator zE = self.myRagDolls.begin();
            zE != self.myRagDolls.end(); ++zE)
        {
            assert ((*zE).getParentNode ());
            assert ((*zE).getParentNode ().getParent());
            (static_cast<SceneNode*> ((*zE).getParentNode ().getParent())).removeAndDestroyChild(
                (*zE).getParentNode ().getName ());
            self._mgr.destroyMovableObject((*zE).getName(), OgreOde_Prefab.RagdollFactory.FACTORY_TYPE_NAME);
                
		}   
		delete myOdeRay;

		self._over.hide();
		self._mgr.destroySceneNode("gun");
		self._mgr.destroyEntity("gun");
		self._mgr.destroyQuery (self.mRayQuery);
	}

	## Return our name for the test application to display
	virtual const String& getName()
	{
		static String name = "RagDoll Shooting Gallery";
		return name;
	}

	## Return a description of the keys that the user can use in this test
	virtual const String& getKeys()
	{
		static String keys = "X - Shoot";
		return keys;
	}

   
#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    virtual void frameStarted(Real time, InputReader* input)
#else
    virtual void frameStarted(Real time, OIS.Keyboard* input, OIS.Mouse* mouse)
#endif ##OGRE_VERSION not heihort
	{
		self._last_ragdoll += time;

        ## Do default key handling
#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
        SimpleScenes.frameEnded(time, input);
#else
        SimpleScenes.frameEnded(time, input, mouse);
#endif ##OGRE_VERSION not heihort

		self._gun_node.setOrientation(self._camera.getOrientation());
		self._gun_node.setPosition(self._camera.getPosition() + (self._camera.getOrientation() * ogre.Vector3(0.3,-0.15,-1.1)));

        for ( self.RagDollList.iterator zE = self.myRagDolls.begin();
            zE != self.myRagDolls.end(); 
            ++zE)
        {   
            ragDollData *b = any_cast<ragDollData *> ((*zE).getUserAny());
            if ((*zE).isPhysical())
            {
                if (!(*zE).isStatic())
                {

                    b.timeSincePhysical += time;
                    (*zE).update();## RagDoll Activated
                }
            }
            else
            {
                (*zE).getAnimationState(self.meshAnimation[b.self.sSelectedMesh]).addTime(time * self._animation_speed);

                (*zE).getParentNode ().translate(
                    (*zE).getParentNode ().getOrientation() * (Vector3.UNIT_Z * time * 2.5));
                
            }
        }

	}


    def frameEnded(self, time, input, mouse)
    {
        ## Do default processing
        SimpleScenes.frameEnded(self,time, input, mouse);
#		self._shot_time -= time;

		if (self._key_delay > 0 && input.isKeyDown(OIS.KC_C))
        {  
            for ( self.RagDollList.iterator zE = self.myRagDolls.begin();
                zE != self.myRagDolls.end(); 
                ++zE)
            {
                if ((*zE).isPhysical())
                {
                    (*zE).releasePhysicalControl();
                    ragDollData *b = any_cast<ragDollData *> ((*zE).getUserAny());
                    (*zE).getAnimationState(self.meshAnimation[b.self.sSelectedMesh]).setEnabled(True);
                }
            }

            self._key_delay = SimpleScenes.KEY_DELAY;
		}



        if (self._shot_time <= 0.0 &&             
#if (OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
             (input.isKeyDown(OIS.KC_X) || input.getMouseButton (0)))
#else
            ( input.isKeyDown(OIS.KC_X) || mouse.getMouseState().buttonDown(OIS.MB_Left)))
#endif
		{

			Ray pickRay = self._camera.getCameraToViewportRay(0.5, 0.5);
			myOdeRay.setDefinition(pickRay.getOrigin(), pickRay.getDirection());

			self._shot_time = 0.2;

			self.mRayQuery.setRay (pickRay);
			const RaySceneQueryResult& result = self.mRayQuery.execute();
			if (!result.empty())
			{
				RaySceneQueryResult.const_iterator i = result.begin();
		
				##self.mRayQuery.setSortByDistance (True, 1);##only one hit
				myOdeRay.enable ();
				while((i != result.end()))
				{

					self._last_ragdoll = 5.0;

					OgreOde_Prefab.Ragdoll * const radgoll =
						static_cast<OgreOde_Prefab.Ragdoll *> (i.movable);

				   ## uncomment to see nice debug mesh of animated mesh
###define _TESTTRIMESH
#ifdef _TESTTRIMESH 

					## ray cast could be tested against that instead of ragdoll.
					## depending on complexity of mesh could be simpler
					OgreOde.EntityInformer ei((*zE), (*zE).getParentNode ()._getFullTransform());
					_geoms.append(ei.createStaticTriangleMesh(self._space));
#else ##_TESTTRIMESH
					bool wasPhysical = radgoll.isPhysical ();
					if (!wasPhysical)
					{
						ragDollData * const b = any_cast<ragDollData *> (radgoll.getUserAny());
						radgoll.getAnimationState(self.meshAnimation[b.self.sSelectedMesh]).setEnabled(False);

						
						## Create the ragdoll
						radgoll.takePhysicalControl(self._world, self._space, False);
						radgoll.setSelfCollisions(False);

					}


                    OgreOde.Body *hit_body;
                    ogre.Vector3 hit_point;

					bool is_hit = False;
					if (radgoll.pick(myOdeRay, hit_body, hit_point))
					{
						if (hit_body)
						{
							hit_body.addForceAt(pickRay.getDirection() * 250000, hit_point);
							is_hit = True;
							ragDollData *b = any_cast<ragDollData *> (radgoll.getUserAny());
							b.timeSincePhysical = 0.0f;
						}
					}

					if (!is_hit&& !wasPhysical)
					{
						radgoll.releasePhysicalControl();
						ragDollData *b = any_cast<ragDollData *> (radgoll.getUserAny());
						radgoll.getAnimationState(self.meshAnimation[b.self.sSelectedMesh]).setEnabled(True);
					}
#endif ##_TESTTRIMESH	

					++i;
				}	## if results.	
			}
			myOdeRay.disable ();
        }
			
	           
		for ( self.RagDollList.iterator zE = self.myRagDolls.begin();
			zE != self.myRagDolls.end(); )
		{

			OgreOde_Prefab.Ragdoll * const radgoll = (*zE);
			ragDollData *b = any_cast<ragDollData *> (radgoll.getUserAny());
			b.timeSinceBirth += time;

			## turn to stone to improve fps,
			## better way to do that would be to tweak
			## simulation parameters to be less jitty.
			## better auto sleep
			if (radgoll.isPhysical() 
				&& !radgoll.isStatic() 
				&& radgoll.isAwake()
				&& b.timeSincePhysical > 5.0)
			{
				##radgoll.turnToStone();
				radgoll.sleep();
			}
			
            
			if ( b.timeSinceBirth > self.sMaxRagdoll)
			{
				(static_cast<SceneNode*> (radgoll.getParentNode ().getParent())).removeAndDestroyChild(
					radgoll.getParentNode ().getName ());
				self._mgr.destroyMovableObject(radgoll.getName(), OgreOde_Prefab.RagdollFactory.FACTORY_TYPE_NAME);
				zE = self.myRagDolls.erase(zE);
			}
			else
			{
				++zE;
			}
		}
        if (self._last_ragdoll > 5.0 && 
            self.myRagDolls.size() < static_cast<size_t>(self.sMaxRagdoll))
            createRagDoll();
		
	}


