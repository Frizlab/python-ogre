import os        
def header_files( Version ):
    ret= [   "CEGUI.h"
            , "CEGUIXMLParser.h"
            , "Ogre.h"
            , "CEGUIXMLHandler.h"
            , "CEGUIXMLAttributes.h"
            , "CEGUIXMLSerializer.h"
            ]
    if Version.startswith ("0.6"):
        ret.append("OgreCEGUIRenderer.h")
        ret.append("OgreCEGUIResourceProvider.h")
    else:    
        ret.append("CEGUIOgreRenderer.h")
        ret.append("CEGUIOgreResourceProvider.h")


    if os.sys.platform == 'darwin':
      ret.insert(0,'Carbon/Carbon.h')
    return ret

def huge_classes( Version ):
    return []
