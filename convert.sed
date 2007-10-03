# sedscript to convert C++ Ogre programs t0 python
# simple stuff - comments, -> and :: to ., true/false spelling, remove ;, remove trailing 'f' on floats
s/\-\>/\./
s/\/\//##/
s/\-\>/\./
s/\:\:/\./
s/true/True/
s/false/False/
s/NULL/None/
s/this/self/
s/;/ /
s/const//
#s/\(^W*\)Ogre::^w+\(^W*\)/\1/
s/\([0-9]\)f/\1/
# put trailing : after if and else
s/\(^\W*if.*$\)/\1:/
s/\(^\W*else.*$\)/\1:/
# open and closing braces on their own lines get removed
s/^\W*{\W*$//
s/^\W*}\W*$//

#    (r'^[\W]*{[\W]*$\n',''),
#    (r'^[\W]*}[\W]*$\n',''),
    #(r'void \W*(\w+)\(','def $1('),
##    (r'^/\*.*\*/', 'special3')
