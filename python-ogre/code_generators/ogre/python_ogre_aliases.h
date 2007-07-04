typedef Ogre::SharedPtr<Ogre::AnimableValue> AnimableValuePtr;
typedef Ogre::SharedPtr<Ogre::FileInfoList> FileInfoListPtr;
//typedef Ogre::SharedPtr<Ogre::CodecData> CodecDataPtr;
typedef Ogre::SharedPtr<Ogre::ControllerFunction<Ogre::Real> > ControllerFunctionRealPtr;
typedef Ogre::SharedPtr<Ogre::ControllerValue<Ogre::Real> > ControllerValueRealPtr;
typedef Ogre::SharedPtr<Ogre::MemoryDataStream> MemoryDataStreamPtr;
typedef Ogre::SharedPtr<Ogre::DataStream> DataStreamPtr;
typedef Ogre::SharedPtr<Ogre::DataStreamList> DataStreamListPtr;
typedef Ogre::SharedPtr<Ogre::GpuProgramParameters> GpuProgramParametersSharedPtr;
// typedef std::multimap<Ogre::HardwareVertexBuffer*, Ogre::VertexBufferBinding::HardwareVertexBufferSharedPtr> FreeTemporaryVertexBufferMap;
//typedef std::map<unsigned short, Ogre::VertexBufferBinding::HardwareVertexBufferSharedPtr> VertexBufferBindingMap;
typedef Ogre::SharedPtr<Ogre::Resource> ResourcePtr;

typedef Ogre::SharedPtr<Ogre::ShadowCameraSetup> ShadowCameraSetupPtr;
typedef Ogre::SharedPtr<Ogre::DefaultShadowCameraSetup> DefaultShadowCameraSetupPtr;
typedef Ogre::SharedPtr<Ogre::FocusedShadowCameraSetup> FocusedShadowCameraSetupPtr;
typedef Ogre::SharedPtr<Ogre::LiSPSMShadowCameraSetup> LiSPSMShadowCameraSetupPtr;
typedef Ogre::SharedPtr<Ogre::PlaneOptimalShadowCameraSetup> PlaneOptimalShadowCameraSetupPtr;

typedef Ogre::SharedPtr<Ogre::StringVector> StringVectorPtr;

typedef std::list<Ogre::Particle*> ParticleList;
typedef std::list<Ogre::ParticleEmitter*> ParticleEmitterList;
typedef std::vector<Ogre::Particle*> ParticlePool;

typedef Ogre::MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::GpuConstantDefinition, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::GpuConstantDefinition> > > >\
	MapIteratorGpuConstantDefinition;
typedef std::vector<std::pair<unsigned, unsigned>, std::allocator<std::pair<unsigned, unsigned> > >\
	VectorUnsignedUnsigned;
		
typedef std::vector<Ogre::GpuProgramParameters::AutoConstantEntry, std::allocator<Ogre::GpuProgramParameters::AutoConstantEntry> >\
	VectorAutoConstantEntry;
	
typedef Ogre::ResourceManager::ResourceCreateOrRetrieveResult ResourceCreateOrRetrieveResult;

typedef Ogre::FactoryObj<Ogre::Archive> ArchiveFactoryObj;
typedef Ogre::FactoryObj<Ogre::ParticleSystemRenderer> ParticleSystemRendererFactoryObj;

// these are uglys to remove messages in the generate_code output - doesn't impact the generated code :)
typedef Ogre::MapIterator<std::map<short unsigned int, Ogre::VertexAnimationTrack*, std::less<short unsigned int>, std::allocator<std::pair<const short unsigned int, Ogre::VertexAnimationTrack*> > > >\
    MapIteratorVertexAnimationTrackPtr;
typedef Ogre::MapIterator<std::map<short unsigned int, Ogre::NumericAnimationTrack*, std::less<short unsigned int>, std::allocator<std::pair<const short unsigned int, Ogre::NumericAnimationTrack*> > > >\
    MapIteratorNumericAnimationTrackPtr;
typedef Ogre::MapIterator<std::map<short unsigned int, Ogre::NodeAnimationTrack*, std::less<short unsigned int>, std::allocator<std::pair<const short unsigned int, Ogre::NodeAnimationTrack*> > > >\
    MapIteratorNodeAnimationTrackPtr;
