// sizeof ( Ogre::VectorIterator );
// sizeof ( Ogre::MapIterator );
// sizeof ( Ogre::ConstVectorIterator );
// sizeof ( Ogre::ConstMapIterator );



sizeof ( Ogre::MapIterator< std::multimap<Ogre::String, Ogre::String> > );
sizeof ( Ogre::MapIterator< std::map<Ogre::String, std::multimap<Ogre::String, Ogre::String>*> > );
sizeof ( Ogre::MaterialPtr );

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
sizeof( stdext::_Hash<stdext::_Hmap_traits<std::string, Ogre::Node*, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, Ogre::Node*> >, false> > );
//sizeof( stdext::_Hash<stdext::_Hmap_traits<std::string, unsigned short, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, unsigned short> >, false> > );
sizeof( stdext::_Hash<stdext::_Hmap_traits<std::string, Ogre::SharedPtr<Ogre::Resource>, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, Ogre::SharedPtr<Ogre::Resource> > >, false> > );
sizeof( stdext::_Hash<stdext::_Hmap_traits<std::string, Ogre::MovableObject*, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, Ogre::MovableObject*> >, false> > );
#endif

#if OGRE_VERSION_MINOR == 4

#else
    sizeof ( Ogre::ResourceBackgroundQueue );
    sizeof ( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > );
    sizeof ( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)1> > );
    sizeof ( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)2> > );
    sizeof ( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)3> > );
    sizeof ( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)4> > );
    sizeof ( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)5> > );
    sizeof ( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)6> > );
    sizeof ( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)7> > );
    sizeof ( Ogre::ConstVectorIterator<std::vector<Ogre::Technique::GPUVendorRule, std::allocator<Ogre::Technique::GPUVendorRule> > > );
    sizeof ( Ogre::MapIterator<std::map<int, Ogre::InstancedGeometry::InstancedObject*, std::less<int>, std::allocator<std::pair<const int, Ogre::InstancedGeometry::InstancedObject*> > > > );
    sizeof ( Ogre::ConstVectorIterator<std::vector<Ogre::Technique::GPUDeviceNameRule, std::allocator<Ogre::Technique::GPUDeviceNameRule> > > );
    
#endif


sizeof( std::pair<Ogre::SharedPtr<Ogre::Resource>, bool> );

std::vector< int > v;
std::set< std::string > s;
sizeof ( std::vector< float > );
std::multimap< std::string, std::string > ms;
sizeof ( v );
sizeof ( s );
sizeof ( ms );

Ogre::AnyNumeric AnyNumInt (1);
Ogre::AnyNumeric AnyNumReal ((Ogre::Real)1.0);
Ogre::AnyNumeric AnyNumUint8 ((Ogre::uint8)0);
Ogre::AnyNumeric AnyNumUint16 ((Ogre::uint16)0);
Ogre::AnyNumeric AnyNumUint32 ((Ogre::uint32)0);
Ogre::AnyNumeric AnyNumUint ((Ogre::uint)0);
Ogre::AnyNumeric AnyNumUlong ((Ogre::ulong)0);
Ogre::AnyNumeric AnyNumUshort ((Ogre::ushort)0);
Ogre::AnyNumeric AnyNumUchar ((Ogre::uchar) 0);

Ogre::Any AnyInt (1);
Ogre::Any AnyReal ((Ogre::Real)1.0);
Ogre::Any AnyUint8 ((Ogre::uint8)0);
Ogre::Any AnyUint16 ((Ogre::uint16)0);
Ogre::Any AnyUint32 ((Ogre::uint32)0);
Ogre::Any AnyUint ((Ogre::uint)0);
Ogre::Any AnyUlong ((Ogre::ulong)0);
Ogre::Any AnyUshort ((Ogre::ushort)0);
Ogre::Any AnyUchar ((Ogre::uchar) 0);
Ogre::Any AnyString ((Ogre::String) "");

//sizeof ( Ogre::Any::placeholder );
sizeof ( AnyNumInt );
sizeof ( AnyNumReal );
sizeof ( AnyNumUint8 );
sizeof ( AnyNumUint16 );
sizeof ( AnyNumUint32 );
sizeof ( AnyNumUint );
sizeof ( AnyNumUlong );
sizeof ( AnyNumUshort );
sizeof ( AnyNumUchar );

