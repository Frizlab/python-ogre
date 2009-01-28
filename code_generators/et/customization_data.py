import os
def header_files( version ):
    ret = ['ETTerrainInfo.h',
            'ETTerrainManager.h',
            'ETBrush.h',
            'ETSplattingManager.h',
            'Ogre.h',
            'OgreColourValue.h',
            'OgreImage.h'
            ]
    if os.sys.platform == 'darwin':
        ret.insert(0,'Carbon/Carbon.h')
    return ret
##,
##            'OgreColourValue.h'  
def huge_classes( version ):
    return []
   