typedef Ogre::ConstMapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::GpuConstantDefinition, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::GpuConstantDefinition> > > >\
    ConstMapIteratorGpuConstantDef;
typedef Ogre::MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObjectFactory*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObjectFactory*> > > >\
    MapIteratorMovableObjectFactory;
typedef Ogre::MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > >\
    MapIteratorString;
    
typedef std::map<unsigned short, unsigned short> MapShortShort;
typedef std::set<Ogre::SceneQuery::WorldFragmentType> SupportedWorldFragments;
typedef std::vector<std::pair<Ogre::String, Ogre::String> > customParameters;
typedef std::list<Ogre::Plane> StdListPlane;
typedef std::list<Ogre::Particle*> StdListParticlePtr;
typedef std::pair<bool, float> StdPairBoolFloat;
typedef std::pair<unsigned, unsigned> StdPairUnsignedUnsigned;
typedef Ogre::AlignedAllocator<Ogre::Vector4, 0> AlignedAllocatorVector40;
typedef Ogre::AlignedAllocator<Ogre::Vector4, 0>::rebind<Ogre::Vector4> AlignedAllocatorVector40Rebind;

typedef std::vector<Ogre::Vector4> VectorVector4;
typedef Ogre::MapIterator<Ogre::VertexAnimationTrack*> MapIVertexAnimationTrackPtr;
typedef Ogre::MapIterator<Ogre::NumericAnimationTrack*> MapINumericAnimationTrackPtr;
typedef Ogre::MapIterator<Ogre::NodeAnimationTrack*> MapINodeAnimationTrackPtr;
typedef Ogre::MapIterator<Ogre::MovableObject*> MapIMovableObjectPtr;
typedef std::map<Ogre::String, Ogre::MovableObjectFactory*> MovableObjectFactoryMap;

// these could be simpler except parts of the 'simple' version are protected in the contex
typedef Ogre::RadixSort<std::vector<Ogre::RenderablePass>,Ogre::RenderablePass,unsigned int> RadixSortRenerablePassUnsigned;
typedef Ogre::RadixSort<std::vector<Ogre::RenderablePass>,Ogre::RenderablePass,float> RadixSortRenerablePassFloat;
typedef Ogre::RadixSort<std::list<Ogre::Particle*>,Ogre::Particle*,float> RadixSortParticlePtrFloat;
typedef Ogre::RadixSort<std::list<Ogre::Billboard*>,Ogre::Billboard*,float> RadixSortBillboardPtrFloat;

// typedef Ogre::Any::holder<Ogre::UserDefinedObject*> AnyUserDefinedObjectPtr; // protected so can't tweak
typedef Ogre::ExceptionCodeType<0>  ExceptionCodeType0;
typedef Ogre::ExceptionCodeType<1>  ExceptionCodeType1;
typedef Ogre::ExceptionCodeType<2>  ExceptionCodeType2;
typedef Ogre::ExceptionCodeType<3>  ExceptionCodeType3;
typedef Ogre::ExceptionCodeType<4>  ExceptionCodeType4;
typedef Ogre::ExceptionCodeType<5>  ExceptionCodeType5;
typedef Ogre::ExceptionCodeType<6>  ExceptionCodeType6;
typedef Ogre::ExceptionCodeType<7>  ExceptionCodeType7;
typedef Ogre::ExceptionCodeType<8>  ExceptionCodeType8;
typedef Ogre::ExceptionCodeType<9>  ExceptionCodeType9;

typedef Ogre::Controller<float> ControllerFloat;
typedef Ogre::ControllerValue<float> ControllerValueFloat;
typedef Ogre::ControllerFunction<float> ControllerFunctionFloat;

typedef  Ogre::Singleton<Ogre::FontManager> SingletonFontManager;
typedef  Ogre::Singleton<Ogre::ExternalTextureSourceManager> SingletonExternalTextureSourceManager;
typedef  Ogre::Singleton<Ogre::DynLibManager> SingletonDynLibManager;

