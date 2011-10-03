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
import dotscene as DotScene
import os, sys

class DotSceneApplication(sf.Application):
    def _setUpResources ( self ):
        # first load the default resources
        sf.Application._setUpResources ( self )
        
        # Now load any extra resource locations that we might need..  
        # in the example I'm adding the entire tree under the base directory
        bases = ["../media_extra/quake"]
        for base in bases: 
            for directory in os.listdir ( base ):
                fullPath = os.path.join ( base, directory )
                if os.path.isdir( fullPath ):
                    ogre.ResourceGroupManager.getSingleton().addResourceLocation(fullPath,"FileSystem", "General")
        
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
        
        # to keep things clean I'm creating a parent node to add the car to..            
        self.carNode = self.sceneManager.getRootSceneNode().createChildSceneNode("DotSceneRoot")
        
        # now load the 'scene', which in this case is a vechile made from multiple meshes..
        self.dotscene = DotScene.DotScene("../media_extra/quake/roadster/roadster.scene", self.sceneManager, self.carNode)
        
        camera.setPosition ( -80,40,100)
        camera.lookAt ( 0,0,0)
                
    def __del__(self):
        del self.particleSystem2
        sf.Application.__del__(self)     
                
if __name__ == '__main__':
    try:
        application = DotSceneApplication()
        application.go()
    except ogre.OgreException, e:
        print e
