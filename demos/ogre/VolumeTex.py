
# /**	Generate 3D julia sets and render them as volume texture
# 	This demonstrates
# 	   - User generated textures
# 	   - Procedural volume textures (Julia makes nice dust clouds)
# 	   - Custom renderables
# 	@author W.J. van der Laan
# */

TexturePtr ptex 
SimpleRenderable *vrend 
SimpleRenderable *trend 
Overlay* overlay 
float xtime = 0.0 
SceneNode *snode,*fnode 
AnimationState* mOgreAnimState = 0 

class VolumeTexFrameListener (sf.ExampleFrameListener)
	
    def __init__( self, win,  cam) 
        sf.ExampleFrameListener.__init__( win, cam )
		self.global_real = 0.4 
		self.global_imag = 0.6 
		self.global_theta = 0.0 
		self.generate() 
		
		self.updateInfoParamReal() 
		self.updateInfoParamImag() 
		self.updateInfoParamTheta() 

	
	def generate(self):

# 		/* Evaluate julia fractal for each point */
		Julia julia(self.global_real, self.global_imag, self.global_theta) 
		 float scale = 2.5 
		 float vcut = 29.0 
		 float vscale = 1.0/vcut 
		
		HardwarePixelBufferSharedPtr buffer = ptex->getBuffer(0, 0) 
		std.stringstream d 
		d << "HardwarePixelBuffer " << buffer->getWidth() << " " << buffer->getHeight() << " " << buffer->getDepth() 
		LogManager.getSingleton().logMessage(d.str()) 
		
		buffer->lock(HardwareBuffer.HBL_NORMAL) 
		 PixelBox &pb = buffer->getCurrentLock() 
		d.str("") 
		d << "PixelBox " << pb.getWidth() << " " << pb.getHeight() << " " << pb.getDepth() << " " << pb.rowPitch << " " << pb.slicePitch << " " << pb.data << " " << PixelUtil.getFormatName(pb.format) 
		LogManager.getSingleton().logMessage(d.str()) 
		
		uint32 *pbptr = static_cast<uint32*>(pb.data) 
		for(size_t z=pb.front  z<pb.back; z++) 

            for(size_t y=pb.top  y<pb.bottom; y++)

                for(size_t x=pb.left  x<pb.right; x++)

                    if(z==pb.front || z==(pb.back-1) || y==pb.top|| y==(pb.bottom-1) ||:
						x==pb.left || x==(pb.right-1))

						## On border, must be zero
						pbptr[x] = 0 

					else:

						float val = julia.eval(((float)x/pb.getWidth()-0.5) * scale, 
								((float)y/pb.getHeight()-0.5) * scale, 
								((float)z/pb.getDepth()-0.5) * scale) 
						if(val > vcut):
							val = vcut 
						
						PixelUtil.packColour((float)x/pb.getWidth(), (float)y/pb.getHeight(), (float)z/pb.getDepth(), (1.0-(val*vscale))*0.7f, PF_A8R8G8B8, &pbptr[x]) 
						


                pbptr += pb.rowPitch 

            pbptr += pb.getSliceSkip() 

		buffer->unlock() 

	void updateInfoParamReal()

		OverlayManager.getSingleton().getOverlayElement("Example/VolTex/Param_real") \
			->setCaption("[1/2]real: "+StringConverter.toString(self.global_real)) 

	void updateInfoParamImag()

		OverlayManager.getSingleton().getOverlayElement("Example/VolTex/Param_imag") \
			->setCaption("[3/4]imag: "+StringConverter.toString(self.global_imag)) 

	void updateInfoParamTheta()

		OverlayManager.getSingleton().getOverlayElement("Example/VolTex/Param_theta") \
			->setCaption("[5/6]theta: "+StringConverter.toString(self.global_theta)) 


    bool frameStarted(  FrameEvent& evt )

	using namespace OIS 
	static float mTimeUntilNextToggle = 0.0 
        if( ExampleFrameListener.frameStarted( evt ) == False ):
		return False 
		
		mTimeUntilNextToggle -= evt.timeSinceLastFrame 
		
        if( (mKeyboard->isKeyDown( KC_1 ) || mKeyboard->isKeyDown( KC_2 )) :
				&& mTimeUntilNextToggle <= 0) {
        	self.global_real += mKeyboard->isKeyDown( KC_1 )? -0.1 : 0.1f 
			generate() 
			mTimeUntilNextToggle = 0.5 
			updateInfoParamReal() 

		 if( (mKeyboard->isKeyDown( KC_3 ) || mKeyboard->isKeyDown( KC_4 )) :
				&& mTimeUntilNextToggle <= 0) {
        	self.global_imag += mKeyboard->isKeyDown( KC_3 )? -0.1 : 0.1f 
			generate() 
			mTimeUntilNextToggle = 0.5 
			updateInfoParamImag() 

		if( (mKeyboard->isKeyDown( KC_5 ) || mKeyboard->isKeyDown( KC_6 )) :
				&& mTimeUntilNextToggle <= 0) {
        	self.global_theta += mKeyboard->isKeyDown( KC_5 )? -0.1 : 0.1f 
			generate() 
			mTimeUntilNextToggle = 0.5 
			updateInfoParamTheta() 

		
		xtime += evt.timeSinceLastFrame 
		xtime = fmod(xtime, 10.0) 
		##snode->roll(Degree(evt.timeSinceLastFrame * 20.0)) 
		##fnode->roll(Degree(evt.timeSinceLastFrame * 20.0)) 
		static_cast<ThingRenderable*>(trend)->addTime(evt.timeSinceLastFrame * 0.05) 
		mOgreAnimState->addTime(evt.timeSinceLastFrame) 
        return True 

	~VolumeTexFrameListener()

		delete vrend 
		delete trend 





class VolumeTexApplication : public ExampleApplication

public:
    VolumeTexApplication() {}


protected:
	
    virtual void createFrameListener(void)

        mFrameListener= new VolumeTexFrameListener(mWindow, mCamera) 
        mFrameListener->showDebugOverlay(True) 
        mRoot->addFrameListener(mFrameListener) 



	virtual void createViewports(void)

		## Create one viewport, entire window
        Viewport* vp = mWindow->addViewport(mCamera) 
        vp->setBackgroundColour(ColourValue(0,0,0)) 

        ## Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio( 
            Real(vp->getActualWidth()) / Real(vp->getActualHeight())) 

	void createCamera(void)

		## Create the camera
        mCamera = mSceneMgr->createCamera("PlayerCam") 
        mCamera->setPosition(Vector3(220,-2,176)) 
        mCamera->lookAt(Vector3(0,0,0)) 
        mCamera->setNearClipDistance(5) 
		mCamera->setFixedYawAxis(False) 

    ## Just override the mandatory create scene method
    void createScene(void)

		## Check capabilities
        /*
		 RenderSystemCapabilities* caps = Root.getSingleton().getRenderSystem()->getCapabilities() 
        if (!caps->hasCapability(RSC_TEXTURE_3D)):

            OGRE_EXCEPT(Exception.ERR_NOT_IMPLEMENTED, "Your card does not support 3D textures (or you selected D3D7), so cannot "
                "run self demo. Sorry!", 
                "VolTex.createScene") 

        */
		
        ## Create dynamic texture
		ptex = TextureManager.getSingleton().createManual(
			"DynaTex","General", TEX_TYPE_3D, 64, 64, 64, 0, PF_A8R8G8B8) 
		


		## Set ambient light
        mSceneMgr->setAmbientLight(ColourValue(0.6, 0.6, 0.6)) 
		mSceneMgr->setSkyBox(True, "Examples/MorningSkyBox", 50 ) 

        ##mRoot->getRenderSystem()->clearFrameBuffer(FBT_COLOUR, ColourValue(255,255,255,0)) 

        ## Create a light
        Light* l = mSceneMgr->createLight("MainLight") 
        l->setDiffuseColour(0.75, 0.75, 0.80) 
		l->setSpecularColour(0.9, 0.9, 1) 
        l->setPosition(-100,80,50) 
		mSceneMgr->getRootSceneNode()->attachObject(l) 

		## Create manual material
		
		
		## Create volume renderable
		snode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,0,0))       
		
        vrend = new VolumeRenderable(32, 750.0, "DynaTex") 
        snode->attachObject( vrend ) 
		
		trend = new ThingRenderable(90.0, 32, 7.5f) 
		trend->setMaterial("Examples/VTDarkStuff") 
		snode->attachObject(trend) 
		
		## Ogre head node
		fnode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,0,0)) 
		## Load ogre head
		Entity* head = mSceneMgr->createEntity("head", "ogrehead.mesh") 
		fnode->attachObject(head) 
		
		## Animation for ogre head
		## Create a track for the light
        Animation* anim = mSceneMgr->createAnimation("OgreTrack", 10) 
        ## Spline it for nice curves
        anim->setInterpolationMode(Animation.IM_SPLINE) 
        ## Create a track to animate the camera's node
        NodeAnimationTrack* track = anim->createNodeTrack(0, fnode) 
        ## Setup keyframes
        TransformKeyFrame* key = track->createNodeKeyFrame(0)  ## A startposition
        key->setTranslate(Vector3(0.0, -15.0f, 0.0f)) 
        key = track->createNodeKeyFrame(5) ##B
        key->setTranslate(Vector3(0.0, 15.0f, 0.0f)) 
        key = track->createNodeKeyFrame(10) ##C
        key->setTranslate(Vector3(0.0, -15.0f, 0.0f)) 
        ## Create a new animation state to track self
        mOgreAnimState = mSceneMgr->createAnimationState("OgreTrack") 
        mOgreAnimState->setEnabled(True) 
     
        ##mFountainNode->attachObject(pSys2) 
		## show GUI
		overlay = OverlayManager.getSingleton().getByName("Example/VolTexOverlay")     
		overlay->show() 


	void destroyScene()

		ptex.setNull() 




#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32:
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus:
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32:
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else:
int main(int argc, char *argv[])
#endif

    ## Create application object
    VolumeTexApplication app 

    try {
        app.go() 
    } catch( Ogre.Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32:
        MessageBox( None, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL) 
#else:
        std.cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std.endl 
#endif


    return 0 


#ifdef __cplusplus:

#endif
