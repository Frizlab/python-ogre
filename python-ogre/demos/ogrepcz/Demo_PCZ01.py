# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http://python-ogre.org/
#
# It is likely based on original code from OGRE and/or PyOgre
# For the latest info, see http://www.ogre3d.org/
#
#       Specialisation of OGRE's framework application to test/demo the
#       Portal Connected Zone (PCZ) Scene Manager Plugin.
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------
import sys
import os
sys.path.insert(0,'..')
import PythonOgreConfig

## this here becase the Plug_PCZSceneManager.dll needs to be accessed by both the python module and the ogre plugins
if sys.platform == 'win32': 
    newpath = os.path.join ( os.path.abspath(os.path.dirname(__file__)), '../../plugins')
    os.environ['PATH'] =  newpath +';' + os.environ['PATH']

import ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ogre.io.OIS as ois
import ogre.renderer.ogrepcz as pcz

# import RoomObject

Global_buildingNode = None #pcz.PCZSceneNode()
Global_buildingTranslate = None # ogre.Vector3()
Global_raySceneQuery = None # ogre.RaySceneQuery()
Global_targetMO = None #ogre.MovableObject()


# # class PCZTestFrameListener ( sf.ExampleFrameListener)
# # 
# # public:
# #     PCZTestFrameListener(RenderWindow* win, Camera* cam) : ExampleFrameListener( win, cam )
# #     
# #         self.MoveSpeed = 15.0
# #         self.updateRay=ogre.Ray()
# #     
# # 
# #     void moveCamera()
# #     
# #         # Make all the spatial changes to the camera's scene node
# #         # Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW
# #         #(e.g. airplane)
# #         self.camera.getParentSceneNode().translate(self.translateVector, Node.TS_LOCAL)
# #         self.camera.getParentSceneNode().pitch(mRotY)
# #         self.camera.getParentSceneNode().yaw(mRotX, Node.TS_WORLD)
# #         Global_buildingNode.translate(Global_buildingTranslate, Node.TS_LOCAL)
# #     
# # 
# #     bool frameRenderingQueued( const FrameEvent& evt )
# #     
# #         if( ExampleFrameListener.frameRenderingQueued( evt ) == False )
# #         return False
# # 
# #         Global_buildingTranslate = ogre.Vector3(0,0,0)
# #         if( self.keyboard.isKeyDown( ois.KC_LBRACKET ) )
# #         
# #             Global_buildingTranslate = ogre.Vector3(0,-10,0)
# #         
# #         if( self.keyboard.isKeyDown( ois.KC_RBRACKET ) )
# #         
# #             Global_buildingTranslate = ogre.Vector3(0,10,0)
# #         
# # 
# #         if( self.keyboard.isKeyDown( ois.KC_LSHIFT ) ||
# #             self.keyboard.isKeyDown( ois.KC_RSHIFT ))
# #         
# #             self.MoveSpeed = 150
# #         
# #         else
# #         
# #             self.MoveSpeed = 15
# #         
# # 
# #         # test the ray scene query by showing bounding box of whatever the camera is pointing directly at 
# #         # (takes furthest hit)
# #         self.updateRay.setOrigin(self.camera.getParentSceneNode().getPosition())
# #         self.updateRay.setDirection(self.camera.getParentSceneNode().getOrientation()*ogre.Vector3.NEGATIVE_UNIT_Z)
# #         Global_raySceneQuery.setRay(updateRay)
# #         zone = ((PCZSceneNode*)(self.camera.getParentSceneNode())).getHomeZone()
# #         ((PCZRaySceneQuery*)Global_raySceneQuery).setStartZone(zone)
# #         ((PCZRaySceneQuery*)Global_raySceneQuery).setExcludeNode(self.camera.getParentSceneNode())
# #         RaySceneQueryResult& qryResult = Global_raySceneQuery.execute()
# #         RaySceneQueryResult.iterator i = qryResult.begin()
# #         if (i != qryResult.end())
# #         
# #             RaySceneQueryResult.reverse_iterator ri = qryResult.rbegin()
# #             MovableObject * mo = ri.movable
# #             if (Global_targetMO != mo)
# #             
# #                 if (Global_targetMO != 0)
# #                 
# #                     Global_targetMO.getParentSceneNode().showBoundingBox(False)
# #                 
# #                 Global_targetMO = mo
# #                 Global_targetMO.getParentSceneNode().showBoundingBox(True)
# #             
# #         
# # 
# #         return True
    


class PCZTestApplication (sf.Application):
    def __del__(self):    
        del Global_raySceneQuery
    

    def _chooseSceneManager ( self ):
    
        # Create the SceneManager, in this case a generic one
        self.sceneManager = self.root.createSceneManager("PCZSceneManager", "PCZSceneManager")
        # initialize the scene manager using terrain as default zone
        zoneTypeName = "ZoneType_Default"
        self.pczSM = pcz.castAsPCZSceneManager( self.sceneManager ) # store this for future reference
        self.pczSM.init(zoneTypeName)
        #self.sceneManager.showBoundingBoxes(True)
    