sizeof ( AnyInt );
sizeof ( AnyReal );
sizeof ( AnyUint8 );
sizeof ( AnyUint16 );
sizeof ( AnyUint32 );
sizeof ( AnyUint );
sizeof ( AnyUlong );
sizeof ( AnyUshort );
sizeof ( AnyUchar );
sizeof ( AnyString );

sizeof ( Ogre::SharedPtr<Ogre::WaveformControllerFunction> );
// sizeof ( Ogre::ControllerFunction<float>);
// sizeof ( Ogre::SharedPtr<Ogre::ControllerFunction<float>>);
sizeof ( std::list<Ogre::Particle*> );
sizeof ( std::vector<Ogre::Particle*>);
sizeof ( Ogre::RenderTarget::FrameStats );

sizeof ( Ogre::SharedPtr<Ogre::AnimableValue> );
sizeof ( Ogre::SharedPtr<Ogre::FileInfoList> );
sizeof ( Ogre::SharedPtr<Ogre::Texture> );

sizeof ( Ogre::TexturePtr );

//sizeof ( Ogre::SharedPtr<Ogre::CodecData> );
sizeof ( Ogre::SharedPtr<Ogre::ControllerFunction<Ogre::Real> > );
sizeof ( Ogre::SharedPtr<Ogre::ControllerValue<Ogre::Real> > );
sizeof ( Ogre::SharedPtr<Ogre::MemoryDataStream> );
sizeof ( Ogre::SharedPtr<Ogre::DataStream> );
sizeof ( Ogre::SharedPtr<Ogre::DataStreamList> );
sizeof ( Ogre::SharedPtr<Ogre::GpuProgramParameters> );
// sizeof ( std::multimap<Ogre::HardwareVertexBuffer*, Ogre::VertexBufferBinding::HardwareVertexBufferSharedPtr> FreeTemporaryVertexBufferMap;
//sizeof ( std::map<unsigned short, Ogre::VertexBufferBinding::HardwareVertexBufferSharedPtr>); // VertexBufferBindingMap;
sizeof ( Ogre::SharedPtr<Ogre::Resource> ); //ResourcePtr;
sizeof ( Ogre::SharedPtr<Ogre::ShadowCameraSetup> ); //ShadowCameraSetupPtr;
sizeof ( Ogre::SharedPtr<Ogre::StringVector> ); //StringVectorPtr;

sizeof ( Ogre::SharedPtr<DefaultShadowCameraSetup> );
sizeof ( Ogre::SharedPtr<FocusedShadowCameraSetup> );
sizeof ( Ogre::SharedPtr<LiSPSMShadowCameraSetup> );
sizeof ( Ogre::SharedPtr<PlaneOptimalShadowCameraSetup> );

sizeof ( Ogre::ResourceManager::ResourceCreateOrRetrieveResult);



// sizeof ( exception );

sizeof ( Ogre::Singleton<Ogre::FontManager> );
sizeof ( Ogre::Singleton<Ogre::ExternalTextureSourceManager> );
sizeof (  Ogre::Singleton<Ogre::DynLibManager> );

sizeof (std::pair<bool, float> );
sizeof (std::pair<unsigned int,unsigned int> );
//sizeof ( std::pair<Ogre::SharedPtr<Ogre::Resource>, bool> );

sizeof (Ogre::Root::PluginLibList );
sizeof (Ogre::Root::PluginInstanceList );

sizeof (Ogre::RadixSort<std::vector<Ogre::RenderablePass>,Ogre::RenderablePass,unsigned int> );
sizeof (Ogre::RadixSort<std::vector<Ogre::RenderablePass>,Ogre::RenderablePass,float> );
sizeof (Ogre::RadixSort<std::list<Ogre::Particle*>,Ogre::Particle*,float> );
sizeof (Ogre::RadixSort<std::list<Ogre::Billboard*>,Ogre::Billboard*,float> );
sizeof (std::map<Ogre::String, Ogre::MovableObjectFactory*> );

