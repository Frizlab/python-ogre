#
# helper function to update .dll's 
#
import os, sys
import shutil
sys.path.insert(0,'..') # need this to get enrivonment.py into path
import environment
for name,cls in environment.projects.items():
    if cls.active and cls.pythonModule:
        destpath=os.path.join("..", environment.package_dir_name, cls.parent, cls.ModuleName)
        for m in cls.moduleLibs:
            try:
                shutil.copy (  m +'.dll' , destpath )
                print "Copied", m +'.dll' , "to", destpath
            except:
                print "Error Copying", m +'.dll' , "to", destpath
                
