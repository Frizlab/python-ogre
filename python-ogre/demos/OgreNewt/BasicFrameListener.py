import Ogre
import ODE
import OgreNewt as OgreNewt
import SampleFramework as sf

class BasicFrameListener (Ogre.FrameListener ): ##, Ogre.WindowEventListener ): # sf.FrameListener):

    def __init__(  self, win,  SceneManager,  World , update_framerate): 
##BasicFrameListener(Ogre::RenderWindow* win, Ogre::SceneManager* mgr, OgreNewt::World* W, int update_framerate = 60);

#        sf.FrameListener.__init__ (self, win, Camera )
        Ogre.FrameListener.__init__(self)
#        Ogre.WindowEventListener.__init__(self)
        self.World = World
        self.desired_framerate = update_framerate
        self.renderWindow = win
        self.sceneManager = SceneManager
        self.update = (1.0 / self.desired_framerate)
        self.elapsed = 0.0;
    
    #  add the standard debug viewer.
    #  OgreNewt.Debugger.getSingleton().init( mgr )
    
    def __del__ ( self ):
        pass
        #Ogre.FrameListener.__del__(self)

    def frameStarted(self, evt):
        Ogre.FrameListener.frameStarted(self, evt)
        self.elapsed += evt.timeSinceLastFrame
    #   Ogre::LogManager::getSingleton().logMessage("   Newton Frame Listener... m_elapsed: "+Ogre::StringConverter::toString(m_elapsed)+
    #       "  m_update:"+Ogre::StringConverter::toString(m_update));
    
        count = 0
        ## print "in framestated", self.elapsed, self.update
        if ((self.elapsed > self.update) and (self.elapsed < (1.0)) ):
            while (self.elapsed > self.update):
                self.World.update( self.update )
#                print "1..world updated", self.update
                self.elapsed -= self.update
                count += 1
        else:
            if (self.elapsed < self.update):
                ## not enough time has passed this loop, so ignore for now.
                pass
            else:
                self.World.update( self.elapsed )
#                print "2..world updated", self.elapsed
                count += 1
                self.elapsed = 0.0 ## reset the elapsed time so we don't become "eternally behind"
    
    #   Ogre::LogManager::getSingleton().logMessage("   Newton updates this loop: "+Ogre::StringConverter::toString(count));
    #   ##      DEBUGGER
    #   mInputDevice->capture();
    
    #   if (mInputDevice->isKeyDown(Ogre.KC_F3)):
    #       OgreNewt.Debugger.getSingleton().showLines( self.World )
    #   else:
    #       OgreNewt.Debugger.getSingleton().hideLines()
       # return Ogre.FrameListener.frameStarted( self, evt )    
        return True #sf.FrameListener.frameStarted( self, evt )