sizeof( Ogre::Controller<float> );
sizeof( Ogre::ControllerValue<float> );
sizeof( Ogre::ControllerFunction<float> );

sizeof( Ogre::Mesh::SubMeshNameMap );
sizeof( Ogre::RenderSystem );

sizeof( Ogre::ConvexBody::PolygonList );
sizeof( Ogre::Polygon::VertexList );
sizeof( Ogre::Polygon::EdgeMap );
sizeof( Ogre::Polygon::Edge );
sizeof( Ogre::WindowEventUtilities::WindowEventListeners );
sizeof( Ogre::WindowEventUtilities::Windows );
sizeof( Ogre::RenderSystemList );
sizeof( Ogre::SceneTypeMask );
sizeof( Ogre::ConstShadowTextureConfigIterator );
sizeof( Ogre::ShadowTextureConfigList );
sizeof( Ogre::ShadowTextureList );
sizeof( Ogre::SceneQueryMovableWorldFragmentIntersectionList );
sizeof( Ogre::SceneQueryMovableIntersectionList );
sizeof( Ogre::SceneQueryMovableObjectWorldFragmentPair );
sizeof( Ogre::SceneQueryMovableObjectPair );
sizeof( Ogre::RaySceneQueryResult );
sizeof( Ogre::SceneQueryResultWorldFragmentList );
sizeof( Ogre::SceneQueryResultMovableList );
sizeof( Ogre::BackgroundProcessTicket );
#if OGRE_VERSION_MINOR == 4
    sizeof( Ogre::DLL_CREATERENDERWINDOW );
#endif
sizeof( Ogre::RenderTargetPriorityMap );
sizeof( Ogre::RenderTargetMap );
sizeof( Ogre::ConfigOptionMap );
sizeof( Ogre::ConfigOption );
//sizeof( Ogre::_ConfigOption );

sizeof( Ogre::RenderQueueInvocationIterator );
sizeof( Ogre::RenderQueueInvocationList );
sizeof( Ogre::IlluminationPassList );
sizeof( Ogre::ATTRIBUTE_PARSER );
sizeof( Ogre::ShadowCameraSetupPtr );
sizeof( Ogre::VertexBoneAssignment );
sizeof( Ogre::ControllerFunctionRealPtr );
sizeof( Ogre::ControllerValueRealPtr );
sizeof( Ogre::ConstImagePtrList );
sizeof( Ogre::ImagePtrList );
sizeof( Ogre::ConfigFile::SettingsMultiMap );
sizeof( Ogre::ConfigFile::SettingsIterator );
sizeof( Ogre::ConfigFile::SettingsBySection );
sizeof( Ogre::ConfigFile::SectionIterator );
sizeof( Ogre::PlaneBoundedVolumeList );
sizeof( Ogre::GpuProgramParametersSharedPtr );

sizeof( Ogre::GpuConstantDefinitionMap );
sizeof( Ogre::GpuConstantDefinitionIterator );
sizeof( Ogre::GpuProgramParameters::AutoConstantIterator );


