#ifndef CAELUMPREREQUISITES_H
#define CAELUMPREREQUISITES_H

// Include external headers
#include "Ogre.h"

// Define the dll export qualifier if compiling for Windows
// // #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// // 	#ifdef CAELUM_LIB
// // 		#define DllExport __declspec (dllexport)
// // 	#else
// // 		#ifdef __MINGW32__
// // 			#define DllExport
// // 		#else
// // 			#define DllExport __declspec (dllimport)
// // 		#endif
// // 	#endif
// // #else
	#define DllExport
// // #endif

// Define the version code
#define CAELUM_VERSION_MAIN 0
#define CAELUM_VERSION_SEC 2
#define CAELUM_VERSION_TER 1
#define CAELUM_VERSION = (CAELUM_VERSION_MAIN << 16) | (CAELUM_VERSION_SEC << 8) | CAELUM_VERSION_TER

/* Set a global camera distances modifier for the clipping distance. 
   This threshold will be substracted from the far clipping distance if applicable, else it will be 
   added to the near clipping distance (ie, if infinite far clipping distance is used).
 */
#define CAMERA_DISTANCE_MODIFIER 0.1

// Log macro
#define LOG(msg) Ogre::LogManager::getSingleton().logMessage(msg);

#endif //CAELUMPREREQUISITES_H
