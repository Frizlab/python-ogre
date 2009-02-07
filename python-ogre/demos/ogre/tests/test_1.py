#!/bin/python 

# Testcase for PythonOgre 1.6RC1 asX hand-wrapper bug. 

import ogre.renderer.OGRE as ogre 

# initialize ogre 
root = ogre.Root()  # defaults to using local plugins.cfg 
if not root.showConfigDialog(): 
    raise Exception("Failed to pick rendering system.") 
renderWindow = root.initialise(True, "OGRE Render Window") 

sceneManager = root.createSceneManager(ogre.ST_GENERIC, "ExampleSMInstance") 
pssm = ogre.LiSPSMShadowCameraSetup() 
sceneManager.setShadowCameraSetup(pssm) 

print "Hey, it worked." 
