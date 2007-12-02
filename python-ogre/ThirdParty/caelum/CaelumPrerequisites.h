#ifndef CAELUMPREREQUISITES_H
#define CAELUMPREREQUISITES_H

// Include external headers
#include "Ogre.h"

// Define the dll export qualifier if compiling for Windows
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#ifdef CAELUM_LIB
		#define DllExport __declspec (dllexport)
	#else
		#ifdef __MINGW32__
			#define DllExport
		#else
			#define DllExport __declspec (dllimport)
		#endif
	#endif
#else
	#define DllExport
#endif

// Define the version code
#define CAELUM_VERSION_MAIN 0
#define CAELUM_VERSION_SEC 2
#define CAELUM_VERSION_TER 1
#define CAELUM_VERSION = (CAELUM_VERSION_MAIN << 16) | (CAELUM_VERSION_SEC << 8) | CAELUM_VERSION_TER

namespace caelum {
    /// Resource group name for caelum resources.
    extern DllExport Ogre::String RESOURCE_GROUP_NAME;

    // Render group for caelum stuff
    // It's best to have them all together
    enum CaelumRenderQueueGroupId
    {
        CAELUM_RENDER_QUEUE_STARFIELD = Ogre::RENDER_QUEUE_SKIES_EARLY + 0,
        CAELUM_RENDER_QUEUE_SKYDOME = Ogre::RENDER_QUEUE_SKIES_EARLY + 1,
        CAELUM_RENDER_QUEUE_SUN = Ogre::RENDER_QUEUE_SKIES_EARLY + 2,
        CAELUM_RENDER_QUEUE_CLOUDS = Ogre::RENDER_QUEUE_SKIES_EARLY + 3,
        CAELUM_RENDER_QUEUE_GROUND_FOG = Ogre::RENDER_QUEUE_SKIES_EARLY + 4,
    };
}

// Log macro
#define LOG(msg) Ogre::LogManager::getSingleton().logMessage(msg);

#endif //CAELUMPREREQUISITES_H
