/*-----------------------------------------------------------------------------
WARNING!!! main.cpp, Appliation.h, and Appliation.cpp are TEMPORARY source files
that demonstrate PagedGeometry. They are extremely messy, unstructured, and they
should NOT be taken as an example. This is the beta version of PagedGeometry 1.0
and does not include the examples and tutorials that the final version will have.
-----------------------------------------------------------------------------*/

//Application.cpp
//-------------------------------------------------------------------------------------

#include "Application.h"
#include "ExampleFrameListener.h"

#include "OgreRoot.h"
#include "OgreFrameListener.h"
#include "OgreCamera.h"
#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"
#include "OgreSceneQuery.h"
#include "OgreRay.h"
using namespace Ogre;

#include "PagedGeometry.h"
#include "ImpostorPage.h"
#include "BatchPage.h"

#include "TreeLoader2D.h"
#include "TreeLoader3D.h"
#include "GrassLoader.h"


//Global pointer to the Application object
Application *app;
std::string mResourcePath;

bool GrassFrameListener::frameStarted(const FrameEvent& evt)
{
	app->trees->update();
	app->grass->update();
	grassloader->updateAnimation();
	return true; 
}

//Standard constructor
Application::Application()
{
	frameListener = 0;
	root = 0;
	
	//set resource path depending on platform
	mResourcePath = getResourcePath();
}

//Standard destructor
Application::~Application()
{
	if (frameListener)
		delete frameListener;
	if (root)
		delete root;
}

//Starts application. Called automatically from main()
void Application::go()
{
	//Initialize
	if (!setup())
		return;
	
	//Load world
	createScene();
	
	//Start render loop
	root->startRendering();
	
	//Unload world
	destroyScene();
}

//Initializes OGRE scene manager, resources, cameras, viewports, etc.
bool Application::setup()
{
	//Create Ogre::Root
	root = new Root();
	
	// OSX does not have a working config dialog at the moment
	#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		root->loadPlugin("RenderSystem_GL");
		root->loadPlugin("Plugin_CgProgramManager");
		root->loadPlugin("Plugin_BSPSceneManager");
		root->loadPlugin("Plugin_OctreeSceneManager");
		root->setRenderSystem(root->getAvailableRenderers()->front());
		root->getRenderSystem()->setConfigOption("RTT Preferred Mode", "RTT");
		root->getRenderSystem()->setConfigOption("Full Screen", "No");
		root->getRenderSystem()->setConfigOption("Video Mode", "800 x 600");
		window = root->initialise(true);
	#else
		//Show initialization dialog
		if(root->showConfigDialog())
			window = root->initialise(true);
		else
			return false;
	#endif
	
	//Select scene manager
	sceneMgr = root->createSceneManager(ST_EXTERIOR_CLOSE, "SceneManager");

	//Create the camera
	camera = sceneMgr->createCamera("MainCamera");
	camera->setPosition(Vector3(0,0,0));
	camera->lookAt(Vector3(0,0,1));
	camera->setNearClipDistance(1);

	//Create a viewport
	Viewport* vp = window->addViewport(camera);
	vp->setBackgroundColour(ColourValue(0,0,0));

	//Alter the camera aspect ratio to match the viewport
	camera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

    //Set default mipmap level (NB some APIs ignore this)
    TextureManager::getSingleton().setDefaultNumMipmaps(5);

	//Load media (materials, textures, models, etc.)
	loadMedia();

	//Create frame listener
	frameListener = new ExampleFrameListener(window, camera);
	root->addFrameListener(frameListener);

	return true;
}

//Load media (materials, textures, models, etc.)
void Application::loadMedia()
{
    //Load resource paths from config file
    std::string resource_cfg = mResourcePath + "resources.cfg";
	ConfigFile cf;
    cf.load(resource_cfg);

    //Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(
                 mResourcePath + archName, typeName, secName);
        }
    }
/*
	// Load resources
	const String resourceGrpName ("PagedGeometry");
	ResourceGroupManager *rsm = ResourceGroupManager::getSingletonPtr();
	StringVector groups = rsm->getResourceGroups();        
	if (std::find(groups.begin(), groups.end(), resourceGrpName) == groups.end())
	{
		FileInfoListPtr finfo =  ResourceGroupManager::getSingleton().findResourceFileInfo (
			"Bootstrap", "axes.mesh");
		const bool isSDK =  (!finfo->empty()) && 
			StringUtil::startsWith (finfo->begin()->archive->getName(), "../../media/packs/ogrecore.zip", true);

		rsm->createResourceGroup(resourceGrpName);

		String basePath;
		if (isSDK)
			basePath = "../../../";
		else
			basePath = "../../../../../ogreaddons/";

		rsm->addResourceLocation (basePath + "PagedGeometry/examples/media/terrain","FileSystem", resourceGrpName);
		rsm->addResourceLocation (basePath + "PagedGeometry/examples/media/trees","FileSystem", resourceGrpName);
		rsm->addResourceLocation (basePath + "PagedGeometry/examples/media/grass","FileSystem", resourceGrpName);
	}
*/
	//Now load the resources
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}