sizeof( Ogre::BufferUsageList );
sizeof( Ogre::ResourcePtr );
sizeof( Ogre::ResourceHandle );
sizeof( Ogre::ParamDictionaryMap );
sizeof( Ogre::ParamCommandMap );
sizeof( Ogre::ParameterList );
sizeof( Ogre::Rect );
sizeof( Ogre::FloatRect );
sizeof( Ogre::AliasTextureNamePairList );
sizeof( Ogre::NameValuePairList );
sizeof( Ogre::BinaryOptionList );
sizeof( Ogre::UnaryOptionList );
sizeof( Ogre::LightList );
sizeof( Ogre::TrackVertexColourType );
sizeof( Ogre::FileInfoListPtr );
sizeof( Ogre::FileInfoList );
sizeof( Ogre::MemoryDataStreamPtr );
sizeof( Ogre::DataStreamListPtr );
sizeof( Ogre::DataStreamList );
sizeof( Ogre::DataStreamPtr );
sizeof( Ogre::ConstEnabledAnimationStateIterator );
sizeof( Ogre::EnabledAnimationStateList );
sizeof( Ogre::ConstAnimationStateIterator );
sizeof( Ogre::AnimationStateIterator );
sizeof( Ogre::AnimationStateMap );
sizeof( Ogre::PoseList );
sizeof( Ogre::VertexPoseKeyFrame::PoseRefList );
sizeof( Ogre::VertexPoseKeyFrame::PoseRefIterator );
sizeof( Ogre::VertexPoseKeyFrame::ConstPoseRefIterator );
sizeof( Ogre::PlaneList );
sizeof( Ogre::AnimableValuePtr );
sizeof( Ogre::StringVectorPtr );
sizeof( Ogre::StringVector );
sizeof( Ogre::BGRA );
sizeof( Ogre::ABGR );
sizeof( Ogre::ARGB );
sizeof( Ogre::RGBA );
sizeof( Ogre::_StringHash );
sizeof( Ogre::StringUtil::StrStreamType );
sizeof( Ogre::CompositionTargetPass::Passes );
sizeof( Ogre::CompositionTargetPass::PassIterator );
sizeof( Ogre::CompositionTechnique::TargetPasses );
sizeof( Ogre::CompositionTechnique::TargetPassIterator );
sizeof( Ogre::CompositionTechnique::TextureDefinitions );
sizeof( Ogre::CompositionTechnique::TextureDefinitionIterator );
sizeof( Ogre::CompositorInstance::TargetOperation::RenderQueueBitSet );
sizeof( Ogre::CompositorInstance::QuadMaterialMap );
sizeof( Ogre::CompositorInstance::RenderSystemOpPair );
sizeof( Ogre::CompositorInstance::RenderSystemOpPairs );
sizeof( Ogre::CompositorInstance::CompiledState );
sizeof( Ogre::CompositorChain::Instances );
sizeof( Ogre::CompositorChain::InstanceIterator );
sizeof( Ogre::Compositor::Techniques );
sizeof( Ogre::Compositor::TechniqueIterator );
sizeof( Ogre::VertexDeclaration::VertexElementList );
sizeof( Ogre::VertexData::HardwareAnimationDataList );
sizeof( Ogre::VertexBufferBinding::VertexBufferBindingMap );
sizeof( Ogre::TextureUnitState::EffectMap );
sizeof( Ogre::Technique::PassIterator );
sizeof( Ogre::Technique::IlluminationPassIterator );
sizeof( Ogre::SubMesh::IndexMap );
sizeof( Ogre::SubMesh::VertexBoneAssignmentList );
sizeof( Ogre::SubMesh::BoneAssignmentIterator );
sizeof( Ogre::SubMesh::AliasTextureIterator );

sizeof( Ogre::StaticGeometry::SubMeshLodGeometryLink );
sizeof( Ogre::InstancedGeometry::SubMeshLodGeometryLink );
sizeof ( ::pyplusplus::aliases::SubMeshLodGeometryLinkList );
sizeof ( ::pyplusplus::aliases::InstSubMeshLodGeometryLinkList );

sizeof( Ogre::InstancedGeometry::OptimisedSubMeshGeometryList );
sizeof( Ogre::InstancedGeometry::SubMeshLodGeometryLinkList );
sizeof( Ogre::InstancedGeometry::SubMeshGeometryLookup );
sizeof( Ogre::InstancedGeometry::QueuedSubMeshList );
sizeof( Ogre::InstancedGeometry::QueuedSubMeshOriginList );
sizeof( Ogre::InstancedGeometry::QueuedGeometryList );
sizeof( Ogre::InstancedGeometry::InstancedObject::GeometryBucketList );
sizeof( Ogre::InstancedGeometry::MaterialBucket::GeometryIterator );
sizeof( Ogre::InstancedGeometry::LODBucket::MaterialBucketMap );
sizeof( Ogre::InstancedGeometry::LODBucket::MaterialIterator );
sizeof( Ogre::InstancedGeometry::BatchInstance::LODBucketList );
sizeof( Ogre::InstancedGeometry::BatchInstance::ObjectsMap );
sizeof( Ogre::InstancedGeometry::BatchInstance::LODIterator );
sizeof( Ogre::InstancedGeometry::BatchInstanceMap );
sizeof( Ogre::InstancedGeometry::RenderOperationVector );
sizeof( Ogre::InstancedGeometry::BatchInstanceIterator );

