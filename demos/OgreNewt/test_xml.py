# test xml
filename ="c:/development/pywork/demos/media/models/zombie_rag_hull.xml"

from xml.dom.minidom import parse, parseString
doc = parse(filename) # parse an XML file by name
print doc
print dir(doc)
root = doc.documentElement
print "Root", root

first = root.firstChild
bone = root.getElementsByTagName ("Bone")
print "DIR OF BONE:\n", dir(bone[0]), "\n\n"
first = bone[0]
print "\n\n"
print first
if first.attributes:
    print "a:", first.attributes
    print dir(first.attributes)
    for i in first.attributes.items():
        print i
    for k in first.attributes.keys():
        print k
    
    print first.attributes.getNamedItem('dir').value
    print dir(first.attributes.getNamedItem('dir'))

print bone[0].getAttribute("dir").split()  #vector3

child = bone[0].firstChild;
for b in  bone[0].childNodes:
    if b.nodeName == "Bone":
    
        print "CHILD:", b
    
        
while (child):
    print child
    child =child.nextChild


