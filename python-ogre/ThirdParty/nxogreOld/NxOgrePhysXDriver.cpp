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
#include "NxOgreResourceManager.h"
#include "NxOgreOgreResourceSystem.h"

#if defined NX_WIN32 && defined NX_DEBUG
#	include "windows.h"
#endif

#include "OgreRoot.h"
#include "OgreSingleton.h"

namespace NxOgre {

////////////////////////////////////////////////////////////////////////////////////////////////

void PhysXDriverParams::setToDefault() {
	
	mHasFrameListener = true;
	mShutdownOnErrors = false;
	mInitResources = true;

#ifdef NX_DEBUG
	mLogType = LT_HTML;
#else
	mLogType = LT_NONE;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////

void PhysXDriverParams::parse(Parameters P) {

	NxUnderConstruction;
	setToDefault();

	for (Parameters::iterator p = P.begin(); p != P.end();p++) {

		if (Set("framelistener", (*p), mHasFrameListener)) continue;
		if (Set("shutdown-on-errors", (*p), mShutdownOnErrors)) continue;
		if (Set("init-resources", (*p), mInitResources)) continue;

		
		if ((*p).first == "log") {

			toLower((*p).second);
			if ((*p).second == "none")			mLogType = LT_NONE;
			else if ((*p).second == "text")		mLogType = LT_TEXT;
			else if ((*p).second == "html")		mLogType = LT_HTML;
			else if ((*p).second == "phpbb")	mLogType = LT_PHPBB;
			continue;
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////

PhysXDriver::PhysXDriver(World* w, PhysXDriverParams params) : mFramelistener(false), mWorld(w), mSDK(0), mTime(0), mTimeModifier(1), mLog(0), mDebugger(0) {

#ifdef NX_DEBUG
	NxDebug("NxOgre " + Nx_Version_Full + "'" + Nx_Version_Codename + "'");
#endif
	
	mError = new Error(this, params.mShutdownOnErrors);
	mAllocator = NxCreateAllocator("Global");

	if (params.mLogType == params.LT_TEXT) {
		mLog = new Log(Log::TEXT);
		mError->addReporter(mLog, true);
	}
	else if (params.mLogType == params.LT_HTML) {
		mLog = new Log(Log::HTML);
		mError->addReporter(mLog, true);
	}
	else if (params.mLogType == params.LT_PHPBB) {
		mLog = new Log(Log::PHPBB);
		mError->addReporter(mLog, true);
	}
	else {
		mLog = 0;
	}

	if (params.mHasFrameListener)
		_createFrameListener();

	_createSDK();

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

}

////////////////////////////////////////////////////////////////////////////////////////////////

PhysXDriver::~PhysXDriver() {

	if (mFramelistener)
		_destroyFrameListener();

	if (mDebugger)
		destroyDebuggerConnection();

	NxCloseCooking();

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
	
	

	NxSDKCreateError errorCode = NXCE_NO_ERROR;

	NxPhysicsSDKDesc desc;
	desc.setToDefault();
	mSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, mAllocator, mError, desc, &errorCode);
	
	if(!mSDK && errorCode != 0) {
		std::stringstream s;
#ifdef NX_DEBUG

		s   << "Unable to start the PhysX SDK!" << std::endl << std::endl;
		
		switch (errorCode) {

			default: 
				s << "Unknown Error." 
				  << "Please report to the NxOgre team. " << std::endl;
			break;
						
			case NXCE_PHYSX_NOT_FOUND:
				s << "Unable to find the PhysX libraries. The PhysX drivers are not installed correctly." << std::endl;
				s << "- You can download the PhysX Drivers (also known as SystemSoftware) from http://www.ageia.com/drivers/" << std::endl;
				s << "- Install/Reinstall the drivers. Make sure the version of the SystemSoftware is compatible with the SDK." << std::endl;
				s << "- Look at the SDK release notes for further information, or visit http://devsupport.ageia.com" << std::endl;
			break;

			case NXCE_WRONG_VERSION:
				s << "The application supplied a version number that does not match with the libraries." << std::endl;
				s << "- Make sure that the NxCreatePhysicsSDK function passes on NX_PHYSICS_SDK_VERSION." << std::endl;
			break;

			case NXCE_DESCRIPTOR_INVALID:
				s << "The supplied SDK descriptor is invalid." << std::endl;
				s << " - Look at NxPhysicsSDKDesc in nxphysicssdk.h for more information." << std::endl;
			break;
			
			case NXCE_CONNECTION_ERROR:
				s << "A PhysX card was found, but there are problems when communicating with the card." << std::endl;
				s << " - Please visit http://support.ageia.com and/or your PhysX card manufacture." << std::endl; 
			break;
			
			case NXCE_RESET_ERROR: 
				s << "A PhysX card was found, but it did not reset (or initialize) properly." << std::endl;
				s << " - Please visit http://support.ageia.com and/or your PhysX card manufacture." << std::endl; 
			break;

			case NXCE_IN_USE_ERROR:
				s << "A PhysX card was found, but it is already in use by another application." << std::endl;
				s << " - Close down all programs using the PhysX hardware before using this application." << std::endl; 
			break;

			case NXCE_BUNDLE_ERROR:
				s << "A PhysX card was found, but there are issues with loading the firmware." << std::endl;
				s << " - Please visit http://support.ageia.com and/or your PhysX card manufacture." << std::endl;
			break;

		}
#else
		s << "Unable to start the PhysX SDK!" << std::endl << std::endl << "Please refer to the developer or publisher's website or support line for information relating to the errors detailed."
		  << std::endl << std::endl << "	-> Error Code: ";

		switch (errorCode) {
			case NXCE_NO_ERROR:				s << "NXCE_NO_ERROR" << std::endl; break;
			case NXCE_PHYSX_NOT_FOUND:		s << "NXCE_PHYSX_NOT_FOUND" << std::endl; break;
			case NXCE_WRONG_VERSION:		s << "NXCE_WRONG_VERSION" << std::endl; break;
			case NXCE_DESCRIPTOR_INVALID:	s << "NXCE_DESCRIPTOR_INVALID" << std::endl; break;
			case NXCE_CONNECTION_ERROR:		s << "NXCE_CONNECTION_ERROR" << std::endl; break;
			case NXCE_RESET_ERROR:			s << "NXCE_RESET_ERROR" << std::endl; break;
			case NXCE_IN_USE_ERROR:			s << "NXCE_IN_USE_ERROR" << std::endl; break;
			case NXCE_BUNDLE_ERROR:			s << "NXCE_BUNDLE_ERROR" << std::endl; break;
		}

		s   << " - Reason: ";
		
		switch (errorCode) {
			default: s << "Unknown Error" << std::endl; break;
			case NXCE_NO_ERROR: s << "No Error(!!?)" << std::endl; break;
			case NXCE_PHYSX_NOT_FOUND: s << "Unable to find the PhysX libraries. The PhysX drivers are not installed correctly." << std::endl; break;
			case NXCE_WRONG_VERSION: s << "The application supplied a version number that does not match with the libraries." << std::endl; break;
			case NXCE_DESCRIPTOR_INVALID: s << "The supplied SDK descriptor is invalid." << std::endl; break;
			case NXCE_CONNECTION_ERROR: s << "A PhysX card was found, but there are problems when communicating with the card." << std::endl; break;
			case NXCE_RESET_ERROR: s << "A PhysX card was found, but it did not reset (or initialize) properly." << std::endl; break;
			case NXCE_IN_USE_ERROR: s << "A PhysX card was found, but it is already in use by another application." << std::endl; break;
			case NXCE_BUNDLE_ERROR: s << "A PhysX card was found, but there are issues with loading the firmware." << std::endl; break;
		}

		s << std::endl;
#endif

		s <<  std::endl << std::endl << std::endl <<  "Application information:" << std::endl
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

		s << "- Error  => ";
		
		switch (errorCode) {
			case NXCE_NO_ERROR:				s << "NXCE_NO_ERROR" << std::endl; break;
			case NXCE_PHYSX_NOT_FOUND:		s << "NXCE_PHYSX_NOT_FOUND" << std::endl; break;
			case NXCE_WRONG_VERSION:		s << "NXCE_WRONG_VERSION" << std::endl; break;
			case NXCE_DESCRIPTOR_INVALID:	s << "NXCE_DESCRIPTOR_INVALID" << std::endl; break;
			case NXCE_CONNECTION_ERROR:		s << "NXCE_CONNECTION_ERROR" << std::endl; break;
			case NXCE_RESET_ERROR:			s << "NXCE_RESET_ERROR" << std::endl; break;
			case NXCE_IN_USE_ERROR:			s << "NXCE_IN_USE_ERROR" << std::endl; break;
			case NXCE_BUNDLE_ERROR:			s << "NXCE_BUNDLE_ERROR" << std::endl; break;
		}

		NxThrow_Error(s.str());

#if defined NX_WIN32 && defined NX_DEBUG
		s << std::endl << "On clicking of OK this Application will quit. See log or console window for a copy of this message and further information.";
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

////////////////////////////////////////////////////////////////////////////////////////////////

bool PhysXDriver::hasHardware() {
	return (mSDK->getHWVersion() != NX_HW_VERSION_NONE);
}

////////////////////////////////////////////////////////////////////////////////////////////////

}; //End of NxOgre namespace.
