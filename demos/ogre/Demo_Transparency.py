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
import SampleFramework as sf

class TransparencyApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)

        light = sceneManager.createLight('MainLight')
        light.setPosition ( 20, 80, 50 )
        
        theplane = self.sceneManager.createEntity("ThePlane", ogre.SceneManager.PT_PLANE )
        theplane.setMaterialName( "Simple/Translucent" )
        
        planeNode = self.sceneManager.getRootSceneNode().createChildSceneNode()
        planeNode.attachObject( theplane )
        planeNode.setPosition( ogre.Vector3(0,0,0) )
        planeNode.setOrientation( ogre.Quaternion( ogre.Degree(d=-90), ogre.Vector3(1,0,0) ) )

#         planeEntity = self.sceneManager.createEntity('Plane', ogre.SceneManager.PT_PLANE)
#         planeEntity.setMaterialName('Examples/BumpyMetal')

        knotEntity = self.sceneManager.createEntity('Knot', 'knot.mesh')
        knotEntity.setMaterialName("Examples/WaterStream") #'Examples/Transparent')

        rootNode = self.sceneManager.getRootSceneNode()
        #rootNode.attachObject(planeEntity)
#         rootNode.attachObject(theplane)
        rootNode.attachObject(knotEntity)

        for index in xrange(0,10):
            node = self.sceneManager.createSceneNode()
            nodePos = ogre.Vector3()
            nodePos.x = ogre.Math.SymmetricRandom() * 500.0
            nodePos.y = ogre.Math.SymmetricRandom() * 500.0
            nodePos.z = ogre.Math.SymmetricRandom() * 500.0
            node.setPosition (nodePos)
            rootNode.addChild(node)
            knotEntityClone = knotEntity.clone('Knot%d' % index )
            node.attachObject(knotEntityClone)

if __name__ == '__main__':
    try:
        application = TransparencyApplication()
        application.go()
    except ogre.OgreException, e:
        print e    
