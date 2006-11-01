#ifndef __python_CEGUI_h_01
#define __python_CEGUI_h_01

//See best practices section in Py++ documentation

#include <vector>
#include <string>
#include <map>
#include <iostream>


// #include <CEGUI/CEGUIImageset.h>
// #include <CEGUI/CEGUISystem.h>
// #include <CEGUI/CEGUILogger.h>
// #include <CEGUI/CEGUISchemeManager.h>
// #include <CEGUI/CEGUIWindowManager.h>
// #include <CEGUI/CEGUIWindow.h>
// #include <CEGUI/CEGUIPropertyHelper.h>
// #include <CEGUI/elements/CEGUICombobox.h>
// #include <CEGUI/elements/CEGUIListbox.h>
// #include <CEGUI/elements/CEGUIListboxTextItem.h>
// #include <CEGUI/elements/CEGUIPushButton.h>
// #include <CEGUI/elements/CEGUIScrollbar.h>

#include "CEGUI.h"
// #include "CEGUIXMLParser.h"
// #include "CEGUIXMLHandler.h"
// #include "CEGUIXMLAttributes.h"
// #include "CEGUIXMLserializer.h"
// #include "OgreCEGUIRenderer.h"
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"

// namespace  CEGUI {

// class EventConnection
// {
// public:
//     EventConnection();
//     bool connected();
//     void disconnect();
// };
// }
namespace python_CEGUI{ 

    inline void instantiate(){
        using namespace CEGUI;
    }

} 

#endif
