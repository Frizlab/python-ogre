#
# helper function to update .dll's 
#
import os, sys
import shutil
import hashlib
sys.path.insert(0,'..') # need this to get enrivonment.py into path
import environment
from optparse import OptionParser

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

def getLibs( ):
    # Handle majority of cases where libs can be found from environment.py
    libs=set()
    libDirs=set()
    for name,cls in environment.projects.items():
        if cls.active and cls.pythonModule:
            for neededLib in cls.libs:
                libs.add ( neededLib )
            for p in cls.lib_dirs:  
                libDirs.add(p)
    return libs, libDirs
                    
def copyFile ( filename, srcPath, destPath ):
    if not samefile ( os.path.join(srcPath, filename), os.path.join(destPath, filename) ):
        shutil.copy ( os.path.join(srcPath,filename), destPath )
        print "Copied", filename , "to", destPath
    else:
        print "Already Exists:", filename ,"\tin", destPath

def parseInput():
    """Handle command line input """
    usage = "Usage: %prog destination_dir [options]  \n\
      \n  Finds all development libraries and copies them to destination"
    parser = OptionParser(usage=usage, version="%prog 1.0")
    parser.add_option("-c", "--clean", action="store_true", default=False,dest="clean", help="clean (delete) lib files in the destination directory first")
    (options, args) = parser.parse_args()
    return (options,args)
            
if __name__ == '__main__':
    (options, args) = parseInput()

    if len (args) == 0 :
        exit("You must specifiy a destination directory")

    destPath = args[0]    
    if not os.path.exists( destPath ):
        os.mkdir ( destPath )
    if options.clean:
        print "Need to delete library files"
    libs, libDirs = getLibs()
    for l in libs:
        for srcPath in libDirs:
            filename = l + ".lib"
            if os.path.exists( os.path.join (srcPath, filename )): 
                copyFile ( filename, srcPath, destPath )
                break