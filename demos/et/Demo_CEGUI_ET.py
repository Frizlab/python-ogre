#!/usr/bin/env python

## EDITABLE TERRAIN MANAGER
## ==========================
##    The Editable Terrain Manager (ETM) is an addon library for OGRE which manages
##    and displays terrain based on heightmap data. 

## ETM AUTHOR:
## ===========
## created 2007 by Holger Frydrych
## Find it at: http://www.oddbeat.de

## For further info see this thread.
##     http://www.ogre3d.org/phpBB2/viewtopic.php?t=30955&postdays=0&postorder=asc&start=0

## DEMO/MEDIA:
## ===========
##  This demo and the media directory ../media/ET are based entirely on the  ETSample
##  supplied with the EDITABLE TERRAIN MANAGER and are released under the same license 
##  as the ETM (see LICENSE).

## LICENSE:
## ========
## ETM is licensed under the GPLv2 with runtime exception. This means that you are free to use the library in both open and closed source, commercial and non-commercial applications of yours free of charge or other obligations. You can link dynamically or statically to it, whichever fits your taste. Only if you make changes to the ETM library itself, then you must release these changes under the terms of the GPL to comply to the license.

## For your legal concerns, here is a copy of the runtime exception:

## ================ RUNTIME EXCEPTION ================================
##   As a special exception, you may use this file as part of a free software
##   library without restriction.  Specifically, if other files instantiate
##   templates or use macros or inline functions from this file, or you compile
##   this file and link it with other files to produce an executable, this
##   file does not by itself cause the resulting executable to be covered by
##   the GNU General Public License.  This exception does not however
##   invalidate any other reasons why the executable file might be covered by
##   the GNU General Public License.
## ===================================================================


## TODO
##
##    Brush
##    ----------------
##      1) Replace float* with List in ctor 
##         Brush(const float* brush, size_t width, size_t height);
##      2) Return float& as a List (?)
##         float& at(size_t x, size_t y)
##
##    TerrainInfo
##    ----------------
##
##      1) To Test:
##         loadHeightmapFromRawData(TerrainInfo& info, Ogre::DataStream& stream,
##         size_t width, size_t height);
##      2) To wrap/remap std::ostream& stream
##         void _ETManagerExport saveHeightmapToRawData(const TerrainInfo& info,
##                                        std::ostream& stream, unsigned int bpp = 2);
##
##      3) Replace float* with List in ctor 
##         TerrainInfo(size_t width, size_t height, const float* heightmap = 0);
##
##      4) Replace float* with List 
##         void setHeightmap(size_t width, size_t height, const float* heightmap = 0);
##
##    SplattingManager
##    ----------------
##
##      1) typedef std::vector<Ogre::ColourValue> ColourList; - can't seem to get this
##         to work no matter what I add to aliases/sizeof
##         
##         createColourMap(Ogre::Image& image, const ColourList& colours);
##
##         Updated to pass in list of colours - To Test
##
##    Demo
##    ----
##     Problems with max textures 5 on opengl on ogre 1.4.2 and prior
##     Workaround comment out the 6th Material in ../media/ET/ETTerrain.material. The
##     demo will currently "paint" black if OIS.KC_6 is selected. On linux including
##     the above material leads to some garbled displays. 
##
##      ETTerrain.material
##      //{
##      //  texture splatting5.png
##      //}
##
##
##    TODO
##    ----
##      add Menus
##      add proper brushes
##      add libnoise capabilities
import sys
sys.path.insert(0,'..')
import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import ogre.addons.et as ET
import ogre.io.OIS as OIS
import ogre.gui.CEGUI as CEGUI
import SampleFramework
import exceptions, random, os
from CEGUI_framework import *   ## we need the OIS version of the framelistener etc

### You may have to include this, it causes a problem for me with 
### the way I load CEGUI 
### -
##import os


MEDIADIR    = "../../media"


