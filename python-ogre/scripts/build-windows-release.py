#
# helper function to build release packages
#
import os, sys
from os.path import join, getsize
import shutil
import hashlib
import subprocess
from optparse import OptionParser
sys.path.insert(0,'..') # need this to get enrivonment.py into path
import environment
from utility import * # get samefile helper function

default_demo_data=['.log', '.pyc', 'ogre.cfg'] # first symbol indicates include or exclude
parent_dir= os.path.abspath(os.path.join(os.getcwd(), '..'))
print parent_dir
packages = {}  # global dictionary

def get_packages():
    """ build a list of packages and the classes in each
    """
    for name,cls in environment.projects.items():
        if cls.active and cls.pythonModule:
            if hasattr(cls,"package_name"):
                package_name = cls.package_name
            else: package_name = name.lower()
            if packages.has_key(package_name):
                # note: this single line doesn't work -- Python BUG ???-- need to expand !!!
                # packages[package_name] =  packages[package_name].append(cls)
                l = packages[package_name]
                l.append(cls)
                packages[package_name] = l
            else:
                packages[package_name] = [cls,]

def build_release( name ):
   master=set()
   get_packages()
   if not packages.has_key(name):
      exit("invalid module name:"+name)
   for cls in packages[name]:
      if hasattr(cls,"package_data_dirs"): package_data_dirs = cls.package_data_dirs
      else: package_data_dirs = {os.path.join('demos', name.lower()): default_demo_data }

      # walk the module tree and build a "Module" master file list
      moduleSource=os.path.normpath(os.path.join(".", environment.package_dir_name, cls.parent, cls.ModuleName))
      for f in os.listdir(os.path.join(parent_dir, moduleSource) ):
         root,ext = os.path.splitext(f)
         if ext in ('.py', '.pyd', '.dll', '.txt'):
            master.add(os.path.join(moduleSource,f )) # add the file name to the master list
      head, tail  = os.path.split ( moduleSource )
      while head:
         if os.path.isfile ( os.path.join(parent_dir,head,'__init__.py') ):
            master.add( os.path.join(head,'__init__.py') )
         head, tail  = os.path.split ( head )
         
      # now we do the same thing for the data directories
      for path in package_data_dirs.keys():
         for root, dirs, files in os.walk(os.path.join(parent_dir,path)):
            if not ".svn" in root:
               for f in files:
                  r,ext = os.path.splitext(f)
                  # the remove list can include specific files or extensions
                  if not f in package_data_dirs[path] and not ext in package_data_dirs[path]:
                     # add it and remove the leading parent dir from the path..
                     master.add(os.path.join(root, f)[len(parent_dir)+1:])

         
   # now we take the master list, and sort it just to make it pretty
   s = []
   for f in master:
      s.append(f)
   s.sort()
   return s
        
#        master.add()
#         for root, dirs, files in os.walk('python/Lib/email'):
#             print root, "consumes",
#             print sum(getsize(join(root, name)) for name in files),
#             print "bytes in", len(files), "non-directory files"
#             if 'CVS' in dirs:
#                 dirs.remove('CVS')  # don't visit CVS directories

def list_possible_modules():
    get_packages()
    for cls in packages:
        print cls
        
def parseInput():
    """Handle command line input """
    usage = "Usage: %prog [options] [module to test] \n\
      \n  Example: build a module\
      \n      %prog <modulename>\n"
    parser = OptionParser(usage=usage, version="%prog 1.0")
    parser.add_option("-v", "--verbose", action="store_true", default=False,dest="verbose", help="Output all output to stdout rather then saving to the log file")
    parser.add_option("-l", "--listmodules", action="store_true", default=False, dest="list_modules", help="List all available modules that can be built")
    parser.add_option("-f", "--logfilename",  default="log.out" ,dest="logfilename", help="Override the default log file name")
    (options, args) = parser.parse_args()
    return (options,args)

if __name__ == '__main__':
   (options, args) = parseInput()
   if options.list_modules:
      list_possible_modules()
      sys.exit()

   if len (args) != 1 :
      exit("You need to specify a module to build")
   setupLogging(options.logfilename)
   logger = logging.getLogger('PythonOgre.BuildRelease')

   # get the list of files that need packaging
   towrite = build_release ( args[0] )
   f =open (os.path.join(parent_dir,"__filelist"), "w")
   for s in towrite:
      f.write ( s )
      f.write ("\n")
   f.close()