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
import sys
sys.path.insert(0,'..')
import PythonOgreConfig
import ogre.renderer.OGRE as ogre
import ogre.io.OIS as ois
import sdk_framework as sf
import ogre.renderer.ogrepaging as ogrepaging
import ogre.renderer.ogreterrain as ogreterrain
import math as Math
import ctypes as ctypes
import random as random

PAGING = False
#PAGING = True
SHADOWS_IN_LOW_LOD_MATERIAL = False
TERRAIN_PAGE_MIN_X= 0
TERRAIN_PAGE_MIN_Y= 0
TERRAIN_PAGE_MAX_X= 0
TERRAIN_PAGE_MAX_Y= 0

TERRAIN_FILE_PREFIX ="testTerrain"
TERRAIN_FILE_SUFFIX ="dat"
TERRAIN_WORLD_SIZE =12000
TERRAIN_SIZE=513

MODE_NORMAL = 0
MODE_EDIT_HEIGHT = 1
MODE_EDIT_BLEND = 2
MODE_COUNT = 3
SHADOWS_NONE = 0
SHADOWS_COLOUR = 1
SHADOWS_DEPTH = 2
SHADOWS_COUNT = 3

def Clamp ( val, low, high ):
    if val < low: return low
    if val > high: return high
    return val
    
if PAGING:
    #/ This class just pretends to provide prcedural page content to avoid page loading
    class DummyPageProvider ( ogrepaging.PageProvider):
        def __init__  (self):
            ogrepaging.PageProvider.__init__(self)
        def prepareProceduralPage(self, page, section):
            return True
        def loadProceduralPage(self, page,  section):
            return True
        def unloadProceduralPage(self, page, section):
            return True
        def unprepareProceduralPage(self, page, section):
            return True

    mDummyPageProvider= DummyPageProvider()