class CEGUIFrameListener(SampleFramework.FrameListener , OIS.KeyListener, OIS.MouseListener):
    """A default frame listener, which takes care of basic mouse and keyboard
    input."""

    def __init__(self, renderWindow, camera, sceneManager, guiRenderer, terrainMgr, splatMgr):
        SampleFramework.FrameListener.__init__(self, renderWindow, camera, True, True, True)
        global MEDIADIR
        self.terrainMgr = terrainMgr
        self.splatMgr   = splatMgr
        self.terrainInfo = self.terrainMgr.getTerrainInfo()
 

        OIS.KeyListener.__init__(self)
        OIS.MouseListener.__init__(self)
        self.Mouse.setEventCallback(self)
        self.Keyboard.setEventCallback(self)

        self.ShutdownRequested = False
        self.GUIRenderer = guiRenderer
        self.keepRendering = True   # whether to continue rendering or not
        self.numScreenShots = 0     # screen shot count


        # Setup default variables
        self.mPointer = None
        self.mLMouseDown = False
        self.mRMouseDown = False
        self.mMMouseDown = False
        self.mSceneMgr = sceneManager
        self.camera = camera

        # Reduce move speed
        self.mMoveSpeed = 320.0
        #self.mRotateSpeed = ogre.Degree(36)*0.008
        self.mRotateSpeed = 8.0
        self.mDirection = ogre.Vector3.ZERO

        ## Create RaySceneQuery
        self.mRaySceneQuery = self.mSceneMgr.createRayQuery( ogre.Ray() )

        ## Create a "pointer" for use on the terrain
        pointer = self.mSceneMgr.createEntity("Pointer", "ogrehead.mesh")
        self.mPointer = self.mSceneMgr.getRootSceneNode().createChildSceneNode()
        self.mPointer.attachObject(pointer)

        ## initialise CEGUI mouse cursor position
        CEGUI.MouseCursor.getSingleton().setPosition(CEGUI.Point(0,0))

        ## ETM brush
        self.brushSize = 16
        self.mEditBrush = None
        self.mDeform = True
        self.mChosenTexture = 0
        self.createEditBrush()


        ## camera
        self.mCamNode = camera.getParentSceneNode()
        self.camera   = camera

        ## minimap - works ok but causes problems with lightmap
        #self.showMiniMap(True)

        self.mediadir = MEDIADIR

    ## ------------------------------------------------------------------

    def __del__(self):
        del self.mEditBrush 
        del self.mRaySceneQuery
        SampleFramework.FrameListener.__del__(self)

    ## ------------------------------------------------------------------

    def showMiniMap(self, show):
        """Turns the mini  on or off."""
        overlay2 = ogre.OverlayManager.getSingleton().getByName('MiniMap/MainOverlay')
        if overlay2 is None:
            errDesc = "Could not find overlay MiniMap/MainOverlay"
            errFunc = "CEGUIFrameListener::ShowMiniMap"
            #raise ogre.Exception(111, errDesc , errFunc)
        else:
            if show:
                overlay2.show()
            else:
                overlay2.hide()

            ogre.OverlayManager.getSingleton().getOverlayElement('MiniMap/MapPanel').setMaterialName ("ETTerrainMaterial")


    ##----------------------------------------------------------------##

    def getTerrainInfo( self):
        self.terrainInfo = self.terrainMgr.getTerrainInfo()
        return self.terrainInfo

    ##----------------------------------------------------------------##

    def keyPressed( self, arg ):
        if arg.key == OIS.KC_ESCAPE:
            self.ShutdownRequested = True
            #ogre.OverlayManager.getSingleton().destroyOverlayElement ('MiniMap/MapPanel')
            #ogre.OverlayManager.getSingleton().destroy('MiniMap/MainOverlay')
        if arg.key == OIS.KC_1:
            self.mDeform = False
            self.mChosenTexture = 0
            return True
        if arg.key == OIS.KC_2:
            self.mDeform = False
            self.mChosenTexture = 1
            return True
        if arg.key == OIS.KC_3:
            self.mDeform = False
            self.mChosenTexture = 2
            return True
        if arg.key == OIS.KC_4:
            self.mDeform = False
            self.mChosenTexture = 3
            return True
        if arg.key == OIS.KC_5:
            self.mDeform = False
            self.mChosenTexture = 4
            return True
        if arg.key == OIS.KC_6:
            self.mDeform = False
            self.mChosenTexture = 5
            return True
        if arg.key == OIS.KC_E:
            self.mDeform = True
            return True
        if arg.key == OIS.KC_W:
            self.mDirection.z+= -1
            return True
        if arg.key == OIS.KC_S:
            self.mDirection.z+= 1
            return True
        if arg.key == OIS.KC_A:
            self.mDirection.x+= -1
            return True
        if arg.key == OIS.KC_D:
            self.mDirection.x+= 1
            return True
        if arg.key == OIS.KC_O:
            self.saveTerrain()
            return True
        if arg.key == OIS.KC_P:
            self.loadTerrain()
            return True
        if arg.key == OIS.KC_M:
            self.showMiniMap(False)
            return True
        if arg.key == OIS.KC_N:
            self.showMiniMap(True)
            return True
        if arg.key == OIS.KC_F1:
            self.decreaseBrushSize()
            return True
        if arg.key == OIS.KC_F2:
            self.increaseBrushSize()
            return True

        return True

    ##----------------------------------------------------------------##

    def keyReleased( self, arg ):
        ## not interested in this
        if arg.key == OIS.KC_W:
            self.mDirection.z-= -1
            return True
        if arg.key == OIS.KC_S:
            self.mDirection.z-= 1
            return True
        if arg.key == OIS.KC_A:
            self.mDirection.x-= -1
            return True
        if arg.key == OIS.KC_D:
            self.mDirection.x-= 1
            return True
        return False

    ## ------------------------------------------------------------------

    def mouseMoved( self, arg):
        ## HACK
        self.terrainInfo = self.getTerrainInfo()

        ## Update CEGUI with the mouse motion
        CEGUI.System.getSingleton().injectMouseMove( arg.get_state().X.rel, arg.get_state().Y.rel )

        ## whenever the mouse is moved, we update the position on the
        ## terrain to which the mouse is currently pointing

        pos_w = float(arg.get_state().X.abs) / float(arg.get_state().width)
        pos_h = float(arg.get_state().Y.abs) / float(arg.get_state().height)
        mouseRay = self.camera.getCameraToViewportRay(pos_w, pos_h)


        ## since ETM is no longer a scene manager, we don't use a ray scene query, 
        ## but instead query the terrain info directly. The result is a 
        ## std::pair<bool, Vector3>  but we dont wrap directly instead we return
        ## a python list:
        ##   [intersects, (x,y,z) ]
        ##      result[0] : bool intersects 
        ##      result[1] : float tuple (x,y,z) intersection point
        ##

        result = self.terrainInfo.rayIntersects(mouseRay)
        intersects = result[0]
        ## update pointer's position
        if (intersects):
            x = result[1][0]
            y = result[1][1]
            z = result[1][2]
            ## Application.debugText("Intersect %f, %f, %f " % ( x, y, z) )
            self.mPointer.setPosition(ogre.Vector3(x, y, z))

        ## If we are dragging the middle mouse button.
        if ( self.mMMouseDown ):
            self.camera.yaw(   ogre.Degree(-arg.get_state().X.rel * self.mRotateSpeed) )
            self.camera.pitch( ogre.Degree(-arg.get_state().Y.rel * self.mRotateSpeed) )

        return True

    ## ------------------------------------------------------------------
    def mousePressed( self, arg, id ):

        ## Left mouse button down
        if ( id == OIS.MB_Left ):
            self.mLMouseDown = True
        ## Middle mouse button down
        elif ( id == OIS.MB_Middle ):
            CEGUI.MouseCursor.getSingleton().hide( )
            self.mMMouseDown = True
        elif (id == OIS.MB_Right):
            self.mRMouseDown = True
        return True

    ## ------------------------------------------------------------------
    def mouseReleased( self, arg, id ):

        ## Left mouse button down
        if ( id == OIS.MB_Left ):
            self.mLMouseDown = False
            ## after completed deformation steps, recalculate the lightmap
            if (self.mDeform):
                self.updateLightmap()

        ## Middle mouse button down
        elif ( id == OIS.MB_Middle ):
            CEGUI.MouseCursor.getSingleton().show( )
            self.mMMouseDown = False

        elif (id == OIS.MB_Right):
            self.mRMouseDown = False
            if (self.mDeform):
                self.updateLightmap()
        return True

    ##----------------------------------------------------------------##
    def requestShutdown( self ):
        ## tell the frame listener to exit at the end of the next frame
        self.ShutdownRequested = True
  
    def frameEnded(self, evt):
        if self.ShutdownRequested:
            return False
        else:
            return SampleFramework.FrameListener.frameEnded(self, evt)
 
