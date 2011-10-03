from optparse import OptionParser
import sys
import os
import time
import hashlib
import shutil
    
def samefile ( originalFile, latestFile):
    if not os.path.exists( latestFile ):
        return False
    if not os.path.exists( originalFile ):
        return False
    inhash = hashlib.md5() 
    inhash.update ( open ( originalFile ).read() )
    outhash = hashlib.md5()
    outhash.update ( open ( latestFile ).read() )
    if inhash.digest() != outhash.digest():
        return False    
    return True
    
def makePath ( pathIn ):
   if not os.path.exists (pathIn ):
      os.makedirs ( pathIn )
            

def copyFile ( fileName, originalPath, latestPath, patchPath ):
   originalFile = os.path.join ( originalPath, fileName )
   latestFile = os.path.join ( latestPath, fileName)
   patchFile = os.path.join ( patchPath, fileName)
   # we only copy the file if it's a different size and or date
   if not samefile( originalFile ,latestFile ):
      makePath (patchPath)
      shutil.copyfile( latestFile, patchFile )
      print "Copying ", latestFile, " to ", patchFile
                                
def copyTree ( originalPath, latestPath, patchPath,
                      badPaths=['.svn', 'tofix', 'tests', 'wxOgre', 'showcase_01', 'media_old', 'nxogre', 'navi', 'tilemanager', 'dshow'],
                      badExtensions=['pyc','bak','log', 'ogre.cfg'],
                      files = [], recursive=True ):
    """ function to do a nice tree copy with file filtering and directory creation etc 
    """
            
    def filterExtensions ( listin, extensions):
        returnList=[]
        for i in listin:
            insertOK = True
            for e in extensions:
               if i.endswith (e):
                  insertOK = False
                  break
            if insertOK:
               returnList.append(i)
                     
#         for i in listin:
#             e=i.split('.')
#             if len(e) >0:
#                 if e[-1] not in extensions:
#                     returnList.append(i)
        return returnList  
        
 
    ## Main code starts here
    
    if not recursive: # Ok so we only care about a single directory 
        if files == []:
            files =  os.listdir( latestPath )   # get files
        if len( files ) > 0: 
            makePath ( patchPath )   # make a path if need be
            for fileName in files:
                copyFile ( fileName, originalPath, latestPath, patchPath)  # copy the file
                
    else:   # need to do a recursive copy
        level = 0
        for currentDir, dirs, files in os.walk(latestPath):
            # remember the initial directory so we can get the subdirs later
            if not level:
                rootPath= currentDir
                subPath = ""
                level += 1
            else:
                subPath = currentDir[ len (rootPath) + 1:] # extract the sub directory path
            pathOK = True
            for s in badPaths:
               if s in subPath:
                  pathOK=False
                  break
            if not pathOK:
               continue                 
            # filter the file names
            files = filterExtensions ( files, badExtensions )
            
            # Make all the directories in the dest Tree (for this level)
#             if len(files)>0:
#                makePath  (os.path.join (  patchPath, subPath))
#                print "makepath", patchPath, subPath
#                print files
#             for dirName in dirs:
#                 makePath ( os.path.join (  patchPath, subPath, dirName ) )
                
            # finally do the copy                
            for fileName in files:
                copyFile ( fileName, os.path.join( originalPath, subPath), currentDir, os.path.join(patchPath, subPath ) )
                    
def parseInput():
    """Handle command line input """
    usage = "usage: %prog [options] original new dest"
    parser = OptionParser(usage=usage, version="%prog 1.0")
#     parser.add_option("-v", "--verbose", action="store_true", default=False,dest="verbose", help="Output all output to stdout rather then saving to the log file")
#     parser.add_option("-l", "--logfilename",  default="log.out" ,dest="logfilename", help="Override the default log file name")

    (options, args) = parser.parse_args()
    return (options,args)

if __name__ == '__main__':
   PythonVersionString = str(sys.version_info[0]) + '.' + str(sys.version_info[1])
   
   directories = [ 'plugins', 'demos', 'packages_'+PythonVersionString ]
   files = ['ChangeLog.txt', 'Setup.py']
   
   original = "C:/PythonOgre845"
   latest = "c:/development/python-ogre"
   dest  = "c:/temp/patch"
   for d in directories:
      copyTree (os.path.join ( original, d), os.path.join(latest, d), os.path.join ( dest,d ) )
   for f in files:
      copyFile (f, original,latest, dest)
      