/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/


#ifndef _WATER_MESH_H_
#define _WATER_MESH_H_

// need to remove these as defines and allow them to be set...
#define CIRCLE_SIZE 500.0
#define CIRCLE_TIME 0.5f
#define COMPLEXITY 64 		// watch out - number of polys is 2*ACCURACY*ACCURACY !
#define PLANE_SIZE 3000.0f
#define CIRCLES_MATERIAL "Examples/Water/Circles"

#include "OgrePlatform.h"
#include "Ogre.h"

using namespace Ogre ;
void prepareCircleMaterial( );

class WaterMesh
{
private:
	MeshPtr mesh ;
	SubMesh *subMesh ; 
	float *vertexBuffers[3] ; // we need 3 vertex buffers
	int currentBuffNumber ;
	int complexity ;
	String meshName ;
	int numFaces ;
	int numVertices ;
	Vector3* vNormals ;

	HardwareVertexBufferSharedPtr posVertexBuffer ;
	HardwareVertexBufferSharedPtr normVertexBuffer ;
	HardwareVertexBufferSharedPtr texcoordsVertexBuffer ;
	HardwareIndexBufferSharedPtr indexBuffer ;

	Real lastTimeStamp ;
	Real lastAnimationTimeStamp;
	Real lastFrameTime ;
	
	void calculateFakeNormals();
	void calculateNormals();
public:
	WaterMesh(const String& meshName, Real planeSize, int complexity) ;
    
    virtual ~WaterMesh ();


	/** "pushes" a mesh at position [x,y]. Note, that x,y are float, hence 
	*	4 vertices are actually pushed
	*	@note 
	*		This should be replaced by push with 'radius' parameter to simulate
	*  		big objects falling into water
	*/
	void push(Real x, Real y, Real depth, bool absolute=false) ;

	/** gets height at given x and y, takes average value of the closes nodes */
	Real getHeight(Real x, Real y);

	/** updates mesh */
	void updateMesh(Real timeSinceLastFrame) ;
	
	Real PARAM_C ; // ripple speed 
	Real PARAM_D ; // distance
	Real PARAM_U ; // viscosity
	Real PARAM_T ; // time
	bool useFakeNormals ;
} ;


/* =========================================================================*/
/*               WaterCircle class                                          */
/* =========================================================================*/

class WaterCircle
{
private:
	String name ;
	SceneNode *node ;
	MeshPtr mesh ;
	SubMesh *subMesh ;
	Entity *entity ;
	Real tm ;
	SceneManager *sceneMgr ;
	static bool first;
	// some buffers shared by all circles
	static HardwareVertexBufferSharedPtr posnormVertexBuffer;
	static HardwareIndexBufferSharedPtr indexBuffer; // indices for 2 faces
	static HardwareVertexBufferSharedPtr * texcoordsVertexBuffers ;
	float *texBufData;
	void _prepareMesh();
	
public:
	int lvl ;
	void setTextureLevel();
	WaterCircle(const String& name, Real x, Real y, SceneManager * sm);
	~WaterCircle();
	void animate(Real timeSinceLastFrame);
	
	static void clearStaticBuffers();
} ;

#endif
