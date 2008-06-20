# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http://python-ogre.org/
#
# It is likely based on original code from OGRE and/or PyOgre
# For the latest info, see http://www.ogre3d.org/
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------

# /**
#     \file 
#         Grass.py
#     \brief
#         Specialisation of OGRE's framework application to show the
#         use of the StaticGeometry class to create 'baked' instances of
#       many meshes, to create effects like grass efficiently.
# **/
import sys
sys.path.insert(0,'..')
import PythonOgreConfig


GRASS_HEIGHT = 300
GRASS_WIDTH = 250
GRASS_MESH_NAME = "grassblades"
GRASS_MATERIAL = "Examples/GrassBlades"
OFFSET_PARAM = 999
import ctypes, math
import ogre.renderer.OGRE as ogre
import ogre.io.OIS as OIS
import SampleFramework as sf
import random
VERTEXDATA = None

# self.mLight 
# self.mLightNode = 0 
mAnimState = None ## it's the ligth animation track - needs to be created in createscene and access by framelistener 
mMinLightColour = ogre.ColourValue (0.5, 0.1, 0.0) 
mMaxLightColour = ogre.ColourValue (1.0, 0.6, 0.0) 
mMinFlareSize = 40 
mMaxFlareSize = 80 
mStaticGeom = None # ogre.StaticGeometry()
vertexdata=None


#  This class 'wibbles' the light and billboard 
class LightWibbler ( ogre.ControllerValueFloat ):
    def __init__ ( self, light, billboard, minColour, maxColour, minSize, maxSize):
        ogre.ControllerValueFloat.__init__(self)    ## don't forget this !!!
        self.mLight = light 
        self.mBillboard = billboard 
        self.mColourRange = ogre.ColourValue()
        self.mColourRange.r = (maxColour.r - minColour.r) * 0.5 
        self.mColourRange.g = (maxColour.g - minColour.g) * 0.5 
        self.mColourRange.b = (maxColour.b - minColour.b) * 0.5 
        self.mHalfColour = minColour + self.mColourRange 
        self.mMinSize = minSize 
        self.mSizeRange = maxSize - minSize 
        self.intensity = 0

    def getValue (self) :
        return self.intensity 

    def setValue (self, value):
        self.intensity = value 
        newColour = ogre.ColourValue()
        ## Attenuate the brightness of the light
        newColour.r = self.mHalfColour.r + (self.mColourRange.r * self.intensity) 
        newColour.g = self.mHalfColour.g + (self.mColourRange.g * self.intensity) 
        newColour.b = self.mHalfColour.b + (self.mColourRange.b * self.intensity) 
        self.mLight.setDiffuseColour(newColour) 
        self.mBillboard.setColour(newColour) 
        ## set billboard size
        newSize = self.mMinSize + (self.intensity * self.mSizeRange) 
        self.mBillboard.setDimensions(newSize, newSize) 

        
class GrassListener ( sf.FrameListener ):
    def __init__ (self, win, cam,  sceneManager):
        sf.FrameListener.__init__(self, win, cam), 
        self.sceneManager = sceneManager
        self.mShowBBs = False
        self.xinc = math.pi * 0.4 
        self.zinc = math.pi * 0.55 
        self.timeDelay = 0
        self.xpos = (random.random () * (math.pi*2)) - math.pi 
        self.zpos = (random.random () * (math.pi*2)) - math.pi 
        
    def waveGrass(self, timeElapsed):
        global mStaticGeom, OFFSET_PARAM
        
        self.xpos += self.xinc * timeElapsed 
        self.zpos += self.zinc * timeElapsed 

        ## Update vertex program parameters by binding a value to each renderable
        offset = ogre.Vector4(0,0,0,0) 
        for reg in mStaticGeom.getRegionIterator():
            ## a little randomness
            self.xpos += reg.getCentre().x * 0.001 
            self.zpos += reg.getCentre().z * 0.001 
            offset.x = math.sin(self.xpos) * 5 
            offset.z = math.sin(self.zpos) * 5 
            for lod in reg.getLODIterator(): 
                for mat in lod.getMaterialIterator():  
                    for geom in mat.getGeometryIterator():
                        geom.setCustomParameter(OFFSET_PARAM, offset) 

    def frameStarted( self, evt) :
        global mAnimState
        if sf.FrameListener.frameStarted(self, evt) == False:
            return False
        self.timeDelay -= evt.timeSinceLastFrame 

        if (mAnimState):
            mAnimState.addTime(evt.timeSinceLastFrame) 

        if self._isToggleKeyDown(OIS.KC_B, 0.1):
            self.mShowBBs = not self.mShowBBs 
            self.sceneManager.showBoundingBoxes(self.mShowBBs) 

        self.waveGrass(evt.timeSinceLastFrame) 
        return True 


        
