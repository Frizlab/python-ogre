
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
   sizeof( stdext::_Hash<stdext::_Hmap_traits<std::string, Ogre::Node*, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, Ogre::Node*> >, false> > );
   //sizeof( stdext::_Hash<stdext::_Hmap_traits<std::string, unsigned short, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, unsigned short> >, false> > );
   sizeof( stdext::_Hash<stdext::_Hmap_traits<std::string, Ogre::SharedPtr<Ogre::Resource>, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, Ogre::SharedPtr<Ogre::Resource> > >, false> > );
   sizeof( stdext::_Hash<stdext::_Hmap_traits<std::string, Ogre::MovableObject*, stdext::hash_compare<std::string, std::less<std::string> >, std::allocator<std::pair<std::string const, Ogre::MovableObject*> >, false> > );
#endif
#if OGRE_VERSION_MINOR < 7
    sizeof( Ogre::Mesh::LodDistanceList );
    sizeof( Ogre::Material::LodDistanceList );
    sizeof( Ogre::Material::LodDistanceIterator );

#else
    sizeof(Ogre::ConstVectorIterator<std::vector<Ogre::Technique::GPUDeviceNameRule, Ogre::STLAllocator<Ogre::Technique::GPUDeviceNameRule, Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > > > );
    sizeof(Ogre::ConstVectorIterator<std::vector<Ogre::Technique::GPUVendorRule, Ogre::STLAllocator<Ogre::Technique::GPUVendorRule, Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > > > );
    sizeof(Ogre::ConstVectorIterator<std::vector<float, Ogre::STLAllocator<float, Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > > > );
    sizeof(Ogre::ConstMapIterator<std::map<unsigned char, Ogre::RenderQueueGroup*, std::less<unsigned char>, Ogre::STLAllocator<std::pair<const unsigned char, Ogre::RenderQueueGroup*>, Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL> > > > );
    sizeof(Ogre::MapIterator<std::map<int, Ogre::InstancedGeometry::InstancedObject*, std::less<int>, Ogre::STLAllocator<std::pair<const int, Ogre::InstancedGeometry::InstancedObject*>, Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > > > );
    sizeof(Ogre::MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::Archive*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, Ogre::STLAllocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::Archive*>, Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > > > );
    sizeof(Ogre::MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::LodStrategy*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, Ogre::STLAllocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::LodStrategy*>, Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > > > );
    sizeof(Ogre::SharedPtr<std::list<Ogre::SharedPtr<Ogre::ConcreteNode>, Ogre::STLAllocator<Ogre::SharedPtr<Ogre::ConcreteNode>, Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL> > > > );
    sizeof(Ogre::SharedPtr<std::vector<Ogre::SharedPtr<Ogre::ScriptToken>, Ogre::STLAllocator<Ogre::SharedPtr<Ogre::ScriptToken>, Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL> > > > );
    sizeof(std::pair<bool, Ogre::SharedPtr<Ogre::Resource> >);
    sizeof(std::pair<bool,Ogre::SharedPtr<Ogre::Resource> >);
    sizeof(std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >,Ogre::MovableObject*> );
    sizeof(std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >,Ogre::Node*> );
    sizeof(std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >,Ogre::SharedPtr<Ogre::Resource> > );
    sizeof(std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >,short unsigned int> );
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    sizeof(std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >,stdext::hash_map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::SharedPtr<Ogre::Resource>, stdext::hash_compare<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::SharedPtr<Ogre::Resource> > > > > );
#endif
    sizeof(std::pair<std::string const, Ogre::MovableObject*> );
    sizeof(std::pair<std::string const, Ogre::Node*>);
    sizeof(std::pair<std::string const, Ogre::SharedPtr<Ogre::Resource> >);
    sizeof(std::pair<std::string const, unsigned short>);
    sizeof(std::pair<bool,Ogre::Vector3>);
   // sizeof(Ogre::SceneManager::RenderContext);
    sizeof(Ogre::CompositorLogic);
    sizeof(Ogre::CustomCompositionPass);

    
