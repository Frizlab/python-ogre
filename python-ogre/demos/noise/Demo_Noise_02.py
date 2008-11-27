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
import ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ogre.addons.noise as noise
 
 
def sinFunction( tl, br ):
    return 20.0 * ogre.Math.Sin(tl.x - br.y)
 
def CreatePerlinCache(topLeft, bottomRight, edgeLength):
    perl = noise.Perlin()
    perl = noise.Perlin()
    perl.SetFrequency(0.024)
    perl.SetOctaveCount(3)
 
    step = float( edgeLength ) / (bottomRight[0] - topLeft[0])
    pos = topLeft
 
    cache = []
 
    for x in range(edgeLength):
        ly = []
        for y in range(edgeLength):
            val = perl.GetValue( pos[0], pos[1], 0 )
            ly.append(val)
            pos[1] += step
        cache.append(ly)
        pos[0] += step
 
    del perl
    return cache
 
class LandTile:
    def __init__(self, topLeft, bottomRight, quads=48):
        self.tl = topLeft # the base value
        self.br = bottomRight
 
        self.topLeft = ogre.Vector2(topLeft.x, topLeft.y) # an incremented value
 
        self.numVerts = quads
 
        self.verts = []
        self.uvs = []
        self.normals = []
        self.triangles = []
 
        self.isGenerated = False
        self.edgeDist = self.br.x - self.tl.x
 
        self.heightMapFunction = sinFunction
 
        self.perlin = noise.Perlin()
        self.perlin.SetFrequency(0.012)
        self.perlin.SetOctaveCount(2)
 
    def getRandom(self, pos):
        return 30.0 * self.perlin.GetValue( pos.x, pos.y, 0 )
 
    def calcTriangleNormal( self, v1, v2, v3 ):
        return (v2-v1).crossProduct(v3-v1).normalisedCopy()
 
 
    def generate(self):
        incr = float ( (self.br.x - self.tl.x) / self.numVerts )
        mNumVerts = 0
 
        for xv in range( 0, self.numVerts, 4):
            for yv in range(0 , self.numVerts, 4):
                v1 = ( self.topLeft.x + incr, self.getRandom(ogre.Vector2( self.topLeft.x + incr, self.topLeft.y )), self.topLeft.y )
                v2 = ( self.topLeft.x, self.getRandom( ogre.Vector2( self.topLeft.x, self.topLeft.y + incr ) ), self.topLeft.y + incr)
                v3 = ( self.topLeft.x + incr, self.getRandom(self.topLeft + incr), self.topLeft.y + incr)
                v4 = ( self.topLeft.x, self.getRandom(self.topLeft), self.topLeft.y)
 
                self.verts +=  [v1, v2, v3, v4]
 
                tri1 = (mNumVerts+3, mNumVerts+1, mNumVerts)             # [ (0,3,2), (3,2,1) ]
                tri2 = (mNumVerts+1, mNumVerts+2, mNumVerts)
 
                self.triangles += [ tri1, tri2 ]
 
                u0 = (self.topLeft.x - self.tl.x) / self.edgeDist
                v0 = (self.topLeft.y - self.tl.y) / self.edgeDist
                u1 = (self.topLeft.x + incr - self.tl.x) / self.edgeDist
                v1 = (self.topLeft.y + incr - self.tl.y) / self.edgeDist
 
                self.uvs += [ (u1,v0), (u0,v1), (u1,v1), (u0,v0) ]
                self.topLeft.y += incr
                mNumVerts += 4
            self.topLeft.x += incr
            self.topLeft.y = self.tl.y
 
        self.normals = [[0,0,0]] * len(self.verts)
 
        for tri in self.triangles:
            v1 = ogre.Vector3(*self.verts[tri[0]])
            v2 = ogre.Vector3(*self.verts[tri[1]])
            v3 = ogre.Vector3(*self.verts[tri[2]])
 
            norm = self.calcTriangleNormal( v1, v2, v3 )
 
            self.normals[ tri[0] ] = [norm.x, norm.y, norm.z]
            self.normals[ tri[1] ] = [norm.x, norm.y, norm.z]
            self.normals[ tri[2] ] = [norm.x, norm.y, norm.z]
 
 
    def buildGeometry(self, msh):
        msh.begin( "Ogre/Skin" )
        for x in range(len(self.verts)):
            msh.position(*self.verts[x])
            if len(self.normals):
                msh.normal(*self.normals[x])
            if len(self.uvs):
                msh.textureCoord(*self.uvs[x])
 
        for x in range(len(self.triangles)):
            msh.triangle(*self.triangles[x])
 
        msh.end() 
 
 
class NoiseApplication(sf.Application):
    def _createScene( self ):
        sceneManager = self.sceneManager
        camera = self.camera
 
        sceneManager.ambientLight = ogre.ColourValue (0.5, 0.5, 0.5)
 
        sceneManager.setSkyBox(True, "Examples/CloudyNoonSkyBox")
 
        light = sceneManager.createLight('MainLight')
        light.setPosition (20, 20, 50)
        light.setDirection(ogre.Vector3(-1.0, 0, 0.0))
 
        manObj = sceneManager.createManualObject("NoisyLand")
 
        test = LandTile( ogre.Vector2( 0,0 ), ogre.Vector2( 2000, 2000 ), 512 )
        test.generate()
        test.buildGeometry( manObj )
 
        node = sceneManager.getRootSceneNode().createChildSceneNode()
        node.attachObject( manObj )
 
 
if __name__ == '__main__':
    try:
        application = NoiseApplication()
        application.go()
    except ogre.OgreException, e:
        print e