class sample (sf.sample):
    def __init__ ( self ):
        sf.sample.__init__(self)

        self.mTerrainGroup= 0
        self.mTerrainPaging=0
        self.mPageManager= 0
        self.mFly=False
        self.mFallVelocity=0
        self.mMode= MODE_NORMAL
        self.mLayerEdit=1
        self.mBrushSizeTerrainSpace=0.02
        self.mHeightUpdateCountDown=0
        self.mTerrainPos = ogre.Vector3(1000,0,5000)
        self.mTerrainsImported= False

        self.mInfo["Title"] = "Terrain"
        self.mInfo["Description"] = "Demonstrates use of the terrain rendering plugin."
        self.mInfo["Thumbnail"] = "thumb_terrain.png"
        self.mInfo["Category"] = "Environment"
        self.mInfo["Help"] = "Left click and drag anywhere in the scene to look around. Let go again to show \
            cursor and access widgets. Use WASD keys to move. Use +/- keys when in edit mode to change content."

        # Update terrain at max 20fps
        self.mHeightUpdateRate = 1.0 / 20.0


    def testCapabilities(self, caps):
        if not caps.hasCapability(ogre.RSC_VERTEX_PROGRAM) or not caps.hasCapability(ogre.RSC_FRAGMENT_PROGRAM):
            ogre.OGRE_EXCEPT(ogre.Exception.ERR_NOT_IMPLEMENTED, "Your graphics card does not support vertex or fragment shaders, "
                        "so you cannot run this sample. Sorry!", "Sample_Terrain.testCapabilities")

    def getRequiredPlugins(self):
        return ogre.StringVector()

    def doTerrainModify(self,  terrain, centrepos, timeElapsed):
        tsPos = ogre.Vector3()
        terrain.getTerrainPosition(centrepos, tsPos)  ## AJM TO CHECK !!
        if self.mKeyboard.isKeyDown(ois.KC_EQUALS) or self.mKeyboard.isKeyDown(ois.KC_MINUS):
            if self.mMode == MODE_EDIT_HEIGHT:
                # we need point coords
                terrainSize = (terrain.getSize() - 1)
                startx = long((tsPos.x - self.mBrushSizeTerrainSpace) * terrainSize)
                starty = long((tsPos.y - self.mBrushSizeTerrainSpace) * terrainSize)
                endx = long((tsPos.x + self.mBrushSizeTerrainSpace) * terrainSize)
                endy= long((tsPos.y + self.mBrushSizeTerrainSpace) * terrainSize)
                startx = max(startx, 0)
                starty = max(starty, 0)
                endx = min(endx, long(terrainSize))
                endy = min(endy, long(terrainSize))
                for y in range (starty, endy+1):
                    for   x in range (startx, endx + 1):
                        tsXdist = (float(x) / float(terrainSize)) - tsPos.x
                        tsYdist = (float(y) / float(terrainSize))  - tsPos.y
                        weight = float(Math.sqrt(tsYdist * tsYdist + tsXdist * tsXdist)) / float((0.5 * self.mBrushSizeTerrainSpace))
                        weight = min (1.0, weight)
                        weight = 1.0 - (weight * weight)
                        addedHeight = weight * 250.0 * timeElapsed
                        if self.mKeyboard.isKeyDown(ois.KC_EQUALS):
                            newheight = terrain.getHeightAtPoint(x, y) + addedHeight
                        else :
                            newheight = terrain.getHeightAtPoint(x, y) - addedHeight
                        terrain.setHeightAtPoint(x, y, newheight)
                if self.mHeightUpdateCountDown == 0:
                    self.mHeightUpdateCountDown = self.mHeightUpdateRate
            elif self.mMode == MODE_EDIT_BLEND:
                    layer = terrain.getLayerBlendMap(self.mLayerEdit)
                    # we need image coords
                    imgSize = terrain.getLayerBlendMapSize()
                    startx = long((tsPos.x - self.mBrushSizeTerrainSpace) * imgSize)
                    starty = long((tsPos.y - self.mBrushSizeTerrainSpace) * imgSize)
                    endx = long((tsPos.x + self.mBrushSizeTerrainSpace) * imgSize)
                    endy= long((tsPos.y + self.mBrushSizeTerrainSpace) * imgSize)
                    startx = max(startx, 0)
                    starty = max(starty, 0)
                    endx = min(endx, imgSize)
                    endy = min(endy, imgSize)
                    for y in range(starty,endy+1):
                        for x in range(startx,endx+1) :
                            tsXdist = (float(x) / float(imgSize)) - tsPos.x
                            tsYdist = (float(y) / float(imgSize))  - tsPos.y

                            weight = min(1.0,
                                float(Math.sqrt(tsYdist * tsYdist + tsXdist * tsXdist)) / float(0.5 * self.mBrushSizeTerrainSpace))
                            weight = 1.0 - (weight * weight)
                            paint = weight * timeElapsed
                            imgY = imgSize - y
                            if self.mKeyboard.isKeyDown(ois.KC_EQUALS):
                                val = layer.getBlendValue(x, imgY) + paint
                            else:
                                val = layer.getBlendValue(x, imgY) - paint
                            val = Clamp(val, 0.0, 1.0)
                            layer.setBlendValue(x, imgY, val)
                    layer.update()

    def frameRenderingQueued(self,  evt):
        if self.mMode != MODE_NORMAL:
            # fire ray
            ray = self.mTrayMgr.getCursorRay(self.mCamera)

            rayResult = self.mTerrainGroup.rayIntersects(ray)
            if rayResult.hit:
                self.mEditMarker.setVisible(True)
                self.mEditNode.setPosition(rayResult.position)

                # figure out which terrains this affects
                brushSizeWorldSpace = TERRAIN_WORLD_SIZE * self.mBrushSizeTerrainSpace
                sphere = ogre.Sphere(rayResult.position, brushSizeWorldSpace)
                terrainList = self.mTerrainGroup.sphereIntersects(sphere)
                for ti in terrainList:
                    self.doTerrainModify(ti, rayResult.position, evt.timeSinceLastFrame)
            else :
                self.mEditMarker.setVisible(False)

        if not self.mFly:
            # clamp to terrain
            camPos = self.mCamera.getPosition()
            ray = ogre.Ray()
            ray.setOrigin(ogre.Vector3(camPos.x, self.mTerrainPos.y + 10000, camPos.z))
            ray.setDirection(ogre.Vector3().NEGATIVE_UNIT_Y)

            rayResult = self.mTerrainGroup.rayIntersects(ray)
            distanceAboveTerrain = 50
            fallSpeed = 300
            newy = camPos.y
            if rayResult.hit:
                if camPos.y > rayResult.position.y + distanceAboveTerrain:
                    self.mFallVelocity += evt.timeSinceLastFrame * 20
                    self.mFallVelocity = min(self.mFallVelocity, fallSpeed)
                    newy = camPos.y - self.mFallVelocity * evt.timeSinceLastFrame
                newy = max(rayResult.position.y + distanceAboveTerrain, newy)
                self.mCamera.setPosition(camPos.x, newy, camPos.z)


        if self.mHeightUpdateCountDown > 0 :
            self.mHeightUpdateCountDown -= evt.timeSinceLastFrame
            if self.mHeightUpdateCountDown <= 0:
                self.mTerrainGroup.update()
                self.mHeightUpdateCountDown = 0

        if self.mTerrainGroup.isDerivedDataUpdateInProgress():
            self.mTrayMgr.moveWidgetToTray(self.mInfoLabel, sf.TL_TOP, 0)
            self.mInfoLabel.show()
            if self.mTerrainsImported:
                self.mInfoLabel.setCaption("Building terrain, please wait...")
            else:
                self.mInfoLabel.setCaption("Updating textures, patience...")
        else :
            self.mTrayMgr.removeWidgetFromTray(self.mInfoLabel)
            self.mInfoLabel.hide()
            if self.mTerrainsImported:
                self.saveTerrains(True)
                self.mTerrainsImported = False
        return sf.sample.frameRenderingQueued(self, evt)

    def saveTerrains(self, onlyIfModified):
        self.mTerrainGroup.saveAllTerrains(onlyIfModified)

    def keyPressed (self, e) :
        key = e.key
        if key == ois.KC_S:
            # CTRL-S to save
            if self.mKeyboard.isKeyDown(ois.KC_LCONTROL) or self.mKeyboard.isKeyDown(ois.KC_RCONTROL):
                self.saveTerrains(True)
            else:
                return sf.sample.keyPressed(self, e)
