## minimal application from Dermont 
import sys
import os
sys.path.insert(0,'..')
import PythonOgreConfig
import ogre.renderer.OGRE as ogre

if os.name == 'nt':
    plugin_path = os.path.join(os.getcwd(),'..','plugins.cfg.nt')
    renderName = "Direct3D9 Rendering Subsystem"
else:
    plugin_path = os.path.join(os.getcwd(),'..','plugins.cfg.posix')
    renderName = "OpenGL Rendering Subsystem"
    
root = ogre.Root(plugin_path, "ogre.cfg", "ogre.log")
resGroup = 'Bootstrap'
ogre.ResourceGroupManager.getSingleton().createResourceGroup(resGroup)
#ogre.ResourceGroupManager.getSingleton().addResourceLocation('./test64bit', 'FileSystem',  resGroup, False)
ogre.ResourceGroupManager.getSingleton().addResourceLocation('../media/packs/OgreCore.zip', 'Zip', resGroup, False)

#carryOn = root.restoreConfig()
carryOn = root.showConfigDialog()

if carryOn:
   rsys = root.getRenderSystemByName(renderName)
   rsys.setConfigOption("Full Screen", "No")
   root.setRenderSystem(rsys)
   renderWindow = root.initialise(True, "Ogre Render Window")

ogre.ResourceGroupManager.getSingleton().initialiseResourceGroup(resGroup)
del root