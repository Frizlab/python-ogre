import sys
import os
sys.path.insert(0,'..')
# import PythonOgreConfig

import ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ctypes, math, random


def testLoadMesh(meshSerializer, mesh_name):
    f = file(os.path.join("../../media/models", mesh_name), 'rb')
    fileString = f.read()
    memBuffer = ctypes.create_string_buffer( fileString )
    memstream = ogre.MemoryDataStream ( ogre.CastVoidPtr(ctypes.addressof (memBuffer )), len (fileString) + 1 ) ##+1??
    f.close()

    print dir(memstream)
    print len (memBuffer)
    print type(memstream)
    
    mesh = ogre.Mesh(ogre.MeshManager.getSingletonPtr(), mesh_name, 0, ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME)
    print mesh
# # # #     mesh = meshSerializer.importMesh(memstream, mesh)
    print mesh
    memstream.close()
    print "Load Mesh Num Sub Meshes %d" % mesh.numSubMeshes
    del mesh    



##--------- Main Content -----------------------------    

logMgr = ogre.LogManager()
logMgr.createLog('Test', False)
rgm = ogre.ResourceGroupManager()
mth = ogre.Math()
meshMgr = ogre.MeshManager()
matMgr = ogre.MaterialManager()
matMgr.initialise()
skelMgr = ogre.SkeletonManager()
meshSerializer = ogre.MeshSerializer()
skeletonSerializer = ogre.SkeletonSerializer()
bufferManager = ogre.DefaultHardwareBufferManager()# needed because we don't have a rendersystem


## import mesh           
testLoadMesh(meshSerializer, 'ogreHead.mesh')


##--------- cleanup -----------------------------    
del skeletonSerializer
del meshSerializer
del skelMgr
del matMgr
del meshMgr
del bufferManager
del mth
del rgm
del logMgr        