typedef  Ogre::Singleton<Ogre::CompositorManager> SingletonCompositorManager;
typedef  Ogre::Singleton<Ogre::SkeletonManager> SingletonSkeletonManager;
typedef  Ogre::Singleton<Ogre::Root> SingletonRoot;
typedef  Ogre::Singleton<Ogre::SceneManagerEnumerator> SingletonSceneManagerEnumerator;
typedef  Ogre::Singleton<Ogre::ShadowTextureManager> SingletonShadowTextureManager;
typedef  Ogre::Singleton<Ogre::ResourceBackgroundQueue> SingletonResourceBackgroundQueue;
typedef  Ogre::Singleton<Ogre::TextureManager> SingletonTextureManager;
typedef  Ogre::Singleton<Ogre::Profiler> SingletonProfiler;
typedef  Ogre::Singleton<Ogre::ParticleSystemManager> SingletonParticleSystemManager;
typedef  Ogre::Singleton<Ogre::OverlayManager> SingletonOverlayManager;
typedef  Ogre::Singleton<Ogre::MeshManager> SingletonMeshManager;
typedef  Ogre::Singleton<Ogre::MaterialManager> SingletonMaterialManager;
typedef  Ogre::Singleton<Ogre::LogManager> SingletonLogManager;
typedef  Ogre::Singleton<Ogre::HighLevelGpuProgramManager> SingletonHighLevelGpuProgramManager;
typedef  Ogre::Singleton<Ogre::GpuProgramManager> SingletonGpuProgramManager;
typedef  Ogre::Singleton<Ogre::HardwareBufferManager> SingletonHardwareBufferManager;
typedef  Ogre::Singleton<Ogre::ControllerManager> SingletonControllerManager;
typedef  Ogre::Singleton<Ogre::ArchiveManager> SingletonArchiveManager;
typedef  Ogre::Singleton<Ogre::ResourceGroupManager> SingletonResourceGroupManager;
typedef  Ogre::Singleton<Ogre::ArchiveManager> SingletonArchiveManager;
typedef  Ogre::RenderSystem* RenderSystemPtr;

typedef Ogre::RenderSystem RenderSystem;
typedef Ogre::ConvexBody::PolygonList PolygonList;
typedef Ogre::Polygon::VertexList VertexList;
typedef Ogre::Polygon::EdgeMap EdgeMap;
typedef Ogre::Polygon::Edge Edge;
typedef Ogre::WindowEventUtilities::WindowEventListeners WindowEventListeners;
typedef Ogre::WindowEventUtilities::Windows Windows;
typedef Ogre::RenderSystemList RenderSystemList;
typedef Ogre::SceneTypeMask SceneTypeMask;
typedef Ogre::ConstShadowTextureConfigIterator ConstShadowTextureConfigIterator;
typedef Ogre::ShadowTextureConfigList ShadowTextureConfigList;
typedef Ogre::ShadowTextureList ShadowTextureList;
typedef Ogre::SceneQueryMovableWorldFragmentIntersectionList SceneQueryMovableWorldFragmentIntersectionList;
typedef Ogre::SceneQueryMovableIntersectionList SceneQueryMovableIntersectionList;
typedef Ogre::SceneQueryMovableObjectWorldFragmentPair SceneQueryMovableObjectWorldFragmentPair;
typedef Ogre::SceneQueryMovableObjectPair SceneQueryMovableObjectPair;
typedef Ogre::RaySceneQueryResult RaySceneQueryResult;
typedef Ogre::SceneQueryResultWorldFragmentList SceneQueryResultWorldFragmentList;
typedef Ogre::SceneQueryResultMovableList SceneQueryResultMovableList;
typedef Ogre::BackgroundProcessTicket BackgroundProcessTicket;
typedef Ogre::RenderTargetPriorityMap RenderTargetPriorityMap;
typedef Ogre::RenderTargetMap RenderTargetMap;
typedef Ogre::ConfigOptionMap ConfigOptionMap;
typedef Ogre::ConfigOption ConfigOption;
//typedef Ogre::_ConfigOption _ConfigOption;

