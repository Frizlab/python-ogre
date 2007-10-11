#ifndef CAELUMPREREQUISITES_H
#define CAELUMPREREQUISITES_H

// Include external headers
#include "Ogre.h"

#define DllExport

// Define the version code
#define CAELUM_VERSION_MAIN 0
#define CAELUM_VERSION_SEC 2
#define CAELUM_VERSION_TER 1
#define CAELUM_VERSION = (CAELUM_VERSION_MAIN << 16) | (CAELUM_VERSION_SEC << 8) | CAELUM_VERSION_TER

namespace caelum {
    /// Resource group name for caelum resources.
    extern DllExport Ogre::String RESOURCE_GROUP_NAME;

    /* Set a global camera distances modifier for the clipping distance. 
       This threshold will be substracted from the far clipping distance if applicable, else it will be 
       added to the near clipping distance (ie, if infinite far clipping distance is used).
     */
    const float CAMERA_DISTANCE_MODIFIER = 0;

    // Render grounp for caelum stuff
    // It's best to have them all together
    enum CaelumRenderQueueGroupId
    {
        CAELUM_RENDER_QUEUE_STARFIELD = Ogre::RENDER_QUEUE_SKIES_EARLY + 0,
        CAELUM_RENDER_QUEUE_SKYDOME = Ogre::RENDER_QUEUE_SKIES_EARLY + 1,
        CAELUM_RENDER_QUEUE_SUN = Ogre::RENDER_QUEUE_SKIES_EARLY + 2,
        CAELUM_RENDER_QUEUE_CLOUDS = Ogre::RENDER_QUEUE_SKIES_EARLY + 3,
    };
}

// Log macro
#define LOG(msg) Ogre::LogManager::getSingleton().logMessage(msg);

#endif //CAELUMPREREQUISITES_H
