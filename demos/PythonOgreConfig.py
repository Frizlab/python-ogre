#
# a single config file that is imported 'first' by the Python-Ogre demos
# this is primarly used to manage Linux demos when we have binaries in non standard (safe) locations

import os,sys
if os.name == 'nt':
    libpath=""
    pythonpath = ""
else:    
    localpath = os.path.abspath('.')   ## should be one of the demo directories
    libpath = os.path.abspath(os.path.join(localpath,'../../lib'))
    if not os.path.exists ( libpath ):   #we must be running from the development environment
        libpath = os.path.abspath(os.path.join(localpath,'../../../root/usr/lib'))
        if not os.path.exists ( libpath ):
            ## should die here
            print "**Problem finding libraries"
            libpath=localpath 
               
    ## path to the Python-Ogre modules        
    pythonpath = os.path.abspath(os.path.join(libpath, 'python2.5/site-packages') )
    ## and insert it into the system path
    sys.path.insert(0,pythonpath)
    
    ## now we need to set the library path so the shared libraries can be found
    try:
        ldpath = os.environ['LD_LIBRARY_PATH']
    except:
        ldpath=""
    newldpath = libpath + ":" + ldpath 
    os.environ['LD_LIBRARY_PATH'] = newldpath
    