typedef Ogre::RenderQueueInvocationIterator RenderQueueInvocationIterator;
typedef Ogre::RenderQueueInvocationList RenderQueueInvocationList;
typedef Ogre::IlluminationPassList IlluminationPassList;
typedef Ogre::ATTRIBUTE_PARSER ATTRIBUTE_PARSER;
typedef Ogre::ShadowCameraSetupPtr ShadowCameraSetupPtr;
typedef Ogre::VertexBoneAssignment VertexBoneAssignment;
typedef Ogre::ControllerFunctionRealPtr ControllerFunctionRealPtr;
typedef Ogre::ControllerValueRealPtr ControllerValueRealPtr;
typedef Ogre::ConstImagePtrList ConstImagePtrList;
typedef Ogre::ImagePtrList ImagePtrList;
typedef Ogre::ConfigFile::SettingsMultiMap SettingsMultiMap;
typedef Ogre::ConfigFile::SettingsIterator SettingsIterator;
typedef Ogre::ConfigFile::SettingsBySection SettingsBySection;
typedef Ogre::ConfigFile::SectionIterator SectionIterator;
typedef Ogre::PlaneBoundedVolumeList PlaneBoundedVolumeList;

// typedef Ogre::GpuProgramParametersSharedPtr GpuProgramParametersSharedPtr;  // no longer in source
// typedef Ogre::GpuProgramParameters::RealConstantIterator RealConstantIterator;
//typedef Ogre::GpuProgramParameters::IntConstantIterator IntConstantIterator;
typedef Ogre::GpuProgramParameters::AutoConstantIterator AutoConstantIterator;

typedef Ogre::BufferUsageList BufferUsageList;
typedef Ogre::ResourcePtr ResourcePtr;
typedef Ogre::ResourceHandle ResourceHandle;
typedef Ogre::ParamDictionaryMap ParamDictionaryMap;
typedef Ogre::ParamCommandMap ParamCommandMap;
typedef Ogre::ParameterList ParameterList;
typedef Ogre::Rect Rect; 
typedef Ogre::FloatRect FloatRect;
//typedef Ogre::AliasTextureNamePairList AliasTextureNamePairList;  // overlaps NameValuePairList
typedef Ogre::NameValuePairList NameValuePairList;
// typedef Ogre::BinaryOptionList BinaryOptionList; // overlaps NameValuePairList
typedef Ogre::UnaryOptionList UnaryOptionList;
typedef Ogre::LightList LightList;
typedef Ogre::TrackVertexColourType TrackVertexColourType;
typedef Ogre::FileInfoListPtr FileInfoListPtr;
typedef Ogre::FileInfoList FileInfoList;
typedef Ogre::MemoryDataStreamPtr MemoryDataStreamPtr;
typedef Ogre::DataStreamListPtr DataStreamListPtr;
typedef Ogre::DataStreamList DataStreamList;
typedef Ogre::DataStreamPtr DataStreamPtr;
typedef Ogre::ConstEnabledAnimationStateIterator ConstEnabledAnimationStateIterator;
typedef Ogre::EnabledAnimationStateList EnabledAnimationStateList;
typedef Ogre::ConstAnimationStateIterator ConstAnimationStateIterator;
typedef Ogre::AnimationStateIterator AnimationStateIterator;
typedef Ogre::AnimationStateMap AnimationStateMap;
typedef Ogre::PoseList PoseList;
typedef Ogre::VertexPoseKeyFrame::PoseRefList PoseRefList;
typedef Ogre::VertexPoseKeyFrame::PoseRefIterator PoseRefIterator;
typedef Ogre::VertexPoseKeyFrame::ConstPoseRefIterator ConstPoseRefIterator;
typedef Ogre::PlaneList PlaneList;
typedef Ogre::AnimableValuePtr AnimableValuePtr;
typedef Ogre::StringVectorPtr StringVectorPtr;
typedef Ogre::StringVector StringVector;
typedef Ogre::BGRA BGRA;
typedef Ogre::ABGR ABGR;
typedef Ogre::ARGB ARGB;
typedef Ogre::RGBA RGBA;
typedef Ogre::_StringHash _StringHash;
typedef Ogre::StringUtil::StrStreamType StrStreamType;
typedef Ogre::CompositionTargetPass::Passes Passes;
typedef Ogre::CompositionTargetPass::PassIterator CompositionTargetPassIterator;
typedef Ogre::CompositionTechnique::TargetPasses TargetPasses;
typedef Ogre::CompositionTechnique::TargetPassIterator TargetPassIterator;
typedef Ogre::CompositionTechnique::TextureDefinitions TextureDefinitions;
typedef Ogre::CompositionTechnique::TextureDefinitionIterator TextureDefinitionIterator;
typedef Ogre::CompositorInstance::TargetOperation::RenderQueueBitSet RenderQueueBitSet;
typedef Ogre::CompositorInstance::QuadMaterialMap QuadMaterialMap;
typedef Ogre::CompositorInstance::RenderSystemOpPair RenderSystemOpPair;
typedef Ogre::CompositorInstance::RenderSystemOpPairs RenderSystemOpPairs;
typedef Ogre::CompositorInstance::CompiledState CompiledState;
typedef Ogre::CompositorChain::Instances CompositorChainInstances;
typedef Ogre::CompositorChain::InstanceIterator InstanceIterator;
typedef Ogre::Compositor::Techniques Techniques;
typedef Ogre::Compositor::TechniqueIterator CompositorTechniqueIterator;
typedef Ogre::VertexDeclaration::VertexElementList VertexElementList;
typedef Ogre::VertexData::HardwareAnimationDataList HardwareAnimationDataList;
typedef Ogre::VertexBufferBinding::VertexBufferBindingMap VertexBufferBindingMap;
typedef Ogre::TextureUnitState::EffectMap EffectMap;
typedef Ogre::Technique::PassIterator TechniquePassIterator;
typedef Ogre::Technique::IlluminationPassIterator IlluminationPassIterator;
typedef Ogre::SubMesh::IndexMap IndexMap;
typedef Ogre::SubMesh::VertexBoneAssignmentList VertexBoneAssignmentList;
typedef Ogre::SubMesh::BoneAssignmentIterator BoneAssignmentIterator;
typedef Ogre::SubMesh::AliasTextureIterator AliasTextureIterator;


