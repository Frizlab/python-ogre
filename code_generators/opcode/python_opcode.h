
#undef _MSC_VER
#include "Opcode.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_opcode_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_opcode{ namespace details{
inline void instantiate(){
 using namespace Opcode;
 #include "python_opcode_sizeof.h"
 
} } }


