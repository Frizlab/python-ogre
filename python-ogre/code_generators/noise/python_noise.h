#if defined __APPLE_CC__
    #include "strings.h"
    #include "Carbon/Carbon.h"
#endif
#include "noise.h"
#include "noiseutils.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_noise_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//


namespace python_noise{ namespace details{
inline void instantiate(){
 using namespace ::noise;
 //using namespace noise::model;
 //using namespace noise::module;
 //using namespace noise::utils;
 #include "python_noise_sizeof.h"
 
} } }


