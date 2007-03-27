
def header_files( version ):
    if not version.startswith("1.2"):
        return [ 'python_ogre_masterlist.h'
                ,'py_shared_ptr.h'
                , 'generators.h'
##                , 'return_pointee_value.hpp'
                ,'boost/python/ssize_t.hpp' ## needed for python2.4 as it doesn't define ssize_t

        ]

      
    else: #1.2
        return [ 'Ogre.h'
                , 'OgreErrorDialog.h'
                , 'OgreConfigDialog.h'
                , 'OgreInputEvent.h'
                , 'OgreEventListeners.h'
                , 'OgreTagPoint.h'
                , 'OgreTargetManager.h'
                , 'OgreOverlayElementFactory.h'
                , 'OgreArchiveFactory.h'
                , 'OgreParticleSystemRenderer.h'
                , 'OgreParticleEmitterFactory.h'
                , 'OgreParticleAffectorFactory.h'
                , 'OgreSkeletonInstance.h'
                , 'OgreSkeletonManager.h'
                , 'OgreSkeleton.h'
                , 'OgreCompositor.h'
                , 'OgrePatchMesh.h'
                , 'OgreMesh.h'
                , 'OgreHighLevelGpuProgram.h'
                , 'OgreHardwarePixelBuffer.h'
                , 'OgreEventQueue.h'
                , 'OgreKeyEvent.h'
                , 'OgreHardwareOcclusionQuery.h'
                , 'py_shared_ptr.h'

        ]

def huge_classes( version ):
    if not version.startswith("1.2"):
        return [ 'RenderSystem'
                , 'StaticGeometry'
                , 'Node'
                , 'Pass'
                , 'BillboardSet'
                , 'ParticleEmitter'
                , 'ParticleSystem'
                , 'SceneManager'
                , 'MovableObject'
                , 'OverlayElement'
                , 'ManualObject'
                , 'RenderTarget'
                , 'Frustum'
                , 'SceneNode'
            ]
    else: #1.2
        return [ 'RenderSystem'
            , 'StaticGeometry'
            , 'Node'
            , 'Pass'
            , 'BillboardSet'
            , 'ParticleEmitter'
            , 'ParticleSystem'
            , 'SceneManager'
            , 'MovableObject'
            , 'OverlayElement'
            , 'ManualObject'
            , 'RenderTarget'
            , 'Frustum'
            , 'SceneNode'
        ]
