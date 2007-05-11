# import the ogre / ois modules
import os, sys
import ogre.renderer.OGRE as ogre
import ogre.io.OIS as OIS
import ogre.physics.OgreNewt as OgreNewt
from system.defaultFL import DefaultFrameListener
from system.dotscene import *
from system.datamanager import *

# try importing psyco
try:
    import psyco
    psyco.full()
except:
    print 'No Psyco'

# our main application
class TechDemoApp:
    
    def __init__(self):
        # declarations the basic parts
        self.frameListener = None
        self.plugins = ['RenderSystem_GL', 'Plugin_BSPSceneManager', 'Plugin_CgProgramManager', \
                        'Plugin_OctreeSceneManager', 'RenderSystem_Direct3D9', \
                        'Plugin_ParticleFX']
        self.paths = ['./plugins']
        self.root = None
        self.camera = None
        self.renderWindow = None
        self.sceneManager = None
        self.playerListener = None
        self.world = None
        
    def __del__(self):
        # delete in order
        del self.camera
        print '1'
        del self.sceneManager
        print '2'
        del self.mainFrameListener
        print '3'
        del self.datamanager
        print '4'
        del self.world
        print '5'
        del self.root
        print '6'
        del self.renderWindow
        print '7'
        print 'Application says Bye-Bye'
        

        
# -----------------------------------------------------------------------------
        
    def start(self):
        # start the ogre engine - checking for the user to cancel
        if not self.setupOgre():
            return False
        
        # User pressed 'ok', engine is now running (or crashed)
        # Now make the visual elements appear by calling our own handler
        print 'Visuals'
        self.setupVisuals()
        
        # call the resource manager setup routine
        print 'Resources'
        self.setupResources()
        
        # start our own dataManager class
        # this deals with all the loading and running of levels
        # datamanger is the only namespace with a permanent reference
        # to the physics world as well
        print 'Datamanager'
        self.datamanager = DataManager(self.sceneManager, self.viewport, self.renderWindow, \
                                        self.camera, 30, True) # True = createphysics
                                        
        #self.sceneManager.shadowTechnique = ogre.SHADOWTYPE_TEXTURE_ADDITIVE
        
        
        # Load our default frame listener. 
        # This is a kind of 'super' framelistener
        # it runs the datamanager,and simply listens
        # for a quit. other listeners are added later.
        print 'FrameListener'
        self.mainFrameListener = DefaultFrameListener(self.renderWindow,
                                                        self.camera, self.datamanager)
        self.root.addFrameListener(self.mainFrameListener)
        
        # Take the first action, and load the main menu
        #self.datamanager.showMainMenu()
        fadeColour = (0.90, 0.89, 0.89) # B G R
        
        self.sceneManager.setFog(ogre.FogMode.FOG_LINEAR, fadeColour, 0, 6000, 12000)
        self.sceneManager.shadowTechnique = ogre.SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED
        self.sceneManager.setShadowTextureSize(512)
        self.sceneManager.setShadowColour(ogre.ColourValue(0.2, 0.2, 0.2))
        self.renderWindow.getViewport(0).backgroundColour = fadeColour
        
        # render one Frame, to init the compositor etc.
        self.root.renderOneFrame()
        
        # Finally, kick off the main program loop
        print 'Render'
        while self.render():
            pass
        #self.root.startRendering()
        
    def render(self):
        return self.root.renderOneFrame()
        
