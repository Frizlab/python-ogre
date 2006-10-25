def aliases( Version ):
    return {
        "Singleton<CEGUI::Logger>"
              : "SingletonLogger"
         ,"Singleton<CEGUI::FontManager>"
               : "SingletonFontManager"
         ,"Singleton<CEGUI::GlobalEventSet>"
               : "SingletonGlobalEventSet"
         ,"Singleton<CEGUI::ImagesetManager>"
               : "SingletonImagesetManager"
         ,"Singleton<CEGUI::MouseCursor>"
               : "SingletonMouseCursor"
         ,"Singleton<CEGUI::SchemeManager>"
               : "SingletonSchemeManager"
         ,"Singleton<CEGUI::System>"
               : "SingletonSystem"
         ,"Singleton<CEGUI::WidgetLookManager>"
               : "SingletonWidgetLookManager"
         ,"Singleton<CEGUI::WindowFactoryManager>"
               : "SingletonWindowFactoryManager"
         ,"Singleton<CEGUI::WindowManager>"
               : "SingletonWindowManager"
         ,"Singleton<CEGUI::WindowRendererManager>"
               : "SingletonWindowRendererManager"
    }

def header_files( Version ):
    return [ "CEGUI.h"
            , "CEGUIXMLParser.h"
              ]

def huge_classes( Version ):
    return []
