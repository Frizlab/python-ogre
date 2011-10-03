#include "../ogre/python_ogre_include_OGRE.h"

#include "OgreRTShaderSystem.h"
#include "OgreShaderProgramSet.h"
#include "OgreShaderHLSLProgramProcessor.h"
#include "OgreShaderGLSLProgramProcessor.h"
#include "OgreShaderCGProgramProcessor.h"
#include "OgreShaderHLSLProgramWriter.h"
#include "OgreShaderGLSLProgramWriter.h"
#include "OgreShaderCGProgramWriter.h"


// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ogrertshadersystem_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ogrepaging{ namespace details{
inline void instantiate(){
 using namespace Ogre;
 #include "python_ogrertshadersystem_sizeof.h"
 
} } }


