import os, sys, new, ConfigParser

CONFIGFILE = "config.ini"
root_dir = os.path.abspath(os.path.dirname(__file__) )## The root directory is where this module is located
SourceParent

# 
# first we need to get the master class so we know all the attributes to set
#

class Default:
    version = ""
    pythonNodule = False
    moduleName = ""
    home = ""
    librarypaths = [ ]
    libraries = [ ]
    parent = None
    children = []
    codeGenerationFlags=[]
    dependsOn = []
    compileFlags = []
    linkFlags= []
    includePaths = []
    includes = []
    libaries = myLibraries # a list of the libraries I need plus
    source = []   # where to get the source - tuples, {'svn|cvs|ftp|http','utl'}
    buildCmds = []    # commands to build the library with
    
    
def createClass ( name ):
    c=ConfigParser.ConfigParser()
    c.read( CONFIGFILE )
    items = c.items(name)
    cls = Default ()    # create a new class
    # now the specifics
    for i in items:
        name, value = i
        g=getattr(cls, name )
        print g, type ( g )
#         setattr(cls, name, value )

if __name__ == '__main__':
    c=createClass ( 'ogre' )
    print c
    print dir(c)    