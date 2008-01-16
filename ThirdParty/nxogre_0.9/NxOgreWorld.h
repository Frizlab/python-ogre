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

#ifndef __NXOGRE_WORLD_H__
#define __NXOGRE_WORLD_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreContainer.h"				// For: mScenes
#include "NxOgrePhysXDriver.h"				// For: World::World() & World::mDriver
#include "NxOgreScene.h"					// For: SceneParams

namespace NxOgre {

	/** The container of all Scenes and everything therein.
		@remarks
			World is the top-most NxOgre class, it's similar to Ogre's Root. It is the
			first NxOgre class to be created and the last to be destroyed.

	 */
	class NxExport World {

		friend class PhysXDriver;
		friend class Error;
		friend class Scene;
		
		public:

			/** Constructor
				@remarks
					Starts up World, and creates the PhysX singleton.

				@param PhysXDriverParams Basic parameters of the World; Log type used.

				@see PhysXDriverParams
			*/
			World(PhysXDriverParams = PhysXDriverParams());

			
			/** Destructor
				@remarks
					Destroys the entire World and shut's down PhysX.All owned Scenes and 
					owned instances of classes in the scene will be	deleted. All custom classes, 
					or custom storage types should delete them 	before deleting World.
			  */
			~World();


			/** createScene
				@remarks
					A Scene is a sub-section of a World. Although a Scene is infinite in size like World. 
					Scenes are considered to be seperate from each other. Actors from one scene cannot 
					interact with ones from another. Scenes have also their own Materials, Groups, etc. 
					which cannot be shared.	Scenes are used in containers. Creating a Scene through the World factory method makes the Scene "owned"
					by NxOgre, on deletion of the Scene it will remove the presence of the Scene and delete the pointer.
					Manually creating the Scene "new Scene()" will only add a presence, it is up to you to delete it at the
					correct time.


				@param Identifier Unique identifier of the Scene
				@param Ogre::SceneManager* SceneManger for this Scene to Use.
				@param SceneParams SceneParams for this Scene to use.

				@see SceneParams

				@return Scene				The scene itself.
			 */
			Scene*					createScene(const NxString& Identifier, Ogre::SceneManager*, SceneParams = "");


			/** destroyScene
				@remarks
					Destroys the Scene and the contents therein. 
					
					If the Scene is owned by NxOgre use World::SceneDestroy, if not then use "delete mScene".

				@see World::destroyScene
			*/
			void					destroyScene(const NxString& name);

		   /** getRoot
				@remarks
					Returns the Ogre Root pointer, equivalent to Ogre::Root::GetSingletonPtr()

				@return Ogre::Scene
			*/
			Ogre::Root*				getRoot()					{
																	return mRoot;
																}
			
			/** getPhysXDriver
				
				@remarks
					Returns the PhysXDriver, the inbetween PhysX and NxOgre, and the class responsible
					for frame by frame operations. Automatically created during the construction
					of the World.

			 @return The PhysXDriver
			 */
			PhysXDriver*			getPhysXDriver()			{
																	return mDriver;
																}


#if (NX_USE_CHARACTER_API == 1)
			/** getCharacterController
				
				@remarks
					Returns the CharacterController. The main class responsible and controls for
					all characters for all Scenes. Automatically created during the construction
					of the World.

			 @return The CharacterController
			 */
			CharacterController*	getCharacterController()	{
																	return mCharacterController;
																}
#endif
			/** getSDK
				
				@remarks
					Returns the PhysX SDK instance. Thar be Sea monsters.

			 @return The NxPhysicsSDK instance
			 */
			NxPhysicsSDK*			getSDK();

			/** simulate
				
				@remarks
					Advances the state of the scene by x seconds. This is automatically controlled
					by PhysXDriver if NxOgre is attached to a frameListener. To advance the simulation by
					hand or without an attached FrameListener; Provide a "FrameListener: no" params
					to the World constructor, and advance the simulation in PhysXDriver::simulate(..).

				@param time Seconds passed since last simulate
				
				@see PhysXDriver::simulate

			 */
			void					simulate(float time);
			
			/** simulate
				
				@remarks
					Renders the new state of the scene. This is automatically controlled
					by PhysXDriver if NxOgre is attached to a frameListener. To render the simulation by
					hand or without an attached FrameListener. Provide a "FrameListener: no" params
					to the World constructor, and call PhysXDriver::render(..).

				@param time Seconds passed since last simulate. Must be as the same as the previous simulate.
				
				@see PhysXDriver::render

			 */
			void					render(float time);


			Scenes*					getScenes();
		
			////////////////////////////////////////////////////////////

#if (NX_USE_DEBUG_RENDERER_API == 1)

			/** createDebugRenderable
				
				@remarks
					...

				@param Ogre::SceneManager SceneManager to render to.
				

			 */
			void					createDebugRenderer(Ogre::SceneManager* s);
			
			/** destroyDebugRenderable
				@remarks
					Removes the Debug render from the Scene.
			 */
			void					destroyDebugRenderer();
#endif


			void					setCCD(bool enabled, NxReal epsilon = 0.01f);
			bool					getCCD();
			NxReal					getCCDEpsilon();

		protected:

			void _registerScene(const NxString& name, Scene*);
			void _unregisterScene(const NxString&);

			////////////////////////////////////////////////////////////


			/// Ogre Root Pointer
			Ogre::Root					*mRoot;
			
			/// PhysXDriver
			PhysXDriver*				mDriver;

			/// Scenes
			Scenes						mScenes;

#if (NX_USE_CHARACTER_API == 1)
			CharacterController*		mCharacterController;
#endif

#if (NX_USE_DEBUG_RENDERER_API == 1)
			DebugRenderer*				mDebugRenderer;
#endif

			void						shutdown();

//			Serialiser*					mSerialiser;

		private:

	};

};

#endif
