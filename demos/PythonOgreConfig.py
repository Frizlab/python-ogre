#
# a single config file that is imported 'first' by the Python-Ogre demos
# this is primarly used to manage Linux demos when we have binaries in non standard (safe) locations

import os,sys
if os.name != 'nt':
    libpath=""
    pythonpath = ""
    