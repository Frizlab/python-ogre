# This code is in the public domain
import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.renderer.plsm2 as plsm
import SampleFramework as sf


# Listener
class MyCustomPagingLandScapeListener(plsm.CustomPagingLandScapeListener):
    def __init__(self,sm):
        plsm.CustomPagingLandScapeListener.__init__(self, sm)

    def terrainReady(self, p):
        print "ZZZZZZZZZZZZZZZZZZZZZZZZ Terrain Ready", dir(p)			

    def pagePreloaded(self, p) :
        print "XXXXXXXXXXXXXXXXXX Preloaded %d %d" % (p.mPagex, p.mPagez)
        
    def pageLoaded(self, p) :
        print "XXXXXXXXXXXXXXXXXX Page Loaded %d %d" % (p.mPagex, p.mPagez)

    def pageShow(self, p) :
        print "XXXXXXXXXXXXXXXXXX Page Show %d %d" % (p.mPagex, p.mPagez)

        
def testOcclusionBoundingBox():
    o = plsm.OcclusionBoundingBox()
    print dir(o)
def testOcclusion():
    o = plsm.Occlusion()
    print dir(o)
def testOcclusionElement():
    o = plsm.OcclusionElement()
    print dir(o)
def testVisibilityData():
    o = plsm.VisibilityData()
    print dir(o)
def testQuerySet():
    mQueryPool = plsm.QuerySet()
    mQueryPool.setRenderSystem()
    mQueryPool.setAutoextend (True)
    mQueryPool.setPoolSize (300)
    mIsQueryPoolNotInitiated = False
    #q.setPoolSize(10)
    print dir(mQueryPool)
    assert mQueryPool.getPoolSize()==300

def testOptions(s):
    sm = plsm.castAsSceneManager(s)
    print type(s)
    print type(sm)
    sVector = ogre.StringVector()
    options = sm.getOptionKeys(sVector)
    for option in sVector:
        print "Option: ", option
        svV = ogre.StringVector()
        values = sm.getOptionValues(option, svV)
        for v in  svV:
            print "Possible Value: ", v
    return        
    ret,w = sm.getOption ( 'Width' , 1)
    ret,h = sm.getOption ( 'Height', 1)
    ret, pageSize = sm.getOption( 1, 'PageSize', 1)
    ret, scaleX = sm.getOption (  'ScaleX', 1  )
    ret, scaleY = sm.getOption (  'ScaleY', 1  )
    ret, scaleZ = sm.getOption (  'ScaleZ', 1  )
    print "Width %d Height %d PageSize %d Scale %f %f %f" %(w,h, pageSize, scaleX, scaleY, scaleZ)
    

def testSceneManager(s):
    print "======================SceneManager===================="
    print s.getTypeName()
    print type(s)

    
def sanityTests(sm=None):
    testSceneManager(sm)
    testOcclusionBoundingBox()
    testQuerySet()
    testOcclusion()
    testOcclusionElement()
    testVisibilityData()
    testOptions(sm)
        
