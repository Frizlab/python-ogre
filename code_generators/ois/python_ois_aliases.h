// placeholder for any aliaiss that are needed
typedef std::string String;
typedef OIS::ParamList ParamList;
typedef std::vector<OIS::Axis> AxisVector;
//typedef OIS::SupportedEffectList SupportedEffectList;
typedef std::multimap<OIS::Type,std::basic_string<char, std::char_traits<char>, std::allocator<char> >,std::less<OIS::Type>,std::allocator<std::pair<const OIS::Type, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >\
    multimapOISType;
typedef std::vector<bool> stdVectorBool; // nicer name than _Bvector

#if defined( __WIN32__ ) || defined( _WIN32 )
    typedef std::_Vb_reference<unsigned int, int, std::vector<bool, std::allocator<bool> > > stdVectorBoolReference; //
#endif
