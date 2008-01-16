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

#ifndef __NXOGRE_PHYSXDRIVER_H__
#define __NXOGRE_PHYSXDRIVER_H__

#include "NxOgrePrerequisites.h"
#include "NxOgreParams.h"

namespace NxOgre {

	///	Params
	///		->	FrameListener: <bool>				| Default: Yes
	///		->	IDCompanyName: <String>
	///		->	IDApplicationName: <String>
	///		->	IDApplicationVersion: <String>
	///		->	IDApplicationUserDefined: <String>

	class NxExport PhysXDriverParams : Params {

		public:

			PhysXDriverParams() {setToDefault();}
			PhysXDriverParams(const char* p){process(p);}
			PhysXDriverParams(NxString p){process(p);}

			void setToDefault();
			void parse(Parameters);

			bool use_SDK_ID;
			NxString SDK_ID_CompanyName;			// Character string for the game or application developer company name
			NxString SDK_ID_ApplicationName;		// Character string for the game or application name
			NxString SDK_ID_ApplicationVersion;		// Character string for the game or application version
			NxString SKD_ID_ApplicationUserDefined;	// Character string for additional, user defined data

			bool useFrameListener;
			bool shutdownOnErrors;					// Default: No

			enum LogType {
				LT_NONE = 0 ,
				LT_TEXT = 1,
				LT_HTML = 2,
				LT_PHPBB
			};

			LogType logtype;

	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class NxExport PhysXDriver : public Ogre::FrameListener {

		public:

			PhysXDriver(World*, PhysXDriverParams = PhysXDriverParams());
			~PhysXDriver();

			NxPhysicsSDK*	getSDK();

			void start();
			void stop();
			void reset();
			bool hasHardware();

			bool frameStarted(const Ogre::FrameEvent& evt);
			void simulate(const float time);
			bool frameEnded(const Ogre::FrameEvent& evt);
			void render(const float time);
			void setTimeModifier(NxReal modifier) {mTimeModifier = modifier;}

			World*	getWorld(){return mWorld;}
			Error*  getError(){return mError;}
			NxReal	getTime(){return mTime;}

			void	createDebuggerConnection(const NxString& address = "localhost", unsigned int port = 5425);
			void	destroyDebuggerConnection();
			RemoteDebuggerConnection* getRemoteDebuggerConnection() {return mDebugger;}

		protected:

			void	_createSDK();
			void	_createSDKfromID(NxString,NxString,NxString,NxString);
			void	_createFrameListener();
			void	_destroyFrameListener();
			void	_destroySDK();

			World		 *mWorld;
			NxPhysicsSDK* mSDK;
			bool		  mFramelistener;
			NxOgre::UserAllocator* mAllocator;
			Error*		  mError;
			Log*		  mLog;
			NxReal		  mTime, mTimeModifier;
			NxOgre::MeshManager*	mMeshManager;
			RemoteDebuggerConnection* mDebugger;
			

		private:

	};

};

#endif
