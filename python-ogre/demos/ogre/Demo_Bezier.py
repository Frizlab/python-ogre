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

# 
# #     \file 
# #         Bezier.h
# #     \brief
# #         Specialisation of OGRE's framework application to show off
# #         the bezier patch support.
# 
import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ctypes, math

##patch = ogre.PatchMesh()
##Pass = ogre.Pass()

## Logging class
class MyLog(ogre.LogListener):
    def __init__(self):
        # Creates a C++ log that will try and write to console and file
        ogre.LogListener.__init__(self)
                 
    def messageLogged(self, message, level, debug, logName):
        # This should be called by Ogre instead of logging
        pass
        #print message
        
        
## Event handler to add ability to alter subdivision
class BezierListener(sf.FrameListener):
    def __init__(self, renderWindow, camera ):
        self.timeLapse = 0.0 
        self.factor = 0.0 
        self.wireframe = 0 
        self.renderWindow = renderWindow
        sf.FrameListener.__init__(self, renderWindow, camera)
        
    def frameStarted(self, frameEvent):
        global patchPass, patch
        if( sf.FrameListener.frameStarted(self, frameEvent) == False ):
            return False 
        self.timeLapse += frameEvent.timeSinceLastFrame 
        ## Prgressively grow the patch
        if (self.timeLapse > 1.0):
            self.factor += 0.2 
            if (self.factor > 1.0) :
                self.wireframe = not self.wireframe 
                ##self.camera.setPolygonMode(wireframe ? PM_WIREFRAME : PM_SOLID) 
                if self.wireframe:
                    patchPass.setPolygonMode(ogre.PolygonMode.PM_WIREFRAME )
                else:
                    patchPass.setPolygonMode(ogre.PolygonMode.PM_SOLID)
                self.factor = 0.0 
            patch.setSubdivision(self.factor) 
            sf.Application.debugText  = "Bezier subdivision factor: " + str(self.factor)
            self.timeLapse = 0.0 

        ## Call default
        return True 