#       elif key == ois.KC_F10:
            # dump
#               TerrainGroup.TerrainIterator ti = self.mTerrainGroup.getTerrainIterator()
#               while (ti.hasMoreElements())
#               {
#                   Ogre.uint32 tkey = ti.peekNextKey()
#                   TerrainGroup.TerrainSlot* ts = ti.getNext()
#                   if (ts.instance && ts.instance.isLoaded())
#                       ts.instance._dumpTextures("terrain_" + StringConverter.toString(tkey), ".png")
        else:
            return sf.sample.keyPressed(self, e)

    def itemSelected(self,  menu):
        if (menu.getName() == 'EditMode'):
            self.mMode = self.mEditMenu.getSelectionIndex()
        elif (menu.getName() == 'Shadows'):
            self.mShadowMode = self.mShadowsMenu.getSelectionIndex()
            self.changeShadows()

    def checkBoxToggled(self, box):
        if (box.getName() == 'Fly'):
            self.mFly = self.mFlyBox.isChecked()


#   TerrainGlobalOptions* self.mTerrainGlobals
#   TerrainGroup* self.mTerrainGroup
#   bool mPaging
#   TerrainPaging* self.mTerrainPaging
#   PageManager* self.mPageManager

# endif
#   bool self.mFly
#   Real self.mFallVelocity
#   enum Mode
#   {
#   }
#   Mode self.mMode
#   ShadowMode self.mShadowMode
#   Ogre.uint8 self.mLayerEdit
#   Real self.mBrushSizeTerrainSpace
#   SceneNode* self.mEditNode
#   Entity* self.mEditMarker
#   Real self.mHeightUpdateCountDown
#   Real self.mHeightUpdateRate
#   Vector3 self.mTerrainPos
#   SelectMenu* self.mEditMenu
#   SelectMenu* self.mShadowsMenu
#   CheckBox* self.self.mFlyBox
#   OgreBites.Label* self.mInfoLabel
#   bool self.mTerrainsImported
#   ShadowCameraSetupPtr self.mPSSMSetup
#
#   typedef list<Entity*> EntityList
#   EntityList mHouseList
#


    def defineTerrain(self,  x, y, flat = False):
        # if a file is available, use it
        # if not, generate file from import

        # Usually in a real project you'll know whether the compact terrain data is
        # available or not I'm doing it this way to save distribution size
        if (flat):
            self.mTerrainGroup.defineTerrain(x, y, 0.0)
        else:
            filename = self.mTerrainGroup.generateFilename(x, y)
            
            if (ogre.ResourceGroupManager.getSingleton().resourceExists(self.mTerrainGroup.getResourceGroup(), filename)):
                self.mTerrainGroup.defineTerrain(x, y)
            else:
                img = ogre.Image()
                b1 = False
                if x % 2 != 0:
                    b1 = True
                b2 = False
                if x % 2 != 0:
                    b2 = True
                self.getTerrainImage(b1, b2, img)
                self.mTerrainGroup.defineTerrain(x, y, img)
                self.mTerrainsImported = True

    def getTerrainImage(self, flipX, flipY, img):
        img.load("terrain.png", ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME)
        if (flipX):
            img.flipAroundY()
        if (flipY):
            img.flipAroundX()

    def initBlendMaps(self, terrain):
        blendMap0 = terrain.getLayerBlendMap(1)
        blendMap1 = terrain.getLayerBlendMap(2)
        minHeight0 = 70
        fadeDist0 = 40.0
        minHeight1 = 70
        fadeDist1 = 15.0

        pBlend1 = blendMap1.getBlendPointer()    # returns the address of the buffer
        size = terrain.getLayerBlendMapSize() * terrain.getLayerBlendMapSize()
        blend_data=(ctypes.c_float * size).from_address(pBlend1)
        index = 0
        for y in range(terrain.getLayerBlendMapSize()):
            for x in range( terrain.getLayerBlendMapSize() ):
                # using ctypes
                tx=ctypes.c_float(0.0)
                ty=ctypes.c_float(0.0)
                blendMap0.convertImageToTerrainSpace(x, y, ctypes.addressof(tx), ctypes.addressof(ty))
                height = terrain.getHeightAtTerrainPosition(tx.value, ty.value)
                val = (height - minHeight0) / fadeDist0
                val = Clamp(val, 0, 1)
                #*pBlend0++ = val

                val = (height - minHeight1) / fadeDist1
                val = Clamp(val, 0, 1)
                blend_data [index] = val
                index += 1
        blendMap0.dirty()
        blendMap1.dirty()
        #blendMap0.loadImage("blendmap1.png", ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME)
        blendMap0.update()
        blendMap1.update()

        # set up a colour map
