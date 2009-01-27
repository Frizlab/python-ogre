import os        
def header_files( Version ):
    ret= [   "CEGUI.h"
            , "CEGUIXMLParser.h"
            , "Ogre.h"
            , "CEGUIXMLHandler.h"
            , "CEGUIXMLAttributes.h"
            , "CEGUIXMLSerializer.h"
            , "OgreCEGUIRenderer.h"
            ,"OgreCEGUIResourceProvider.h"
            ]
    if os.sys.platform == 'darwin':
      ret.insert(0,'Carbon/Carbon.h')
    return ret

def huge_classes( Version ):
    return []
