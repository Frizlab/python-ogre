#
#  UnitTest the Python-Ogre Demo Files
#

## Curent

from optparse import OptionParser
import subprocess
import logging
import sys
import types
import os
import time

logger = None
FULL_LOGGING = False    # Set to true to log everything, even if successful
ENV_SET = False         # global to ensure we don't set the environment too many times and break the shell.
VERBOSE = False
ScreenShotPath = ""

def setupLogging (logfilename):
    # set up logging to file
    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
                        datefmt='%m-%d %H:%M',
                        filename=logfilename,
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
               
def exit( ExitMessage ):
     logger.error( ExitMessage )
     sys.exit( -1 )
     
def spawnTask ( task, cwdin = '', getoutput=None ):
    """Execute a command line task and manage the return code etc
    """
    global ENV_SET, ScreenShotPath
    if not os.path.exists ( ScreenShotPath ):
        logger.debug ( "Creating ScreenShot Directory: " + ScreenShotPath )
        os.path.mkdir ( ScreenShotPath )

    env = os.environ
    if not ENV_SET: # this actually changes the environment so we shouldn't do it more than once
        env["PythonOgreUnitTestPath"]=ScreenShotPath
        ENV_SET=True        
     
    logger.debug ( "Spawning '%s' in '%s'" % (task,cwdin) )

    if VERBOSE:
        out, err = "", ""
        process = subprocess.Popen (task, shell=True, cwd = cwdin, env=env)
        returncode = process.wait()
    else:
        process = subprocess.Popen (task, shell=True,stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd = cwdin, env=env)
        try:
            out,err = process.communicate()
            returncode = process.returncode
        except:
            returncode = -1
    
    if getoutput is not None:
        if returncode != -1:
            getoutput.write(out)
            getoutput.write(err)

    if returncode != 0:
        logger.warning ( "Task Failed" )
        logger.debug ( out )
        logger.debug ( err )
    elif FULL_LOGGING:
        logger.warning ( "Full Logging ON" )
        logger.debug ( out )
        logger.debug ( err )
            
    if returncode != 0 and FAILHARD:
        exit(" The following command failed %s" % task)
    return returncode     
    
def getDemoFiles ( parent, base ):
    knownBad=[] #'Demo_Basic.py', 'Demo_NetworkServer.py', 'Demo_NetworkClient.py','Demo_Compositor.py']
    ret = []
    p = os.path.abspath( os.path.join ( parent, base) )
    for f in os.listdir ( p ):
        if f.lower().startswith("demo") and f.lower().endswith(".py") and not f in knownBad:
            if os.path.isfile ( os.path.join(p,f) ):
                file = open ( os.path.join(p,f) )
                l = file.readline()
                if not "NO_UNITTEST" in l:
                    ret.append ( os.path.join(p,f) )
    return ret            
    
     
def runTest ( base, shortName, fullPath ):
    """ run the test
    """
    logger.info ( "Running test " + shortName )
    ret = spawnTask ( 'python ' +shortName+'.py', os.path.split(fullPath)[0] ) 


FAILHARD=False
def parseInput():
    """Handle command line input """
    usage = "Usage: %prog [options] [demo_program to test] \n\
      \n  Example: - run all tests in a number of demo directories\
      \n      %prog -d ogre -d cegui -d ogreode\n\
      \n  Example: - run a specific demo (base defaults to ogre)\
      \n      %prog demo_smoke\n"
    parser = OptionParser(usage=usage, version="%prog 1.0")
    parser.add_option("-v", "--verbose", action="store_true", default=False,dest="verbose", help="Output all output to stdout rather then saving to the log file")
    parser.add_option("-s", "--ScreenShotPath", default=os.path.join(os.getcwd(), "..", "screenshots"), action="store_true", dest="ScreenShotPath", help="Specify the directory for the snapshots")
    parser.add_option("-p", "--DemoParentPath", default=os.path.join(os.getcwd(), "..", "demos"), action="store_true", dest="DemoParentPath", help="Specify the base directory for the demo files")
    parser.add_option("-l", "--logfilename",  default="log.out" ,dest="logfilename", help="Override the default log file name")
    parser.add_option("-d", "--DemoBases", action="append", default=[], help="List the module demo locations you want to test")
    (options, args) = parser.parse_args()
    return (options,args)
    
if __name__ == '__main__':
    (options, args) = parseInput()
    if not options.DemoBases:
        options.DemoBases.append("ogre")
    
    if len (args) >0 and len (options.DemoBases) > 1:
        exit("You can't specify a demo and multiple bases")

        
    setupLogging(options.logfilename)
    logger = logging.getLogger('PythonOgre.UnitTest')
        
    VERBOSE=options.verbose
    ScreenShotPath = options.ScreenShotPath
    
    if not os.path.exists( ScreenShotPath ):
        os.mkdir ( ScreenShotPath )    

    if len (args) ==0 :
        ## run every demo we can find...
        for b in options.DemoBases:
            for d in  getDemoFiles ( options.DemoParentPath, b ):
                shortName = os.path.splitext( os.path.split(d)[1])[0]
                runTest ( b, shortName, d )
    else:
        shortName = args[0]
        b = options.DemoBases[0]
        d = os.path.abspath( os.path.join ( options.DemoParentPath, b, shortName ) )
        runTest ( b, shortName, d )
        