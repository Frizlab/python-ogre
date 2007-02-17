import Ogre
import sys

def boneIterator(iterator):
   while iterator.hasMoreElements():
       yield iterator.peekNextKey(),iterator.getNext()
                
class SectionIteratorTest ( Ogre.SectionIterator ):
    def __init__(self, iterator ):
        self.iterator = iterator
    def __iter__(self):
            return self
    def next(self):
        if self.iterator.hasMoreElements():
            return self.iterator.getNext()
            #return self.iterator.peekNextKey(), self.iterator.getNext()
        else:
            raise StopIteration
           
            
config = Ogre.ConfigFile()
config.load('resources.cfg' ) #, '', False )

# seci = boneIterator ( config.getSectionIterator() )
# seci = SectionIteratorTest ( config.getSectionIterator() )
iter = config.getSectionIterator()
while iter.hasMoreElements():
    SectionName = iter.peekNextKey() 
    Section = iter.getNext() 
    for item in Section:
        print SectionName, item.key, item.value
       
        
sys.exit()
for (Section,item) in seci:
   # print key,item  
    for key in item.keys() :
        for i in item[key]:
            print Section, "\t", key, "\t\t", i    

    
# seciNew =  config.getSectionIterator() 
# print dir (seciNew)
# for (key, item ) in seciNew:
#     print key, item