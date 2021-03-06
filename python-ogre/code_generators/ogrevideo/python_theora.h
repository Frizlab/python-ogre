#include "../ogre/python_ogre_include_OGRE.h"

/*#include "TheoraVideoManager.h"
#include "TheoraVideoClip.h"
#include "TheoraAudioInterface.h"
#include "OpenAL_AudioInterface.h"
*/

#include "OgreVideoManager.h"
#include "OgreVideoExport.h"
#include "OgreTheoraDataStream.h"
#include "OpenAL_AudioInterface.h"
#include "TheoraVideoFrame.h"

#include "TheoraVideoManager.h"

#include "OgreExternalTextureSource.h"
#include "OgreExternalTextureSourceManager.h"



// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_theora_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_nxogre{ namespace details{
inline void instantiate(){
 using namespace Ogre;      
 #include "python_theora_sizeof.h"
 
} } }


