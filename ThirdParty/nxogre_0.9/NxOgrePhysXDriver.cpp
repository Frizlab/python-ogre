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

#include "NxOgreStable.h"
#include "NxOgrePhysXDriver.h"
#include "NxOgreWorld.h"			// For: World & PhysXDriver work together.
#include "NxOgreError.h"			// For: Starting up Error.
#include "NxOgreLog.h"				// For: Setting up Log, and passing on to Error.
#include "NxOgreUserAllocator.h"
#include "NxCooking.h"				// For: NxInitCooking
#include "NxOgreRemoteDebuggerConnection.h"
#include "NxOgreMeshManager.h"
#include <iostream>

#if defined NX_WIN32 && defined NX_DEBUG
#include "windows.h"
#endif

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

void PhysXDriverParams::setToDefault() {
	
	useFrameListener = true;
	use_SDK_ID = false;
	SDK_ID_CompanyName = "";
	SDK_ID_ApplicationName = "";
	SDK_ID_ApplicationVersion = "";
	SKD_ID_ApplicationUserDefined = "";
	shutdownOnErrors = false;
#ifdef NX_DEBUG
	logtype = LT_HTML;
#else
	logtype = LT_NONE;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////

void PhysXDriverParams::parse(Parameters P) {

	NxUnderConstruction;
	setToDefault();

	for (Parameters::iterator p = P.begin(); p != P.end();p++) {

		if (Set("framelistener", (*p), useFrameListener)) continue;
		if (Set("shutdown-on-errors", (*p), shutdownOnErrors)) continue;
		
		if ((*p).first == "log") {

			toLower((*p).second);
			if ((*p).second == "none")			logtype = LT_NONE;
			else if ((*p).second == "text")		logtype = LT_TEXT;
			else if ((*p).second == "html")		logtype = LT_HTML;
			else if ((*p).second == "phpbb")	logtype = LT_PHPBB;
			continue;
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

PhysXDriver::PhysXDriver(World* w, PhysXDriverParams params) : mFramelistener(false), mWorld(w), mSDK(0), mTime(0), mTimeModifier(1), mLog(0), mDebugger(0) {

#ifdef NX_DEBUG
	NxDebug("NxOgre " + Nx_Version_Full + "'" + Nx_Version_Codename + "'");
#endif
	std::cout << "Driver 1\n";
	mError = new Error(this, params.shutdownOnErrors);
	mAllocator = new NxOgre::UserAllocator();
    std::cout << "Driver 2\n";
	
	if (params.logtype == params.LT_TEXT) {
		mLog = new Log(Log::TEXT);
		mError->addReporter(mLog, true);
	}
	else if (params.logtype == params.LT_HTML) {
		mLog = new Log(Log::HTML);
		mError->addReporter(mLog, true);
	}
	else if (params.logtype == params.LT_PHPBB) {
		mLog = new Log(Log::PHPBB);
		mError->addReporter(mLog, true);
	}
	else {
		mLog = 0;
	}
    std::cout << "Driver 3\n";
	
	if (params.useFrameListener)
		_createFrameListener();
    std::cout << "Driver 4\n";
	
	if (!params.use_SDK_ID)
		_createSDK();
	else
		_createSDKfromID(params.SDK_ID_CompanyName,params.SDK_ID_ApplicationName,params.SDK_ID_ApplicationVersion, params.SKD_ID_ApplicationUserDefined);
    std::cout << "Driver 5\n";
	
	std::stringstream s;
	s	<< "NxOgre (" << Nx_Version_Full << ") Started, working with:" << std::endl << std::endl
		<< "  - PhysX    => " << NX_SDK_VERSION_MAJOR << "." << NX_SDK_VERSION_MINOR << "." << NX_SDK_VERSION_BUGFIX << std::endl
		<< "  - Ogre     => " << OGRE_VERSION_MAJOR << "." << OGRE_VERSION_MINOR << "." << OGRE_VERSION_PATCH << " '" << OGRE_VERSION_NAME << "'" << std::endl;
	
	if (mSDK->getHWVersion() != NX_HW_VERSION_NONE) {
		s << "  - Hardware => ";
		if (mSDK->getHWVersion() == NX_HW_VERSION_ATHENA_1_0)
			s << "Athena 1.0";
		else
			s << "Unknown PhysX hardware";
		s << std::endl;
	}

	#ifdef NX_WIN32
	s	<< "  - Platform => Windows";
	#endif

	#ifdef NX_LINUX
			s	<< "  - Platform => Linux";
	#endif

	#ifdef NX_DEBUG
			s	<< " Debug";
	#endif

	s	<< std::endl;

	NxDebug(s.str());
	
	NxInitCooking(NULL, NULL);

	mMeshManager = new NxOgre::MeshManager(this);

}

////////////////////////////////////////////////////////////////////////////////////////////////

PhysXDriver::~PhysXDriver() {

	if (mFramelistener)
		_destroyFrameListener();

	if (mDebugger)
		destroyDebuggerConnection();

	delete mMeshManager;

	_destroySDK();

	NxDebug("PhysX SDK has stopped.");

	delete mAllocator;
	delete mError;

}

////////////////////////////////////////////////////////////////////////////////////////////////

NxPhysicsSDK* PhysXDriver::getSDK() {
	return mSDK;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void PhysXDriver::_createFrameListener() {
	mWorld->getRoot()->addFrameListener(this);
	mFramelistener = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void PhysXDriver::_destroyFrameListener() {
	mWorld->getRoot()->removeFrameListener(this);
	mFramelistener = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void PhysXDriver::_createSDK() {
	
	NxPhysicsSDKDesc desc;
	desc.setToDefault();
	mSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, mAllocator, mError, desc, 0);
	//mSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, mAllocator); ///* To-Implement:, mAllocator, mError */);
	
	if(!mSDK) {
		std::stringstream s;
#ifdef NX_DEBUG
		s   << "Unable to start the PhysX SDK!" << std::endl << std::endl 
			<< "Possible problems:" << std::endl
			<< std::endl
			<< "1.	Do you have the SystemSoftware.exe installed?" << std::endl
			<< "	http://www.ageia.com/drivers/" << std::endl
			<< std::endl << std::endl
			<< "2.	Does the System software support the NxOgre version you have?" << std::endl 
			<< "	PhysX DLL version: " << NX_SDK_VERSION_MAJOR << "." << NX_SDK_VERSION_MINOR << "." << NX_SDK_VERSION_BUGFIX << std::endl
			<< std::endl << std::endl 
			<< "3.	Are you using the latest version of NxOgre?" << std::endl
			<< "	" << Nx_Version_Full << std::endl
			<< "	Most recent versions are at http://get.nxogre.org/" << std::endl;
#else
		s << "Unable to start the PhysX SDK!" << std::endl << std::endl << "Please refer to the developer or publisher's website or support line for information relating to the errors detailed."
		  << std::endl << std::endl << "	-> Error Code: PhysX SDK is unavailable. Please install the PhysX System Software." << std::endl;
#endif

		s <<  std::endl << std::endl << std::endl <<  "Version information:" << std::endl
		<< "- PhysX    => " << NX_SDK_VERSION_MAJOR << "." << NX_SDK_VERSION_MINOR << "." << NX_SDK_VERSION_BUGFIX << std::endl
		<< "- NxOgre   => " << Nx_Version_Full << std::endl
		<< "- Ogre     => " << OGRE_VERSION_MAJOR << "." << OGRE_VERSION_MINOR << "." << OGRE_VERSION_PATCH << " '" << OGRE_VERSION_NAME << "'." << std::endl;
			
		#ifdef NX_WIN32
		s	<< "- Platform => Windows";
		#endif


		#ifdef NX_LINUX
				s	<< "- Platform => Linux";
		#endif

		#ifdef NX_DEBUG
				s	<< " Debug";
		#endif

		s << std::endl;

		NxThrow_Error(s.str());

#if defined NX_WIN32 && defined NX_DEBUG
		s << std::endl << "On clicking of OK this Application will quit. See log or console window for more information.";
		MessageBox( NULL, s.str().c_str(), "Unable to start the PhysX SDK!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#endif


		mWorld->shutdown();
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

void PhysXDriver::_createSDKfromID(NxString a,NxString b,NxString c,NxString d) {
	NxUnderConstruction;
//	mSDK = NxCreatePhysicsSDKWithID(NX_PHYSICS_SDK_VERSION,a.c_str(),b.c_str(),c.c_str(),d.c_str()/* To-Implement:, mAllocator, mError */);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void PhysXDriver::_destroySDK() {
	mSDK->release();
	mSDK = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool PhysXDriver::frameStarted(const Ogre::FrameEvent& evt) {
	simulate(evt.timeSinceLastFrame);
#if (NX_RENDER_IN_FRAMESTARTED == 1)
	render(evt.timeSinceLastFrame);
#endif
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void PhysXDriver::simulate(float time) {
	
	if (mTimeModifier) {
		mTime += time * mTimeModifier;
		mWorld->simulate(time * mTimeModifier);
		if (mDebugger)
			mDebugger->simulate(time);
	}

	
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool PhysXDriver::frameEnded(const Ogre::FrameEvent& evt) {

#if (NX_RENDER_IN_FRAMESTARTED == 0)
	render(evt.timeSinceLastFrame);
#endif
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void PhysXDriver::render(float time) {

	if (mTimeModifier) {
		mWorld->render(time * mTimeModifier);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

void PhysXDriver::createDebuggerConnection(const NxString& address, unsigned int port) {
	if (mDebugger)
		return;
	mDebugger = new RemoteDebuggerConnection(address, port, this);
}

////////////////////////////////////////////////////////////////////////////////////////////////

void PhysXDriver::destroyDebuggerConnection() {
	if (mDebugger) {
		delete mDebugger;
		mDebugger = 0;
	}
}

}; //End of NxOgre namespace.
