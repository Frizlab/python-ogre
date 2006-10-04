#ifndef __python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__
#define __python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__

//See best practices section in Py++ documentation
#define OGRE_VERSION_CVS
#include "Ogre.h"

namespace python_ogre{ namespace details{

inline void instantiate(){
    using namespace Ogre;
    
    sizeof( Controller<float> );

    sizeof( SharedPtr< Controller<float> >);

    sizeof( SharedPtr< WaveformControllerFunction > );
    // lets clean up some of the file names
    sizeof( SharedPtr<Ogre::Resource> );
    sizeof ( std::vector<Ogre::Vector4, std::allocator< Ogre::Vector4> > );

    // Needed for AliasTextureIterator
    sizeof( ConstMapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > > );
    sizeof( std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > );    			
    sizeof (MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >*> > > >);
#ifdef OGRE_VERSION_CVS
    sizeof (std::multimap<Ogre::RenderWindow*,Ogre::WindowEventListener*,std::less<Ogre::RenderWindow*>,std::allocator<std::pair<Ogre::RenderWindow* const, Ogre::WindowEventListener*> > >);
    sizeof ( std::multimap<RenderWindow*, WindowEventListener*> ) ; // WindowEventListeners;
#endif

    // this is needed to expose within DataStream.pypp.cpp - required as a return value 
    // from with in resource group manager
    sizeof( SharedPtr<DataStream> );
    
    sizeof( SharedPtr<ControllerValueRealPtr> );
    sizeof( SharedPtr<ControllerFunctionRealPtr> );
    
    sizeof ( std::map<String, String> );    // NameValuePairList
    
    // needed for configfile section iterator..
    sizeof ( MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >*> > > > );
    // needed for configfile settings iterator..
    sizeof ( MapIterator<std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > >);
    // SettingsBySection
    sizeof ( std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >,std::basic_string<char, std::char_traits<char>, std::allocator<char> >,std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >,std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >);
    // Section Iterator - getnext..
    sizeof ( MapIterator<std::map<std::string, std::multimap<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >*, std::less<std::string>, std::allocator<std::pair<std::string const, std::multimap<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >*> > > > );
    
 
}

} } //python_ogre::details


#endif//__python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__
