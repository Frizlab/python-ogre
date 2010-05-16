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

sys.path.insert(0,os.path.join('..', 'code_generators')) # need this to get svn version from common_utils
import common_utils

import environment

from utility import * # get samefile helper function

default_demo_data=['.log', '.pyc', '.log','ogre.cfg'] # first symbol indicates include or exclude
parent_dir= os.path.abspath(os.path.join(os.getcwd(), '..'))
temp_area = os.path.join(parent_dir, "temp")
packager = '"\\Program Files\\7-Zip\\7z.exe"'
uploader = '"\\Program Files\\PuTTY\\pscp"'
upload_dest = 'frs.sourceforge.net:/home/pfs/project/p/py/python-ogre/Latest/1.7.1'

if not os.path.isdir(temp_area):
   os.mkdir(temp_area)
packages = {}  # global dictionary

def get_svn_rev():
   i = common_utils.getSVNVersion (environment)
   return i.split(':')[0]
     
def get_packages():
    """ build a list of packages and the classes in each
    """
    for name,cls in environment.projects.items():
        if cls.pythonModule and (cls.active or not hasattr(cls, 'parent')):
            packs = []
            if hasattr(cls,"package_name"):
               if isinstance (cls.package_name, str):
                  packs.append(cls.package_name)  #only one package
               else: # it must be a list of packages that the module can be included in
                  packs = cls.package_name
            else:
               packs.append(name.lower())  # use the module name
#             # lets add the tail of parent to the possibilities as well
#             if hasattr(cls,'parent'):
#                possible = cls.parent.rsplit('/',1)[1]
#                packs.append(possible)
            for package_name in packs:
               if packages.has_key(package_name):
                   # note: this single line doesn't work -- Python BUG ???-- need to expand !!!
                   # packages[package_name] =  packages[package_name].append(cls)
                   l = packages[package_name]
                   l.append(cls)
                   packages[package_name] = l
               else:
                   packages[package_name] = [cls,]

def spawn_task ( task, directory, verbose=False ):
   if verbose:
      print "Spawning:", task
      out = err = ""
      process = subprocess.Popen (task, shell=True, cwd = directory)
      returncode = process.wait()
   else:
      process = subprocess.Popen (task, shell=True,stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd = directory)
      try:
         out,err = process.communicate()
         returncode = process.returncode
      except:
         returncode = -1

   if returncode != 0:
      logger.warning ( "Task Failed" )
      logger.debug ( out )
      logger.debug ( err )
      return False
   else:
      logger.debug ( "OK:" + out )
      logger.debug ( "OK:" + err )
   return True
   
def create_package ( name, filelist ):
   global logger
   listfile = "__"+filename+".lst"
   packagefile = filename+".7z"
   # delete the existing archive file if it exists
   if os.path.isfile (os.path.join(temp_area,packagefile)):
      os.remove(os.path.join(temp_area,packagefile))
   # create the list of files to be archived
   f=open (os.path.join(temp_area,listfile), "w")
   for s in filelist:
      f.write ( s )
      f.write ("\n")
   f.close()
   task = packager + " a " + os.path.join(temp_area,packagefile )  + " @" +os.path.join(temp_area, listfile)
   return spawn_task ( task, parent_dir )

def upload_file ( filename, username, password='' ):
   """
   uploader media-1.7.1-r1113.7z username@upload_dest
   """
   if password:
      options = ' -pw ' + password + ' '
   else: options = ' '
   task = uploader + options + filename + ' ' + username + '@' + upload_dest
   return spawn_task (task, '.', True)

def build_file_list( name ):
   master=set()
   get_packages()
   if not packages.has_key(name):
      exit("invalid module name:"+name)
   for cls in packages[name]:
      if hasattr(cls,"package_data_dirs"): package_data_dirs = cls.package_data_dirs
      else: package_data_dirs = {os.path.join('demos', cls.ModuleName.lower()): default_demo_data }

      # check if it's a 'module/package' with module files-- ie the dummy 'media' package doesn't
      if hasattr(cls, 'parent'):
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
         npath = os.path.normpath(path)
         if len(package_data_dirs[path])>0 and package_data_dirs[path][0] == '+':
            # we want specific files from a single directory
            for f in os.listdir(os.path.join(parent_dir,npath)):
               r,ext = os.path.splitext(f)
               if f in package_data_dirs[path] or ext in package_data_dirs[path]:
                  master.add(os.path.join(npath, f))
         else:
            # we want everything in a tree with certain exceptions
            for root, dirs, files in os.walk(os.path.join(parent_dir,npath)):
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
    #print packages
    for key in packages:
      print key,
      classes = packages[key]
      if len(classes)>1:
         print ":",
         for c in classes:
            print c.name,
      print
      
def parseInput():
    """Handle command line input """
    usage = "Usage: %prog [options] [module to test] \n\
      \n  Example: build a module\
      \n      %prog <modulename>\n"
    parser = OptionParser(usage=usage, version="%prog 1.0")
    parser.add_option("-v", "--verbose", action="store_true", default=False,dest="verbose", help="Output all output to stdout rather then saving to the log file")
    parser.add_option("-l", "--listmodules", action="store_true", default=False, dest="list_modules", help="List all available modules that can be built")
    parser.add_option("-f", "--logfilename",  default="log.out" ,dest="logfilename", help="Override the default log file name")
    parser.add_option("-u", "--username",  default="" ,dest="username", help="Soruceforge username for file upload")
    parser.add_option("-p", "--password",  default="" ,dest="password", help="Sourceforge password for file upload")
    (options, args) = parser.parse_args()
    return (options,args)

if __name__ == '__main__':
   global logger
   logger = None
   (options, args) = parseInput()
   if options.list_modules:
      list_possible_modules()
      sys.exit()

   if len (args) < 1 :
      exit("You need to specify a module to build")
   setupLogging(options.logfilename)
   logger = logging.getLogger('PythonOgre.BuildRelease')
   svn_ver =  get_svn_rev()
   for mod in args:
      print "Processing:", mod
      # filename to use
      filename = mod +'-'+environment.ogre.version+'-r'+ svn_ver
      # get the list of files that need packaging
      filelist = build_file_list ( mod )
      status = create_package ( filename, filelist )
      if status and options.username:
         status = upload_file (os.path.join(temp_area,filename+'.7z'), options.username, options.password )
      print status