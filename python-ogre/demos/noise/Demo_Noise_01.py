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

## Note the first frame doesn't get displayed ...........

import ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ogre.io.OIS as OIS
import ogre.addons.noise as noise
import ctypes, math, random



class NoiseOgreImage(object):
    def __init__(self):

        self.noiseImage = None

        mountainTerrain = noise.RidgedMulti()
        baseFlatTerrain = noise.Billow()

        ##increase the frequency of this noise module by a factor of 2. This will generate smaller, more numerous lumps in the flat terrain
        baseFlatTerrain.SetFrequency(2.0)
        flatTerrain = noise.ScaleBias()
        flatTerrain.SetSourceModule (0, baseFlatTerrain)
        flatTerrain.SetScale(0.125)
        flatTerrain.SetBias(-0.75)

        ## define the areas that these terrains will appear in the final height map. To do this, you'll create a terrain-type map using a Perlin-noise module:
        terrainType=noise.Perlin() 
        terrainType.SetFrequency (0.5)
        terrainType.SetPersistence(0.25)

        finalTerrain = noise.Select() 
        finalTerrain.SetSourceModule(0, flatTerrain)
        finalTerrain.SetSourceModule(1, mountainTerrain)
        finalTerrain.SetControlModule(terrainType)
        finalTerrain.SetBounds(0.0, 1000.0)
        finalTerrain.SetEdgeFalloff(0.125)

        heightMap = noise.NoiseMap()

        heightMapBuilder = noise.NoiseMapBuilderPlane()
        heightMapBuilder.SetSourceModule(finalTerrain)
        heightMapBuilder.SetDestNoiseMap(heightMap)
        heightMapBuilder.SetDestSize (256, 256)
        heightMapBuilder.SetBounds (6.0, 10.0, 1.0, 5.0)
        heightMapBuilder.Build ()

        ## render image
        renderer = noise.RendererImage()
        image = noise.Image()
        renderer.SetSourceNoiseMap(heightMap)
        renderer.SetDestImage(image)
        renderer.ClearGradient ()

        renderer.AddGradientPoint (-1.00, noise.Color ( 32, 160,   0, 255))## grass
        renderer.AddGradientPoint (-0.25, noise.Color (224, 224,   0, 255))## dirt
        renderer.AddGradientPoint ( 0.25, noise.Color (128, 128, 128, 255))## rock
        renderer.AddGradientPoint ( 1.00, noise.Color (255, 255, 255, 255))## snow
        renderer.EnableLight()
        renderer.SetLightContrast(3.0)
        renderer.SetLightBrightness(2.0)
        renderer.Render()
        self.noiseImage = image

        width  = self.noiseImage.Width
        height = self.noiseImage.Height
        bufferSize = height*width*4

        ## save typing
        materialManager = ogre.MaterialManager.getSingleton()
        textureManager  = ogre.TextureManager.getSingleton()

        ## create material
        material = materialManager.create( "NoiseMaterial",
                      ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME )
        material.getTechnique(0).getPass(0).setSceneBlending(ogre.SBT_TRANSPARENT_ALPHA)

        format = ogre.PF_BYTE_BGRA

        ## create our texture
        texture = textureManager.createManual(
                             "NoiseTexture" , 
                             ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME,
                             ogre.TEX_TYPE_2D,      
                             width, height,    
                             0,                
                             format,   
                             ogre.TU_DEFAULT)

        t = material.getTechnique(0).getPass(0).createTextureUnitState("NoiseTexture")

        self.texture = texture

    def __del__(self):
        del self.noiseImage

    def updateOgreTexture(self):

        width    = self.texture.getWidth()
        height   = self.texture.getHeight()
        byteSize =  ogre.PixelUtil.getNumElemBytes(self.texture.getDesiredFormat())
        bufferSize =  width * height * byteSize

        ## sanity checks
        assert self.noiseImage.Width == width
        assert self.noiseImage.Height == height
        #assert (noiseImage.Width * noiseImage.Height * 4) == bufferSize

        # get and lock texture's pixel buffer
        pixelBuffer = self.texture.getBuffer()
        pointer = pixelBuffer.lock(0, bufferSize, ogre.HardwareBuffer.HBL_NORMAL)

        # python-ogre hack
        storageclass = ctypes.c_uint8 * ( bufferSize )
        cbuffer=storageclass.from_address(ogre.CastInt(pointer))

        ## copy image buffer to texture
        k = 0
        for i in range(height):
            for j in range(width):
                c = self.noiseImage.GetSlabPtr(i,j)
                cbuffer[k]   = c.blue
                cbuffer[k+1] = c.green
                cbuffer[k+2] = c.red
                if byteSize>3:
                    cbuffer[k+3] = c.alpha
                    k+=4
                else: 
                    k+=3


        # Unlock the pixel buffer
        pixelBuffer.unlock() 


class NoiseApplication(sf.Application):

    def __init__(self):
        "Init Noise Application"
        self.noiseExample  = None
        sf.Application.__init__(self)

    def __del__(self):
        del self.noiseExample
        sf.Application.__del__(self)


    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera
        sceneManager.ambientLight = [0.9, 0.9, 0.9]

        # Create the SkyBox
        sceneManager.setSkyBox(True, "Examples/CloudyNoonSkyBox")

        ## create simple cube
        entity = sceneManager.createEntity("cube" , "cube.mesh")
        node = sceneManager.getRootSceneNode().createChildSceneNode()
        node.attachObject(entity)
        node.setScale(2,2,2)
        node.setPosition(ogre.Vector3(20, 0, 0))

        ## our noise example
        self.noiseExample = NoiseOgreImage()
        self.noiseExample.updateOgreTexture()

        entity.setMaterialName("NoiseMaterial")

        
    def _createFrameListener(self):
        self.frameListener = NoiseListener(self.renderWindow, self.camera)
        self.root.addFrameListener(self.frameListener)

class NoiseListener(sf.FrameListener):
    def __init__(self, renderWindow, camera):
        sf.FrameListener.__init__(self, renderWindow, camera)

    def frameStarted(self, frameEvent):
        return sf.FrameListener.frameStarted(self, frameEvent)

if __name__ == '__main__':
    try:
        application = NoiseApplication()
        application.go()
    except ogre.OgreException, e:
        print e
