#include <sl.h>
#include <sm.h>
#include <ul.h>
#include <net.h>

#include <ssg.h>



// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_plib_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_plib{ namespace details{
inline void instantiate(){
 #include "python_plib_sizeof.h"
 
} } }


