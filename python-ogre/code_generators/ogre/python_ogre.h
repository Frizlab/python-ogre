#ifndef __python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__
#define __python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__

//See best practices section in Py++ documentation
#include "Ogre.h"
#ifndef OGRE_VERSION_CVS        // these are needed for the 1.2.x SDK
    #include "OgreEventListeners.h"
    #include "OgreEventQueue.h"
    #include "OgreKeyEvent.h"
    #include "OgreMouseEvent.h"

namespace Ogre {
class CombinedListener : public Ogre::FrameListener, public Ogre::KeyListener,
                 public Ogre::MouseListener, public Ogre::MouseMotionListener
{
public:
    CombinedListener() {}
    virtual ~CombinedListener() {}
    
    // KeyListener methods
    virtual void keyClicked(Ogre::KeyEvent *e) {}
    virtual void keyPressed(Ogre::KeyEvent *e) {}
    virtual void keyReleased(Ogre::KeyEvent *e) {}
    
    // MouseListener methods
    virtual void mouseClicked(Ogre::MouseEvent *e) {}
    virtual void mousePressed(Ogre::MouseEvent *e) {}
    virtual void mouseReleased(Ogre::MouseEvent *e) {}
    virtual void mouseEntered(Ogre::MouseEvent *e) {}
    virtual void mouseExited(Ogre::MouseEvent *) {}
    
    // MouseMotionListener methods
    virtual void mouseMoved(Ogre::MouseEvent *e) {}
    virtual void mouseDragged(Ogre::MouseEvent *e) {}
    
    // FrameEvent methods
    virtual bool frameStarted (const FrameEvent &evt) {return true;}
    virtual bool frameEnded (const FrameEvent &evt) {return true;}
};

}
#endif

