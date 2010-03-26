// typedef name nicename;
typedef std::vector<Ogre::Terrain::LayerInstance, Ogre::STLAllocator<Ogre::Terrain::LayerInstance, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdVectorTerrainLayerInstance;
typedef std::vector<Ogre::TerrainLayerSampler, Ogre::STLAllocator<Ogre::TerrainLayerSampler, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdVectorTerrainLayerSampler;
typedef std::vector<Ogre::TerrainLayerSamplerElement, Ogre::STLAllocator<Ogre::TerrainLayerSamplerElement, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdVectorTerrainLayerSamplerElement;
typedef std::vector<Ogre::TerrainMaterialGenerator::Profile*, Ogre::STLAllocator<Ogre::TerrainMaterialGenerator::Profile*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdVectorTerrainMaterialGenerator;
typedef std::list<Ogre::HardwareVertexBufferSharedPtr, Ogre::STLAllocator<Ogre::HardwareVertexBufferSharedPtr, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >\
    stdListOgreHardwareVertexBufferSharedPtr;
typedef std::vector<Ogre::Terrain*,Ogre::STLAllocator<Ogre::Terrain*, Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > >\
    stdVectorOgreTerrain;
    
// typedef Ogre::vector<Ogre::Terrain*, Ogre::STLAllocator<Ogre::Terrain*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > \
//     OgreVectorTerrainPtr;
typedef Ogre::vector<Ogre::Terrain*, Ogre::STLAllocator<Ogre::Terrain*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > \
    TerrainList;
    
typedef Ogre::vector<Ogre::TerrainQuadTreeNode::LodLevel*, Ogre::STLAllocator<Ogre::TerrainQuadTreeNode::LodLevel*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >\
    OgreVectorTerrainQuadTreeNode;
typedef Ogre::vector<Ogre::Terrain::LayerInstance, Ogre::STLAllocator<Ogre::Terrain::LayerInstance, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > \
    OgreVectorTerrainLayerBlendMap;
typedef Ogre::vector<Ogre::TerrainLayerBlendMap*, Ogre::STLAllocator<Ogre::TerrainLayerBlendMap*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > \
    OgreVectorTerrainLayerBlendMapPtr;
typedef Ogre::vector<Ogre::TerrainMaterialGenerator::Profile*, Ogre::STLAllocator<Ogre::TerrainMaterialGenerator::Profile*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > \
    OgreVectorTerrainMaterialGeneratorProfilePtr;
typedef Ogre::vector<Ogre::TerrainLayerSampler, Ogre::STLAllocator<Ogre::TerrainLayerSampler, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > \
    OgreVectorTerrainLayerSampler;
typedef Ogre::vector<Ogre::TerrainLayerSamplerElement, Ogre::STLAllocator<Ogre::TerrainLayerSamplerElement, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > \
    OgreVectorTerrainLayerSamplerElement;
typedef Ogre::Singleton<Ogre::TerrainGlobalOptions> OgreSingletonTerrainGlobalOptions;
typedef std::map<Ogre::uint32, Ogre::TerrainGroup::TerrainSlot*> TerrainSlotMap;

typedef Ogre::map<unsigned int, Ogre::TerrainGroup::TerrainSlot*, std::less<unsigned int>, Ogre::STLAllocator<std::pair<unsigned int const, Ogre::TerrainGroup::TerrainSlot*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > \
    OgreMapTerrainGroupTerrainSlotPtr;
    
typedef ::std::map<unsigned int, Ogre::TerrainGroup::TerrainSlot*, std::less<unsigned int>, Ogre::STLAllocator<std::pair<unsigned int const, Ogre::TerrainGroup::TerrainSlot*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > \
    stdMapOgreTerrainGroupTerrainSlotPtr;
typedef Ogre::IteratorWrapper<std::map<unsigned int, Ogre::TerrainGroup::TerrainSlot*, std::less<unsigned int>, Ogre::STLAllocator<std::pair<unsigned int const, Ogre::TerrainGroup::TerrainSlot*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >, std::_Tree<std::_Tmap_traits<unsigned int, Ogre::TerrainGroup::TerrainSlot*, std::less<unsigned int>, Ogre::STLAllocator<std::pair<unsigned int const, Ogre::TerrainGroup::TerrainSlot*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> >, false> >::const_iterator, Ogre::TerrainGroup::TerrainSlot*>\
    OgreConstIteratorWrapperMapUnsignedIntTerrainGroupSlot;
typedef Ogre::IteratorWrapper<std::map<unsigned int, Ogre::TerrainGroup::TerrainSlot*, std::less<unsigned int>, Ogre::STLAllocator<std::pair<unsigned int const, Ogre::TerrainGroup::TerrainSlot*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >, std::_Tree<std::_Tmap_traits<unsigned int, Ogre::TerrainGroup::TerrainSlot*, std::less<unsigned int>, Ogre::STLAllocator<std::pair<unsigned int const, Ogre::TerrainGroup::TerrainSlot*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> >, false> >::iterator, Ogre::TerrainGroup::TerrainSlot*>\
    OgreIteratorWrapperMapUnsignedIntTerrainGroupSlot;
typedef Ogre::MapIteratorWrapper<std::map<unsigned int, Ogre::TerrainGroup::TerrainSlot*, std::less<unsigned int>, Ogre::STLAllocator<std::pair<unsigned int const, Ogre::TerrainGroup::TerrainSlot*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >, std::_Tree<std::_Tmap_traits<unsigned int, Ogre::TerrainGroup::TerrainSlot*, std::less<unsigned int>, Ogre::STLAllocator<std::pair<unsigned int const, Ogre::TerrainGroup::TerrainSlot*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> >, false> >::const_iterator>\
    OgreConstMapIteratorMapUnsignedIntTerrainGroupSlot;
typedef Ogre::MapIteratorWrapper<std::map<unsigned int, Ogre::TerrainGroup::TerrainSlot*, std::less<unsigned int>, Ogre::STLAllocator<std::pair<unsigned int const, Ogre::TerrainGroup::TerrainSlot*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > >, std::_Tree<std::_Tmap_traits<unsigned int, Ogre::TerrainGroup::TerrainSlot*, std::less<unsigned int>, Ogre::STLAllocator<std::pair<unsigned int const, Ogre::TerrainGroup::TerrainSlot*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> >, false> >::iterator>\
    OgreMapIteratorMapUnsignedIntTerrainGroupSlot;


