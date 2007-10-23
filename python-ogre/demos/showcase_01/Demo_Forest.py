



import warnings 
warnings.simplefilter('ignore', RuntimeWarning) 
import ogre.renderer.OGRE as ogre
import ogre.addons.ogreforests as forests
import SampleFramework as sf
import math, random
warnings.simplefilter('default', RuntimeWarning)

class ForestApplication(sf.Application):
    def _chooseSceneManager(self):
        # self.sceneManager = self.root.createSceneManager("TerrainSceneManager")
        self.sceneManager = self.root.createSceneManager(ogre.ST_EXTERIOR_CLOSE)
        # self.sceneManager.setShadowTechnique(ogre.SHADOWTYPE_TEXTURE_ADDITIVE)
        
    def getTerrainHeight( self, x, z ):
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
    
        terrain_cfg = "./triplanar_terrain.cfg"
    
        self.sceneManager.setWorldGeometry( terrain_cfg )
        ## Infinite far plane?
        if self.root.getRenderSystem().getCapabilities().hasCapability(ogre.RSC_INFINITE_FAR_PLANE):
            camera.setFarClipDistance(0)
        ##camera.setFarClipDistance(1000)
    
        ## Set a nice viewpoint
        camera.setPosition(750,100,750)
        camera.lookAt(750, 100, 750+1)
        ##camera.setPolygonMode(PM_WIREFRAME)

        plane = ogre.Plane()
        n = plane.normal
        n.x, n.y, n.z = 0, -1, 0
        plane.d = 5000
        self.sceneManager.setSkyPlane(True, plane, 'Examples/CloudySky', 1000, 3)

        #self.sceneManager.setSkyPlane(True, 'Examples/CloudySky', 5, 8)

        
        ##---- PagedGeometry test ----
        
        ## TREES
        ##Initialize the PagedGeometry engine
        self.trees = forests.PagedGeometry()
        self.trees.setCamera( camera )
        self.trees.setPageSize( 100 )
        self.trees.setBounds( ogre.FloatRect(0, 0, 1500, 1500) )
    
        ##Set up LODs
        ##self.trees.addDetailLevelEntityPage(50, 50)
        self.trees.addDetailLevelBatchPage(50, 50)
        self.trees.addDetailLevelImpostorPage(900, 50)
    
        ##Set up a TreeLoader for easy use
        self.treeLoader = forests.TreeLoader2D( self.trees, ogre.FloatRect(0, 0, 1500, 1500) )
        self.trees.setPageLoader(self.treeLoader)
        self.treeLoader.setHeightFunction(self, "getTerrainHeight")
    
        ##And add 20,000 trees to the scene through the "easy-to-use" TreeLoader class
        palm1 = self.sceneManager.createEntity("MyTree", "KinkyPalm.mesh")
        palm2 = self.sceneManager.createEntity("pineTree", "StraightPalm.mesh")
        palm3 = self.sceneManager.createEntity("palmBendy", "BendyPalm.mesh")
        bush1 = self.sceneManager.createEntity("bush1", "BigDarkBush.mesh")
        birch = self.sceneManager.createEntity("birch", "birch.mesh")
        benTree = self.sceneManager.createEntity("benTree", "benTree.mesh")
        spot_shroom = self.sceneManager.createEntity("redShroom", "spotty_shroom.mesh")
        blue_shroom = self.sceneManager.createEntity("blueShroom", "blue_shroom.mesh")
        beech = self.sceneManager.createEntity("beech", "beech_tree.mesh")

        self.treeModels = [ bush1, benTree, spot_shroom, blue_shroom, beech]
        # fix the damn rotations
##        rotNode = self.sceneManager.getRootSceneNode().createChildSceneNode( "rotNode" )
##        rotNode.attachObject( self.palm1 )
##        rotNode.attachObject( self.palm2 )
##        rotNode.attachObject( self.palm3 )
##        rotNode.attachObject( self.bush1 )
##        rotNode.pitch(ogre.Degree(-90).valueRadians())
        
        
        
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
            
            scale = random.randrange(9, 20) / 10

            t_type = random.randrange(0, len(self.treeModels))
            self.treeLoader.addTree(self.treeModels[t_type], ogre.Vector2(x, z), ogre.Degree(yaw), scale)

