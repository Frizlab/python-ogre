import ogre.renderer.OGRE as ogre 
class program: 
    def __init__(self): 
        self.root = ogre.Root("plugins.cfg","ogre.cfg","ogre.txt") 
        self.root.showConfigDialog() 
        self.root.initialise(True) 
main = program() 
while True: 
    main.root.renderOneFrame()