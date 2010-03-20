import math as Math

PAGING = False

TERRAIN_PAGE_MIN_X= 0
TERRAIN_PAGE_MIN_Y= 0
TERRAIN_PAGE_MAX_X= 0
TERRAIN_PAGE_MAX_Y= 0

TERRAIN_FILE_PREFIX ="testTerrain"
TERRAIN_FILE_SUFFIX ="dat"
TERRAIN_WORLD_SIZE =12000
TERRAIN_SIZE=513

MODE_NORMAL = 0
MODE_EDIT_HEIGHT = 1
MODE_EDIT_BLEND = 2
MODE_COUNT = 3
SHADOWS_NONE = 0
SHADOWS_COLOUR = 1
SHADOWS_DEPTH = 2
SHADOWS_COUNT = 3


class sample (sf.sample):
    def __init__ ( self ):
        sf.sample.__init__(self)
        
		self.self.mTerrainGroup= 0
		self.mTerrainPaging=0
		self.mPageManager= 0
		self.mFly=False
		self.mFallVelocity=0
		self.mMode= MODE_NORMAL
		self.self.mLayerEdit=1
		self.mBrushSizeTerrainSpace=0.02
		self.self.mHeightUpdateCountDown=0
		self.mTerrainPos = ogre.Vector3(1000,0,5000)
		self.mTerrainsImported= False

		self.mInfo["Title"] = "Terrain"
		self.mInfo["Description"] = "Demonstrates use of the terrain rendering plugin.";
		self.mInfo["Thumbnail"] = "thumb_terrain.png";
		self.mInfo["Category"] = "Environment";
		self.mInfo["Help"] = "Left click and drag anywhere in the scene to look around. Let go again to show \
			cursor and access widgets. Use WASD keys to move. Use +/- keys when in edit mode to change content.";

		# Update terrain at max 20fps
		self.self.mHeightUpdateRate = 1.0 / 20.0


    def testCapabilities(self, caps):
        if not caps.hasCapability(RSC_VERTEX_PROGRAM) or not caps.hasCapability(RSC_FRAGMENT_PROGRAM):
			ogre.OGRE_EXCEPT(ogre.Exception.ERR_NOT_IMPLEMENTED, "Your graphics card does not support vertex or fragment shaders, "
                        "so you cannot run this sample. Sorry!", "Sample_Terrain.testCapabilities");

	def getRequiredPlugins(self):
		return ogre.StringVector()

	def doTerrainModify(self,  terrain, centrepos, timeElapsed):
		tsPos = ogre.Vector3()
		terrain.getTerrainPosition(centrepos, tsPos)  ## AJM TO CHECK !!
		if self.mKeyboard.isKeyDown(OIS.KC_EQUALS) or self.mKeyboard.isKeyDown(OIS.KC_MINUS):
			if self.mMode == MODE_EDIT_HEIGHT:
				# we need point coords
				terrainSize = (terrain.getSize() - 1);
				startx = (tsPos.x - self.mBrushSizeTerrainSpace) * terrainSize;
				starty = (tsPos.y - self.mBrushSizeTerrainSpace) * terrainSize;
				endx = (tsPos.x + self.mBrushSizeTerrainSpace) * terrainSize;
				endy= (tsPos.y + self.mBrushSizeTerrainSpace) * terrainSize;
				startx = std.max(startx, 0L);
				starty = std.max(starty, 0L);
				endx = std.min(endx, (long)terrainSize);
				endy = std.min(endy, (long)terrainSize);
				for y in range (starty, endy+1):
					for   x in range (startx, endx + 1):
						tsXdist = (x / terrainSize) - tsPos.x;
						tsYdist = (y / terrainSize)  - tsPos.y;

						weight = std.min((Real)1.0,
							Math.Sqrt(tsYdist * tsYdist + tsXdist * tsXdist) / (0.5 * self.mBrushSizeTerrainSpace));
						weight = 1.0 - (weight * weight);

						addedHeight = weight * 250.0 * timeElapsed;
						newheight;
						if self.mKeyboard.isKeyDown(OIS.KC_EQUALS):
							newheight = terrain.getHeightAtPoint(x, y) + addedHeight;
						else :
							newheight = terrain.getHeightAtPoint(x, y) - addedHeight;
						terrain.setHeightAtPoint(x, y, newheight);
				if self.mHeightUpdateCountDown == 0:
					self.mHeightUpdateCountDown = self.mHeightUpdateRate;
			elif self.mMode == MODE_EDIT_HEIGHT:
					layer = terrain.getLayerBlendMap(self.mLayerEdit);
					# we need image coords
					imgSize = terrain.getLayerBlendMapSize();
					startx = (tsPos.x - self.mBrushSizeTerrainSpace) * imgSize;
					starty = (tsPos.y - self.mBrushSizeTerrainSpace) * imgSize;
					endx = (tsPos.x + self.mBrushSizeTerrainSpace) * imgSize;
					endy= (tsPos.y + self.mBrushSizeTerrainSpace) * imgSize;
					startx = max(startx, 0L);
					starty = max(starty, 0L);
					endx = min(endx, (long)imgSize);
					endy = min(endy, (long)imgSize);
					for y in range(starty,endy):
						for x in range(startx,endx) :
							tsXdist = (x / imgSize) - tsPos.x;
							tsYdist = (y / imgSize)  - tsPos.y;

							weight = min(1.0,
								Math.Sqrt(tsYdist * tsYdist + tsXdist * tsXdist) / (0.5 * self.mBrushSizeTerrainSpace));
							weight = 1.0 - (weight * weight);

							paint = weight * timeElapsed;
							imgY = imgSize - y;
							if self.mKeyboard.isKeyDown(OIS.KC_EQUALS):
								val = layer.getBlendValue(x, imgY) + paint;
							else:
								val = layer.getBlendValue(x, imgY) - paint;
							val = Math.Clamp(val, 0.0f, 1.0f);
							layer.setBlendValue(x, imgY, val);
					layer.update();

    def frameRenderingQueued(self,  evt)
		if self.mMode != MODE_NORMAL:
			# fire ray
			ray = mTrayMgr.getCursorRay(self.mCamera);

			rayResult = self.self.mTerrainGroup.rayIntersects(ray);
			if rayResult.hit:
				self.mEditMarker.setVisible(True);
				self.mEditNode.setPosition(rayResult.position);

				# figure out which terrains this affects
				terrainList =TerrainGroup.TerrainList()
				brushSizeWorldSpace = TERRAIN_WORLD_SIZE * self.mBrushSizeTerrainSpace;
				sphere = Sphere(rayResult.position, brushSizeWorldSpace);
				self.mTerrainGroup.sphereIntersects(sphere, terrainList);

				for (TerrainGroup.TerrainList.iterator ti = terrainList.begin();
					ti != terrainList.end(); ++ti)
					doTerrainModify(*ti, rayResult.position, evt.timeSinceLastFrame);
			}
			else
			{
				self.mEditMarker.setVisible(False);
			}
		}

		if (!mFly)
		{
			# clamp to terrain
			Vector3 camPos = self.mCamera.getPosition();
			Ray ray;
			ray.setOrigin(Vector3(camPos.x, self.mTerrainPos.y + 10000, camPos.z));
			ray.setDirection(Vector3.NEGATIVE_UNIT_Y);

			TerrainGroup.RayResult rayResult = self.self.mTerrainGroup.rayIntersects(ray);
			Real distanceAboveTerrain = 50;
			Real fallSpeed = 300;
			Real newy = camPos.y;
			if (rayResult.hit)
			{
				if (camPos.y > rayResult.position.y + distanceAboveTerrain)
				{
					self.mFallVelocity += evt.timeSinceLastFrame * 20;
					self.mFallVelocity = std.min(self.mFallVelocity, fallSpeed);
					newy = camPos.y - self.mFallVelocity * evt.timeSinceLastFrame;

				}
				newy = std.max(rayResult.position.y + distanceAboveTerrain, newy);
				self.mCamera.setPosition(camPos.x, newy, camPos.z);
				
			}

		}

		if (self.mHeightUpdateCountDown > 0)
		{
			self.mHeightUpdateCountDown -= evt.timeSinceLastFrame;
			if (self.mHeightUpdateCountDown <= 0)
			{
				self.mTerrainGroup.update();
				self.mHeightUpdateCountDown = 0;

			}

		}

		if (self.mTerrainGroup.isDerivedDataUpdateInProgress())
		{
			mTrayMgr.moveWidgetToTray(mInfoLabel, TL_TOP, 0);
			mInfoLabel.show();
			if (mTerrainsImported)
			{
				mInfoLabel.setCaption("Building terrain, please wait...");
			}
			else
			{
				mInfoLabel.setCaption("Updating textures, patience...");
			}

		}
		else
		{
			mTrayMgr.removeWidgetFromTray(mInfoLabel);
			mInfoLabel.hide();
			if (mTerrainsImported)
			{
				saveTerrains(True);
				mTerrainsImported = False;
			}
		}

		return SdkSample.frameRenderingQueued(evt);  # don't forget the parent updates!
    }

	void saveTerrains(bool onlyIfModified)
	{
		self.mTerrainGroup.saveAllTerrains(onlyIfModified);
	}

	bool keyPressed (const OIS.KeyEvent &e)
	{
#if OGRE_PLATFORM != OGRE_PLATFORM_IPHONE
		switch (e.key)
		{
		case OIS.KC_S:
			# CTRL-S to save
			if (self.mKeyboard.isKeyDown(OIS.KC_LCONTROL) || self.mKeyboard.isKeyDown(OIS.KC_RCONTROL))
			{
				saveTerrains(True);
			}
			else
				return SdkSample.keyPressed(e);
			break;
		case OIS.KC_F10:
			# dump
			{
				TerrainGroup.TerrainIterator ti = self.self.mTerrainGroup.getTerrainIterator();
				while (ti.hasMoreElements())
				{
					Ogre.uint32 tkey = ti.peekNextKey();
					TerrainGroup.TerrainSlot* ts = ti.getNext();
					if (ts.instance && ts.instance.isLoaded())
					{
						ts.instance._dumpTextures("terrain_" + StringConverter.toString(tkey), ".png");
					}
				}
			}
			break;
		default:
			return SdkSample.keyPressed(e);
		}
#endif

		return True;
	}

	void itemSelected(SelectMenu* menu)
	{
		if (menu == mEditMenu)
		{
			self.mMode = (Mode)mEditMenu.getSelectionIndex();
		}
		else if (menu == mShadowsMenu)
		{
			mShadowMode = (ShadowMode)mShadowsMenu.getSelectionIndex();
			changeShadows();
		}
	}

	void checkBoxToggled(CheckBox* box)
	{
		if (box == self.mFlyBox)
		{
			mFly = self.mFlyBox.isChecked();
		}
	}

