#ifndef QUICKGUIEXPORTDLL_H
#define QUICKGUIEXPORTDLL_H

#ifdef QUICKGUI_LIB
    #define _QuickGUIExport 
#else
    #ifdef QUICKGUI_EXPORTS 
        #define _QuickGUIExport __declspec(dllexport) 
    #else 
        #define _QuickGUIExport __declspec(dllimport) 
    #endif
#endif

#endif
