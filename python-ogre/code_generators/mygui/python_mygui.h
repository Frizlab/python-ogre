
#include "MyGUI.h"
#include "MYGUI_RenderItem.h"
#include "MYGUI_LayerItemKeeper.h"
#include "MYGUI_LayerKeeper.h"
#include "MYGUI_WidgetSkinInfo.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus { namespace aliases {
    
 #include "python_mygui_aliases.h"
} } 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_mygui{ namespace details{
inline void instantiate(){
 using namespace MyGUI;
 #include "python_mygui_sizeof.h"
 
} } }


