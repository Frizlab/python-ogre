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


typedef ::Ogre::Controller<float> ControllerFloat;
typedef ::Ogre::ControllerValue<float> ControllerValueFloat;
typedef ::Ogre::ControllerFunction<float> ControllerFunctionFloat;
typedef  ::Ogre::Singleton<Ogre::CompositorManager> SingletonCompositorManager;
typedef  ::Ogre::Singleton<Ogre::SkeletonManager> SingletonSkeletonManager;
typedef  ::Ogre::Singleton<Ogre::Root> SingletonRoot;
typedef  ::Ogre::Singleton<Ogre::SceneManagerEnumerator> SingletonSceneManagerEnumerator;
typedef  ::Ogre::Singleton<Ogre::ShadowTextureManager> SingletonShadowTextureManager;
typedef  ::Ogre::Singleton<Ogre::ResourceBackgroundQueue> SingletonResourceBackgroundQueue;
typedef  ::Ogre::Singleton<Ogre::TextureManager> SingletonTextureManager;
typedef  ::Ogre::Singleton<Ogre::Profiler> SingletonProfiler;
typedef  ::Ogre::Singleton<Ogre::ParticleSystemManager> SingletonParticleSystemManager;
typedef  ::Ogre::Singleton<Ogre::OverlayManager> SingletonOverlayManager;
typedef  ::Ogre::Singleton<Ogre::MeshManager> SingletonMeshManager;
typedef  ::Ogre::Singleton<Ogre::MaterialManager> SingletonMaterialManager;
typedef  ::Ogre::Singleton<Ogre::LogManager> SingletonLogManager;
typedef  ::Ogre::Singleton<Ogre::HighLevelGpuProgramManager> SingletonHighLevelGpuProgramManager;
typedef  ::Ogre::Singleton<Ogre::GpuProgramManager> SingletonGpuProgramManager;
typedef  ::Ogre::Singleton<Ogre::HardwareBufferManager> SingletonHardwareBufferManager;
typedef  ::Ogre::Singleton<Ogre::ControllerManager> SingletonControllerManager;
typedef  ::Ogre::Singleton<Ogre::ArchiveManager> SingletonArchiveManager;
typedef  ::Ogre::Singleton<Ogre::ResourceGroupManager> SingletonResourceGroupManager;
typedef  ::Ogre::Singleton<Ogre::ArchiveManager> SingletonArchiveManager;

