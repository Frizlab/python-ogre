#
# helper function to update .dll's 
#
import os, sys
import shutil
import hashlib
sys.path.insert(0,'..') # need this to get enrivonment.py into path
import environment
from utility import * # get samefile helper function

# Handle majority of cases where needed dlls are configured in environment.py
for name,cls in environment.projects.items():
    if cls.active and cls.pythonModule:
        destpath=os.path.join("..", environment.package_dir_name, cls.parent, cls.ModuleName)
        for neededLib in cls.moduleLibs:
            (path,filename) = os.path.split ( neededLib )
            filename = filename +".dll"
            try:
                if not samefile ( neededLib +'.dll' , destpath + "/" + filename   ):
                    shutil.copy (  neededLib +'.dll' , destpath )
                    print "Copied", neededLib +'.dll' , "to", destpath
                else:
                    print "Already Exists:", filename ,"\tin", destpath
            except:
                print "Error Copying", neededLib +'.dll' , "to", destpath
                
# now handle ../plugins
srcpath=os.path.join(environment.Config.PATH_Ogre, 'bin','release')
destpath= os.path.join (environment.root_dir, 'plugins') 
for filename in os.listdir(srcpath):
    ext = os.path.splitext(filename)[1]
    if ext == '.dll' and (filename.startswith ('Plug') or filename.startswith ('Render') ):
        try:
            if not samefile ( os.path.join(srcpath,filename), os.path.join(destpath, filename) ):
                shutil.copy ( os.path.join(srcpath,filename), destpath )
                print "Copied", filename , "to", destpath
            else:
                print "Already Exists:", filename ,"\tin", destpath
        except:
            print "Error Copying", filename , "to", destpath