sizeof( Ogre::StaticGeometry::OptimisedSubMeshGeometryList );
sizeof( Ogre::StaticGeometry::SubMeshLodGeometryLinkList );
sizeof( Ogre::StaticGeometry::SubMeshGeometryLookup );
sizeof( Ogre::StaticGeometry::QueuedSubMeshList );
sizeof( Ogre::StaticGeometry::QueuedGeometryList );

// classes that don't get exposed by default - probably don't need this..
sizeof( Ogre::StaticGeometry::LODBucket );
sizeof( Ogre::StaticGeometry::Region );
sizeof( Ogre::StaticGeometry::MaterialBucket );


sizeof( Ogre::StaticGeometry::LODBucket::MaterialBucketMap );
sizeof( Ogre::StaticGeometry::LODBucket::MaterialIterator );
sizeof( Ogre::StaticGeometry::MaterialBucket );
sizeof( Ogre::StaticGeometry::MaterialBucket::GeometryBucketList );
sizeof( Ogre::StaticGeometry::MaterialBucket::GeometryIterator );
sizeof( Ogre::StaticGeometry::Region );
sizeof( Ogre::StaticGeometry::Region::LODBucketList );
sizeof( Ogre::StaticGeometry::Region::LODIterator );
sizeof( Ogre::StaticGeometry::RegionMap );
sizeof( Ogre::StaticGeometry::RegionIterator );

