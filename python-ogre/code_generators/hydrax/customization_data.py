import os
def header_files( version ):
    ret= [ 'Hydrax.h',
            "ProjectedGrid.h",
            "SimpleGrid.h",
            "Perlin.h"
        ]
    if os.sys.platform == 'darwin':
        ret.insert(0,'Carbon/Carbon.h')
    return ret  
def huge_classes( version ):
    return []
   