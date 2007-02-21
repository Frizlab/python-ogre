import Ogre
import sys

config = Ogre.ConfigFile()
config.load('resources.cfg' ) #, '', False )

### Note we have to use the C++ access to the iterator as we need access to the 'KEY' value
### if we use the python iterator style (for x in iter:) we only get the value

iter = config.getSectionIterator()
while iter.hasMoreElements():
    SectionName = iter.peekNextKey() 
    Section = iter.getNext() 
    for item in Section:
        print SectionName, item.key, item.value
       