class Grass_Application(sf.Application):
    def __del__( self ):
        ## Fix to stop crashes at exit -- C++ tries to delete vertexData which is ugly if it's a python object
        #self.sm.vertexData=None
        return
        
    def createGrassMesh(self):
        global GRASS_MESH_NAME
        self.SetToNone = []
        ## Each grass section is 3 planes at 60 degrees to each other
        ## Normals point straight up to simulate correct lighting
        msh = ogre.MeshManager.getSingleton().createManual(GRASS_MESH_NAME, 
            ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME) 
        self.msh = msh
        self.hw = ogre.HardwareBufferManager.getSingleton()
        sm = msh.createSubMesh() 
        
        sm.useSharedVertices = False 

           
        self.vertexdata = ogre.createVertexData() ## Create the VertexData Object
        
        ## NOTE the submesh will delete the vertexData when it is destroyed which is BAD -- ie we have C++ specifically
        ## deleting a Python object which makes everything confused..  The fix is to set the submesh vertexData to 
        ## 'None' before exiting -- see the __del__ function
        sm.vertexData = self.vertexdata     
        
        sm.vertexData.vertexStart = 0 
        sm.vertexData.vertexCount = 12 
        dcl = sm.vertexData.vertexDeclaration 
        offset = 0 
        dcl.addElement(0, offset, ogre.VertexElementType.VET_FLOAT3, ogre.VertexElementSemantic.VES_POSITION) 
        offset += ogre.VertexElement.getTypeSize(ogre.VertexElementType.VET_FLOAT3) 
        dcl.addElement(0, offset, ogre.VertexElementType.VET_FLOAT3, ogre.VertexElementSemantic.VES_NORMAL) 
        offset += ogre.VertexElement.getTypeSize(ogre.VertexElementType.VET_FLOAT3) 
        dcl.addElement(0, offset, ogre.VertexElementType.VET_FLOAT2, ogre.VertexElementSemantic.VES_TEXTURE_COORDINATES) 
        offset += ogre.VertexElement.getTypeSize(ogre.VertexElementType.VET_FLOAT2) 

        vbuf = ogre.HardwareBufferManager.getSingleton().createVertexBuffer(
                offset, 12, ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY) 

        #lock the buffer and get a pointer to the data      
        pointer = vbuf.lock(ogre.HardwareBuffer.HBL_DISCARD)
         
        baseVec = ogre.Vector3(GRASS_WIDTH/2, 0, 0) 
        vec = baseVec 
        rot = ogre.Quaternion () 
        rot.FromAngleAxis(ogre.Degree(d=60), ogre.Vector3.UNIT_Y) 
        
        # change from the C++ demo - we build a buffer and then write it in one go
        buffer=[]
        for i in range ( 3 ) :
            ## position
            buffer.append( -vec.x )
            buffer.append( GRASS_HEIGHT )
            buffer.append( -vec.z )
            ## normal
            buffer.append( 0 )
            buffer.append( 1 )
            buffer.append( 0 )
            ## uv
            buffer.append( 0 )
            buffer.append( 0 )
            
            ## position
            buffer.append( vec.x )
            buffer.append( GRASS_HEIGHT )
            buffer.append( vec.z )
            ## normal
            buffer.append( 0 )
            buffer.append( 1 )
            buffer.append( 0 )
            ## uv
            buffer.append( 1 )
            buffer.append( 0 )
            
            ## position
            buffer.append( -vec.x )
            buffer.append( 0 )
            buffer.append( -vec.z )
            ## normal
            buffer.append( 0 )
            buffer.append( 1 )
            buffer.append( 0 )
            ## uv
            buffer.append( 0 )
            buffer.append( 1 )
            
            ## position
            buffer.append( vec.x )
            buffer.append( 0 )
            buffer.append( vec.z )
            ## normal
            buffer.append( 0 )
            buffer.append( 1 )
            buffer.append( 0 )
            ## uv
            buffer.append( 1 )
            buffer.append( 1 )
            
            vec = rot * vec 
            
        ## Python-Ogre extension function to write a list of floats to a buffer
        ogre.setFloat( pointer, buffer )
        vbuf.unlock() 
              
        sm.vertexData.vertexBufferBinding.setBinding(0, vbuf) 
          
        sm.indexData.indexCount = 6*3 
        
        sm.indexData.indexBuffer = ogre.HardwareBufferManager.getSingleton().createIndexBuffer(
                ogre.HardwareIndexBuffer.IT_16BIT, sm.indexData.indexCount,
                ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY) 
        
        pointer = sm.indexData.indexBuffer.lock(ogre.HardwareBuffer.HBL_DISCARD)

        ## here is one way to do this using a Python-Ogre helper function        
