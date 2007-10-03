/*-----------------------------------------------------------------------------
WARNING!!! main.cpp, Appliation.h, and Appliation.cpp are TEMPORARY source files
that demonstrate PagedGeometry. They are extremely messy, unstructured, and they
should NOT be taken as an example. This is the beta version of PagedGeometry 1.0
and does not include the examples and tutorials that the final version will have.
-----------------------------------------------------------------------------*/

//Application.h
//-------------------------------------------------------------------------------------
#include <string>

#include "PagedGeometry.h"
#include "GrassLoader.h"

#include "OgreRoot.h"
#include "OgreFrameListener.h"
#include "OgreCamera.h"
#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"
#include "OgreSceneQuery.h"
#include "OgreRay.h"
using namespace Ogre;


class myRaySceneQueryListener;

class GrassFrameListener :public FrameListener
{
public:
	GrassFrameListener(GrassLoader *Gloader){grassloader = Gloader;}

	bool frameStarted(const FrameEvent& evt);

private:
	GrassLoader *grassloader;
};

class Application
{
public:
	Application();
	~Application();

	void go();				//Starts application. Called automatically from main()
	
	Root *root;
    Camera *camera;
    SceneManager *sceneMgr;
	FrameListener *frameListener;
    RenderWindow *window;

	PagedGeometry *trees;
	PagedGeometry *grass;

	RaySceneQuery* raySceneQuery;
	Ray updateRay;
	myRaySceneQueryListener *raySceneQueryListener;

	GrassFrameListener *grassFrameListener;

protected:
	bool setup();			//Initializes OGRE scene manager, cameras, lights, etc.
	void loadMedia();		//Called by setup() to load media

	void createScene();		//Loads world
	void destroyScene();	//Unloads world
	std::string getResourcePath(); //get resource path, depending on platform
};

//Global pointer to Application object
extern Application *app;


class myRaySceneQueryListener : public RaySceneQueryListener
{
public:
	inline bool queryResult(SceneQuery::WorldFragment *fragment, Real  	distance)
	{
		resultDistance = distance;
		return false;
	}
	inline bool RaySceneQueryListener::queryResult(MovableObject *  obj, Real  	distance)
	{
		resultDistance = distance;
		return false;
	}

	float resultDistance;
};

inline float getTerrainHeight(const float x, const float z){
	if (x < 0 || z < 0 || x > 1500 || z > 1500) return 0;

	app->updateRay.setOrigin(Vector3(x, 0.0f, z));
	app->updateRay.setDirection(Vector3::UNIT_Y);
	app->raySceneQuery->setRay(app->updateRay);
	app->raySceneQuery->execute(app->raySceneQueryListener);
	
	return app->raySceneQueryListener->resultDistance;
}


