// typedef name nicename;
typedef  Ogre::Singleton<QuickGUI::GUIManager> SingletonGUIManager;
typedef  Ogre::Singleton<QuickGUI::MouseCursor> SingletonMouseCursor;
typedef std::vector<QuickGUI::Widget*, std::allocator<QuickGUI::Widget*> > VectorWidget;

typedef std::list<QuickGUI::RenderObject*, std::allocator<QuickGUI::RenderObject*> > ListRenderObject;
typedef std::map<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >\
    MapPairString;
typedef std::vector<QuickGUI::Vertex, std::allocator<QuickGUI::Vertex> > VectorVertex;
typedef std::vector<std::string, std::allocator<std::string> > VectorString;    