typedef Ogre::StaticGeometry::OptimisedSubMeshGeometryList OptimisedSubMeshGeometryList;
typedef Ogre::StaticGeometry::SubMeshLodGeometryLinkList SubMeshLodGeometryLinkList;
typedef Ogre::StaticGeometry::SubMeshLodGeometryLink SubMeshLodGeometryLink;
typedef Ogre::StaticGeometry::SubMeshGeometryLookup SubMeshGeometryLookup;
typedef Ogre::StaticGeometry::QueuedSubMeshList QueuedSubMeshList;
typedef Ogre::StaticGeometry::QueuedGeometryList QueuedGeometryList;
typedef Ogre::StaticGeometry::LODBucket::MaterialBucketMap MaterialBucketMap;
typedef Ogre::StaticGeometry::LODBucket::MaterialIterator MaterialIterator;
typedef Ogre::StaticGeometry::MaterialBucket::GeometryBucketList GeometryBucketList;
typedef Ogre::StaticGeometry::MaterialBucket::GeometryIterator GeometryIterator;
typedef Ogre::StaticGeometry::Region::LODBucketList LODBucketList;
typedef Ogre::StaticGeometry::Region::LODIterator LODIterator;
typedef std::map<Ogre::uint32, Ogre::StaticGeometry::Region * > RegionMap;
//typedef Ogre::StaticGeometry::RegionMap RegionMap;
typedef Ogre::StaticGeometry::RegionIterator RegionIterator;

typedef Ogre::InstancedGeometry::OptimisedSubMeshGeometryList InstOptimisedSubMeshGeometryList;
typedef Ogre::InstancedGeometry::SubMeshLodGeometryLinkList InstSubMeshLodGeometryLinkList;
typedef Ogre::InstancedGeometry::SubMeshLodGeometryLink InstSubMeshLodGeometryLink;
typedef Ogre::InstancedGeometry::SubMeshGeometryLookup InstSubMeshGeometryLookup;
typedef Ogre::InstancedGeometry::QueuedSubMeshList InstQueuedSubMeshList;
typedef Ogre::InstancedGeometry::QueuedGeometryList InstQueuedGeometryList;

// typedef Ogre::InstancedGeometry::QueuedSubMeshOriginList QueuedSubMeshOriginListI;  // StringVector overlap
typedef Ogre::InstancedGeometry::InstancedObject::GeometryBucketList InstGeometryBucketList;
typedef Ogre::InstancedGeometry::MaterialBucket::GeometryIterator InstGeometryIterator;
typedef Ogre::InstancedGeometry::LODBucket::MaterialBucketMap InstMaterialBucketMap;
typedef Ogre::InstancedGeometry::LODBucket::MaterialIterator InstMaterialIterator;
typedef Ogre::InstancedGeometry::BatchInstance::LODBucketList InstLODBucketList;
typedef Ogre::InstancedGeometry::BatchInstance::ObjectsMap InstObjectsMap;
typedef Ogre::InstancedGeometry::BatchInstance::LODIterator InstLODIterator;
typedef Ogre::InstancedGeometry::BatchInstanceMap InstBatchInstanceMap;
typedef Ogre::InstancedGeometry::RenderOperationVector InstRenderOperationVector;
typedef Ogre::InstancedGeometry::BatchInstanceIterator InstBatchInstanceIterator;