namespace python_ogre{ namespace details{

inline void instantiate(){
    using namespace Ogre;
    
    sizeof( Controller<float> );

    sizeof( SharedPtr< Controller<float> >);

    sizeof( SharedPtr< WaveformControllerFunction > );
    sizeof ( AnimableValue );
    sizeof ( SharedPtr<AnimableValue>  );
    sizeof ( AnimableValuePtr );

    sizeof ( Ogre::DataStream );
    sizeof ( Ogre::DataStreamPtr );
    sizeof ( Ogre::FileInfo );
    sizeof ( std::pair< std::wstring, std::wstring > );
    sizeof ( std::list<Ogre::AnimationState*, std::allocator<Ogre::AnimationState*> > );
    sizeof ( Ogre::SharedPtr<Ogre::DataStream> );
    sizeof ( Ogre::SharedPtr<std::vector<std::string, std::allocator<std::string> > > );
//     sizeof ( Ogre::SharedPtr<std::vector<Ogre::FileInfo, std::allocator<Ogre::FileInfo> > > ); ## problem exposing
    sizeof ( Ogre::Singleton<Ogre::ArchiveManager> );
    sizeof ( Ogre::MaterialPtr );
    sizeof ( Ogre::TexturePtr );

    
        // lets clean up some of the file names
    sizeof( SharedPtr<Ogre::Resource> );
    sizeof ( std::vector<Ogre::Vector4, std::allocator< Ogre::Vector4> > );
    sizeof ( Ogre::StringVectorPtr );
    sizeof ( Ogre::StringVector );
    
    sizeof ( SharedPtr<Ogre::StringVectorPtr> );
    sizeof ( SharedPtr<Ogre::StringVector> );
    
    // Ogre::ConstMapIterator requirements
    sizeof ( std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, 
                    Ogre::AnimationState*, std::less<std::basic_string<char, std::char_traits<char>, 
                    std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, 
                    std::char_traits<char>, std::allocator<char> >, Ogre::AnimationState*> > > );
    sizeof ( Ogre::ConstMapIterator<std::map<std::basic_string<char, std::char_traits<char>, 
                    std::allocator<char> >, Ogre::AnimationState*, std::less<std::basic_string<char, 
                    std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, 
                    std::char_traits<char>, std::allocator<char> >, Ogre::AnimationState*> > > > );
    sizeof ( Ogre::ConstVectorIterator<std::list<Ogre::AnimationState*, std::allocator<Ogre::AnimationState*> > > );
    
    sizeof ( std::map<unsigned int,class Ogre::StaticGeometry::Region *,struct std::less<unsigned int>,
                    class std::allocator<struct std::pair<unsigned int const ,class Ogre::StaticGeometry::Region *> > >);
    
    // StaticGeometry Iterator
    sizeof ( Ogre::MapIterator<std::map<unsigned int, Ogre::StaticGeometry::Region*, std::less<unsigned int>, 
                    std::allocator<std::pair<const unsigned int, Ogre::StaticGeometry::Region*> > > > ) ;
    
    // Needed for AliasTextureIterator
    sizeof( ConstMapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > > );
    sizeof( std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > );
    // sharedptr<string vector >
    sizeof (std::vector<std::string, std::allocator<std::string> >);
    sizeof (SharedPtr<std::vector<std::string, std::allocator<std::string> > >);
    sizeof (std::vector<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > >  );
    sizeof( Ogre::SharedPtr<std::vector<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >  );

    sizeof (MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > >*> > > >);
#ifdef OGRE_VERSION_CVS
    sizeof (std::multimap<Ogre::RenderWindow*,Ogre::WindowEventListener*,std::less<Ogre::RenderWindow*>,std::allocator<std::pair<Ogre::RenderWindow* const, Ogre::WindowEventListener*> > >);
    sizeof ( std::multimap<RenderWindow*, WindowEventListener*> ) ; // WindowEventListeners;
#endif

   // needed for moveablemapeiterator
//    	sizeof (MapIterator<std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >,Ogre::MovableObject*,std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >,std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, Ogre::MovableObject*> > > >);
//    	sizeof (std::vector<std::list<std::pair<std::string const, Ogre::MovableObject*>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > >::iterator, std::allocator<std::list<std::pair<std::string const, Ogre::MovableObject*>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > >::iterator> >);
////
sizeof(Ogre::MapIterator<std::map<std::string, std::multimap<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >*, std::less<std::string>, std::allocator<std::pair<std::string const, std::multimap<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >*> > > > );
sizeof(Ogre::MapIterator<std::multimap<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > > > );
sizeof(Ogre::MapIterator<std::map<std::string, Ogre::MovableObject*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::MovableObject*> > > > );
sizeof(Ogre::MapIterator<std::map<std::string, Ogre::Animation*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::Animation*> > > >);
sizeof(Ogre::MapIterator<std::map<std::string, Ogre::Camera*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::Camera*> > > >);
sizeof(Ogre::MapIterator<std::map<std::string, Ogre::RenderTarget*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::RenderTarget*> > > >);
sizeof(Ogre::MapIterator<std::map<unsigned short, Ogre::RenderPriorityGroup*, std::less<unsigned short>, std::allocator<std::pair<unsigned short const, Ogre::RenderPriorityGroup*> > > >);
sizeof(Ogre::MapIterator<std::map<std::string, Ogre::ParticleSystem*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::ParticleSystem*> > > >);
sizeof(Ogre::MapIterator<std::map<std::string, Ogre::OverlayElement*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::OverlayElement*> > > >);
sizeof(Ogre::MapIterator<std::map<unsigned long, Ogre::SharedPtr<Ogre::Resource>, std::less<unsigned long>, std::allocator<std::pair<unsigned long const, Ogre::SharedPtr<Ogre::Resource> > > > >);
sizeof(Ogre::MapIterator<std::map<std::string, Ogre::ResourceManager*, std::less<std::string>, std::allocator<std::pair<std::string const, Ogre::ResourceManager*> > > >);

    
//    	typedef std::map<String, Ogre::MovableObject*> MovableObjectMap;
// 	typedef MapIterator<MovableObjectMap> MovableObjectIterator;
//     sizeof ( MovableObjectIterator);
    

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
    sizeof ( std::vector<String> );
    sizeof ( SharedPtr< std::vector<String> > );

}

} } //python_ogre::details


#endif//__python_ogre_h_5B402FBF_8DE8_49C9_AF71_1CF9634B5344__
