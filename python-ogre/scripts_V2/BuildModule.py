#
#  BuildModule will build a Python-Ogre module.
#

## Curent

from optparse import OptionParser
import subprocess
import environment
import logging
import sys
import types
import os

logger = None

def setupLogging ():
    # set up logging to file
    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
                        datefmt='%m-%d %H:%M',
                        filename='log.out',
                        filemode='w')
    # define a Handler which writes INFO messages or higher to the sys.stderr
    console = logging.StreamHandler()
    console.setLevel(logging.INFO)
    # set a format which is simpler for console use
    formatter = logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s')
    # tell the handler to use this format
    console.setFormatter(formatter)
    # add the handler to the root logger
    logging.getLogger('').addHandler(console)
  
    
def getClassList ():
    """ create a dictionary of classes from the enfironment modules
    """
    dict = {}
    for c in dir(environment):
        var = environment.__dict__[c]
        if isinstance ( var, types.ClassType ) :    ## OK so we know it's a class      
#             logger.debug ( "getClassList: Checking %s" % c )
            if hasattr(var, 'active') and hasattr(var, 'pythonModule'):   # and it looks like one we can about
#                 logger.debug ( "getClassList: it's one of ours")
                dict[c] = var
    return dict
                
def exit( ExitMessage ):
     logger.error( ExitMessage )
     sys.exit( -1 )

  
## This stuff has to be in my spawing sub process        
# # export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$PREFIX/lib/pkgconfig
# # export LD_LIBRARY_PATH=$PREFIX/lib
# # export CFLAGS="-I$PREFIX/include -L$PREFIX/lib"
# # export CXXFLAGS=$CFLAGS
# # export LDFLAGS="-Wl,-rpath='\$\$ORIGIN/../../lib' -Wl,-rpath='\$\$ORIGIN' -Wl,-z,origin"
# # export PATH=$PREFIX/bin:$PATH
# # export PYTHONPATH=$PREFIX/lib/python$PYTHONVERSION/site-packages     
     
     
def spawnTask ( task, cwdin = '' ):
    """Execute a command line task and manage the return code etc
    """
    PREFIX = environment.PREFIX
    PATH = os.environ["PATH"]
    env = os.environ
    env["PKG_CONFIG_PATH"]=PREFIX+"/lib/pkgconfig"
    env["LD_LIBRARY_PATH"]=PREFIX+"/lib"
    env["CFLAGS"]="\"-I"+PREFIX+"/include -L"+PREFIX+"/lib\""
    env["CXXFLAGS"]="\"-I"+PREFIX+"/include -L"+PREFIX+"/lib\""
    env["LDFLAGS"]="\"-Wl,-rpath='\$\$ORIGIN/../../lib' -Wl,-rpath='\$\$ORIGIN' -Wl,-z,origin\""
    env["PATH"]=PREFIX+"/bin:" + PATH
    env["PYTHONPATH"]=PREFIX+"/lib/python25/site-packages"

     
    logger.debug ( "Spawning '%s' in '%s' " % (task,cwdin) )
    process = subprocess.Popen (task, shell=True,stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd = cwdin, env=env)
    out,err = process.communicate()
    returncode = process.returncode
    if returncode != 0:
        logger.warning ( "Task Failed" )
        logger.debug ( out )
        logger.debug ( err )
    return returncode     
     
def retrieveSource ( module ):
    """ Here's we we retrieve the necessary source files
    """    
    for cmd,args,cwd in module.source:
        logger.info ("Retrieving %s" % args )
        ret = spawnTask ( cmd+ " " + args, cwd  )
        
def buildModule ( module ):
    """ Execute the build commands for the module
    """
    for type_,cmd,path  in module.buildCmds:
        if path =='':
            path = os.getcwd()
        if type_ == 0 : # it's a shell command
            logger.info ("Build Command %s " % cmd)
            ret = spawnTask ( cmd, path )
        elif type_ == 1 :
            logger.debug ("Compiling %s " % cmd )
            code = compile ( cmd, '<string>', 'exec' )
            logger.debug ("executing codeblock "  )
            exec code 
            logger.debug ( "Exec done")
            
def parseInput():
    """Handle command line input """
    usage = "usage: %prog [options] moduleName"
    parser = OptionParser(usage=usage, version="%prog 1.0")
    parser.add_option("-r", "--retrieve", action="store_true", default=True,dest="retrieve", help="Retrieve the appropiate source before building") #setting this to True !!
    parser.add_option("-n", "--nobuild", action="store_true", default=False ,dest="nobuild", help="Don't Build the appropiate module")
    (options, args) = parser.parse_args()
    return (options,args)
    
if __name__ == '__main__':
    setupLogging()
    logger = logging.getLogger('PythonOgre.BuildModule')
    classList = getClassList ()
    
    (options, args) = parseInput()
    if len(args) != 1:
        exit("The module to build wasn't specified.  Use -h for help")
        
    moduleName = args[0]        
    if not classList.has_key( moduleName ):
        exit("Module specificed was not found (%s is not in environment.py) " % moduleName )
    
    if not os.path.exists( environment.downloadPath ):
        os.mkdir ( environment.downloadPath )    
    if options.retrieve:    
        retrieveSource ( classList[ moduleName ] )
    if not options.nobuild :       
        buildModule ( classList[ moduleName ] )
    