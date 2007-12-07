// keep everything in lowercase (except the namespace which must match the C++ library) to stop Linux/Windows issues
#include "ParticleUniverseSystemManager.h"  // the master include for the c++ library

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_particleuniverse_aliases.h"   // we use this to fix up what would otherwise be ugly names
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_particleuniverse{ namespace details{
inline void instantiate(){
 using namespace ParticleUniverse;              // note the use of the 'actual' namespace here -- case sensitive
 #include "python_particleuniverse_sizeof.h"    // this is used to expose classes/variables that don't happen automatically
 
} } }


