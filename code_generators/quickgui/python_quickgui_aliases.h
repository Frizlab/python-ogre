// typedef name nicename;
typedef std::vector<QuickGUI::Widget*, std::allocator<QuickGUI::Widget*> > VectorWidget;
typedef std::vector<unsigned short, std::allocator<unsigned short> > VectorShort;

//typedef std::list<QuickGUI::RenderObject*, std::allocator<QuickGUI::RenderObject*> > ListRenderObject;
typedef std::map<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >\
    MapPairString;
typedef std::list<QuickGUI::Quad*, std::allocator<QuickGUI::Quad*> > ListQuad;    
typedef std::vector<QuickGUI::Vertex, std::allocator<QuickGUI::Vertex> > VectorVertex;
typedef std::vector<std::string, std::allocator<std::string> > VectorString;    
typedef std::list<QuickGUI::QuadContainer*, std::allocator<QuickGUI::QuadContainer*> > QuadContainerList;
