# NO_UNITTEST
# This code is in the Public Domain
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http://python-Ogre.org/
#
# It is likely based on original code from OGRE and/or PyOgre
# For the latest info, see http://www.ogre3d.org/
#
# You may use this sample code for anything you like, it is not covered by the
# LGPL.
# -----------------------------------------------------------------------------
# */
# /* Static water simulation by eru
#  * Started 29.05.2003, 20:54:37
#  */
import sys
try:
   import psyco
   psyco.full()
except ImportError:
   sys.exit()
sys.path.insert(0,'..')
import PythonOgreConfig
import ogre.renderer.OGRE as Ogre
import ogre.io.OIS as OIS
import WaterMesh as WaterMesh
import math
import SampleFramework as sf
import random
import ctypes as ctypes
import array as array
# AnimationState* self.mAnimState 

## Mesh stuff
MESH_NAME ="WaterMesh"
ENTITY_NAME ="waterEntity"
MATERIAL_PREFIX ="Examples/Water"
MATERIAL_NAME ="Examples/Water0"
COMPLEXITY = 64     ## watch out - number of polys is 2*ACCURACY*ACCURACY !
PLANE_SIZE =3000.0

RAIN_HEIGHT_RANDOM = 5
RAIN_HEIGHT_CONSTANT = 5

CIRCLES_MATERIAL ="Examples/Water/Circles"

##
## Note that this function makes use of CTypes and def ptr casting to access Ogre Library functions
##
def prepareCircleMaterial():
    global CIRCLES_MATERIAL
    bmap = array.array('B')  # unsigned char -- int in python
    for x in range (256*256*4):
        bmap.append(127)
    
    for  b in range(16):
        x0 = b % 4  
        y0 = b >> 2  
        radius = 4.0 + 1.4 * b  
        for x in range(64):
            for y in range (64) :
                dist = math.sqrt((x-32)*(x-32)+(y-32)*(y-32))  ## 0..ca.45
                dist = math.fabs(dist-radius-2) / 2.0  
                dist = dist * 255.0 
                
                if (dist>255):
                    dist=255  
                colour = 255- int(dist)
                colour = int (float(15-b)/15.0 * colour )
                bmap[4*(256*(y+64*y0)+x+64*x0)+0]=colour  
                bmap[4*(256*(y+64*y0)+x+64*x0)+1]=colour 
                bmap[4*(256*(y+64*y0)+x+64*x0)+2]=colour 
                bmap[4*(256*(y+64*y0)+x+64*x0)+3]=colour 
                
    ### Need to  pass the address to MemoryDataStream
    ##  
    imgstream = Ogre.MemoryDataStream(pMem=Ogre.castAsVoidPtr(bmap.buffer_info()[0]),  size = 256 * 256 *4 )  
    Ogre.TextureManager.getSingleton().loadRawData(CIRCLES_MATERIAL,
        Ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME,
        imgstream, 256, 256  , Ogre.PixelFormat.PF_A8R8G8B8 ) 
    material = Ogre.MaterialManager.getSingleton().create( CIRCLES_MATERIAL,
        Ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME) 
    texLayer = material.getTechnique(0).getPass(0).createTextureUnitState( CIRCLES_MATERIAL ) 
    texLayer.setTextureAddressingMode( Ogre.TextureUnitState.TAM_CLAMP ) 
    material.setSceneBlending( Ogre.SceneBlendType.SBT_ADD ) 
    material.setDepthWriteEnabled( False )  
    material.load() 


# /* =========================================================================*/
# /*               WaterListener class                                          */
# /* =========================================================================*/
## Event handler
class WaterListener(sf.FrameListener):

    def processcircles(self,timeSinceLastFrame):
        for i in self.circles :
            i.animate(timeSinceLastFrame) 
        for count in range (len(self.circles)-1,0,-1):
            if self.circles[count].lvl >= 16:
                c = self.circles[count]
                del c
                del self.circles[count]
                #break
            

    def processParticles(self):
        global PLANE_SIZE, COMPLEXITY, RAIN_HEIGHT_RANDOM, RAIN_HEIGHT_CONSTANT