##            if t_type == 0:
##                self.treeLoader.addTree(self.palm1, ogre.Vector2(x, z), ogre.Degree(yaw), scale)
##            elif t_type == 1:
##                self.treeLoader.addTree(self.palm2, ogre.Vector2(x, z), ogre.Degree(yaw), scale)
##            elif t_type == 2:
##                self.treeLoader.addTree(self.palm3, ogre.Vector2(x, z), ogre.Degree(yaw), scale)
##            elif t_type == 3:
##                self.treeLoader.addTree(self.bush1, ogre.Vector2(x, z), ogre.Degree(yaw), scale)
        ##Grass
        
        ##Initialize the PagedGeometry engine
        self.grass = forests.PagedGeometry(camera, 30)
        ##trees = new PagedGeometry(camera, 50, ogre.FloatRect(0, 0, 1500, 1500))
        
        ##Set up LODs
        self.grass.addDetailLevelGrassPage(100.0,0.0)
        
        ##Set up a TreeLoader for easy use
        self.grassLoader = forests.GrassLoader(self.grass)
        self.grass.setPageLoader(self.grassLoader)
        self.grassLoader.setHeightFunction(self, "getTerrainHeight")

        ## iterate grass materials
        for nam in ["grass", "dry_grass", "tall_grass"]:
            grassLayer = self.grassLoader.addLayer(nam)
            
            grassLayer.setAnimationEnabled(True)
            grassLayer.setSwaySpeed(1.5)
            grassLayer.setSwayLength(0.08)
            grassLayer.setSwayDistribution(10.0)

            grassLayer.setDensity(0.6)

            grassLayer.setMapBounds(ogre.FloatRect(0, 0, 1500, 1500))

            grassLayer.setColorMap("terrain_texture_greeb.jpg")
            grassLayer.setColorMapFilter(forests.MAPFILTER_BILINEAR)

            grassLayer.setDensityMap("densitymap.png")
            grassLayer.setDensityMapFilter(forests.MAPFILTER_NONE)
        
            ##grassLayer.setMinimumSize(0.5,0.5)
            ##grassLayer.setMaximumSize(1.0, 1.0) 

            grassLayer.setMinimumSize(4.0,2.0)
            grassLayer.setMaximumSize(4.0,2.0)

            grassLayer.setFadeTechnique(forests.FADETECH_GROW)

        self.robots = []

        for i in range(5):
            robo = Robot(self)
            robo.spawn(ogre.Vector3(700 + (i * 10), 100, 750), i)
            self.robots.append(robo)


        
        #rNode.translate(ogre.Vector3(750,70,760))
        #rNode.setScale(ogre.Vector3(0.5,0.5,0.5))
        
        self.updateRay = ogre.Ray();
    
        ##RaySceneQuery for terrain height check
        self.updateRay.setOrigin(camera.getPosition())
        self.updateRay.setDirection(ogre.Vector3.NEGATIVE_UNIT_Y)
        self.raySceneQuery = self.sceneManager.createRayQuery(self.updateRay)
        self.raySceneQuery.setQueryTypeMask(ogre.SceneManager.WORLD_GEOMETRY_TYPE_MASK )   
        self.raySceneQuery.setWorldFragmentType(ogre.SceneQuery.WFT_SINGLE_INTERSECTION) 
        self.raySceneQueryListener = myRaySceneQueryListener()
        
        tipi_ent = self.sceneManager.createEntity("tipi", "tipi.mesh")
        tipi_node = self.sceneManager.getRootSceneNode().createChildSceneNode()
        tipi_node.attachObject(tipi_ent)
        tipi_node.setPosition(ogre.Vector3(700, 100, 750))
        pos = tipi_node.getPosition()
        tipi_node.setPosition(pos.x, self.getTerrainHeight(700,750) + 0, pos.z)

    def _createFrameListener(self):
        self.frameListener = GrassFrameListener(self)
        self.root.addFrameListener(self.frameListener)

        self.roboListener = robotsListener(self)
        self.root.addFrameListener(self.roboListener)