class TerrainApplication(sf.Application):

    def __del__(self):
        del self.myCustomPagingLandScapeListener
        # unregister our scenemanager factory  
        self.root.getSingleton().removeSceneManagerFactory(self.pagingLandScapePlugin)
        del self.pagingLandScapePlugin
        del self.root

    def _chooseSceneManager(self):
    
        # register our paging landscape scenemanager factory  
        self.pagingLandScapePlugin = plsm.PagingLandScapeSceneManagerFactory()
        self.root.getSingleton().addSceneManagerFactory(self.pagingLandScapePlugin)
        self.sceneManager = self.root.createSceneManager(ogre.ST_EXTERIOR_REAL_FAR, "PLSM2")
        
        print dir(self.sceneManager)
        self.sceneManager.setOption( 'ShowOctree', True)
        #ret,size = self.sceneManager.getOption ( 'Size', ogre.AxisAlignedBox() )
        
        
    def _createMap(self, map_name):
        if map_name == "alpes":
            self.camera.position = ogre.Vector3(633.658, 619.683, 1108.19)
        elif map_name == "hf":
            self.camera.position= ogre.Vector3(-4098.0, 30644.0,293.0)
        elif map_name == "ps":
            self.camera.position= ogre.Vector3(41000.0, 674000.0,8531.0)
        elif map_name == "grand":
            self.camera.position= ogre.Vector3(-4702.0, 250245.0,-19140.0)
 

    def _createCamera(self):
        "Create our camera."
        self.camera = self.sceneManager.createCamera('PlayerCam')

        map_name = "alpes" 
        #map_name = "hf" 
        #map_name = "ps" 
        #map_name = "grand" 
        

        if map_name == "alpes":
            self.camera.position = ogre.Vector3(633.658, 619.683, 1108.19)
        elif map_name == "hf":
            self.camera.position= ogre.Vector3(-4098.0, 30644.0,293.0)
        elif map_name == "ps":
            self.camera.position= ogre.Vector3(41000.0, 674000.0,8531.0)
        elif map_name == "grand":
            self.camera.position= ogre.Vector3(-4702.0, 250245.0,-19140.0)

        self.camera.lookAt((0, 0, -300))
        self.camera.nearClipDistance = 1

        print dir(self.sceneManager)


        rendSysCap = self.root.getRenderSystem().getCapabilities()
        # infinte far clip plane?
        if rendSysCap.hasCapability(ogre.RSC_INFINITE_FAR_PLANE):
            self.camera.farClipDistance = 0
        else:
            self.camera.farClipDistance = 100000


   
            
    def _createScene(self):
        sceneManager = self.sceneManager
        sceneManager.setSkyDome(True, 'Examples/CloudySky', 5, 8)

        sceneManager.setWorldGeometry('paginglandscape2.cfg')


        camera = self.camera
       
        sceneManager.AmbientLight = (0.5, 0.5, 0.5)

        self.player = sceneManager.createEntity('Player', 'ninja.mesh')
        player_node = sceneManager.getRootSceneNode().createChildSceneNode(ogre.Vector3(0,0,0))
        player_node.attachObject(self.player)
       
        fadeColour = ogre.ColourValue(0.93, 0.86, 0.76)
        #sceneManager.setFog(ogre.FOG_LINEAR, fadeColour, 0, 500.0, 1000.0)
        self.renderWindow.getViewport(0).backgroundColour = fadeColour

   
        # tests
        sanityTests(sceneManager)
        myCustomPagingLandScapeListener = MyCustomPagingLandScapeListener(self.sceneManager)
        print "ZZZZZZZZZZZZZZZZZZZZZZZZZZ", dir(myCustomPagingLandScapeListener)
        self.myCustomPagingLandScapeListener = myCustomPagingLandScapeListener
   
   
    def _createFrameListener(self):
        self.frameListener = TerrainListener( self.renderWindow,
                                              self.camera,
                                              self.sceneManager )
        self.root.addFrameListener(self.frameListener)

class TerrainListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, sceneManager):
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.sceneManager = sceneManager
        self.moveSpeed = 50.0
        self.raySceneQuery = sceneManager.createRayQuery(ogre.Ray(camera.getPosition(),
                                                         ogre.Vector3().NEGATIVE_UNIT_Y))
        self.camera = camera
        self.camera.position = self.camera.position + ogre.Vector3(0.0, 10.0, 0.0)

    def frameStarted(self, frameEvent):
        #print self.camera.getPosition()
        return sf.FrameListener.frameStarted(self, frameEvent)

        # clamp to terrain
        updateRay = ogre.Ray()
        updateRay.origin = self.camera.position + ogre.Vector3(0.0, 10.0, 0.0)
        updateRay.direction = ogre.Vector3().NEGATIVE_UNIT_Y
        self.raySceneQuery.ray = updateRay
        for queryResult in self.raySceneQuery.execute():
            if queryResult.worldFragment is not None:
                pos = self.camera.position
                #if (pos.y<18700):
                self.camera.setPosition( pos.x, 
                                         pos.y - queryResult.distance + 15.0,
                                         pos.z )
                break

if __name__ == '__main__':
    try:
        application = TerrainApplication()
        application.go()
    except ogre.OgreException, e:
        print e 
