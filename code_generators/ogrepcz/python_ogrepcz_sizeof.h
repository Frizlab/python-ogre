// sizeof ( variable );
sizeof( Ogre::PCZCamera );
sizeof ( Ogre::ZoneData );
sizeof ( Ogre::Singleton<Ogre::PCZoneFactoryManager> );
sizeof ( Ogre::MapIterator<std::map<std::string, Ogre::PCZone*, std::less<std::string>, Ogre::STLAllocator<std::pair<std::string const, Ogre::PCZone*>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > >);
sizeof ( Ogre::MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::PCZoneFactory*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, Ogre::STLAllocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::PCZoneFactory*>, Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL> > > > );
#if(OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR > 6)
    sizeof ( Ogre::AntiPortal );
    sizeof ( Ogre::Capsule );
    sizeof ( Ogre::Segment );
#endif
