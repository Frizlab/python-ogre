# This code is in the Public Domain
import  ogre.renderer.OGRE as ogre
import SampleFramework as sf
import ctypes, math,sys

class TestApplication(sf.Application):
    def _createScene(self):
        sceneManager = self.sceneManager
# #         result = materialManager.createOrRetrieve(texture, group) 
# #         created = result.second 
# #         material = materialManager.getByName(texture) 
        ## getByName returns a 'Resource' object
        tm=ogre.TextureManager.getSingleton()
        for r in tm.getResourceIterator():
            name = r.getName()
            # now try to get it byname
            Res = tm.getByName(name)
            print "**TEST** : ", name, " should be a Texture object", Res
        fm=ogre.FontManager.getSingleton()
        for r in fm.getResourceIterator():
            name = r.getName()
            # now try to get it byname
            Res = fm.getByName(name)
            print "**TEST** : ", name, " should be a Font object", Res
        print "TESTING NODE RETURN"    
        skel = ogre.SkeletonManager.getSingleton().load("jaiqua.skeleton", ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME,False, ogre.ManualResourceLoader(), ogre.NameValuePairList()) 
        print "TEST\n",skel,"\n"
        for i in range( skel.numBones): 
            bone = skel.getBone(i)
            print "BONE", bone 
            print dir ( bone )
            node = bone.asNode()
            print "NODE", node
            print dir ( node )
            c = bone.createChild ( 90 )
            print "CHILD:", c
            print bone.numChildren()
            print c.numChildren()
            n = ogre.Node.createChild ( c.asNode(), "test" )
            print "Child Node:", n
            sys.exit()
            for j in range(bone.numChildren()): 
                # Child bones, one per part than can be damaged 
                child = bone.getChild(j) 
                print "Child1", child
                for k in range(child.numChildren()): 
                    # Grandchild bones, one damaged and one undamaged 
                    # These guys have geometry attached, should therefore be SceneNodes 
                    print "CHILD TYPE", child, child.getChild(k) 
        
        ##### code for testing OverlayContainer.getChildIterator
        ##### --strank
        overman = ogre.OverlayManager.getSingleton()
        statspanel = overman.getOverlayElement('Core/StatPanel', False)
        print statspanel
        
        # Old/C++ style (works fine)
#         childiter = statspanel.getChildIterator()
#         while childiter.hasMoreElements():
#             name = childiter.getNext().getName()
#             print "found child:", name
        # Using Python Iterators
        for child in statspanel.getChildIterator():
            print "Child:", child.getName()
        
        sys.exit()        
        
        
        

        
if __name__ == '__main__':
    try:
        application = TestApplication()
        application.go()
    except ogre.OgreException, e:
        print e