# -----------------------------------------------------------------------------
        
    def setupOgre(self):
        # --this is where we get the engine running
        # --and show the config dialog
        
        # load the renderer libraries and initialize PO
        self.setupRoot()
        
        # optional frame smoothing setting
        self.root.setFrameSmoothingPeriod (10.0)
        
        # --Now we show the config dialog, and if
        # --the user says ok, start a new render window
        # --with those settings
        if self.root.showConfigDialog():
            # Create a window: automatically (True) and called 'Tek Demo'
            self.renderWindow = self.root.initialise(True, "Tek Demo")
            return True
        else:
            # Cancel was pressed, all we have to do is
            # return False to stop the app
            print "GAME: User cancelled"
            return False
        
    def setupRoot(self):
        self.root = ogre.Root('')
        extension = '.so'
        if 'win32' == sys.platform:
            extension = '.dll'
        # Loop over list of desired plugins
        for plugin in self.plugins:
            plugin_name = plugin + extension
            print' Searching for: %s' % plugin_name
            found = False
           
            # Check to see if plugin exists anywhere along the list of possible paths
            for dir in self.paths:
                plugin_path = os.path.join(dir, plugin_name)
               
                # If found, load it
                if os.path.exists(plugin_path):
                    self.root.loadPlugin(plugin_path)
                    found = True

            # Fail if we can't load a plugin
            if not found:
                raise GraphicsError('Could not load plugin: %s' % plugin_name)
        
        
    def setupVisuals(self):
        # --Start by choosing what type of scene to render
        # --This can be changed later on.
        # --We create a window, attach a camera
        # --Then set some view options and get a reference
        # --to the ogre Compositor for screen effects
        
        # Choose which scenemanager to use
        self.sceneManager = self.root.createSceneManager( ogre.ST_GENERIC,"DemoSM" )
        
        # create a camera, and set the view distance
        self.camera = self.sceneManager.createCamera('PlayerCam')
        self.camera.nearClipDistance = 0.1
        self.camera.setPosition( 0.0, 40, 350.0)
        self.camera.lookAt(ogre.Vector3(0, -50, -300))
        
        # debug Light Creation
        light = self.sceneManager.createLight('BlueLight')
        light.type = ogre.Light.LT_DIRECTIONAL
        light.setPosition (-200, 500, 100)
        dirvec = -light.getPosition()
        dirvec.normalise()
        light.setDirection(dirvec)
        light.setDiffuseColour(0.9, 0.9, 1.0)
        
        # connect the camera to the renderWindow
        self.viewport = self.renderWindow.addViewport(self.camera)
        
        # Enable shadows
        self.viewport.setShadowsEnabled(True)
        
        # set a background color
        self.viewport.backgroundColour = (0,0,0)
        

        
        # get a pointer to ogre's compositor manager system
        self.cms = ogre.CompositorManager.getSingleton()
        
    def setupResources(self):
        # --Many ways to do this:, here is a simple dirty python
        # --script parser, for those who dont like configfile iterators
        # --I dont really think it makes much impact on loading times.
        f = open('resources.cfg' ,'r').read().splitlines()
        for l in f:
            l = l.strip()
            if len(l)>0:
                if not l[0] == '#':
                    if "#" in l:
                        l = l.split('#')[0].strip()
                    if '[' in l:
                        section = l.strip('[]')
                        print section
                    else:
                        key, path = l.split('=')
                        ogre.ResourceGroupManager.getSingleton().addResourceLocation(path, key, section)
        print 'Resources Loaded'
                        
        # --and here is the iterator version
                        
        #config = ogre.ConfigFile()
        #config.load('resources.cfg' )
        #section_iter = config.getSectionIterator()
        #while section_iter.hasMoreElements():
        #   section_name = section_iter.peekNextKey()
        #   settings = section_iter.getNext()
        #   for item in settings:
        #       ogre.ResourceGroupManager.getSingleton().addResourceLocation(item.value, item.key, section_name)
        # Setup the number of mipmaps for all textures.
        ogre.TextureManager.getSingleton().defaultNumMipmaps = 5
        # Init the core resource group by default:
        ogre.ResourceGroupManager.getSingleton().initialiseResourceGroup('Core')
        return True
    
    def setupPhysics( self ):
        self.world = OgreNewt.World()
        
# Start the Game here
if __name__=='__main__':
    try:
        app =  TechDemoApp()
        app.start()
    except ogre.OgreException, e:
        print '## EXCEPTION ##'
        print str(e)

        
        
        
        
        
