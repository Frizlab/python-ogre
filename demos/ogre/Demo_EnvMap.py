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
import Ogre as ogre
import SampleFramework as sf

class EnvMapApplication(sf.Application):
    def _createScene( self ):
        sceneManager = self.sceneManager
        camera = self.camera
        
        sceneManager.ambientLight = ogre.ColourValue (0.5, 0.5, 0.5)

        light = sceneManager.createLight('MainLight')
        light.setPosition (20, 80, 50)

        entity = sceneManager.createEntity('head', 'ogrehead.mesh')
        entity.setMaterialName("Examples/EnvMappedRustySteel")

        sceneManager.getRootSceneNode().createChildSceneNode().attachObject(entity)

if __name__ == '__main__':
    try:
        application = EnvMapApplication()
        application.go()
    except ogre.Exception, e:
        print e
