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
import environment
#add common utils to the pass
if sys.platform == 'win32': 
    newpath = os.path.join ( os.path.abspath(os.path.dirname(__file__)), 'plugins')
    os.environ['PATH'] =  newpath +';' + os.environ['PATH']
    
      
if __name__ == '__main__':

    moduleList =      ( 'ogre.renderer.OGRE',
                        'ogre.io.OIS', 
                        'ogre.gui.betagui', 'ogre.gui.CEGUI', 'ogre.gui.QuickGUI',
                        'ogre.sound.OgreAL', 
                        'ogre.physics.ODE', 'ogre.physics.OgreOde', 
                        'ogre.physics.OgreNewt', 'ogre.physics.Opcode',
                        'ogre.physics.PhysX','ogre.physics.NxOgre',
                        'ogre.physics.bullet','ogre.physics.OgreBulletC', 'ogre.physics.OgreBulletD',
                        'ogre.addons.ogrevideo', 'ogre.addons.plib',
                        'ogre.addons.ogreforests', 'ogre.addons.et', 'ogre.addons.caelum',
                        'ogre.addons.noise', 
                        'ogre.addons.particleuniverse', 'ogre.addons.cadunetree',
                        'ogre.renderer.ogrepcz', 'ogre.addons.hydrax',
                        'ogre.gui.hikari','ogre.gui.canvas','ogre.gui.mygui',
                        'ogre.addons.raknet',
                        'ogre.renderer.plsm2' )
                        
#    bm.setupLogging("version.info") # options.logfilename)
#    logger = logging.getLogger('PythonOgre.ReportVersionInfo')
    #for name in moduleList:
    print "   PythonOgre Ver: %s" % (o.PythonOgreVersion__)
    print "   Python Version: %s" % (o.PythonVersion__)
    print
    for name,cls in environment.projects.items():
        if cls.active and cls.pythonModule:
            name='.'.join([cls.parent.replace('/','.'), cls.ModuleName])
            #logger.info ("Attemping to import " + name )
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
    #             logger.info ( "Import Successful: " + basename )
            except ImportError:
#                pass
                print "Import failed:", basename
     #           logger.error ( "Import failed:" + basename )
            except TypeError:
                print "Type Error:"
