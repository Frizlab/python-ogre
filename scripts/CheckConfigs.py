import sys
sys.path.insert(0,'..') # need this to get enrivonment.py into path

import PythonOgreConfig_nt as p1
import PythonOgreConfig_posix as p2
import PythonOgreConfig_system as p3
f = open ( "../environment.py" )
line = ''.join (f.readlines ())
f.close()

for name,value in p1.__dict__.items():
    if name.startswith ('PATH'):
        if not p2.__dict__.has_key(name):
            if name in line:
                print "Missing:", name
            else:
                print "Not Used:", name
               