#include "../ogre/python_ogre_include_OGRE.h"

#include "CanvasPO.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {

 #include "python_canvas_aliases.h"
} }

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_canvas{ namespace details{
inline void instantiate(){
 using namespace CanvasNS;
 #include "python_canvas_sizeof.h"

} } }


