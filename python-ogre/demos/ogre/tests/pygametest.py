#Written by: Josh Taylor (deficite) <joshtaylor.mail@gmail.com>
import random, math,pygame
from pygame.locals import *
import ogre.renderer.OGRE as ogre
from ctypes import *

# from pygame.locals import *

#constants
WINSIZE = [640, 480]
WINCENTER = [320, 240]
NUMSTARS = 150
white = 255, 240, 200
black = 20, 20, 40
	
def init_star():
    "creates new star values"
    dir = random.randrange(100000)
    velmult = random.random()*.6+.4
    vel = [math.sin(dir) * velmult, math.cos(dir) * velmult]
    return vel, WINCENTER[:]


def initialize_stars():
    "creates a new starfield"
    stars = []
    for x in range(NUMSTARS):
        star = init_star()
        vel, pos = star
        steps = random.randint(0, WINCENTER[0])
        pos[0] = pos[0] + (vel[0] * steps)
        pos[1] = pos[1] + (vel[1] * steps)
        vel[0] = vel[0] * (steps * .09)
        vel[1] = vel[1] * (steps * .09)
        stars.append(star)
    move_stars(stars)
    return stars
    

def draw_stars(surface, stars, color):
    "used to draw (and clear) the stars"
    for vel, pos in stars:
        pos = (int(pos[0]), int(pos[1]))
        surface.set_at(pos, color)


def move_stars(stars):
    "animate the star values"
    for vel, pos in stars:
        pos[0] = pos[0] + vel[0]
        pos[1] = pos[1] + vel[1]
        if not 0 <= pos[0] <= WINSIZE[0] or not 0 <= pos[1] <= WINSIZE[1]:
            vel[:], pos[:] = init_star()
        else:
            vel[0] = vel[0] * 1.05
            vel[1] = vel[1] * 1.05
    

class PyGameOGREApp:
    "Provides a base for an application using PyGame and PyOgre"
    def __init__(self, width=640, height=480, fullscreen=True):
        self._initPyGame()
        self._initPyOgre()
        self._createWindow(width, height, fullscreen)
        self._createViewport()
        self._loadResources("resources.cfg")
        self._createEntities()

    def _initPyGame(self):
        "Starts up PyGame"
        pygame.init()

    def _initPyOgre(self):
        "Instantiates the PyOgre root and sceneManager objects and initialises"
        self.root = ogre.Root("plugins.cfg")
        self.root.showConfigDialog()
        self.root.initialise(False)
        self.sceneManager = self.root.createSceneManager(ogre.ST_GENERIC, "Default SceneManager")

    def _createWindow(self, width, height, fullscreen):
        "Creates a PyGame window and sets PyOgre to render onto it"
        self.screen = pygame.display.set_mode((width,height), pygame.DOUBLEBUF|pygame.HWSURFACE|pygame.FULLSCREEN )
        renderParameters = ogre.NameValuePairList()
        renderParameters['externalWindowHandle'] = str(pygame.display.get_wm_info()['window'])
        self.renderWindow = self.root.createRenderWindow('PyOgre through PyGame', width, height, fullscreen, renderParameters)
        self.renderWindow.active = True

    def _createViewport(self):
        "Creates the user's viewport and camera"
        self.camera = self.sceneManager.createCamera("camera")
        self.camera.position = (0, 0, 500)
        self.camera.lookAt((0, 0, -300))
        self.camera.nearClipDistance = 5
        self.viewport = self.renderWindow.addViewport(self.camera)
        self.viewport.backgroundColour = (0, 0, 0)

    def _loadResources(self, rcfile):
        "Loads the resource paths from specified resource config file"
        cf = ogre.ConfigFile()
        cf.load("resources.cfg")

        seci = cf.getSectionIterator()
        while seci.hasMoreElements():
            secName = seci.peekNextKey()
            settings = seci.getNext()

            for item in settings:
                typeName = item.key
                archName = item.value
                ogre.ResourceGroupManager.getSingleton().addResourceLocation(archName, typeName, secName)
        ogre.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()


    def _createEntities(self):
        "For simple demo purposes this can be used to create entities and attach them to sceneNode's"
        self.entities = [self.sceneManager.createEntity("robot", "robot.mesh")]
        self.sceneNodes = [self.sceneManager.getRootSceneNode().createChildSceneNode("robotNode")]
        for i in range(len(self.sceneNodes)):
            self.sceneNodes[i].attachObject(self.entities[i])
        #create our starfield
        random.seed()
        self.stars = initialize_stars()
        self.clock = pygame.time.Clock()
            
    def _createScene(self):
        "Prepare the scene. All logic should go here"
        draw_stars(self.screen, self.stars, black)
        move_stars(self.stars)
        draw_stars(self.screen, self.stars, white)
#         pygame.display.update()
        
    def _presentScene(self):
        "Render the scene and anything else needed done at the time of presentation"
#         return
        ogre.WindowEventUtilities().messagePump()
        self.root.renderOneFrame()
        pygame.display.flip()
        
    def run(self):
        "Brings the application to life"
        sceneManager = self.sceneManager
        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
#         sceneManager.setSkyBox(True, "Examples/SpaceSkyBox")
        
        # Need a light 
        light = sceneManager.createLight('MainLight')
        light.setPosition (20, 80, 50)
        while self._processEvents():
            self._createScene()
            self._presentScene()
#             self.clock.tick(50)
        pygame.quit()

    def _processEvents(self):
        "Process events and take appropriate action"
        for event in pygame.event.get():
            if event.type is pygame.QUIT:
                return False
            elif event.type is pygame.KEYDOWN and event.key is pygame.K_ESCAPE:
                return False
        return True

# Instantiate and run!
app = PyGameOGREApp()
    
app.run()