# #         buff=[]
# #         for i in range ( 3 ):
# #             off = i*4 
# #             buff.append( 0 + off )
# #             buff.append( 3 + off ) 
# #             buff.append( 1 + off )
# #             
# #             buff.append( 0 + off )
# #             buff.append( 2 + off )
# #             buff.append( 3 + off )
# #             
# #         # another Python-Ogre helper function to write unsigned ints to a buffer
# #         ogre.setUint16( pointer, buff )   

        ## Here is a way to do it with ctypes....
        pVert= ( ctypes.c_uint16 * (sm.indexData.indexCount)).from_address ( ogre.CastInt ( pointer ) ) 
        index = 0
        for i in range ( 3 ):
            off = i*4
            pVert[ index ] = 0 + off
            index +=1
            pVert[ index ] = 3 + off
            index +=1
            pVert[ index ] = 1 + off
            index +=1
            pVert[ index ] = 0 + off
            index +=1
            pVert[ index ] = 2 + off
            index +=1
            pVert[ index ] = 3 + off
            index +=1
        sm.indexData.indexBuffer.unlock() 
        
        
        sm.setMaterialName(GRASS_MATERIAL) 
        msh.load()
        ## we need to keep access to the submesh so we can set .vertexData to None at exit (see __del__ )
        self.sm = sm 



    def  setupLighting(self):
        global mMinLightColour, mMaxLightColour, mMinFlareSize, mMaxFlareSize, mAnimState
        ## Set ambient light
        print "3"
        self.sceneManager.setAmbientLight(ogre.ColourValue(0.2, 0.2, 0.2)) 
        ## Point light, movable, reddish
        self.mLight = self.sceneManager.createLight("Light2") 
        self.mLight.setDiffuseColour(mMinLightColour) 
        self.mLight.setSpecularColour(1, 1, 1) 
        self.mLight.setAttenuation(8000,1,0.0005,0) 

        ## Create light node
        self.mLightNode = self.sceneManager.getRootSceneNode().createChildSceneNode(
            "MovingLightNode") 
        self.mLightNode.attachObject(self.mLight) 
        ## create billboard set
        bbs = self.sceneManager.createBillboardSet("lightbbs", 1) 
        bbs.setMaterialName("Examples/Flare") 
        bb = bbs.createBillboard(0,0,0,mMinLightColour) 
        ## attach
        self.mLightNode.attachObject(bbs) 
        
        ## create controller, after self is will get updated on its own
        ## note you need to keep the func and val 'classes' around hence they are self.* 's
        self.func = ogre.WaveformControllerFunction(ogre.WFT_SINE, 0.0, 0.5)
        self.val = LightWibbler(self.mLight, bb, mMinLightColour, mMaxLightColour, 
            mMinFlareSize, mMaxFlareSize)
        contMgr = ogre.ControllerManager.getSingleton() 
        controller = contMgr.createController(contMgr.getFrameTimeSource(), self.val, self.func) 

        self.mLightNode.setPosition(ogre.Vector3(300,250,-300)) 
        
        ## Create a track for the light
        anim = self.sceneManager.createAnimation("LightTrack", 20) 
        ## Spline it for nice curves
        anim.setInterpolationMode(ogre.Animation.IM_SPLINE) 
        ## Create a track to animate the camera's node
        track = anim.createNodeTrack(0, self.mLightNode) 
        ## Setup keyframes
        key = track.createNodeKeyFrame(0)  ## A startposition
        key.setTranslate(ogre.Vector3(300,550,-300)) 
        key = track.createNodeKeyFrame(2) ##B
        key.setTranslate(ogre.Vector3(150,600,-250)) 
        key = track.createNodeKeyFrame(4) ##C
        key.setTranslate(ogre.Vector3(-150,650,-100)) 
        key = track.createNodeKeyFrame(6) ##D
        key.setTranslate(ogre.Vector3(-400,500,-200)) 
        key = track.createNodeKeyFrame(8) ##E
        key.setTranslate(ogre.Vector3(-200,500,-400)) 
        key = track.createNodeKeyFrame(10) ##F
        key.setTranslate(ogre.Vector3(-100,450,-200)) 
        key = track.createNodeKeyFrame(12) ##G
        key.setTranslate(ogre.Vector3(-100,400,180)) 
        key = track.createNodeKeyFrame(14) ##H
        key.setTranslate(ogre.Vector3(0,250,600)) 
        key = track.createNodeKeyFrame(16) ##I
        key.setTranslate(ogre.Vector3(100,650,100)) 
        key = track.createNodeKeyFrame(18) ##J
        key.setTranslate(ogre.Vector3(250,600,0)) 
        key = track.createNodeKeyFrame(20) ##K == A
        key.setTranslate(ogre.Vector3(300,550,-300)) 
        ## Create a new animation state to track self
        mAnimState = self.sceneManager.createAnimationState("LightTrack") 
        mAnimState.setEnabled(True) 
        print "4"


    def _createScene(self):
        global GRASS_MESH_NAME, VES_TANGENT, mStaticGeom
        self.sceneManager.setSkyBox(True, "Examples/SpaceSkyBox") 
        self.setupLighting() 

        plane = ogre.Plane() 
        plane.normal = ogre.Vector3.UNIT_Y 
        plane.d = 0 
        ogre.MeshManager.getSingleton().createPlane("Myplane",
            ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, plane,
            14500,14500,10,10,True,1,50,50,ogre.Vector3.UNIT_Z) 
        pPlaneEnt = self.sceneManager.createEntity( "plane", "Myplane" ) 
        pPlaneEnt.setMaterialName("Examples/GrassFloor") 
        pPlaneEnt.setCastShadows(False) 
        self.sceneManager.getRootSceneNode().createChildSceneNode().attachObject(pPlaneEnt) 

        minV = ogre.Vector3 (-2000,0,-2000) 
        maxV = ogre.Vector3 (2000,0,2000) 


        self.createGrassMesh() 

        e = self.sceneManager.createEntity("1", GRASS_MESH_NAME) 

        s = self.sceneManager.createStaticGeometry("bing") 
        s.setRegionDimensions(ogre.Vector3(1000,1000,1000)) 
        ## Set the region origin so the centre is at 0 world
        s.setOrigin(ogre.Vector3(-500, 500, -500)) 

        for  x in range(-1950,1950,150):
            for z in range(-1950, 1950,  150):
                pos = ogre.Vector3 ( x + random.randrange(-25, 25), 0, z + random.randrange(-25, 25)) 
                orientation = ogre.Quaternion()
                orientation.FromAngleAxis(
                    ogre.Degree(random.randrange(0, 359)),
                    ogre.Vector3.UNIT_Y) 
                scale = ogre.Vector3( 1, random.random()*(1.15-0.85)+0.85, 1)
                s.addEntity(e, pos, orientation, scale) 
        s.build() 
        mStaticGeom = s     ## need to make it available else where
        print "1"
        ## Put an Ogre head in the middle
        m = ogre.MeshManager.getSingleton().load("ogrehead.mesh", 
           ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME) 
        
          
        if ogre.GetPythonOgreVersion()[1] == '1' and int(ogre.GetPythonOgreVersion()[1]) < 2:   ## Python Ogre after 1.2 has API changes        
            ### Note the python version of suggestTangentVectorBuildParams!!
            ret, src, dest = m.suggestTangentVectorBuildParams(ogre.VES_TANGENT)
            if ( not ret):
                m.buildTangentVectors(ogre.VES_TANGENT, src, dest) 
        else:   ## note the change for more recent version of Python-Ogre...                
            ### Note the python version of suggestTangentVectorBuildParams!!
            ret, src, dest = m.suggestTangentVectorBuildParams(ogre.VES_TANGENT,1,1)
            if ( not ret):
                m.buildTangentVectors(ogre.VES_TANGENT, src, dest) 

        e = self.sceneManager.createEntity("head", "ogrehead.mesh") 
        e.setMaterialName("Examples/OffsetMapping/Specular") 
        headNode = self.sceneManager.getRootSceneNode().createChildSceneNode() 
        headNode.attachObject(e) 
        headNode.setScale(7,7,7) 
        headNode.setPosition(0,200,0) 
        headNode.yaw(ogre.Degree(15))
        self.camera.move(ogre.Vector3(0,350,0))
	
    def _createFrameListener(self):
        self.frameListener = GrassListener(self.renderWindow, self.camera, self.sceneManager)
        self.root.addFrameListener(self.frameListener)

if __name__ == '__main__':
    import exceptions
    try:
        application = Grass_Application()
        application.go()
    except ogre.OgreException, e:
        print e
