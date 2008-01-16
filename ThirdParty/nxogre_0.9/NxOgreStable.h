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

#ifndef __NXOGRE_STABLE_H__
#define __NXOGRE_STABLE_H__



///////////////////////////////////////////////////////////////
//															 //
//		Configuration										 //
//															 //
///////////////////////////////////////////////////////////////



// Report any variables in containers, that are not "locked" as 
// possible leaks in the log.
// - Comment to disable, Uncomment to enable
#define NX_TREAT_POSSIBLE_LEAKS_AS_LEAKS


// Allow the DebugRenderer to be used.
// 0 - Disallow (Default) 1 - Allow
#define NX_USE_DEBUG_RENDERER_API 0


// Compile with some of the unstable parts of the NxOgre API.
// 0 - Disallow (Default) 1 - Allow
#define NX_UNSTABLE 0


// Use some of the older terms of the API with NxOgre
// 0 - Disallow (Default) 1 - Allow
//	->	MeshShape as TriangleMeshShape
#define NX_USE_LEGACY_TERMS 0


// Print any Reported Errors, Debug messages or Warnings to the Console.
// 0 - Disallow (Default) 1 - Allow
#define NX_CONSOLE_CHATTER 1


// Use JSON import/export fileformat. Requires JSONCPP dependency.
// Remember to add or remove jsoncpp.lib/jsoncppD.lib in the linker settings!
// 0 - Disallow (Default) 1 - Allow
#define NX_USE_JSON 0


// Use TinyXML for NxOgreXML format. Requires TinyXML dependency.
// Remember to add or remove tinyxml.lib/tinyxmlD.lib in the linker settings!
// 0 - Disallow (Default) 1 - Allow
#define NX_USE_TINYXML 0


// Use NxuStream serialisation system by Ageia. Requires NxuStream dependency.
// Remember to add or remove nxustream.lib/nxustream.lib in the linker settings!
// 0 - Disallow (Default) 1 - Allow
#define NX_USE_NXUSTREAM 0


// Use NxSceneTransform to move actors in the Scene.
// Will be stable in '38
// 0 - Disallow 1 - Allow (Default)
#define NX_UNSTABLE_USE_SCENE_ACTIVE_TRANSFORM 1


// When a framelistener is bound to the PhysXDriver, it will call render scene
// at the beginning of the frame, rather than the end.
// 0 - Render at the end of the frame, 1 - Render at the beginning of the frame.
#define NX_RENDER_IN_FRAMESTARTED 1


///////////////////////////////////////////////////////////////
// End of Configuration										 //
///////////////////////////////////////////////////////////////

#include "NxVersionNumber.h"

#if defined WIN32
#	define NX_WIN32
#	define NX_MEMORY_COOKING 1
#	define NX_DISK_COOKING 1

// #	if defined(NXOGRE_EXPORTS)
// #		define NxExport __declspec(dllexport)
// #	else
// #		define NxExport __declspec(dllimport)
// #	endif
#	define NxExport

#	if defined _DEBUG
#		define NX_DEBUG
#	endif
#endif

#if defined NX_LINUX
#	define NX_USE_FLUID_API 0
#	define NX_USE_SOFTBODY_API 1
#	define NX_MEMORY_COOKING 0
#	define NX_DISK_COOKING 1
#	define NxExport

#	if defined DEBUG
#		define NX_DEBUG
#	endif

#endif

#if (NX_SDK_VERSION_NUMBER >= 270)
#	define NX_USE_CHARACTER_API 1

#	ifndef NX_LINUX
#		define NX_USE_FLUID_API 1
#	endif

#	define NX_USE_SOFTBODY_API 1

#	if (NX_SDK_VERSION_NUMBER >= 272)
#		define NX_USE_FORCEFIELD_API 1
#	else
#		define NX_USE_FORCEFIELD API 0
#	endif

#else
	#define NX_USE_CHARACTER_API 0
	#define NX_USE_FORCEFIELD_API 0
	#define NX_DISABLE_FLUIDS
	#define NX_DISABLE_SOFTBODY
	#define NX_DISABLE_CLOTH
#endif

// Temp
#define NX_USE_FORCEFIELD_API 1

//
//////////////////////////////////////////////////////////////
//
//		Compiler Warnings
//
#pragma warning(disable: 4267)	// warning C4267: conversion from 'size_t' to 'unsigned int', possible loss of data
//
//
//////////////////////////////////////////////////////////////
//
#include <list>
#include <Ogre.h>

#ifdef NX_WIN32
#	include <OgreNoMemoryMacros.h>
#endif

#include <Nx.h>
#include <NxPhysics.h>
#include <ControllerManager.h>

#ifdef NX_WIN32
#	include <OgreMemoryMacros.h>
#endif
//
//////////////////////////////////////////////////////////////

#endif
