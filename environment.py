import os
import sys
import getpass

if 'roman' in getpass.getuser():
    gccxml_path = r'd:/gccxml_cvs/gccxml-build/bin/release/gccxml.exe'
    pygccxml_base_dir = r'D:\pygccxml_sources\sources'
    working_dir = r'D:\python-ogre'
    headers_dir = r'D:\OgreSDK\include'
    python_headers_dir = 'c:/python/include'
else:
    gccxml_path = r'c:/development/gccxml/bin/release/gccxml.exe'
    pygccxml_base_dir = r'c:\development\pyplus'
    working_dir = r'c:\development\python-ogre-r'
    headers_dir = r'c:\development\ogresdk7\include'
    python_headers_dir = 'c:/python25/include'


sys.path.append (pygccxml_base_dir + r'/pygccxml_dev')
sys.path.append (pygccxml_base_dir + r'/pyplusplus_dev')

cache_dir = working_dir + r'/xml_cache'
build_dir = working_dir + r'/build'
declarations_cache_file = working_dir + r'/declarations_cache/cache.xml'

