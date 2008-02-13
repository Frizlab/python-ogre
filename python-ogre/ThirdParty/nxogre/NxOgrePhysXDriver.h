/** \file    NxOgrePhysXDriver.h
 *  \brief   Header for the PhysXDriver and PhysXDriverParams classes.
 *  \version 1.0-20
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

#ifndef __NXOGRE_PHYSXDRIVER_H__
#define __NXOGRE_PHYSXDRIVER_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreParams.h"
#include "OgreFrameListener.h"

namespace NxOgre {

	/**
	 	\page physxdriver PhysXDriverParams
		
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
		
		-	framelistener (bool)
			- Description: Use NxOgre's Framelistener to self-inject time events.
			- Example: "framelistener: yes"
			- See: PhysXDriverParams::mHasFrameListener.	

		-	shutdown-on-errors (bool)
			- Description: On a thrown error, NxOgre will shutdown the application.
			- Example: "shutdown-on-errors: no"
			- See: PhysXDriverParams::mShutdownOnErrors.	

		-	log (multiple-choice: none, text, html, phpbb)
			- Description: Log format to write to.
			- Example: "log: html"
			- See: PhysXDriverParams::mLogType.	

		-	init-resources (bool)
			- Description: Initalises and loads in resources on creation of world. In some cases this may
						   want to be delayed until a custom ResourceSystem is loaded in, or until a
						   game has reached the loading scene sequence.
			- Example: "init-resources: No"
			- See: PhysXDriverParams::mInitResources

		-	IDCompanyName: <String>
		
		-	IDApplicationName: <String>
		
		-	IDApplicationVersion: <String>
		
		-	IDApplicationUserDefined: <String>
	
	*/
	
	/** PhysXDriverParams

		See \ref physxdriverparams for the full string argument documentation.
	*/	
	class NxPublicClass PhysXDriverParams : Params {

		public:

			PhysXDriverParams() {setToDefault();}
			PhysXDriverParams(const char* p){process(p);}
			PhysXDriverParams(NxString p){process(p);}

			void setToDefault();
			void parse(Parameters);
			
			/** \brief On an instance of a Error, shutdown the application straight away.
				\param shutdown-on-errors (bool)
				\default true
			*/
			bool mShutdownOnErrors;

			/** \brief Auto initalise (and load) resources
				\param init-resources (bool)
				\default true
			*/
			bool mInitResources;
			
			/** \brief Formats of Log */
			enum LogType {
				LT_NONE = 0,
				LT_TEXT = 1,
				LT_HTML = 2,
				LT_PHPBB
			};

			/** \brief Format of the log to write to 
				\param log (multiple-choice: none, text, html, phpbb)
				\default LT_HTML(Debug) or LT_NONE (Release)
			*/
			LogType mLogType;

			enum TimeControllerType {
				TC_NONE,
				TC_CUSTOM
#if (NX_USE_OGRE == 1)
				, TC_OGRE
#endif
			};

			/** \brief Time controller to use.
				\default TC_NONE
			*/
			TimeControllerType mTimeController;

			/** \brief Custom time controller (make sure TC_CUSTOM) mTimeController is set.
				\default 0
			*/
			TimeController* mCustomTimeControllerPtr;

	};

	/////////////////////////////////////////////////////////

	/** \brief The PhysXDriver is the direct interface between
			   the NxPhysicsSDK and NxOgre.
	*/
	class NxPublicClass PhysXDriver {

		friend class World;

		protected:

			/** \brief Creates the PhysXDriver, Starts up the PhysX instance and gets
				       everything ready for World.
				\note As PhysXDriver and World work together, World should be created first.
					  The World creates the PhysXDriver for you.
			*/
			PhysXDriver(World*, PhysXDriverParams = PhysXDriverParams());

			/** \brief Totally shuts down PhysX and NxOgre.
			*/
			~PhysXDriver();

		public:

			/** \brief Get's a copy of the PhysicsSDK, the top-most PhysX class.
				\return The SDK
			*/
			NxPhysicsSDK*	getSDK();

			/** \brief Checks if the current hardware
			has a PhysX PPU or not.
			*/
			bool hasHardware();


			/** \brief Advances the simulation since time last passed. Does not 
					   visually update the Scene.
				\param Time passed since last simulation.
				\example <code>
						   mPhysXDriver->simulate(0.16667f);
						   mPhysXDriver->render(0.16667f);
						 </code>
			*/
			void simulate(const float time);

			/** \brief Renders the changes of the simulation, using each SceneRenderer
				       for each Scene.
				\param Time passed since last simulation.
				\example <code>
						   mPhysXDriver->simulate(0.16667f);
						   mPhysXDriver->render(0.16667f);
						 </code>
			*/
			void render(const float time);

			/** \brief Sets the time modifier for injected timeModifier
				\param modifier value (1.0 is real-time, 2.0 is twice real-time, 0.5 half real-time).
			*/
			void setTimeModifier(NxReal modifier) {
			                                       mTimeModifier = modifier;
			                                      }

			/** \brief Returns a copy of the World class
				\return The World.
			*/
			World*	getWorld() {
			                    return mWorld;
			                   }

			/** \brief Returns a copy of the Error singleton class
				\return the error class
			*/
			Error*  getError() {
			                    return mError;
			                   }

			/** \brief Returns the total time simulated.
				\return time passed in seconds
			*/
			NxReal	getTime() const {
			                         return mTime;
			                        }

			/** \brief Retuns the total number of times simulated.
			*/
			unsigned int getNbSimulations() const {
			                                       return mNbSimulations;
			                                      }


			/** \brief Creates the remote debugger connection class, and attempts to communicate
			    with the remote debugger based on the network address and port given.
				\param address The network address of the computer running the the debugger (default: localhost). 
				\param port The port the remote debugger is running at (default: 5245).
			*/
			void	createDebuggerConnection(const NxString& address = "localhost", unsigned int port = 5425);
			
			/** \brief Removes the remote debugger connection class, and stops communicating
				with the remote debugger.
			*/
			void	destroyDebuggerConnection();

			/** \brief Returns a copy of the RemoteDebuggerConnection class.
			*/
			RemoteDebuggerConnection* getRemoteDebuggerConnection() {
			                                                         return mDebugger;
			                                                        }

			/** \brief Get's a copy of the TimeController class.
			*/
			TimeController*  getTimeController() {
				return mTimeController;
			}

		protected:

			/** \internal
			*/
			void  _createSDK();

			/** \internal
			*/
			void  _createSDKfromID(NxString, NxString, NxString, NxString);

			/** \internal
			*/
			void  _destroySDK();

			Allocator*                  mAllocator;
			World*                      mWorld;
			NxPhysicsSDK*               mSDK;
			bool                        mFramelistener;
			UserAllocator*              mUserAllocator;
			Error*                      mError;
			Log*                        mLog;
			NxReal                      mTime,
			                            mTimeModifier;
			unsigned int                mNbSimulations;
			RemoteDebuggerConnection*   mDebugger;
			TimeController*             mTimeController;

		private:

	};

};

#endif
