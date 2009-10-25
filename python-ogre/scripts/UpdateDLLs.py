#
# helper function to update .dll's 
#
import os, sys
import shutil
import hashlib
sys.path.insert(0,'..') # need this to get enrivonment.py into path
import environment

def samefile ( sourcefile, destfile):
    if not os.path.exists( destfile ):
        return False
    if not os.path.exists( sourcefile ):
        return False
        
    inhash = hashlib.md5() #needs python 2.5 or greater
    inhash.update ( open ( sourcefile ).read() )
    outhash = hashlib.md5()
    outhash.update ( open ( destfile ).read() )
    if inhash.digest() != outhash.digest():
        return False    
        
    ## probably don't need these as hash should handle it fine..    
    if os.stat(sourcefile).st_mtime > os.stat(destfile).st_mtime:
        return False
    if os.stat(sourcefile).st_size != os.stat(destfile).st_size:
        return False
    return True

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
                