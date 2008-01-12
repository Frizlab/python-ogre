typedef std::string String;
typedef std::map<std::string, std::string> MapStringString;
typedef std::pair<float, float> StdPairFloatFloat;

typedef std::vector<int> stdVectorInt;
typedef std::vector<float> stdVectorFloat;
typedef std::vector<std::string> NameList;
typedef std::vector<Ogre::Vector4> VectorVector4;

typedef std::pair<bool, float> StdPairBoolFloat;
typedef std::pair<bool, std::allocator<Ogre::Vector3> > stdPairBoolVector;

//1
//typedef std::vector<Ogre::ColourValue, std::allocator<Ogre::ColourValue> > ColourList;
//typedef std::vector<Ogre::Image, std::allocator<Ogre::Image> > ImageList;

//2
//typedef std::vector<Ogre::ColourValue> ColourList;
//typedef std::vector<Ogre::Image> ImageList;

//3
// typedef ET::ColourList ColourList;
// typedef ET::ImageList ImageList;

// these are uglys to remove messages in the generate_code output - doesn't impact the generated code :)
//typedef std::vector<Ogre::Image> ImageList;

//typedef std::vector<Ogre::Image, std::allocator<Ogre::Image> > ImageList;
//typedef std::vector<Ogre::ColourValue, std::allocator<Ogre::ColourValue> > ColourList;

//typedef std::vector<Ogre::Image, std::less<Ogre::Image>, std::allocator<Ogre::Image> > \
//    ImageList;

//typedef std::vector<Ogre::Image> ImageList;

//typedef std::vector<Ogre::ColourValue> ColourList;
//typedef std::vector<Ogre::ColourValue, std::allocator<Ogre::ColourValue> >;

