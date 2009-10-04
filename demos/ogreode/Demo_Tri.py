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
import ogre.physics.OgreOde as OgreOde

class TestApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        sceneManager.setSkyDome(True, 'Examples/CloudySky', 5.0, 8.0)
        self._world = OgreOde.World(sceneManager)
        
        Verts = [(0,0,0), (1,0,0), (1,2,0), (0,2,0), (1,2,2,), (3,4,5)]
        indicies = [0,1,2,3,4,2,2,1,2,10]
        v=[]
        for x in Verts:
           v.append ( ogre.Vector3(*x) )
        t = OgreOde.makeTriangleMeshGeometry( v, len(v), indicies, len(indicies), self._world ),  self._world.getDefaultSpace())
        print "TRIANGLE",t                                       

        t = OgreOde.makeConvexGeometry( v, len(v), indicies, len(indicies), self._world,  self._world.getDefaultSpace())
        print "Convex",t                                       

       
                
if __name__ == '__main__':
    try:
        application = TestApplication()
        application.go()
    except ogre.OgreException, e:
        print e
