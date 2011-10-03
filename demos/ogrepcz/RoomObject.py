##
##  Filename : RoomObject.py
##  based on C++ demo in Ogre
##

import sys
import os
sys.path.insert(0,'..')
import PythonOgreConfig

## this here becase the Plug_PCZSceneManager.dll needs to be accessed by both the python module and the ogre plugins
if sys.platform == 'win32': 
    newpath = os.path.join ( os.path.abspath(os.path.dirname(__file__)), '../../plugins')
    os.environ['PATH'] =  newpath +';' + os.environ['PATH']

import ogre.renderer.OGRE as ogre
import ogre.renderer.ogrepcz as pcz

DOOR_NONE   = 0x00
DOOR_TOP    = 0x01
DOOR_BOT    = 0x02
DOOR_FRONT  = 0x04
DOOR_BACK   = 0x08
DOOR_LEFT   = 0x10
DOOR_RIGHT  = 0x20
DOOR_ALL    = 0xFF

class RoomObject:
    def __init__ ( self ):
        self.initMaterial=False 
        self.count = 0
        self.points =[ogre.Vector3]*32

    def createTestBuilding(self, scene,name):
        self.count+=1 
        ## set points to building exterior size
        self.createPoints(ogre.Vector3(60.0, 40.0, 60.0), ogre.Vector3(4.0, 10.0, 4.0)) 
    
        ## create the building exterior
        exterior = scene.createEntity( name + "_building_exterior", "building_exterior.mesh" ) 
    
        ## make the enclosure a child node of the root scene node
        exteriorNode = scene.getRootSceneNode().createChildSceneNode(name +"_building_exterior_node",  ogre.Vector3( 0, 0, 0 ) )
        exteriorNode.attachObject(exterior) 
        scene.addPCZSceneNode(exteriorNode, scene.getDefaultZone()) 
    
        ## create portals for the building exterior
        self.createPortals(scene, 
                      exterior, 
                      exteriorNode, 
                      scene.getDefaultZone(),
                      DOOR_FRONT|DOOR_BACK|DOOR_LEFT|DOOR_RIGHT, 
                      True) 
    
        ## reset points to room size
        self.createPoints(ogre.Vector3(20.0, 10.0, 20.0), ogre.Vector3(4.0, 10.0, 4.0)) 
    
        ## create an interior room
        room = scene.createEntity( name +"_room1", "room_nzpz.mesh" ) 
    
        ## add the room as a child node to the enclosure node
        roomNode = exteriorNode.createChildSceneNode( name +"_room1_node", ogre.Vector3( 0.0, 0.0, 20.0 ) )
        roomNode.attachObject(room) 
    
        ## room needs it's own zone
        zoneType = "ZoneType_Default" 
        zoneName = name +"_room1_zone" 
        newZone = scene.createZone(zoneType, zoneName) 
        newZone.setEnclosureNode(roomNode) 
        scene.addPCZSceneNode(roomNode, newZone) 
    
        ## create portals for the room
        self.createPortals(scene, 
                      room, 
                      roomNode, 
                      newZone,
                      DOOR_FRONT|DOOR_BACK, 
                      False) 
    
        ## create another interior room
        room = scene.createEntity( name +"_room2", "room_nxpxnypynzpz.mesh" ) 
    
        ## add the room as a child node to the enclosure node
        roomNode = exteriorNode.createChildSceneNode( name +"_room2_node", ogre.Vector3( 0.0, 0.0, 0.0 ) )
        roomNode.attachObject(room) 
    
        ## room needs it's own zone
        zoneName = name +"_room2_zone" 
        newZone = scene.createZone(zoneType, zoneName) 
        newZone.setEnclosureNode(roomNode) 
        scene.addPCZSceneNode(roomNode, newZone) 
    
        ## create portals for the room
        self.createPortals(scene, 
                      room, 
                      roomNode, 
                      newZone,
                      DOOR_FRONT|DOOR_BACK|DOOR_LEFT|DOOR_RIGHT|DOOR_TOP|DOOR_BOT, 
                      False) 
    
        ## create another interior room
        room = scene.createEntity( name +"_room3", "room_nzpz.mesh" ) 
    
        ## add the room as a child node to the enclosure node
        roomNode = exteriorNode.createChildSceneNode( name +"_room3_node", ogre.Vector3( 0.0, 0.0, -20.0 ) )
        roomNode.attachObject(room) 
    
        ## room needs it's own zone
        zoneName = name +"_room3_zone" 
        newZone = scene.createZone(zoneType, zoneName) 
        newZone.setEnclosureNode(roomNode) 
        scene.addPCZSceneNode(roomNode, newZone) 
    
        ## create portals for the room
        self.createPortals(scene, 
                      room, 
                      roomNode, 
                      newZone,
                      DOOR_FRONT|DOOR_BACK, 
                      False) 
    
        ## create another interior room
        room = scene.createEntity( name +"_room4", "room_nxpx.mesh" ) 
    
        ## add the room as a child node to the enclosure node
        roomNode = exteriorNode.createChildSceneNode( name +"_room4_node", ogre.Vector3( -20.0, 0.0, 0.0 ) )
        roomNode.attachObject(room) 
    
        ## room needs it's own zone
        zoneName = name +"_room4_zone" 
        newZone = scene.createZone(zoneType, zoneName) 
        newZone.setEnclosureNode(roomNode) 
        scene.addPCZSceneNode(roomNode, newZone) 
    
        ## create portals for the room
        self.createPortals(scene, 
                      room, 
                      roomNode, 
                      newZone,
                      DOOR_LEFT|DOOR_RIGHT, 
                      False) 
    
        ## create another interior room
        room = scene.createEntity( name +"_room5", "room_nxpx.mesh" ) 
    
        ## add the room as a child node to the enclosure node
        roomNode = exteriorNode.createChildSceneNode( name +"_room5_node", ogre.Vector3( 20.0, 0.0, 0.0 ) )
        roomNode.attachObject(room) 
    
        ## room needs it's own zone
        zoneName = name +"_room5_zone" 
        newZone = scene.createZone(zoneType, zoneName) 
        newZone.setEnclosureNode(roomNode) 
        scene.addPCZSceneNode(roomNode, newZone) 
    
        ## create portals for the room
        self.createPortals(scene, 
                      room, 
                      roomNode, 
                      newZone,
                      DOOR_LEFT|DOOR_RIGHT, 
                      False) 
    
        ## create another interior room
        room = scene.createEntity( name +"_room6", "room_ny.mesh" ) 
    
        ## add the room as a child node to the enclosure node
        roomNode = exteriorNode.createChildSceneNode( name +"_room6_node", ogre.Vector3( 0.0, 10.0, 0.0 ) )
        roomNode.attachObject(room) 
    
        ## room needs it's own zone
        zoneName = name +"_room6_zone" 
        newZone = scene.createZone(zoneType, zoneName) 
        newZone.setEnclosureNode(roomNode) 
        scene.addPCZSceneNode(roomNode, newZone) 
    
        ## create portals for the room
        self.createPortals(scene, 
                      room, 
                      roomNode, 
                      newZone,
                      DOOR_BOT, 
                      False) 
    
        ## create another interior room
        room = scene.createEntity( name +"_room7", "room_py.mesh" ) 
    
        ## add the room as a child node to the enclosure node
        roomNode = exteriorNode.createChildSceneNode( name +"_room7_node", ogre.Vector3( 0.0, -50.0, 0.0 ) )
        roomNode.attachObject(room) 
    
        ## room needs it's own zone
        zoneName = name +"_room7_zone" 
        newZone = scene.createZone(zoneType, zoneName) 
        newZone.setEnclosureNode(roomNode) 
        scene.addPCZSceneNode(roomNode, newZone) 
    
        ## create portals for the room
        self.createPortals(scene, 
                      room, 
                      roomNode, 
                      newZone,
                      DOOR_TOP, 
                      False) 
    
        ## reset points to tall room size
        self.createPoints(ogre.Vector3(20.0, 40.0, 20.0), ogre.Vector3(4.0, 10.0, 4.0)) 
    
        ## create another interior room
        room = scene.createEntity( name +"_room8", "room_nypy_4y.mesh" ) 
    
        ## add the room as a child node to the enclosure node
        roomNode = exteriorNode.createChildSceneNode( name +"_room8_node", ogre.Vector3( 0.0, -25.0, 0.0 ) )
        roomNode.attachObject(room) 
    
        ## room needs it's own zone
        zoneName = name +"_room8_zone" 
        newZone = scene.createZone(zoneType, zoneName) 
        newZone.setEnclosureNode(roomNode) 
        scene.addPCZSceneNode(roomNode, newZone) 
    
        ## create portals for the room
        self.createPortals(scene, 
                      room, 
                      roomNode, 
                      newZone,
                      DOOR_BOT|DOOR_TOP, 
                      False) 
    
    
        ## resolve portal zone pointers
        scene.connectPortalsToTargetZonesByLocation() 
    
        return exteriorNode 



    def createRoom(self, scene, name, doorFlags, isEnclosure, dimensions, doorDimensions):
        self.addMaterial(name, ogre.ColourValue(1,1,1,.75), Ogre.SBT_TRANSPARENT_ALPHA) 
        room = scene.createManualObject(name)  
        room.begin(name, ogre.RenderOperation.OT_TRIANGLE_LIST)  
    
        ## create points
        self.createPoints(dimensions, doorDimensions) 
    
        fade=0.5 
        solid=0.8 
        color = ogre.ColourValue(0, 0, solid, solid) 
    
        ## copy to room
        for i in range(32):
            room.position(self.self.points[i]) 
            room.colour(color) 
    
        self.createWalls(room, doorFlags, isEnclosure) 
        room.end()  
        return room 


    def addMaterial( self, mat, clr, sbt):
        if(self.initMaterial):
            return 
        else:
            self.initMaterial=True 
        matptr = ogre.MaterialManager.getSingleton().create(mat, "General")  
        matptr.setReceiveShadows(False)  
        matptr.getTechnique(0).setLightingEnabled(True) 
        matptr.getTechnique(0).getPass(0).setDiffuse(clr)  
        matptr.getTechnique(0).getPass(0).setAmbient(clr)  
        matptr.getTechnique(0).getPass(0).setSelfIllumination(clr)  
        matptr.getTechnique(0).getPass(0).setSceneBlending(sbt) 
        matptr.getTechnique(0).getPass(0).setLightingEnabled(False) 
        matptr.getTechnique(0).getPass(0).setVertexColourTracking(Ogre.TVC_DIFFUSE) 


    def createPoints(self, dimensions, doorDimensions):

        l = dimensions.x/2 
        h = dimensions.y/2 
        w = dimensions.z/2 
        
    ##           4       7
    ##            *-------*
    ##           /|      /|
    ##          / |     / |         y
    ##         / 5|   3/ 6|         |
    ##       0*---*---*---*         *-- x 
    ##        |  /    |  /         /
    ##        | /     | /         z 
    ##        |/      |/
    ##       1*-------*2
    
        self.points[0] = ogre.Vector3(-l, h, w) ##0
        self.points[1] = ogre.Vector3(-l, -h, w) ##1
        self.points[2] = ogre.Vector3(l, -h, w) ##2
        self.points[3] = ogre.Vector3(l, h, w) ##3
    
        self.points[4] = ogre.Vector3(-l, h, -w) ##4
        self.points[5] = ogre.Vector3(-l, -h, -w) ##5
        self.points[6] = ogre.Vector3(l, -h, -w) ##6
        self.points[7] = ogre.Vector3(l, h, -w) ##7
    
        ## doors
        l2 = doorDimensions.x/2 
        h2 = doorDimensions.y/2 
        w2 = doorDimensions.z/2 
    
        ## front door
        self.points[8] = ogre.Vector3(-l2, h2, w) ##8
        self.points[9] = ogre.Vector3(-l2, -h2, w) ##9
        self.points[10] = ogre.Vector3(l2, -h2, w) ##10
        self.points[11] = ogre.Vector3(l2, h2, w) ##11
    
        ## back door
        self.points[12] = ogre.Vector3(-l2, h2, -w) ##12
        self.points[13] = ogre.Vector3(-l2, -h2, -w) ##13
        self.points[14] = ogre.Vector3(l2, -h2, -w) ##14
        self.points[15] = ogre.Vector3(l2, h2, -w) ##15
    
        ## top door
        self.points[16] = ogre.Vector3(-l2, h, -w2) ##16
        self.points[17] = ogre.Vector3(-l2, h, w2) ##17
        self.points[18] = ogre.Vector3(l2, h, w2) ##18
        self.points[19] = ogre.Vector3(l2, h, -w2) ##19
    
        ## bottom door
        self.points[20] = ogre.Vector3(-l2, -h, -w2) ##20
        self.points[21] = ogre.Vector3(-l2, -h, w2) ##21
        self.points[22] = ogre.Vector3(l2, -h, w2) ##22
        self.points[23] = ogre.Vector3(l2, -h, -w2) ##23
    
        ## left door
        self.points[24] = ogre.Vector3(-l, h2, w2) ##24
        self.points[25] = ogre.Vector3(-l, -h2, w2) ##25
        self.points[26] = ogre.Vector3(-l, -h2, -w2) ##26
        self.points[27] = ogre.Vector3(-l, h2, -w2) ##27
    
        ## right door
        self.points[28] = ogre.Vector3(l, h2, w2) ##28
        self.points[29] = ogre.Vector3(l, -h2, w2) ##29
        self.points[30] = ogre.Vector3(l, -h2, -w2) ##30
        self.points[31] = ogre.Vector3(l, h2, -w2) ##31


    def createWalls(self, room, doorFlags, isEnclosure):
        if (isEnclosure):
            if(doorFlags & DOOR_FRONT):
                ## make front wall outward facing with door
                room.quad(0, 8, 11, 3) 
                room.quad(1, 9, 8, 0) 
                room.quad(2, 10, 9, 1) 
                room.quad(3, 11, 10, 2) 
            else:
                ## make front wall outward facing without door
                room.quad(0, 1, 2, 3)
                 
            if(doorFlags & DOOR_BACK):
                ## make back wall outward facing with door
                room.quad(7, 15, 12, 4) 
                room.quad(6, 14, 15, 7) 
                room.quad(5, 13, 14, 6) 
                room.quad(4, 12, 13, 5) 
            else:
                ## make back wall outward facing without door
                room.quad(7, 6, 5, 4) 
    
            if(doorFlags & DOOR_TOP):
                ## make top wall outward facing with door
                room.quad(0, 17, 16, 4) 
                room.quad(4, 16, 19, 7) 
                room.quad(7, 19, 18, 3) 
                room.quad(3, 18, 17, 0) 
            else:
                ## make top wall outward facing without door
                room.quad(0, 3, 7, 4) 
    
            if(doorFlags & DOOR_BOT):
                ## make bottom wall outward facing with door
                room.quad(5, 20, 21, 1) 
                room.quad(6, 23, 20, 5) 
                room.quad(2, 22, 23, 6) 
                room.quad(1, 21, 22, 2) 
            else:
                ## make bottom wall outward facing without door
                room.quad(2, 1, 5, 6) 
    
            if(doorFlags & DOOR_LEFT):
                ## make left wall outward facing with door
                room.quad(0, 24, 25, 1) 
                room.quad(4, 27, 24, 0) 
                room.quad(5, 26, 27, 4) 
                room.quad(1, 25, 26, 5) 
            else:
                ## make left side wall outward facing without door
                room.quad(1, 0, 4, 5) 
    
            if(doorFlags & DOOR_RIGHT):
                ## make right wall outward facing with door
                room.quad(2, 29, 28, 3) 
                room.quad(6, 30, 29, 2) 
                room.quad(7, 31, 30, 6) 
                room.quad(3, 28, 31, 7) 
            else:
                ## make right side wall outward facing without door
                room.quad(3, 2, 6, 7) 
    
        else:
    
            ## front back
            if(doorFlags & DOOR_FRONT):
                ## make front wall inward facing with door
                room.quad(3, 11, 8, 0) 
                room.quad(0, 8, 9, 1) 
                room.quad(1, 9, 10, 2) 
                room.quad(2, 10, 11, 3) 
            else:
                ## make front wall inward facing without door
                room.quad(3, 2, 1, 0) 
    
            if(doorFlags & DOOR_BACK):
                ## make back wall inward facing with door
                room.quad(4, 12, 15, 7) 
                room.quad(7, 15, 14, 6) 
                room.quad(6, 14, 13, 5) 
                room.quad(5, 13, 12, 4) 
            else:
                ## make back wall inward facing without door
                room.quad(4, 5, 6, 7) 
    
            ## top bottom
            if(doorFlags & DOOR_TOP):
                ## make top wall inward facing with door
                room.quad(4, 16, 17, 0) 
                room.quad(7, 19, 16, 4) 
                room.quad(3, 18, 19, 7) 
                room.quad(0, 17, 18, 3) 
            else:
                ## make top wall inward facing without door
                room.quad(4, 7, 3, 0) 
    
            if(doorFlags & DOOR_BOT):
                ## make bottom wall inward facing with door
                room.quad(1, 21, 20, 5) 
                room.quad(5, 20, 23, 6) 
                room.quad(6, 23, 22, 2) 
                room.quad(2, 22, 21, 1) 
            else:
                ## make bottom wall inward facing without door
                room.quad(6, 5, 1, 2) 
    
            ## end caps
            if(doorFlags & DOOR_LEFT):
                ## make left wall inward facing with door
                room.quad(1, 25, 24, 0) 
                room.quad(0, 24, 27, 4) 
                room.quad(4, 27, 26, 5) 
                room.quad(5, 26, 25, 1) 
            else:
                ## make left side wall inward facing without door
                room.quad(5, 4, 0, 1) 
    
            if(doorFlags & DOOR_RIGHT):
                ## make right wall inward facing with door
                room.quad(3, 28, 29, 2) 
                room.quad(2, 29, 30, 6) 
                room.quad(6, 30, 31, 7) 
                room.quad(7, 31, 28, 3) 
            else:
                ## make right side wall inward facing without door
                room.quad(7, 6, 2, 3) 