sizeof( Ogre::Skeleton::BoneList );
sizeof( Ogre::Skeleton::BoneIterator );
sizeof( Ogre::Skeleton::LinkedSkeletonAnimSourceList );
sizeof( Ogre::Skeleton::LinkedSkeletonAnimSourceIterator );
sizeof( Ogre::Skeleton::BoneHandleMap );
sizeof( Ogre::ShadowCaster::ShadowRenderableList );
sizeof( Ogre::ShadowCaster::ShadowRenderableListIterator );
sizeof( Ogre::SceneNode::ObjectMap );
sizeof( Ogre::SceneNode::ObjectIterator );
sizeof( Ogre::SceneNode::ConstObjectIterator );
sizeof( Ogre::SceneManagerEnumerator::Instances );
sizeof( Ogre::SceneManagerEnumerator::MetaDataList );
sizeof( Ogre::SceneManagerEnumerator::MetaDataIterator );
sizeof( Ogre::SceneManagerEnumerator::SceneManagerIterator );
sizeof( Ogre::SceneManager::CameraIterator );
sizeof( Ogre::SceneManager::AnimationIterator );
sizeof( Ogre::SceneManager::MovableObjectIterator );
sizeof( Ogre::Root::MovableObjectFactoryIterator );
sizeof( Ogre::RibbonTrail::NodeList );
sizeof( Ogre::RibbonTrail::NodeIterator );
sizeof( Ogre::ResourceManager::ResourceMap );
sizeof( Ogre::ResourceManager::ResourceHandleMap );
sizeof( Ogre::ResourceManager::ResourceMapIterator );
sizeof( Ogre::ResourceGroupManager::ResourceDeclarationList );
sizeof( Ogre::ResourceGroupManager::ResourceManagerMap );
sizeof( Ogre::ResourceGroupManager::ResourceManagerIterator );
sizeof( Ogre::RenderSystem::RenderTargetIterator );
sizeof( Ogre::RenderQueueGroup::PriorityMap );
sizeof( Ogre::RenderQueueGroup::PriorityMapIterator );
sizeof( Ogre::RenderQueue::RenderQueueGroupMap );
sizeof( Ogre::RenderQueue::QueueGroupIterator );
sizeof( Ogre::ProgressiveMesh::LODFaceList );
sizeof( Ogre::Pose::VertexOffsetMap );
sizeof( Ogre::Pose::VertexOffsetIterator );
sizeof( Ogre::Pose::ConstVertexOffsetIterator );
sizeof( Ogre::PlaneBoundedVolume::PlaneList );
sizeof( Ogre::Pass::PassSet );
sizeof( Ogre::Pass::TextureUnitStateIterator );
sizeof( Ogre::Pass::ConstTextureUnitStateIterator );
sizeof( Ogre::ParticleSystemManager::ParticleTemplateMap );
sizeof( Ogre::ParticleSystemManager::ParticleAffectorFactoryMap );
sizeof( Ogre::ParticleSystemManager::ParticleEmitterFactoryMap );
sizeof( Ogre::ParticleSystemManager::ParticleSystemRendererFactoryMap );
sizeof( Ogre::ParticleSystemManager::ParticleAffectorFactoryIterator );
sizeof( Ogre::ParticleSystemManager::ParticleEmitterFactoryIterator );
sizeof( Ogre::ParticleSystemManager::ParticleRendererFactoryIterator );
sizeof( Ogre::ParticleSystemManager::ParticleSystemTemplateIterator );
sizeof( Ogre::OverlayManager::OverlayMap );
sizeof( Ogre::OverlayManager::ElementMap );
sizeof( Ogre::OverlayManager::OverlayMapIterator );
sizeof( Ogre::OverlayManager::TemplateIterator );
sizeof( Ogre::OverlayContainer::ChildMap );
sizeof( Ogre::OverlayContainer::ChildIterator );
sizeof( Ogre::OverlayContainer::ChildContainerMap );
sizeof( Ogre::OverlayContainer::ChildContainerIterator );
sizeof( Ogre::Overlay::OverlayContainerList );
sizeof( Ogre::Overlay::Overlay2DElementsIterator );
sizeof( Ogre::Node::ChildNodeMap );
sizeof( Ogre::Node::ChildNodeIterator );
sizeof( Ogre::Node::ConstChildNodeIterator );
sizeof( Ogre::Mesh::LodDistanceList );
sizeof( Ogre::Mesh::VertexBoneAssignmentList );
sizeof( Ogre::Mesh::BoneAssignmentIterator );
sizeof( Ogre::Mesh::SubMeshList );
sizeof( Ogre::Mesh::IndexMap );
sizeof( Ogre::Mesh::SubMeshNameMap );
sizeof( Ogre::Mesh::SubMeshIterator );
sizeof( Ogre::Mesh::PoseIterator );
sizeof( Ogre::Mesh::ConstPoseIterator );
sizeof( Ogre::Material::LodDistanceList );
sizeof( Ogre::Material::LodDistanceIterator );
sizeof( Ogre::Material::TechniqueIterator );
sizeof( Ogre::ManualObject::SectionList );
sizeof( Ogre::Image::Box );
sizeof( Ogre::Image::Rect );
sizeof( Ogre::HighLevelGpuProgramManager::FactoryMap );
sizeof( Ogre::GpuProgramManager::SyntaxCodes );
sizeof( Ogre::Entity::EntitySet );
sizeof( Ogre::Entity::ChildObjectList );
sizeof( Ogre::Entity::ChildObjectListIterator );
sizeof( Ogre::EdgeData::TriangleFaceNormalList );
sizeof( Ogre::EdgeData::TriangleLightFacingList );
sizeof( Ogre::EdgeData::TriangleList );
sizeof( Ogre::EdgeData::EdgeList );
sizeof( Ogre::EdgeData::EdgeGroupList );
sizeof( Ogre::BillboardChain::ElementList );
sizeof( Ogre::Animation::NodeTrackList );
sizeof( Ogre::Animation::NodeTrackIterator );
sizeof( Ogre::Animation::NumericTrackList );
sizeof( Ogre::Animation::NumericTrackIterator );
sizeof( Ogre::Animation::VertexTrackList );
sizeof( Ogre::Animation::VertexTrackIterator );
sizeof( Ogre::Animation::TrackHandleList );
sizeof( Ogre::String );
sizeof( Ogre::_StringBase );
sizeof( Ogre::ulong );
sizeof( Ogre::uint );
sizeof( Ogre::ushort );
sizeof( Ogre::uchar );
sizeof( Ogre::Real );
sizeof( Ogre::uint8 );
sizeof( Ogre::uint16 );
sizeof( Ogre::uint32 );
