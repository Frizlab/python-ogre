#I think it is possible to create code that will do that automaticly.

def aliases( version ):
    if version == "CVS":
        return {
              "map<unsigned, Ogre::StaticGeometry::Region*, std::less<unsigned>, std::allocator<std::pair<unsigned const, Ogre::StaticGeometry::Region*> > >"
                : "MapOfRegionIterator"
              
            ,  "map<std::string, bool, std::less<std::string>, std::allocator<std::pair<std::string const, bool> > >"
              : "MapOfStringToBool"
            , "map<std::string, Ogre::AnimationState*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::AnimationState*> > >" 
              :  "MapOfAnimationState"
            , "map<std::string, Ogre::OverlayElement*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::OverlayElement*> > >"
              : "MapOfStringToOverlayElement"
            , "map<std::string, Ogre::MovableObject*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > >"
              : "MapOfStringToMovableObject"
            , "map<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >"
              : "MapOfStringToString"
            , "map<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >"
              : "NamedValuePairList"
            , "vector<float, std::allocator<float> >"
              : "VectorOfFloat"
            , "vector<std::string, std::allocator<std::string> >"
              : "VectorOfString"
            , "pair<int, Ogre::CompositorInstance::RenderSystemOperation*>"
              : "PairOfIntAndRenderSystemOperationPtr"
            , "list<Ogre::Plane, std::allocator<Ogre::Plane> >"
              : "ListOfPlane"
            , "set<Ogre::SceneQuery::WorldFragmentType, std::less<Ogre::SceneQuery::WorldFragmentType>, std::allocator<Ogre::SceneQuery::WorldFragmentType> >"
              : "SetOfWorldFragmentTypes"
            , "set<std::string, std::less<std::string>, std::allocator<std::string> >"
              : "SetOfStrings"
            , "map<std::string, Ogre::OverlayElement*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::OverlayElement*> > >"
              : "MapOfOverlayElement"
            , "MapIterator<std::map<std::string, Ogre::OverlayElement*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::OverlayElement*> > > >"
              : "MapIteratorOfOverlayElement"
            , "Controller<float>"
              : "FloatController"
            , "ControllerFunction<float>"
              : "FloatControllerFunction"
            , "ControllerValue<float>"
              : "FloatControllerValue"
            , "TRect<float>"
              : "FloatRect"
            , "map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >,Ogre::_ConfigOption,std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >,std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::_ConfigOption> > >"
              : "MapOfStringToConfigOption"
            , "vector<Ogre::StaticGeometry::SubMeshLodGeometryLink,std::allocator<Ogre::StaticGeometry::SubMeshLodGeometryLink> >"
              : "VectorOfSubMeshLodGeometryLink"
            , "vector<Ogre::HardwareBuffer::Usage,std::allocator<Ogre::HardwareBuffer::Usage> >"
              : "VectorOfHardwareBufferUsage"
            , "vector<const Ogre::Image*,std::allocator<const Ogre::Image*> >"
              : "VectorOfImagePtr"
              
## AJM removed these as exposing them in python_ogre caused them to disappear?              
#             , "set<Ogre::Entity*,std::less<Ogre::Entity*>,std::allocator<Ogre::Entity*> >"
#               : "SetOfEntityPtr"
#             , "vector<Ogre::RenderWindow*,std::allocator<Ogre::RenderWindow*> >"
#               : "VectorOfRenderWindowPtr"
#                , "multimap<Ogre::Vector3,Ogre::Vector3,std::less<Ogre::Vector3>,std::allocator<std::pair<const Ogre::Vector3, Ogre::Vector3> > >"
#                 : "Vector3Multimap"    

             , "vector<Ogre::Vector4, std::allocator<Ogre::Vector4> >"
               : "VectorOfVector4"
            , "vector<unsigned short, std::allocator<unsigned short> >"
              : "VectorOfUnsignedShort"
#             ,"map<std::string, Ogre::MovableObject*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > >"
# 		        : "MovableObjectMapIterator"
		     , "MapIterator<std::map<std::string, Ogre::MovableObject*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > > >"
		        : "MovableObjectMapIterator"
               , "map<Ogre::Vector3, unsigned, Ogre::EdgeListBuilder::vectorLess, std::allocator<std::pair<Ogre::Vector3 const, unsigned> > >"
                : "Vector3EdgeMap"
              , "map<unsigned, Ogre::Vector3, std::less<unsigned>, std::allocator<std::pair<unsigned const, Ogre::Vector3> > >"
                : "Vector3Map"    
   
              
              
        }
    else: #1.2
        return {
          "map<std::string, bool, std::less<std::string>, std::allocator<std::pair<std::string const, bool> > >"
          : "MapOfStringToBool"
        , "map<std::string, Ogre::OverlayElement*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::OverlayElement*> > >"
          : "MapOfStringToOverlayElement"
        , "map<std::string, Ogre::MovableObject*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > >"
          : "MapOfStringToMovableObject"
        , "map<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >"
          : "MapOfStringToString"
        , "map<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >"
          : "NamedValuePairList"
        , "vector<float, std::allocator<float> >"
          : "VectorOfFloat"
        , "vector<std::string, std::allocator<std::string> >"
          : "VectorOfString"
        , "pair<int, Ogre::CompositorInstance::RenderSystemOperation*>"
          : "PairOfIntAndRenderSystemOperationPtr"
        , "list<Ogre::Plane, std::allocator<Ogre::Plane> >"
          : "ListOfPlane"
        , "set<Ogre::SceneQuery::WorldFragmentType, std::less<Ogre::SceneQuery::WorldFragmentType>, std::allocator<Ogre::SceneQuery::WorldFragmentType> >"
          : "SetOfWorldFragmentTypes"
        , "set<std::string, std::less<std::string>, std::allocator<std::string> >"
          : "SetOfStrings"
        , "map<std::string, Ogre::OverlayElement*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::OverlayElement*> > >"
          : "MapOfOverlayElement"
        , "MapIterator<std::map<std::string, Ogre::OverlayElement*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::OverlayElement*> > > >"
          : "MapIteratorOfOverlayElement"
        , "Controller<float>"
          : "FloatController"
        , "ControllerFunction<float>"
          : "FloatControllerFunction"
        , "ControllerValue<float>"
          : "FloatControllerValue"
        , "TRect<float>"
          : "FloatRect"
        , "map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >,Ogre::_ConfigOption,std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >,std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::_ConfigOption> > >"
          : "MapOfStringToConfigOption"
        , "vector<Ogre::StaticGeometry::SubMeshLodGeometryLink,std::allocator<Ogre::StaticGeometry::SubMeshLodGeometryLink> >"
          : "VectorOfSubMeshLodGeometryLink"
        , "vector<Ogre::HardwareBuffer::Usage,std::allocator<Ogre::HardwareBuffer::Usage> >"
          : "VectorOfHardwareBufferUsage"
        , "vector<const Ogre::Image*,std::allocator<const Ogre::Image*> >"
          : "VectorOfImagePtr"
        , "set<Ogre::Entity*,std::less<Ogre::Entity*>,std::allocator<Ogre::Entity*> >"
          : "SetOfEntityPtr"
    }

def header_files( version ):
    if version == "CVS":
        return [ 'Ogre.h'
                , 'OgreErrorDialog.h'
                , 'OgreConfigDialog.h'
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
                , 'py_shared_ptr.h'
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
    if version == "CVS":
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
