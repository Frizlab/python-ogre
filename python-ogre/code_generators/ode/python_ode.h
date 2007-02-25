#ifndef __python_ode_h_01
#define __python_ode_h_01

//See best practices section in Py++ documentation

#include <vector>
#include <string>
#include <map>

#include "ode/ode.h"
#include "ode/odecpp.h"
#include "ode/odecpp_collision.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_ODE_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_ODE{ namespace details{
inline void instantiate(){
 #include "python_ODE_sizeof.h"
 
} } }


#endif