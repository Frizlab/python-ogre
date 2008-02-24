import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.addons.ogreforests as forests
import SampleFramework as sf
import math, random

class ForestApplication(sf.Application):
    def _chooseSceneManager(self):
        # self.sceneManager = self.root.createSceneManager("TerrainSceneManager")
        self.sceneManager = self.root.createSceneManager(ogre.ST_EXTERIOR_CLOSE)

    def getTerrainHeight( self, x, z, UserData=None ):
        if ( x < 0 or z < 0 or x > 1500 or z > 1500 ):  return 0
        self.updateRay.setOrigin( ogre.Vector3(x, 0.0, z) )
        self.updateRay.setDirection( ogre.Vector3.UNIT_Y )
        self.raySceneQuery.setRay( self.updateRay )
        self.raySceneQuery.execute( self.raySceneQueryListener )
        return self.raySceneQueryListener.resultDistance

    ##Loads world
    def _createScene(self):
        camera = self.camera
        
        ## Set ambient light
        self.sceneManager.setAmbientLight((0.5, 0.5, 0.5))
    
        ## Create a light
        l = self.sceneManager.createLight("MainLight")
        l.setPosition(20,80,50)
        ## Fog
        ## NB it's VERY important to set this before calling setWorldGeometry 
        ## because the vertex program picked will be different
        ##ColourValue fadeColour(0.93, 0.86, 0.76)
        fadeColour = (120.0/255, 173.0/255, 246.0/255)
        self.sceneManager.setFog( ogre.FOG_LINEAR, fadeColour, .001, 200, 1000)
        self.renderWindow.getViewport(0).setBackgroundColour(fadeColour)
    
        terrain_cfg = "./media/terrains/terrain.cfg"
    
        self.sceneManager.setWorldGeometry( terrain_cfg )
        ## Infinite far plane?
        if self.root.getRenderSystem().getCapabilities().hasCapability(ogre.RSC_INFINITE_FAR_PLANE):
            camera.setFarClipDistance(0)
        ##camera.setFarClipDistance(1000)
    
        ## Set a nice viewpoint
        camera.setPosition(750,100,750)
        camera.lookAt(750, 100, 750+1)
        ##camera.setPolygonMode(PM_WIREFRAME)
        
        self.updateRay = ogre.Ray();
    
        ##RaySceneQuery for terrain height check
        self.updateRay.setOrigin(camera.getPosition())
        self.updateRay.setDirection(ogre.Vector3.NEGATIVE_UNIT_Y)
        self.raySceneQuery = self.sceneManager.createRayQuery(self.updateRay)
        self.raySceneQuery.setQueryTypeMask(ogre.SceneManager.WORLD_GEOMETRY_TYPE_MASK )   
        self.raySceneQuery.setWorldFragmentType(ogre.SceneQuery.WFT_SINGLE_INTERSECTION) 
        self.raySceneQueryListener = myRaySceneQueryListener()
        
        ##---- PagedGeometry test ----
        ## TREES
        ##Initialize the PagedGeometry engine
        self.trees = forests.PagedGeometry()
        self.trees.setCamera( camera )
        self.trees.setPageSize( 50 )
        self.trees.setBounds( ogre.FloatRect(0, 0, 1500, 1500) )
    
        ##Set up LODs
        ##trees.addDetailLevelEntityPage(50)
        self.trees.addDetailLevelBatchPage(200, 50)
        self.trees.addDetailLevelImpostorPage(700, 50)
    
        ##Set up a TreeLoader for easy use
        self.treeLoader = forests.TreeLoader2D( self.trees, ogre.FloatRect(0, 0, 1500, 1500) )
        self.trees.setPageLoader(self.treeLoader)
        self.treeLoader.setHeightFunction(self, "getTerrainHeight")
    
        ##And add 20,000 trees to the scene through the "easy-to-use" TreeLoader class
        self.myTree = self.sceneManager.createEntity("MyTree", "tree2.mesh")
        x=0
        z=0
        for i in range (10000):
            yaw = random.randrange(0, 360)
    
            if (random.randrange(0, 1000) <= 800):
                x += random.randrange(-10.0, 10.0)
                z += random.randrange(-10.0, 10.0)
                if (x < 0): x = 0 
                elif (x > 1500): x = 1500
                if (z < 0): z = 0 
                elif (z > 1500): z = 1500
            else:
                x = random.randrange(0, 1500)
                z = random.randrange(0, 1500)
            
            scale = random.randrange(9, 11) / 10
            y = self.getTerrainHeight(x, z);

            self.treeLoader.addTree(self.myTree, ogre.Vector3(x, y, z), ogre.Degree(yaw), scale)
        
        ##Grass
        
        ##Initialize the PagedGeometry engine
        self.grass = forests.PagedGeometry(camera, 30)
        ##trees = new PagedGeometry(camera, 50, ogre.FloatRect(0, 0, 1500, 1500))
        
        ##Set up LODs
        self.grass.addDetailLevelGrassPage(80.0,0.0)
        
        ##Set up a TreeLoader for easy use
        self.grassLoader = forests.GrassLoader(self.grass)
        self.grass.setPageLoader(self.grassLoader)
        self.grassLoader.setHeightFunction(self, "getTerrainHeight")
    
        grassLayer = self.grassLoader.addLayer("grass")    
        
        grassLayer.setAnimationEnabled(True)
        grassLayer.setSwaySpeed(0.5)
        grassLayer.setSwayLength(0.05)
        grassLayer.setSwayDistribution(10.0)

        grassLayer.setDensity(0.6)

        grassLayer.setMapBounds(ogre.FloatRect(0, 0, 1500, 1500))

        grassLayer.setColorMap("terrain_texture.jpg")
        grassLayer.setColorMapFilter(forests.MAPFILTER_BILINEAR)

        grassLayer.setDensityMap("densitymap.png")
        grassLayer.setDensityMapFilter(forests.MAPFILTER_NONE)
    
        ##grassLayer.setMinimumSize(0.5,0.5)
        ##grassLayer.setMaximumSize(1.0, 1.0) 

        grassLayer.setMinimumSize(4.0,2.0)
        grassLayer.setMaximumSize(4.0,2.0)

        grassLayer.setFadeTechnique(forests.FADETECH_GROW)
        

    def _createFrameListener(self):
        self.frameListener = GrassFrameListener(self)
        self.root.addFrameListener(self.frameListener)
    
    