#     def _createFrameListener( self ):
#         self.frameListener= PCZTestFrameListener(self.renderWindow, self.camera)
#         self.root.addFrameListener(self.frameListener)
    
    def _createCamera(self):
        # Create the camera
        self.camera = self.sceneManager.createCamera("PlayerCam")

        # NEW: create a node for the camera and control that instead of camera directly.
        # We do this because PCZSceneManager requires camera to have a node 
        self.cameraNode = self.sceneManager.getRootSceneNode().createChildSceneNode("PlayerCamNode")
        # attach the camera to the node
        self.cameraNode.attachObject(self.camera)
        # fix the yaw axis of the camera
        self.cameraNode.setFixedYawAxis(True)

        self.camera.setNearClipDistance(2)
        self.camera.setFarClipDistance( 1000 )
        # set camera zone
#       ((PCZSceneNode*)(self.cameraNode)).setHomeZone(((PCZSceneManager*)(self.sceneManager)).getDefaultZone())

    
    # utility function to create terrain zones easily
    def createTerrainZone(self, zoneName, terrain_cfg):
        # load terrain into the terrain zone
        terrainZone = self.pczSM.createZone("ZoneType_Terrain", zoneName)
        terrainZone.notifyCameraCreated(self.camera)
        self.pczSM.setZoneGeometry( zoneName, pcz.castAsPCZSceneNode(self.sceneManager.getRootSceneNode()), terrain_cfg )

        # create aab portal(s) around the terrain
        corners =[None]*2
        aabb = ogre.AxisAlignedBox()

        # make portal from terrain to default
        terrainZone.getAABB(aabb)
        portalName = "PortalFrom"+zoneName+"ToDefault_Zone"
        p = self.pczSM.createPortal(portalName, pcz.Portal.PORTAL_TYPE_AABB)
        corners[0] = aabb.getMinimum()
        corners[1] = aabb.getMaximum()
        p.setCorner(0, corners[0])
        p.setCorner(1, corners[1])
        p.setDirection(ogre.Vector3.NEGATIVE_UNIT_Z) # this indicates an "inward" pointing normal
        # associate the portal with the terrain's main node
        p.setNode(terrainZone.getEnclosureNode())
        # IMPORTANT: Update the derived values of the portal
        p.updateDerivedValues()
        # add the portal to the zone
        terrainZone._addPortal(p)
    
        # make portal from default to terrain
        portalName = "PortalFromDefault_ZoneTo"+zoneName
        p2 = self.pczSM.createPortal(portalName, pcz.Portal.PORTAL_TYPE_AABB)
        corners[0] = aabb.getMinimum()
        corners[1] = aabb.getMaximum()
        p2.setCorner(0, corners[0])
        p2.setCorner(1, corners[1])
        p2.setDirection(ogre.Vector3.UNIT_Z) # this indicates an "outward" pointing normal
        # associate the portal with the terrain's main node
        p2.setNode(terrainZone.getEnclosureNode())
        # IMPORTANT: Update the derived values of the portal
        p2.updateDerivedValues()
        # add the portal to the zone
        self.pczSM.getDefaultZone()._addPortal(p2)

        # connect the portals manually
        p.setTargetPortal(p2)
        p2.setTargetPortal(p)
        p.setTargetZone(self.pczSM.getDefaultZone())
        p2.setTargetZone(terrainZone)

        return terrainZone
    

    # Just override the mandatory create scene method
    def _createScene( self ):
    
        # Set ambient light
        self.sceneManager.setAmbientLight(ogre.ColourValue(0.25, 0.25, 0.25))

        # Create a skybox
        self.sceneManager.setSkyBox(True, "Examples/CloudyNoonSkyBox", 500 )
        # put the skybox node in the default zone
#         self.pczSM.setSkyZone(None)

        # Create a light
        l = self.sceneManager.createLight("MainLight")
        l.setPosition(0,0,0) 
        l.setAttenuation(500, 0.5, 1.0, 0.0)
        # Accept default settings: point light, white diffuse, just set position
        # attach light to a scene node so the PCZSM can handle it properly (zone-wise)
        # IMPORTANT: Lights (just like cameras) MUST be connected to a scene node!
        lightNode = self.cameraNode.createChildSceneNode("light_Node")
        lightNode.attachObject(l)      

        # Fog
        # NB it's VERY important to set this before calling setWorldGeometry 
        # because the vertex program picked will be different
        fadeColour= ogre.ColourValue(0.101, 0.125, 0.1836)
        self.sceneManager.setFog( ogre.FOG_LINEAR, fadeColour, .001, 500, 1000)
        self.renderWindow.getViewport(0).setBackgroundColour(fadeColour)

        # create a terrain zone
        terrain_cfg = "terrain.cfg"
        zoneName ="Terrain1_Zone"
        terrainZone = self.createTerrainZone(zoneName, terrain_cfg)