#       /*
#       if (!terrain.getGlobalColourMapEnabled())
#       {
#           terrain.setGlobalColourMapEnabled(True)
#           Image colourMap
#           colourMap.load("testcolourmap.jpg", ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME)
#           terrain.getGlobalColourMap().loadImage(colourMap)
#       }
#       */


    def configureTerrainDefaults(self, l):
        # Configure global
        self.mTerrainGlobals.setMaxPixelError(8)
        # testing composite map
        self.mTerrainGlobals.setCompositeMapDistance(3000)
        #self.mTerrainGlobals.setUseRayBoxDistanceCalculation(True)
        #self.mTerrainGlobals.getDefaultMaterialGenerator().setDebugLevel(1)
        #self.mTerrainGlobals.setLightMapSize(256)

        #matProfile.setLightmapEnabled(False)
        # Important to set these so that the terrain knows what to use for derived (non-realtime) data
        self.mTerrainGlobals.setLightMapDirection(l.getDerivedDirection())
        self.mTerrainGlobals.setCompositeMapAmbient(self.mSceneMgr.getAmbientLight())
        #self.mTerrainGlobals.setCompositeMapAmbient(ColourValue.Red)
        self.mTerrainGlobals.setCompositeMapDiffuse(l.getDiffuseColour())

        # Configure default import settings for if we use imported image
        defaultimp = self.mTerrainGroup.getDefaultImportSettings()
        defaultimp.terrainSize = TERRAIN_SIZE
        defaultimp.worldSize = TERRAIN_WORLD_SIZE
        defaultimp.inputScale = 600
        defaultimp.minBatchSize = 33
        defaultimp.maxBatchSize = 65
        # textures
        defaultimp.layerList.append(ogreterrain.Terrain.LayerInstance() )
        defaultimp.layerList.append(ogreterrain.Terrain.LayerInstance() )
        defaultimp.layerList.append(ogreterrain.Terrain.LayerInstance() )
        defaultimp.layerList[0].worldSize = 100
        defaultimp.layerList[0].textureNames.append("dirt_grayrocky_diffusespecular.dds")
        defaultimp.layerList[0].textureNames.append("dirt_grayrocky_normalheight.dds")
        defaultimp.layerList[1].worldSize = 30
        defaultimp.layerList[1].textureNames.append("grass_green-01_diffusespecular.dds")
        defaultimp.layerList[1].textureNames.append("grass_green-01_normalheight.dds")
        defaultimp.layerList[2].worldSize = 200
        defaultimp.layerList[2].textureNames.append("growth_weirdfungus-03_diffusespecular.dds")
        defaultimp.layerList[2].textureNames.append("growth_weirdfungus-03_normalheight.dds")