class Robot:
    def __init__(self, app):
        self.app = app
        self.speed = 5.0
        self.decisionTime = 5.0
        self.direction = ogre.Vector2(random.uniform(-1.0, 1.0), random.uniform(-1.0, 1.0))
        self.mustRotate = True
        
    def spawn(self, position, numActors):
        self.timer = 0.0
        self.ent = self.app.sceneManager.createEntity("robot_" + str(numActors), "robot.mesh")
        self.posNode = self.app.sceneManager.getRootSceneNode().createChildSceneNode()
        self.rotNode = self.posNode.createChildSceneNode()
        self.posNode.setScale(ogre.Vector3(0.1, 0.1, 0.1))
        self.rotNode.attachObject(self.ent)
        self.posNode.setPosition(position)
        self.rotNode.yaw(ogre.Degree(90))
        print self.posNode.position
        self.animation = self.ent.getAnimationState('Walk')
        self.animation.Enabled = True
        
        

    def addTime(self, time):
        self.animation.addTime(time)
        #print self.Node.position
        self.timer += time
        if self.timer >= self.decisionTime:
            self.timer = 0
            self.direction = ogre.Vector2(random.uniform(-1.0, 1.0), random.uniform(-1.0, 1.0))
            self.direction.normalise()
            self.mustRotate = True
            self.posNode.setDirection(self.direction.x, 0, self.direction.y, self.posNode.TS_WORLD)
            print self.posNode.position
        if self.mustRotate:
            pass
                
        pos = self.posNode.position
        off = self.direction * (self.speed * time)
        self.posNode.setPosition(pos.x + off.x, pos.y, pos.z + off.y)
        
        
        
class robotsListener(sf.FrameListener):
    def __init__(self, app):
        sf.FrameListener.__init__(self, app.renderWindow, app.camera)
        self.app = app
        self.raySceneQuery = app.sceneManager.createRayQuery(ogre.Ray(app.camera.getPosition(),
                                                            ogre.Vector3.NEGATIVE_UNIT_Y))
        self.Ray = ogre.Ray()

        
    def frameStarted(self, frameEvent):
        for r in self.app.robots:
            r.addTime(frameEvent.timeSinceLastFrame)
            self.Ray.setOrigin(r.posNode.position)
            self.Ray.setDirection(ogre.Vector3.NEGATIVE_UNIT_Y)
            self.raySceneQuery.Ray = self.Ray
            for queryResult in self.raySceneQuery.execute():
                if queryResult.worldFragment is not None:
                    pos = r.posNode.position
                    r.posNode.setPosition(pos.x, pos.y - queryResult.distance + 0.6, pos.z)
                    break
        return sf.FrameListener.frameStarted(self, frameEvent)
    
class GrassFrameListener(sf.FrameListener):
    def __init__(self, app):
        sf.FrameListener.__init__(self, app.renderWindow, app.camera)
        self.app = app
        self.timeSince = 0.0
        self.updateToggle = True
        # use this to tweak the persons position
        self.raySceneQuery = app.sceneManager.createRayQuery(ogre.Ray(app.camera.getPosition(),
                                                                    ogre.Vector3.NEGATIVE_UNIT_Y))

        
    def frameStarted(self, frameEvent):
        if self.updateToggle:
            self.app.trees.update()
        else:
            self.app.grass.update()
        self.updateToggle = not self.updateToggle
        self.app.grassLoader.updateAnimation()
        self.timeSince = frameEvent.timeSinceLastFrame
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

    def _moveCamera(self):
        self.camera.yaw(self.rotationX)
        self.camera.pitch(self.rotationY)
        try:
            self.camera.translate(self.translateVector * 0.1 * self.timeSince) # for using OgreRefApp
        except AttributeError:
            self.camera.moveRelative(self.translateVector * 0.1 * self.timeSince)


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
