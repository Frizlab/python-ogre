# this is a common import that we can use to ensure the demos run correctly
import sys
import os

## I want a version string 2.5 or 2.6 etc
PythonVersionString = str(sys.version_info[0]) + '.' + str(sys.version_info[1])

if os.name == 'posix': # linux or mac
    # add the search path to load modules from the build area...
    paths=[]
    paths.append(os.path.abspath( "../../packages_"+PythonVersionString))
    paths.append(os.path.abspath( "../packages_"+PythonVersionString)) # incase you are running from the ./python-ogre/demos directory
    paths.append(os.path.abspath( "./packages_"+PythonVersionString)) # incase you are running from the ./python-ogre
    
    for p in paths:
        if os.path.exists (p):
            sys.path.append ( p )
            break # only add one
            
    # anything special goes here...        
    if os.sys.platform == 'darwin': # it's a mac
        pass
    else: # linux
        pass
        
    
    