#   def addTextureDebugOverlay(self, loc, tex, i):
#       addTextureDebugOverlay(loc, tex.getName(), i)

    def addTextureDebugOverlay(self, loc, texname, size_t ):
        # Create material
        matName = "Ogre/DebugTexture" + str(i)
        debugMat = MaterialManager.getSingleton().getByName(matName)
        if (debugMat.isNull()):
            debugMat = ogre.MaterialManager.getSingleton().create(matName,
                ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME)
        p = debugMat.getTechnique(0).getPass(0)
        p.removeAllTextureUnitStates()
        p.setLightingEnabled(False)
        t = p.createTextureUnitState(texname)
        t.setTextureAddressingMode(ogre.TextureUnitState.TAM_CLAMP)

        # create template
        if ( not ogre.OverlayManager.getSingleton().hasOverlayElement("Ogre/DebugTexOverlay", True)):
            e = OverlayManager.getSingleton().createOverlayElement("Panel", "Ogre/DebugTexOverlay", True)
            e.setMetricsMode(GMM_PIXELS)
            e.setWidth(128)
            e.setHeight(128)

        # add widget
        widgetName = "DebugTex"+ str(i)
        w = self.mTrayMgr.getWidget(widgetName)
        if (not w):
            w = self.mTrayMgr.createDecorWidget(loc, widgetName, "Ogre/DebugTexOverlay")
        w.getOverlayElement().setMaterialName(matName)


    def addTextureShadowDebugOverlay(self, loc, num):
        for i in range (num):
            shadowTex = self.mSceneMgr.getShadowTexture(i)
            addTextureDebugOverlay(loc, shadowTex, i)

    def buildDepthShadowMaterial(self, textureName):
        matName = "DepthShadows/" + textureName

        ret = ogre.MaterialManager.getSingleton().getByName(matName)
        if not ret:
            baseMat = ogre.MaterialManager.getSingleton().getByName("Ogre/shadow/depth/integrated/pssm")
            ret = baseMat.clone(matName)
            p = ret.getTechnique(0).getPass(0)
            p.getTextureUnitState("diffuse").setTextureName(textureName)

            splitPoints = ogre.Vector4()
            splitPointList =self.mPSSMSetup.getSplitPoints()
            splitPoints.x = splitPointList[0]
            splitPoints.y = splitPointList[1]
            splitPoints.z = splitPointList[2]