//Loads world
void Application::createScene()
{
	// Set ambient light
	sceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	// Create a light
	Light* l = sceneMgr->createLight("MainLight");
	// Accept default settings: point light, white diffuse, just set position
	// NB I could attach the light to a SceneNode if I wanted it to move automatically with
	//  other objects, but I don't
	l->setPosition(20,80,50);
	//l->setDiffuseColour(1, 1, 1);
	//l->setSpecularColour(1, 0, 0);

	// Fog
	// NB it's VERY important to set this before calling setWorldGeometry 
	// because the vertex program picked will be different
	//ColourValue fadeColour(0.93, 0.86, 0.76);
	ColourValue fadeColour(120.0f/255, 173.0f/255, 246.0f/255);
	sceneMgr->setFog( FOG_LINEAR, fadeColour, .001, 200, 1000);
	window->getViewport(0)->setBackgroundColour(fadeColour);

	std::string terrain_cfg = mResourcePath + "terrain.cfg";

	sceneMgr->setWorldGeometry( terrain_cfg );
	// Infinite far plane?
	if (root->getRenderSystem()->getCapabilities()->hasCapability(RSC_INFINITE_FAR_PLANE))
	    camera->setFarClipDistance(0);
	//camera->setFarClipDistance(1000);

	// Set a nice viewpoint
	camera->setPosition(750,100,750);
	camera->lookAt(750, 100, 750+1);
	//camera->setPolygonMode(PM_WIREFRAME);
	
	//---- PagedGeometry test ----
	
		// TREES
	
			//Initialize the PagedGeometry engine
			trees = new PagedGeometry();
			trees->setCamera(camera);
			trees->setPageSize(50);
			trees->setBounds(TBounds(0, 0, 1500, 1500));

			//Set up LODs
			//trees->addDetailLevel<EntityPage>(50);
			trees->addDetailLevel<BatchPage>(200, 50);
			trees->addDetailLevel<ImpostorPage>(700, 50);

			//Set up a TreeLoader for easy use
			TreeLoader2D *treeLoader = new TreeLoader2D(trees, TBounds(0, 0, 1500, 1500));
			trees->setPageLoader(treeLoader);
			treeLoader->setHeightFunction(&getTerrainHeight);

			//And add 20,000 trees to the scene through the "easy-to-use" TreeLoader class
			Entity *myTree = app->sceneMgr->createEntity("MyTree", "tree2.mesh");
			float x = 0, z = 0, yaw, scale;
			for (int i = 0; i < 10000; i++){
				yaw = Math::RangeRandom(0, 360);

				if (Math::RangeRandom(0, 1) <= 0.8f){
					x += Math::RangeRandom(-10.0f, 10.0f);
					z += Math::RangeRandom(-10.0f, 10.0f);
					if (x < 0) x = 0; else if (x > 1500) x = 1500;
					if (z < 0) z = 0; else if (z > 1500) z = 1500;
				} else {
					x = Math::RangeRandom(0, 1500);
					z = Math::RangeRandom(0, 1500);
				}

				scale = Math::RangeRandom(0.9f, 1.1f);

				treeLoader->addTree(myTree, Vector2(x, z), Degree(yaw), scale);
			}
	
		//Grass
		
			//Initialize the PagedGeometry engine
			grass = new PagedGeometry(camera, 30);
			//trees = new PagedGeometry(camera, 50, TBounds(0, 0, 1500, 1500));

			//Set up LODs
			grass->addDetailLevel<GrassPage>(80);

			//Set up a TreeLoader for easy use
			GrassLoader *grassLoader = new GrassLoader(grass);
			grass->setPageLoader(grassLoader);
			grassLoader->setHeightFunction(&getTerrainHeight);

			GrassLayer* grassLayer = grassLoader->addLayer("grass");    
			
			grassLayer->setAnimationEnabled(true);
			grassLayer->setSwaySpeed(0.5f);
			grassLayer->setSwayLength(0.05f);
			grassLayer->setSwayDistribution(10.0f);

			grassLayer->setDensity(0.6f);

			grassLayer->setMapBounds(TBounds(0, 0, 1500, 1500));

			grassLayer->setColorMap("terrain_texture.jpg");
			grassLayer->setColorMapFilter(MAPFILTER_BILINEAR);

			grassLayer->setDensityMap("densitymap.png");
			grassLayer->setDensityMapFilter(MAPFILTER_NONE);

			//grassLayer->setMinimumSize(0.5,0.5);
			//grassLayer->setMaximumSize(1.0, 1.0); 

			grassLayer->setMinimumSize(4.0,2.0);
			grassLayer->setMaximumSize(4.0,2.0);

			grassLayer->setFadeTechnique(FADETECH_GROW);
	

	//RaySceneQuery for terrain height check
	updateRay.setOrigin(camera->getPosition());
	updateRay.setDirection(Vector3::NEGATIVE_UNIT_Y);
	raySceneQuery = sceneMgr->createRayQuery(updateRay);
	raySceneQuery->setQueryTypeMask(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK );   
	raySceneQuery->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION); 
	raySceneQueryListener = new myRaySceneQueryListener;

	grassFrameListener = new GrassFrameListener(grassLoader);
	Root::getSingleton().addFrameListener(grassFrameListener);
}

//Unloads world
void Application::destroyScene()
{
	delete trees->getPageLoader();	//Delete TreeLoader object
	delete trees;					//Delete PagedGeoemtry object

	delete grass->getPageLoader();	//Delete GrassLoader object
	delete grass;					//Delete PagedGeoemtry object

	delete grassFrameListener;

	delete raySceneQuery;
	delete raySceneQueryListener;

}

//get resource path, depending on platform
std::string Application::getResourcePath()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	char path[1024];
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	assert( mainBundle );
	
	CFURLRef mainBundleURL = CFBundleCopyBundleURL( mainBundle);
	assert( mainBundleURL);
	
	CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
	assert( cfStringRef);
	
	CFStringGetCString( cfStringRef, path, 1024, kCFStringEncodingASCII);
	
	CFRelease( mainBundleURL);
	CFRelease( cfStringRef);
	
	return std::string(path)  + "/Contents/Resources/";
#else
	return "" ;
#endif
}