class GrassFrameListener(sf.FrameListener):
    def __init__(self, app):
        sf.FrameListener.__init__(self, app.renderWindow, app.camera)
        self.app = app
        # use this to tweak the persons position
        self.raySceneQuery = app.sceneManager.createRayQuery(ogre.Ray(app.camera.getPosition(),
                                                                    ogre.Vector3.NEGATIVE_UNIT_Y))

        
    def frameStarted(self, frameEvent):
        self.app.trees.update()
        self.app.grass.update()
        ##self.app.grassLoader.update() ## Animation()
        
        updateRay = ogre.Ray()
        updateRay.setOrigin (self.camera.getPosition() + ogre.Vector3(0.0, 10.0, 0.0))
        updateRay.setDirection (ogre.Vector3.NEGATIVE_UNIT_Y)
        self.raySceneQuery.Ray = updateRay
        for queryResult in self.raySceneQuery.execute():
            if queryResult.worldFragment is not None:
                pos = self.camera.getPosition()
                self.camera.setPosition (pos.x, pos.y - queryResult.distance + 18.0, pos.z)
                break
        return sf.FrameListener.frameStarted(self, frameEvent)


class myRaySceneQueryListener ( ogre.RaySceneQueryListener ):
    def queryResult(self, fragment,  distance):
      self.resultDistance = distance
      return False
          

if __name__ == '__main__':
    try:
        application = ForestApplication()
        application.go()
    except ogre.OgreException, e:
        print e
