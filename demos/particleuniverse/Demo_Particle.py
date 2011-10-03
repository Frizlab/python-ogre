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
import sys,os
sys.path.insert(0,'..')
sys.path.insert(0,'.')

## this here becase the ParticelUnivser.dll needs to be access by both the python module and the ogre plugins
## need a better fix ling term
if sys.platform == 'win32': 
    newpath = os.path.join ( os.path.abspath(os.path.dirname(__file__)), '../../plugins')
    os.environ['PATH'] =  newpath +';' + os.environ['PATH']
    
import PythonOgreConfig
import ogre.renderer.OGRE as ogre
import ogre.addons.particleuniverse as PU
import SampleFramework as sf
import ogre.io.OIS as OIS


        
class ParticleApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        sceneManager.ambientLight = ogre.ColourValue(1.0, 1.0, 1.0)
#         sceneManager.setSkyDome(True, 'Examples/CloudySky', 5.0, 8.0)

        # create a main node to hang the effects off
        self.fountainNode = sceneManager.getRootSceneNode().createChildSceneNode()
        self.fountainNode.setPosition(ogre.Vector3(0, 0, -800))
        ## Setup Camera
        camera.setPosition(ogre.Vector3(0,0,300))
        camera.lookAt(ogre.Vector3(0,0,-300))
        
        
        mParticleSystemManager = PU.ParticleSystemManager.getSingletonPtr()
        self.names = ogre.stdVectorString()
        mParticleSystemManager.particleSystemTemplateNames(self.names)
	    
        for n in self.names:
            print "PU template:", n
            
        self.currentDemo=0   
        # create the particle system
        self.particleSystem2 = PU.ParticleSystemManager.getSingleton().createParticleSystem("mySystem", self.names[self.currentDemo], sceneManager)
        
        # attach it to the node
        self.fountainNode.attachObject(self.particleSystem2)
        
        # and don't forget to start it
        self.particleSystem2.start()
        name= self.particleSystem2.name
        self.ps = self.sceneManager.getMovableObject(name, PU.ParticleSystemFactory.PU_FACTORY_TYPE_NAME)
        
    def _createFrameListener(self):
        self.frameListener = ParticleListener(self.renderWindow, self.camera, self.sceneManager, self )
        self.root.addFrameListener(self.frameListener)


class ParticleListener(sf.FrameListener):
    def __init__(self, renderWindow, camera, sceneManager, app):
        self.app = app
        sf.FrameListener.__init__(self, renderWindow, camera)
        
    def frameStarted(self, frameEvent):
        app = self.app
        if self._isToggleKeyDown(OIS.KC_N, 0.1) and 0:
            app.currentDemo += 1
            if app.currentDemo > len ( app.names ):
                app.currentDemo = 0
            app.particleSystem2.stop()
                        
            app.particleSystem2 = PU.ParticleSystemManager.getSingleton().createParticleSystem("mySystem", app.names[app.currentDemo], app.sceneManager)
            # attach it to the node
            app.fountainNode.attachObject(app.particleSystem2)
            
            # and don't forget to start it
            app.particleSystem2.start()
            name= app.particleSystem2.name
            app.ps = app.sceneManager.getMovableObject(name, PU.ParticleSystemFactory.PU_FACTORY_TYPE_NAME)
            
        return sf.FrameListener.frameStarted(self, frameEvent)
            
if __name__ == '__main__':
    try:
        application = ParticleApplication()
        application.go()
    except ogre.OgreException, e:
        print e
