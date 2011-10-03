import ogre.renderer.OGRE as ogre
import ctypes
import SampleFramework as sf

class textureListener(ogre.FrameListener):
    def __init__(self, app):
        ogre.FrameListener.__init__(self)
        self.app = app
        self.grnVal = 0
        self.fading = True
        
    def frameStarted(self, frameEvent):
        self.grnVal = self.fading and self.grnVal + 1 or self.grnVal -1
        if self.grnVal == 255:
            self.fading = False
        elif self.grnVal == 0:
            self.fading = True
        colArray = [0,self.grnVal,0,128] * 16 * 16
        self.app.setTextureManual(self.app.mTex, 16,16, colArray)
        return True
        
        
class textureTest(sf.Application):
    def __init__(self):
        sf.Application.__init__(self)
        
    def _createScene(self):
        self.mTex = ogre.TextureManager.getSingleton().createManual(
                                "DynamicTexture",
                                ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME,
                                ogre.TEX_TYPE_2D,
                                16, 16,
                                0,
                                ogre.PF_BYTE_BGRA,
                                ogre.TU_DEFAULT)
        colArray = [0,255,0,128] * 16 * 16
        self.setTextureManual(self.mTex, 16, 16, colArray)
        self.material = ogre.MaterialManager.getSingleton().create(
                            "DynamicTextureMaterial",
                            ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME)
                            
        self.material.getTechnique(0).getPass(0).createTextureUnitState("DynamicTexture")
        self.material.getTechnique(0).getPass(0).setSceneBlending(ogre.SBT_TRANSPARENT_ALPHA)
        self.ent = self.sceneManager.createEntity("mrben", "knot.mesh")
        self.ent.setMaterialName("DynamicTextureMaterial")
        self.node = self.sceneManager.rootSceneNode.createChildSceneNode('Knot3Node', (-100, -10, -200))
        self.node.attachObject(self.ent)
        
        self.lstnr = textureListener(self)
        ogre.Root.getSingleton().addFrameListener(self.lstnr)
        
    #Texture is a manually created Texture 
    #dataArray is an array of pixel colour data 
    def setTextureManual(self, texture, width, height, dataArray): 
            import ctypes 
            # Get the pixel buffer 
            pixelBuffer = texture.getBuffer() 
            # Lock the pixel buffer and get a pixel box 
            pointer = pixelBuffer.lock(0,width*height,ogre.HardwareBuffer.HBL_NORMAL) 
            #number 4 here is use to accommodate the bytes for r, g, b, a may vary with different pixel formats 
            storageclass = ctypes.c_uint8 * (width*height*4) 
            cbuffer=storageclass.from_address(ogre.CastInt(pointer)) 
            pos=0 
            for j in range(height): 
                for i in range( width ) : 
                    cbuffer[pos]= dataArray[pos] # B 
                    pos+=1 
                    cbuffer[pos]= dataArray[pos]  # G 
                    pos+=1 
                    cbuffer[pos]= dataArray[pos]  # R 
                    pos+=1 
                    cbuffer[pos]= dataArray[pos]   # A 
                    pos+=1 
            # Unlock the pixel buffer 
            pixelBuffer.unlock()
            
if __name__=='__main__':
    ta = textureTest()
    ta.go()