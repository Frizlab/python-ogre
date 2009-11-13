#
# module to assit with versioning on python-ogre
#
import os, sys, time, shutil
import logging
#import BuildModule as bm
from optparse import OptionParser

import ogre.renderer.OGRE as o

#add environment to the path
sys.path.append(  '.' )
sys.path.append(  os.path.join(os.getcwd(), 'plugins' ))

_quiet = False

#add common utils to the pass
if sys.platform == 'win32': 
    newpath = os.path.join ( os.path.abspath(os.path.dirname(__file__)), 'plugins')
    os.environ['PATH'] =  newpath +';' + os.environ['PATH']
    
def reportDetails ( name ):
    try:
        basename = name.split('.')[-1]
        mod = __import__(name)
        components = name.split('.')
        for comp in components[1:]:
            mod = getattr(mod, comp)
        if _quiet:
            print "Module %s ( %s )" % (basename, name),   
            try:
                print "Version: %s" % (getattr(mod, "Version__") )
            except:
                print "UNKNOWN"
        else:
            print "Module %s ( %s )" % (basename, name)   
            try:
                print "   Module Version: %s" % (getattr(mod, "Version__") )
                print "   Compiled      : %s %s" % (getattr(mod, 'CompileTime__'), getattr(mod, 'CompileDate__') )
                print "   Raw Detail    : %s" % (getattr(mod, 'PythonOgreDetail__') )
            except AttributeError:
                print "*** Missing Attribute"
    except ImportError:
        if not _quiet:
            print "Import failed:", basename
    except TypeError:
        if not _quiet:
            print "Type Error:"

def parseInput():
    """Handle command line input """
    usage = "Usage: %prog [options] \n"
    parser = OptionParser( usage=usage, version="%prog 1.0")
    parser.add_option("-q", "--quiet", action="store_true", default=False,dest="quiet", help="Limit Output")
    (options, args) = parser.parse_args()
    return (options,args)
        
if __name__ == '__main__':
    (options, args) = parseInput()
    print "PythonOgre Ver: %s" % (o.PythonOgreVersion__)
    print "Python Version: %s" % (o.PythonVersion__)
    print

    if options.quiet: _quiet = True
    try:
        import environment  # in the development space
        for name,cls in environment.projects.items():
            if cls.active and cls.pythonModule:
                name='.'.join([cls.parent.replace('/','.'), cls.ModuleName])
                reportDetails ( name )
    except:
        import setup as s   # lets get the module list from setup.py
        for m in s.PACKAGEDATA["packages"]:
            if m.count('.') == 2: # it's a complete package name
                reportDetails ( m )
                