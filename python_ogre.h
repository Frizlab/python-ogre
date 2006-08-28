#ifndef __python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__
#define __python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__

//See best practices section in Py++ documentation

#include "Ogre.h"

namespace python_ogre{ namespace details{

inline void instantiate(){
    using namespace Ogre;

    sizeof( Controller<float> );

    sizeof( SharedPtr< Controller<float> >);

    sizeof( SharedPtr< WaveformControllerFunction > );
}

} } //python_ogre::details


#endif//__python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__
