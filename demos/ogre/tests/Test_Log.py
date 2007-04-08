import ogre.renderer.OGRE as Ogre

class MyLog(Ogre.LogListener):
    def __init__(self):
        # Creates a C++ log that will try and write to console and file
        Ogre.LogListener.__init__(self)
                 
    def messageLogged(self, message, level, debug, logName):
        # This should be called by Ogre instead of logging
        print 'Python Logger Called -- Listener works !!!'

# Create the global log manager instance
logMgr = Ogre.LogManager()

# create the instance of our log listener
myLog = MyLog()

# create a "log"
currentLog = Ogre.LogManager.getSingletonPtr().createLog("dummy.log" 
                                                    , True  # it's the default log
                                                    , False     # I don't want it sent to the debug window
                                                    , False     # it's a virtual log, so you need a listener :)
                                                    )  
# register our listener
currentLog.addListener ( myLog )    

# And test it
Ogre.LogManager.getSingletonPtr().logMessage('Should Not Appear',
                                             Ogre.LML_CRITICAL, False)  