#             TODO Andy -- Vector 4 doesn't support index assignment
#             for i in range (3):
#                 splitPoints[i] = splitPointList[i]
            p.getFragmentProgramParameters().setNamedConstant("pssmSplitPoints", splitPoints)
        return ret

    def changeShadows(self):
        bool = False
        if  self.mShadowMode != SHADOWS_NONE:
            bool = True
        self.configureShadows(bool, self.mShadowMode == SHADOWS_DEPTH)

    def configureShadows(self, enabled, depthShadows):
        #AJM Note the use of 'Pointer' here to get to the actual object. In the C++
        # code it was -> accessed
        matProfile = self.mTerrainGlobals.getDefaultMaterialGenerator().Pointer.getActiveProfile()
        matProfile.setReceiveDynamicShadowsEnabled(enabled)
        if SHADOWS_IN_LOW_LOD_MATERIAL:
            matProfile.setReceiveDynamicShadowsLowLod(True)
        else:
            matProfile.setReceiveDynamicShadowsLowLod(False)

        # Default materials
        for i in self.mHouseList:
            i.setMaterialName("Examples/TudorHouse")

        if (enabled):
            # General scene setup
            self.mSceneMgr.setShadowTechnique(ogre.SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED)
            self.mSceneMgr.setShadowFarDistance(3000)

            # 3 textures per directional light (PSSM)
            self.mSceneMgr.setShadowTextureCountPerLightType(ogre.Light.LT_DIRECTIONAL, 3)

            if (not self.mPSSMSetup):
                # shadow camera setup
                pssmSetup = ogre.PSSMShadowCameraSetup() # TODO check if we need to keep this around
                pssmSetup.setSplitPadding(self.mCamera.getNearClipDistance())
                pssmSetup.calculateSplitPoints(3, self.mCamera.getNearClipDistance(), self.mSceneMgr.getShadowFarDistance())
                pssmSetup.setOptimalAdjustFactor(0, 2)
                pssmSetup.setOptimalAdjustFactor(1, 1)
                pssmSetup.setOptimalAdjustFactor(2, 0.5)

                self.mPSSMSetup=pssmSetup

            self.mSceneMgr.setShadowCameraSetup(self.mPSSMSetup)

            if (depthShadows):
                self.mSceneMgr.setShadowTextureCount(3)
                self.mSceneMgr.setShadowTextureConfig(0, 2048, 2048, ogre.PF_FLOAT32_R)
                self.mSceneMgr.setShadowTextureConfig(1, 1024, 1024, ogre.PF_FLOAT32_R)
                self.mSceneMgr.setShadowTextureConfig(2, 1024, 1024, ogre.PF_FLOAT32_R)
                self.mSceneMgr.setShadowTextureSelfShadow(True)
                self.mSceneMgr.setShadowCasterRenderBackFaces(True)
                self.mSceneMgr.setShadowTextureCasterMaterial("PSSM/shadow_caster")

                houseMat = self.buildDepthShadowMaterial("fw12b.jpg")
                for i in self.mHouseList:
                    i.setMaterial(houseMat)
            else:
                self.mSceneMgr.setShadowTextureCount(3)
                self.mSceneMgr.setShadowTextureConfig(0, 2048, 2048, ogre.PF_X8B8G8R8)
                self.mSceneMgr.setShadowTextureConfig(1, 1024, 1024, ogre.PF_X8B8G8R8)
                self.mSceneMgr.setShadowTextureConfig(2, 1024, 1024, ogre.PF_X8B8G8R8)
                self.mSceneMgr.setShadowTextureSelfShadow(False)
                self.mSceneMgr.setShadowCasterRenderBackFaces(False)
                self.mSceneMgr.setShadowTextureCasterMaterial(ogre.StringUtil.BLANK)

            matProfile.setReceiveDynamicShadowsDepth(depthShadows)
            matProfile.setReceiveDynamicShadowsPSSM((self.mPSSMSetup))

            #addTextureShadowDebugOverlay(sf.TL_RIGHT, 3)
        else:
            self.mSceneMgr.setShadowTechnique(ogre.SHADOWTYPE_NONE)

#   /*-----------------------------------------------------------------------------
#   | Extends setupView to change some initial camera settings for this sample.
#   -----------------------------------------------------------------------------*/
    def setupView(self):
        sf.sample.setupView(self)

#         self.mCamera.setPosition(self.mTerrainPos + ogre.Vector3(1683, 50, 2116))
        self.mCamera.setPosition(ogre.Vector3(1575,310,5890))
