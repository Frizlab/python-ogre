# This code is in the Public Domain
import  Ogre as ogre
import SampleFramework as sf
import ctypes, math,sys, os, os.path

pluginfile = "plugins.cfg"
root = ogre.Root( pluginfile )

config = ogre.ConfigFile()
config.load('resources.cfg' ) 

print "\n**Test: checking c++ style iteration with configFile"
section_iter = config.getSectionIterator()
while section_iter.hasMoreElements():
    section_name = section_iter.peekNextKey()
    settings = section_iter.getNext()
    for item in settings:
#         print "   Adding:", item.value, item.key, section_name
        ogre.ResourceGroupManager.getSingleton().addResourceLocation(item.value, item.key, section_name)
        
sceneManager = root.createSceneManager(ogre.ST_GENERIC,"ExampleSMInstance")

print "\n**Test: iteration through the Renderers"
for r in root.getAvailableRenderers():
    print "   Renderer",r.getName(), r, type(r)
    
root.setRenderSystem(r)    
root.initialise(False)
ogre.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()    
print "\n**Test: Iteration through Resources, and correct return types"    
## getByName returns a 'Resource' object
tm=ogre.TextureManager.getSingleton()
for r in tm.getResourceIterator():
    name = r.getName()
    # now try to get it byname
    Res = tm.getByName(name)
    print "   ", name, " should be a Texture object", Res
fm=ogre.FontManager.getSingleton()
for r in fm.getResourceIterator():
    name = r.getName()
    # now try to get it byname
    Res = fm.getByName(name)
    print "   ", name, " should be a Font object", Res

        
print "\n**Test: checking getChildIterators"        
overman = ogre.OverlayManager.getSingleton()
statspanel = overman.getOverlayElement('Core/StatPanel', False)
        # Old/C++ style (works fine)
#         childiter = statspanel.getChildIterator()
#         while childiter.hasMoreElements():
#             name = childiter.getNext().getName()
#             print "found child:", name
        # Using Python Iterators
for child in statspanel.getChildIterator():
    print "   Child:", child.getName()
        
       
        
        
        
