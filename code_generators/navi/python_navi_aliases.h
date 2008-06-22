typedef std::string String;
typedef NaviLibrary::NaviUtilities::InlineVector<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >\
     InlineVectorString;
// typedef std::map<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >\
//     MapStringString;
typedef std::vector<std::string, std::allocator<std::string> > VectorString;
// typedef NaviLibrary::NaviUtilities::InlineVector<std::string> InlineVectorStrings;
typedef NaviLibrary::NaviUtilities::InlineVector<NaviLibrary::NaviUtilities::MultiValue> InlineVectorArgs;
typedef NaviLibrary::Singleton< NaviLibrary::NaviManager > SingletonManager;
typedef NaviLibrary::Singleton< NaviLibrary::NaviMouse > SingletonMouse;
typedef NaviLibrary::FastDelegate1< NaviLibrary::NaviData const&, void > FastDelegateNaviData;