#         self.mCamera.lookAt(ogre.Vector3(1963, 50, 1660))
        self.mCamera.lookAt(ogre.Vector3(2330,206,6346))
        self.mCamera.setNearClipDistance(0.1)
        self.mCamera.setFarClipDistance(50000)

        if (self.mRoot.getRenderSystem().getCapabilities().hasCapability(ogre.RSC_INFINITE_FAR_PLANE)):
            self.mCamera.setFarClipDistance(0)   # enable infinite far clip distance if we can

    def setupControls(self):
        self.mTrayMgr.showCursor()

        # make room for the controls
        self.mTrayMgr.showLogo(sf.TL_TOPRIGHT)
        self.mTrayMgr.showFrameStats(sf.TL_TOPRIGHT)
        self.mTrayMgr.toggleAdvancedFrameStats()

        self.mInfoLabel = self.mTrayMgr.createLabel(sf.TL_TOP, "TInfo", "", 350)

        self.mEditMenu = self.mTrayMgr.createLongSelectMenu(sf.TL_BOTTOM, "EditMode", "Edit Mode", 370, 250, 3)
        self.mEditMenu.addItem("None")
        self.mEditMenu.addItem("Elevation")
        self.mEditMenu.addItem("Blend")
        self.mEditMenu.selectItem(0)  # no edit mode

        self.mFlyBox = self.mTrayMgr.createCheckBox(sf.TL_BOTTOM, "Fly", "Fly")
        self.mFlyBox.setChecked(False, False)

        self.mShadowsMenu = self.mTrayMgr.createLongSelectMenu(sf.TL_BOTTOM, "Shadows", "Shadows", 370, 250, 3)
        self.mShadowsMenu.addItem("None")
        self.mShadowsMenu.addItem("Colour Shadows")
        self.mShadowsMenu.addItem("Depth Shadows")
        self.mShadowsMenu.selectItem(0)  # no edit mode

        # a friendly reminder
        names = ogre.StringVector()
        names.append("Help")
        self.mTrayMgr.createParamsPanel(sf.TL_TOPLEFT, "Help", 100, names).setParamValue(0, "H/F1")

    def setupContent(self):
        self.mHouseList=[]
        self.mPSSMSetup = None

        blankTerrain = False
        #blankTerrain = True

        self.mTerrainGlobals = ogreterrain.TerrainGlobalOptions()

        self.mEditMarker = self.mSceneMgr.createEntity("editMarker", "sphere.mesh")
        self.mEditNode = self.mSceneMgr.getRootSceneNode().createChildSceneNode()
        self.mEditNode.attachObject(self.mEditMarker)
        self.mEditNode.setScale(0.05, 0.05, 0.05)

        self.setupControls()

        self.mCameraMan.setTopSpeed(50)

        self.setDragLook(True)

        ogre.MaterialManager.getSingleton().setDefaultTextureFiltering(ogre.TFO_ANISOTROPIC)
        ogre.MaterialManager.getSingleton().setDefaultAnisotropy(7)

        self.mSceneMgr.setFog(ogre.FOG_LINEAR, ogre.ColourValue(0.7, 0.7, 0.8), 0, 10000, 25000)