## ------------------------------------------------------------------
## CVS HEAD
## ------------------------------------------------------------------
##    def frameRenderingQueued(self, evt):
##
##        ## call parent class it may cause problems with rotations etc
##        if not SampleFramework.FrameListener.frameRenderingQueued(self,evt):
##            return False
##        return self.processFrame(evt)
## ------------------------------------------------------------------
## OGRE 1.4
## ------------------------------------------------------------------
    def frameStarted(self, evt):

        ## call parent class it may cause problems with rotations etc
        if not SampleFramework.FrameListener.frameStarted(self,evt):
            return False
        return self.processFrame(evt)

    ## ------------------------------------------------------------------
    def processFrame(self, evt):

        
        ## OR don't call parent class to resolve above problems with rotations etc

        #if(self.renderWindow.isClosed()):
        #    return False
        
        ##Need to capture/update each device - this will also trigger any listeners
        #self.Keyboard.capture()    
        #self.Mouse.capture()
        #if( self.Joy ):
        #    self.Joy.capture()

        ## hack
        self.terrainInfo = self.getTerrainInfo()

        ## handle movement
        self.mCamNode.translate( self.camera.getOrientation()* self.mDirection*\
                                 self.mMoveSpeed* evt.timeSinceLastFrame)

        ## Ensure we stay above terrain (somehow buggy still, doesn't work reliably)
        camPos = self.mCamNode.getPosition( )

        ## get terrain height at camPos
        terrainHeight = float( self.terrainInfo.getHeightAt(camPos.x, camPos.z) )
        if ((terrainHeight + 30.0 ) > camPos.y):
            self.mCamNode.setPosition(camPos.x, terrainHeight + 30.0, camPos.z)

        ## deform or paint terrain on mouse down
        ## left button (raises) right button (lowers)
        if (self.mLMouseDown or self.mRMouseDown):

            ## for calculating brush intensity
            dr = -1
            if self.mLMouseDown:
                dr = 1

            ## translate our cursor position to vertex indexes
            pos = self.mPointer.getPosition()
            x = int( self.terrainInfo.posToVertexX(pos.x) )
            z = int( self.terrainInfo.posToVertexZ(pos.z) )

            if (self.mDeform):

                ## choose a brush intensity, this determines
                ## how extreme our brush works on the terrain
                ## and tell the ETM to deform the terrain
                brushIntensity = float(evt.timeSinceLastFrame * 0.4 * dr)
                self.terrainMgr.deform(x, z, self.mEditBrush, brushIntensity)

            else:
                ## need to set our brush intensity larger for painting.
                ## for painting, all texture channels are only 1 byte
                ## large, so with a small intensity you won't get any
                ## effect at all.
                ## And use the splatting manager to update the coverage maps
                brushIntensity = float(evt.timeSinceLastFrame * 5.0 * dr )
                self.splatMgr.paint(self.mChosenTexture, x, z, self.mEditBrush, brushIntensity)

        return self.keepRendering


    ## ------------------------------------------------------------------
    def updateBrush(self):
        if not self.mEditBrush is None:
            self.mEditBrush = None
        image = ogre.Image()
        image.load("brush.png", "ET")
        image.resize(self.brushSize, self.brushSize)
        self.mEditBrush = ET.loadBrushFromImage(image)

    ## ------------------------------------------------------------------
    def createEditBrush(self):
        ## load the edit brush for terrain editing
        self.updateBrush()

    ## ------------------------------------------------------------------
    def decreaseBrushSize(self):
        self.brushSize-=4
        if self.brushSize<4: 
            self.brushSize=4
        self.updateBrush()

    ## ------------------------------------------------------------------
    def increaseBrushSize(self):
        self.brushSize+=4
        if self.brushSize>64: 
            self.brushSize=64
        self.updateBrush()

    ## ------------------------------------------------------------------
    def loadTerrain(self):

        ## destroy current terrain
        self.terrainMgr.destroyTerrain()

        ## load terrain map
        image = ogre.Image()
        image.load("ETterrain.png", "ET")
        info = ET.TerrainInfo()
        ET.loadHeightmapFromImage(info, image)
        info.setExtents(ogre.AxisAlignedBox(0, 0, 0, 1500, 300, 1500))
        self.terrainMgr.createTerrain(info)
        self.terrainInfo = self.terrainMgr.getTerrainInfo()

        ## load splatting maps
        for i in xrange(self.splatMgr.getNumMaps()):
            filename = "ETcoverage.%s.%s" %( str(i), "png" )
            print "Loading Splatting Map %s " %filename
            image.load( filename , "ET")
            self.splatMgr.loadMapFromImage(i, image)

        ## update lightmap
        self.updateLightmap()

    ## ------------------------------------------------------------------
    def saveTerrain(self):

        ### There is a bug somewhere in the code, I should be able to use
        ### self.terrainInfo but the data isn't there  
        ## check data array
        ##for j in xrange(self.terrainInfo.getHeight()):
        ##    for i in xrange(self.terrainInfo.getWidth()):
        ##        if self.terrainInfo.at(i, j)>0.5:
        ##             print self.terrainInfo.at(i, j)


        ## HACK
        self.terrainInfo = self.getTerrainInfo()

        ## save terrain map 
        image = ogre.Image()
        ET.saveHeightmapToImage(self.terrainInfo, image)
        image.save( os.path.join(self.mediadir, "ET/ETterrain.png") )

        ## save the splatting maps
        for i in xrange(self.splatMgr.getNumMaps()):
            self.splatMgr.saveMapToImage(i, image)
            filename = os.path.join(self.mediadir, "ET/ETcoverage.%s.%s" % ( str(i) , "png")  )
            print "Saving Splatting Map %s " %filename
            image.save( filename )

        ## save light map
        lightmap = ogre.Image()
        ET.createTerrainLightmap(
                             self.terrainInfo ,
                             lightmap, 512, 512 ,
                             ogre.Vector3(1, -1, 1) ,
                             ogre.ColourValue(1 ,1, 1) ,      
                             ogre.ColourValue(1, 1,  1) )

        lightmap.save(os.path.join(self.mediadir,"ET/ETlightmap.png") )


        ## generate a base texture for this terrain (could be used for older hardware instead of splatting)
        textures = []
        for i in range(6): 
            image = ogre.Image()
            textures.append(image)
            textures[i].load("splatting%s.%s" %( str(i), "png" ) , "ET")
        ## create the base texture
        baseTexture = ogre.Image()
        self.splatMgr.createBaseTexture(baseTexture, 512, 512, textures, 20, 20)
        baseTexture.save(os.path.join(self.mediadir,"ET/ETbase.png"))

        ## finally create a minimap using the lightmap and the generated base texture
        minimap = ET.createMinimap(baseTexture, lightmap)
        print "Saving MinMap to ", os.path.join(self.mediadir,"ET/ETminimap.png")
        minimap.save(os.path.join(self.mediadir,"ET/ETminimap.png"))




    ## ------------------------------------------------------------------

    def updateLightmap(self):
        ## HACK
        self.terrainInfo = self.getTerrainInfo()

        lightmap = ogre.Image()
        ET.createTerrainLightmap(
                             self.terrainInfo ,
                             lightmap, 128, 128 ,
                             ogre.Vector3(1, -1, 1) ,
                             ogre.ColourValue(1 ,1, 1) ,      
                             ogre.ColourValue(0.3, 0.3,  0.3) )

        ## get our dynamic texture and update its contents
        tex = ogre.TextureManager.getSingleton().getByName("ETLightmap")
        l = lightmap.getPixelBox(0, 0)
        tex.getBuffer(0, 0).blitFromMemory(lightmap.getPixelBox(0, 0))