## Create portals for every door
    def createPortals(self, scene, room, roomNode, zone, doorFlags, isEnclosure):
        corners=[ogre.Vector3] * 4
        if (isEnclosure):
            if(doorFlags & DOOR_FRONT):
                ## set the corners to the front door corners
                corners[0] = self.points[8] 
                corners[1] = self.points[9] 
                corners[2] = self.points[10] 
                corners[3] = self.points[11] 
                ## create the portal
                portalName = room.getName() + "_FrontDoorPortal"
                p = scene.createPortal(portalName) 
                p.setCorners(corners) 
                ## associate the portal with the roomnode
                p.setNode(roomNode) 
                ## add the portal to the zone
                zone._addPortal(p) 
                ## update derived values for the portal
                p.updateDerivedValues() 
    
            if(doorFlags & DOOR_BACK):
                ## set the corners to the front door corners
                corners[0] = self.points[15] 
                corners[1] = self.points[14] 
                corners[2] = self.points[13] 
                corners[3] = self.points[12] 
                ## create the portal
                portalName = room.getName() + "_BackDoorPortal" 
                p = scene.createPortal(portalName) 
                p.setCorners(corners) 
                ## associate the portal with the roomnode
                p.setNode(roomNode) 
                ## add the portal to the zone
                zone._addPortal(p) 
                ## update derived values for the portal
                p.updateDerivedValues() 
    
            if(doorFlags & DOOR_TOP):
                ## set the corners to the front door corners
                corners[0] = self.points[16] 
                corners[1] = self.points[17] 
                corners[2] = self.points[18] 
                corners[3] = self.points[19] 
                ## create the portal
                portalName = room.getName() + "_TopDoorPortal" 
                p = scene.createPortal(portalName) 
                p.setCorners(corners) 
                ## associate the portal with the roomnode
                p.setNode(roomNode) 
                ## add the portal to the zone
                zone._addPortal(p) 
                ## update derived values for the portal
                p.updateDerivedValues() 
    
            if(doorFlags & DOOR_BOT):
                ## set the corners to the front door corners
                corners[0] = self.points[23] 
                corners[1] = self.points[22] 
                corners[2] = self.points[21] 
                corners[3] = self.points[20] 
                ## create the portal
                portalName = room.getName() + "_BottomDoorPortal" 
                p = scene.createPortal(portalName) 
                p.setCorners(corners) 
                ## associate the portal with the roomnode
                p.setNode(roomNode) 
                ## add the portal to the zone
                zone._addPortal(p) 
                ## update derived values for the portal
                p.updateDerivedValues() 
    
            if(doorFlags & DOOR_LEFT):
                ## set the corners to the front door corners
                corners[0] = self.points[27] 
                corners[1] = self.points[26] 
                corners[2] = self.points[25] 
                corners[3] = self.points[24] 
                ## create the portal
                portalName = room.getName() + "_LeftDoorPortal"
                p = scene.createPortal(portalName) 
                p.setCorners(corners) 
                ## associate the portal with the roomnode
                p.setNode(roomNode) 
                ## add the portal to the zone
                zone._addPortal(p) 
                ## update derived values for the portal
                p.updateDerivedValues() 
    
            if(doorFlags & DOOR_RIGHT):
                ## set the corners to the front door corners
                corners[0] = self.points[28] 
                corners[1] = self.points[29] 
                corners[2] = self.points[30] 
                corners[3] = self.points[31] 
                ## create the portal
                portalName = room.getName() + "_RightDoorPortal"
                p = scene.createPortal(portalName) 
                p.setCorners(corners) 
                ## associate the portal with the roomnode
                p.setNode(roomNode) 
                ## add the portal to the zone
                zone._addPortal(p) 
                ## update derived values for the portal
                p.updateDerivedValues() 
        else:
            if(doorFlags & DOOR_FRONT):
                ## set the corners to the front door corners
                corners[0] = self.points[11] 
                corners[1] = self.points[10] 
                corners[2] = self.points[9] 
                corners[3] = self.points[8] 
                ## create the portal
                portalName = room.getName() + "_FrontDoorPortal" 
                p = scene.createPortal(portalName) 
                p.setCorners(corners) 
                ## associate the portal with the roomnode
                p.setNode(roomNode) 
                ## add the portal to the zone
                zone._addPortal(p) 
                ## update derived values for the portal
                p.updateDerivedValues() 
    
            if(doorFlags & DOOR_BACK):
                ## set the corners to the front door corners
                corners[0] = self.points[12] 
                corners[1] = self.points[13] 
                corners[2] = self.points[14] 
                corners[3] = self.points[15] 
                ## create the portal
                portalName = room.getName() + "_BackDoorPortal" 
                p = scene.createPortal(portalName) 
                p.setCorners(corners) 
                ## associate the portal with the roomnode
                p.setNode(roomNode) 
                ## add the portal to the zone
                zone._addPortal(p) 
                ## update derived values for the portal
                p.updateDerivedValues() 
    
            if(doorFlags & DOOR_TOP):
                ## set the corners to the front door corners
                corners[0] = self.points[19] 
                corners[1] = self.points[18] 
                corners[2] = self.points[17] 
                corners[3] = self.points[16] 
                ## create the portal
                portalName = room.getName() + "_TopDoorPortal" 
                p = scene.createPortal(portalName) 
                p.setCorners(corners) 
                ## associate the portal with the roomnode
                p.setNode(roomNode) 
                ## add the portal to the zone
                zone._addPortal(p) 
                ## update derived values for the portal
                p.updateDerivedValues() 
    
            if(doorFlags & DOOR_BOT):
                ## set the corners to the front door corners
                corners[0] = self.points[20] 
                corners[1] = self.points[21] 
                corners[2] = self.points[22] 
                corners[3] = self.points[23] 
                ## create the portal
                portalName = room.getName() + "_BottomDoorPortal"
                p = scene.createPortal(portalName) 
                p.setCorners(corners) 
                ## associate the portal with the roomnode
                p.setNode(roomNode) 
                ## add the portal to the zone
                zone._addPortal(p)
                ## update derived values for the portal
                p.updateDerivedValues() 
    
            if(doorFlags & DOOR_LEFT):
                ## set the corners to the front door corners
                corners[0] = self.points[24] 
                corners[1] = self.points[25] 
                corners[2] = self.points[26] 
                corners[3] = self.points[27] 
                ## create the portal
                portalName = room.getName() + "_LeftDoorPortal" 
                p = scene.createPortal(portalName) 
                p.setCorners(corners) 
                ## associate the portal with the roomnode
                p.setNode(roomNode) 
                ## add the portal to the zone
                zone._addPortal(p) 
                ## update derived values for the portal
                p.updateDerivedValues() 
    
            if(doorFlags & DOOR_RIGHT):
                ## set the corners to the front door corners
                corners[0] = self.points[31] 
                corners[1] = self.points[30] 
                corners[2] = self.points[29] 
                corners[3] = self.points[28] 
                ## create the portal
                portalName = room.getName() + "_RightDoorPortal" 
                p = scene.createPortal(portalName) 
                p.setCorners(corners) 
                ## associate the portal with the roomnode
                p.setNode(roomNode) 
                ## add the portal to the zone
                zone._addPortal(p) 
                ## update derived values for the portal
                p.updateDerivedValues() 
    