//sizeof( ControllerFloat );
typedef ::Ogre::Mesh::SubMeshNameMap SubMeshNameMap;
//sizeof( SubMeshNameMap );
typedef ::Ogre::RenderSystem RenderSystem;
//sizeof( RenderSystem );
typedef ::Ogre::ConvexBody::PolygonList PolygonList;
//sizeof( PolygonList );
typedef ::Ogre::Polygon::VertexList VertexList;
//sizeof( VertexList );
typedef ::Ogre::Polygon::EdgeMap EdgeMap;
//sizeof( EdgeMap );
typedef ::Ogre::Polygon::Edge Edge;
//sizeof( Edge );
typedef ::Ogre::WindowEventUtilities::WindowEventListeners WindowEventListeners;
//sizeof( WindowEventListeners );
typedef ::Ogre::WindowEventUtilities::Windows Windows;
//sizeof( Windows );
typedef ::Ogre::RenderSystemList RenderSystemList;
//sizeof( RenderSystemList );
typedef ::Ogre::SceneTypeMask SceneTypeMask;
//sizeof( SceneTypeMask );
typedef ::Ogre::ConstShadowTextureConfigIterator ConstShadowTextureConfigIterator;
//sizeof( ConstShadowTextureConfigIterator );
typedef ::Ogre::ShadowTextureConfigList ShadowTextureConfigList;
//sizeof( ShadowTextureConfigList );
typedef ::Ogre::ShadowTextureList ShadowTextureList;
//sizeof( ShadowTextureList );
typedef ::Ogre::SceneQueryMovableWorldFragmentIntersectionList SceneQueryMovableWorldFragmentIntersectionList;
//sizeof( SceneQueryMovableWorldFragmentIntersectionList );
typedef ::Ogre::SceneQueryMovableIntersectionList SceneQueryMovableIntersectionList;
//sizeof( SceneQueryMovableIntersectionList );
typedef ::Ogre::SceneQueryMovableObjectWorldFragmentPair SceneQueryMovableObjectWorldFragmentPair;
//sizeof( SceneQueryMovableObjectWorldFragmentPair );
typedef ::Ogre::SceneQueryMovableObjectPair SceneQueryMovableObjectPair;
//sizeof( SceneQueryMovableObjectPair );
typedef ::Ogre::RaySceneQueryResult RaySceneQueryResult;
//sizeof( RaySceneQueryResult );
typedef ::Ogre::SceneQueryResultWorldFragmentList SceneQueryResultWorldFragmentList;
//sizeof( SceneQueryResultWorldFragmentList );
typedef ::Ogre::SceneQueryResultMovableList SceneQueryResultMovableList;
//sizeof( SceneQueryResultMovableList );
typedef ::Ogre::BackgroundProcessTicket BackgroundProcessTicket;
//sizeof( BackgroundProcessTicket );
// typedef ::Ogre::DLL_CREATERENDERWINDOW DLL_CREATERENDERWINDOW;
// //sizeof( DLL_CREATERENDERWINDOW );
typedef ::Ogre::RenderTargetPriorityMap RenderTargetPriorityMap;
//sizeof( RenderTargetPriorityMap );
typedef ::Ogre::RenderTargetMap RenderTargetMap;
//sizeof( RenderTargetMap );
typedef ::Ogre::ConfigOptionMap ConfigOptionMap;
//sizeof( ConfigOptionMap );
typedef ::Ogre::ConfigOption ConfigOption;
//sizeof( ConfigOption );
typedef ::Ogre::RenderQueueInvocationIterator RenderQueueInvocationIterator;
//sizeof( RenderQueueInvocationIterator );
typedef ::Ogre::RenderQueueInvocationList RenderQueueInvocationList;
//sizeof( RenderQueueInvocationList );
typedef ::Ogre::IlluminationPassList IlluminationPassList;
//sizeof( IlluminationPassList );
typedef ::Ogre::ATTRIBUTE_PARSER ATTRIBUTE_PARSER;
//sizeof( ATTRIBUTE_PARSER );
typedef ::Ogre::ShadowCameraSetupPtr ShadowCameraSetupPtr;
//sizeof( ShadowCameraSetupPtr );
typedef ::Ogre::VertexBoneAssignment VertexBoneAssignment;
//sizeof( VertexBoneAssignment );
typedef ::Ogre::ControllerFunctionRealPtr ControllerFunctionRealPtr;
//sizeof( ControllerFunctionRealPtr );
typedef ::Ogre::ControllerValueRealPtr ControllerValueRealPtr;
//sizeof( ControllerValueRealPtr );
typedef ::Ogre::ConstImagePtrList ConstImagePtrList;
//sizeof( ConstImagePtrList );
typedef ::Ogre::ImagePtrList ImagePtrList;
//sizeof( ImagePtrList );
typedef ::Ogre::ConfigFile::SettingsMultiMap SettingsMultiMap;
//sizeof( SettingsMultiMap );
typedef ::Ogre::ConfigFile::SettingsIterator SettingsIterator;
//sizeof( SettingsIterator );
typedef ::Ogre::ConfigFile::SettingsBySection SettingsBySection;
//sizeof( SettingsBySection );
typedef ::Ogre::ConfigFile::SectionIterator SectionIterator;
//sizeof( SectionIterator );
typedef ::Ogre::PlaneBoundedVolumeList PlaneBoundedVolumeList;
//sizeof( PlaneBoundedVolumeList );
typedef ::Ogre::GpuProgramParametersSharedPtr GpuProgramParametersSharedPtr;
//sizeof( GpuProgramParametersSharedPtr );
typedef ::Ogre::GpuProgramParameters::RealConstantIterator RealConstantIterator;
//sizeof( RealConstantIterator );
typedef ::Ogre::GpuProgramParameters::IntConstantIterator IntConstantIterator;
//sizeof( IntConstantIterator );
typedef ::Ogre::GpuProgramParameters::AutoConstantIterator AutoConstantIterator;
//sizeof( AutoConstantIterator );
typedef ::Ogre::BufferUsageList BufferUsageList;
//sizeof( BufferUsageList );
typedef ::Ogre::ResourcePtr ResourcePtr;
//sizeof( ResourcePtr );
typedef ::Ogre::ResourceHandle ResourceHandle;
//sizeof( ResourceHandle );
typedef ::Ogre::ParamDictionaryMap ParamDictionaryMap;
//sizeof( ParamDictionaryMap );
typedef ::Ogre::ParamCommandMap ParamCommandMap;
//sizeof( ParamCommandMap );
typedef ::Ogre::ParameterList ParameterList;
//sizeof( ParameterList );
typedef ::Ogre::Rect Rect;
//sizeof( Rect );
typedef ::Ogre::FloatRect FloatRect;
//sizeof( FloatRect );
typedef ::Ogre::AliasTextureNamePairList AliasTextureNamePairList;
//sizeof( AliasTextureNamePairList );
typedef ::Ogre::NameValuePairList NameValuePairList;
//sizeof( NameValuePairList );
typedef ::Ogre::BinaryOptionList BinaryOptionList;
//sizeof( BinaryOptionList );
typedef ::Ogre::UnaryOptionList UnaryOptionList;
//sizeof( UnaryOptionList );
typedef ::Ogre::LightList LightList;
//sizeof( LightList );
typedef ::Ogre::TrackVertexColourType TrackVertexColourType;
//sizeof( TrackVertexColourType );
typedef ::Ogre::FileInfoListPtr FileInfoListPtr;
//sizeof( FileInfoListPtr );
typedef ::Ogre::FileInfoList FileInfoList;
//sizeof( FileInfoList );
typedef ::Ogre::MemoryDataStreamPtr MemoryDataStreamPtr;
//sizeof( MemoryDataStreamPtr );
typedef ::Ogre::DataStreamListPtr DataStreamListPtr;
//sizeof( DataStreamListPtr );
typedef ::Ogre::DataStreamList DataStreamList;
//sizeof( DataStreamList );
typedef ::Ogre::DataStreamPtr DataStreamPtr;
//sizeof( DataStreamPtr );
typedef ::Ogre::ConstEnabledAnimationStateIterator ConstEnabledAnimationStateIterator;
//sizeof( ConstEnabledAnimationStateIterator );
typedef ::Ogre::EnabledAnimationStateList EnabledAnimationStateList;
//sizeof( EnabledAnimationStateList );
typedef ::Ogre::ConstAnimationStateIterator ConstAnimationStateIterator;
//sizeof( ConstAnimationStateIterator );
typedef ::Ogre::AnimationStateIterator AnimationStateIterator;
//sizeof( AnimationStateIterator );
typedef ::Ogre::AnimationStateMap AnimationStateMap;
//sizeof( AnimationStateMap );
typedef ::Ogre::PoseList PoseList;
//sizeof( PoseList );
typedef ::Ogre::VertexPoseKeyFrame::PoseRefList PoseRefList;
//sizeof( PoseRefList );
typedef ::Ogre::VertexPoseKeyFrame::PoseRefIterator PoseRefIterator;
//sizeof( PoseRefIterator );
typedef ::Ogre::VertexPoseKeyFrame::ConstPoseRefIterator ConstPoseRefIterator;
//sizeof( ConstPoseRefIterator );
typedef ::Ogre::PlaneList PlaneList;
//sizeof( PlaneList );
typedef ::Ogre::AnimableValuePtr AnimableValuePtr;
//sizeof( AnimableValuePtr );
typedef ::Ogre::StringVectorPtr StringVectorPtr;
//sizeof( StringVectorPtr );
typedef ::Ogre::StringVector StringVector;
//sizeof( StringVector );
typedef ::Ogre::BGRA BGRA;
//sizeof( BGRA );
typedef ::Ogre::ABGR ABGR;
//sizeof( ABGR );
typedef ::Ogre::ARGB ARGB;
//sizeof( ARGB );
typedef ::Ogre::RGBA RGBA;
//sizeof( RGBA );
typedef ::Ogre::_StringHash _StringHash;
//sizeof( _StringHash );
typedef ::Ogre::StringUtil::StrStreamType StrStreamType;
//sizeof( StrStreamType );
typedef ::Ogre::CompositionTargetPass::Passes Passes;
//sizeof( Passes );
typedef ::Ogre::CompositionTargetPass::PassIterator CompositionTargetPassIterator;
//sizeof( CompositionTargetPassIterator );
typedef ::Ogre::CompositionTechnique::TargetPasses TargetPasses;
//sizeof( TargetPasses );
typedef ::Ogre::CompositionTechnique::TargetPassIterator TargetPassIterator;
//sizeof( TargetPassIterator );
typedef ::Ogre::CompositionTechnique::TextureDefinitions TextureDefinitions;
//sizeof( TextureDefinitions );
typedef ::Ogre::CompositionTechnique::TextureDefinitionIterator TextureDefinitionIterator;
//sizeof( TextureDefinitionIterator );
typedef ::Ogre::CompositorInstance::TargetOperation::RenderQueueBitSet RenderQueueBitSet;
//sizeof( RenderQueueBitSet );
typedef ::Ogre::CompositorInstance::QuadMaterialMap QuadMaterialMap;
//sizeof( QuadMaterialMap );
typedef ::Ogre::CompositorInstance::RenderSystemOpPair RenderSystemOpPair;
//sizeof( RenderSystemOpPair );
typedef ::Ogre::CompositorInstance::RenderSystemOpPairs RenderSystemOpPairs;
//sizeof( RenderSystemOpPairs );
typedef ::Ogre::CompositorInstance::CompiledState CompiledState;
//sizeof( CompiledState );
typedef ::Ogre::CompositorChain::Instances CompositorChainInstances;
//sizeof( CompositorChainInstances );
typedef ::Ogre::CompositorChain::InstanceIterator InstanceIterator;
//sizeof( InstanceIterator );
typedef ::Ogre::Compositor::Techniques Techniques;
//sizeof( Techniques );
typedef ::Ogre::Compositor::TechniqueIterator CompositorTechniqueIterator;
//sizeof( CompositorTechniqueIterator );
typedef ::Ogre::VertexDeclaration::VertexElementList VertexElementList;
//sizeof( VertexElementList );
typedef ::Ogre::VertexData::HardwareAnimationDataList HardwareAnimationDataList;
//sizeof( HardwareAnimationDataList );
typedef ::Ogre::VertexBufferBinding::VertexBufferBindingMap VertexBufferBindingMap;
//sizeof( VertexBufferBindingMap );
typedef ::Ogre::TextureUnitState::EffectMap EffectMap;
//sizeof( EffectMap );
typedef ::Ogre::Technique::PassIterator TechniquePassIterator;
//sizeof( TechniquePassIterator );
typedef ::Ogre::Technique::IlluminationPassIterator IlluminationPassIterator;
//sizeof( IlluminationPassIterator );
typedef ::Ogre::SubMesh::IndexMap IndexMap;
//sizeof( IndexMap );
typedef ::Ogre::SubMesh::VertexBoneAssignmentList VertexBoneAssignmentList;
//sizeof( VertexBoneAssignmentList );
typedef ::Ogre::SubMesh::BoneAssignmentIterator BoneAssignmentIterator;
//sizeof( BoneAssignmentIterator );
typedef ::Ogre::SubMesh::AliasTextureIterator AliasTextureIterator;
//sizeof( AliasTextureIterator );
typedef ::Ogre::StaticGeometry::OptimisedSubMeshGeometryList OptimisedSubMeshGeometryList;
//sizeof( OptimisedSubMeshGeometryList );
typedef ::Ogre::StaticGeometry::SubMeshLodGeometryLinkList SubMeshLodGeometryLinkList;
//sizeof( SubMeshLodGeometryLinkList );
typedef ::Ogre::StaticGeometry::SubMeshGeometryLookup SubMeshGeometryLookup;
//sizeof( SubMeshGeometryLookup );
typedef ::Ogre::StaticGeometry::QueuedSubMeshList QueuedSubMeshList;
//sizeof( QueuedSubMeshList );
typedef ::Ogre::StaticGeometry::QueuedGeometryList QueuedGeometryList;
//sizeof( QueuedGeometryList );
typedef ::Ogre::StaticGeometry::LODBucket::MaterialBucketMap MaterialBucketMap;
//sizeof( MaterialBucketMap );
typedef ::Ogre::StaticGeometry::LODBucket::MaterialIterator MaterialIterator;
//sizeof( MaterialIterator );
typedef ::Ogre::StaticGeometry::MaterialBucket::GeometryBucketList GeometryBucketList;
//sizeof( GeometryBucketList );
typedef ::Ogre::StaticGeometry::MaterialBucket::GeometryIterator GeometryIterator;
//sizeof( GeometryIterator );
typedef ::Ogre::StaticGeometry::Region::LODBucketList LODBucketList;
//sizeof( LODBucketList );
typedef ::Ogre::StaticGeometry::Region::LODIterator LODIterator;
//sizeof( LODIterator );
typedef ::Ogre::StaticGeometry::RegionMap RegionMap;
//sizeof( RegionMap );
typedef ::Ogre::StaticGeometry::RegionIterator RegionIterator;
//sizeof( RegionIterator );
typedef ::Ogre::Skeleton::BoneList BoneList;
//sizeof( BoneList );
typedef ::Ogre::Skeleton::BoneIterator BoneIterator;
//sizeof( BoneIterator );
typedef ::Ogre::Skeleton::LinkedSkeletonAnimSourceList LinkedSkeletonAnimSourceList;
//sizeof( LinkedSkeletonAnimSourceList );
typedef ::Ogre::Skeleton::LinkedSkeletonAnimSourceIterator LinkedSkeletonAnimSourceIterator;
//sizeof( LinkedSkeletonAnimSourceIterator );
typedef ::Ogre::Skeleton::BoneHandleMap BoneHandleMap;
//sizeof( BoneHandleMap );
typedef ::Ogre::ShadowCaster::ShadowRenderableList ShadowRenderableList;
//sizeof( ShadowRenderableList );
typedef ::Ogre::ShadowCaster::ShadowRenderableListIterator ShadowRenderableListIterator;
//sizeof( ShadowRenderableListIterator );
typedef ::Ogre::SceneNode::ObjectMap ObjectMap;
//sizeof( ObjectMap );
typedef ::Ogre::SceneNode::ObjectIterator ObjectIterator;
//sizeof( ObjectIterator );
typedef ::Ogre::SceneNode::ConstObjectIterator ConstObjectIterator;
//sizeof( ConstObjectIterator );
typedef ::Ogre::SceneManagerEnumerator::Instances SceneManagerEnumeratorInstances;
//sizeof( SceneManagerEnumeratorInstances );
typedef ::Ogre::SceneManagerEnumerator::MetaDataList MetaDataList;
//sizeof( MetaDataList );
typedef ::Ogre::SceneManagerEnumerator::MetaDataIterator MetaDataIterator;
//sizeof( MetaDataIterator );
typedef ::Ogre::SceneManagerEnumerator::SceneManagerIterator SceneManagerIterator;
//sizeof( SceneManagerIterator );
typedef ::Ogre::SceneManager::CameraIterator CameraIterator;
//sizeof( CameraIterator );
typedef ::Ogre::SceneManager::AnimationIterator AnimationIterator;
//sizeof( AnimationIterator );
typedef ::Ogre::SceneManager::MovableObjectIterator MovableObjectIteratorOne;
//sizeof( MovableObjectIterator );
typedef ::Ogre::Root::MovableObjectFactoryIterator MovableObjectFactoryIterator;
//sizeof( MovableObjectFactoryIterator );
typedef ::Ogre::RibbonTrail::NodeList NodeList;
//sizeof( NodeList );
typedef ::Ogre::RibbonTrail::NodeIterator NodeIterator;
//sizeof( NodeIterator );
typedef ::Ogre::ResourceManager::ResourceMap ResourceMap;
//sizeof( ResourceMap );
typedef ::Ogre::ResourceManager::ResourceHandleMap ResourceHandleMap;
//sizeof( ResourceHandleMap );
typedef ::Ogre::ResourceManager::ResourceMapIterator ResourceMapIterator;
//sizeof( ResourceMapIterator );
typedef ::Ogre::ResourceGroupManager::ResourceDeclarationList ResourceDeclarationList;
//sizeof( ResourceDeclarationList );
typedef ::Ogre::ResourceGroupManager::ResourceManagerMap ResourceManagerMap;
//sizeof( ResourceManagerMap );
typedef ::Ogre::ResourceGroupManager::ResourceManagerIterator ResourceManagerIterator;
//sizeof( ResourceManagerIterator );
typedef ::Ogre::RenderSystem::RenderTargetIterator RenderTargetIterator;
//sizeof( RenderTargetIterator );
typedef ::Ogre::RenderQueueGroup::PriorityMap PriorityMap;
//sizeof( PriorityMap );
typedef ::Ogre::RenderQueueGroup::PriorityMapIterator PriorityMapIterator;
//sizeof( PriorityMapIterator );
typedef ::Ogre::RenderQueue::RenderQueueGroupMap RenderQueueGroupMap;
//sizeof( RenderQueueGroupMap );
typedef ::Ogre::RenderQueue::QueueGroupIterator QueueGroupIterator;
//sizeof( QueueGroupIterator );
typedef ::Ogre::ProgressiveMesh::LODFaceList LODFaceList;
//sizeof( LODFaceList );
typedef ::Ogre::Pose::VertexOffsetMap VertexOffsetMap;
//sizeof( VertexOffsetMap );
typedef ::Ogre::Pose::VertexOffsetIterator VertexOffsetIterator;
//sizeof( VertexOffsetIterator );
typedef ::Ogre::Pose::ConstVertexOffsetIterator ConstVertexOffsetIterator;
//sizeof( ConstVertexOffsetIterator );
typedef ::Ogre::PlaneBoundedVolume::PlaneList PlaneList;
//sizeof( PlaneList );
typedef ::Ogre::Pass::PassSet PassSet;
//sizeof( PassSet );
typedef ::Ogre::Pass::TextureUnitStateIterator TextureUnitStateIterator;
//sizeof( TextureUnitStateIterator );
typedef ::Ogre::Pass::ConstTextureUnitStateIterator ConstTextureUnitStateIterator;
//sizeof( ConstTextureUnitStateIterator );
typedef ::Ogre::ParticleSystemManager::ParticleTemplateMap ParticleTemplateMap;
//sizeof( ParticleTemplateMap );
typedef ::Ogre::ParticleSystemManager::ParticleAffectorFactoryMap ParticleAffectorFactoryMap;
//sizeof( ParticleAffectorFactoryMap );
typedef ::Ogre::ParticleSystemManager::ParticleEmitterFactoryMap ParticleEmitterFactoryMap;
//sizeof( ParticleEmitterFactoryMap );
typedef ::Ogre::ParticleSystemManager::ParticleSystemRendererFactoryMap ParticleSystemRendererFactoryMap;
//sizeof( ParticleSystemRendererFactoryMap );
typedef ::Ogre::ParticleSystemManager::ParticleAffectorFactoryIterator ParticleAffectorFactoryIterator;
//sizeof( ParticleAffectorFactoryIterator );
typedef ::Ogre::ParticleSystemManager::ParticleEmitterFactoryIterator ParticleEmitterFactoryIterator;
//sizeof( ParticleEmitterFactoryIterator );
typedef ::Ogre::ParticleSystemManager::ParticleRendererFactoryIterator ParticleRendererFactoryIterator;
//sizeof( ParticleRendererFactoryIterator );
typedef ::Ogre::ParticleSystemManager::ParticleSystemTemplateIterator ParticleSystemTemplateIterator;
//sizeof( ParticleSystemTemplateIterator );
typedef ::Ogre::OverlayManager::OverlayMap OverlayMap;
//sizeof( OverlayMap );
typedef ::Ogre::OverlayManager::ElementMap ElementMap;
//sizeof( ElementMap );
typedef ::Ogre::OverlayManager::OverlayMapIterator OverlayMapIterator;
//sizeof( OverlayMapIterator );
typedef ::Ogre::OverlayManager::TemplateIterator TemplateIterator;
//sizeof( TemplateIterator );
typedef ::Ogre::OverlayContainer::ChildMap ChildMap;
//sizeof( ChildMap );
typedef ::Ogre::OverlayContainer::ChildIterator ChildIteratorOne;
//sizeof( ChildIterator );
typedef ::Ogre::OverlayContainer::ChildContainerMap ChildContainerMap;
//sizeof( ChildContainerMap );
typedef ::Ogre::OverlayContainer::ChildContainerIterator ChildContainerIterator;
//sizeof( ChildContainerIterator );
typedef ::Ogre::Overlay::OverlayContainerList OverlayContainerList;
//sizeof( OverlayContainerList );
typedef ::Ogre::Overlay::Overlay2DElementsIterator Overlay2DElementsIterator;
//sizeof( Overlay2DElementsIterator );
typedef ::Ogre::Node::ChildNodeMap ChildNodeMap;
//sizeof( ChildNodeMap );
typedef ::Ogre::Node::ChildNodeIterator ChildNodeIterator;
//sizeof( ChildNodeIterator );
typedef ::Ogre::Node::ConstChildNodeIterator ConstChildNodeIterator;
//sizeof( ConstChildNodeIterator );
typedef ::Ogre::Mesh::LodDistanceList LodDistanceList;
//sizeof( LodDistanceList );
typedef ::Ogre::Mesh::VertexBoneAssignmentList VertexBoneAssignmentList;
//sizeof( VertexBoneAssignmentList );
typedef ::Ogre::Mesh::BoneAssignmentIterator BoneAssignmentIterator;
//sizeof( BoneAssignmentIterator );
typedef ::Ogre::Mesh::SubMeshList SubMeshList;
//sizeof( SubMeshList );
typedef ::Ogre::Mesh::IndexMap IndexMap;
//sizeof( IndexMap );
typedef ::Ogre::Mesh::SubMeshNameMap SubMeshNameMap;
//sizeof( SubMeshNameMap );
typedef ::Ogre::Mesh::SubMeshIterator SubMeshIterator;
//sizeof( SubMeshIterator );
typedef ::Ogre::Mesh::PoseIterator PoseIterator;
//sizeof( PoseIterator );
typedef ::Ogre::Mesh::ConstPoseIterator ConstPoseIterator;
//sizeof( ConstPoseIterator );
typedef ::Ogre::Material::LodDistanceList LodDistanceList;
//sizeof( LodDistanceList );
typedef ::Ogre::Material::LodDistanceIterator LodDistanceIterator;
//sizeof( LodDistanceIterator );
typedef ::Ogre::Material::TechniqueIterator MaterialTechniqueIterator;
//sizeof( MaterialTechniqueIterator );
typedef ::Ogre::ManualObject::SectionList SectionList;
//sizeof( SectionList );
typedef ::Ogre::Image::Box ImagBox;
//sizeof( ImagBox );
typedef ::Ogre::Image::Rect ImagRect;
//sizeof( ImagRect );
typedef ::Ogre::HighLevelGpuProgramManager::FactoryMap FactoryMap;
//sizeof( FactoryMap );
typedef ::Ogre::GpuProgramManager::SyntaxCodes SyntaxCodes;
//sizeof( SyntaxCodes );
typedef ::Ogre::Entity::EntitySet EntitySet;
//sizeof( EntitySet );
typedef ::Ogre::Entity::ChildObjectList ChildObjectList;
//sizeof( ChildObjectList );
typedef ::Ogre::Entity::ChildObjectListIterator ChildObjectListIterator;
//sizeof( ChildObjectListIterator );
typedef ::Ogre::EdgeData::TriangleFaceNormalList TriangleFaceNormalList;
//sizeof( TriangleFaceNormalList );
typedef ::Ogre::EdgeData::TriangleLightFacingList TriangleLightFacingList;
//sizeof( TriangleLightFacingList );
typedef ::Ogre::EdgeData::TriangleList TriangleList;
//sizeof( TriangleList );
typedef ::Ogre::EdgeData::EdgeList EdgeList;
//sizeof( EdgeList );
typedef ::Ogre::EdgeData::EdgeGroupList EdgeGroupList;
//sizeof( EdgeGroupList );
typedef ::Ogre::BillboardChain::ElementList ElementList;
//sizeof( ElementList );
typedef ::Ogre::Animation::NodeTrackList NodeTrackList;
//sizeof( NodeTrackList );
typedef ::Ogre::Animation::NodeTrackIterator NodeTrackIterator;
//sizeof( NodeTrackIterator );
typedef ::Ogre::Animation::NumericTrackList NumericTrackList;
//sizeof( NumericTrackList );
typedef ::Ogre::Animation::NumericTrackIterator NumericTrackIterator;
//sizeof( NumericTrackIterator );
typedef ::Ogre::Animation::VertexTrackList VertexTrackList;
//sizeof( VertexTrackList );
typedef ::Ogre::Animation::VertexTrackIterator VertexTrackIterator;
//sizeof( VertexTrackIterator );
typedef ::Ogre::Animation::TrackHandleList TrackHandleList;
//sizeof( TrackHandleList );
typedef ::Ogre::String String;
//sizeof( String );
// typedef ::Ogre::_StringBase _StringBase;
// //sizeof( _StringBase ));



     