#endif

Ogre::SharedPtr<Ogre::DataStream> d;
Ogre::AnyNumeric AnyNumReal ((Ogre::Real)1.0);
Ogre::AnyNumeric AnyNumInt ((int)1);
sizeof( AnyNumReal );
sizeof( AnyNumInt );

// Ogre::Any AnyPython ((boost::python::object)0); 
Ogre::Any AnyString ((Ogre::String) "");
Ogre::Any AnyReal ((Ogre::Real)1.0);
Ogre::Any AnyInt ((int)1);

// sizeof( AnyPython );
sizeof( AnyString );
sizeof( AnyReal );
sizeof( AnyInt );

std::list<Ogre::SceneNode *> specialList; // adding this to stop duplicate exposing in scene managers

sizeof(d);
sizeof( std::vector<float> );
sizeof( std::vector<Ogre::Particle*>);
sizeof( std::vector< unsigned long > );
sizeof( std::vector< unsigned int > );
sizeof( std::vector< unsigned char > );
sizeof( std::vector< long > );
sizeof( std::vector< int > );
sizeof( std::vector< float > );
sizeof( std::vector< char > );
sizeof( std::vector< bool > );
sizeof( std::set< std::string > );
sizeof( std::pair<unsigned int,unsigned int> );
sizeof( std::pair<unsigned char,unsigned char> );
sizeof( std::pair<bool,String> );
sizeof( std::pair<bool, float> );
sizeof( std::pair<std::string, unsigned short> );

sizeof( std::pair<Ogre::SharedPtr<Ogre::Resource>, bool> );
sizeof( std::pair<bool, Ogre::SharedPtr<Ogre::Resource> > );
sizeof( std::pair<std::string, Ogre::SharedPtr<Ogre::Resource> > );

sizeof( std::multimap< std::string, std::string > );
sizeof( std::map<Ogre::String, Ogre::MovableObjectFactory*> );
sizeof( std::map<Ogre::String, Ogre::Archive*> );
sizeof( std::list<Ogre::Particle*> );
sizeof( std::list<Ogre::Plane> );
sizeof( std::pair<bool, std::string> );
sizeof( std::pair<std::string const, Ogre::Node*> );
sizeof( std::pair<std::string const, unsigned short> );
sizeof( std::pair<std::string const, Ogre::MovableObject*> );
sizeof( std::pair<std::string const, Ogre::SharedPtr<Ogre::Resource> > );


