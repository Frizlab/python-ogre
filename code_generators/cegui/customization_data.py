        
def header_files( Version ):
    return [   "CEGUI.h"
            , "CEGUIXMLParser.h"
            , "OgreCEGUIRenderer.h"
            , "CEGUIXMLHandler.h"
            , "CEGUIXMLAttributes.h"
            , "CEGUIXMLSerializer.h"
            , "OgreCEGUIRenderer.h"
            ,"OgreCEGUIResourceProvider.h"
            , "OgreRenderWindow.h"  ## needed for Linux in OgreCEGUIRenderer.pypp.cpp
                  ]


def huge_classes( Version ):
    return []
