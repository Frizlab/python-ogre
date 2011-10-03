import ogre.renderer.OGRE as Ogre
import sys

config = Ogre.ConfigFile()
config.load('resources.cfg' ) #, '', False )

### Note we have to use the C++ access to the iterator as we need access to the 'KEY' value
### if we use the python iterator style (for x in iter:) we only get the value

iter = config.getSectionIterator()
print iter
for i in iter:
    print "__ ", i.keys(), i
    for j in i:
        print j.key, j.value

print dir(i)

iter = config.getSectionIterator()
while iter.hasMoreElements():
    SectionName = iter.peekNextKey() 
    Section = iter.getNext() 
    for item in Section:
        print SectionName, item.key, item.value
       