protected:

	TerrainGlobalOptions* self.mTerrainGlobals;
	TerrainGroup* self.self.mTerrainGroup;
	bool mPaging;
	TerrainPaging* self.mTerrainPaging;
	PageManager* self.mPageManager;
#ifdef PAGING
	#/ This class just pretends to provide prcedural page content to avoid page loading
	class DummyPageProvider : public PageProvider
	{
	public:
		bool prepareProceduralPage(Page* page, PagedWorldSection* section) { return True; }
		bool loadProceduralPage(Page* page, PagedWorldSection* section) { return True; }
		bool unloadProceduralPage(Page* page, PagedWorldSection* section) { return True; }
		bool unprepareProceduralPage(Page* page, PagedWorldSection* section) { return True; }
	};
	DummyPageProvider mDummyPageProvider;
# endif
# 	bool self.mFly;
# 	Real self.mFallVelocity;
# 	enum Mode
# 	{
# 	};
# 	Mode self.mMode;
# 	ShadowMode mShadowMode;
# 	Ogre.uint8 self.mLayerEdit;
# 	Real self.mBrushSizeTerrainSpace;
# 	SceneNode* self.mEditNode;
# 	Entity* self.mEditMarker;
# 	Real self.mHeightUpdateCountDown;
# 	Real self.mHeightUpdateRate;
# 	Vector3 self.mTerrainPos;
# 	SelectMenu* mEditMenu;
# 	SelectMenu* mShadowsMenu;
# 	CheckBox* self.mFlyBox;
# 	OgreBites.Label* mInfoLabel;
# 	bool self.mTerrainsImported;
# 	ShadowCameraSetupPtr mPSSMSetup;
#
# 	typedef std.list<Entity*> EntityList;
# 	EntityList mHouseList;
#


	void defineTerrain(long x, long y, bool flat = False)
	{
		# if a file is available, use it
		# if not, generate file from import

		# Usually in a real project you'll know whether the compact terrain data is
		# available or not; I'm doing it this way to save distribution size

		if (flat)
		{
			self.mTerrainGroup.defineTerrain(x, y, 0.0f);
		}
		else
		{
			String filename = self.self.mTerrainGroup.generateFilename(x, y);
			if (ResourceGroupManager.getSingleton().resourceExists(self.mTerrainGroup.getResourceGroup(), filename))
			{
				self.mTerrainGroup.defineTerrain(x, y);
			}
			else
			{
				Image img;
				getTerrainImage(x % 2 != 0, y % 2 != 0, img);
				self.mTerrainGroup.defineTerrain(x, y, &img);
				mTerrainsImported = True;
			}

		}
	}

	void getTerrainImage(bool flipX, bool flipY, Image& img)
	{
		img.load("terrain.png", ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME);
		if (flipX)
			img.flipAroundY();
		if (flipY)
			img.flipAroundX();

	}

	void initBlendMaps(Terrain* terrain)
	{
		TerrainLayerBlendMap* blendMap0 = terrain.getLayerBlendMap(1);
		TerrainLayerBlendMap* blendMap1 = terrain.getLayerBlendMap(2);
		Real minHeight0 = 70;
		Real fadeDist0 = 40;
		Real minHeight1 = 70;
		Real fadeDist1 = 15;
		float* pBlend1 = blendMap1.getBlendPointer();
		for (Ogre.uint16 y = 0; y < terrain.getLayerBlendMapSize(); ++y)
		{
			for (Ogre.uint16 x = 0; x < terrain.getLayerBlendMapSize(); ++x)
			{
				Real tx, ty;

				blendMap0.convertImageToTerrainSpace(x, y, &tx, &ty);
				Real height = terrain.getHeightAtTerrainPosition(tx, ty);
				Real val = (height - minHeight0) / fadeDist0;
				val = Math.Clamp(val, (Real)0, (Real)1);
				#*pBlend0++ = val;

				val = (height - minHeight1) / fadeDist1;
				val = Math.Clamp(val, (Real)0, (Real)1);
				*pBlend1++ = val;


			}
		}
		blendMap0.dirty();
		blendMap1.dirty();
		#blendMap0.loadImage("blendmap1.png", ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME);
		blendMap0.update();
		blendMap1.update();

		# set up a colour map
		/*
		if (!terrain.getGlobalColourMapEnabled())
		{
			terrain.setGlobalColourMapEnabled(True);
			Image colourMap;
			colourMap.load("testcolourmap.jpg", ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME);
			terrain.getGlobalColourMap().loadImage(colourMap);
		}
		*/

	}

	void configureTerrainDefaults(Light* l)
	{
		# Configure global
		mTerrainGlobals.setMaxPixelError(8);
		# testing composite map
		mTerrainGlobals.setCompositeMapDistance(3000);
		#mTerrainGlobals.setUseRayBoxDistanceCalculation(True);
		#mTerrainGlobals.getDefaultMaterialGenerator().setDebugLevel(1);
		#mTerrainGlobals.setLightMapSize(256);

		#matProfile.setLightmapEnabled(False);
		# Important to set these so that the terrain knows what to use for derived (non-realtime) data
		mTerrainGlobals.setLightMapDirection(l.getDerivedDirection());
		mTerrainGlobals.setCompositeMapAmbient(mSceneMgr.getAmbientLight());
		#mTerrainGlobals.setCompositeMapAmbient(ColourValue.Red);
		mTerrainGlobals.setCompositeMapDiffuse(l.getDiffuseColour());

		# Configure default import settings for if we use imported image
		Terrain.ImportData& defaultimp = self.self.mTerrainGroup.getDefaultImportSettings();
		defaultimp.terrainSize = TERRAIN_SIZE;
		defaultimp.worldSize = TERRAIN_WORLD_SIZE;
		defaultimp.inputScale = 600;
		defaultimp.minBatchSize = 33;
		defaultimp.maxBatchSize = 65;
		# textures
		defaultimp.layerList.resize(3);
		defaultimp.layerList[0].worldSize = 100;
		defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
		defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
		defaultimp.layerList[1].worldSize = 30;
		defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
		defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
		defaultimp.layerList[2].worldSize = 200;
		defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
		defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");


	}

	void addTextureDebugOverlay(TrayLocation loc, TexturePtr tex, size_t i)
	{
		addTextureDebugOverlay(loc, tex.getName(), i);
	}
	void addTextureDebugOverlay(TrayLocation loc, const String& texname, size_t i)
	{
		# Create material
		String matName = "Ogre/DebugTexture" + StringConverter.toString(i);
		MaterialPtr debugMat = MaterialManager.getSingleton().getByName(matName);
		if (debugMat.isNull())
		{
			debugMat = MaterialManager.getSingleton().create(matName,
				ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME);
		}
		Pass* p = debugMat.getTechnique(0).getPass(0);
		p.removeAllTextureUnitStates();
		p.setLightingEnabled(False);
		TextureUnitState *t = p.createTextureUnitState(texname);
		t.setTextureAddressingMode(TextureUnitState.TAM_CLAMP);

		# create template
		if (!OverlayManager.getSingleton().hasOverlayElement("Ogre/DebugTexOverlay", True))
		{
			OverlayElement* e = OverlayManager.getSingleton().createOverlayElement("Panel", "Ogre/DebugTexOverlay", True);
			e.setMetricsMode(GMM_PIXELS);
			e.setWidth(128);
			e.setHeight(128);
		}

		# add widget
		String widgetName = "DebugTex"+ StringConverter.toString(i);
		Widget* w = mTrayMgr.getWidget(widgetName);
		if (!w)
		{
			w = mTrayMgr.createDecorWidget(loc, widgetName, "Ogre/DebugTexOverlay");
		}
		w.getOverlayElement().setMaterialName(matName);

	}

	void addTextureShadowDebugOverlay(TrayLocation loc, size_t num)
	{
		for (size_t i = 0; i < num; ++i)
		{
			TexturePtr shadowTex = mSceneMgr.getShadowTexture(i);
			addTextureDebugOverlay(loc, shadowTex, i);

		}

	}
		
	MaterialPtr buildDepthShadowMaterial(const String& textureName)
	{
		String matName = "DepthShadows/" + textureName;

		MaterialPtr ret = MaterialManager.getSingleton().getByName(matName);
		if (ret.isNull())
		{
			MaterialPtr baseMat = MaterialManager.getSingleton().getByName("Ogre/shadow/depth/integrated/pssm");
			ret = baseMat.clone(matName);
			Pass* p = ret.getTechnique(0).getPass(0);
			p.getTextureUnitState("diffuse").setTextureName(textureName);

			Vector4 splitPoints;
			const PSSMShadowCameraSetup.SplitPointList& splitPointList = 
				static_cast<PSSMShadowCameraSetup*>(mPSSMSetup.get()).getSplitPoints();
			for (int i = 0; i < 3; ++i)
			{
				splitPoints[i] = splitPointList[i];
			}
			p.getFragmentProgramParameters().setNamedConstant("pssmSplitPoints", splitPoints);


		}

		return ret;
	}

	void changeShadows()
	{
		configureShadows(mShadowMode != SHADOWS_NONE, mShadowMode == SHADOWS_DEPTH);
	}

	void configureShadows(bool enabled, bool depthShadows)
	{
		TerrainMaterialGeneratorA.SM2Profile* matProfile = 
			static_cast<TerrainMaterialGeneratorA.SM2Profile*>(mTerrainGlobals.getDefaultMaterialGenerator().getActiveProfile());
		matProfile.setReceiveDynamicShadowsEnabled(enabled);
#ifdef SHADOWS_IN_LOW_LOD_MATERIAL
		matProfile.setReceiveDynamicShadowsLowLod(True);
#else
		matProfile.setReceiveDynamicShadowsLowLod(False);
#endif

		# Default materials
		for (EntityList.iterator i = mHouseList.begin(); i != mHouseList.end(); ++i)
		{
			(*i).setMaterialName("Examples/TudorHouse");
		}

		if (enabled)
		{
			# General scene setup
			mSceneMgr.setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
			mSceneMgr.setShadowFarDistance(3000);

			# 3 textures per directional light (PSSM)
			mSceneMgr.setShadowTextureCountPerLightType(Ogre.Light.LT_DIRECTIONAL, 3);

			if (mPSSMSetup.isNull())
			{
				# shadow camera setup
				PSSMShadowCameraSetup* pssmSetup = new PSSMShadowCameraSetup();
				pssmSetup.setSplitPadding(self.mCamera.getNearClipDistance());
				pssmSetup.calculateSplitPoints(3, self.mCamera.getNearClipDistance(), mSceneMgr.getShadowFarDistance());
				pssmSetup.setOptimalAdjustFactor(0, 2);
				pssmSetup.setOptimalAdjustFactor(1, 1);
				pssmSetup.setOptimalAdjustFactor(2, 0.5);

				mPSSMSetup.bind(pssmSetup);

			}
			mSceneMgr.setShadowCameraSetup(mPSSMSetup);

			if (depthShadows)
			{
				mSceneMgr.setShadowTextureCount(3);
				mSceneMgr.setShadowTextureConfig(0, 2048, 2048, PF_FLOAT32_R);
				mSceneMgr.setShadowTextureConfig(1, 1024, 1024, PF_FLOAT32_R);
				mSceneMgr.setShadowTextureConfig(2, 1024, 1024, PF_FLOAT32_R);
				mSceneMgr.setShadowTextureSelfShadow(True);
				mSceneMgr.setShadowCasterRenderBackFaces(True);
				mSceneMgr.setShadowTextureCasterMaterial("PSSM/shadow_caster");

				MaterialPtr houseMat = buildDepthShadowMaterial("fw12b.jpg");
				for (EntityList.iterator i = mHouseList.begin(); i != mHouseList.end(); ++i)
				{
					(*i).setMaterial(houseMat);
				}

			}
			else
			{
				mSceneMgr.setShadowTextureCount(3);
				mSceneMgr.setShadowTextureConfig(0, 2048, 2048, PF_X8B8G8R8);
				mSceneMgr.setShadowTextureConfig(1, 1024, 1024, PF_X8B8G8R8);
				mSceneMgr.setShadowTextureConfig(2, 1024, 1024, PF_X8B8G8R8);
				mSceneMgr.setShadowTextureSelfShadow(False);
				mSceneMgr.setShadowCasterRenderBackFaces(False);
				mSceneMgr.setShadowTextureCasterMaterial(StringUtil.BLANK);
			}

			matProfile.setReceiveDynamicShadowsDepth(depthShadows);
			matProfile.setReceiveDynamicShadowsPSSM(static_cast<PSSMShadowCameraSetup*>(mPSSMSetup.get()));

			#addTextureShadowDebugOverlay(TL_RIGHT, 3);


		}
		else
		{
			mSceneMgr.setShadowTechnique(SHADOWTYPE_NONE);
		}


	}

	/*-----------------------------------------------------------------------------
	| Extends setupView to change some initial camera settings for this sample.
	-----------------------------------------------------------------------------*/
	void setupView()
	{
		SdkSample.setupView();

		self.mCamera.setPosition(mTerrainPos + Vector3(1683, 50, 2116));
		self.mCamera.lookAt(Vector3(1963, 50, 1660));
		self.mCamera.setNearClipDistance(0.1);
		self.mCamera.setFarClipDistance(50000);

		if (mRoot.getRenderSystem().getCapabilities().hasCapability(RSC_INFINITE_FAR_PLANE))
        {
            self.mCamera.setFarClipDistance(0);   # enable infinite far clip distance if we can
        }
	}

	void setupControls()
	{
		mTrayMgr.showCursor();

		# make room for the controls
		mTrayMgr.showLogo(TL_TOPRIGHT);
		mTrayMgr.showFrameStats(TL_TOPRIGHT);
		mTrayMgr.toggleAdvancedFrameStats();

		mInfoLabel = mTrayMgr.createLabel(TL_TOP, "TInfo", "", 350);

		mEditMenu = mTrayMgr.createLongSelectMenu(TL_BOTTOM, "EditMode", "Edit Mode", 370, 250, 3);
		mEditMenu.addItem("None");
		mEditMenu.addItem("Elevation");
		mEditMenu.addItem("Blend");
		mEditMenu.selectItem(0);  # no edit mode

		mFlyBox = mTrayMgr.createCheckBox(TL_BOTTOM, "Fly", "Fly");
		mFlyBox.setChecked(False, False);

		mShadowsMenu = mTrayMgr.createLongSelectMenu(TL_BOTTOM, "Shadows", "Shadows", 370, 250, 3);
		mShadowsMenu.addItem("None");
		mShadowsMenu.addItem("Colour Shadows");
		mShadowsMenu.addItem("Depth Shadows");
		mShadowsMenu.selectItem(0);  # no edit mode

		# a friendly reminder
		StringVector names;
		names.push_back("Help");
		mTrayMgr.createParamsPanel(TL_TOPLEFT, "Help", 100, names).setParamValue(0, "H/F1");
	}

	void setupContent()
	{
		bool blankTerrain = False;
		#blankTerrain = True;

		mTerrainGlobals = OGRE_NEW TerrainGlobalOptions();

		self.mEditMarker = mSceneMgr.createEntity("editMarker", "sphere.mesh");
		self.mEditNode = mSceneMgr.getRootSceneNode().createChildSceneNode();
		self.mEditNode.attachObject(self.mEditMarker);
		self.mEditNode.setScale(0.05, 0.05, 0.05);

		setupControls();

		self.mCameraMan.setTopSpeed(50);

		setDragLook(True);

		MaterialManager.getSingleton().setDefaultTextureFiltering(TFO_ANISOTROPIC);
		MaterialManager.getSingleton().setDefaultAnisotropy(7);

		mSceneMgr.setFog(FOG_LINEAR, ColourValue(0.7, 0.7, 0.8), 0, 10000, 25000);

		LogManager.getSingleton().setLogDetail(LL_BOREME);

		Vector3 lightdir(0.55, -0.3, 0.75);
		lightdir.normalise();


		Light* l = mSceneMgr.createLight("tstLight");
		l.setType(Light.LT_DIRECTIONAL);
		l.setDirection(lightdir);
		l.setDiffuseColour(ColourValue.White);
		l.setSpecularColour(ColourValue(0.4, 0.4, 0.4));

		mSceneMgr.setAmbientLight(ColourValue(0.2, 0.2, 0.2));


		self.mTerrainGroup = OGRE_NEW TerrainGroup(mSceneMgr, Terrain.ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
		self.mTerrainGroup.setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX);
		self.mTerrainGroup.setOrigin(mTerrainPos);

		configureTerrainDefaults(l);
#ifdef PAGING
		# Paging setup
		mPageManager = OGRE_NEW PageManager();
		# Since we're not loading any pages from .page files, we need a way just 
		# to say we've loaded them without them actually being loaded
		mPageManager.setPageProvider(&mDummyPageProvider);
		mPageManager.addCamera(self.mCamera);
		mTerrainPaging = OGRE_NEW TerrainPaging(mPageManager);
		PagedWorld* world = self.mPageManager.createWorld();
		mTerrainPaging.createWorldSection(world, self.self.mTerrainGroup, 2000, 3000,
			TERRAIN_PAGE_MIN_X, TERRAIN_PAGE_MIN_Y, 
			TERRAIN_PAGE_MAX_X, TERRAIN_PAGE_MAX_Y);
#else
		for (long x = TERRAIN_PAGE_MIN_X; x <= TERRAIN_PAGE_MAX_X; ++x)
			for (long y = TERRAIN_PAGE_MIN_Y; y <= TERRAIN_PAGE_MAX_Y; ++y)
				defineTerrain(x, y, blankTerrain);
		# sync load since we want everything in place when we start
		self.mTerrainGroup.loadAllTerrains(True);
#endif

		if (mTerrainsImported)
		{
			TerrainGroup.TerrainIterator ti = self.self.mTerrainGroup.getTerrainIterator();
			while(ti.hasMoreElements())
			{
				Terrain* t = ti.getNext().instance;
				initBlendMaps(t);
			}
		}

		self.mTerrainGroup.freeTemporaryResources();



		# create a few entities on the terrain
		Entity* e = mSceneMgr.createEntity("tudorhouse.mesh");
		Vector3 entPos(mTerrainPos.x + 2043, 0, self.mTerrainPos.z + 1715);
		Quaternion rot;
		entPos.y = self.self.mTerrainGroup.getHeightAtWorldPosition(entPos) + 65.5 + self.mTerrainPos.y;
		rot.FromAngleAxis(Degree(Math.RangeRandom(-180, 180)), Vector3.UNIT_Y);
		SceneNode* sn = mSceneMgr.getRootSceneNode().createChildSceneNode(entPos, rot);
		sn.setScale(Vector3(0.12, 0.12, 0.12));
		sn.attachObject(e);
		mHouseList.push_back(e);

		e = mSceneMgr.createEntity("tudorhouse.mesh");
		entPos = Vector3(mTerrainPos.x + 1850, 0, self.mTerrainPos.z + 1478);
		entPos.y = self.self.mTerrainGroup.getHeightAtWorldPosition(entPos) + 65.5 + self.mTerrainPos.y;
		rot.FromAngleAxis(Degree(Math.RangeRandom(-180, 180)), Vector3.UNIT_Y);
		sn = mSceneMgr.getRootSceneNode().createChildSceneNode(entPos, rot);
		sn.setScale(Vector3(0.12, 0.12, 0.12));
		sn.attachObject(e);
		mHouseList.push_back(e);

		e = mSceneMgr.createEntity("tudorhouse.mesh");
		entPos = Vector3(mTerrainPos.x + 1970, 0, self.mTerrainPos.z + 2180);
		entPos.y = self.self.mTerrainGroup.getHeightAtWorldPosition(entPos) + 65.5 + self.mTerrainPos.y;
		rot.FromAngleAxis(Degree(Math.RangeRandom(-180, 180)), Vector3.UNIT_Y);
		sn = mSceneMgr.getRootSceneNode().createChildSceneNode(entPos, rot);
		sn.setScale(Vector3(0.12, 0.12, 0.12));
		sn.attachObject(e);
		mHouseList.push_back(e);

		mSceneMgr.setSkyBox(True, "Examples/CloudyNoonSkyBox");


	}

	void _shutdown()
	{
		if (mTerrainPaging)
		{
			OGRE_DELETE self.mTerrainPaging;
			OGRE_DELETE self.mPageManager;
		}
		else
			OGRE_DELETE self.self.mTerrainGroup;

		OGRE_DELETE self.mTerrainGlobals;

		SdkSample._shutdown();
	}


};

#endif