#class Brush():
#    def __init__(self):
#         pass 

class GuiApplication ( SampleFramework.Application ):
   
    def __init__(self):
        SampleFramework.Application.__init__(self)
        self.GUIRenderer=0
        self.GUIsystem =0
        self.EditorGuiSheet=0
        self.terrainMgr = None
        self.terrainInfo = None
        self.terrainMgr = None
        self.splatMgr = None

        
    def __del__(self):
        ##
        ## important that things get deleted int he right order
        ##
        if not self.terrainMgr is None:
            del self.terrainInfo
            del self.splatMgr
            del self.terrainMgr

        del self.camera
        del self.sceneManager
        del self.frameListener
        try:
            if self.EditorGuiSheet:
                CEGUI.WindowManager.getSingleton().destroyWindow(self.EditorGuiSheet) 
        except:
            pass
        del self.GUIsystem
        del self.GUIRenderer
        del self.root

        del self.renderWindow        
   

    ## Just override the mandatory create scene manager method
    ## ------------------------------------------------------------------
    def _createSceneManager():
         ## Create instance of the Editable Terrain Scene Manager
         self.sceneManager = self.root.createSceneManager("OctreeSceneManager", "ETInstance")

    ## Just override the mandatory create scene method
    ## ------------------------------------------------------------------
    def _createScene(self):

        self.sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        ## Create a skydome
        ## self.sceneManager.setSkyDome(True, "Examples/CloudySky", 5, 8) 

        ## create terrain manager
        self.terrainMgr =  ET.TerrainManager(self.sceneManager)
        self.terrainMgr.setLODErrorMargin(2, self.camera.getViewport().getActualHeight())
        self.terrainMgr.setUseLODMorphing(True, 0.2, "morphFactor")

        ## create a fresh, mid-high terrain for editing
        heightMapValues = ET.stdVectorFloat()
        for i in xrange(129):
            for j in xrange(129):
                heightMapValues.append(float(0.50))
        self.terrainInfo = ET.TerrainInfo (129, 129, heightMapValues )

        ## save typing self
        terrainInfo  = self.terrainInfo 
        sceneManager = self.sceneManager
        terrainMgr   = self.terrainMgr

        ## set position and size of the terrain
        terrainInfo.setExtents(ogre.AxisAlignedBox(0, 0, 0, 1500, 300, 1500))
        ## now render it
        terrainMgr.createTerrain(terrainInfo)

        ## create the splatting manager
        self.splatMgr = ET.SplattingManager("ETSplatting", "ET", 128, 128, 3)
        ## specify number of splatting textures we need to handle
        self.splatMgr.setNumTextures(6)

        ## create a manual lightmap texture
        lightmapTex = ogre.TextureManager.getSingleton().createManual(
        "ETLightmap", "ET", ogre.TEX_TYPE_2D, 128, 128, 1, ogre.PF_BYTE_RGB)

        lightmap = ogre.Image()
        ET.createTerrainLightmap(terrainInfo, lightmap, 128, 128,\
                                 ogre.Vector3(1, -1, 1),\
                                 ogre.ColourValue.White,\
                                 ogre.ColourValue(0.3, 0.3, 0.3,1.0))
        lightmapTex.getBuffer(0, 0).blitFromMemory(lightmap.getPixelBox(0, 0))

        ##  load the terrain material and assign it
        material = ogre.MaterialManager.getSingleton().getByName("ETTerrainMaterial")
        self.terrainMgr.setMaterial(material)

        ## Set camera look point
        camNode = self.sceneManager.getRootSceneNode().createChildSceneNode("CamNode")
        camNode.setPosition( 40, 300, 580 )
        camNode.attachObject(self.camera)
        self.camera.pitch( ogre.Degree(-30) )
        self.camera.yaw( ogre.Degree(-45) )

        ## CEGUI setup
        self.GUIRenderer = CEGUI.OgreCEGUIRenderer(self.renderWindow, 
            ogre.RENDER_QUEUE_OVERLAY, False, 3000, self.sceneManager) 
        self.GUIsystem = CEGUI.System(self.GUIRenderer) 

        CEGUI.Logger.getSingleton().setLoggingLevel( CEGUI.Informative ) 
        CEGUI.SchemeManager.getSingleton().loadScheme("TaharezLookSkin.scheme") 
        CEGUI.MouseCursor.getSingleton().setImage("TaharezLook", "MouseArrow")


    ## ------------------------------------------------------------------
    def _createViewports(self):
        ## Create one viewport, entire window
        vp = self.renderWindow.addViewport(self.camera)
        vp.setBackgroundColour(ogre.ColourValue(0,0,0))

        ## Alter the camera aspect ratio to match the viewport
        self.camera.setAspectRatio((vp.getActualWidth()) / (vp.getActualHeight()))


    ## Create new frame listener
    def _createFrameListener(self):
        self.frameListener = CEGUIFrameListener( self.renderWindow,\
                                                 self.camera,\
                                                 self.sceneManager,\
                                                 self.GUIRenderer,\
                                                 self.terrainMgr,\
                                                 self.splatMgr )
        self.root.addFrameListener(self.frameListener)
        self.frameListener.showDebugOverlay(True)


if __name__ == '__main__':
    try:
        ta = GuiApplication()
        ta.go()
    except ogre.OgreException, e:
        print e

    
        

