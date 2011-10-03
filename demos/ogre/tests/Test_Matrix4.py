# This code is in the Public Domain.
import ogre.renderer.OGRE as ogre
import SampleFramework as sf
import sys,operator

class TestApplication(sf.Application):

    def _createScene(self):

        sceneManager = self.sceneManager
        camera = self.camera
        
        
        skel = ogre.SkeletonManager.getSingleton().load("jaiqua.skeleton", 
                        ogre.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME ) 
                        #,False, ogre.ManualResourceLoader(), ogre.NameValuePairList()) 
        
        m=ogre.Matrix4(0.0,1.0,2.0,3.0,
                       4.0,5.0,6.0,7.0,
                       8.0,9.0,10.0,11.0,
                       12.0,13.0,14.0,15.0)
        print "\n\ntesting", m
        ##skel._getBoneMatrices( m )    ## this line causes a crash...    
        ##print "\nAfter call", m, "\n\n"
        omgr = ogre.OverlayManager.getSingleton()
        mLoadOverlay = omgr.getByName("Core/LoadOverlay")
        if ( not mLoadOverlay):
            raise OGRE_EXCEPT(Exception.ERR_ITEM_NOT_FOUND, 
                "Cannot find loading overlay", "ExampleLoadingBar.start")
        mLoadOverlay.show()
        mLoadOverlay._getWorldTransforms ( m )
        print m

if __name__ == '__main__':
    try:
        application = TestApplication()
        application.go()
    except:
        print "Unexpected error:", sys.exc_info()[0]
        raise
            
