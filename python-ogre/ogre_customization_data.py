name2alias = {
    "map<std::string, bool, std::less<std::string>, std::allocator<std::pair<std::string const, bool> > >":
        "MapOfStringToBool",
    "map<std::string, Ogre::OverlayElement*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::OverlayElement*> > >":
        "MapOfStringToOverlayElement",
    "map<std::string, Ogre::MovableObject*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > >":
        "MapOfStringToMovableObject",
    "map<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >":
        "MapOfStringToString",
    "vector<float, std::allocator<float> >":
        "VectorOfFloat",
    "vector<std::string, std::allocator<std::string> >":
        "VectorOfString",
}

name2alias_class_decl = {
    "map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >,Ogre::_ConfigOption,std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >,std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::_ConfigOption> > >":
        "MapOfStringToConfigOption",
    "vector<Ogre::StaticGeometry::SubMeshLodGeometryLink,std::allocator<Ogre::StaticGeometry::SubMeshLodGeometryLink> >":
        "VectorOfSubMeshLodGeometryLink",
    "vector<Ogre::HardwareBuffer::Usage,std::allocator<Ogre::HardwareBuffer::Usage> >":
        "VectorOfHardwareBufferUsage",
    "vector<const Ogre::Image*,std::allocator<const Ogre::Image*> >":
        "VectorOfImagePtr"

}

# vim:et:ts=4:sts=4:sw=4