typedef Ogre::Skeleton::BoneList BoneList;
typedef Ogre::Skeleton::BoneIterator BoneIterator;
typedef Ogre::Skeleton::LinkedSkeletonAnimSourceList LinkedSkeletonAnimSourceList;
typedef Ogre::Skeleton::LinkedSkeletonAnimSourceIterator LinkedSkeletonAnimSourceIterator;
//typedef Ogre::Skeleton::BoneHandleMap BoneHandleMap; //overlaps IndexMap
typedef Ogre::ShadowCaster::ShadowRenderableList ShadowRenderableList;
typedef Ogre::ShadowCaster::ShadowRenderableListIterator ShadowRenderableListIterator;
typedef Ogre::SceneNode::ObjectMap ObjectMap;
typedef Ogre::SceneNode::ObjectIterator ObjectIterator;
typedef Ogre::SceneNode::ConstObjectIterator ConstObjectIterator;
typedef Ogre::SceneManagerEnumerator::Instances SceneManagerEnumeratorInstances;
typedef Ogre::SceneManagerEnumerator::MetaDataList MetaDataList;
typedef Ogre::SceneManagerEnumerator::MetaDataIterator MetaDataIterator;
typedef Ogre::SceneManagerEnumerator::SceneManagerIterator SceneManagerIterator;
typedef Ogre::SceneManager::CameraIterator CameraIterator;
typedef Ogre::SceneManager::AnimationIterator AnimationIterator;
typedef Ogre::SceneManager::MovableObjectIterator MovableObjectIterator;
typedef Ogre::Root::MovableObjectFactoryIterator MovableObjectFactoryIterator;
typedef Ogre::RibbonTrail::NodeList NodeList;
typedef Ogre::RibbonTrail::NodeIterator NodeIterator;
typedef Ogre::ResourceManager::ResourceMap ResourceMap;
typedef Ogre::ResourceManager::ResourceHandleMap ResourceHandleMap;
typedef Ogre::ResourceManager::ResourceMapIterator ResourceMapIterator;
typedef Ogre::ResourceGroupManager::ResourceDeclarationList ResourceDeclarationList;
typedef Ogre::ResourceGroupManager::ResourceManagerMap ResourceManagerMap;
typedef Ogre::ResourceGroupManager::ResourceManagerIterator ResourceManagerIterator;
typedef Ogre::RenderSystem::RenderTargetIterator RenderTargetIterator;
typedef Ogre::RenderQueueGroup::PriorityMap PriorityMap;
typedef Ogre::RenderQueueGroup::PriorityMapIterator PriorityMapIterator;
typedef Ogre::RenderQueue::RenderQueueGroupMap RenderQueueGroupMap;
typedef Ogre::RenderQueue::QueueGroupIterator QueueGroupIterator;
typedef Ogre::ProgressiveMesh::LODFaceList LODFaceList;
typedef Ogre::Pose::VertexOffsetMap VertexOffsetMap;
typedef Ogre::Pose::VertexOffsetIterator VertexOffsetIterator;
typedef Ogre::Pose::ConstVertexOffsetIterator ConstVertexOffsetIterator;
typedef Ogre::PlaneBoundedVolume::PlaneList PlaneList;
typedef Ogre::Pass::PassSet PassSet;
typedef Ogre::Pass::TextureUnitStateIterator TextureUnitStateIterator;
typedef Ogre::Pass::ConstTextureUnitStateIterator ConstTextureUnitStateIterator;
typedef Ogre::ParticleSystemManager::ParticleTemplateMap ParticleTemplateMap;
typedef Ogre::ParticleSystemManager::ParticleAffectorFactoryMap ParticleAffectorFactoryMap;
typedef Ogre::ParticleSystemManager::ParticleEmitterFactoryMap ParticleEmitterFactoryMap;
typedef Ogre::ParticleSystemManager::ParticleSystemRendererFactoryMap ParticleSystemRendererFactoryMap;
typedef Ogre::ParticleSystemManager::ParticleAffectorFactoryIterator ParticleAffectorFactoryIterator;
typedef Ogre::ParticleSystemManager::ParticleEmitterFactoryIterator ParticleEmitterFactoryIterator;
typedef Ogre::ParticleSystemManager::ParticleRendererFactoryIterator ParticleRendererFactoryIterator;
typedef Ogre::ParticleSystemManager::ParticleSystemTemplateIterator ParticleSystemTemplateIterator;



