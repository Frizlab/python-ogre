// typedef name nicename;
//typedef ::Ogre::Singleton< Ogre::PCZoneFactoryManager > SingletonPCZoneFactoryManager;

//---------------------------
//PagingLandScapePreRes
//---------------------------
// Check the following defined in ogre
//typedef std::list < MovableObject* > MovableObjectList;
//typedef std::list < Renderable* > RenderableList;

typedef std::list< Ogre::PagingLandScapeCamera* > PagingLandScapeCameraList;
typedef std::list< Ogre::PagingLandScapeData2D* > PagingLandScapeData2DList;
typedef std::list<Ogre::OcclusionElement*> OcclusionElementList;
typedef std::list<Ogre::PagingLandScapeOctreeNode*> PagingLandScapeOctreeNodeList;
typedef std::list< Ogre::PagingLandScapePage* > PagingLandScapePageList;
typedef std::list< Ogre::PagingLandScapeTile* > PagingLandScapeTileList;
typedef std::list< Ogre::SceneNode* > stdSceneList;

typedef std::vector< Ogre::PagingLandScapePage* > PagingLandScapePageArray;
typedef Ogre::PagingLandScapePageArray PagingLandScapePageRow;
typedef std::vector< Ogre::PagingLandScapePageRow > PagingLandScapePages;

typedef std::vector< Ogre::PagingLandScapeData2D* > PagingLandScapeData2DArray;
//typedef std::vector< Ogre::PagingLandScapeData2D* > PagingLandScapeData2DRow;
//typedef std::vector< Ogre::PagingLandScapeData2D*>  PagingLandScapeData2DMap;
typedef std::vector< Ogre::PagingLandScapeData2DRow > PagingLandScapeData2DPages;
typedef std::vector< Ogre::PagingLandScapeTile* > PagingLandScapeTileRow;
typedef std::vector< Ogre::PagingLandScapeTileRow > PagingLandScapeTiles;



//---------------------------
//General
//---------------------------
typedef std::vector <Ogre::Real> stdVectorReal;
//typedef std::vector <Ogre::ColourValue> stdVectorColourValue;

//---------------------------
//PagingLandScape Options
//---------------------------
//typedef std::vector <Ogre::ColourValue>	matColor;
//typedef std::vector <Ogre::Real> matHeight;
//typedef stdVectorColourValue matColor;
typedef stdVectorReal matHeight;

typedef	std::vector<Ogre::String> SplatDetailMapNames;

//---------------------------
//PagingLandScape TileInfo
//---------------------------
//typedef std::vector<Ogre::Real>* mMinLevelDistSqr;
typedef stdVectorReal mMinLevelDistSqr;

//---------------------------
//PagingLandScapeTextureManager
//---------------------------
//typedef std::vector<Real> heights;
//typedef	std::vector<Real> dividers;
//typedef std::vector<ColourValue> colors;
typedef stdVectorReal heights;
typedef stdVectorReal dividers;
//typedef stdVectorColourValue colours;
//TODO
//const std::vector<Real> *channelModifList;


typedef Ogre::PagingLandScapeQueue<Ogre::PagingLandScapePage> PagingLandScapeQueuePage;
//typedef std::list<Ogre::PagingLandScapePage *>  MsgQueTypePageList; //defined above as PagingLandScapePageList
typedef Ogre::distanceToBoxSort<Ogre::PagingLandScapePage>  distanceToBoxSortPage;

typedef Ogre::PagingLandScapeQueue<Ogre::PagingLandScapeTile> PagingLandScapeQueueTile;
//typedef std::list<Ogre::PagingLandScapeTile *>  MsgQueTypeTileList; // defined above as PagingLandScapeTileList
typedef Ogre::distanceToBoxSort<Ogre::PagingLandScapeTile>  distanceToBoxSortTile;


typedef Ogre::PoolSet<Ogre::PagingLandScapeOctree> PagingLandScapeOctreePoolSet;
typedef Ogre::PoolSet<Ogre::HardwareOcclusionQuery> HardwareOcclusionQueryPoolSet;
typedef Ogre::PoolSet<Ogre::PagingLandScapeRenderable> PagingLandScapeRenderablePoolSet;

typedef fastdelegate::FastDelegate1<Ogre::PagingLandscapeEvent *, void> PagingLandscapeDelegate;