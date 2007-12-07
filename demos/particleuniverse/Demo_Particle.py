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

        
class SmokeApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
        camera = self.camera

        sceneManager.ambientLight = ogre.ColourValue(0.5, 0.5, 0.5)
#         sceneManager.setSkyDome(True, 'Examples/CloudySky', 5.0, 8.0)

        self.fountainNode = sceneManager.getRootSceneNode().createChildSceneNode()
        
        print dir(PU)
        sm = PU.ParticleSystemManager.getSingletonPtr()
        print sm
# 	std::vector<Ogre::String> names;
# 	mParticleSystemManager->particleSystemTemplateNames(names);
# 	std::vector<Ogre::String>::iterator it;
# 	std::vector<Ogre::String>::iterator itEnd = names.end();
# 	CEGUI::ListboxTextItem* item;
# 	for (it = names.begin(); it != itEnd; ++it)
# 	{
# 		item = new CEGUI::ListboxTextItem((*it), 0);
# 		mListBox->addItem(item);
# 	}

        
        
#         particleSystem1 = sceneManager.createParticleSystem('fountain1', 'Examples/Smoke')
        
        particleSystem2 = PU.ParticleSystemManager.getSingleton().createParticleSystem("mySystem", "example_003", sceneManager)
        print particleSystem2
        
        node = self.fountainNode.createChildSceneNode()
        node.attachObject(particleSystem2)
        
        
       
                
if __name__ == '__main__':
    try:
        application = SmokeApplication()
        application.go()
    except ogre.OgreException, e:
        print e