class BezierApplication(sf.Application):
    def __init__(self):
        sf.Application.__init__(self)
        patchDecl = ogre.VertexDeclaration()
        self.patchCtlPoints =0
        # Create the global log manager instance
        self.logMgr = ogre.LogManager()
        # create the instance of our log listener
        self.myLog = MyLog()
        # create a "log"
        self.currentLog = ogre.LogManager.getSingletonPtr().createLog("dummy.log" 
                                                    , True  # it's the default log
                                                    , False     # I don't want it sent to the debug window
                                                    , True     # it's a virtual log, so you need a listener :)
                                                    )  
        # register our listener
        self.currentLog.addListener ( self.myLog )    
        
         
    def _createScene(self):
        global patchPass, patch, pVert
        sceneManager = self.sceneManager

        ## Set ambient light
        sceneManager.setAmbientLight( (0.2, 0.2, 0.2) ) 
        
        ## Create a point light
        l = sceneManager.createLight("MainLight") 
        l.setType(ogre.Light.LT_DIRECTIONAL) 
        l.setDirection(-0.5, -0.5, 0) 

        ## Create patch - here we are creating a memoryblock that is 8 floats (x,y,z,nx,ny,nz,u,v)
        ## Notice how we increment the offset for each new element
        offset = 0
        
        patchDecl = ogre.HardwareBufferManager.getSingleton().createVertexDeclaration() 
        patchDecl.addElement(0, 0, ogre.VertexElementType.VET_FLOAT3, ogre.VertexElementSemantic.VES_POSITION)
        offset += ogre.VertexElement.getTypeSize(ogre.VertexElementType.VET_FLOAT3) 

        patchDecl.addElement(0, offset, ogre.VertexElementType.VET_FLOAT3, ogre.VertexElementSemantic.VES_NORMAL) 
        offset += ogre.VertexElement.getTypeSize(ogre.VertexElementType.VET_FLOAT3) 

        patchDecl.addElement(0, offset, ogre.VertexElementType.VET_FLOAT2, ogre.VertexElementSemantic.VES_TEXTURE_COORDINATES, 0) 
        offset += ogre.VertexElement.getTypeSize(ogre.VertexElementType.VET_FLOAT2) 

        ## Make a patch for test - there are 9 'patches, each with 8 floats as above
        storageclass = ctypes.c_float * (8*9)
        pVert=storageclass(1.1)
        ctypes.memset ( pVert, 0, 8*9 )      # just showing how this can be done

         ## Patch data
        inc = 0
        pVert[inc+0] = -500.0
        pVert[inc+1] = 200.0
        pVert[inc+2] = -500.0
        pVert[inc+3] = -0.5  
        pVert[inc+4] = 0.5 
        pVert[inc+5] = 0.0
        pVert[inc+6] = 0.0  
        pVert[inc+7] = 0.0
        inc=8
        pVert[inc+0] = 0.0  
        pVert[inc+1] = 500.0 
        pVert[inc+2] = -750.0
        pVert[inc+3] = 0.0  
        pVert[inc+4] = 0.5 
        pVert[inc+5] = 0.0
        pVert[inc+6] = 0.5  
        pVert[inc+7] = 0.0
        inc = 16 
        pVert[inc+0] = 500.0  
        pVert[inc+1] = 1000.0 
        pVert[inc+2] = -500.0
        pVert[inc+3] = 0.5  
        pVert[inc+4] = 0.5 
        pVert[inc+5] = 0.0
        pVert[inc+6] = 1.0  
        pVert[inc+7] = 0.0
        inc = 24 
        pVert[inc+0] = -500.0  
        pVert[inc+1] = 0.0 
        pVert[inc+2] = 0.0
        pVert[inc+3] = -0.5  
        pVert[inc+4] = 0.5 
        pVert[inc+5] = 0.0
        pVert[inc+6] = 0.0  
        pVert[inc+7] = 0.5
        inc = 32
        pVert[inc+0] = 0.0  
        pVert[inc+1] = 500.0 
        pVert[inc+2] = 0.0
        pVert[inc+3] = 0.0  
        pVert[inc+4] = 0.5 
        pVert[inc+5] = 0.0
        pVert[inc+6] = 0.5  
        pVert[inc+7] = 0.5
        inc = 40 
        pVert[inc+0] = 500.0  
        pVert[inc+1] = -50.0 
        pVert[inc+2] = 0.0
        pVert[inc+3] = 0.5  
        pVert[inc+4] = 0.5 
        pVert[inc+5] = 0.0
        pVert[inc+6] = 1.0  
        pVert[inc+7] = 0.5
        inc = 48
        pVert[inc+0] = -500.0  
        pVert[inc+1] = 0.0 
        pVert[inc+2] = 500.0
        pVert[inc+3] = -0.5  
        pVert[inc+4] = 0.5 
        pVert[inc+5] = 0.0
        pVert[inc+6] = 0.0  
        pVert[inc+7] = 1.0
        inc = 56 
        pVert[inc+0] = 0.0  
        pVert[inc+1] = 500.0 
        pVert[inc+2] = 500.0
        pVert[inc+3] = 0.0  
        pVert[inc+4] = 0.5 
        pVert[inc+5] = 0.0
        pVert[inc+6] = 0.5  
        pVert[inc+7] = 1.0
        inc = 64 
        pVert[inc+0] = 500.0  
        pVert[inc+1] = 200.0 
        pVert[inc+2] = 800.0
        pVert[inc+3] = 0.5  
        pVert[inc+4] = 0.5 
        pVert[inc+5] = 0.0
        pVert[inc+6] = 1.0  
        pVert[inc+7] = 1.0
        print "\n1"
        
        ##
        ## NOTE: Python-Ogre Special :)
        ## createBezierPatch requires the address of te buffer which we pass using the addressof method
        ## in ctypes..
        ##
        patch = ogre.MeshManager.getSingleton().createBezierPatch(
            "Bezier1", ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME,
            ctypes.addressof(pVert), patchDecl, 3,3, 5, 5, visibleSide=ogre.PatchSurface.VS_BOTH )
            
        ## the rest of these are default settings - so you can use them or Python-Ogre will fill in the
        ## default values for you
        ##    ,vbUsage=ogre.HardwareBuffer.HBU_STATIC_WRITE_ONLY, ibUsage=ogre.HardwareBuffer.HBU_DYNAMIC_WRITE_ONLY,
        ##    vbUseShadow=True, ibUseShadow=True) 
        
        ## Start patch at 0 detail
        patch.setSubdivision(0.0) 
        ## Create entity based on patch
        patchEntity = sceneManager.createEntity("Entity1", "Bezier1") 

        pMat = ogre.MaterialManager.getSingleton().create("TextMat", 
            ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME) 
        pMat.getTechnique(0).getPass(0).createTextureUnitState( "BumpyMetal.jpg" ) 
        patchEntity.setMaterialName("TextMat") 
        patchPass = pMat.getTechnique(0).getPass(0)
        
#        print patchPass
#        print dir(patchPass)
#        print patchPass.msDirtyHashListMutex
#        sys.exit()


        ## Attach the entity to the root of the scene
        sceneManager.getRootSceneNode().attachObject(patchEntity) 

        self.camera.setPosition(500,500, 1500) 
        self.camera.lookAt(0,200,-300) 
        sf.Application.debugText  = "STARTED"


# # #     def __del__(self):
# # # 
# # #         ## free up the pointer before we shut down OGRE
# # #         patch.setNull() 

    def _createFrameListener(self):
        self.frameListener = BezierListener(self.renderWindow, self.camera)
        self.root.addFrameListener(self.frameListener)
        self.frameListener.showDebugOverlay( True )
        
        
        
if __name__ == '__main__':
    import exceptions,sys
    try:
        application = BezierApplication()
        application.go()
    except ogre.OgreException, e:
        print e
