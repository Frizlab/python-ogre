typedef Ogre::MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > > \
    MapIteratorStringString;
typedef  std::pair<std::string const, std::string> PairStringString;
typedef  std::pair<std::string const, unsigned> PairStringUnsigned;
typedef  std::pair<std::string const, std::vector<std::string, std::allocator<std::string> > > PairStringVector;
typedef  std::list<std::pair<std::string const, unsigned short>, std::allocator<std::pair<std::string const, unsigned short> > > \
    ListStringShort;
typedef  std::set<std::string, std::less<std::string>, std::allocator<std::string> > \
    SetStringString;
typedef  std::map<std::string, bool, std::less<std::string>, std::allocator<std::pair<std::string const, bool> > > \
    MapStringBool;
typedef  std::vector<std::string, std::allocator<std::string> > VectorString;
typedef  std::vector<Ogre::ParticleEmitter*, std::allocator<Ogre::ParticleEmitter*> > VectorParticleEmitterPtr;
typedef  std::vector<float, std::allocator<float> > VectorFloat;
typedef  std::vector<Ogre::Node*, std::allocator<Ogre::Node*> > VectorNodePtr;
typedef  std::vector<Ogre::Vector4, std::allocator<Ogre::Vector4> > VectorVector4;
typedef  std::vector<unsigned short, std::allocator<unsigned short> > VectorUnsignedShort;
typedef  std::vector<Ogre::DynLib*, std::allocator<Ogre::DynLib*> > VectorDynLibPtr;
typedef  std::vector<std::pair<std::string, std::string>, std::allocator<std::pair<std::string, std::string> > > \
    VectorPairString;
typedef  std::vector<Ogre::Quaternion, std::allocator<Ogre::Quaternion> >  VectorQuaternion;
typedef  std::pair<unsigned, unsigned> PairUnsigned;
typedef  std::pair<bool, float> PairBoolFloat;
typedef  AlignedAllocator<Ogre::Vector4, 0> AlignedAllocatorVector40;
typedef  VertexBoneAssignment_s VertexBoneAssignment_s;
typedef  Singleton<Ogre::CompositorManager> SingletonCompositorManager;
typedef  Singleton<Ogre::SkeletonManager> SingletonSkeletonManager;
typedef  Singleton<Ogre::Root> SingletonRoot;
typedef  Singleton<Ogre::SceneManagerEnumerator> SingletonSceneManagerEnumerator;
typedef  Singleton<Ogre::ShadowTextureManager> SingletonShadowTextureManager;
typedef  Singleton<Ogre::ResourceBackgroundQueue> SingletonResourceBackgroundQueue;
typedef  Singleton<Ogre::TextureManager> SingletonTextureManager;
typedef  Singleton<Ogre::Profiler> SingletonProfiler;
typedef  Singleton<Ogre::ParticleSystemManager> SingletonParticleSystemManager;
typedef  Singleton<Ogre::OverlayManager> SingletonOverlayManager;
typedef  Singleton<Ogre::MeshManager> SingletonMeshManager;
typedef  Singleton<Ogre::MaterialManager> SingletonMaterialManager;
typedef  Singleton<Ogre::LogManager> SingletonLogManager;
typedef  Singleton<Ogre::HighLevelGpuProgramManager> SingletonHighLevelGpuProgramManager;
typedef  Singleton<Ogre::GpuProgramManager> SingletonGpuProgramManager;
typedef  Singleton<Ogre::HardwareBufferManager> SingletonHardwareBufferManager;
typedef  Singleton<Ogre::ControllerManager> SingletonControllerManager;
typedef  Singleton<Ogre::ArchiveManager> SingletonArchiveManager;
typedef  Singleton<Ogre::ResourceGroupManager> SingletonResourceGroupManager;
typedef  TRect<float> TRectFloat;
typedef  ExceptionCodeType<2> ExceptionCodeType_less_2;
typedef  ExceptionCodeType<5> ExceptionCodeType_less_5;
typedef  ExceptionCodeType<9> ExceptionCodeType_less_9;
typedef  ExceptionCodeType<8> ExceptionCodeType_less_8;
typedef  ExceptionCodeType<3> ExceptionCodeType_less_3;
typedef  ExceptionCodeType<7> ExceptionCodeType_less_7;
typedef  ExceptionCodeType<4> ExceptionCodeType_less_4;
typedef  ExceptionCodeType<1> ExceptionCodeType_less_1;
typedef  ExceptionCodeType<0> ExceptionCodeType_less_0;
typedef  ExceptionCodeType<6> ExceptionCodeType_less_6;
typedef  MapIterator<std::map<std::string, Ogre::OverlayElement*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::OverlayElement*> > > > \
     MapIteratorOverlayElementPtr;
typedef  MapIterator<std::map<std::string, Ogre::MovableObject*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > > > \
     MapIteratorMovableObjectPtr;   
typedef  ControllerValue<float> ControllerValueFloat;
typedef  ControllerFunction<float> ControllerFunctionFloat;
typedef  Controller<float> ControllerFloat;
typedef  std::list<std::pair<std::string const, Ogre::MovableObject*>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > > \
    ListStringMovableObject;
typedef  std::list<std::pair<std::string const, Ogre::Node*>, std::allocator<std::pair<std::string const, Ogre::Node*> > > \
    ListStringNode;
typedef  std::list<Ogre::Particle*, std::allocator<Ogre::Particle*> > \
    ListParticle;
