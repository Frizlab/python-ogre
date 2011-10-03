# NO_UNITTEST
import Pyro.core
import Pyro.naming
import ogre.renderer.OGRE as ogre

class main(Pyro.core.ObjBase):

    def __init__(self):

        #This is a Pyro object, like 'extends Remote' in java
        Pyro.core.ObjBase.__init__(self)

        #init Ogre
        self.root = ogre.Root("plugins.cfg","ogre.cfg")

    def showConfig(self):
        self.root.showConfigDialog()

    def initWindow(self):
        self.root.initialise(True, "ogrepyro-server")

    def startRendering(self):
        self.root.startRendering()

    def getOgreRoot(self):
         return self.root

#init pyro and run the pyro listening server
daemon=Pyro.core.Daemon()
uri=daemon.connect(main(),"app_main")
daemon.requestLoop()