sizeof( specialList);
sizeof( Ogre::ushort );
sizeof( Ogre::ulong );
sizeof( Ogre::uint8 );
sizeof( Ogre::uint32 );
sizeof( Ogre::uint16 );
sizeof( Ogre::uint );
sizeof( Ogre::uchar );
sizeof( Ogre::_StringHash );
sizeof( Ogre::_StringBase );
sizeof( Ogre::WindowEventUtilities::Windows );
sizeof( Ogre::WindowEventUtilities::WindowEventListeners );
sizeof( Ogre::VertexPoseKeyFrame::PoseRefList );
sizeof( Ogre::VertexPoseKeyFrame::PoseRefIterator );
sizeof( Ogre::VertexPoseKeyFrame::ConstPoseRefIterator );
sizeof( Ogre::VertexDeclaration::VertexElementList );
sizeof( Ogre::VertexData::HardwareAnimationDataList );
sizeof( Ogre::VertexBufferBinding::VertexBufferBindingMap );
sizeof( Ogre::VertexBoneAssignment );
sizeof( Ogre::UnaryOptionList );
sizeof( Ogre::TrackVertexColourType );
sizeof( Ogre::TextureUnitState::EffectMap );
sizeof( Ogre::TexturePtr );
sizeof( Ogre::Technique * );
sizeof( Ogre::Technique::PassIterator );
sizeof( Ogre::Technique::IlluminationPassIterator );
sizeof( Ogre::SubMesh::VertexBoneAssignmentList );
sizeof( Ogre::SubMesh::IndexMap );
sizeof( Ogre::SubMesh::BoneAssignmentIterator );
sizeof( Ogre::SubMesh::AliasTextureIterator );
sizeof( Ogre::StringVectorPtr );
sizeof( Ogre::StringVector );
sizeof( Ogre::StringUtil::StrStreamType );
sizeof( Ogre::String );
sizeof( Ogre::StaticGeometry::SubMeshLodGeometryLinkList );
sizeof( Ogre::StaticGeometry::SubMeshLodGeometryLink );
sizeof( Ogre::StaticGeometry::SubMeshGeometryLookup );
sizeof( Ogre::StaticGeometry::RegionMap );
sizeof( Ogre::StaticGeometry::RegionIterator );
sizeof( Ogre::StaticGeometry::Region::LODIterator );
sizeof( Ogre::StaticGeometry::Region::LODBucketList );
sizeof( Ogre::StaticGeometry::Region );
sizeof( Ogre::StaticGeometry::Region );
sizeof( Ogre::StaticGeometry::QueuedSubMeshList );
sizeof( Ogre::StaticGeometry::QueuedGeometryList );
sizeof( Ogre::StaticGeometry::OptimisedSubMeshGeometryList );
sizeof( Ogre::StaticGeometry::MaterialBucket::GeometryIterator );
sizeof( Ogre::StaticGeometry::MaterialBucket::GeometryBucketList );
sizeof( Ogre::StaticGeometry::MaterialBucket );
sizeof( Ogre::StaticGeometry::MaterialBucket );
sizeof( Ogre::StaticGeometry::LODBucket::MaterialIterator );
sizeof( Ogre::StaticGeometry::LODBucket::MaterialBucketMap );
sizeof( Ogre::StaticGeometry::LODBucket );
sizeof( Ogre::Skeleton::LinkedSkeletonAnimSourceList );
sizeof( Ogre::Skeleton::LinkedSkeletonAnimSourceIterator );
sizeof( Ogre::Skeleton::BoneList );
sizeof( Ogre::Skeleton::BoneIterator );
sizeof( Ogre::Skeleton::BoneHandleMap );
sizeof( Ogre::Singleton<Ogre::FontManager> );
sizeof( Ogre::Singleton<Ogre::ExternalTextureSourceManager> );
sizeof( Ogre::SharedPtr<std::vector<Ogre::SharedPtr<Ogre::ScriptToken> > > );
sizeof( Ogre::SharedPtr<std::list<Ogre::SharedPtr<Ogre::ConcreteNode> > > );
sizeof( Ogre::SharedPtr<PlaneOptimalShadowCameraSetup> );
sizeof( Ogre::SharedPtr<Ogre::WaveformControllerFunction> );
sizeof( Ogre::SharedPtr<Ogre::Texture> );
sizeof( Ogre::SharedPtr<Ogre::StringVector> ); //StringVectorPtr;
sizeof( Ogre::SharedPtr<Ogre::ShadowCameraSetup> ); //ShadowCameraSetupPtr;
sizeof( Ogre::SharedPtr<Ogre::Resource> ); //ResourcePtr;
sizeof( Ogre::SharedPtr<Ogre::RenderToVertexBuffer> );
sizeof( Ogre::SharedPtr<Ogre::RenderToVertexBuffer> );
sizeof( Ogre::SharedPtr<Ogre::PSSMShadowCameraSetup> );
sizeof( Ogre::SharedPtr<Ogre::Mesh> );
sizeof( Ogre::SharedPtr<Ogre::MemoryDataStream> );
sizeof( Ogre::SharedPtr<Ogre::Material> );
sizeof( Ogre::SharedPtr<Ogre::GpuProgramParameters> );
sizeof( Ogre::SharedPtr<Ogre::Font> ); //FontPtr;
sizeof( Ogre::SharedPtr<Ogre::FileInfoList> );
sizeof( Ogre::SharedPtr<Ogre::DataStreamList> );
sizeof( Ogre::SharedPtr<Ogre::DataStream> );
sizeof( Ogre::SharedPtr<Ogre::ControllerValue<Ogre::Real> > );
sizeof( Ogre::SharedPtr<Ogre::ControllerFunction<Ogre::Real> > );
sizeof( Ogre::SharedPtr<Ogre::AnimableValue> );
sizeof( Ogre::SharedPtr<Ogre::AbstractNode> );
sizeof( Ogre::SharedPtr<LiSPSMShadowCameraSetup> );
sizeof( Ogre::SharedPtr<FocusedShadowCameraSetup> );
sizeof( Ogre::SharedPtr<DefaultShadowCameraSetup> );
sizeof( Ogre::ShadowTextureList );
sizeof( Ogre::ShadowTextureConfigList );
sizeof( Ogre::ShadowCaster::ShadowRenderableListIterator );
sizeof( Ogre::ShadowCaster::ShadowRenderableList );
sizeof( Ogre::ShadowCameraSetupPtr );
sizeof( Ogre::SceneTypeMask );
sizeof( Ogre::SceneQueryResultWorldFragmentList );
sizeof( Ogre::SceneQueryResultMovableList );
sizeof( Ogre::SceneQueryMovableWorldFragmentIntersectionList );
sizeof( Ogre::SceneQueryMovableObjectWorldFragmentPair );
sizeof( Ogre::SceneQueryMovableObjectPair );
sizeof( Ogre::SceneQueryMovableIntersectionList );
sizeof( Ogre::SceneNode::ObjectMap );
sizeof( Ogre::SceneNode::ObjectIterator );
sizeof( Ogre::SceneNode::ConstObjectIterator );
sizeof( Ogre::SceneManagerEnumerator::SceneManagerIterator );
sizeof( Ogre::SceneManagerEnumerator::MetaDataList );
sizeof( Ogre::SceneManagerEnumerator::MetaDataIterator );
sizeof( Ogre::SceneManagerEnumerator::Instances );
sizeof( Ogre::SceneManager::MovableObjectIterator );
sizeof( Ogre::SceneManager::CameraIterator );
sizeof( Ogre::SceneManager::AnimationIterator );
sizeof( Ogre::Root::PluginLibList );
sizeof( Ogre::Root::PluginInstanceList );
sizeof( Ogre::Root::MovableObjectFactoryIterator );
sizeof( Ogre::RibbonTrail::NodeList );
sizeof( Ogre::RibbonTrail::NodeIterator );
sizeof( Ogre::ResourcePtr );
sizeof( Ogre::ResourceManager::ResourceMapIterator );
sizeof( Ogre::ResourceManager::ResourceMap );
sizeof( Ogre::ResourceManager::ResourceHandleMap );
sizeof( Ogre::ResourceManager::ResourceCreateOrRetrieveResult);
sizeof( Ogre::ResourceHandle );
sizeof( Ogre::ResourceGroupManager::ResourceManagerMap );
sizeof( Ogre::ResourceGroupManager::ResourceManagerIterator );
sizeof( Ogre::ResourceGroupManager::ResourceDeclarationList );
sizeof( Ogre::ResourceBackgroundQueue );
sizeof( Ogre::RenderTargetPriorityMap );
sizeof( Ogre::RenderTargetMap );
sizeof( Ogre::RenderTarget::FrameStats );
sizeof( Ogre::RenderSystemList );
sizeof( Ogre::RenderSystem::RenderTargetIterator );
sizeof( Ogre::RenderSystem );
sizeof( Ogre::RenderQueueInvocationList );
sizeof( Ogre::RenderQueueInvocationIterator );
sizeof( Ogre::RenderQueueGroup::PriorityMapIterator );
sizeof( Ogre::RenderQueueGroup::PriorityMap );
sizeof( Ogre::RenderQueue::RenderQueueGroupMap );
sizeof( Ogre::RenderQueue::QueueGroupIterator );
sizeof( Ogre::Rect );
sizeof( Ogre::Real );
sizeof( Ogre::RaySceneQueryResult );
sizeof( Ogre::RadixSort<std::vector<Ogre::RenderablePass>,Ogre::RenderablePass,unsigned int> );
sizeof( Ogre::RadixSort<std::vector<Ogre::RenderablePass>,Ogre::RenderablePass,float> );
sizeof( Ogre::RadixSort<std::list<Ogre::Particle*>,Ogre::Particle*,float> );
sizeof( Ogre::RadixSort<std::list<Ogre::Billboard*>,Ogre::Billboard*,float> );
sizeof( Ogre::RGBA );
sizeof( Ogre::ProgressiveMesh::LODFaceList );
sizeof( Ogre::PoseList );
sizeof( Ogre::Pose::VertexOffsetMap );
sizeof( Ogre::Pose::VertexOffsetIterator );
sizeof( Ogre::Pose::ConstVertexOffsetIterator );
sizeof( Ogre::Polygon::VertexList );
sizeof( Ogre::Polygon::EdgeMap );
sizeof( Ogre::Polygon::Edge );
sizeof( Ogre::PlaneList );
sizeof( Ogre::PlaneBoundedVolumeList );
sizeof( Ogre::PlaneBoundedVolume::PlaneList );
sizeof( Ogre::Pass::TextureUnitStateIterator );
sizeof( Ogre::Pass::PassSet );
sizeof( Ogre::Pass::ConstTextureUnitStateIterator );
sizeof( Ogre::ParticleSystemManager::ParticleTemplateMap );
sizeof( Ogre::ParticleSystemManager::ParticleSystemTemplateIterator );
sizeof( Ogre::ParticleSystemManager::ParticleSystemRendererFactoryMap );
sizeof( Ogre::ParticleSystemManager::ParticleRendererFactoryIterator );
sizeof( Ogre::ParticleSystemManager::ParticleEmitterFactoryMap );
sizeof( Ogre::ParticleSystemManager::ParticleEmitterFactoryIterator );
sizeof( Ogre::ParticleSystemManager::ParticleAffectorFactoryMap );
sizeof( Ogre::ParticleSystemManager::ParticleAffectorFactoryIterator );
sizeof( Ogre::ParameterList );
sizeof( Ogre::ParamDictionaryMap );
sizeof( Ogre::ParamCommandMap );
sizeof( Ogre::OverlayManager::TemplateIterator );
sizeof( Ogre::OverlayManager::OverlayMapIterator );
sizeof( Ogre::OverlayManager::OverlayMap );
sizeof( Ogre::OverlayManager::ElementMap );
sizeof( Ogre::OverlayContainer::ChildMap );
sizeof( Ogre::OverlayContainer::ChildIterator );
sizeof( Ogre::OverlayContainer::ChildContainerMap );
sizeof( Ogre::OverlayContainer::ChildContainerIterator );
sizeof( Ogre::Overlay::OverlayContainerList );
sizeof( Ogre::Overlay::Overlay2DElementsIterator );
sizeof( Ogre::Node::ConstChildNodeIterator );
sizeof( Ogre::Node::ChildNodeMap );
sizeof( Ogre::Node::ChildNodeIterator );
sizeof( Ogre::NameValuePairList );
sizeof( Ogre::Mesh::VertexBoneAssignmentList );
sizeof( Ogre::Mesh::SubMeshNameMap );
sizeof( Ogre::Mesh::SubMeshNameMap );
sizeof( Ogre::Mesh::SubMeshList );
sizeof( Ogre::Mesh::SubMeshIterator );
sizeof( Ogre::Mesh::PoseIterator );
sizeof( Ogre::Mesh::IndexMap );
sizeof( Ogre::Mesh::ConstPoseIterator );
sizeof( Ogre::Mesh::BoneAssignmentIterator );
sizeof( Ogre::MemoryDataStreamPtr );
sizeof( Ogre::MaterialPtr );
sizeof( Ogre::Material::TechniqueIterator );
sizeof( Ogre::MapIterator<std::map<int, Ogre::InstancedGeometry::InstancedObject*, std::less<int>, std::allocator<std::pair<const int, Ogre::InstancedGeometry::InstancedObject*> > > > );
sizeof( Ogre::MapIterator<std::map<Ogre::String, Ogre::Archive*> > );
sizeof( Ogre::MapIterator< std::multimap<Ogre::String, Ogre::String> > );
sizeof( Ogre::MapIterator< std::map<Ogre::String, std::multimap<Ogre::String, Ogre::String>*> > );
sizeof( Ogre::ManualObject::SectionList );
sizeof( Ogre::LightList );
sizeof( Ogre::InstancedGeometry::SubMeshLodGeometryLinkList );
sizeof( Ogre::InstancedGeometry::SubMeshLodGeometryLink );
sizeof( Ogre::InstancedGeometry::SubMeshGeometryLookup );
sizeof( Ogre::InstancedGeometry::RenderOperationVector );
sizeof( Ogre::InstancedGeometry::QueuedSubMeshOriginList );
sizeof( Ogre::InstancedGeometry::QueuedSubMeshList );
sizeof( Ogre::InstancedGeometry::QueuedGeometryList );
sizeof( Ogre::InstancedGeometry::OptimisedSubMeshGeometryList );
sizeof( Ogre::InstancedGeometry::MaterialBucket::GeometryIterator );
sizeof( Ogre::InstancedGeometry::LODBucket::MaterialIterator );
sizeof( Ogre::InstancedGeometry::LODBucket::MaterialBucketMap );
sizeof( Ogre::InstancedGeometry::InstancedObject::GeometryBucketList );
sizeof( Ogre::InstancedGeometry::BatchInstanceMap );
sizeof( Ogre::InstancedGeometry::BatchInstanceIterator );
sizeof( Ogre::InstancedGeometry::BatchInstance::ObjectsMap );
sizeof( Ogre::InstancedGeometry::BatchInstance::LODIterator );
sizeof( Ogre::InstancedGeometry::BatchInstance::LODBucketList );
sizeof( Ogre::ImagePtrList );
sizeof( Ogre::Image::Rect );
sizeof( Ogre::Image::Box );
sizeof( Ogre::IlluminationPassList );
sizeof( Ogre::HighLevelGpuProgramManager::FactoryMap );
sizeof( Ogre::GpuProgramParametersSharedPtr );
sizeof( Ogre::GpuProgramParameters::AutoConstantIterator );
sizeof( Ogre::GpuProgramManager::SyntaxCodes );
sizeof( Ogre::GpuConstantDefinitionMap );
sizeof( Ogre::GpuConstantDefinitionIterator );
sizeof( Ogre::FloatRect );
sizeof( Ogre::FileInfoListPtr );
sizeof( Ogre::FileInfoList );
sizeof( Ogre::Entity::EntitySet );
sizeof( Ogre::Entity::ChildObjectListIterator );
sizeof( Ogre::Entity::ChildObjectList );
sizeof( Ogre::EnabledAnimationStateList );
sizeof( Ogre::EdgeData::TriangleList );
sizeof( Ogre::EdgeData::TriangleLightFacingList );
sizeof( Ogre::EdgeData::TriangleFaceNormalList );
sizeof( Ogre::EdgeData::EdgeList );
sizeof( Ogre::EdgeData::EdgeGroupList );
sizeof( Ogre::DataStreamPtr );
sizeof( Ogre::DataStreamListPtr );
sizeof( Ogre::DataStreamList );
sizeof( Ogre::ConvexBody::PolygonList );
sizeof( Ogre::ControllerValueRealPtr );
sizeof( Ogre::ControllerValue<float> );
sizeof( Ogre::ControllerFunctionRealPtr );
sizeof( Ogre::ControllerFunction<float> );
sizeof( Ogre::Controller<float> );
sizeof( Ogre::ConstVectorIterator<std::vector<Ogre::Technique::GPUVendorRule, std::allocator<Ogre::Technique::GPUVendorRule> > > );
sizeof( Ogre::ConstVectorIterator<std::vector<Ogre::Technique::GPUDeviceNameRule, std::allocator<Ogre::Technique::GPUDeviceNameRule> > > );
sizeof( Ogre::ConstShadowTextureConfigIterator );
sizeof( Ogre::ConstImagePtrList );
sizeof( Ogre::ConstEnabledAnimationStateIterator );
sizeof( Ogre::ConstAnimationStateIterator );
sizeof( Ogre::ConfigOptionMap );
sizeof( Ogre::ConfigOption );
sizeof( Ogre::ConfigFile::SettingsMultiMap );
sizeof( Ogre::ConfigFile::SettingsIterator );
sizeof( Ogre::ConfigFile::SettingsBySection );
sizeof( Ogre::ConfigFile::SectionIterator );
sizeof( Ogre::CompositorInstance::TargetOperation::RenderQueueBitSet );
sizeof( Ogre::CompositorInstance::RenderSystemOpPairs );
sizeof( Ogre::CompositorInstance::RenderSystemOpPair );
sizeof( Ogre::CompositorInstance::QuadMaterialMap );
sizeof( Ogre::CompositorInstance::CompiledState );
sizeof( Ogre::CompositorChain::Instances );
sizeof( Ogre::CompositorChain::InstanceIterator );
sizeof( Ogre::Compositor::Techniques );
sizeof( Ogre::Compositor::TechniqueIterator );
sizeof( Ogre::CompositionTechnique::TextureDefinitions );
sizeof( Ogre::CompositionTechnique::TextureDefinitionIterator );
sizeof( Ogre::CompositionTechnique::TargetPasses );
sizeof( Ogre::CompositionTechnique::TargetPassIterator );
sizeof( Ogre::CompositionTargetPass::Passes );
sizeof( Ogre::CompositionTargetPass::PassIterator );
sizeof( Ogre::BufferUsageList );
sizeof( Ogre::BinaryOptionList );
sizeof( Ogre::BillboardChain::ElementList );
sizeof( Ogre::BackgroundProcessTicket );
sizeof( Ogre::BGRA );
sizeof( Ogre::AnimationStateMap );
sizeof( Ogre::AnimationStateIterator );
sizeof( Ogre::Animation::VertexTrackList );
sizeof( Ogre::Animation::VertexTrackIterator );
sizeof( Ogre::Animation::TrackHandleList );
sizeof( Ogre::Animation::NumericTrackList );
sizeof( Ogre::Animation::NumericTrackIterator );
sizeof( Ogre::Animation::NodeTrackList );
sizeof( Ogre::Animation::NodeTrackIterator );
sizeof( Ogre::AnimableValuePtr );
sizeof( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)7> > );
sizeof( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)6> > );
sizeof( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)5> > );
sizeof( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)4> > );
sizeof( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)3> > );
sizeof( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)2> > );
sizeof( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)1> > );
sizeof( Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > );
sizeof( Ogre::AliasTextureNamePairList );
sizeof( Ogre::ATTRIBUTE_PARSER );
sizeof( Ogre::ARGB );
sizeof( Ogre::ABGR );

sizeof( ::pyplusplus::aliases::SubMeshLodGeometryLinkList );
sizeof( ::pyplusplus::aliases::InstSubMeshLodGeometryLinkList );
sizeof( Ogre::Singleton<Ogre::DynLibManager> );


