import os
import sys

gccxml_path = r"/home/lakin/.local/bin/gccxml" 
pygccxml_base_dir = r'/home/lakin/Documents/non-backup/Software/Compile/pygccxml'
working_dir = r'/home/lakin/code_personal/python-ogre/'
headers_dir = r'/home/lakin/.local/include/OGRE'

python_headers_dir = r'/usr/include/python' + os.environ['PYTHON_VERSION']

sys.path.append (pygccxml_base_dir + r'/pygccxml_dev')
sys.path.append (pygccxml_base_dir + r'/pyplusplus_dev')


cache_dir = working_dir + r'/xml_cache'
build_dir = working_dir + r'/build'


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