typedef  std::list<Ogre::Plane, std::allocator<Ogre::Plane> > ListPlane;
typedef  std::list<Ogre::ParticleEmitter*, std::allocator<Ogre::ParticleEmitter*> > ListParticleEmitter;
typedef  std::list<Ogre::Billboard*, std::allocator<Ogre::Billboard*> > ListBillboard;
typedef  std::list<std::pair<std::string const, Ogre::SharedPtr<Ogre::Resource> >, std::allocator<std::pair<std::string const, Ogre::SharedPtr<Ogre::Resource> > > > \
    ListPairStringResource;
typedef  std::set<Ogre::FrameListener*, std::less<Ogre::FrameListener*>, std::allocator<Ogre::FrameListener*> > \
    SetFrameListener;
typedef  std::set<Ogre::SceneQuery::WorldFragmentType, std::less<Ogre::SceneQuery::WorldFragmentType>, std::allocator<Ogre::SceneQuery::WorldFragmentType> > \
    SetSceneQueryWorldFragmentType;
typedef  std::map<std::string, Ogre::Archive*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::Archive*> > > \
    MapStringArchivePtr;
typedef  std::map<std::string, Ogre::OverlayElement*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::OverlayElement*> > > \
    MapStringOverlayElementPtr;
typedef  std::map<std::string, Ogre::MovableObject*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > > \
    MapStringMovableObjectPtr;
typedef  std::pair<std::string const, Ogre::ParamDictionary> PairStringParamDictionary;
typedef  std::pair<unsigned short const, Ogre::RenderPriorityGroup*> PairRenderPriorityGroupPtr;
typedef  std::pair<std::string const, Ogre::ParamCommand*> PairParamCommandPtr;
typedef  std::pair<unsigned const, Ogre::Vector4> PairUnsignedVector4;
typedef  std::pair<std::string const, Ogre::Archive*> PairStringArchivePtr;
typedef  std::pair<std::string const, Ogre::Animation*> PairStringAnimationPtr;
typedef  std::pair<std::string const, Ogre::Camera*> PairStringCameraPtr;
typedef  std::pair<std::string const, Ogre::RenderTarget*> PairStringRenderTargetPtr;
typedef  std::pair<std::string const, Ogre::ParticleSystem*> PairStringParticleSystemPtr;
typedef  std::pair<std::string const, Ogre::OverlayElement*> PairStringOverlayElementPtr;
typedef  std::pair<unsigned long const, Ogre::SharedPtr<Ogre::Resource> > PairUnsignedLongResource;
typedef  std::pair<std::string const, Ogre::ResourceManager*> PairStringResourceManagerPtr;
typedef  std::pair<unsigned short const, Ogre::VertexAnimationTrack*> PairUnsignedShortVertexAnimationTrackPtr;
typedef  std::pair<unsigned short const, Ogre::NumericAnimationTrack*> PairUnsignedShortNumericAnimationTrackPtr;
typedef  std::pair<unsigned short const, Ogre::NodeAnimationTrack*> PairUnsignedShortNodeAnimationTrackPtr;
typedef  std::pair<Ogre::MovableObject*, Ogre::MovableObject*> PairMovableObjectPtr;
typedef  std::pair<Ogre::MovableObject*, Ogre::SceneQuery::WorldFragment*> PairMovableObjectPtrWorldFragmentPtr;
typedef  std::pair<int, Ogre::CompositorInstance::RenderSystemOperation*> PairIntRenderSystemOperationPtr;
typedef  std::pair<std::string const, bool (*)(std::string&, Ogre::MaterialScriptContext&)> PairStringMaterialScriptContext;
typedef  std::pair<std::string const, Ogre::ArchiveFactory*> PairStringArchiveFactoryPtr;
typedef  std::map<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > > \
    MapStringString;
typedef  Ogre::RadixSort<std::vector<Ogre::RenderablePass, std::allocator<Ogre::RenderablePass> >,Ogre::RenderablePass,float>
   RadixSortRenderablePassFloat;
typedef RadixSort<std::vector<Ogre::RenderablePass, std::allocator<Ogre::RenderablePass> >, Ogre::RenderablePass, unsigned> \
    RadixSortRenderablePassUInt;
typedef RadixSort<std::list<Ogre::Particle*, std::allocator<Ogre::Particle*> >, Ogre::Particle*, float> \
    RadixSortParticleFloat;
typedef RadixSort<std::list<Ogre::Billboard*, std::allocator<Ogre::Billboard*> >, Ogre::Billboard*, float> \
    RadixSortBillboardFloat;
typedef Ogre::RadixSort<std::vector<Ogre::RenderablePass, std::allocator<Ogre::RenderablePass> >,Ogre::RenderablePass,float> \
    RadixSortVectorRenderablePass;
typedef ::Ogre::VectorIterator<std::vector<Ogre::CompositionPass*, std::allocator<Ogre::CompositionPass*> > > \
    VectorIteratorCompositionPassPtr;
typedef ::Ogre::VectorIterator<std::vector<Ogre::Pass*, std::allocator<Ogre::Pass*> > > \
    VectorIteratorPassPtr;
typedef ::Ogre::VectorIterator<std::vector<Ogre::Technique*, std::allocator<Ogre::Technique*> > > \  
    VectorIteratorTechniquePtr;
typedef::Ogre::VectorIterator<std::vector<Ogre::CompositionTechnique*, std::allocator<Ogre::CompositionTechnique*> > >\
    VectorIteratorCompositionTechniquePtr; 
        