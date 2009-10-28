#
# module to assit with versioning on python-ogre
#
import os, sys, time, shutil
import logging
#import BuildModule as bm
import ogre.renderer.OGRE as o

#add environment to the path
sys.path.append(  '.' )
sys.path.append(  os.path.join(os.getcwd(), 'plugins' ))

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
        print "Module %s ( %s )" % (basename, name)                
        print "   Module Version: %s" % (getattr(mod, "Version__") )
        print "   Compiled      : %s %s" % (getattr(mod, 'CompileTime__'), getattr(mod, 'CompileDate__') )
        print "   Raw Detail    : %s" % (getattr(mod, 'PythonOgreDetail__') )
    except ImportError:
        print "Import failed:", basename
    except TypeError:
        print "Type Error:"

        
if __name__ == '__main__':
    print "   PythonOgre Ver: %s" % (o.PythonOgreVersion__)
    print "   Python Version: %s" % (o.PythonVersion__)
    print
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
                