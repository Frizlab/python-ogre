import os
import sys

gccxml_path = r'c:/development/gccxml/bin/release/gccxml.exe'
pygccxml_base_dir = r'c:\development\pyplus'
working_dir = r'c:\development\python-ogre-r'
headers_dir = r'c:\development\ogresdk7\include'

python_headers_dir = 'c:/python25/include'

sys.path.append (pygccxml_base_dir + r'/pygccxml_dev')
sys.path.append (pygccxml_base_dir + r'/pyplusplus_dev')


cache_dir = working_dir + r'\xml_cache'
build_dir = working_dir + r'/build'
decl_cache_dir = working_dir + r'/decl_cache'


ogre_header_list = [
    'OgreErrorDialog.h',
    'OgreConfigDialog.h',
    'OgreInputEvent.h',
    'OgreEventListeners.h',
    'OgreTagPoint.h',
    'OgreTargetManager.h',
    'OgreOverlayElementFactory.h',
    'OgreArchiveFactory.h',
    'OgreParticleSystemRenderer.h',
    'OgreParticleEmitterFactory.h',
    'OgreParticleAffectorFactory.h',
    'OgreSkeletonInstance.h',
    'OgreSkeletonManager.h',
    'OgreSkeleton.h',
    'OgreCompositor.h',
    'OgrePatchMesh.h',
    'OgreMesh.h',
    'OgreHighLevelGpuProgram.h',
    'OgreHardwarePixelBuffer.h',
    'OgreEventQueue.h',
    'OgreKeyEvent.h',
]