###        for particle in self.app.particleSystem.getParticles(): # note that this helper function returns copies
        for x in range (self.app.particleSystem.getNumParticles()):
            particle = self.app.particleSystem.getParticle(x)
            ppos = particle.position 
            if ppos.y<=0 and particle.timeToLive>0 : ## hits the water!:
                ## delete particle
                particle.timeToLive = 0.0 
                ## push the water
                x = ppos.x / PLANE_SIZE * COMPLEXITY  
                y = ppos.z / PLANE_SIZE * COMPLEXITY  
                h = random.uniform(0,RAIN_HEIGHT_RANDOM-1) + RAIN_HEIGHT_CONSTANT  
                if (x<1): x=1 
                if (x>COMPLEXITY-1): x=COMPLEXITY-1 
                if (y<1): y=1 
                if (y>COMPLEXITY-1): y=COMPLEXITY-1
                self.WaterMesh.push(x,y,-h)  
                circle = WaterMesh.WaterCircle( "Circle#"+str(self.pindex), x, y, self.app.sceneManager, COMPLEXITY)
                self.pindex+=1 
                self.circles.append(circle) 
                
    ## Head animation */
    def animateHead(self, timeSinceLastFrame):
        #return
        ## sine track? :)
        for i in range(4):
            self.sines[i]+=self.adds[i]*timeSinceLastFrame 

        tx = ((math.sin(self.sines[0]) + math.sin(self.sines[1])) / 4 + 0.5 ) * (COMPLEXITY-2) + 1  
        ty = ((math.sin(self.sines[2]) + math.sin(self.sines[3])) / 4 + 0.5 ) * (COMPLEXITY-2) + 1  
        self.WaterMesh.push(tx,ty, -self.headDepth) 
        step = PLANE_SIZE / COMPLEXITY  
        self.app.headNode.resetToInitialState() 
        self.app.headNode.setScale((3,3,3)) 
        newPos = Ogre.Vector3(step*tx, self.headDepth, step*ty) 
        diffPos = newPos - self.oldPos  
        headRotation = Ogre.Vector3().UNIT_Z.getRotationTo(diffPos) 
        self.oldPos = newPos  
        self.app.headNode.translate(newPos) 
        self.app.headNode.rotate(headRotation) 

    ## GUI updaters
    def updateInfoParamC(self):
        Ogre.OverlayManager.getSingleton().getOverlayElement("Example/Water/Param_C") \
            .setCaption("[1/2]Ripple speed: "+str(self.WaterMesh.PARAM_C)) 

    def updateInfoParamD(self):
        Ogre.OverlayManager.getSingleton().getOverlayElement("Example/Water/Param_D") \
            .setCaption("[3/4]Distance: "+str(self.WaterMesh.PARAM_D)) 

    def updateInfoParamU(self):
        Ogre.OverlayManager.getSingleton().getOverlayElement("Example/Water/Param_U") \
            .setCaption("[5/6]Viscosity: "+str(self.WaterMesh.PARAM_U)) 

    def updateInfoParamT(self):
        Ogre.OverlayManager.getSingleton().getOverlayElement("Example/Water/Param_T") \
            .setCaption("[7/8]Frame time: "+str(self.WaterMesh.PARAM_T)) 

    def updateInfoNormals(self):
        cap = "[N]Normals: "
        if self.WaterMesh.normalsToUse == 0:
            cap += "fake"
        elif self.WaterMesh.normalsToUse == 1:
            cap += "real - Python"
        else:
            cap += "real - cython"
        Ogre.OverlayManager.getSingleton().getOverlayElement("Example/Water/Normals") \
            .setCaption(cap) 

    def switchNormals(self):
        if WaterMesh.CYTHONOK:
            self.WaterMesh.normalsToUse =  (self.WaterMesh.normalsToUse +1 ) % 3 
        else:  # only have 2 options as cython module didn't load
            self.WaterMesh.normalsToUse =  (self.WaterMesh.normalsToUse +1 ) % 2 
        self.updateInfoNormals()  

    def updateInfoHeadDepth(self):
        Ogre.OverlayManager.getSingleton().getOverlayElement("Example/Water/Depth").setCaption("[U/J]Head depth: "+str(self.headDepth))
             
    def updateInfoSkyBox(self):
        cap = "[B]SkyBox: "
        if self.skyBoxOn:
            cap += "On"
        else: cap += "Off"
        Ogre.OverlayManager.getSingleton().getOverlayElement("Example/Water/SkyBox").setCaption(cap) 

    def updateMaterial(self):
        global MATERIAL_PREFIX
        materialName = MATERIAL_PREFIX+str(self.materialNumber) 
        material = Ogre.MaterialManager.getSingleton().getByName(materialName) 
        if not material:
            if(self.materialNumber):
                self.materialNumber = 0  
                self.updateMaterial() 
                return  
            else:
                Ogre.OGRE_EXCEPT(Exception.ERR_INTERNAL_ERROR,
                    "Material "+materialName+"doesn't exist!",
                    "WaterListener.updateMaterial") 

        self.waterEntity.setMaterialName(materialName) 
        Ogre.OverlayManager.getSingleton().getOverlayElement("Example/Water/Material").setCaption("[M]Material: "+materialName) 


    def switchMaterial(self):
        self.materialNumber+=1 
        self.updateMaterial() 

    def switchSkyBox(self):
        self.skyBoxOn = not self.skyBoxOn 
        self.app.sceneManager.setSkyBox(self.skyBoxOn, "Examples/SceneSkyBox2") 
        self.updateInfoSkyBox() 


    def __init__(self, renderWindow, camera, WaterMesh, waterEntity, app ):
    
        sf.FrameListener.__init__(self, renderWindow, camera)
        self.app = app
        self.camera = camera
        self.WaterMesh = WaterMesh  
        self.waterEntity = waterEntity  
        self.materialNumber = 8 
        self.timeoutDelay = 0.0 
        self.headDepth = 1.7 
        self.skyBoxOn = False  
        RAIN_HEIGHT_RANDOM = 5
        RAIN_HEIGHT_CONSTANT = 5
        self.sines = [0,100,200,300] 
        self.adds=[0.3,-1.6,1.1,0.5] 
        self.oldPos = Ogre.Vector3().UNIT_Z 
        self.pindex = 0  
        self.circles = []
        
        self.updateMaterial() 
        self.updateInfoParamC() 
        self.updateInfoParamD() 
        self.updateInfoParamU() 
        self.updateInfoParamT() 
        self.updateInfoNormals() 
        self.updateInfoHeadDepth() 
        self.updateInfoSkyBox() 


    def __del__ ( self ):
        ## If when you finish the application is still raining there
        ## are water self.circles that are still being processed
        activecircles = self.circles.size () 

        ## Kill the active water self.circles
        for i in range ( activecircles ):
            del (self.circles[i]) 


    def frameRenderingQueued(self, frameEvent):
        result = sf.FrameListener.frameRenderingQueued(self,frameEvent)
        evt = frameEvent
        
        if( result == False ):
            ## check if we are exiting, if so, clear static HardwareBuffers to adef segfault
            #WaterCircle.clearStaticBuffers() 
            return False 

        self.app.mAnimState.addTime(evt.timeSinceLastFrame) 

        ## process keyboard events
        changeSpeed = evt.timeSinceLastFrame  

        ## adjust keyboard speed with SHIFT (increase) and CONTROL (decrease)
        if (self.Keyboard.isKeyDown(OIS.KC_LSHIFT) or self.Keyboard.isKeyDown(OIS.KC_RSHIFT)):
            changeSpeed *= 10.0  

        if (self.Keyboard.isKeyDown(OIS.KC_LCONTROL)):
            changeSpeed /= 10.0  

        ## rain
        self.processcircles(evt.timeSinceLastFrame) 
        if (self.Keyboard.isKeyDown(OIS.KC_SPACE)):
            self.app.particleEmitter.setEmissionRate(20.0) 
        else:
            self.app.particleEmitter.setEmissionRate(0.0) 

        self.processParticles() 

        ## adjust values (some macros for faster change
        def ADJUST_RANGE(_value,_plus,_minus,_minVal,_maxVal,_change,_macro):
            if (self.Keyboard.isKeyDown(_plus)):
                _value+=_change
                if (_value>=_maxVal):
                     _value = _maxVal
                _macro 
            if (self.Keyboard.isKeyDown(_minus)):
                _value-=_change  
                if (_value<=_minVal):
                     _value = _minVal
                _macro 
            return _value


        self.headDepth=ADJUST_RANGE(self.headDepth, OIS.KC_U, OIS.KC_J, 0, 10, 0.5*changeSpeed, self.updateInfoHeadDepth()) 
        self.WaterMesh.PARAM_C=ADJUST_RANGE(self.WaterMesh.PARAM_C, OIS.KC_2, OIS.KC_1, 0, 10, 0.1*changeSpeed, self.updateInfoParamC())  
        self.WaterMesh.PARAM_D=ADJUST_RANGE(self.WaterMesh.PARAM_D, OIS.KC_4, OIS.KC_3, 0.1, 10, 0.1*changeSpeed, self.updateInfoParamD())  
        self.WaterMesh.PARAM_U=ADJUST_RANGE(self.WaterMesh.PARAM_U, OIS.KC_6, OIS.KC_5, -2, 10, 0.1*changeSpeed, self.updateInfoParamU())  
        self.WaterMesh.PARAM_T=ADJUST_RANGE(self.WaterMesh.PARAM_T, OIS.KC_8, OIS.KC_7, 0, 10, 0.1*changeSpeed, self.updateInfoParamT())  

        self.timeoutDelay-=evt.timeSinceLastFrame  
        if (self.timeoutDelay<=0):
            self.timeoutDelay = 0 

        def SWITCH_VALUE(_key,_timeDelay, _macro):
            res = self.Keyboard.isKeyDown(_key)
            if self.Keyboard.isKeyDown(_key) and self.timeoutDelay==0 :
                self.timeoutDelay = _timeDelay   
                _macro ()

        SWITCH_VALUE(OIS.KC_N, 0.5, self.switchNormals) 
        SWITCH_VALUE(OIS.KC_M, 0.5, self.switchMaterial) 
        SWITCH_VALUE(OIS.KC_B, 0.5, self.switchSkyBox) 
        
        self.animateHead(evt.timeSinceLastFrame) 
        self.WaterMesh.updateMesh(evt.timeSinceLastFrame) 

        return True 