#         ogre.LogManager.getSingleton().setLogDetail(LL_BOREME)

        lightdir= ogre.Vector3 (0.55, -0.3, 0.75)
        lightdir.normalise()


        l = self.mSceneMgr.createLight("tstLight")
        l.setType(ogre.Light.LT_DIRECTIONAL)
        l.setDirection(lightdir)
        l.setDiffuseColour(ogre.ColourValue().White)
        l.setSpecularColour(ogre.ColourValue(0.4, 0.4, 0.4))

        self.mSceneMgr.setAmbientLight(ogre.ColourValue(0.2, 0.2, 0.2))


        self.mTerrainGroup = ogreterrain.TerrainGroup(self.mSceneMgr, ogreterrain.Terrain.ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE)
        self.mTerrainGroup.setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX)
        self.mTerrainGroup.setOrigin(self.mTerrainPos)
        self.configureTerrainDefaults(l)
        if PAGING:
            # Paging setup
            self.mPageManager = ogrepaging.PageManager()
            # Since we're not loading any pages from .page files, we need a way just
            # to say we've loaded them without them actually being loaded
            self.mPageManager.setPageProvider(mDummyPageProvider)
            self.mPageManager.addCamera(self.mCamera)
            self.mTerrainPaging = ogreterrain.TerrainPaging(self.mPageManager)
            world = self.mPageManager.createWorld()
            self.mTerrainPaging.createWorldSection(world, self.mTerrainGroup, 2000, 3000,
                TERRAIN_PAGE_MIN_X, TERRAIN_PAGE_MIN_Y,
                TERRAIN_PAGE_MAX_X, TERRAIN_PAGE_MAX_Y)
        else:
            for x in range(TERRAIN_PAGE_MIN_X, TERRAIN_PAGE_MAX_X +1 ):
                for y in range(TERRAIN_PAGE_MIN_Y, TERRAIN_PAGE_MAX_Y +1):
                    self.defineTerrain(x, y, blankTerrain)
            # sync load since we want everything in place when we start
            self.mTerrainGroup.loadAllTerrains(True)

        if (self.mTerrainsImported):
            it = self.mTerrainGroup.getTerrainIterator()
            for t in self.mTerrainGroup.getTerrainIterator():
                #t = ti.getNext().instance
                self.initBlendMaps(t.instance)

        self.mTerrainGroup.freeTemporaryResources()



        # create a few entities on the terrain
        e = self.mSceneMgr.createEntity("tudorhouse.mesh")
        entPos = ogre.Vector3 (self.mTerrainPos.x + 2043, 0, self.mTerrainPos.z + 1715)
        rot = ogre.Quaternion()
        entPos.y = self.mTerrainGroup.getHeightAtWorldPosition(entPos) + 65.5 + self.mTerrainPos.y
        rot.FromAngleAxis(ogre.Degree(random.randrange(-180, 180)), ogre.Vector3().UNIT_Y)
        sn = self.mSceneMgr.getRootSceneNode().createChildSceneNode(entPos, rot)
        sn.setScale(ogre.Vector3(0.12, 0.12, 0.12))
        sn.attachObject(e)
        self.mHouseList.append(e)

        e = self.mSceneMgr.createEntity("tudorhouse.mesh")
        entPos = ogre.Vector3(self.mTerrainPos.x + 1850, 0, self.mTerrainPos.z + 1478)
        entPos.y = self.mTerrainGroup.getHeightAtWorldPosition(entPos) + 65.5 + self.mTerrainPos.y
        rot.FromAngleAxis(ogre.Degree(random.randrange(-180, 180)), ogre.Vector3().UNIT_Y)
        sn = self.mSceneMgr.getRootSceneNode().createChildSceneNode(entPos, rot)
        sn.setScale(ogre.Vector3(0.12, 0.12, 0.12))
        sn.attachObject(e)
        self.mHouseList.append(e)

        e = self.mSceneMgr.createEntity("tudorhouse.mesh")
        entPos = ogre.Vector3(self.mTerrainPos.x + 1970, 0, self.mTerrainPos.z + 2180)
        entPos.y = self.mTerrainGroup.getHeightAtWorldPosition(entPos) + 65.5 + self.mTerrainPos.y
        rot.FromAngleAxis(ogre.Degree(random.randrange(-180, 180)), ogre.Vector3().UNIT_Y)
        sn = self.mSceneMgr.getRootSceneNode().createChildSceneNode(entPos, rot)
        sn.setScale(ogre.Vector3(0.12, 0.12, 0.12))
        sn.attachObject(e)
        self.mHouseList.append(e)

        self.mSceneMgr.setSkyBox(True, "Examples/CloudyNoonSkyBox")

    def __del__(self):
        print "IN DELETE"
        del self.mTerrainPaging
        print "IN DELETE"
        print self.mTerrainGroup
        print dir(self.mTerrainGroup)
        self.mTerrainGroup.freeTemporaryResources()
        print "Freed resources"
        i=self.mTerrainGroup.getTerrainIterator()
        print i
        print dir(i)
        while i.hasMoreElements():
            v=i.getNext()
            print v
            #del v
        slots= self.mTerrainGroup.getTerrainSlot(0,0)
        print "Got slot"
        del self.mTerrainGlobals
        print slots

        slots.freeInstance()
        print "Freeinstance"
        self.mTerrainGroup.removeAllTerrains()
        print "Unloaded"

        print "deleting"
        del self.mTerrainGroup
        print "OK"
        sys.exit()

        if (self.mTerrainPaging):
            print "IN DELETE1"
            del self.mTerrainPaging
            del self.mPageManager
            print "IN DELETE2"
        else:
            print "IN DELETE3"
            print self.mTerrainGroup
            del self.mTerrainGroup
        print "IN DELETE4"
        del self.mTerrainGlobals
        print "Leaving delete"

if __name__ == '__main__':
    con = sf.context()
    s = sample ()
    con.go(s)