typedef Ogre::OverlayManager::OverlayMap OverlayMap;
typedef Ogre::OverlayManager::ElementMap ElementMap;
typedef Ogre::OverlayManager::OverlayMapIterator OverlayMapIterator;
// typedef Ogre::OverlayManager::TemplateIterator TemplateIterator; //overlaps ChildIterator
typedef Ogre::OverlayContainer::ChildMap ChildMap;
typedef Ogre::OverlayContainer::ChildIterator ChildIterator;
typedef Ogre::OverlayContainer::ChildContainerMap ChildContainerMap;
typedef Ogre::OverlayContainer::ChildContainerIterator ChildContainerIterator;
typedef Ogre::Overlay::OverlayContainerList OverlayContainerList;
typedef Ogre::Overlay::Overlay2DElementsIterator Overlay2DElementsIterator;
typedef Ogre::Node::ChildNodeMap ChildNodeMap;
typedef Ogre::Node::ChildNodeIterator ChildNodeIterator;
typedef Ogre::Node::ConstChildNodeIterator ConstChildNodeIterator;
typedef Ogre::Mesh::LodDistanceList LodDistanceList;
typedef Ogre::Mesh::VertexBoneAssignmentList VertexBoneAssignmentList;
typedef Ogre::Mesh::BoneAssignmentIterator BoneAssignmentIterator;
typedef Ogre::Mesh::SubMeshList SubMeshList;
typedef Ogre::Mesh::IndexMap IndexMap;
typedef Ogre::Mesh::SubMeshNameMap SubMeshNameMap;
typedef Ogre::Mesh::SubMeshIterator SubMeshIterator;
typedef Ogre::Mesh::PoseIterator PoseIterator;
typedef Ogre::Mesh::ConstPoseIterator ConstPoseIterator;
typedef Ogre::Material::LodDistanceList LodDistanceList;
typedef Ogre::Material::LodDistanceIterator LodDistanceIterator;
typedef Ogre::Material::TechniqueIterator MaterialTechniqueIterator;
typedef Ogre::ManualObject::SectionList SectionList;
//typedef Ogre::Image::Box ImagBox; //overlaps Box
//typedef Ogre::Image::Rect ImagRect; // overlaps Rect
typedef Ogre::HighLevelGpuProgramManager::FactoryMap FactoryMap;
typedef Ogre::GpuProgramManager::SyntaxCodes SyntaxCodes;
typedef Ogre::Entity::EntitySet EntitySet;
typedef Ogre::Entity::ChildObjectList ChildObjectList;
//typedef Ogre::Entity::ChildObjectListIterator ChildObjectListIterator; //overlaps MovableObjectIterator
typedef Ogre::EdgeData::TriangleFaceNormalList TriangleFaceNormalList;
typedef Ogre::EdgeData::TriangleLightFacingList TriangleLightFacingList;
typedef Ogre::EdgeData::TriangleList TriangleList;
typedef Ogre::EdgeData::EdgeList EdgeList;
typedef Ogre::EdgeData::EdgeGroupList EdgeGroupList;
typedef Ogre::BillboardChain::ElementList ElementList;
typedef Ogre::Animation::NodeTrackList NodeTrackList;
typedef Ogre::Animation::NodeTrackIterator NodeTrackIterator;
typedef Ogre::Animation::NumericTrackList NumericTrackList;
typedef Ogre::Animation::NumericTrackIterator NumericTrackIterator;
typedef Ogre::Animation::VertexTrackList VertexTrackList;
typedef Ogre::Animation::VertexTrackIterator VertexTrackIterator;
typedef Ogre::Animation::TrackHandleList TrackHandleList;
typedef Ogre::String String;



     
