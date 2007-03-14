#!/usr/bin/env python
# -----------------------------------------------------------------------------
# This source file is part of Python-Ogre
# For the latest info, see http://www.python-ogre.org/
#
# Copyright (c) 2006-2007 Python-Ogre Development Team
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free Software
# Foundation; either version 2 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 59 Temple
# Place - Suite 330, Boston, MA 02111-1307, USA, or go to
# http://www.gnu.org/copyleft/lesser.txt.
#
# You may alternatively use this source under the terms of a specific version of
# the Python-Ogre Unrestricted License provided you have obtained such a license
# -----------------------------------------------------------------------------
#

DESCRIPTION = """Python-Ogre is a Python wrapper module for the
OGRE 3D Graphics library. It contains python functions and classes
that will allow you to fully utilitize the Ogre library.  It is 
Unique in that the python wrappers are created automatically by
Py++, hence making maintainence etc very simple.
This version is built against the Ogre CVS"""

METADATA = {
    "name":             "Python-Ogre",
    "version":          "0.94 Candidate",
    "license":          "LGPL",
    "url":              "http://python-ogre.python-hosting.com/",
    "author":           "Lakin Wecker, Roman Yakovenko, Andy",
    "author_email":     "python-ogre-developers@googlegroups.com",
    "description":      "Py++ Ogre Python Binding",
    "long_description": DESCRIPTION,
}
from distutils.core import setup
import os, sys

def GetFileList ( basepath ):
    filesout = []
    for root, dirs, files in os.walk(basepath):
        for name in files:
            filesout.append(os.path.join(root, name))
    return filesout

base = os.path.join(os.getcwd(), os.path.dirname(sys.argv[0]) )
demofiles = GetFileList ( os.path.join (base, "demos") )

PACKAGEDATA = {
       "packages":    ['Ogre', 'OIS', 'CEGUI', 'OgreRefApp', 'OgreNewt', 'OgreOde','ODE', 'OgreAL' ],
       "package_dir": {'': 'packages'},
       "package_data": {'': ['*.pyd', '*.dll']}

}

PACKAGEDATA.update(METADATA)
setup(**PACKAGEDATA)
 

 