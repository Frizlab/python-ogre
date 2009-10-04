// typedef name nicename;
typedef ::Ogre::Singleton< Ogre::PCZoneFactoryManager > SingletonPCZoneFactoryManager;
typedef std::list<Ogre::Portal*, Ogre::STLAllocator<Ogre::Portal*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdListPortal;
typedef std::set<Ogre::PCZSceneNode*, std::less<Ogre::PCZSceneNode*>, Ogre::STLAllocator<Ogre::PCZSceneNode*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdSetPCZSceneNode;
typedef std::vector<Ogre::SceneNode*, Ogre::STLAllocator<Ogre::SceneNode*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdVectorSceneNode;
typedef Ogre::MapIterator<std::map<std::string, Ogre::PCZone*, std::less<std::string>, Ogre::STLAllocator<std::pair<std::string const, Ogre::PCZone*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > > MapIteratorPCZone;
typedef Ogre::MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::PCZoneFactory*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, Ogre::STLAllocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::PCZoneFactory*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > >  MapIteratorStringPCZoneFactory;


#if(OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR > 6)
    typedef std::map<std::string, Ogre::PCZone*, std::less<std::string>, Ogre::STLAllocator<std::pair<std::string const, Ogre::PCZone*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdMapStringPCZone;
    typedef std::list<Ogre::AntiPortal*, Ogre::STLAllocator<Ogre::AntiPortal*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdListAntiPortal;
#endif    