class WaterApplication(sf.Application,Ogre.RenderTargetListener):
    
## Just override the mandatory create scene method
    def _createScene(self):
        global PLANE_SIZE, MESH_NAME, COMPLEXITY, ENTITY_NAME
        sceneManager = self.sceneManager
        camera = self.camera
        ## Set ambient light
        sceneManager.setAmbientLight(Ogre.ColourValue(0.75, 0.75, 0.75)) 

        ## Create a light
        l = sceneManager.createLight("MainLight") 
        ## Accept default settings: point light, white diffuse, just set position
        l.setPosition(200,300,100) 

#         ## Create water mesh and entity
        self.WaterMesh = WaterMesh.WaterMesh(MESH_NAME, PLANE_SIZE, COMPLEXITY) 
        self.waterEntity = sceneManager.createEntity(ENTITY_NAME, MESH_NAME) 
        
        ## self.waterEntity.setMaterialName(MATERIAL_NAME) 
        waterNode = sceneManager.getRootSceneNode().createChildSceneNode() 
        waterNode.attachObject(self.waterEntity) 

        ## Add a head, give it it's own node
        self.headNode = waterNode.createChildSceneNode() 
        ent = sceneManager.createEntity("head", "ogrehead.mesh") 
        self.headNode.attachObject(ent) 

        ## Make sure the camera track self node
        ## self.camera.setAutoTracking(True, self.headNode) 

        ## Create the camera node, set its position & attach camera
        camNode = sceneManager.getRootSceneNode().createChildSceneNode() 
        camNode.translate(0, 500, PLANE_SIZE) 
        camNode.yaw(Ogre.Degree(-45)) 
        camNode.attachObject(self.camera) 

        ## Create light node
        lightNode = sceneManager.getRootSceneNode().createChildSceneNode() 
        lightNode.attachObject(l) 

        ## set up spline animation of light node
        anim = sceneManager.createAnimation("WaterLight", 20) 
         
        ## create a random spline for light
        track = anim.createNodeTrack(0, lightNode) 
        key = track.createNodeKeyFrame(0) 
        for  ff in range (1,20):
            key = track.createNodeKeyFrame(ff) 
            lpos = Ogre.Vector3(
                random.uniform(0,PLANE_SIZE-1) , ##- PLANE_SIZE/2,
                random.uniform(0,299)+100,
                random.uniform(0,PLANE_SIZE-1) ##- PLANE_SIZE/2
                ) 
            key.setTranslate(lpos) 

        key = track.createNodeKeyFrame(20) 

        ## Create a new animation state to track self
        self.mAnimState = sceneManager.createAnimationState("WaterLight") 
        self.mAnimState.setEnabled(True) 

        ## Put in a bit of fog for the hell of it
        ##sceneManager.setFog(FOG_EXP, ColourValue.White, 0.0002) 

        ## show overlay
        self.waterOverlay = Ogre.OverlayManager.getSingleton().getByName("Example/WaterOverlay") 
        self.waterOverlay.show() 

        ## Let there be rain
        self.particleSystem = sceneManager.createParticleSystem("rain",
            "Examples/Water/Rain") 
        self.particleEmitter = self.particleSystem.getEmitter(0) 
        rNode = sceneManager.getRootSceneNode().createChildSceneNode() 
        rNode.translate(PLANE_SIZE/2.0, 3000, PLANE_SIZE/2.0) 
        rNode.attachObject(self.particleSystem) 
        ## Fast-forward the rain so it looks more natural
        #self.particleSystem.fastForward(20) 
        ## It can't be set in .particle file, and we need it  )
        self.particleSystem.getRenderer().setBillboardOrigin(Ogre.BBO_BOTTOM_CENTER) 
        prepareCircleMaterial() 
        

#     ## Create new frame listener
    def _createFrameListener(self):
        self.frameListener = WaterListener(self.renderWindow, self.camera,
                                        self.WaterMesh, self.waterEntity, self)
        self.root.addFrameListener(self.frameListener)

 

if __name__ == '__main__':
    application = WaterApplication()
    if False:  # use for profiling
        import cProfile
        import pstats
        cProfile.run('application.go()', 'profile.out')
        p = pstats.Stats('profile.out').strip_dirs()
        p.sort_stats('time','cumulative').print_stats(15)
    else:
        application.go()