# # /*      # Create another terrain zone
# #         terrain_cfg = "terrain.cfg"
# #         zoneName = "Terrain2_Zone"
# #         terrainZone = createTerrainZone(zoneName, terrain_cfg)
# #         # move second terrain next to first terrain
# #         terrainZone.getEnclosureNode().setPosition(1500, 0, 0)
# # 
# #         # Create another terrain zone
# #         terrain_cfg = "terrain.cfg"
# #         zoneName = "Terrain3_Zone"
# #         terrainZone = createTerrainZone(zoneName, terrain_cfg)
# #         # move terrain next to first terrain
# #         terrainZone.getEnclosureNode().setPosition(0, 0, 1500)
# # 
# #         # Create another terrain zone
# #         terrain_cfg = "terrain.cfg"
# #         zoneName = "Terrain4_Zone"
# #         terrainZone = createTerrainZone(zoneName, terrain_cfg)
# #         # move terrain next to first terrain
# #         terrainZone.getEnclosureNode().setPosition(-1500, 0, 0)
# # 
# #         # Create another terrain zone
# #         terrain_cfg = "terrain.cfg"
# #         zoneName = "Terrain5_Zone"
# #         terrainZone = createTerrainZone(zoneName, terrain_cfg)
# #         # move terrain next to first terrain
# #         terrainZone.getEnclosureNode().setPosition(0, 0, -1500)
# # 
# #         # Create another terrain zone
# #         terrain_cfg = "terrain.cfg"
# #         zoneName = "Terrain6_Zone"
# #         terrainZone = createTerrainZone(zoneName, terrain_cfg)
# #         # move terrain next to first terrain
# #         terrainZone.getEnclosureNode().setPosition(1500, 0, 1500)
# # 
# #         # Create another terrain zone
# #         terrain_cfg = "terrain.cfg"
# #         zoneName = "Terrain7_Zone"
# #         terrainZone = createTerrainZone(zoneName, terrain_cfg)
# #         # move terrain next to first terrain
# #         terrainZone.getEnclosureNode().setPosition(-1500, 0, -1500)
# # 
# #         # Create another terrain zone
# #         terrain_cfg = "terrain.cfg"
# #         zoneName = "Terrain8_Zone"
# #         terrainZone = createTerrainZone(zoneName, terrain_cfg)
# #         # move terrain next to first terrain
# #         terrainZone.getEnclosureNode().setPosition(-1500, 0, 1500)
# # 
# #         # Create another terrain zone
# #         terrain_cfg = "terrain.cfg"
# #         zoneName = "Terrain9_Zone"
# #         terrainZone = createTerrainZone(zoneName, terrain_cfg)
# #         # move terrain next to first terrain
# #         terrainZone.getEnclosureNode().setPosition(1500, 0, -1500)
# # */
        # set far clip plane to one terrain zone width (we have a LOT of terrain here, so we need to do far clipping!)
        self.camera.setFarClipDistance(1500)

#         # create test buildinig
#         RoomObject roomObj
#         Global_buildingNode = roomObj.createTestBuilding(self.sceneManager, String("1"))
#         Global_buildingNode.setPosition(500, 165, 570)
#         #ogre.Radian r = Radian(3.1416/7.0)
#         #Global_buildingNode.rotate(ogre.Vector3.UNIT_Y, r)
# 
#         # create another test buildinig
#         RoomObject roomObj2
#         Global_buildingNode = roomObj2.createTestBuilding(self.sceneManager, String("2"))
#         Global_buildingNode.setPosition(400, 165, 570)
#         #ogre.Radian r = Radian(3.1416/7.0)
#         #Global_buildingNode.rotate(ogre.Vector3.UNIT_Y, r)
# 
#         # Position camera in the center of the building
#         self.cameraNode.setPosition(Global_buildingNode.getPosition())
#         # Look back along -Z
#         self.camera.lookAt(self.cameraNode._getDerivedPosition() + ogre.Vector3(0,0,-300))
#         # Update bounds for camera
#         self.cameraNode._updateBounds()
# 
#         # create the ray scene query
#         Global_raySceneQuery = self.sceneManager.createRayQuery(
#             Ray(self.camera.getParentNode().getPosition(), ogre.Vector3.NEGATIVE_UNIT_Z))
#         Global_raySceneQuery.setSortByDistance(True, 5)

    

if __name__ == '__main__':
    import exceptions,sys
    try:
        application = PCZTestApplication()
        application.go()
    except ogre.OgreException, e:
        print e

